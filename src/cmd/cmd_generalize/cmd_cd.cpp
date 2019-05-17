#include "cmd_cd.h"

CCmdCD::CCmdCD(CHandleFileSys* l_p_handle_file_sys)
    :CCmdBase(DEF_CMD_CD, l_p_handle_file_sys)
{
}

CCmdCD::~CCmdCD()
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
    std::string l_str_path = a_vec_args[0];

    CDealString::PathConver((char*)l_str_path.c_str(), l_str_path.length());

    const char l_chr = l_str_path[0];
    if (('.' != l_chr) || ('/' != l_chr))
    {
        a_str_proc_resault = "路径起始无法识别,内部路径:";
        a_str_proc_resault += l_str_path;
        a_str_proc_resault += ",原路径";
        a_str_proc_resault += a_vec_args[0];

        LOG_RECORD(LOG_ERR,a_str_proc_resault);

        return false;
    }

    // 先找出当前的绝对路径
    std::string l_str_cur_path;
    if ('/' == l_chr)
    {
        l_str_cur_path = "/";
    }
    else
    {
        CEnv::Instance()->GetPwd(l_str_cur_path);
    }

    std::vector<std::string> l_vec_cur_path;
    CDealString::SplitString(l_str_path, "/", l_vec_cur_path);

    std::vector<std::string> l_vec_node_name;
    CDealString::SplitString(l_str_path, "/", l_vec_node_name);

    
    for (size_t i=0; i<l_vec_node_name.size(); ++i)
    {
        const std::string& l_str_node_name = l_vec_node_name[i];

        // 当前节点
        if (( 1 == l_str_node_name.length()) && ("." == l_str_node_name))
        {
            continue;
        }

        // 父节点
        if ((2 == l_str_node_name.length()) && (".." == l_str_node_name))
        {
            if (!l_vec_cur_path.empty())
            {
                l_vec_cur_path.pop_back();
            }
        }

        l_vec_cur_path.push_back(l_str_node_name);
    }

    std::string l_str_new_path = "/";
    for (size_t i=0; i<l_vec_cur_path.size(); ++i)
    {
        l_str_new_path += l_vec_cur_path[i];
        l_str_new_path += "/";
    }

    if ("/" != l_str_new_path)
    {
        l_str_new_path.substr(0, l_str_new_path.length() - 1);
    }


    if (NULL == m_p_handle_file_sys)
    {
        a_str_proc_resault = "文件系统未初始化";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    SCateNode l_o_cata_node;
    // 判断目录是否存在
    if (m_p_handle_file_sys->CheckPathExist(l_str_new_path, l_o_cata_node))
    {
        a_str_proc_resault = "路径不存在:";
        a_str_proc_resault += l_str_new_path;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    if (CNODE_DIR != l_o_cata_node.m_i_type)
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
