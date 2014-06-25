//
//  SapServer.h
//  mystack
//
//  Created by mike on 14-6-22.
//  Copyright (c) 2014年 km. All rights reserved.
//

#ifndef __mystack__SapServer__
#define __mystack__SapServer__

#include <boost/asio.hpp>
#include <string>
#include <map>
#include "ISapServer.h"
#include "SapConnection.h"

//namespace sdo {
//    namespace sap {

class CSapConnection;	
typedef boost::shared_ptr<CSapConnection> SapConnection_ptr;
class CSapServer : public ISapServer
{
public:
    CSapServer();
    ~CSapServer(){}
    static CSapServer *Instance();
    
    virtual int StartServer(unsigned int dwPort);
    virtual void StopServer();
    virtual void SetCallback(ISapCallback* pCallback){ m_Callback = pCallback; }
    virtual void SetInterval(unsigned int dwRequestTimeout,int pHeartInterval,int pHeartDisconnectInterval);
    
	void OnConnectResult(int nId,int nResult);
	void OnReceiveConnection(int nId,const std::string& strRemoteIp,unsigned int dwRemotePort);
	void OnReceiveMessage(int nId,const void *pBuffer,int nLen,const std::string& strRemoteIp,unsigned int dwRemotePort);
	void OnPeerClose(int nId);

private:
	void _StartAccept();
	void _HandleAccept(SapConnection_ptr new_connection,const boost::system::error_code &e);
	void _HandleStop();

private:
    static CSapServer *m_pInstance;
    ISapCallback *m_Callback;
    
    boost::asio::io_service &m_IoService;
    boost::asio::ip::tcp::acceptor m_Acceptor;
    
    bool m_Running;
};
//    }
//}


#endif /* defined(__mystack__SapServer__) */
