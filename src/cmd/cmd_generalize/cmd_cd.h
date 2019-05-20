#ifndef __CMD_CD_H__
#define __CMD_CD_H__

#include "../cmd_base.h"
#include "../../base/env.h"

class CCmdBase;
class CEnv;

class CCmdCD : public CCmdBase
{
public:
    explicit CCmdCD(CHandleFileSys* l_p_handle_file_sys);

public:
    virtual bool CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault);
    virtual bool Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault);
};

#endif//__CMD_CD_H__