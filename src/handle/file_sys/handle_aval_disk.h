#ifndef __HANDLE_AVAL_DISK_H__
#define __HANDLE_AVAL_DISK_H__

#include "../..//base/base.h"

class CHandleAvalDisk
{
public:
    explicit CHandleAvalDisk();
    ~CHandleAvalDisk();

public:
    // 是否初始化完成, 必须initData后方可使用
    inline bool InitOver() {return m_b_init_over;};

    // 初始化磁盘管理信息,只能在创建磁盘文件系统时使用
    const SAvalDiskInfo& InitAvalDiskInfo();
    
    // 成员数据初始化
    bool InitData(byte* l_p_aval_disk_space, const SAvalDiskInfo& a_o_aval_disk_info);

    // 清理数据
    void ClearData();

public:
    // 获取指定空间
    // input: a_i_size 指定开辟的空间大小
    // out: a_p_give_space 开辟空间的位置的索引
    bool ApplaySpace(const uint64 a_i_size, uint64& a_i_space_index);

    // 归还指定的空间块,用于删除文件时
    // intpu: a_i_index 删除位置的索引
    // intpu：a_i_size  删除空间的大小
    bool ReleaseSpace(const uint64 a_i_index, const uint32 a_i_size);

    // 获得可用的空间大小
    bool GetAvalSpace(uint64& l_i_aval_space);

    // 获得可用空间的首地址
    inline byte* GetAvalSpacePtr() { return  m_p_aval_disk_space;};

public:
    // 获取可用磁盘信息
    inline SAvalDiskInfo& GetAvalDiskInfo() { return m_o_aval_disk_info;};  

private:
    // 设置可用磁盘空间的首地址
    bool SetAvalDiskSpace(byte* l_p_aval_disk_space);

    // 设置可用磁盘信息
    bool SetAvalDiskInfo(const SAvalDiskInfo& a_o_aval_disk_info);

private:
    byte*               m_p_aval_disk_space;    // 可用磁盘空间的首地址
    bool                m_b_init_over;          // 初始化完成标识
    SAvalDiskInfo       m_o_aval_disk_info;     // 可用磁盘信息
};


#endif//__HANDLE_AVAILIABLE_DISK_H__
