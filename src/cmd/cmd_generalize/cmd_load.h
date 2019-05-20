#ifndef __CMD_LOAD_H__
#define __CMD_LOAD_H__

#include<stdio.h>
#include<stdlib.h>

#include "../cmd_base.h"

class CCmdBase;

class CCmdLoad : public CCmdBase
{
public:
    explicit CCmdLoad(CHandleFileSys* l_p_handle_file_sys);

public:
    virtual bool CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault);
    virtual bool Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault);
};

#endif//__CMD_LOAD_H__