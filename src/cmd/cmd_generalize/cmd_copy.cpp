#include "cmd_copy.h"

CCmdCopy::CCmdCopy(CHandleFileSys* l_p_handle_file_sys)
    :CCmdBase(DEF_CMD_COPY, l_p_handle_file_sys)
{
}

CCmdCopy::~CCmdCopy()
{
}


bool CCmdCopy::CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    const uint32 l_i_args_size = a_vec_args.size();
    if (2 != l_i_args_size)
    {
        a_str_proc_resault = "参数个数不是2个，输入参数个数为:";
        a_str_proc_resault += l_i_args_size;

        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    if (a_vec_args[0].empty() || a_vec_args[1].empty())
    {
        a_str_proc_resault = "参数为空";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    if ( (config::CONST_MAX_SIZE_FILE_NAME <= a_vec_args[0].length()) || (config::CONST_MAX_SIZE_FILE_NAME <= a_vec_args[1].length()) )
    {
        a_str_proc_resault = "路径过长";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    return true;
}

bool CCmdCopy::Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault)
{
    if (NULL == m_p_handle_file_sys)
    {
        a_str_proc_resault = "文件系统未初始化";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    // 注意: 在创建文件前必须先对可行性做严格的检测,否则可能创建出多余的空文件

    std::string l_str_src_file = a_vec_args[0];
    const std::string& l_str_dst_file = a_vec_args[1];

    if (l_str_src_file.empty() || l_str_dst_file.empty())
    {
        a_str_proc_resault = "路径为空: src=";
        a_str_proc_resault += l_str_src_file;
        a_str_proc_resault += ",dst=";
        a_str_proc_resault += l_str_dst_file;

        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    if ('@' == l_str_dst_file[0])
    {
        a_str_proc_resault = "目标文件为物理路径,暂不支持:";
        a_str_proc_resault += l_str_dst_file;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    // 源文件的大小
    uint64 l_i_src_file_size = 0;
    uint64 l_i_src_file_index = 0; // 源文件的索引下标,只在逻辑磁盘使用

    // 源文件是否是真实的路径
    bool l_b_real_disk_file = '@' == l_str_src_file[0];

    // 源文件如果是真实的路径下的文件，则拷贝进虚拟磁盘
    if (l_b_real_disk_file)
    {
        l_str_src_file.erase(l_str_src_file.begin()); // 去掉物理磁盘标识符号@

        // 检测文件是否存在
        if(! CDealFile::CheckFileExist(l_str_src_file.c_str()))
        {
            a_str_proc_resault = "源文件不存在:";
            a_str_proc_resault += l_str_src_file;
            LOG_RECORD(LOG_ERR,a_str_proc_resault);
            return false;
        }

        // 文件大小
        if (! CDealFile::FileGetSize(l_str_src_file.c_str(), l_i_src_file_size))
        {
            a_str_proc_resault = "获取源文件大小失败:";
            a_str_proc_resault += l_str_src_file;
            LOG_RECORD(LOG_ERR,a_str_proc_resault);
            return false;
        }
    }
    else // 虚拟磁盘
    {

        // 路径转换
        CDealString::PathConver((char*)l_str_src_file.c_str(), l_str_src_file.length());

        std::string l_str_src_full_name;
        if(! m_p_handle_file_sys->GetFullPath(l_str_src_file,l_str_src_full_name))
        {
            a_str_proc_resault = "获取源文件的绝对路径失败:";
            a_str_proc_resault += l_str_src_file;
            LOG_RECORD(LOG_ERR,a_str_proc_resault);
            return false;
        }

        SCateNode l_o_src_cata_node;
        if (! m_p_handle_file_sys->CheckPathExist(l_str_src_full_name, l_o_src_cata_node))
        {
            a_str_proc_resault = "源文件不存在:";
            a_str_proc_resault += l_str_src_file;
            LOG_RECORD(LOG_ERR,a_str_proc_resault);
            return false;
        }

        l_i_src_file_size = l_o_src_cata_node.m_i_file_size;
        l_i_src_file_index = l_o_src_cata_node.m_i_disk_index;
    }
    
    // 可用空间大小检测
    uint64 l_aval_size = 0;
    if (! m_p_handle_file_sys->GetAvalSpace(l_aval_size))
    {
        a_str_proc_resault = "获取磁盘可用空间失败";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    // 空间不足
    if (l_i_src_file_size > l_aval_size)
    {
        a_str_proc_resault = "可用空间不足,无法拷贝文件,需要:";
        a_str_proc_resault += l_i_src_file_size;
        a_str_proc_resault += ",可用空间大小：";
        a_str_proc_resault += l_aval_size;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    // 目标文件
    std::string l_str_dst_full_name;
    if(! m_p_handle_file_sys->GetFullPath(l_str_dst_file,l_str_dst_full_name))
    {
        a_str_proc_resault = "无法找出目标路径: dst=";
        a_str_proc_resault += l_str_dst_file;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

     SCateNode l_o_cata_node;
     if (m_p_handle_file_sys->CheckPathExist(l_str_dst_full_name, l_o_cata_node))
     {
        a_str_proc_resault = "目标文件已存在,不可复制,dst=";
        a_str_proc_resault += l_str_dst_full_name;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    // 先创建,再向目录节点中插入数据
    SCateNode* l_p_new_node = NULL;
    if (! m_p_handle_file_sys->AddNewFile(l_str_dst_full_name, CNODE_FILE, (ACCESS_WRITE|ACCESS_RREAD), l_p_new_node))
    {
        return false;
    }

    if (NULL == l_p_new_node)
    {
        a_str_proc_resault = "文件节点创建失败";
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }


    // 以下为开始占用磁盘空间, 前提是源文件不是空文件

    if (l_i_src_file_size <= 0) // 不需要拷贝
    {
        a_str_proc_resault = "拷贝完成,源文件为空，新文件为:";
        a_str_proc_resault += l_str_dst_full_name;
        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return true;
    }

    // TODO: 以下处理如果失败, 则需要将已经创建的虚拟磁盘文件删除

    // 开辟新的磁盘空间
    byte* l_p_writ_space = NULL;
    uint64 l_i_write_index = 0;
    if (! m_p_handle_file_sys->ApplayAvalSpace(l_i_src_file_size, l_p_writ_space, l_i_write_index))
    {
        a_str_proc_resault = "申请磁盘空间失败";

        if (!m_p_handle_file_sys->DelFile(l_str_dst_full_name))
        {
            a_str_proc_resault += ",后续回滚操作失败";
        }


        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    if (NULL == l_p_writ_space)
    {
        a_str_proc_resault = "申请磁盘空间失败";

        if (!m_p_handle_file_sys->DelFile(l_str_dst_full_name))
        {
            a_str_proc_resault += ",后续回滚操作失败";
        }


        LOG_RECORD(LOG_ERR,a_str_proc_resault);
        return false;
    }

    // 设置节点占用空间信息,后续释放节点，就释放掉占用的资源
    l_p_new_node->m_i_disk_index = l_i_write_index;
    l_p_new_node->m_i_file_size = l_i_src_file_size;

    // 开始向可用磁盘写入数据
    if (l_b_real_disk_file) // 物理磁盘
    {
        FILE* l_p_file = NULL;
        if(! CDealFile::FileOpen(l_str_src_file.c_str(), "rw+", l_p_file))
        {
            a_str_proc_resault = "无法打开源文件,";
            a_str_proc_resault += l_str_src_file;
            
            if (!m_p_handle_file_sys->DelFile(l_str_dst_full_name))
            {
                a_str_proc_resault += ",后续回滚操作失败";
            }

            LOG_RECORD(LOG_ERR,a_str_proc_resault);
            return false;
        }

        if (! CDealFile::FileRead(l_p_writ_space,l_i_src_file_size, 1, l_i_src_file_size, l_p_file))
        {
            a_str_proc_resault = "无法读取源文件,";
            a_str_proc_resault += l_str_src_file;
            
            if (!m_p_handle_file_sys->DelFile(l_str_dst_full_name))
            {
                a_str_proc_resault += ",后续回滚操作失败";
            }

            LOG_RECORD(LOG_ERR,a_str_proc_resault);
            return false;
        }
    }
    else // 逻辑节点
    {
        byte* l_p_space = NULL;
        if (! m_p_handle_file_sys->GetAvalSpaceByIndex(l_i_src_file_index, l_p_space))
        {
            a_str_proc_resault = "无法读取源文件,";
            a_str_proc_resault += l_str_src_file;
            
            if (!m_p_handle_file_sys->DelFile(l_str_dst_full_name))
            {
                a_str_proc_resault += ",后续回滚操作失败";
            }

            LOG_RECORD(LOG_ERR,a_str_proc_resault);
            return false;
        }

        // 逻辑磁盘直接内存拷贝
        memcpy(l_p_writ_space, l_p_space, l_i_src_file_size);
    }
    

    a_str_proc_resault = "文件拷贝完成,目标文件:";
    a_str_proc_resault += l_str_dst_full_name;
    LOG_RECORD(LOG_ERR,a_str_proc_resault);

    return true;
}

