//
//  ISapCallback.h
//  mystack
//
//  Created by mike on 14-6-22.
//  Copyright (c) 2014年 km. All rights reserved.
//

#ifndef mystack_ISapCallback_h
#define mystack_ISapCallback_h

#include <string>

//namespace sdo {
//    namespace sap {

class ISapCallback
{
public:
    virtual void OnConnectResult(int nId,int nResult) = 0;
    virtual void OnReceiveConnection(int nId,const std::string &strRemoteIp,unsigned int dwRemotPort)=0;
    virtual void OnReceiveMessage(int nId,const void* pBuffer,int nLen,const std::string &strRemoteIp,unsigned int dwRemotPort) = 0;
    virtual void OnPeerClose(int nId) = 0;

    virtual ~ISapCallback(){}
};
        
//    }
//}


#endif
