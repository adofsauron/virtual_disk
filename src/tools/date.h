#ifndef __DATE_H__
#define __DATE_H__

#include <time.h>
#include <string>

class CDate
{
public:
	CDate();
	~CDate();

public:
	// 获取当前时间的格式化字符串
	static const std::string& GetCTime();
};

#endif//__DATE_H__