#include "cmd_base.h"


CCmdBase::CCmdBase(const std::string& l_str_cmd_name, CHandleFileSys* l_p_handle_file_sys)
    :m_p_cmd_name(l_str_cmd_name),
    m_p_handle_file_sys(l_p_handle_file_sys)
{
}

bool CCmdBase::Execute(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    if (!this->CheckFeasibility(a_vec_args, a_str_proc_resault))
    {
        return false;
    }

    if (!this->Dispose(a_vec_args, a_str_proc_resault))
    {
        return false;
    }

    return true;
}
