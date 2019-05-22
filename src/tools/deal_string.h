#ifndef __PROC_STRING_H__
#define __PROC_STRING_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <string>
#include <iostream>
#include <vector>
#include <regex>

#include "../base/base.h"

class CDealString
{
private:
	CDealString();
	CDealString(const CDealString&);
	CDealString& operator = (const CDealString&);

public:
	//--------------------------------------------------------
	// c风格的处理

	// 是否是中文
	// return 1:ok, 0:false
	static int IsZhCh(char a_c_char);

	// 是否是支持的特殊字符
	// return 1:ok, 0:false
	static int IsSpecChar(char a_c_char);

	// 字符串合法性检查
	static bool CheckStringAvail(char* pstr, int len, char& err_char);

	// 字符串替换
	static char* StringReplace(char* str, int len, const char* oldstr, const char* newstr);

	// 合并重复字符
	static char* CombineChar(char* pstr, int len, char a_c_char);

	// 路径转换
	static char* PathConver(char* path, int len);

	// 转换为小写
	static char* ConvertToLower(char* buff, int len);

	//--------------------------------------------------------
	// c++风格的处理
	
	// 分隔字符串 
	static std::vector<std::string>& SplitString(const std::string& str,const std::string& a_str_pattern, std::vector<std::string>& resultVec);

	// 通配单个字符
	// a_str_pre 原字符串；  通配字符 ?； a_str_given要比较的字符串
	static bool CheckRegexSin(const std::string& a_str_pre, const char a_chr, const std::string& a_str_given);

	// 查找子字符串
	static bool FindSubStr(const std::string& a_str_given, const std::string& a_str_sub);

	static bool PathToIner(std::string& a_str_path);

};

#endif//__PROC_STRING_H__
