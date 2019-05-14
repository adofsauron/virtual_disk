#include "logger.h"


CLogger* CLogger::m_p_this = NULL;

CLogger::CLogger()
{
    m_b_init_over = false;
    m_p_log_name = config::CONST_LOG_FILE_PATH;
    m_p_log_name += config::CONST_LOG_FILE_NAME;

    std::string l_str_date = CDate::GetCTime();
    CDealString::StringReplace((char*)l_str_date.c_str(), l_str_date.length(), " ", "_");
    CDealString::CombineChar((char*)l_str_date.c_str(), l_str_date.length(), '_');

    m_p_log_name += "_";
    m_p_log_name += l_str_date;
    m_p_log_name += ".log";


    if (!CDealFile::CheckFileExist(m_p_log_name.c_str()))
    {
        if(! CDealFile::FileCreate(m_p_log_name.c_str()))
        {
            //
            return;
        }
    }

    if (!CDealFile::FileOpen(m_p_log_name.c_str(), "w", m_p_log))
    {
        //
        return;
    }

    m_b_init_over = true;
}

CLogger::~CLogger()
{
    DELETE_PTR(m_p_this);
    if (NULL != m_p_log)
    {
        if (!CDealFile::FileClose(m_p_log))
        {
            // 
            return;
        }

        m_p_log = NULL;
    }
}

CLogger* CLogger::Instance()
{
    if (NULL == m_p_this)
    {
        m_p_this = new CLogger();
    }

    return m_p_this;
}

bool CLogger::WriteRecord(const std::string& a_str_record)
{
    if (NULL == m_p_log)
    {
        return false;
    }

    if (!CDealFile::FileWrite(a_str_record.c_str(), a_str_record.length(), 1, a_str_record.length(), m_p_log))
    {
        return false;
    }

    return true;
}
