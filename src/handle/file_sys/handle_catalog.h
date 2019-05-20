#ifndef __HANDLE_CATELOG_H__
#define __HANDLE_CATELOG_H__

#include "string.h"

#include <time.h>
#include <stdlib.h>
#include <map>
#include <set>

#include "../..//base/base.h"
#include "../../base/pack_def.h"

class CHandleCatelog
{
public:
    CHandleCatelog();
    ~CHandleCatelog();

public:
    // 创建新的目录节点
    // out:a_i_new_node_id 返回的文件节点id
    bool AddCateNode(uint32& a_i_new_node_id);

    // 根据id删除目录节点
    bool DelCateNode(const uint32 a_i_id);

    // 检查id是否存在
    bool CheckNodeIdExist(const uint32 a_i_id);

    // 检查路径是否存在
     bool CheckFullPathExist(const std::string& l_str_full_path);

    // 获得目录节点属性,不存在则返回false
    bool GetCateNode(const uint32 a_i_id, SCateNode*& a_p_node);

    // 设置目录节点内容
    bool SetCateNode(const uint32 a_i_id, const SCateNode& a_o_node);

    // 向父节点添加子节点id
    bool FatherAddSonId(const uint32 a_i_fid, const uint32 a_i_sid);

    // 父节点删除子节点id
    bool FatherDelSonId(const uint32 a_i_fid, const uint32 a_i_sid);  

    // 返回文件节点映射
    inline std::map<uint32, SCateNode>& GetMapCatalog() { return m_map_catalog;};

    // 返回节点数量
    inline uint32 GetMapCataNodeNum() { return m_map_catalog.size(); };

    // 根据全路径名称找到节点id
    bool GetNodeIdByFullName(const std::string& a_str_full_name, uint32& l_i_node_id);

    // 插入文件缓存信息
    bool InserCacheFullName2Id(const std::string& a_str_full_name, const uint32 l_i_node_id);

    // 删除缓存信息
    bool DelCacheFullName2Id(const std::string& a_str_full_name);

    void PrintfAllNode();

public:
    // 建立缓存
    bool InitCacheFullName2Id();

    // 重建目录树，只有根节点
    void InitMapCatalog();

    // 清理数据
    void ClearData();

private:
    // 获得新的id，不保证不重复, 业务调用方检查
    uint32 GetNewNodeId();

private:
    // 关联容器如果用map,那每次插入节点和删除节点都会导致内部红黑树重新构建,影响性能
    // 但使用这种哈希方式,为了避免映射冲突,会占用过多的内存
    // 看主要瓶颈在cpu还是内存
    std::map<uint32, SCateNode>            m_map_catalog;              // 目录节点id -> 目录节点的指针
    const uint32                           m_i_root_node_id;           // 根节点,永远为1,且不可修改
    std::map<std::string, uint32>          m_map_full_name_to_id;      // cache, 绝对路径+文件名 -> 节点id

};

#endif//__HANDLE_CATELOG_H__