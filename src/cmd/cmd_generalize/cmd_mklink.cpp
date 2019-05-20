#include "cmd_mklink.h"

CCmdMklink::CCmdMklink(CHandleFileSys* l_p_handle_file_sys)
    :CCmdBase(DEF_CMD_MKLINK, l_p_handle_file_sys)
{
}

bool CCmdMklink::CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    if (2 != a_vec_args.size())
    {
        a_str_proc_resault = "参数不是2个:";

         char buff[256] = {0x00};
        snprintf(buff, 256, "%u", a_vec_args.size());

        a_str_proc_resault += buff;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    if (a_vec_args[0].empty() || a_vec_args[1].empty())
    {
        a_str_proc_resault = "参数为空";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    if ( (config::CONST_MAX_SIZE_FILE_NAME < a_vec_args[0].length()) || (config::CONST_MAX_SIZE_FILE_NAME < a_vec_args[1].length()) )
    {
        a_str_proc_resault = "路径过长";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }


    return true;
}

bool CCmdMklink::Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    if (NULL == m_p_handle_file_sys)
    {
        a_str_proc_resault = "文件系统未初始化";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    std::string l_str_file = a_vec_args[0];
    std::string l_str_link_file = a_vec_args[1];

     // 路径转换
    CDealString::PathConver((char*)l_str_file.c_str(), l_str_file.length());
    CDealString::PathConver((char*)l_str_link_file.c_str(), l_str_link_file.length());


    std::string l_str_full_name;
    if(!m_p_handle_file_sys->GetFullPath(l_str_file,l_str_full_name))
    {
        a_str_proc_resault = "获取源头文件的绝对路径失败:";
        a_str_proc_resault += l_str_file;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }
    
    std::string l_str_link_full_name;
    if(!m_p_handle_file_sys->GetFullPath(l_str_link_file,l_str_link_full_name))
    {
        a_str_proc_resault = "获取链接文件的绝对路径失败:";
        a_str_proc_resault += l_str_file;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

     SCateNode* l_p_to_cata_node;
    if (!m_p_handle_file_sys->CheckPathExist(l_str_full_name, l_p_to_cata_node))
    {
        a_str_proc_resault = "源文件不存在,不可创建链接:";
        a_str_proc_resault += l_str_full_name;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }


    const uint32 l_i_src_id = l_p_to_cata_node->m_i_id;

    if (m_p_handle_file_sys->CheckPathExist(l_str_link_full_name, l_p_to_cata_node))
    {
        a_str_proc_resault = "链接文件已存在,不可创建链接:";
        a_str_proc_resault += l_str_link_full_name;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }


    // 先创建,再向目录节点中插入数据
    SCateNode* l_p_new_node = NULL;
    if (! m_p_handle_file_sys->AddNewFile(l_str_link_full_name, CNODE_LINK, (ACCESS_WRITE|ACCESS_RREAD|ACCESS_EXECUTE), l_p_new_node))
    {
        a_str_proc_resault = "文件创建失败";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    if (NULL == l_p_new_node)
    {
        a_str_proc_resault = "文件节点创建失败";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }


    l_p_new_node->m_i_link_id = l_i_src_id;


    a_str_proc_resault = "文件创建成功:";
    a_str_proc_resault += l_str_full_name;
    LOG_RECORD(LOG_DEBUG,a_str_proc_resault);
    return true;
}
