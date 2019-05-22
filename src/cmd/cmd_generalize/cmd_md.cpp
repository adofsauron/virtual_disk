#include "cmd_md.h"

CCmdMd::CCmdMd(CHandleFileSys* l_p_handle_file_sys)
    :CCmdBase(CONST_CMD_MD, l_p_handle_file_sys)
{
}

bool CCmdMd::CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    if (1 != a_vec_args.size())
    {
        a_str_proc_resault = "参数不是1个";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    if (a_vec_args[0].empty())
    {
        a_str_proc_resault = "参数为空";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    return true;
}

bool CCmdMd::Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    if (NULL == m_p_handle_file_sys)
    {
        a_str_proc_resault = "文件系统未初始化";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    std::string l_str_path = a_vec_args[0];

     // 路径转换
    CDealString::PathToIner(l_str_path);

     std::string l_str_full_name;
    if(!m_p_handle_file_sys->GetFullPath(l_str_path,l_str_full_name))
    {
        a_str_proc_resault = "获取文件的绝对路径失败:";
        a_str_proc_resault += l_str_path;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    LOG_INFO(l_str_full_name);

    SCateNode* l_p_src_cata_node;
    if (m_p_handle_file_sys->CheckPathExist(l_str_full_name, l_p_src_cata_node))
    {
        a_str_proc_resault = "文件已经存在,不可创建";
        a_str_proc_resault += l_str_full_name;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    // 先创建,再向目录节点中插入数据
    SCateNode* l_p_new_node = NULL;
    if (! m_p_handle_file_sys->AddNewFile(l_str_full_name, CNODE_DIR, (ACCESS_WRITE|ACCESS_RREAD|ACCESS_EXECUTE), l_p_new_node))
    {
        a_str_proc_resault = "文件节点创建失败";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    if (NULL == l_p_new_node)
    {
        a_str_proc_resault = "文件节点创建失败";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }


    a_str_proc_resault = "文件创建成功:";
    a_str_proc_resault += l_str_full_name;
    LOG_RECORD(LOG_DEBUG,a_str_proc_resault);
    return true;
}
