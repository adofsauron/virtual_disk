#include "cmd_del.h"

CCmdDel::CCmdDel(CHandleFileSys* l_p_handle_file_sys)
    :CCmdBase(CONST_CMD_DEL, l_p_handle_file_sys)
{
}

bool CCmdDel::CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    if (a_vec_args.empty())
    {
        a_str_proc_resault = "参数为空";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    
    return true;
}

bool CCmdDel::Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    if (NULL == m_p_handle_file_sys)
    {
        a_str_proc_resault = "文件系统未初始化";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    bool l_b_del_all = false; // 删除文件
    bool l_b_del_link_real = false;
    std::vector<std::string> l_vec_args = a_vec_args;

    if ("/s" == a_vec_args[0])
    {
        l_b_del_all = true;
        l_vec_args.erase(l_vec_args.begin());
    }

    if ("/f" == a_vec_args[0])
    {
        l_b_del_link_real = true;
        l_vec_args.erase(l_vec_args.begin());
    }

    for (uint32 i=0; i<l_vec_args.size(); ++i)
    {
         std::string l_str_name = l_vec_args[i];

        // 路径转换
        CDealString::PathToIner(l_str_name);

        if ("/" == l_str_name)
        {
            a_str_proc_resault = "不可删除根目录::";
            a_str_proc_resault += l_str_name;
            LOG_RECORD(LOG_ERR,a_str_proc_resault);
            return false;
        }

        std::string l_str_src_full_name;
        if(!m_p_handle_file_sys->GetFullPath(l_str_name,l_str_src_full_name))
        {
            a_str_proc_resault = "获取文件绝对路径失败:";
            a_str_proc_resault += l_str_name;
            LOG_RECORD(LOG_ERR,a_str_proc_resault);
            return false;
        }

        LOG_DEBUG(l_str_src_full_name);

        SCateNode* l_o_src_cata_node = NULL;
        if (! m_p_handle_file_sys->CheckPathExist(l_str_src_full_name, l_o_src_cata_node))
        {
            a_str_proc_resault = "文件不存在:";
            a_str_proc_resault += l_str_name;
            LOG_RECORD(LOG_ERR,a_str_proc_resault);
            return false;
        }

        if ( (!l_b_del_all) && (CNODE_DIR == l_o_src_cata_node->m_i_type))
        {
            a_str_proc_resault = "文件夹不可删除，请使用/s强制删除";
            a_str_proc_resault += l_str_name;
            LOG_RECORD(LOG_ERR,a_str_proc_resault);
            return false;
        }

		// 删除被链接的文件
		if ((l_b_del_link_real) && (CNODE_LINK == l_o_src_cata_node->m_i_type))
		{
			const uint32 l_i_link_id = l_o_src_cata_node->m_i_link_id;
			SCateNode* l_o_link_cata_node = NULL;
			if (!m_p_handle_file_sys->GetCateNodeById(l_i_link_id, l_o_link_cata_node))
			{
				a_str_proc_resault = "强制删除被链接的文件时,被链接的文件不存在；";
				a_str_proc_resault += l_str_name;
				LOG_RECORD(LOG_INFO, a_str_proc_resault);
				continue;
			}

			if (!m_p_handle_file_sys->DelFile(l_o_link_cata_node->m_p_full_name))
			{
				a_str_proc_resault = "被链接文件删除失败:";
				a_str_proc_resault += l_o_link_cata_node->m_p_full_name;
				LOG_RECORD(LOG_ERR, a_str_proc_resault);
				return false;
			}
		}

        if (! m_p_handle_file_sys->DelFile(l_str_src_full_name))
        {
            a_str_proc_resault = "文件删除失败:";
            a_str_proc_resault += l_str_name;
            LOG_RECORD(LOG_ERR,a_str_proc_resault);
            return false;
        }
    }

    a_str_proc_resault += "文件删除成功";
    LOG_RECORD(LOG_DEBUG,a_str_proc_resault);
    return true;
}
