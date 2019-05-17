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

// 日志,单例实现
// TODO: 如果不用单例,那管理生命周期会非常令人费解,遍布各种静态函数
class CLogger
{
private:
	CLogger(); // 拒绝外部new创建
	~CLogger(); // 不可显式析构,否则破坏对象声明周期

public:
	// 获取单例对象指针
	static CLogger* Instance();

	// 释放单例对象空间
	static void ReleaseInstance();

public:
	// 写日志
	bool WriteRecord(const std::string& a_str_record);

	// 检测初始化是否完成
	inline bool InitOver() {return m_b_init_over;};

private:
	std::string 	m_p_log_name;
	FILE* 			m_p_log;
	bool 			m_b_init_over;
private:
	static CLogger* m_p_this;
};

#define LOG_RECORD(log_level, str) 													\
do { 																				\
	std::string l_str_log = CDate::GetCTime();										\
	l_str_log += " ";																\
	l_str_log += __FILE__;															\
	l_str_log += ":";																\
	l_str_log += __FUNCTION__;														\
	char buff[56] = {0x00};															\
	snprintf(buff, 56, ":%d\t", __LINE__);											\
	l_str_log += buff;																\
	l_str_log += #log_level;														\
	l_str_log += " | ";																\
	l_str_log += str;																\
	l_str_log += "\n";																\
	CLogger::Instance()->WriteRecord(l_str_log); 									\
} while(0);


#define LOG_INFO(str) 	LOG_RECORD(LOG_INFO, str)
#define LOG_ERR(str) 	LOG_RECORD(LOG_ERR, str)


#endif//__LOGGER_H__
