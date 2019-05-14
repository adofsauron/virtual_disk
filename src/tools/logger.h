#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <iostream>
#include <string>

#include "date.h"
#include "deal_string.h"
#include "deal_file.h"
#include "../base/base.h"

enum ENUM_LOG_LEVEL
{
	LOG_EMERG	= 0,
	LOG_ALERT	= 1,
	LOG_CRIT	= 2,
	LOG_ERR		= 3,
	LOG_WARNING = 4,
	LOG_NOTICE	= 5,
	LOG_INFO	= 6,
	LOG_DEBUG	= 7,
};

//日志单例实现
class CLogger
{
private:
	CLogger();
public:
	~CLogger();

public:
	static CLogger* Instance();
	bool WriteRecord(const std::string& a_str_record);
	inline bool InitOver() {return m_b_init_over;};

private:
	std::string m_p_log_name;
	FILE* m_p_log;
	bool m_b_init_over;
private:
	static CLogger* m_p_this;
};


void test()
{
	do
	{
		std::string l_str_log = CDate::GetCTime();
		l_str_log += " ";
		l_str_log += __FILE__;
		l_str_log += ":";
		l_str_log += __LINE__;
		l_str_log += " ";
		l_str_log += #log_level;
		l_str_log += "|";
		l_str_log += "str";
		l_str_log += "\n";


	} while(0);
}

#define LOG_RECORD(log_level, str) 													\
do { 																				\
	std::string l_str_log = CDate::GetCTime();										\
	l_str_log += " ";																\
	l_str_log += __FILE__;															\
	l_str_log += ":";																\
	l_str_log += __LINE__;															\
	l_str_log += " ";																\
	l_str_log += #log_level;														\
	l_str_log += "|";																\
	l_str_log += "str";																\
	l_str_log += "\n";																\
	CLogger::Instance()->WriteRecord(l_str_log); 									\
} while(0);


#endif//__LOGGER_H__