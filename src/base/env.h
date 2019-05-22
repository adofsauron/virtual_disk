// 环境类

#ifndef __ENV_H__
#define __ENV_H__

#include <string>

#include "base.h"

class CEnv
{
private:
    CEnv();

public:
    static CEnv* Instance();
    static void Realease();

public:
    const std::string& ChgPwd(const std::string& a_str_new_pwd);
    const std::string& GetPwd(std::string& a_str_path);

private:
    std::string m_str_pwd; // 当前路径
    static CEnv* s_p_this;
};

#endif//__ENV_H__