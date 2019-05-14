#include "cmd_manager.h"

CCmdManager::CCmdManager()
{
    m_p_hdle_catelog    = new CHandleCatelog();
    m_p_hdle_aval_disk  = new CHandleAvaliableDisk();
    m_p_hdle_file_sys   = new CHandleFileSys(m_p_hdle_catelog, m_p_hdle_aval_disk);
}

// RAII
CCmdManager::~CCmdManager()
{
    this->ClearCmd();
    DELETE_PTR(m_p_hdle_file_sys);
}

bool CCmdManager::ClearCmd()
{
    if (!this->m_map_cmd.empty())
    {
        std::map<const char*, CCmdBase*>::iterator l_iter_cmd = this->m_map_cmd.begin();
        for (; l_iter_cmd !=  this->m_map_cmd.end(); ++l_iter_cmd)
        {
            DELETE_PTR(l_iter_cmd->second);
        }

        this->m_map_cmd.clear();
    }

    return true;
}


bool CCmdManager::InitCmd()
{
    this->ClearCmd();

    if (NULL == m_p_hdle_file_sys)
    {
        return false;
    }

    if(!RegisterCmd("cd", new CCmdCD("cd", m_p_hdle_file_sys)))
    {
        return false;
    }


    return true;
}

bool CCmdManager::RegisterCmd(const char* l_p_key, CCmdBase* l_p_cmd)
{
    if ((NULL == l_p_key) || (NULL == l_p_cmd))
    {
        return false;
    }

    std::map<const char*, CCmdBase*>::iterator l_iter_cmd = this->m_map_cmd.find(l_p_key);
    if (l_iter_cmd != this->m_map_cmd.end())
    {
        DELETE_PTR(l_iter_cmd->second);
        this->m_map_cmd.erase(l_iter_cmd);
    }

    this->m_map_cmd.insert(std::make_pair(l_p_key, l_p_cmd));
    return true;
}