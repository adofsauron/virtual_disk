#include "handle_file_sys.h"

// RAII
CHandleFileSys::CHandleFileSys()
{
    m_b_init_over = false;

    m_p_hdle_catelog        = new CHandleCatelog();
    m_p_hdle_aval_disk      = new CHandleAvalDisk();
    m_p_hdle_disk_mirror    = new CHandleDiskMirror();

    if (NULL == m_p_hdle_disk_mirror)
    {
        return;
    }

    if (NULL == m_p_hdle_catelog)
    {
        return;
    }

    if (NULL == m_p_hdle_aval_disk)
    {
        return;
    }

    if (!InitFileSys())
    {
        LOG_RECORD(LOG_INFO,"初始化文件系统失败");
        return;
    }

    LOG_RECORD(LOG_INFO,"file sys init oover");
    m_b_init_over = true;
}

CHandleFileSys::~CHandleFileSys()
{
    DELETE_PTR(m_p_hdle_catelog);
    DELETE_PTR(m_p_hdle_aval_disk);
    DELETE_PTR(m_p_hdle_disk_mirror);
}

void CHandleFileSys::InitFileSysInfo()
{
    // 总磁盘空间
    m_o_file_sys_info.m_i_disk_total_size = config::CONST_MAX_SIZE_DISK_TOTAL;

    int32 l_i_index = 0; 

    // 可用磁盘信息地址索引
    l_i_index +=  MCRO_STRUCT_SIZE(SFileSysInfo);
    m_o_file_sys_info.m_i_aval_disk_info_index = l_i_index;

    // 目录树信息地址索引
    l_i_index +=  MCRO_STRUCT_SIZE(SAvalDiskInfo);
    m_o_file_sys_info.m_i_catelog_space_index = l_i_index;

    // 目录树节点数量
    m_o_file_sys_info.m_i_cate_node_num = 1; // 初始化时必须要有跟节点
    
    // 可用磁盘信息索引
    m_o_file_sys_info.m_i_aval_disk_space_index = 0 + config::CONST_MAX_SIZE_FILE_SYS; // 从首地址偏移文件系统占用,此后地址为可用磁盘空间

    // 磁盘块大小
    m_o_file_sys_info.m_i_disk_block_size = config::CONST_DISK_BLOCK_SIZE;
}

// 第一次创建文件系统
bool CHandleFileSys::CreateFileSys()
{
    if (NULL == m_p_hdle_disk_mirror)
    {
        return false;
    }

    if (NULL == m_p_hdle_catelog)
    {
        return false;
    }

    if (NULL == m_p_hdle_aval_disk)
    {
        return false;
    }

    byte* l_p_disk_space = m_p_hdle_disk_mirror->GetTotalDiskSpace();
    if (NULL == l_p_disk_space)
    {
        LOG_RECORD(LOG_INFO,"l_p_disk_space err");
        return false;
    }

    m_p_hdle_catelog->InitMapCatalog();
    this->InitFileSysInfo();

    const SFileSysInfo& l_o_file_sys_info = this->m_o_file_sys_info;

    if(! m_p_hdle_disk_mirror->SetDiskMirrorInfo(l_o_file_sys_info.m_i_disk_total_size, l_o_file_sys_info.m_i_disk_block_size))
    {
        LOG_RECORD(LOG_INFO,"SetDiskMirrorInfo err");
        return false;
    }

    LOG_RECORD(LOG_INFO,"SetDiskMirrorInfo over");

    const SAvalDiskInfo& l_o_aval_disk_info = m_p_hdle_aval_disk->InitAvalDiskInfo();
    if(! m_p_hdle_aval_disk->InitData(l_p_disk_space + l_o_file_sys_info.m_i_aval_disk_space_index, l_o_aval_disk_info))
    {
        LOG_RECORD(LOG_INFO,"m_p_hdle_aval_disk init data");
        return false;
    }

    if (! m_p_hdle_aval_disk->InitOver())
    {
        LOG_RECORD(LOG_INFO,"m_p_hdle_aval_disk not init over");
        return false;
    }

    LOG_RECORD(LOG_INFO,"m_p_hdle_aval_disk init over");

    const uint64 l_i_file_sys_size = l_o_file_sys_info.m_i_disk_total_size - l_o_file_sys_info.m_i_aval_disk_space_index;
    uint64 l_i_disk_index = 0;

    if (! CSerialize::PackFileSysInfo(l_p_disk_space + l_i_disk_index, l_i_file_sys_size - l_i_disk_index, l_o_file_sys_info))
    {
        LOG_ERR("PackFileSysInfo err");
        return false;
    }
    l_i_disk_index += MCRO_STRUCT_SIZE(SFileSysInfo);

    LOG_RECORD(LOG_INFO,"PackFileSysInfo over");

    if (! CSerialize::PackAvalDiskInfo(l_p_disk_space + l_i_disk_index, l_i_file_sys_size - l_i_disk_index, m_p_hdle_aval_disk->GetAvalDiskInfo()))
    {
        LOG_ERR("PackAvalDiskInfo err");
        return false;
    }
    l_i_disk_index += MCRO_STRUCT_SIZE(SAvalDiskInfo);

    LOG_RECORD(LOG_INFO,"PackAvalDiskInfo over");

    std::unordered_map<uint32, SCateNode>& l_map_catelog = m_p_hdle_catelog->GetMapCatalog();
    if(! CSerialize::PackCatelog(l_p_disk_space + l_i_disk_index, l_i_file_sys_size - l_i_disk_index, l_map_catelog))
    {
        LOG_ERR("PackCatelog err");
        return false;
    }

    LOG_RECORD(LOG_INFO,"PackCatelog over");

    // 初始化后立即保存磁盘镜像
    if (! m_p_hdle_disk_mirror->SaveDiskMirror())
    {
        LOG_ERR("SaveDiskMirror err");
        return false;
    }

    LOG_INFO("SaveDiskMirror over");
    return true;
}

