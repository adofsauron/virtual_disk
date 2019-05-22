
#include "date.h"

std::string CDate::m_str_date = "";

const std::string& CDate::GetCTime()
{
	m_str_date.clear();
	time_t seconds;
	seconds = time(NULL);
	m_str_date = ctime(&seconds);
    m_str_date.replace(m_str_date.find("\n"),1,"" );
	return m_str_date;
}

uint32 CDate::GetUnixTime()
{
	return (uint32) time(NULL);
}

const std::string& CDate::GetCTimeByUnixTime(const uint32 a_o_time)
{
	m_str_date.clear();
	time_t seconds = a_o_time;
	m_str_date = ctime(&seconds);
    m_str_date.replace(m_str_date.find("\n"),1,"" );
	return m_str_date;
}
