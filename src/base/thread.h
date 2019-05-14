// 线程封装

#ifndef __THREAD_H__
#define __THREAD_H__

#include <thread>

#include "platform.h"
#include "macro.h"
#include "../tools/logger.h"

enum ENUM_THREAD_STATE
{
	THREAD_INVIAL	= 0,
	THREAD_RUNIING	= 1,
	THREAD_CLOSE	= 2,
};

typedef std::thread::id	tid;

class CThread
{
public:
	CThread();
	virtual ~CThread();

public:
	virtual void Start() = 0;
	void Run();
	inline tid GetTid(){ return m_id;}
	void SetStateRuning();
	void SetStateClose();

private:
	std::thread* m_thread;
	ENUM_THREAD_STATE m_state;
	tid m_id;
};


#endif//__THREAD_H__