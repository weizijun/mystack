//
//  SapStackThread.h
//  mystack
//
//  Created by mike on 14-6-22.
//  Copyright (c) 2014年 km. All rights reserved.
//

#ifndef __mystack__SapStackThread__
#define __mystack__SapStackThread__

#include <boost/asio.hpp>
#include <boost/thread.hpp>


class CSapStackThread
{
public:
    CSapStackThread();
    ~CSapStackThread();
    
    void Start();
    void Stop();
    
    boost::asio::io_service &GetIoService() {return m_IoService; }
    
private:
    boost::asio::io_service m_IoService;
    boost::asio::io_service::work *m_Work;
    boost::thread m_thread;
};

#endif /* defined(__mystack__SapStackThread__) */
