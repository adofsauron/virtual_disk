#ifndef __CMD_MOVE_H__
#define __CMD_MOVE_H__

#include<stdio.h>
#include<stdlib.h>

#include "../cmd_base.h"

class CCmdBase;

class CCmdMove : public CCmdBase
{
public:
    explicit CCmdMove(CHandleFileSys* l_p_handle_file_sys);

public:
    virtual bool CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault);
    virtual bool Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault);
};

#endif//__CMD_MOVE_H__