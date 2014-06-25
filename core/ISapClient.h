//
//  ISapClient.h
//  mystack
//
//  Created by mike on 14-6-22.
//  Copyright (c) 2014年 km. All rights reserved.
//

#ifndef mystack_ISapClient_h
#define mystack_ISapClient_h


//namespace sdo {
//    namespace sap {

class ISapClient
{
    virtual int DoConnect(const std::string& strIp,unsigned int dwPort,int nTimeout,unsigned int dwLocalPort=0) = 0;
    virtual int DoSendMessage(int nId,const void *pBuffer,int nLen,unsigned int dwTimeout=0) = 0;
    virtual void DoClose(int nId)  = 0;
    
    virtual ~ISapClient(){}
};

ISapClient * GetSapClientInstance();
//    }
//}

#endif
