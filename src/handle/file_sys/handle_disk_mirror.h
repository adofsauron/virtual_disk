// 处理虚拟磁盘镜像相关

#ifndef __HANDLE_DISK_MIRROR_H__
#define __HANDLE_DISK_MIRROR_H__

#include "../../base/base.h"
#include "../../tools/deal_file.h"

class CHandleDiskMirror
{
public:
    CHandleDiskMirror();
    ~CHandleDiskMirror();

public:
    // 设置磁盘信息,不随配置生效,除非重建文件系统
    bool SetDiskMirrorInfo(const uint64 a_i_total_disk_size, const uint64 a_i_disk_block);

    // 将磁盘镜像加载入内存
    bool LoadDiskMirror();

    // 将内存磁盘写入磁盘镜像
    bool SaveDiskMirror();

    // 判断镜像文件是否存在
    bool CheckDiskMirrorExist();

    // 清空数据
    void ClearData();

public:
    // 获取整个磁盘的空间首地址
    inline byte* GetTotalDiskSpace() {return m_p_total_disk;};

private:
    byte* m_p_total_disk;                      // 内存磁盘的整个的地址空间

private:
    const char* m_p_disk_mirror_name;           // 磁盘镜像名字
    uint64 m_i_total_disk_size;                 // 总磁盘空间大小
    uint64 m_i_disk_block;                      // 磁盘块大小
    const uint64 m_i_buff_size;                 // 加载到内存的buff大小
};

#endif//__HANDLE_DISK_MIRROR_H__