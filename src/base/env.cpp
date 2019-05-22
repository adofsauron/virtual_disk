#include "env.h"

CEnv* CEnv::s_p_this = NULL;

CEnv::CEnv()
    :m_str_pwd("/")
{
}

CEnv* CEnv::Instance()
{
    if (NULL == s_p_this)
    {
        s_p_this = new CEnv();
    }

    return s_p_this;
}

void CEnv::Realease()
{
    DELETE_PTR(s_p_this);
}

const std::string& CEnv::ChgPwd(const std::string& a_str_new_pwd)
{
    m_str_pwd = a_str_new_pwd;
    return m_str_pwd;
}

const std::string& CEnv::GetPwd(std::string& a_str_path)
{
    a_str_path = m_str_pwd;
    return m_str_pwd;
}
