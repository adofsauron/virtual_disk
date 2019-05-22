#include "client_io.h"

CClientIO::CClientIO(CClientCmdAdaptor* l_p_cmd_adaptor)
{
    m_b_init_over = false;
    if (NULL == l_p_cmd_adaptor)
    {
        return;
    }

    m_p_cmd_adaptor = l_p_cmd_adaptor;

    m_b_exit = false;
    m_b_init_over = true;
}

CClientIO::~CClientIO()
{
}

void CClientIO::StartInput()
{
    while(!m_b_exit)
    {
        m_str_input.clear();
        m_str_output.clear();
        m_vec_commond.clear();

        std::string l_str_cur;
        CEnv::Instance()->GetPwd(l_str_cur);


        std::cout << "virtual_disk ["<<l_str_cur<<"] # ";
        std::getline(std::cin, m_str_input);

        if (m_str_input.empty())
        {
            continue;
        }

        std::cout << "原始输入:" << m_str_input << std::endl;

        // 命令校验
        if(! CheckInput())
        {
            std::cout << m_str_output << std::endl;
            continue;
        }

        // 命令预处理
        if (! PreproccessInput())
        {
            std::cout << m_str_output << std::endl;
            continue;
        }

        // 命令处理
        if (! ProcessInput())
        {
            std::cout << m_str_output << std::endl;
            continue;
        }

        // 输出处理结果
        std::cout << m_str_output << std::endl;
    }

    LOG_RECORD(LOG_INFO, "StartInput over");
}

bool CClientIO::CheckInput()
{
    if (! CClientParser::CheckInputLength(m_str_input))
    {
        m_str_output = "输入命令长度错误";
        return false;
    }

    if (! CClientParser::CheckInputAval(m_str_input))
    {
        m_str_output = "输入非法,请重新输入";
        return false;
    }

    return true;
}

// 仅预处理, 不涉及内部业务
bool CClientIO::PreproccessInput()
{
    // 全部小写
    CClientParser::ConvertInputToLower(m_str_input);

    m_vec_commond.clear();
    CClientParser::SplitInputToCommond(m_str_input, m_vec_commond);

    if (m_vec_commond.empty())
    {
        m_str_output = "输入命令为空,请重新输入";
        return false;
    }

    m_str_cmd_name = m_vec_commond[0];

    if (m_str_cmd_name.empty())
    {
        m_str_output = "输入指令为空,请重新输入";
        return false;
    }

    return true;
}

bool CClientIO::ProcessInput()
{
    if (NULL == m_p_cmd_adaptor)
    {
        m_str_output = "CClientCmdAdaptor未初始化";
        return false;
    }

    if (! m_p_cmd_adaptor->ProcCmd(m_vec_commond, m_str_output))
    {
        return false;
    }

    // 只有命令模块正确处理完成后,才可设置退出标识
    if ("exit" == m_str_cmd_name)
    {
        m_b_exit = true;
    }

    return true;
}
