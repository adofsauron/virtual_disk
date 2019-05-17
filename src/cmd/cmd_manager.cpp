#include "cmd_manager.h"

CCmdManager::CCmdManager(CHandleFileSys* a_p_hdle_file_sys)
{
    m_b_init_over = false;
    if (NULL == a_p_hdle_file_sys)
    {
        return;
    }

    m_p_hdle_file_sys = a_p_hdle_file_sys;

    if (! InitCmd())
    {
        return;
    }

    m_b_init_over = true;

    LOG_INFO("CCmdManager construt over");
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
        std::map<const std::string, CCmdBase*>::iterator l_iter_cmd = this->m_map_cmd.begin();
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

    bool l_b_reg_over = true;

    l_b_reg_over = (l_b_reg_over && RegisterCmd<CCmdCD>());
    l_b_reg_over = (l_b_reg_over && RegisterCmd<CCmdCls>());
    l_b_reg_over = (l_b_reg_over && RegisterCmd<CCmdCopy>());
    l_b_reg_over = (l_b_reg_over && RegisterCmd<CCmdDel>());
    l_b_reg_over = (l_b_reg_over && RegisterCmd<CCmdDir>());
    l_b_reg_over = (l_b_reg_over && RegisterCmd<CCmdExit>());
    l_b_reg_over = (l_b_reg_over && RegisterCmd<CCmdLoad>());
    l_b_reg_over = (l_b_reg_over && RegisterCmd<CCmdMd>());
    l_b_reg_over = (l_b_reg_over && RegisterCmd<CCmdMklink>());
    l_b_reg_over = (l_b_reg_over && RegisterCmd<CCmdMove>());
    l_b_reg_over = (l_b_reg_over && RegisterCmd<CCmdRen>());
    l_b_reg_over = (l_b_reg_over && RegisterCmd<CCmdSave>());

    return l_b_reg_over;
}

template<typename CLASS>
bool CCmdManager::RegisterCmd()
{
    if (NULL == m_p_hdle_file_sys)
    {
        return false;
    }

    CCmdBase* l_p_cmd = new CLASS(m_p_hdle_file_sys);

    if (NULL == l_p_cmd)
    {
        return false;
    }

    const std::string& l_str_cmd_name = l_p_cmd->GetCmdName();

    if (l_str_cmd_name.empty())
    {
        LOG_ERR("l_str_cmd_name.empty()");
        return false;
    }

    // 若已存在,则拒绝插入. 防止手误写俩一样的命令
    std::map<std::string, CCmdBase*>::iterator l_iter_cmd = this->m_map_cmd.find(l_str_cmd_name);
    if (l_iter_cmd != this->m_map_cmd.end())
    {
        std::string log = "cmd exit :";
        log += l_str_cmd_name;
        LOG_ERR(log);
        return false;
    }

    this->m_map_cmd.insert(std::make_pair(l_str_cmd_name, l_p_cmd));

    std::string log = "cmd register :\t";
    log += l_str_cmd_name;
    LOG_INFO(log);

    return true;
}

CCmdBase* CCmdManager::FindCmd(const std::string& a_str_cmd)
{
    std::map<const std::string, CCmdBase*>::iterator l_iter_cmd = this->m_map_cmd.find(a_str_cmd);
    if (l_iter_cmd == this->m_map_cmd.end())
    {
        return NULL;
    }

    return l_iter_cmd->second;
}
