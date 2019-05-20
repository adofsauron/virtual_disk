#ifndef __CMD_BASE_H__
#define __CMD_BASE_H__

#include <string>
#include <vector>

#include "../base/base.h"
#include "../tools/logger.h"
#include "../handle/handle_file_sys.h"

class CHandleFileSys;

class CCmdBase
{
public:
    CCmdBase(const std::string& l_str_cmd_name, CHandleFileSys* l_p_handle_file_sys);
    virtual ~CCmdBase(){};

public:
    
    // 参数合法性校验
    virtual bool CheckFeasibility(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault) = 0;

    // 业务相关处理
    virtual bool Dispose(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault) = 0;

public:
    bool Execute(const std::vector<std::string>& a_vec_args, std::string& a_str_proc_resault);

public:
    inline const std::string& GetCmdName() {return m_p_cmd_name;};

protected:
    const std::string m_p_cmd_name;               // 命令名字
    CHandleFileSys* m_p_handle_file_sys;    // 操作的门面类
};

#endif//__CMD_BASE_H__