// 加载文件系统
bool CHandleFileSys::LoadFileSys()
{
    if (NULL == m_p_hdle_disk_mirror)
    {
        return false;
    }

    if (NULL == m_p_hdle_catelog)
    {
        return false;
    }

    if (NULL == m_p_hdle_aval_disk)
    {
        return false;
    }

    byte* l_p_disk_space = m_p_hdle_disk_mirror->GetTotalDiskSpace();
    if (NULL == l_p_disk_space)
    {
        return false;
    }

    m_p_hdle_disk_mirror->SetDiskMirrorInfo(config::CONST_MAX_SIZE_DISK_TOTAL, config::CONST_DISK_BLOCK_SIZE);

    if (! m_p_hdle_disk_mirror->LoadDiskMirror())
    {
        LOG_RECORD(LOG_ERR, "LoadDiskMirror err");
        return false;
    }

    SFileSysInfo& l_o_file_sys_info= this->m_o_file_sys_info;

    uint64 l_i_disk_index = 0;
    if (!CSerialize::UnPackFileSysInfo(l_p_disk_space + l_i_disk_index, MCRO_STRUCT_SIZE(SFileSysInfo), l_o_file_sys_info))
    {
        LOG_RECORD(LOG_ERR, "UnPackFileSysInfo err");
        return false;
    }
    l_i_disk_index += MCRO_STRUCT_SIZE(SFileSysInfo);

    // TODO: 注意: 严格来说,为了保证磁盘信息的唯一性,解析除数据后,不应再与config数据比较,两者独立，目前是为了数据的安全, 不可在改动配置后不重新格式化磁盘
    // 当改变config配置后,若配置数据缩小,但是文件系统没有格式化,将会导致启动失败

    // 解析出的数据错误, TODO: 只有数据越界的时候才能检测出,如果里边是脏数据,就检测不出了
    if ((0 >= l_o_file_sys_info.m_i_disk_total_size) || (config::CONST_MAX_SIZE_DISK_TOTAL < l_o_file_sys_info.m_i_disk_total_size))
    {
        char buff[2048] = {0x00};
        snprintf(buff, 2048, "disk size check err:%llu", l_o_file_sys_info.m_i_disk_total_size);
        std::string log = buff;
        LOG_RECORD(LOG_ERR, log);
        return false;
    }

    if (0 >= l_o_file_sys_info.m_i_cate_node_num) // 至少要存在一个跟节点，不可能等于0
    {
        LOG_RECORD(LOG_ERR, "disk num err");
        return false;
    }

    const uint64 l_i_file_sys_size = l_o_file_sys_info.m_i_disk_total_size - l_o_file_sys_info.m_i_aval_disk_space_index;
    
    SAvalDiskInfo& l_o_aval_disk_info =  m_p_hdle_aval_disk->GetAvalDiskInfo();
    if (!CSerialize::UnPackAvalDiskInfo(l_p_disk_space + l_i_disk_index, (l_i_file_sys_size - l_i_disk_index), l_o_aval_disk_info))
    {
        LOG_RECORD(LOG_ERR, "UnPackAvalDiskInfo err");
        return false;
    }

    if(! m_p_hdle_aval_disk->InitData(l_p_disk_space + l_o_file_sys_info.m_i_aval_disk_space_index, l_o_aval_disk_info))
    {
        LOG_RECORD(LOG_ERR, "m_p_hdle_aval_disk init data err");
        return false;
    }

    if (! m_p_hdle_aval_disk->InitOver())
    {
        LOG_RECORD(LOG_ERR, "m_p_hdle_aval_disk init data err");
        return false;
    }

    if ((0 >= l_o_aval_disk_info.m_i_aval_disk_size) || ((config::CONST_MAX_SIZE_DISK_TOTAL - config::CONST_MAX_SIZE_FILE_SYS) < l_o_aval_disk_info.m_i_aval_disk_size))
    {
        LOG_RECORD(LOG_ERR, "disk aval size check err");
        return false;
    }

    l_i_disk_index += 0 + MCRO_STRUCT_SIZE(SAvalDiskInfo);

    std::unordered_map<uint32, SCateNode>& l_map_catelog = m_p_hdle_catelog->GetMapCatalog();

    if (! CSerialize::UnPackCatelog(l_p_disk_space + l_i_disk_index, (l_i_file_sys_size - l_i_disk_index), l_o_file_sys_info.m_i_cate_node_num, l_map_catelog))
    {
        LOG_RECORD(LOG_ERR, "UnPackCatelog err");
        return false;
    }

    return true;
}

