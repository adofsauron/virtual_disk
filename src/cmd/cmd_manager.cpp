#include "cmd_manager.h"

CCmdManager::CCmdManager(CHandleFileSys* a_p_hdle_file_sys)
{
    m_b_init_over = false;

    if (NULL == a_p_hdle_file_sys)
    {
        return;
    }
    m_p_hdle_file_sys = a_p_hdle_file_sys;

    // 初始化
    for (uint32 i=ENUM_CMD_NO_CD; i<ENUM_CMD_NO_MAX; ++i)
    {
        m_vec_cmd[i] = NULL;
    }

    if (! InitCmd())
    {
        return;
    }

    m_b_init_over = true;

    LOG_INFO("CCmdManager construt over");
}

CCmdManager::~CCmdManager()
{
    this->ClearCmd();
    m_p_hdle_file_sys = NULL;
    m_b_init_over = false;

    LOG_DEBUG("CCmdManager des");
}

bool CCmdManager::ClearCmd()
{   
    for (uint32 i=ENUM_CMD_NO_CD; i<ENUM_CMD_NO_MAX; ++i)
    {
        DELETE_PTR(m_vec_cmd[i]);
        m_vec_cmd[i] = NULL;
    }

    LOG_DEBUG("cmd map clear");
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
    l_b_reg_over = (l_b_reg_over && RegisterCmd<CCmdHelp>());


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

    int l_i_no = GetCmdNo(l_str_cmd_name);
    if ((l_i_no <= ENUM_CMD_NO_INVL) || (l_i_no >= ENUM_CMD_NO_MAX))
    {
        std::cout << l_i_no << std::endl;
        LOG_ERR("cmd no err");
        return false;
    }
    
    if (NULL != m_vec_cmd[l_i_no])
    {
        LOG_ERR("cmd exist");
        return false;
    }

    m_vec_cmd[l_i_no] = l_p_cmd;

    std::string log = "cmd register :\t";
    log += l_str_cmd_name;
    LOG_INFO(log);

    return true;
}

// 执行命令
bool CCmdManager::RunCmd(const std::string& a_str_cmd, const std::vector<std::string>& a_vec_args, std::string& a_str_resault)
{
    if (a_str_cmd.empty())
    {
        a_str_resault = "cmd empty";
        return false;
    }

    int l_i_no = GetCmdNo(a_str_cmd);
    if ((l_i_no <= ENUM_CMD_NO_INVL) || (l_i_no >= ENUM_CMD_NO_MAX))
    {
        a_str_resault = "没有该命令:";
        a_str_resault += a_str_cmd;
        LOG_ERR("a_str_resault");
        return false;
    }

    if (NULL == m_vec_cmd[l_i_no])
    {
        a_str_resault = "cmd not find";
        LOG_ERR("m_vec_cmd[l_i_no] is null");
        return false;
    }

    return m_vec_cmd[l_i_no]->Execute(a_vec_args, a_str_resault);
}

int CCmdManager::GetCmdNo(const std::string& a_str_cmd)
{
    if(CONST_CMD_CD       == a_str_cmd) return ENUM_CMD_NO_CD	   ;
    if(CONST_CMD_CLS      == a_str_cmd) return ENUM_CMD_NO_CLS     ;
    if(CONST_CMD_COPY     == a_str_cmd) return ENUM_CMD_NO_COPY    ;
    if(CONST_CMD_DEL      == a_str_cmd) return ENUM_CMD_NO_DEL     ;
    if(CONST_CMD_DIR      == a_str_cmd) return ENUM_CMD_NO_DIR     ;
    if(CONST_CMD_EXIT     == a_str_cmd) return ENUM_CMD_NO_EXIT    ;
    if(CONST_CMD_LOAD     == a_str_cmd) return ENUM_CMD_NO_LOAD    ;
    if(CONST_CMD_MD       == a_str_cmd) return ENUM_CMD_NO_MD      ;
    if(CONST_CMD_MKLINK   == a_str_cmd) return ENUM_CMD_NO_MKLINK  ;
    if(CONST_CMD_MOVE     == a_str_cmd) return ENUM_CMD_NO_MOVE    ;
    if(CONST_CMD_REN      == a_str_cmd) return ENUM_CMD_NO_REN     ;
    if(CONST_CMD_SAVE     == a_str_cmd) return ENUM_CMD_NO_SAVE    ;
    if(CONST_CMD_HELP     == a_str_cmd) return ENUM_CMD_NO_HELP    ;

    return ENUM_CMD_NO_INVL;
}
