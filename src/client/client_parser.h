#ifndef __CLIENT_PARSER_H__
#define __CLIENT_PARSER_H__

#include "../base/base.h"
#include "../tools/deal_string.h"

// 客户端命令解析,因为包含了业务相关,所以不放在工具类里
class CClientParser
{
public:
    // 校验输入长度合法性
    static bool CheckInputLength(const std::string& a_str_input);

    // 校验特殊字符
    static bool CheckSpecChar(const std::string& a_str_input);

    // 校验整个输入是否符合规范
    static bool CheckInputAval(const std::string& a_str_input);

    // 校验特殊字符,单个字符
    static bool CheckSpecChar(const char a_chr);

    // 命令全部转换为小写
    static const std::string& ConvertInputToLower(std::string& a_str_input); 

    // 分隔命令进容器
    static std::vector<std::string>& SplitInputToCommond(const std::string& a_str_input, std::vector<std::string>& a_vec_comond);

    // 检测是否需要转换路径
    static bool CheckConvertPath(std::string& a_str_input);

    // 转换路径
    static std::string& ConvertPath();

};

#endif//__CLIENT_PARSER_H__