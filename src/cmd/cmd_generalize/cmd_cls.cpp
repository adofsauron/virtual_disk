#include "cmd_cls.h"

CCmdCls::CCmdCls(CHandleFileSys* l_p_handle_file_sys)
    :CCmdBase(CONST_CMD_CLS, l_p_handle_file_sys)
{
}

bool CCmdCls::CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    return true;
}

bool CCmdCls::Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    const char* l_p_cmd = NULL;
#ifdef __WIN32__
    l_p_cmd = "cls";
#else
    l_p_cmd = "clear";
#endif

    if (0 != system(l_p_cmd))
    {
        a_str_proc_resault = "清屏失败";
        LOG_ERR(a_str_proc_resault);
        return false;
    }
    
    a_str_proc_resault = "\r";
    LOG_RECORD(LOG_DEBUG,a_str_proc_resault);
    return true;
}
