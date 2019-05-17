#include "cmd_save.h"

CCmdSave::CCmdSave(CHandleFileSys* l_p_handle_file_sys)
    :CCmdBase(DEF_CMD_SAVE, l_p_handle_file_sys)
{
}

CCmdSave::~CCmdSave()
{
}

bool CCmdSave::CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    return true;
}

bool CCmdSave::Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    if (NULL == m_p_handle_file_sys)
    {
        a_str_proc_resault = "文件系统未初始化";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    if (! m_p_handle_file_sys->SaveFileSys())
    {
        a_str_proc_resault = "保存文件系统失败";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }    

    a_str_proc_resault = "保存磁盘镜像成功";
    LOG_RECORD(LOG_DEBUG,a_str_proc_resault);
    return true;
}
