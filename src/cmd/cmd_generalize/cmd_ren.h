#ifndef __CMD_REN_H__
#define __CMD_REN_H__

#include<stdio.h>
#include<stdlib.h>

#include "../cmd_base.h"

class CCmdBase;

//TODO: 此命令与del命令存在交集,不再实现

class CCmdRen : public CCmdBase
{
public:
    explicit CCmdRen(CHandleFileSys* l_p_handle_file_sys);

public:
    virtual bool CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault);
    virtual bool Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault);
};

#endif//__CMD_REN_H__