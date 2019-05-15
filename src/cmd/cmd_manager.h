#ifndef __CMD_MANAGER_H__
#define __CMD_MANAGER_H__

#include <map>

#include "../base/base.h"
#include "../handle/handle_file_sys.h"
#include "cmd_base.h"
#include "cmd_generalize/cmd_cd.h"

// 前置声明
class CHandleFileSys;

class CCmdBase;
class CCmdCD;

class CCmdManager
{
public:
    explicit CCmdManager(CHandleFileSys* a_p_hdle_file_sys);
    ~CCmdManager();    

public:
    inline bool InitOver() {return m_b_init_over;};

private:
    // 命令初始化
    bool InitCmd();

    // 命令注册
    bool RegisterCmd(const char* l_p_key, CCmdBase* l_p_cmd);

    // 命令清理
    bool ClearCmd();

private:
    std::map<const char*, CCmdBase*>    m_map_cmd;
    CHandleFileSys*                     m_p_hdle_file_sys;
    bool                                m_b_init_over;

};

#endif//__CMD_MANAGER_H__