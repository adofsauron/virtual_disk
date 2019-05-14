// 处理虚拟磁盘镜像相关

#ifndef __HANDLE_DISK_MIRROR_H__
#define __HANDLE_DISK_MIRROR_H__

#include "../base/base.h"
#include "../tools/deal_file.h"

class CHandleDiskMirror
{
public:
    CHandleDiskMirror();
    ~CHandleDiskMirror();

public:
    // 获得文件系统的地址空间首地址
    byte* GetFileSysSpace();

    // 获得可用磁盘地址空间首地址
    byte* GetAvailDiskSpace();

    // 文件系统大小
    inline uint64 GetFileSysSize() {return m_i_file_sys_size;};

    // 可用磁盘大小
    inline uint64 GetAvailDiskSize() {return m_i_avail_disk_size;};

public:
    // 将磁盘镜像加载入内存
    bool LoadDiskMirror();

    // 将内存磁盘写入磁盘镜像
    bool SaveDiskMirror();

private:
    byte* m_p_total_disk;               // 内存磁盘的整个的地址空间
    const char* m_p_disk_mirror_name;   // 磁盘镜像名字

private:
    // 总磁盘空间大小
     uint64 m_i_total_disk_size;

    // 磁盘块大小
     uint64 m_i_disk_block;
    
    // 可用磁盘空间大小
     uint64 m_i_avail_disk_size;

    // 文件系统空间大小
     uint64 m_i_file_sys_size;
};

#endif//__HANDLE_DISK_MIRROR_H