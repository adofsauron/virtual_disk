#include "handle_aval_disk.h"

CHandleAvalDisk::CHandleAvalDisk(byte* l_p_aval_disk_space)
{
    m_b_init_over = false;
    if (NULL == l_p_aval_disk_space)
    {
        return;
    }

    m_p_aval_disk_space = l_p_aval_disk_space;
    m_b_init_over = true;
}

bool CHandleAvalDisk::ApplaySpace(const uint64 a_i_size, uint64& a_i_space_index)
{
    if (m_o_aval_disk_info.m_i_aval_index >= m_o_aval_disk_info.m_i_aval_disk_size) // 磁盘已满
    {
        return false;
    }

    if ((m_o_aval_disk_info.m_i_aval_disk_size - m_o_aval_disk_info.m_i_aval_index) < a_i_size ) // 可用空间不足
    {
        return false;
    }

    a_i_space_index = m_o_aval_disk_info.m_i_aval_index;
    m_o_aval_disk_info.m_i_aval_index += a_i_size;
    return true;
}

bool CHandleAvalDisk::ReleaseSpace(const uint64 a_i_index, const uint32 a_i_size)
{
    if (a_i_index >= m_o_aval_disk_info.m_i_aval_disk_size) // 传入的索引非法
    {
        return false;
    }

    if ((m_o_aval_disk_info.m_i_aval_disk_size - a_i_index) < a_i_size) // 要释放的空间超过可释放空间的大小
    {
        return false;
    }

    // 擦除磁盘
    memset(m_p_aval_disk_space + a_i_index, 0x00, a_i_size);

    // TODO: 只有最后的空间被擦除后可以被继续使用, 不支持动态处理
    if ((a_i_index + a_i_size) == m_o_aval_disk_info.m_i_aval_index)
    {
        m_o_aval_disk_info.m_i_aval_index -= a_i_size;
    }

    return true;
}

void CHandleAvalDisk::InitAvalDiskInfo()
{
    m_o_aval_disk_info.m_i_aval_disk_size = (config::CONST_MAX_SIZE_DISK_TOTAL - config::CONST_MAX_SIZE_FILE_SYS);
    m_o_aval_disk_info.m_i_aval_index = 0;
}

SAvalDiskInfo& CHandleAvalDisk::SetAvalDiskInfo(SAvalDiskInfo& a_o_aval_disk_info)
{
    memcpy(&m_o_aval_disk_info, &a_o_aval_disk_info, MCRO_STRUCT_SIZE(SAvalDiskInfo));
    return m_o_aval_disk_info;
}