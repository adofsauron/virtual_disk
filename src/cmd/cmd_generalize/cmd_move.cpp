#include "cmd_move.h"

CCmdMove::CCmdMove(CHandleFileSys* l_p_handle_file_sys)
    :CCmdBase(DEF_CMD_MOVE, l_p_handle_file_sys)
{
}

bool CCmdMove::CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    if (2 != a_vec_args.size())
    {
        a_str_proc_resault = "参数不是2个:";
        a_str_proc_resault += a_vec_args.size();
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

bool CCmdMove::Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    if (NULL == m_p_handle_file_sys)
    {
        a_str_proc_resault = "文件系统未初始化";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }


    std::string l_str_file = a_vec_args[0];
    std::string l_str_to_file = a_vec_args[1];

     // 路径转换
    CDealString::PathConver((char*)l_str_file.c_str(), l_str_file.length());
    CDealString::PathConver((char*)l_str_to_file.c_str(), l_str_to_file.length());


    std::string l_str_full_name;
    if(!m_p_handle_file_sys->GetFullPath(l_str_file,l_str_full_name))
    {
        a_str_proc_resault = "获取源头文件的绝对路径失败:";
        a_str_proc_resault += l_str_file;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }
    
    std::string l_str_to_full_name;
    if(!m_p_handle_file_sys->GetFullPath(l_str_to_file,l_str_to_full_name))
    {
        a_str_proc_resault = "获取目的文件的绝对路径失败:";
        a_str_proc_resault += l_str_file;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    SCateNode* l_p_src_cata_node;
    if (!m_p_handle_file_sys->CheckPathExist(l_str_full_name, l_p_src_cata_node))
    {
        a_str_proc_resault = "源文件不存在,不可移动:";
        a_str_proc_resault += l_str_full_name;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }


    const uint32 l_i_src_id = l_p_src_cata_node->m_i_id;
    const uint32 l_i_src_father_id = l_p_src_cata_node->m_i_parent_id;

    SCateNode* l_p_to_cata_node;
    if (!m_p_handle_file_sys->CheckPathExist(l_str_to_full_name, l_p_to_cata_node))
    {
        a_str_proc_resault = "目标文件夹不存在,不可移动:";
        a_str_proc_resault += l_str_to_full_name;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    if (CNODE_DIR != l_p_to_cata_node->m_i_type)
    {
        a_str_proc_resault = "目标文件不是文件夹,不可移动:";
        a_str_proc_resault += l_str_to_full_name;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }


    const uint32 l_i_new_father_id = l_p_to_cata_node->m_i_id;

    if (!m_p_handle_file_sys->MoveNode(l_i_src_id, l_i_src_father_id, l_i_new_father_id))
    {
        a_str_proc_resault = "移动节点失败:";
        a_str_proc_resault += l_str_to_full_name;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;   
    }

    a_str_proc_resault = "移动文件成功：";
    a_str_proc_resault += l_str_full_name;
    a_str_proc_resault += "->";
    a_str_proc_resault += l_str_to_full_name;
    LOG_RECORD(LOG_DEBUG,a_str_proc_resault);
    return true;
}
