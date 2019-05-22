#include "cmd_dir.h"

CCmdDir::CCmdDir(CHandleFileSys* l_p_handle_file_sys)
    :CCmdBase(CONST_CMD_DIR, l_p_handle_file_sys)
{
}

bool CCmdDir::CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    return true;
}

bool CCmdDir::Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    std::string l_str_param;//  = a_vec_args[0]; // 第一个是可选参数
    std::vector<std::string> l_vec_path = a_vec_args;

    std::string l_str_cur;
    CEnv::Instance()->GetPwd(l_str_cur);

    if (l_vec_path.empty())
    {
        l_vec_path.push_back("/ad");
        l_vec_path.push_back(l_str_cur);
    }

    if ((("/ad"== l_vec_path[0]) || ("/s" == l_vec_path[0])) && (1 == l_vec_path.size()))
    {
        l_vec_path.push_back(l_str_cur);
    }

    if (("/ad"== l_vec_path[0]) || ("/s" == l_vec_path[0]))
    {
       l_str_param = l_vec_path[0];
       l_vec_path.erase(l_vec_path.begin()); // 第一个参数为控制字符
    }
    else
    {
        l_str_param = "/ad";
    }

    bool l_b_all = "/s" == l_str_param;

    std::unordered_map<std::string, std::vector<uint32> > l_map_path;

    bool l_b_globbing = false;
    for (uint32 i=0; i<l_vec_path.size(); ++i)
    {
        std::string& l_str_path = l_vec_path[i];
        CDealString::PathToIner(l_str_path);
        std::string l_str_dir_full_name;
        if(! m_p_handle_file_sys->GetFullPath(l_str_path,l_str_dir_full_name))
        {
            a_str_proc_resault = "GetFullPath err:";
            a_str_proc_resault += l_str_path;
            LOG_RECORD(LOG_ERR,a_str_proc_resault);
            return false;
        }

        if (CDealString::FindSubStr(l_str_dir_full_name, "?") && CDealString::FindSubStr(l_str_dir_full_name, "*"))
        {
            a_str_proc_resault = "错误:不可同时存在?和*, ";
            a_str_proc_resault += l_str_dir_full_name;
            LOG_RECORD(LOG_ERR,a_str_proc_resault);
            return false;
        }

        // 通配符处理
        if (CDealString::FindSubStr(l_str_dir_full_name, "?") || CDealString::FindSubStr(l_str_dir_full_name, "*"))
        {
            DisposeWildcard(l_str_dir_full_name, a_str_proc_resault);
            l_b_globbing = true;
            continue;
        }

        SCateNode* l_p_src_cata_node = NULL;
        if (! m_p_handle_file_sys->CheckPathExist(l_str_dir_full_name, l_p_src_cata_node))
        {
            a_str_proc_resault = "路径不存在:";
            a_str_proc_resault += l_str_dir_full_name;
            LOG_RECORD(LOG_ERR,a_str_proc_resault);
            return false;
        }

        if (CNODE_DIR != l_p_src_cata_node->m_i_type)
        {
            a_str_proc_resault = "目标文件不是目录:";
            a_str_proc_resault += l_str_dir_full_name;
            LOG_ERR(a_str_proc_resault);
            return false;
        }

        if (l_b_all)
        {
            if (! m_p_handle_file_sys->CollectDirTotal(l_str_dir_full_name, l_map_path))
            {
                a_str_proc_resault = "收集总子节点信息失败:";
                a_str_proc_resault += l_str_dir_full_name;
                LOG_RECORD(LOG_ERR,a_str_proc_resault);
                return false;
            }
        }
        else
        {
            // 目录下的文件
            std::vector<uint32> l_vec_path;
            if (! m_p_handle_file_sys->CollectDirBrief(l_str_dir_full_name, l_vec_path))
            {
                a_str_proc_resault = "收集下层子节点信息失败:";
                a_str_proc_resault += l_str_dir_full_name;
                LOG_RECORD(LOG_ERR,a_str_proc_resault);
                return false;
            }

            if (! l_vec_path.empty())
            {
                l_map_path.insert(std::make_pair(l_str_dir_full_name, l_vec_path));
            }
        }
    }

    if (l_b_globbing)
    {
        return true;
    }

    if (l_map_path.empty())
    {
        a_str_proc_resault = "文件夹为空";
        LOG_RECORD(LOG_INFO,a_str_proc_resault);
        return true;
    }

    // 以下为有文件的情况

    a_str_proc_resault = "\n\n";
    std::unordered_map<std::string, std::vector<uint32> >::const_iterator l_iter = l_map_path.begin();

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


