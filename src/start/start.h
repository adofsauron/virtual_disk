#ifndef __START_H__
#define __START_H__

#include "../base/base.h"
#include "../handle/handle_file_sys.h"
#include "../cmd/cmd_manager.h"
#include "../tools/logger.h"
#include "../client/client_cmd_adaptor.h"
#include "../client/client_io.h"

class CLogger;
class CHandleFileSys;
class CCmdManager;
class CClientCmdAdaptor;
class CClientIO;
class CEnv;

class CStartClass
{
public:
    CStartClass();
    ~CStartClass();

public:
    inline bool InitOver() {return m_b_init_over;};

    // 启动
    bool Start();

private:
    CHandleFileSys*     m_p_hdle_file_sys;
    CCmdManager*        m_p_cmd_manager;
    CClientCmdAdaptor*  m_p_client_cmd_adaptor;
    CClientIO*          m_p_client_io;

private:
    bool                m_b_init_over;
};

#endif//__START_H__
