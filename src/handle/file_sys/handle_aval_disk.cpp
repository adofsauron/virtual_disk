#include "handle_aval_disk.h"

CHandleAvalDisk::CHandleAvalDisk()
    :m_p_aval_disk_space(NULL),
    m_b_init_over(false)
{
}

CHandleAvalDisk::~CHandleAvalDisk()
{
}

bool CHandleAvalDisk::SetAvalDiskSpace(byte* l_p_aval_disk_space)
{
    if (NULL == l_p_aval_disk_space)
    {
        return false;
    }

    m_p_aval_disk_space = l_p_aval_disk_space;
    return true;
}

bool CHandleAvalDisk::InitData(byte* l_p_aval_disk_space,const SAvalDiskInfo& a_o_aval_disk_info)
{
    m_b_init_over = false;

    if (!SetAvalDiskSpace(l_p_aval_disk_space))
    {
        return false;
    }

    if(!SetAvalDiskInfo(a_o_aval_disk_info))
    {
        return false;
    }


    m_b_init_over = true;
    return true;
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
    if (0 >= a_i_size) // 传入擦除空间大小非法
    {
        return false;
    }

    if (a_i_index >= m_o_aval_disk_info.m_i_aval_disk_size) // 传入的索引非法
    {
        return false;
    }

    if ((m_o_aval_disk_info.m_i_aval_disk_size - a_i_index) < a_i_size) // 要释放的空间超过可释放空间的大小
    {
        return false;
    }

    //TODO: 因为没有文件->磁盘块的映射关系,每个文件用索引+占位空间的方式使用磁盘,导致一旦对磁盘数据移动,文件中的磁盘索引将不可用
    // 所以,该实现不支持动态的内存分配, 决不能用于正式环境! 

    // 擦除磁盘空间, 模拟操作,就算是LSM的做法也不是这么粗暴,这块得申请时间写得精致一些
    memset(m_p_aval_disk_space + a_i_index, 0x00, a_i_size);

    // 只有最后的空间被擦除后可以被继续使用,其他情况则无法使用
    if ((a_i_index + a_i_size) == m_o_aval_disk_info.m_i_aval_index)
    {
        m_o_aval_disk_info.m_i_aval_index -= a_i_size;
    }

    return true;
}

const SAvalDiskInfo& CHandleAvalDisk::InitAvalDiskInfo()
{
    m_o_aval_disk_info.m_i_aval_disk_size = (config::CONST_MAX_SIZE_DISK_TOTAL - config::CONST_MAX_SIZE_FILE_SYS);
    m_o_aval_disk_info.m_i_aval_index = 0;

    return m_o_aval_disk_info;
}

bool CHandleAvalDisk::SetAvalDiskInfo(const SAvalDiskInfo& a_o_aval_disk_info)
{
    if (a_o_aval_disk_info.m_i_aval_disk_size <= 0)
    {
        return false;
    }

    memcpy(&m_o_aval_disk_info, &a_o_aval_disk_info, MCRO_STRUCT_SIZE(SAvalDiskInfo));
    return true;
}


void CHandleAvalDisk::ClearData()
{
    m_p_aval_disk_space = NULL;

    // 不用memset置0x00的方式,初始化数据可能有特定含义
    m_o_aval_disk_info.m_i_aval_disk_size = 0;
    m_o_aval_disk_info.m_i_aval_index = 0;

    // 必须设置相应的成员变量后方可使用
    m_b_init_over = false;
}

bool CHandleAvalDisk::GetAvalSpace(uint64& l_i_aval_space)
{
    if (m_o_aval_disk_info.m_i_aval_disk_size <= 0)
    {
        return false;
    }

    // index 最大是: total_aval_disk_size -1
    if (m_o_aval_disk_info.m_i_aval_disk_size < m_o_aval_disk_info.m_i_aval_index)
    {
        return false;
    }

    l_i_aval_space = m_o_aval_disk_info.m_i_aval_disk_size - m_o_aval_disk_info.m_i_aval_index;
    return true;
}
