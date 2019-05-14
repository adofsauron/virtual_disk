#include "cmd_base.h"


CCmdBase::CCmdBase(const char* l_p_cmd_name, CHandleFileSys* l_p_handle_file_sys)
    :m_p_cmd_name(l_p_cmd_name),
    m_p_handle_file_sys(l_p_handle_file_sys)
{
}

CCmdBase::~CCmdBase()
{
}

bool CCmdBase::Execute()
{
    if (!this->CheckFeasibility())
    {
        return false;
    }

    if (!this->Dispose())
    {
        return false;
    }

    return true;
}