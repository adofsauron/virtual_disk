#include "handle_catalog.h"

CHandleCatelog::CHandleCatelog(CHandleDiskMirror* l_p_hdle_disk_mirror)
    :m_i_node_id_cur_max(1) // 节点从1开始
{
    m_b_init_over = false;
    if ( NULL == l_p_hdle_disk_mirror)
    {
        return;
    }

    m_p_hdle_disk_mirror = l_p_hdle_disk_mirror;



    m_b_init_over = true;
}

CHandleCatelog::~CHandleCatelog()
{
    m_map_catalog.clear();
    m_map_id2son.clear();
    m_map_path2id.clear();
}

bool CHandleCatelog::InitCache()
{

}

bool CHandleCatelog::AddCateNode(uint32& a_i_new_node_id)
{
    // TODO: 没有对磁盘文件系统的目录节点数量做校验, 文件系统的大小可能撑爆指定空间, 第二版添加
    // TODO: 目前这属于hack做法
    if (m_map_catalog.size() >= INT_MAX)
    {
        return false;
    }

    uint32 l_i_nid = GetNewNodeId();

    while (CheckNodeId(l_i_nid))
    {
        l_i_nid = GetNewNodeId();
    }
    
    SCateNode l_o_node;
    l_o_node.m_i_id = l_i_nid;

    m_map_catalog.insert(std::make_pair(l_o_node.m_i_id, l_o_node));

    return true;
}

uint32 CHandleCatelog::GetNewNodeId()
{
    srand((int)time(0));
    return  (uint32) (rand() % (INT_MAX)); // 取模其实没啥意义,rand返回就是int，图个心理安慰,别溢出
}

bool CHandleCatelog::CheckNodeId(const uint32 a_i_id)
{
    std::map<uint32, SCateNode>::const_iterator l_iter = m_map_catalog.find(a_i_id);
    return l_iter != m_map_catalog.end();
}

// 根据id删除目录节点
bool CHandleCatelog::DelCateNode(const uint32 a_i_id)
{
    std::map<uint32, SCateNode>::iterator l_iter = m_map_catalog.find(a_i_id);
    if (l_iter == m_map_catalog.end())
    {
        return false;
    }

    m_map_catalog.erase(l_iter);
    return true;
}

bool CHandleCatelog::GetCateNode(const uint32 a_i_id, SCateNode& a_o_node)
{
    std::map<uint32, SCateNode>::iterator l_iter = m_map_catalog.find(a_i_id);
    if (l_iter == m_map_catalog.end())
    {
        return false;
    }

    a_o_node = l_iter->second;
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

