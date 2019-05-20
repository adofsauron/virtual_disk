#include "handle_catalog.h"

CHandleCatelog::CHandleCatelog()
    :m_i_root_node_id(1) // 根节点id为1
{
    m_map_catalog.clear();
    m_map_full_name_to_id.clear();
}

CHandleCatelog::~CHandleCatelog()
{
    m_map_catalog.clear();
    m_map_full_name_to_id.clear();
}


bool CHandleCatelog::AddCateNode(uint32& a_i_new_node_id)
{
    // TODO: 没有对磁盘文件系统的目录节点数量做校验, 文件系统的大小可能撑爆指定空间,第二版添加,目前这属于hack做法
    if (m_map_catalog.size() >= INT_MAX)
    {
        return false;
    }

    uint32 l_i_nid = GetNewNodeId();
    bool l_b_get = false;
    for (int i=0; i<10; ++i) // 最多试探十次,防止cpu死循环
    {
        if (!CheckNodeIdExist(l_i_nid))
        {
            l_b_get = true;
            break;
        }

        l_i_nid = GetNewNodeId();
    }

    if (! l_b_get)
    {
        return false;
    }
    
    SCateNode& l_o_node = m_map_catalog[l_i_nid];
    memset(&l_o_node, 0x00, MCRO_STRUCT_SIZE(SCateNode));

    l_o_node.m_i_id = l_i_nid;
    l_o_node.m_i_son_num = 0;

    a_i_new_node_id = l_i_nid;

    return true;
}

uint32 CHandleCatelog::GetNewNodeId()
{
    srand((int)time(0));
    return  (uint32) (rand() % (INT_MAX));
}

// 检测节点id是否已存在
bool CHandleCatelog::CheckNodeIdExist(const uint32 a_i_id)
{
    std::map<uint32, SCateNode>::const_iterator l_iter = m_map_catalog.find(a_i_id);
    return l_iter != m_map_catalog.end();
}

bool CHandleCatelog::CheckFullPathExist(const std::string& l_str_full_path)
{
    std::map<std::string, uint32>::const_iterator l_iter = m_map_full_name_to_id.find(l_str_full_path);
    return  l_iter != m_map_full_name_to_id.end();
}

// 根据id删除目录节点
bool CHandleCatelog::DelCateNode(const uint32 a_i_id)
{
    if (m_i_root_node_id == a_i_id) // 根节点不能删除
    {
        return false;
    }

    std::map<uint32, SCateNode>::iterator l_iter = m_map_catalog.find(a_i_id);
    if (l_iter == m_map_catalog.end())
    {
        return false;
    }

    m_map_catalog.erase(l_iter);
    return true;
}

bool CHandleCatelog::GetCateNode(const uint32 a_i_id, SCateNode*& a_p_node)
{
    std::map<uint32, SCateNode>::iterator l_iter = m_map_catalog.find(a_i_id);
    if (l_iter == m_map_catalog.end())
    {
        return false;
    }

    a_p_node = &l_iter->second;
    return true;
}

bool CHandleCatelog::SetCateNode(const uint32 a_i_id, const SCateNode& a_o_node)
{
    std::map<uint32, SCateNode>::iterator l_iter = m_map_catalog.find(a_i_id);
    if (l_iter == m_map_catalog.end())
    {
        return false;
    }

    l_iter->second = a_o_node;
    return true;
}

// 向父节点添加子节点id
bool CHandleCatelog::FatherAddSonId(const uint32 a_i_fid, const uint32 a_i_sid)
{
    PrintfAllNode();

    if ((0 >= a_i_fid) || (0 >= a_i_sid))
    {
        LOG_ERR("");
        return false;
    }

    std::map<uint32, SCateNode>::iterator l_iter = m_map_catalog.find(a_i_sid);
    if (m_map_catalog.end() == l_iter) // 子节点不存在
    {
        LOG_ERR("");
        return false;
    }

    l_iter = m_map_catalog.find(a_i_fid); // 父节点不存在
    if (m_map_catalog.end() == l_iter)
    {
        LOG_ERR("");
        std::cout<<a_i_fid<<std::endl;
        return false;
    }

    SCateNode& l_o_father_node = l_iter->second;

    if ((l_o_father_node.m_i_son_num > 0) && (NULL == l_o_father_node.m_p_son_set))
    {
        LOG_ERR("");
        return false;
    }

    for (int i=0; i<l_o_father_node.m_i_son_num; ++i)
    {
        if (a_i_sid == l_o_father_node.m_p_son_set[i]) // 该节点已存在
        {
            LOG_ERR("");
            return false;
        }
    }

    // 开辟空间,不再判断是否是第一次malloc，由realloc负责,相关数据已经在SCateNode构造函数中赋值
    l_o_father_node.m_p_son_set = (uint32*) realloc (l_o_father_node.m_p_son_set, (l_o_father_node.m_i_son_num +1) * sizeof(uint32));
    if (NULL == l_o_father_node.m_p_son_set)
    {
        LOG_ERR("");
        return false;
    }

    l_o_father_node.m_i_son_num += 1;
    l_o_father_node.m_p_son_set[l_o_father_node.m_i_son_num-1] = a_i_sid;

    return true;
}

