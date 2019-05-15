#ifndef __HANDLE_AVAL_DISK_H__
#define __HANDLE_AVAL_DISK_H__

#include "../..//base/base.h"

class CHandleAvalDisk
{
public:
    // 必须显式指定可用磁盘信息,没有资源释放,不创建析构函数
    explicit CHandleAvalDisk(byte* l_p_aval_disk_space);

public:
    // 是否初始化完成
    inline bool InitOver() {return m_b_init_over;};

    // 初始化磁盘管理信息
    void InitAvalDiskInfo();

public:
    // 获取指定空间
    // input: a_i_size 指定开辟的空间大小
    // out: a_p_give_space 开辟空间的位置的索引
    bool ApplaySpace(const uint64 a_i_size, uint64& a_i_space_index);

    // 归还指定的空间块,用于删除文件时
    // intpu: a_i_index 删除位置的索引
    // intpu：a_i_size  删除空间的大小
    bool ReleaseSpace(const uint64 a_i_index, const uint32 a_i_size);

public:
    // 获取可用磁盘信息
    inline SAvalDiskInfo& GetAvalDiskInfo() { return m_o_aval_disk_info;};

    // 设置可用磁盘信息
    // return: 设置后的成员变量可用磁盘信息
    SAvalDiskInfo& SetAvalDiskInfo(SAvalDiskInfo& a_o_aval_disk_info);

private:
    byte*               m_p_aval_disk_space;    // 可用磁盘空间的首地址
    bool                m_b_init_over;          // 初始化完成标识
    SAvalDiskInfo       m_o_aval_disk_info;     // 可用磁盘信息
};


#endif//__HANDLE_AVAILIABLE_DISK_H__