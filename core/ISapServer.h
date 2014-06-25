//
//  ISapAgent.h
//  mystack
//
//  Created by mike on 14-6-22.
//  Copyright (c) 2014年 km. All rights reserved.
//

#ifndef mystack_ISapServer_h
#define mystack_ISapServer_h

#include "ISapCallback.h"

//namespace sdo {
//    namespace sap {

class ISapServer
{
public:
    virtual int StartServer(unsigned int dwPort) = 0;
    virtual void StopServer() = 0;
    virtual void SetCallback(ISapCallback *pCallback) = 0;
    virtual void SetInterval(unsigned int dwRequestTimeout,int HeartIntervl,int nHeartDisconnectInterval) = 0;
    
    virtual ~ISapServer(){};
    
};
        
ISapServer * GetSapServerInstance();
//    }
//}


#endif
