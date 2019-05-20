#ifndef __CMD_DIR_H__
#define __CMD_DIR_H__

#include <stdio.h>
#include <stdlib.h>

#include <map>

#include "../cmd_base.h"

class CCmdBase;

class CCmdDir : public CCmdBase
{
public:
    explicit CCmdDir(CHandleFileSys* l_p_handle_file_sys);

public:
    virtual bool CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault);
    virtual bool Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault);
};

#endif//__CMD_DIR_H__