bool CHandleFileSys::InitFileSys()
{
    if (NULL == m_p_hdle_disk_mirror)
    {
        return false;
    }

    if (NULL == m_p_hdle_catelog)
    {
        return false;
    }

    if (NULL == m_p_hdle_aval_disk)
    {
        return false;
    }

    if (!m_p_hdle_disk_mirror->CheckDiskMirrorExist()) // 磁盘镜像不存在,则初始化文件系统
    {
        if (!CreateFileSys())
        {
            LOG_RECORD(LOG_ERR,"CreateFileSys err");
            return false;
        }

        LOG_RECORD(LOG_INFO,"CreateFileSys over");
    }
    else // 镜像存在,则加载
    {
        if (!LoadFileSys())
        {
            LOG_RECORD(LOG_ERR,"LoadFileSys err");
            return false;
        }

        LOG_RECORD(LOG_INFO,"LoadFileSys over");
    }

    // 初始化路径_>id映射关系
    if (!m_p_hdle_catelog->InitCacheFullName2Id())
    {
        LOG_RECORD(LOG_ERR,"InitCacheFullName2Id err");
        return false;
    }

    LOG_RECORD(LOG_INFO,"InitCacheFullName2Id over");

    return true;
}

bool CHandleFileSys::ReloadFileSys()
{
    if (NULL == m_p_hdle_disk_mirror)
    {
        return false;
    }

    if (NULL == m_p_hdle_catelog)
    {
        return false;
    }

    if (NULL == m_p_hdle_aval_disk)
    {
        return false;
    }
 
    if (!m_p_hdle_disk_mirror->CheckDiskMirrorExist()) // 磁盘镜像不存在,不可加载
    {
        return false;
    }

    // 清理内存数据
    m_p_hdle_disk_mirror->ClearData();
    m_p_hdle_catelog->ClearData();
    m_p_hdle_aval_disk->ClearData();

    // TODO: 若此时加载失败,但是相关内存数据已经被清理,那文件系统就不可用了。由上层决定是报错还是重新创建
    if (!LoadFileSys())
    {
        return false;
    }

    // 初始化路径_>id映射关系
    if (!m_p_hdle_catelog->InitCacheFullName2Id())
    {
        return false;
    }

    return true;
}

// 路径是否存在,若存在返回节点属性
bool CHandleFileSys::CheckPathExist(const std::string& a_str_path, SCateNode*& a_p_cata_node)
{
    if (NULL == m_p_hdle_catelog)
    {
        return false;
    }

    std::unordered_map<uint32, SCateNode>& l_map_catelog = m_p_hdle_catelog->GetMapCatalog();
    std::unordered_map<uint32, SCateNode>::iterator l_iter = l_map_catelog.begin();

    for (; l_iter != l_map_catelog.end(); ++ l_iter)
    {
        SCateNode* l_p_node = &l_iter->second;
        const std::string l_str_full_name = l_p_node->m_p_full_name;

        if ( a_str_path.length() != l_str_full_name.length())
        {
            continue;
        }

        if (a_str_path == l_str_full_name)
        {
            // std::string log;
            // log = "same:";
            // log += l_p_node->m_p_full_name;
            // log += ", cmp:";
            // log += a_str_path;
            // LOG_DEBUG(log)
            a_p_cata_node = l_p_node;
            return true;
        }
    }

    return false;
}

bool CHandleFileSys::GetCateNodeById(const uint32 a_i_id, SCateNode*& a_p_cata_node)
{
    if (NULL == m_p_hdle_catelog)
    {
        return false;
    }

    return m_p_hdle_catelog->GetCateNode(a_i_id, a_p_cata_node);
}

