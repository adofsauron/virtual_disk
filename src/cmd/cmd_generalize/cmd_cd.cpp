#include "cmd_cd.h"

CCmdCD::CCmdCD(CHandleFileSys* l_p_handle_file_sys)
    :CCmdBase(DEF_CMD_CD, l_p_handle_file_sys)
{
}

// 参数合法性校验
bool CCmdCD::CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    if (1 != a_vec_args.size()) // 只能有一个参数
    {
        a_str_proc_resault = "参数数量错误,参数个数错误,限定为1个,传入参数个数为:";
        a_str_proc_resault += a_vec_args.size();

        LOG_RECORD(LOG_ERR,a_str_proc_resault);

        return false;
    }

    if (a_vec_args[0].empty())
    {
        a_str_proc_resault = "输入参数为空";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    return true;
}

bool CCmdCD::Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    if (NULL == m_p_handle_file_sys)
    {
        a_str_proc_resault = "文件系统未初始化";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    std::string l_str_path = a_vec_args[0];
    CDealString::PathConver((char*)l_str_path.c_str(), l_str_path.length());

    std::string l_str_new_path;
    if(! m_p_handle_file_sys->GetFullPath(l_str_path, l_str_new_path))
    {
        a_str_proc_resault = "绝对路径转换失败";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    SCateNode* l_p_node;
    // 判断目录是否存在
    if (! m_p_handle_file_sys->CheckPathExist(l_str_new_path, l_p_node))
    {
        a_str_proc_resault = "路径不存在:";
        a_str_proc_resault += l_str_new_path;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    if (NULL == l_p_node)
    {
        a_str_proc_resault = "路径不存在:";
        a_str_proc_resault += l_str_new_path;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    if (CNODE_DIR != l_p_node->m_i_type)
    {
        a_str_proc_resault = "路径不是目录:";
        a_str_proc_resault += l_str_new_path;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    CEnv::Instance()->ChgPwd(l_str_new_path);

    a_str_proc_resault = "执行成功,当前路径为:";
    a_str_proc_resault += l_str_new_path;

    LOG_RECORD(LOG_DEBUG,a_str_proc_resault);

    return true;
}
