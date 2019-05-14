#include "thread.h"

CThread::CThread()
	:m_thread(NULL),
	m_state(THREAD_INVIAL)
{
}

CThread::~CThread()
{
	LOG_RECORD(LOG_DEBUG, "base des:" << m_id);
	DELETE_PTR(m_thread);
}

void CThread::Start()
{
	LOG_RECORD(LOG_DEBUG, "thread start:" << m_id);
}

void CThread::Run()
{
	if (m_thread)
	{
		LOG_RECORD(LOG_ERR, "Error:thread has start:" << m_id);
		return;
	}

	m_thread = new std::thread(&CThread::Start, this);
	m_id = m_thread->get_id();
	m_thread->detach();
}

void CThread::SetStateRuning()
{
	LOG_RECORD(LOG_DEBUG, "thread runing:" << m_id);
	m_state = THREAD_RUNIING;
}

void CThread::SetStateClose()
{
	LOG_RECORD(LOG_DEBUG, "thread exit:" << m_id);
	m_state = THREAD_CLOSE;
}