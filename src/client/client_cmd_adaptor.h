#ifndef __CLIENT_CMD_ADAPTOR_H__
#define __CLIENT_CMD_ADAPTOR_H__

#include <vector>
#include <string>

#include "../cmd/cmd_manager.h"

class CCmdManager;

class CClientCmdAdaptor
{
public:
    CClientCmdAdaptor(CCmdManager* l_p_cmd_mangaer);
    ~CClientCmdAdaptor();

public:
    inline bool InitOver() {return m_b_init_over;};

public:
    // 处理命令
    bool ProcCmd(const std::vector<std::string>& a_vec_commond, std::string& a_str_proc_resault);

private:
    CCmdManager* m_p_cmd_mangaer;
    bool m_b_init_over;
};

#endif//__CLIENT_CMD_ADAPTOR_H__