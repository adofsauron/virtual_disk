#include "cmd_cd.h"

CCmdCD::CCmdCD(const char* l_p_cmd_name, CHandleFileSys* l_p_handle_file_sys)
    :CCmdBase(l_p_cmd_name, l_p_handle_file_sys)
{
}

CCmdCD::~CCmdCD()
{
}

bool CCmdCD::CheckFeasibility()
{
    return true;
}

bool CCmdCD::Dispose()
{
    return true;
}
