#include "cmd_ren.h"

CCmdRen::CCmdRen(CHandleFileSys* l_p_handle_file_sys)
    :CCmdBase(CONST_CMD_REN, l_p_handle_file_sys)
{
}

bool CCmdRen::CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    if (2 != a_vec_args.size())
    {
        a_str_proc_resault = "参数不是2个";
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

bool CCmdRen::Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    if (NULL == m_p_handle_file_sys)
    {
        a_str_proc_resault = "文件系统未初始化";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    std::string l_str_file = a_vec_args[0];
    std::string l_str_to_file = a_vec_args[1];

	bool l_b_aval = true;
	for (uint32 i = 0; i<l_str_to_file.length(); ++i)
	{
		const char lc = l_str_to_file[i];

		if ('.' == lc)
		{
			continue;
		}

		if (!((CDealString::IsZhCh(lc) != 0) 
			|| (isdigit(lc) != 0) 
			|| (isalpha(lc) != 0))) 
		{
			l_b_aval = false;
			break;
		}
	}

	if (!l_b_aval)
	{
		a_str_proc_resault = "修改后的文件名包含不支持的字符,仅支持中文,英文字符，数字:";
		a_str_proc_resault += l_str_to_file;
		LOG_RECORD(LOG_ERR, a_str_proc_resault);
		return false;
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
        a_str_proc_resault = "获取目标文件的绝对路径失败:";
        a_str_proc_resault += l_str_file;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

     SCateNode* l_p_src_cate_node = NULL;
    if (!m_p_handle_file_sys->CheckPathExist(l_str_full_name, l_p_src_cate_node))
    {
        a_str_proc_resault = "源文件不存在,不可重命名:";
        a_str_proc_resault += l_str_full_name;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    if (m_p_handle_file_sys->CheckPathExist(l_str_to_full_name, l_p_src_cate_node))
    {
        a_str_proc_resault = "目标文件已存在,不可重命名:";
        a_str_proc_resault += l_str_to_full_name;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    std::string l_str_only_name;
    if (! m_p_handle_file_sys->GetOnlyNameByFullName(l_str_to_full_name, l_str_only_name))
    {
        a_str_proc_resault = "获取简略名字失败";
        a_str_proc_resault += l_str_to_full_name;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

	memset(l_p_src_cate_node->m_p_name, 0x00, config::CONST_MAX_SIZE_FILE_NAME + 1);
	memset(l_p_src_cate_node->m_p_full_name, 0x00, config::CONST_MAX_SIZE_FILE_FULL_NAME + 1);

    memcpy(l_p_src_cate_node->m_p_full_name, l_str_to_full_name.c_str(), l_str_to_full_name.length());
    memcpy(l_p_src_cate_node->m_p_name, l_str_only_name.c_str(), l_str_only_name.length());
	l_p_src_cate_node->m_i_update_time = CDate::GetUnixTime();

    // 修改缓存
    if (! m_p_handle_file_sys->ReBuildCatelogCache())
    {
        a_str_proc_resault = "重建目录树缓存失败,";
        a_str_proc_resault += l_str_to_full_name;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

	if (! m_p_handle_file_sys->MotifySonFullName(l_p_src_cate_node->m_i_id))
	{
		a_str_proc_resault = "重命名子目录绝对路径失败";
		LOG_RECORD(LOG_DEBUG, a_str_proc_resault);
		return false;
	}

    a_str_proc_resault = "重命名成功";
    LOG_RECORD(LOG_DEBUG,a_str_proc_resault);
    return true;
}