bool CHandleFileSys::GetFullPath(const std::string& a_str_give_path, std::string& a_str_full_path)
{
    if (a_str_give_path.empty())
    {
        LOG_RECORD(LOG_ERR, "a_str_give_path.empty()");
        return false;
    }

    std::string l_str_path = a_str_give_path;

    CDealString::PathConver((char*)l_str_path.c_str(), l_str_path.length());

    const char l_chr = l_str_path[0];
    if (('.' != l_chr) && ('/' != l_chr))
    {
        LOG_ERR("l_str_path[0] err");
        return false;
    }

    // 先找出当前的绝对路径
    std::string l_str_cur_path;
    if ('/' == l_chr)
    {
        l_str_cur_path = "/";
    }
    else
    {
        CEnv::Instance()->GetPwd(l_str_cur_path);
    }

    std::vector<std::string> l_vec_cur_path;
    CDealString::SplitString(l_str_cur_path, "/", l_vec_cur_path);

    std::vector<std::string> l_vec_node_name;
    CDealString::SplitString(l_str_path, "/", l_vec_node_name);

    for (size_t i=0; i<l_vec_node_name.size(); ++i)
    {
        const std::string& l_str_node_name = l_vec_node_name[i];

        // 当前节点
        if (( 1 == l_str_node_name.length()) && ("." == l_str_node_name))
        {
            continue;
        }

        // 父节点
        if ((2 == l_str_node_name.length()) && (".." == l_str_node_name))
        {
            if (!l_vec_cur_path.empty())
            {
                l_vec_cur_path.pop_back();
            }

            continue;
        }

        l_vec_cur_path.push_back(l_str_node_name);
    }

    a_str_full_path.clear();

    if (l_vec_cur_path.empty())
    {
        a_str_full_path = "/";
    }
    else
    {
        for (size_t i=0; i<l_vec_cur_path.size(); ++i)
        {
            a_str_full_path += "/";
            a_str_full_path += l_vec_cur_path[i];
        }

         CDealString::CombineChar((char*)a_str_full_path.c_str(), a_str_full_path.length(),'/');
    }
    

    return true;
}

bool CHandleFileSys::GetOnlyNameByFullName(const std::string& a_str_full_path, std::string& a_str_only_name)
{
    if (a_str_full_path.empty())
    {
        return false;
    }

    std::string l_str_path = a_str_full_path;
    std::vector<std::string> l_vec_cur_path;
    CDealString::SplitString(l_str_path, "/", l_vec_cur_path);

    a_str_only_name = l_vec_cur_path.empty() ? "/" : l_vec_cur_path.back();

    return true;
}

// 根据绝对路径找到父路径
bool CHandleFileSys::GetFatherNameByFullName(const std::string& a_str_full_path, std::string& a_str_father_name)
{
    std::string l_str_path = a_str_full_path;
    std::vector<std::string> l_vec_cur_path;
    CDealString::SplitString(l_str_path, "/", l_vec_cur_path);

    if (l_vec_cur_path.empty()) // 没父类, 跟节点
    {
        return false;
    }

    l_vec_cur_path.pop_back();

    a_str_father_name.clear();

    if (l_vec_cur_path.empty())
    {
         a_str_father_name = "/";
    }
    else
    {
        for (size_t i=0; i < l_vec_cur_path.size(); ++i)
        {
            a_str_father_name += "/";
            a_str_father_name += l_vec_cur_path[i];
            
        }

        CDealString::CombineChar((char*)a_str_father_name.c_str(), a_str_father_name.length(), '/');
    }
    
    return true;
}

// 以下创建文件操作,在时序上，存在逻辑上的先后关系
// 1. 父节点属性检测
// 2. 父节点下的子节点不可重名
// 3. 创建新的节点，设置相关属性
// 4. 向父节点的子节点列表插入本节点

// 若3成功,4失败,则必须释放已创建的节点,否则将破坏目录树结构

// 该处理错误情况非常多,一种是将返回值改为int，上层根据返回值决定策略；一种是本函数内记录详细日志
// 真实情况是，返回int，但上层就记录下int值, 没什么策略。所以，选第二种方法，本函数详细记录日志

