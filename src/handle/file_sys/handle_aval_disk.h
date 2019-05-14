#ifndef __HANDLE_AVAL_DISK_H__
#define __HANDLE_AVAL_DISK_H__

#include "../handle_disk_mirror.h"
#include "../base/base.h"
#include "../base/pack_def.h"

class CHandleAvalDisk
{
public:
    explicit CHandleAvalDisk(CHandleDiskMirror* a_p_hdle_disk_mirror);
    ~CHandleAvalDisk();

public:
    inline bool InitOver() {return m_b_init_over;};

public:
    // 获取指定空间
    // input: a_i_size 指定开辟的空间大小
    // out: a_p_give_space 开辟空间的未知的索引
    bool GetSpace(const uint64 a_i_size, uint64& a_i_space_index);

private:
    CHandleDiskMirror*  m_p_hdle_disk_mirror;   // 磁盘镜像管理
    byte*               m_p_aval_disk_space;    // 可用磁盘空间的首地址
    uint64              m_i_aval_index;         // 可用地址空间位置的索引 base_ptr + inde 的方式访问磁盘
    bool                m_b_init_over;          // 初始化完成标识
    
};


#endif//__HANDLE_AVAILIABLE_DISK_H__