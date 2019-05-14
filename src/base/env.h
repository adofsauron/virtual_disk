// 环境类

#ifndef __ENV_H__
#define __ENV_H__

#include <string>

class CEnv
{
public:
    CEnv();
    ~CEnv();

public:
    const std::string& ChgPwd(const std::string& l_str_new_pwd);
    const std::string& GetPwd();

private:
    std::string m_str_pwd; // 当前路径
};

#endif//__ENV_H__