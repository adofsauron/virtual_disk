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

     // 找出对应命令,若没有则返回NULL,调用方决定处理策略
    CCmdBase* FindCmd(const std::string& a_str_cmd);

private:
    // 命令初始化
    bool InitCmd();

    // 命令注册
    template<typename CLASS>
    bool RegisterCmd();

    // 命令清理
    bool ClearCmd();

private:
    std::map<const std::string, CCmdBase*>      m_map_cmd; // 初始化时建立,不会动态吧变化,不需要用哈希
    CHandleFileSys*                             m_p_hdle_file_sys;
    bool                                        m_b_init_over;

};

#endif//__CMD_MANAGER_H__