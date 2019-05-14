#ifndef __CMD_BASE_H__
#define __CMD_BASE_H__

#include "../base/base.h"
#include "../handle/handle_file_sys.h"

class CHandleFileSys;

class CCmdBase
{
public:
    explicit CCmdBase(const char* l_p_cmd_name, CHandleFileSys* l_p_handle_file_sys);
    virtual ~CCmdBase();

public:
    virtual bool CheckFeasibility() = 0;
    virtual bool Dispose() = 0;

public:
    bool Execute();

protected:
    const char* m_p_cmd_name;               // 命令,字符串常亮,故用指针
    CHandleFileSys* m_p_handle_file_sys;    // 操作的门面类
};

#endif//__CMD_BASE_H__