#include "handle_disk_mirror.h"


CHandleDiskMirror::CHandleDiskMirror()
    :m_p_disk_mirror_name(config::CONST_DISK_MIRROR_NAME),
    m_i_buff_size(config::CONST_MAX_SIZE_MIRROR_BUFF)
{
    m_p_total_disk = (byte*) malloc (m_i_buff_size);
    memset(m_p_total_disk, 0x00, m_i_total_disk_size);
}

// RAII
CHandleDiskMirror::~CHandleDiskMirror()
{
    FREE_PTR(m_p_total_disk);
}

bool CHandleDiskMirror::SetDiskMirrorInfo(const uint64 a_i_total_disk_size, const uint64 a_i_disk_block)
{
    if ((0 >= a_i_total_disk_size) || (0 >= a_i_disk_block))
    {
         std::cout << "SetDiskMirrorInfo err, a_i_total_disk_size:" 
            << a_i_total_disk_size << ",a_i_disk_block:"<<a_i_disk_block << std::endl;
        return false;
    }

    m_i_total_disk_size = a_i_total_disk_size;
    m_i_disk_block = a_i_disk_block;

    return true;
}

void CHandleDiskMirror::ClearData()
{
    memset(m_p_total_disk, 0x00, m_i_total_disk_size);
    m_i_total_disk_size = 0;
    m_i_disk_block = 0;
}

// 将磁盘镜像加载入内存
bool CHandleDiskMirror::LoadDiskMirror()
{
    std::string log = "ready LoadDiskMirror";

    LOG_RECORD(LOG_INFO, log);
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
        log = "FileOpen err:";
        log += m_p_disk_mirror_name;
        LOG_RECORD(LOG_ERR, log);
        return false;
    }

    if (NULL == l_p_file)
    {
        return false;
    }

    memset(m_p_total_disk, 0x00, m_i_buff_size);

    uint64 l_i_count = m_i_total_disk_size / m_i_disk_block;


    char buff[1024] = {0x00};
    snprintf(buff, 1024, "ready read: total:%llu block:%llu, count:%llu",
        m_i_total_disk_size, m_i_disk_block, l_i_count);

    log = buff;
    LOG_RECORD(LOG_INFO, log);


    if (!CDealFile::FileRead(m_p_total_disk, m_i_total_disk_size, m_i_disk_block, l_i_count, l_p_file))
    {
        log = "FileRead err:";
        log += m_p_disk_mirror_name;
        LOG_RECORD(LOG_ERR, log);
        return false;
    }

    if (!CDealFile::FileClose(l_p_file))
    {
        log = "FileClose err:";
        log += m_p_disk_mirror_name;
        LOG_RECORD(LOG_ERR, log);
        return false;
    }

    LOG_RECORD(LOG_INFO, "load file sys over");
    return true;
}

// 将内存磁盘写入磁盘镜像
bool CHandleDiskMirror::SaveDiskMirror()
{
    if (NULL == m_p_total_disk)
    {
        std::cout << "NULL == m_p_total_disk"<< std::endl;
        return false;
    }

    // 镜像文件不存在则创建,若存在直接数据覆盖
    if (! CDealFile::CheckFileExist(m_p_disk_mirror_name))
    {
        if(! CDealFile::FileCreate(m_p_disk_mirror_name))
        {
            std::cout << "FileCreate error"<< std::endl;
            return false;
        }
    }

    FILE* l_p_file = NULL;
    if (! CDealFile::FileOpen(m_p_disk_mirror_name, "rb+", l_p_file))
    {
        std::cout << "FileOpen error"<< std::endl;
        return false;
    }

    if (NULL == l_p_file)
    {
        std::cout << "NULL == l_p_file"<< std::endl;
        return false;
    }

    uint64 l_i_count = m_i_total_disk_size / m_i_disk_block;
    if (! CDealFile::FileWrite(m_p_total_disk, m_i_total_disk_size, m_i_disk_block, l_i_count, l_p_file))
    {
        std::cout << "FileWrite error"<< std::endl;
        return false;
    }

    if (!CDealFile::FileClose(l_p_file))
    {
        std::cout << "FileClose error"<< std::endl;
        return false;
    }
    return true;
}

bool CHandleDiskMirror::CheckDiskMirrorExist()
{
    if (NULL == m_p_disk_mirror_name)
    {
        return false;
    }

    return CDealFile::CheckFileExist(m_p_disk_mirror_name);
}