// 父节点删除子节点id
bool CHandleCatelog::FatherDelSonId(const uint32 a_i_fid, const uint32 a_i_sid)
{
     if ((0 >= a_i_fid) || (0 >= a_i_sid))
    {
        return false;
    }

    std::map<uint32, SCateNode>::iterator l_iter = m_map_catalog.find(a_i_sid);
    if (m_map_catalog.end() == l_iter) // 子节点不存在
    {
        return false;
    }

    l_iter = m_map_catalog.find(a_i_fid); // 父节点不存在
    if (m_map_catalog.end() == l_iter)
    {
        return false;
    }

    SCateNode& l_o_father_node = l_iter->second;

    if ((l_o_father_node.m_i_son_num > 0) && (NULL == l_o_father_node.m_p_son_set))
    {
        return false;
    }

    bool l_b_find = false;
    uint32 l_i_index = 0;
    for (; l_i_index < l_o_father_node.m_i_son_num; ++l_i_index)
    {
        if (a_i_sid == l_o_father_node.m_p_son_set[l_i_index])
        {
            l_b_find = true;
            break;
        }
    }

    if (!l_b_find) // 节点id在父目录的子节点集合中未发现
    {
        return false;
    }

    if (1 == l_o_father_node.m_i_son_num) // 只剩一个节点就清理内存
    {
        DELETE_PTR(l_o_father_node.m_p_son_set);
        l_o_father_node.m_i_son_num = 0;
    }
    else
    {
        if (l_i_index != l_o_father_node.m_i_son_num -1) // 使用尾部替换释放尾部的方式清理节点
        {
            l_o_father_node.m_p_son_set[l_i_index] = l_o_father_node.m_p_son_set[l_o_father_node.m_i_son_num -1];
        }

        l_o_father_node.m_p_son_set = (uint32*) realloc (l_o_father_node.m_p_son_set, (l_o_father_node.m_i_son_num - 1) * sizeof(uint32));
        if (NULL == l_o_father_node.m_p_son_set)
        {
            return false;
        }

        l_o_father_node.m_i_son_num -= 1; // 确认无误后再修改子节点数量
    }

    return true;
}

void CHandleCatelog::InitMapCatalog()
{
    m_map_catalog.clear();
    m_map_full_name_to_id.clear();

    SCateNode l_o_root_node;
    l_o_root_node.m_i_id = m_i_root_node_id;
    l_o_root_node.m_i_type = CNODE_DIR;
    l_o_root_node.m_i_access = (ACCESS_RREAD | ACCESS_WRITE | ACCESS_EXECUTE);

    l_o_root_node.m_i_create_time = CDate::GetUnixTime();
    l_o_root_node.m_i_update_time = CDate::GetUnixTime();

    memset(l_o_root_node.m_p_name, 0x00, config::CONST_MAX_SIZE_FILE_NAME + 1);
    memset(l_o_root_node.m_p_full_name, 0x00, config::CONST_MAX_SIZE_FILE_FULL_NAME + 1);

    memcpy(l_o_root_node.m_p_name, "/", strlen("/"));
    memcpy(l_o_root_node.m_p_full_name, "/", strlen("/"));

    m_map_catalog.insert(std::make_pair(l_o_root_node.m_i_id, l_o_root_node));
    m_map_full_name_to_id.insert(std::make_pair(l_o_root_node.m_p_full_name, l_o_root_node.m_i_id));
}

bool CHandleCatelog::InitCacheFullName2Id()
{
    if (m_map_catalog.empty())
    {
        LOG_ERR("m_map_catalog.empty()");
        return false;
    }

    m_map_full_name_to_id.clear();

    std::map<uint32, SCateNode>::const_iterator l_iter = m_map_catalog.begin();
    for (; l_iter != m_map_catalog.end(); ++l_iter)
    {
        const SCateNode& l_o_node = l_iter->second;

        std::string log = "name:";
        log += l_o_node.m_p_full_name;
        log += "\tid:";

        char buff[1024] = {0x00};
        snprintf(buff, 1024, "%u", l_o_node.m_i_id);

        log += buff;
        LOG_INFO(log);

        m_map_full_name_to_id.insert(std::make_pair(l_o_node.m_p_full_name, l_o_node.m_i_id));
    }

    return true;
}

bool CHandleCatelog::GetNodeIdByFullName(const std::string& a_str_full_name, uint32& l_i_node_id)
{
    if (m_map_full_name_to_id.empty())
    {
        return false;
    }

    if (a_str_full_name.empty())
    {
        return false;
    }

    std::map<std::string, uint32>::const_iterator l_iter = m_map_full_name_to_id.find(a_str_full_name);
    if (l_iter == m_map_full_name_to_id.end())
    {
        return false;
    }

    l_i_node_id = l_iter->second;

    return true;
}

void CHandleCatelog::ClearData()
{
    m_map_catalog.clear();
    m_map_full_name_to_id.clear();
}

// 插入文件缓存信息
bool CHandleCatelog::InserCacheFullName2Id(const std::string& a_str_full_name, const uint32 l_i_node_id)
{
    std::map<std::string, uint32>::const_iterator l_iter = m_map_full_name_to_id.find(a_str_full_name);
    if (l_iter != m_map_full_name_to_id.end()) // 缓存信息存在,则拒绝插入,否则可能破坏已有数据关系
    {
        return false;
    }

    m_map_full_name_to_id.insert(std::make_pair(a_str_full_name, l_i_node_id));
    return true;
}

// 删除缓存信息
bool CHandleCatelog::DelCacheFullName2Id(const std::string& a_str_full_name)
{
    std::map<std::string, uint32>::const_iterator l_iter = m_map_full_name_to_id.find(a_str_full_name);
    if (l_iter == m_map_full_name_to_id.end()) // 缓存不存在，则认为数据异常
    {
        return false;
    }

    m_map_full_name_to_id.erase(l_iter);
    return true;
}

void CHandleCatelog::PrintfAllNode()
{
    std::map<uint32, SCateNode>::const_iterator l_iter = m_map_catalog.begin();
    for (;l_iter != m_map_catalog.end(); ++l_iter)
    {
        const SCateNode& l_o_node = l_iter->second;
        SCateNode::Print((SCateNode*)&l_o_node);
    }
}
