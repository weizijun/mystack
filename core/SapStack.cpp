//
//  SapStack.cpp
//  mystack
//
//  Created by mike on 14-6-22.
//  Copyright (c) 2014年 km. All rights reserved.
//

#include "SapStack.h"
#include <boost/bind.hpp>
#include "SapLogHelper.h"
#include <boost/date_time.hpp>

ISapStack *GetSapStackInstance()
{
    return CSapStack::Instance();
}

CSapStack::CSapStack():
ppThreads(NULL),
m_nThread(1),
m_dwIndex(0),
m_bRunning(false)
{ }

void CSapStack::Start(int nThread)
{
    SS_XLOG(XLOG_DEBUG,"CSapStack::%s[%d],running[%d]...\n",__FUNCTION__,nThread,m_bRunning);
	boost::lock_guard<boost::mutex> guard(m_mut);
	if (m_bRunning == false)
	{
		m_bRunning = true;
		m_nThread = nThread;
		ppThreads = new CSapStackThread *[m_nThread];
		for(int i = 0; i<m_nThread; ++i)
		{
			CSapStackThread *pThread = new CSapStackThread;
			ppThreads[i] = pThread;
			pThread->Start();
		}
		//todo
	}
	
}

void CSapStack::Stop()
{
    SS_XLOG(XLOG_DEBUG,"CSapStack::%s...\n",__FUNCTION__);
	boost::lock_guard<boost::mutex> guard(m_mut);
	if (m_bRunning == true)
	{
		m_bRunning == false;
		//todo

		for(int i=0; i<m_nThread; ++i)
		{
			CSapStackThread *pThread = ppThreads[i];
			pThread->Stop();
			delete pThread;
		}
	}
	
}

boost::asio::io_service& CSapStack::GetIoService()
{
    int i = (m_dwIndex++) % m_nThread;
    CSapStackThread *t_Thread = ppThreads[i];
    return t_Thread->GetIoService();
}