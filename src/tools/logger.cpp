#include "logger.h"


CLogger* CLogger::s_p_this = NULL;

CLogger::CLogger()
{
#ifdef LOG_WRITE_FILE    
    m_b_init_over = false;
    m_p_log_name = config::CONST_LOG_FILE_PATH;
    m_p_log_name += config::CONST_LOG_FILE_NAME;

    std::string l_str_date = CDate::GetCTime();
    CDealString::StringReplace((char*)l_str_date.c_str(), l_str_date.length(), " ", "_");
    CDealString::StringReplace((char*)l_str_date.c_str(), l_str_date.length(), ":", "_");
    CDealString::CombineChar((char*)l_str_date.c_str(), l_str_date.length(), '_');

    m_p_log_name += "_";
    m_p_log_name += l_str_date;
    m_p_log_name += ".log";


    if (!CDealFile::CheckFileExist(m_p_log_name.c_str()))
    {
        if(! CDealFile::FileCreate(m_p_log_name.c_str()))
        {
            return;
        }
    }

    if (!CDealFile::FileOpen(m_p_log_name.c_str(), "w", m_p_log))
    {
        //
        return;
    }

#endif

    m_b_init_over = true;
}

CLogger::~CLogger()
{

#ifdef LOG_WRITE_FILE
    if (NULL != m_p_log)
    {
        if (!CDealFile::FileClose(m_p_log))
        {
            // 
            return;
        }

        m_p_log = NULL;
    }
#endif
}

CLogger* CLogger::Instance()
{
    if (NULL == s_p_this)
    {
        s_p_this = new CLogger();
    }

    return s_p_this;
}

void CLogger::ReleaseInstance()
{
    DELETE_PTR(s_p_this);
}

bool CLogger::WriteRecord(const std::string& a_str_record)
{
#ifdef LOG_WRITE_FILE
    if (NULL == m_p_log)
    {
        return false;
    }

    if (! CDealFile::FileWrite(a_str_record.c_str(), a_str_record.length(), a_str_record.length(), 1, m_p_log))
    {
        return false;
    }
#else
	std::cout << a_str_record;
#endif
    return true;
}
