#include "client_parser.h"

bool CClientParser::CheckInputLength(const std::string& a_str_input)
{
    if (a_str_input.empty())
    {
        return false;
    }

    if (a_str_input.length() > config::CONST_MAX_SIZE_INPUT)
    {
        return false;
    }

    return true;
}

bool CClientParser::CheckSpecChar(const char a_chr)
{
    bool l_b_aval = false;
    // TODO: 这些字符是业务支持的,如业务变动,则需修改相应的字符集
    const static char spec[] = { '?', '*', ':', ':', '/', '\\', ' ', '@', '.', '_'};

    // TODO: 第二版改成unorder_map的方式，用哈希表加快查找速度
    for (int i = 0; i < sizeof(spec); ++i)
    {
        if (spec[i] == a_chr)
        {
            l_b_aval = true;
            break;
        }
    }

    return l_b_aval;
}

bool CClientParser::CheckSpecChar(const std::string& a_str_input)
{
    bool l_b_aval = true;
    for (int i=0; i<a_str_input.length(); ++i)
    {
        if (! CheckSpecChar(a_str_input[i]))
        {
            std::string log = "input char err:";
            log += a_str_input[i];
            LOG_RECORD(LOG_ERR, log);
            l_b_aval = false;
            break;
        }
    }

    return l_b_aval;
}

bool CClientParser::CheckInputAval(const std::string& a_str_input)
{
    char c = 0x00;
    return CDealString::CheckStringAvail((char*)a_str_input.c_str(), a_str_input.length(), c);
}

std::vector<std::string>& CClientParser::SplitInputToCommond(const std::string& a_str_input, std::vector<std::string>& a_vec_comond)
{
    return CDealString::SplitString(a_str_input, " ", a_vec_comond);
}

const std::string& CClientParser::ConvertInputToLower(std::string& a_str_input)
{
    CDealString::ConvertToLower((char*) a_str_input.c_str(), a_str_input.length());
    return a_str_input;
}

