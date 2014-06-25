//
//  SapStackThread.cpp
//  mystack
//
//  Created by mike on 14-6-22.
//  Copyright (c) 2014年 km. All rights reserved.
//

#include "SapStackThread.h"
#include "SapLogHelper.h"
#include <boost/bind.hpp>

CSapStackThread::CSapStackThread()
{
    
}

void CSapStackThread::Start()
{
    SS_XLOG(XLOG_DEBUG,"CSapStackThread::%s...\n",__FUNCTION__);
    m_Work = new boost::asio::io_service::work(m_IoService);
    m_thread = boost::thread(boost::bind(&boost::asio::io_service::run,&m_IoService));
}

void CSapStackThread::Stop()
{
    delete m_Work;
    m_thread.join();
    m_IoService.reset();
}

CSapStackThread::~CSapStackThread()
{
    
}