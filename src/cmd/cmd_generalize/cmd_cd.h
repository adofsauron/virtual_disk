#ifndef __CMD_CD_H__
#define __CMD_CD_H__

#include "../cmd_base.h"

class CCmdCD : public CCmdBase
{
public:
    explicit CCmdCD(const char* l_p_cmd_name, CHandleFileSys* l_p_handle_file_sys);
    ~CCmdCD();

public:
    virtual bool CheckFeasibility();
    virtual bool Dispose();
};

#endif//__CMD_CD_H__