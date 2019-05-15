#ifndef __HANDLE_CATELOG_H__
#define __HANDLE_CATELOG_H__

#include "string.h"

#include <time.h>
#include <stdlib.h>
#include <map>
#include <set>

#include "../base/base.h"
#include "../base/pack_def.h"

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

    // 获得目录节点对象的引用
    // 不存在则返回NULL, 调用方必须判断返回值
    // 不返回引用是因为无法区分节点不存在的情况
    SCateNode* GetCateNode(const uint32 a_i_id);

    // 设置目录节点内容
    bool SetCateNode(const uint32 a_i_id, const SCateNode& a_o_node);

    // 向父节点添加子节点id
    bool FatherAddSonId(const uint32 a_i_fid, const uint32 a_i_sid);

    // 父节点删除子节点id
    bool FatherDelSonId(const uint32 a_i_fid, const uint32 a_i_sid);  

    // 返回文件节点映射
    inline std::map<uint32, SCateNode>& GetMapCatalog() { return m_map_catalog;};

    // 根据全路径名称找到节点id
    // input: a_str_full_name 要查找的全路径名称
    // output: l_i_node_id 查找到的节点id
    // return: false 未找到, true 找到
    bool GetNodeIdByFullName(const std::string& a_str_full_name, uint32& l_i_node_id);

public:
    // 建立缓存
    void InitCacheFullName2Id();

    // 重建目录树，只有根节点
    void InitMapCatalog();

private:
    // 获得新的id，不保证不重复, 业务调用方检查
    uint32 GetNewNodeId();

private:
    std::map<uint32, SCateNode>             m_map_catalog;              // 目录节点id -> 目录节点的指针
    const uint32                            m_i_root_node_id;           // 根节点,永远为1,且不可删除
    std::map<std::string, uint32>           m_map_full_name_to_id;      // cache, 绝对路径+文件名 -> 节点id

};

#endif//__HANDLE_CATELOG_H__