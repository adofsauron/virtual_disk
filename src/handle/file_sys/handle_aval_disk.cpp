#include "handle_aval_disk.h"

CHandleAvalDisk::CHandleAvalDisk(CHandleDiskMirror* a_p_hdle_disk_mirror)
{
    m_b_init_over = false;
    if (NULL == a_p_hdle_disk_mirror)
    {
        return;
    }

    m_p_hdle_disk_mirror = a_p_hdle_disk_mirror;
    m_p_aval_disk_space = m_p_hdle_disk_mirror->GetAvailDiskSpace();
    m_b_init_over = true;
}

CHandleAvalDisk::~CHandleAvalDisk()
{
}

