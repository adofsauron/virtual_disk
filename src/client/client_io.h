#ifndef __CLIENT_IO_H__
#define __CLIENT_IO_H__

#include <string>
#include <iostream>
#include <vector>

#include "../base/base.h"
#include "client_cmd_adaptor.h"
#include "client_parser.h"

class CClientCmdAdaptor;
class CClientParser;

class CClientIO
{
public:
    CClientIO(CClientCmdAdaptor* l_p_cmd_adaptor);
    ~CClientIO();

public:
    // 处理客户端输入
    void StartInput();

    // 输入校验
    bool CheckInput();

    // 输入预处理
    bool PreproccessInput();

    // 处理输入指令
    bool ProcessInput();

public:
    inline bool InitOver() {return m_b_init_over;};

private:
    CClientCmdAdaptor* m_p_cmd_adaptor; // 命令处理装饰器
    bool m_b_exit; // 是否退出
    bool m_b_init_over;

private:
    std::string                 m_str_input;    // 输入字符串 
    std::string                 m_str_output;   // 输出字符串
    std::vector<std::string>    m_vec_commond;  // 输入的字符被空格分隔后的容器
    std::string                 m_str_cmd_name; // 命令名字
};

#endif//__CLIENT_IO_H__
