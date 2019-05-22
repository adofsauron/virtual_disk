#include "cmd_move.h"

CCmdMove::CCmdMove(CHandleFileSys* l_p_handle_file_sys)
    :CCmdBase(CONST_CMD_MOVE, l_p_handle_file_sys)
{
}

bool CCmdMove::CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    if (a_vec_args.empty())
    {
        a_str_proc_resault = "参数为空:";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    if ((3 == a_vec_args.size()) && ("/y" != a_vec_args[0]))
    {
        a_str_proc_resault = "命令参数配置错误,必须为/y:";
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


    bool l_b_force = "/y" == a_vec_args[0];


    std::string l_str_file;
    std::string l_str_to_file;
    if (l_b_force)
    {
        l_str_file = a_vec_args[1];
        l_str_to_file = a_vec_args[2];
    }
    else
    {
        l_str_file = a_vec_args[0];
        l_str_to_file = a_vec_args[1];
    }
    

     // 路径转换
    CDealString::PathToIner(l_str_file);
    CDealString::PathToIner(l_str_to_file);


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

    SCateNode* l_p_src_cata_node = NULL;
    if (!m_p_handle_file_sys->CheckPathExist(l_str_full_name, l_p_src_cata_node))
    {
        a_str_proc_resault = "源文件不存在,不可移动:";
        a_str_proc_resault += l_str_full_name;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    std::string l_str_only_name;
    if (! m_p_handle_file_sys->GetOnlyNameByFullName(l_str_full_name, l_str_only_name))
    {
        a_str_proc_resault = "简略名字不存在:";
        a_str_proc_resault += l_str_full_name;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    const uint32 l_i_src_id = l_p_src_cata_node->m_i_id;
    const uint32 l_i_src_father_id = l_p_src_cata_node->m_i_parent_id;

    SCateNode* l_p_to_cata_node = NULL;
    if (! m_p_handle_file_sys->CheckPathExist(l_str_to_full_name, l_p_to_cata_node))
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

    // 检测目标目录下是否存在同名文件
    if (l_p_to_cata_node->m_i_son_num > 0)
    {
        for (uint32 i=0; i<l_p_to_cata_node->m_i_son_num; ++i)
        {
            const uint32 l_i_son = l_p_to_cata_node->m_p_son_set[i];
            SCateNode* l_p_son = NULL;
            if (! m_p_handle_file_sys->GetCateNodeById(l_i_son, l_p_son))
            {
                a_str_proc_resault = "获取目标目录的子文件失败:";
                a_str_proc_resault += l_str_to_full_name;
                LOG_RECORD(LOG_ERR,a_str_proc_resault);
                return false;
            }

            if ((l_p_son->m_p_name == l_str_only_name) && (!l_b_force)) 
            {
                a_str_proc_resault = "目标目录下已存在同名文件,请使用/y强制覆盖执行:";
                a_str_proc_resault += l_str_to_full_name;
                LOG_RECORD(LOG_ERR,a_str_proc_resault);
                return false;
            }

            if ((l_p_son->m_p_name == l_str_only_name) && (l_b_force))
            {
                const std::string l_str_son_full_name = l_p_son->m_p_full_name;
                if (! m_p_handle_file_sys->DelFile(l_str_son_full_name))
                {
                    a_str_proc_resault = "强行删除目录下同名文件失败:";
                    a_str_proc_resault += l_str_son_full_name;
                    LOG_RECORD(LOG_ERR,a_str_proc_resault);
                    return false;
                }
            }
        }
    }


    std::string l_str_new_father_name = l_p_to_cata_node->m_p_full_name;

	if ("/" != l_str_new_father_name)
	{
		l_str_new_father_name += "/";
	}
    
    l_str_new_father_name += l_str_only_name;

    memset(l_p_src_cata_node->m_p_full_name, 0x00, config::CONST_MAX_SIZE_FILE_FULL_NAME+1);
    memcpy(l_p_src_cata_node->m_p_full_name, l_str_new_father_name.c_str(), l_str_new_father_name.length());

    const uint32 l_i_new_father_id = l_p_to_cata_node->m_i_id;

    if (! m_p_handle_file_sys->MoveNode(l_i_src_id, l_i_src_father_id, l_i_new_father_id))
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
