#ifndef __CMD_MANAGER_H__
#define __CMD_MANAGER_H__

#include <map>
#include <string>

#include "../base/base.h"
#include "../handle/handle_file_sys.h"

#include "cmd_base.h"
#include "cmd_generalize/cmd_cd.h"
#include "cmd_generalize/cmd_cls.h"
#include "cmd_generalize/cmd_copy.h"
#include "cmd_generalize/cmd_del.h"
#include "cmd_generalize/cmd_copy.h"
#include "cmd_generalize/cmd_dir.h"
#include "cmd_generalize/cmd_exit.h"
#include "cmd_generalize/cmd_load.h"
#include "cmd_generalize/cmd_md.h"
#include "cmd_generalize/cmd_mklink.h"
#include "cmd_generalize/cmd_move.h"
#include "cmd_generalize/cmd_ren.h"
#include "cmd_generalize/cmd_save.h"

class CHandleFileSys;
class CCmdBase;
class CCmdCD;
class CCmdCls;
class CCmdCopy;
class CCmdDel;
class CCmdCopy;
class CCmdDir;
class CCmdExit;
class CCmdLoad;
class CCmdMd;
class CCmdMklink;
class CCmdMove;
class CCmdRen;
class CCmdSave;

class CCmdManager
{
public:
    explicit CCmdManager(CHandleFileSys* a_p_hdle_file_sys);
    ~CCmdManager();    

public:
    bool& InitOver() {return m_b_init_over;};

     // 执行命令
    bool RunCmd(const std::string& a_str_cmd, const std::vector<std::string>& a_vec_args, std::string& a_str_resault);

private:
    // 命令初始化
    bool InitCmd();

    // 命令注册
    template<typename CLASS>
    bool RegisterCmd();

    // 命令清理
    bool ClearCmd();

    // 找出对应命令,若没有则返回NULL
    CCmdBase* FindCmd(const std::string& a_str_cmd);

    // 根据指令获取指令id, <=0为错误
    int static GetCmdNo(const std::string& a_str_cmd);


private:
    CHandleFileSys*                             m_p_hdle_file_sys;
    bool                                        m_b_init_over;

    CCmdBase*                                   m_vec_cmd[DEF_CMD_NO_MAX];

};

#endif//__CMD_MANAGER_H__
