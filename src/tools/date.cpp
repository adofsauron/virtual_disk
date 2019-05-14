
#include "date.h"

const std::string& CDate::GetCTime()
{
    static std::string l_str_date;
	l_str_date.clear();
	time_t seconds;
	seconds = time(NULL);
	l_str_date = ctime(&seconds);
    l_str_date.replace(l_str_date.find("\n"),1,"" );
	return l_str_date;
}