bool CCmdDir::DisposeWildcard(const std::string& a_str_prex, std::string& a_str_proc_resault)
{
    if (a_str_prex.empty())
    {
        a_str_proc_resault = "错误:路径不可为空";
        LOG_ERR(a_str_proc_resault);
        return false;
    }

    if ((CDealString::FindSubStr(a_str_prex, "?") && (CDealString::FindSubStr(a_str_prex, "*"))))
    {
        a_str_proc_resault = "路径不可同时存在?和*, =>";
        a_str_proc_resault += a_str_prex;
        LOG_ERR(a_str_proc_resault);
        return false;
    }

     std::string l_str_only_name;
    if (! m_p_handle_file_sys->GetOnlyNameByFullName(a_str_prex, l_str_only_name))
    {
        a_str_proc_resault = "GetOnlyNameByFullName err";
        a_str_proc_resault += a_str_prex;
        LOG_ERR(a_str_proc_resault);
        return false;
    }

    std::string l_str_father_full_name;
    if (! m_p_handle_file_sys->GetFatherNameByFullName(a_str_prex, l_str_father_full_name))
    {
        a_str_proc_resault = "GetFatherNameByFullName err";
        a_str_proc_resault += a_str_prex;
        LOG_ERR(a_str_proc_resault);
        return false;
    }

    SCateNode* l_p_node = NULL;
    if (! m_p_handle_file_sys->CheckPathExist(l_str_father_full_name, l_p_node)) // 路径不存在
    {
        a_str_proc_resault = "l_str_father_full_name not exist";
        a_str_proc_resault += l_str_father_full_name;
        LOG_ERR(a_str_proc_resault);
        return false;
    }


    std::string l_str_sub = l_str_only_name;
    if ((CDealString::FindSubStr(l_str_only_name, "*")))
    {
        l_str_sub.erase(l_str_sub.find("*"));
    }
    
    if (NULL == l_p_node)
    {
        a_str_proc_resault = "文件节点获取失败";
        a_str_proc_resault += a_str_prex;
        LOG_ERR(a_str_proc_resault);
        return false;
    }


    if (CNODE_DIR != l_p_node->m_i_type)
    {
        a_str_proc_resault = "目标文件不是目录:";
        a_str_proc_resault += a_str_prex;
        LOG_ERR(a_str_proc_resault);
        return false;
    }

    if (0 >= l_p_node->m_i_son_num)
    {
        a_str_proc_resault = "目录为空";
        a_str_proc_resault += a_str_prex;
        LOG_ERR(a_str_proc_resault);
    }

    if ((0 < l_p_node->m_i_son_num) && (NULL == l_p_node->m_p_son_set))
    {
        a_str_proc_resault = "目录节点出错";
        a_str_proc_resault += a_str_prex;
        LOG_ERR(a_str_proc_resault);
    }

    std::vector<uint32>  l_o_vec_son;
    for (uint32 i=0; i<l_p_node->m_i_son_num; ++i)
    {
        const uint32 l_i_son = l_p_node->m_p_son_set[i];
        SCateNode* l_p_son = NULL;

        if (! m_p_handle_file_sys->GetCateNodeById(l_i_son, l_p_son))
        {
            LOG_ERR("GetCateNodeById err");
            LOG_ERR(l_i_son);
            continue;
        }

        const std::string l_str_name = l_p_son->m_p_name; // 短名字

        // 不可混用，之前已经过校验
        if (CDealString::FindSubStr(l_str_only_name, "?"))
        {
            if (CDealString::CheckRegexSin(l_str_only_name, '?', l_str_name))
            {
                l_o_vec_son.push_back(l_i_son);
            }
        }

        if (CDealString::FindSubStr(l_str_only_name, "*"))
        {
            std::string l_str_sub = l_str_only_name;
            l_str_sub.erase(l_str_sub.find("*"));
    
            if (CDealString::FindSubStr(l_str_name, l_str_sub))
            {
                l_o_vec_son.push_back(l_i_son);
            }
        }
    }

    if (l_o_vec_son.empty())
    {
        a_str_proc_resault = "文件夹为空";
        LOG_INFO(a_str_proc_resault);
        return true;
    }

    for (uint32 i=0; i<l_o_vec_son.size(); ++i)
    {
        if(!m_p_handle_file_sys->PrintNode(l_o_vec_son[i], a_str_proc_resault))
        {
            continue;
        }

        a_str_proc_resault += "\n";    
    }

    //LOG_RECORD(LOG_DEBUG, a_str_proc_resault);
    return true;
}