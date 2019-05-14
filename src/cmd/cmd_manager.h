#ifndef __CMD_MANAGER_H__
#define __CMD_MANAGER_H__

#include <map>

#include "../handle/handle_file_sys.h"
#include "../handle/handle_catalog.h"
#include "../handle/handle_available_disk.h"

#include "cmd_base.h"
#include "cmd_generalize/cmd_cd.h"

class CHandleFileSys;
class CHandleCatelog;
class CHandleAvaliableDisk;


class CCmdBase;
class CCmdCD;

class CCmdManager
{
public:
    CCmdManager();
    ~CCmdManager();    

public:
    bool InitCmd();
    bool RegisterCmd(const char* l_p_key, CCmdBase* l_p_cmd);
    bool ClearCmd();

private:
    std::map<const char*, CCmdBase*>    m_map_cmd;
    CHandleFileSys*                     m_p_hdle_file_sys;
    CHandleCatelog*                     m_p_hdle_catelog;
    CHandleAvaliableDisk*               m_p_hdle_aval_disk;


};

#endif//__CMD_MANAGER_H__