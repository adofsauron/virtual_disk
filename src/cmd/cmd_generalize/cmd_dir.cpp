#include "cmd_dir.h"

CCmdDir::CCmdDir(CHandleFileSys* l_p_handle_file_sys)
    :CCmdBase(DEF_CMD_DIR, l_p_handle_file_sys)
{
}

bool CCmdDir::CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    if (2 > a_vec_args.size())
    {
        a_str_proc_resault = "第一个参数为/s或/as，随后为目录，输入个数错误";

        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    return true;
}

bool CCmdDir::Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{

    const std::string& l_str_param = a_vec_args[0]; // 第一个是参数,随后是列表

    if (("/ad"!= l_str_param) && ("/s" != l_str_param))
    {
        a_str_proc_resault = "命令参数错误:";
        a_str_proc_resault += l_str_param;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        LOG_ERR(l_str_param);
        return false;
    }

    bool l_b_all = "/s" == l_str_param;

    std::vector<std::string> l_vec_path = a_vec_args;
    l_vec_path.erase(l_vec_path.begin()); // 去除第一个

    std::map<std::string, std::vector<uint32> > l_map_path;

    for (uint32 i=0; i<l_vec_path.size(); ++i)
    {
        std::string& l_str_path = l_vec_path[i];

        CDealString::PathConver((char*)l_str_path.c_str(), l_str_path.length());


        LOG_RECORD(LOG_INFO, l_str_path);

        std::string l_str_src_full_name;
        if(! m_p_handle_file_sys->GetFullPath(l_str_path,l_str_src_full_name))
        {
            a_str_proc_resault = "GetFullPath err:";
            a_str_proc_resault += l_str_path;
            LOG_RECORD(LOG_ERR,a_str_proc_resault);
            return false;
        }

        SCateNode* l_p_src_cata_node;
        if (! m_p_handle_file_sys->CheckPathExist(l_str_src_full_name, l_p_src_cata_node))
        {
            a_str_proc_resault = "路径不存在:";
            a_str_proc_resault += l_str_src_full_name;
            LOG_RECORD(LOG_ERR,a_str_proc_resault);
            return false;
        }

        if (l_b_all)
        {
            if (! m_p_handle_file_sys->CollectDirTotal(l_str_src_full_name, l_map_path))
            {
                a_str_proc_resault = "收集总子节点信息失败:";
                a_str_proc_resault += l_str_src_full_name;
                LOG_RECORD(LOG_ERR,a_str_proc_resault);
                return false;
            }
        }
        else
        {
            std::vector<uint32> l_vec_path;
            if (! m_p_handle_file_sys->CollectDirBrief(l_str_src_full_name, l_vec_path))
            {
                a_str_proc_resault = "收集下层子节点信息失败:";
                a_str_proc_resault += l_str_src_full_name;
                LOG_RECORD(LOG_ERR,a_str_proc_resault);
                return false;
            }

            if (! l_vec_path.empty())
            {
                l_map_path.insert(std::make_pair(l_str_src_full_name, l_vec_path));
            }
        }
    }

    if (l_map_path.empty())
    {
        a_str_proc_resault = "文件夹为空";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return true;
    }

    // 以下为有文件的情况

    a_str_proc_resault = "files : \n";
    std::map<std::string, std::vector<uint32> >::const_iterator l_iter = l_map_path.begin();

    for (;l_iter != l_map_path.end(); ++l_iter)
    {
        const std::string& l_str_full_name = l_iter->first;
        const std::vector<uint32> l_vec_son = l_iter->second;

        a_str_proc_resault += l_str_full_name;
        a_str_proc_resault += "\n";
        a_str_proc_resault += "-------------------------";;
        a_str_proc_resault += "\n";

        for (uint32 i=0; i<l_vec_son.size(); ++i)
        {
            if(!m_p_handle_file_sys->PrintNode(l_vec_son[i], a_str_proc_resault))
            {
                continue;
            }

            a_str_proc_resault += "\n";
        
        }

        a_str_proc_resault += "\n====================================\n";
    }

    //LOG_RECORD(LOG_DEBUG, a_str_proc_resault);
    return true;
}
