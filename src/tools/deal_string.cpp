
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
    for (int i = 0; i<sizeof(spec); ++i)
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
        if (!((isalpha(lc) != 0) // 英文字符
            || (isdigit(lc) != 0) // 数字
            || (IsZhCh(lc) != 0) // 中文字符
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
    memset(bstr, 0x00, sizeof(bstr));

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
    bool lb_first = true;

    char* buff = (char*)malloc(len + 1);
    memset(buff, 0x00, len+1);
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
        if (isupper(pstr[i]))
        {
            pstr[i] = tolower(pstr[i]);
        }
    }

    return pstr;
}

