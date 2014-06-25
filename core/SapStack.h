//
//  SapStack.h
//  mystack
//
//  Created by mike on 14-6-22.
//  Copyright (c) 2014年 km. All rights reserved.
//

#ifndef __mystack__SapStack__
#define __mystack__SapStack__

#include "ISapStack.h"
#include "SapStackThread.h"

class CSapStack : public ISapStack
{
public:
    static CSapStack *Instance()
    {
        static CSapStack* t_Instance = new CSapStack();
        return t_Instance;
    }
    
    virtual void Start(int nThread);
    virtual void Stop();
    boost::asio::io_service &GetIoService();
    
private:
    CSapStack();
    CSapStackThread **ppThreads;
    int m_nThread;
    unsigned int m_dwIndex;
    bool m_bRunning;
    boost::mutex m_mut;
};

#endif /* defined(__mystack__SapStack__) */