bool CHandleFileSys::AddNewFile(const std::string& a_str_full_name, const ENUM_CNODE_TYPE a_i_type, const uint8 a_i_access, SCateNode*& a_p_node)
{
    a_p_node = NULL; // 只有执行流程到最后时，才设置此值

    // 参数合法性检测
    if ( ((ACCESS_EXECUTE | ACCESS_WRITE | ACCESS_RREAD)) < a_i_access) // 权限非法
    {
        LOG_ERR("file access err");
        return false;
    }

    if (CNODE_INVIAL == a_i_type) // 类型非法
    {
        LOG_ERR("file type err");
        return false;
    }

    if ("/" == a_str_full_name) // 根路径不能创建
    { 
        LOG_ERR("file can not / err");
        return false;
    }

    if (a_str_full_name.length() > config::CONST_MAX_SIZE_FILE_FULL_NAME) // 总文件名过长
    {
        LOG_ERR("file name langth err");
        return false;
    }

    if (NULL == m_p_hdle_catelog)
    {
        LOG_ERR("m_p_hdle_catelog null");
        return false;
    }

    SCateNode* l_p_node;
    if (CheckPathExist(a_str_full_name, l_p_node)) // 路径已存在
    {
        LOG_ERR("a_str_full_name exist");
        return false;
    }

    std::string l_str_father_full_name;
    if (! GetFatherNameByFullName(a_str_full_name, l_str_father_full_name))
    {
        LOG_ERR("not get father name");
        return false;
    }

    LOG_DEBUG(l_str_father_full_name);
    if (! CheckPathExist(l_str_father_full_name, l_p_node)) // 父路径不存在
    {
        LOG_ERR(l_str_father_full_name);
        LOG_ERR("father is not exist");
        return false;
    }

    std::cout << "l_str_father_full_name::"<<l_str_father_full_name<<std::endl;

    std::string l_str_only_name;
    if (! GetOnlyNameByFullName(a_str_full_name, l_str_only_name))
    {
        LOG_ERR("get only name err");
        return false;
    }

    if (l_str_only_name.length() > config::CONST_MAX_SIZE_FILE_NAME) // 文件名过长
    {
        LOG_ERR("file name langth err");
        return false;
    }

    if ((l_p_node->m_i_son_num > 0) && (NULL == l_p_node->m_p_son_set)) // TODO: 父节点非法,这时候出现了脏数据,目前先不处理,第二版专门处理
    {
        LOG_ERR("father son set err");
        return false;
    }

    for (uint32 i=0; i<l_p_node->m_i_son_num;++i)
    {
        uint32 l_i_sid = l_p_node->m_p_son_set[i];
        SCateNode* l_p_snode = NULL;
        if (! m_p_hdle_catelog->GetCateNode(l_i_sid, l_p_snode))
        {
            //LOG_ERR("GetCateNode err");
            LOG_DEBUG("NOT GetCateNode");
            continue;
        }

        if (NULL == l_p_snode)
        {
            LOG_ERR("GetCateNode err");
            return false;
        }

        // 文件名重复
        if (l_str_only_name == l_p_snode->m_p_name)
        {
            LOG_ERR("same name err");
            return false;
        }
    }

    uint32 l_i_father_id = l_p_node->m_i_id;

    std::cout << "father id:" << l_i_father_id << std::endl;

    // 新增加节点
    uint32 l_i_id = 0;
    if(! m_p_hdle_catelog->AddCateNode(l_i_id))
    {
        LOG_ERR("AddCateNode err");
        return false;
    }

    // 获取新添加的节点的地址
    SCateNode* l_p_new_node = NULL;
    if (! m_p_hdle_catelog->GetCateNode(l_i_id, l_p_new_node))
    {
        LOG_ERR("GetCateNode err");   
        return false;
    }

    if (NULL == l_p_new_node)
    {
        return false;
    }

    // 设置父节点
    l_p_new_node->m_i_parent_id = l_i_father_id;

    // 设置名字
    memcpy(l_p_new_node->m_p_full_name, a_str_full_name.c_str(), a_str_full_name.length());
    memcpy(l_p_new_node->m_p_name, l_str_only_name.c_str(), l_str_only_name.length());

    // 设置节点类型
    l_p_new_node->m_i_type = a_i_type;

    // 设置访问权限
    l_p_new_node->m_i_access = a_i_access;

    // 创建时间
    l_p_new_node->m_i_create_time = CDate::GetUnixTime();
    l_p_new_node->m_i_update_time = CDate::GetUnixTime();

    // 设置文件系统的文件名->id的缓存
    if (! m_p_hdle_catelog->InserCacheFullName2Id(a_str_full_name, l_i_id)) // 插入失败就把原来的节点删除掉
    {
        if (! m_p_hdle_catelog->DelCateNode(l_i_id))
        {
            LOG_ERR("InserCacheFullName2Id err"); 
            return false;
        }

        LOG_ERR("InserCacheFullName2Id err");   
        return false;
    }

    // 向父节点的子节点容器加入本节点, 若失败，必须删除本节点
    if(! m_p_hdle_catelog->FatherAddSonId(l_i_father_id, l_i_id))
    {
        if (!m_p_hdle_catelog->DelCateNode(l_i_id)) // 清理错误数据的时候还失败了，TODO: 第二版把这一块细化, 目前先报错
        {
             LOG_ERR("FatherAddSonId err"); 
            return false;
        }

        LOG_ERR("FatherAddSonId err"); 
        return false;
    }

    if (! ReBuildCatelogCache())
    {
        LOG_ERR("ReBuildCatelogCache err"); 
        return false;
    }

    // 此时才算执行完成
    a_p_node = l_p_new_node;
    return true;
}

bool CHandleFileSys::GetAvalSpace(uint64& l_i_aval_space)
{
    if (NULL == m_p_hdle_aval_disk)
    {
        return false;
    }

    return m_p_hdle_aval_disk->GetAvalSpace(l_i_aval_space);
}

