#ifndef __CMD_EXIT_H__
#define __CMD_EXIT_H__

#include<stdio.h>
#include<stdlib.h>

#include "../cmd_base.h"

// exit在命令处理中，负责文件系统的退出工作

class CCmdBase;

class CCmdExit : public CCmdBase
{
public:
    explicit CCmdExit(CHandleFileSys* l_p_handle_file_sys);
    ~CCmdExit();

public:
    virtual bool CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault);
    virtual bool Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault);
};

#endif//__CMD_EXIT_H__