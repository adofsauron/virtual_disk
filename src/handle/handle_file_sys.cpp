#include "handle_file_sys.h"

// RAII
CHandleFileSys::CHandleFileSys()
{
    m_b_init_over = false;

    m_p_hdle_disk_mirror    = new CHandleDiskMirror();
    m_p_hdle_catelog        = new CHandleCatelog();
    m_p_hdle_aval_disk      = NULL; // 构造时必须给定可用磁盘地址,先占位

    if (!InitFileSys())
    {
        return;
    }

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
    m_o_file_sys_info.m_i_catelog_space_index = 0 + config::CONST_MAX_SIZE_FILE_SYS; // 从首地址偏移文件系统占用,此后地址为可用磁盘空间

    // 磁盘块
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

    if (NULL != m_p_hdle_aval_disk)
    {
        return false;
    }

    byte* l_p_disk_space = m_p_hdle_disk_mirror->GetTotalDiskSpace();
    if (NULL == l_p_disk_space)
    {
        return false;
    }

    m_p_hdle_catelog->InitMapCatalog();
    this->InitFileSysInfo();

    const SFileSysInfo& l_o_file_sys_info = this->m_o_file_sys_info;

    if(m_p_hdle_disk_mirror->SetDiskMirrorInfo(l_o_file_sys_info.m_i_disk_total_size, l_o_file_sys_info.m_i_disk_block_size))
    {
        return false;
    }

    m_p_hdle_aval_disk = new CHandleAvalDisk(l_p_disk_space + l_o_file_sys_info.m_i_aval_disk_space_index);
        if (NULL == m_p_hdle_aval_disk)
    {
        return false;
    }

    if (!m_p_hdle_aval_disk->InitOver())
    {
        return false;
    }

    if (!m_p_hdle_aval_disk->InitOver)
    {
        return false;
    }

    m_p_hdle_aval_disk->InitAvalDiskInfo();

    const uint64 l_i_file_sys_size = l_o_file_sys_info.m_i_disk_total_size - l_o_file_sys_info.m_i_aval_disk_space_index;
    uint64 l_i_disk_index = 0;

    if (!CSerialize::PackFileSysInfo(l_p_disk_space + l_i_disk_index, l_i_file_sys_size - l_i_disk_index, l_o_file_sys_info))
    {
        return false;
    }
    l_i_disk_index += MCRO_STRUCT_SIZE(SFileSysInfo);

    if (CSerialize::PackAvalDiskInfo(l_p_disk_space + l_i_disk_index, l_i_file_sys_size - l_i_disk_index, m_p_hdle_aval_disk->GetAvalDiskInfo()))
    {
        return false;
    }
    l_i_disk_index += MCRO_STRUCT_SIZE(SAvalDiskInfo);

    if(!CSerialize::PackCatelog(l_p_disk_space + l_i_disk_index, l_i_file_sys_size - l_i_disk_index, m_p_hdle_catelog->GetMapCatalog()))
    {
        return false;
    }

    // 初始化后立即保存磁盘镜像
    if (!m_p_hdle_disk_mirror->SaveDiskMirror())
    {
        return false;
    }

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

    if (NULL != m_p_hdle_aval_disk)
    {
        return false;
    }

    byte* l_p_disk_space = m_p_hdle_disk_mirror->GetTotalDiskSpace();
    if (NULL == l_p_disk_space)
    {
        return false;
    }


     if (!m_p_hdle_disk_mirror->LoadDiskMirror())
    {
        return false;
    }

    SFileSysInfo& l_o_file_sys_info = this->m_o_file_sys_info;

    uint64 l_i_disk_index = 0;
    if (!CSerialize::UnPackFileSysInfo(l_p_disk_space + l_i_disk_index, MCRO_STRUCT_SIZE(SFileSysInfo), l_o_file_sys_info))
    {
        return false;
    }
    l_i_disk_index += 0 + MCRO_STRUCT_SIZE(SFileSysInfo);

    // TODO: 注意: 严格来说,为了保证磁盘信息的唯一性,解析除数据后,不应再与config数据比较,两者独立，目前是为了数据的安全, 不可在改动配置后不重新格式化磁盘
    // 当改变config配置后,若配置数据缩小,但是文件系统没有格式化,将会导致启动失败

    // 解析出的数据错误, TODO: 只有数据越界的时候才能检测出,如果里边是脏数据,就检测不出了
    if ((0 >= l_o_file_sys_info.m_i_disk_total_size) || (config::CONST_MAX_SIZE_DISK_TOTAL < l_o_file_sys_info.m_i_disk_total_size))
    {
        return false;
    }

    if (0 >= l_o_file_sys_info.m_i_cate_node_num) // 至少要存在一个跟节点，不可能等于0
    {
        return false;
    }

    m_p_hdle_aval_disk = new CHandleAvalDisk(l_p_disk_space + l_o_file_sys_info.m_i_aval_disk_space_index);
    if (NULL == m_p_hdle_aval_disk)
    {
        return false;
    }

    const uint64 l_i_file_sys_size = l_o_file_sys_info.m_i_disk_total_size - l_o_file_sys_info.m_i_aval_disk_space_index;
    
    SAvalDiskInfo& l_o_aval_disk_info =  m_p_hdle_aval_disk->GetAvalDiskInfo();
    if (!CSerialize::UnPackAvalDiskInfo(l_p_disk_space + l_i_disk_index, (l_i_file_sys_size - l_i_disk_index), l_o_aval_disk_info))
    {
        return false;
    }

    if ((0 >= l_o_aval_disk_info.m_i_aval_disk_size) || ((config::CONST_MAX_SIZE_DISK_TOTAL - config::CONST_MAX_SIZE_FILE_SYS) < l_o_aval_disk_info.m_i_aval_disk_size))
    {
        return false;
    }

    l_i_disk_index += 0 + MCRO_STRUCT_SIZE(SAvalDiskInfo);

    std::map<uint32, SCateNode>& l_map_catelog = m_p_hdle_catelog->GetMapCatalog();

    if (!CSerialize::UnPackCatelog(l_p_disk_space + l_i_disk_index, (l_i_file_sys_size - l_i_disk_index), l_o_file_sys_info.m_i_cate_node_num, l_map_catelog))
    {
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

    if (NULL != m_p_hdle_aval_disk) // 有数据则认为错误,不释放,拒绝启动系统
    {
        return false;
    }

    if (!m_p_hdle_disk_mirror->CheckDiskMirrorExist()) // 磁盘镜像不存在,则初始化文件系统
    {
        if (!CreateFileSys())
        {
            return false;
        }
    }
    else // 镜像存在,则加载
    {
        if (!LoadFileSys())
        {
            return false;
        }
    }

    return true;
}