bool CHandleFileSys::DelFile(const std::string& a_str_full_name)
{
    if (a_str_full_name.empty())
    {
		LOG_ERR("");
        return false;
    }

    // 不可删除跟节点
    if ("/" == a_str_full_name)
    {
		LOG_ERR("");
        return false;
    }

    SCateNode* l_p_node;
    if (!CheckPathExist(a_str_full_name, l_p_node)) // 文件不存在
    {
		LOG_ERR("");
        return false;
    }

    // 1. 清理占用磁盘空间
    if (l_p_node->m_i_file_size > 0)
    {
        if (NULL == m_p_hdle_aval_disk)
        {
			LOG_ERR("");
            return false;
        }

        // 只有可用空间 最后边一块占用内存被删除，空间才能被继续使用，中间的空间，依然不可用
        if (! m_p_hdle_aval_disk->ReleaseSpace(l_p_node->m_i_disk_index, l_p_node->m_i_file_size))
        {
			LOG_ERR("");
            return false;
        }
    }

    if (NULL == m_p_hdle_catelog)
    {
		LOG_ERR("");
        return false;
    }

    // 2. 释放所有的子节点空间和缓存,注意此次操作,只可处理子树
    if ((l_p_node->m_i_son_num > 0) && (NULL == l_p_node->m_p_son_set))
    {
		LOG_ERR("");
        return false;
    }

    // 递归版本, 注意DelFile, 目的是清理掉树形结构的每一节点
    for (uint32 i=0; i<l_p_node->m_i_son_num; ++i)
    {
        uint64 l_i_sid = l_p_node->m_p_son_set[i];
        SCateNode* l_p_snode = NULL;
        if (!m_p_hdle_catelog->GetCateNode(l_i_sid, l_p_snode))
        {
			LOG_ERR("");
            return false;
        }

        if (NULL == l_p_snode)
        {
			LOG_ERR("");
            return false;
        }

        // 深度优先,从最底层开始删除
        if (!DelFile(l_p_snode->m_p_full_name))
        {
			LOG_ERR("");
            return false;
        }
    }

    // 3. 删除父节点中保存的子节点信息
    if (!m_p_hdle_catelog->FatherDelSonId(l_p_node->m_i_parent_id, l_p_node->m_i_id))
    {
		LOG_ERR("");
        return false;
    }

    // 4. 清理目录树节点
    if (!m_p_hdle_catelog->DelCateNode(l_p_node->m_i_id))
    {
		LOG_ERR("");
        return false;
    }

	ReBuildCatelogCache();

    return true;
}

bool CHandleFileSys::ApplayAvalSpace(const uint64 a_i_size, byte*& a_p_aval_space, uint64& a_i_incex)
{
    uint64 l_i_aval_size = 0;
    if (! GetAvalSpace(l_i_aval_size))
    {
        LOG_ERR("");
        return false;
    }

    if (a_i_size > l_i_aval_size)
    {
        LOG_ERR("");
        return false;
    }

    if (NULL == m_p_hdle_aval_disk)
    {
        LOG_ERR("");
        return false;
    }

    byte* l_p_aval_disk_ptr = m_p_hdle_aval_disk->GetAvalSpacePtr();

    if (NULL == l_p_aval_disk_ptr)
    {
        LOG_ERR("");
        return false;
    }

    uint64 l_i_aval_index = 0;
    if (! m_p_hdle_aval_disk->ApplaySpace(a_i_size,a_i_incex))
    {
        LOG_ERR("");
        return false;
    }

    // 申请成功后才能赋值
    a_p_aval_space = l_p_aval_disk_ptr + l_i_aval_index;
    a_i_incex = l_i_aval_index;

    return true;
}

bool CHandleFileSys::GetAvalSpaceByIndex(const uint64 a_i_incex, byte*& a_p_space)
{
    uint64 l_i_aval_size = 0;
    if (! GetAvalSpace(l_i_aval_size))
    {
        return false;
    }

    if (a_i_incex > (l_i_aval_size-1)) // 非法索引
    {
        return false;
    }

    if (NULL == m_p_hdle_aval_disk)
    {
        return false;
    }

    byte* l_p_aval_disk_ptr = m_p_hdle_aval_disk->GetAvalSpacePtr();

    if (NULL == l_p_aval_disk_ptr)
    {
        return false;
    }

    a_p_space = l_p_aval_disk_ptr + a_i_incex;

    return true;
}

// 输出目录下文件, 不包含子目录
bool CHandleFileSys::CollectDirBrief(const std::string& a_str_full_name, std::vector<uint32>& a_vec_son_name)
{
    SCateNode* l_p_node;
    if (! CheckPathExist(a_str_full_name, l_p_node)) // 路径不存在
    {
        LOG_ERR("CheckPathExist err");
        LOG_ERR(a_str_full_name);
        return false;
    }

    if ((l_p_node->m_i_son_num > 0) && (NULL == l_p_node->m_p_son_set)) // 子节点非法
    {
        LOG_ERR("m_i_son_num set err");
        return false;
    }

    if (CNODE_DIR != l_p_node->m_i_type) // 叶子节点
    {
        return true;
    }

    a_vec_son_name.clear();
    // 递归版本, 注意DelFile, 目的是清理掉树形结构的每一节点
    for (uint32 i=0; i<l_p_node->m_i_son_num; ++i)
    {
        uint32 l_i_sid = l_p_node->m_p_son_set[i];
        SCateNode* l_p_snode = NULL;
        if (!m_p_hdle_catelog->GetCateNode(l_i_sid, l_p_snode))
        {
            continue;
        }

        if (NULL == l_p_snode)
        {
            continue;
        }

        a_vec_son_name.push_back(l_i_sid);
    }

    return true;
}

