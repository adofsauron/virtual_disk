#ifndef __HANDLE_CATELOG_H__
#define __HANDLE_CATELOG_H__

#include <time.h>
#include <stdlib.h>
#include <map>
#include <set>

#include "../handle_disk_mirror.h"
#include "../base/base.h"
#include "../base/pack_def.h"

class CHandleDiskMirror;

class CHandleCatelog
{
public:
    explicit CHandleCatelog(CHandleDiskMirror* a_p_hdle_disk_mirror);
    ~CHandleCatelog();

public:
    // 创建新的目录节点
    bool AddCateNode(uint32& a_i_new_node_id);

    // 根据id删除目录节点
    bool DelCateNode(const uint32 a_i_id);

    // 检查id是否存在
    bool CheckNodeId(const uint32 a_i_id);

    // 获得目录节点对象的引用
    bool GetCateNode(const uint32 a_i_id, SCateNode& a_o_node);

    // 设置目录节点内容
    bool SetCateNode(const uint32 a_i_id, const SCateNode& a_o_node);

public:
    inline bool InitOver() {return m_b_init_over;};

    // 建立缓存
    bool InitCache();

private:
    uint32 GetNewNodeId();

private:
    std::map<uint32, SCateNode>             m_map_catalog;              // 目录节点id -> 目录节点的指针
    CHandleDiskMirror*                      m_p_hdle_disk_mirror;       // 磁盘操作
    bool                                    m_b_init_over;              // 初始化完成标识
    uint32                                  m_i_node_id_cur_max;        // 当前最大的node_id
    uint32                                  m_i_root_node_id;           // 跟节点,永远为1,且不可删除
    std::map<uint32, std::set<uint32> >     m_map_id2son;               // 节点id - > 儿子节点集合的映射

private:
    std::map<std::string, uint32>           m_map_path2id;              // cache, 绝对路径+文件名 -> 节点id

};

#endif//__HANDLE_CATELOG_H__