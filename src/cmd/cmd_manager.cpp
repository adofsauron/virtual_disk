#include "cmd_manager.h"

CCmdManager::CCmdManager(CHandleFileSys* a_p_hdle_file_sys)
{
    m_b_init_over = false;
    if (NULL == a_p_hdle_file_sys)
    {
        return;
    }

    if (!a_p_hdle_file_sys->InitOver())
    {
        return;
    }

    m_p_hdle_file_sys = a_p_hdle_file_sys;

    if (!InitCmd())
    {
        return;
    }

    m_b_init_over = true;
}

// RAII
CCmdManager::~CCmdManager()
{
    this->ClearCmd();
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