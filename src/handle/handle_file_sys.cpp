#include "handle_file_sys.h"

CHandleFileSys::CHandleFileSys(CHandleCatelog* l_p_hdle_clog, CHandleAvaliableDisk* l_p_hdle_aval_disk)
    :m_p_hdle_catelog(l_p_hdle_clog),
    m_p_hdle_aval_disk(l_p_hdle_aval_disk)
{
}

CHandleFileSys::~CHandleFileSys()
{
}