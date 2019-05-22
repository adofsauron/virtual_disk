#include "client_cmd_adaptor.h"

CClientCmdAdaptor::CClientCmdAdaptor(CCmdManager* l_p_cmd_mangaer)
{
    m_b_init_over = false;
    if (NULL == l_p_cmd_mangaer)
    {
        return;
    }

    m_p_cmd_mangaer = l_p_cmd_mangaer;
    m_b_init_over = true;
}

CClientCmdAdaptor::~CClientCmdAdaptor()
{
    m_p_cmd_mangaer = NULL;
    m_b_init_over = false;
}

bool CClientCmdAdaptor::ProcCmd(const std::vector<std::string>& a_vec_commond, std::string& a_str_proc_resault)
{
    if (a_vec_commond.empty())
    {
        a_str_proc_resault = "参数为空";
        return false;
    }

    const std::string& l_str_cmd = a_vec_commond[0];

    if (l_str_cmd.empty())
    {
        a_str_proc_resault = "命令为空";
        return false;
    }

    if (NULL == m_p_cmd_mangaer)
    {
        a_str_proc_resault = "命令处理模块未初始化";
        return false;
    }

    std::vector<std::string> l_vec_args = a_vec_commond;
    l_vec_args.erase(l_vec_args.begin()); // 只保留参数

    return  m_p_cmd_mangaer->RunCmd(l_str_cmd, l_vec_args, a_str_proc_resault);
}


