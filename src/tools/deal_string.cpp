
#include "deal_string.h"

// 是否是中文
// return 1:ok, 0:false
int CDealString::IsZhCh(char a_c_char)
{
    return (~(a_c_char >> 8) == 0) ? 1 : 0;
}

// 是否是支持的特殊字符
// return 1:ok, 0:false
int CDealString::IsSpecChar(char a_c_char)
{
    int l_i_find = 0;
    const static char spec[] = { '?', '*', ':', ':', '/', '\\', ' ', '@', '.', '_'};
    for (uint32 i = 0; i<sizeof(spec); ++i)
    {
        if (spec[i] == a_c_char)
        {
            l_i_find = 1;
            break;
        }
    }

    return l_i_find;
}

// 字符串合法性检查
bool CDealString::CheckStringAvail(char* pstr, int len, char& err_char)
{
    bool l_b_aval = true;
    for (int i = 0; i<len; ++i)
    {
        const char lc = pstr[i];
        if (!((IsZhCh(lc) != 0) // 中文字符
            || (isdigit(lc) != 0) // 数字
            || (isalpha(lc) != 0) // 英文字符
            || (IsSpecChar(lc) != 0))) // 支持的特殊字符
        {
            l_b_aval = false;
            err_char = lc;
            break;
        }
    }

    return l_b_aval;
}

// 字符串替换
char* CDealString::StringReplace(char* str, int len, const char* oldstr, const char* newstr)
{
    char* bstr = (char*)malloc(len + 1);
    memset(bstr, 0x00, sizeof(char) * (len + 1));

    for (int i = 0; i < len; ++i)
    {
        if (!strncmp(str + i, oldstr, strlen(oldstr)))
        {
            strcat(bstr, newstr);
            i += strlen(oldstr) - 1;
        }
        else
        {
            strncat(bstr, str + i, 1);
        }
    }

    strcpy(str, bstr);
    FREE_PTR(bstr);
    return str;
}

// 合并重复字符
char* CDealString::CombineChar(char* pstr, int len, char a_c_char)
{
    char* buff = (char*)malloc(len);
    memset(buff, 0x00, len);
    int lindex = 0;

    int i = 0;
    for (; i < len - 1; ++i)
    {
        if ((a_c_char == pstr[i]) && (pstr[i] == pstr[i + 1]))
        {
            continue;
        }

        buff[lindex++] = pstr[i];
    }

    buff[lindex++] = pstr[i++];
    memcpy(pstr, buff, len);
    FREE_PTR(buff);
    return pstr;
}

// 路径转换
char* CDealString::PathConver(char* pstr, int len)
{
    for (int i = 0; i < len; ++i)
    {
        if ('\\' == pstr[i])
        {
            pstr[i] = '/';
        }
    }

    StringReplace(pstr, len, "c:/", "/");

    CombineChar(pstr, len, '/');

    return pstr;

}

// 转换为小写
char* CDealString::ConvertToLower(char* pstr, int len)
{
    for (int i = 0; i < len; ++i)
    {
		if (IsZhCh(pstr[i]) != 0)
		{
			continue;
		}

        if (isupper(pstr[i]))
        {
            pstr[i] = tolower(pstr[i]);
        }
    }

    return pstr;
}

std::vector<std::string>& CDealString::SplitString(const std::string& str,const std::string& a_str_pattern, std::vector<std::string>& resultVec)
{
    resultVec.clear();
    std::string::size_type pos1, pos2;
    pos2 = str.find(a_str_pattern);
    pos1 = 0;
    while(std::string::npos != pos2)
    {
        std::string l_str = str.substr(pos1, pos2-pos1);
        if ((!l_str.empty()) && (l_str != "") )
            resultVec.push_back(l_str);
         
        pos1 = pos2 + a_str_pattern.size();
        pos2 = str.find(a_str_pattern, pos1);
    }

    if(pos1 != str.length())
    {
        std::string l_str = str.substr(pos1);
        if ((!l_str.empty()) && (l_str != "") )
            resultVec.push_back(str.substr(pos1));
    }
    
    return resultVec;
}

// 通配单个字符
bool CDealString::CheckRegexSin(const std::string& a_str_pre, const char a_chr, const std::string& a_str_given)
{
    if (a_str_pre.empty() || a_str_given.empty())
    {
        return false;
    }

    if (! a_str_pre.find(a_chr))
    {
        return false;
    }

    // 两个字符串长度不同,不再比较
    if (a_str_pre.length() != a_str_given.length())
    {
        return false;
    }

    std::string l_str_regex = "(";
    l_str_regex += a_str_pre;
    l_str_regex += ")";

    // 转换成正则表达式
    StringReplace((char*)l_str_regex.c_str(), l_str_regex.length(),"?",".");

    std::regex l_o_pattern(l_str_regex);
    std::smatch l_o_result;
    return std::regex_search(a_str_given, l_o_result, l_o_pattern);
}

bool CDealString::FindSubStr(const std::string& a_str_given, const std::string& a_str_sub)
{
    if (a_str_given.empty())
    {
        return false;
    }

    if (a_str_sub.empty())
    {
        return false;
    }

    return std::string::npos != a_str_given.find(a_str_sub);
}

bool CDealString::PathToIner(std::string& a_str_path)
{
    if (a_str_path.empty())
    {
        return false;
    }

    PathConver((char*)a_str_path.c_str(), a_str_path.length());

    if (('.' != a_str_path[0]) && ('/' != a_str_path[0]))
    {
        a_str_path.insert(0, "./");
    }

    return true;
}