// 输出目录下文件，含子目录
bool CHandleFileSys::CollectDirTotal(const std::string& a_str_full_name, std::unordered_map<std::string, std::vector<uint32> >& a_map_son)
{
    SCateNode* l_p_node;
    if (! CheckPathExist(a_str_full_name, l_p_node)) // 路径不存在
    {
        LOG_ERR("CheckPathExist err");
        std::cout << a_str_full_name << std::endl;
        return false;
    }

    if ((l_p_node->m_i_son_num > 0) && (NULL == l_p_node->m_p_son_set)) // 子节点非法
    {
        LOG_ERR("m_i_son_num set err");
        return false;
    }

    if (CNODE_DIR != l_p_node->m_i_type) // 叶子节点
    {
        LOG_INFO("m_i_type leaf");
        return true;
    }

    if ((l_p_node->m_i_son_num <= 0)) // 无子节点
    {
        return true;
    }

    const std::string l_str_full_name = l_p_node->m_p_full_name;
    std::vector<uint32> l_vec_value;
    
    if (! CollectDirBrief(l_str_full_name, l_vec_value))
    {
        LOG_ERR("CollectDirBrief err");
        return false;
    }

    if (!l_vec_value.empty())
    {
        a_map_son.insert( std::make_pair(l_str_full_name, l_vec_value));
    }

    
    for (uint32 i=0; i<l_p_node->m_i_son_num; ++i)
    {
        uint64 l_i_sid = l_p_node->m_p_son_set[i];
        SCateNode* l_p_snode = NULL;
        if (!m_p_hdle_catelog->GetCateNode(l_i_sid, l_p_snode))
        {
            continue;
        }

        if (NULL == l_p_snode)
        {
            continue;
        }

        if (CNODE_DIR != l_p_snode->m_i_type) // 叶子节点
        {
			continue;
        }

        CollectDirTotal(l_p_snode->m_p_full_name, a_map_son);
    }

    return true;
}

bool CHandleFileSys::PrintNode(const SCateNode* a_p_node, std::string& a_str_info)
{
    char l_p_buff[2048] = {0x00};
    if (CNODE_INVIAL == a_p_node->m_i_type)
    {
        snprintf(l_p_buff, 2048, "node invail: nid[%d], name:[%s]", a_p_node->m_i_id, a_p_node->m_p_name);
        a_str_info = l_p_buff;
        return false;
    }

    const char* l_chr_type = NULL;

    switch (a_p_node->m_i_type)
    {
    case CNODE_INVIAL:
        l_chr_type = "&"; // 非法类型
        break;

    case CNODE_DIR:
        l_chr_type = "<DIR>";
        break;

    case CNODE_FILE:
        l_chr_type = "<FIL>";
        break;

    case CNODE_LINK:
        l_chr_type = "<LNK>";
        break;
    
    default:
        l_chr_type = "<ERR>"; // 非法类型
        break;
    }

    const std::string& l_str_update_time = CDate::GetCTimeByUnixTime(a_p_node->m_i_update_time);

    std::string l_str_acess;
    l_str_acess = (a_p_node->m_i_access >> 2 & 1) == 1 ? "r" : "-";
    l_str_acess += (a_p_node->m_i_access >> 1 & 1) == 1 ? "w" : "-";
    l_str_acess += (a_p_node->m_i_access & 1) == 1 ? "x" : "-";

    // 模仿linux输出格式
    memset(l_p_buff, 0x00, sizeof(l_p_buff));
    snprintf(l_p_buff, 2048, "%4s %s\t%8d bytes\t%s\t%10s", 
        l_chr_type, l_str_acess.c_str(), a_p_node->m_i_file_size, l_str_update_time.c_str(), a_p_node->m_p_name);


    a_str_info += l_p_buff;
    a_str_info += "\t";
    a_str_info += a_p_node->m_p_full_name;

    if (CNODE_LINK == a_p_node->m_i_type)
    {
        uint32 l_i_link_id = a_p_node->m_i_link_id;
        SCateNode* l_p_node = NULL;
        if (!m_p_hdle_catelog->GetCateNode(l_i_link_id, l_p_node))
        {
            a_str_info += "------------>链接文件已失效!";
            return true;
        }

        if (NULL == l_p_node)
        {
            a_str_info += "------------>链接文件已失效!";
            return true;
        }

        a_str_info += "------------>[";
        a_str_info += l_p_node->m_p_full_name;
        a_str_info += "]";
    }

    return true;
}

bool CHandleFileSys::PrintNode(const uint32 l_i_node, std::string& a_str_info)
{
    SCateNode* a_p_cata_node;
    if (!m_p_hdle_catelog->GetCateNode(l_i_node, a_p_cata_node))
    {
        return false;
    }

    return PrintNode(a_p_cata_node, a_str_info);
}

