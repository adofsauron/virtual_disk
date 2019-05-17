#include "start.h"

CStartClass::CStartClass()
{
    m_b_init_over = false;

    if (! CLogger::Instance()->InitOver())
    {
        LOG_RECORD(LOG_ERR,"日志模块启动失败" );
        return;
    }

    CEnv::Instance();

    LOG_RECORD(LOG_DEBUG,"日志启动" );

    m_p_hdle_file_sys = new CHandleFileSys();
    if (! m_p_hdle_file_sys->InitOver())
    {
        LOG_RECORD(LOG_ERR,"m_p_hdle_file_sys init err" );
        return;
    }

    LOG_RECORD(LOG_DEBUG,"m_p_hdle_file_sys ini over" );
    m_p_cmd_manager = new CCmdManager(m_p_hdle_file_sys);
    LOG_RECORD(LOG_DEBUG,"CCmdManager init over" );

    m_p_client_cmd_adaptor = new CClientCmdAdaptor(m_p_cmd_manager);
    if (! m_p_client_cmd_adaptor->InitOver())
    {
        LOG_RECORD(LOG_ERR,"m_p_client_cmd_adaptor init err" );
        return;
    }

    LOG_RECORD(LOG_DEBUG,"m_p_client_cmd_adaptor init over" );

    m_p_client_io = new CClientIO(m_p_client_cmd_adaptor);
    if (! m_p_client_io->InitOver())
    {
        LOG_RECORD(LOG_ERR,"m_p_client_io init err" );
        return;
    }

    LOG_RECORD(LOG_DEBUG,"CClientIO init over" );

    m_b_init_over = true;
}

CStartClass::~CStartClass()
{    
    DELETE_PTR(m_p_hdle_file_sys);
    DELETE_PTR(m_p_cmd_manager);
    DELETE_PTR(m_p_client_cmd_adaptor);
    DELETE_PTR(m_p_client_io);
    
    CLogger::ReleaseInstance();
}

bool CStartClass::Start()
{
    if (! InitOver())
    {
        return false;
    }

    // 启动
    m_p_client_io->StartInput();

    LOG_RECORD(LOG_INFO, "start over");
    return true;
}

int main(int argc, char* argv[])
{
    CStartClass l_o_start;

    if (! l_o_start.InitOver())
    {
        LOG_RECORD(LOG_ERR,"sys init err" );
        return -1;
    }

    LOG_RECORD(LOG_INFO, "sys init over" );
    
    l_o_start.Start();

    LOG_RECORD(LOG_INFO, "sys eixt");
	return 0;
}
