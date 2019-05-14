#include "handle_disk_mirror.h"


CHandleDiskMirror::CHandleDiskMirror()
    :m_p_disk_mirror_name(config::CONST_DISK_MIRROR_NAME),
    m_i_total_disk_size(config::CONST_MAX_SIZE_DISK_TOTAL),
    m_i_disk_block(config::CONST_DISK_BLOCK_SIZE),
    m_i_file_sys_size(config::CONST_MAX_SIZE_FILE_SYS),
    m_i_avail_disk_size(config::CONST_MAX_SIZE_DISK_TOTAL - config::CONST_MAX_SIZE_FILE_SYS)
{
    m_p_total_disk = (byte*) malloc (m_i_total_disk_size);
    memset(m_p_total_disk, 0x00, m_i_total_disk_size);
}

// RAII
CHandleDiskMirror::~CHandleDiskMirror()
{
    FREE_PTR(m_p_total_disk);
}

// 将磁盘镜像加载入内存
bool CHandleDiskMirror::LoadDiskMirror()
{
    if (NULL == m_p_total_disk)
    {
        return false;
    }

    if (!CDealFile::CheckFileExist(m_p_disk_mirror_name))
    {
        return false;
    }

    FILE* l_p_file = NULL;
    if (!CDealFile::FileOpen(m_p_disk_mirror_name, "rb+", l_p_file))
    {
        return false;
    }

    if (NULL == l_p_file)
    {
        return false;
    }

    uint64 l_i_count = m_i_total_disk_size / m_i_disk_block;
    if (!CDealFile::FileRead(m_p_total_disk, m_i_total_disk_size, m_i_disk_block, l_i_count, l_p_file))
    {
        return false;
    }

    if (!CDealFile::FileClose(l_p_file))
    {
        return false;
    }

    return true;
}

// 将内存磁盘写入磁盘镜像
bool CHandleDiskMirror::SaveDiskMirror()
{
    if (NULL == m_p_total_disk)
    {
        return false;
    }

    if (!CDealFile::CheckFileExist(m_p_disk_mirror_name))
    {
        if(! CDealFile::FileCreate(m_p_disk_mirror_name))
        {
            return false;
        }
    }

    FILE* l_p_file = NULL;
    if (!CDealFile::FileOpen(m_p_disk_mirror_name, "rb+", l_p_file))
    {
        return false;
    }

    if (NULL == l_p_file)
    {
        return false;
    }

    uint64 l_i_count = m_i_total_disk_size / m_i_disk_block;
    if (!CDealFile::FileWrite(m_p_total_disk, m_i_total_disk_size, m_i_disk_block, l_i_count, l_p_file))
    {
        return false;
    }

    if (!CDealFile::FileClose(l_p_file))
    {
        return false;
    }

    return true;
}

// 获得文件系统的地址空间首地址
byte* CHandleDiskMirror::GetFileSysSpace()
{
    if (NULL == m_p_total_disk)
    {
        return NULL;
    }

    return m_p_total_disk + 0; // 直接从起始作为文件系统占用
}

// 获得可用磁盘地址空间首地址
byte* CHandleDiskMirror::GetAvailDiskSpace()
{
    if (NULL == m_p_total_disk)
    {
        return NULL;
    }

    return m_p_total_disk + m_i_file_sys_size; // 文件系统之后是可用磁盘，中间不再填充隔离标识
}