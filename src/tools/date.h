#ifndef __DATE_H__
#define __DATE_H__

#include <time.h>
#include <string>

#include "../base/platform.h"

class CDate
{
public:
	CDate();
	~CDate();

public:
	// 获取当前时间的格式化字符串
	static const std::string& GetCTime();

	// 获得当前unix时间戳,以uint32类型返回处理
	static uint32 GetUnixTime();

	// 根据unix时间戳获得格式化输出
	static const std::string& GetCTimeByUnixTime(const uint32 a_o_time);

private:
	static std::string m_str_date;
};

#endif//__DATE_H__
