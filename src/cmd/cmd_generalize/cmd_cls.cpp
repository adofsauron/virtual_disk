#include "cmd_cls.h"

CCmdCls::CCmdCls(CHandleFileSys* l_p_handle_file_sys)
    :CCmdBase(DEF_CMD_CLS, l_p_handle_file_sys)
{
}

CCmdCls::~CCmdCls()
{
}

bool CCmdCls::CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    return true;
}

bool CCmdCls::Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    system("clear");
    a_str_proc_resault = "清屏成功";
    LOG_RECORD(LOG_DEBUG,a_str_proc_resault);
    return true;
}