// 改变node归属
bool CHandleFileSys::MoveNode(const uint32 l_i_id, const uint32 l_i_old_fid, const uint32 l_i_new_fid)
{
    if (!m_p_hdle_catelog->FatherDelSonId(l_i_old_fid, l_i_id))
    {
        LOG_ERR("FatherDelSonId err");
        return false;
    }

    if (!m_p_hdle_catelog->FatherAddSonId(l_i_new_fid, l_i_id))
    {
        LOG_ERR("FatherAddSonId err");
        return false;
    }

    SCateNode* l_p_node = NULL;
    if (!m_p_hdle_catelog->GetCateNode(l_i_id, l_p_node))
    {
        LOG_ERR("GetCateNode err");
        return false;
    }

    if (NULL == l_p_node)
    {
        LOG_ERR("l_p_node null");
        return false;
    }

    l_p_node->m_i_parent_id = l_i_new_fid;

    return true;
}

bool CHandleFileSys::ReBuildCatelogCache()
{
    if (NULL == m_p_hdle_catelog)
    {
        return false;
    }

    return m_p_hdle_catelog->InitCacheFullName2Id();
}

bool CHandleFileSys::SaveFileSys()
{
    if (NULL == m_p_hdle_disk_mirror)
    {
        return false;
    }

    if (NULL == m_p_hdle_catelog)
    {
        return false;
    }

    if (NULL == m_p_hdle_aval_disk)
    {
        return false;
    }

    byte* l_p_disk_space = m_p_hdle_disk_mirror->GetTotalDiskSpace();
    if (NULL == l_p_disk_space)
    {
        return false;
    }

    LOG_INFO("ready save file sys");

    m_o_file_sys_info.m_i_cate_node_num = m_p_hdle_catelog->GetMapCataNodeNum();

    const uint64 l_i_file_sys_size = m_o_file_sys_info.m_i_disk_total_size - m_o_file_sys_info.m_i_aval_disk_space_index;

    uint64 l_i_disk_index = 0;

    LOG_INFO("ready PackFileSysInfo");

    if (! CSerialize::PackFileSysInfo(l_p_disk_space + l_i_disk_index, l_i_file_sys_size - l_i_disk_index, m_o_file_sys_info))
    {
        LOG_RECORD(LOG_ERR, "PackFileSysInfo err");
        return false;
    }
    l_i_disk_index += MCRO_STRUCT_SIZE(SFileSysInfo);

    LOG_INFO("PackFileSysInfo over");

    if (! CSerialize::PackAvalDiskInfo(l_p_disk_space + l_i_disk_index, l_i_file_sys_size - l_i_disk_index, m_p_hdle_aval_disk->GetAvalDiskInfo()))
    {
        LOG_RECORD(LOG_ERR, "PackAvalDiskInfo err");
        return false;
    }
    l_i_disk_index += MCRO_STRUCT_SIZE(SAvalDiskInfo);

    LOG_INFO("PackAvalDiskInfo over");

    if(! CSerialize::PackCatelog(l_p_disk_space + l_i_disk_index, l_i_file_sys_size - l_i_disk_index, m_p_hdle_catelog->GetMapCatalog()))
    {
        LOG_RECORD(LOG_ERR, "PackCatelog err")
        return false;
    }

    LOG_INFO("PackCatelog over");

    // 保存磁盘镜像
    if (! m_p_hdle_disk_mirror->SaveDiskMirror())
    {
        LOG_RECORD(LOG_ERR, "SaveDiskMirror err");
        return false;
    }

    LOG_RECORD(LOG_INFO, "SaveFileSys ok");
    return true;
}


bool CHandleFileSys::MotifySonFullName(const uint32 l_i_fid)
{
	SCateNode* l_p_src_cate_node = NULL;
	if (! GetCateNodeById(l_i_fid, l_p_src_cate_node))
	{
		return false;
	}

	// 修改子文件的绝对名
	if (l_p_src_cate_node->m_i_son_num > 0)
	{
		for (uint32 i = 0; i < l_p_src_cate_node->m_i_son_num; ++i)
		{
			const uint32 l_i_sid = l_p_src_cate_node->m_p_son_set[i];
			SCateNode* l_p_node = NULL;
			if (! GetCateNodeById(l_i_sid, l_p_node))
			{
				continue;
			}

			std::string l_str_new_full_name = l_p_src_cate_node->m_p_full_name;
			l_str_new_full_name += "/";
			l_str_new_full_name += l_p_node->m_p_name;
			memset(l_p_node->m_p_full_name, 0x00, config::CONST_MAX_SIZE_FILE_FULL_NAME + 1);
			memcpy(l_p_node->m_p_full_name, l_str_new_full_name.c_str(), config::CONST_MAX_SIZE_FILE_FULL_NAME + 1);

			if (CNODE_DIR == l_p_node->m_i_type)
			{
				MotifySonFullName(l_i_sid);
			}

		}
	}

	return true;
}
