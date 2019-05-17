#ifndef __CMD_RD_H__
#define __CMD_RD_H__

#include<stdio.h>
#include<stdlib.h>

#include "../cmd_base.h"

class CCmdBase;

//TODO: 此命令与del命令存在交集,不再实现

class CCmdRd : public CCmdBase
{
private:
    explicit CCmdRd(CHandleFileSys* l_p_handle_file_sys);
    ~CCmdRd();

public:
    virtual bool CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault);
    virtual bool Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault);
};

#endif//__CMD_RD_H__