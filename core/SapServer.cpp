//
//  SapServer.cpp
//  mystack
//
//  Created by mike on 14-6-22.
//  Copyright (c) 2014年 km. All rights reserved.
//

#include "SapServer.h"
#include "SapStack.h"
#include "SapLogHelper.h"
#include <boost/bind.hpp>


//namespace sdo {
//    namespace sap {

ISapServer* GetSapServerInstance()
{
    return CSapServer::Instance();
}

CSapServer::CSapServer() :
m_Callback(NULL),
m_IoService(CSapStack::Instance()->GetIoService()),
m_Acceptor(m_IoService),
m_Running(false)
{
    
}

int CSapServer::StartServer(unsigned int dwPort)
{
	SS_XLOG(XLOG_TRACE,"CSapServer::%s...,port[%u]\n",__FUNCTION__,dwPort);
	boost::system::error_code ec;
	m_Acceptor.open(boost::asio::ip::tcp::v4(),ec);
	if (ec)
	{
		SS_XLOG(XLOG_ERROR,"CSapServer::%s,open socket error:%s\n",__FUNCTION__,ec.message().c_str());
		m_Acceptor.close(ec);
		return -1;
	}

	m_Acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true),ec);
	m_Acceptor.set_option(boost::asio::ip::tcp::no_delay(true),ec);
	m_Acceptor.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),dwPort),ec);
	if (ec)
	{
		SS_XLOG(XLOG_ERROR,"CSapServer::%s,bind socket error%s\n",__FUNCTION__,ec.message().c_str());
		m_Acceptor.close(ec);
		return -2;
	}

	m_Acceptor.listen(boost::asio::socket_base::max_connections,ec);
	if (ec)
	{
		SS_XLOG(XLOG_ERROR,"CSapServer::%s,listen socket error%s\n",__FUNCTION__,ec.message().c_str());
		m_Acceptor.close(ec);
		return -3;
	}

	m_Running = true;
	_StartAccept();
    return 0;
}

void CSapServer::StopServer()
{
	SS_XLOG(XLOG_DEBUG,"CSapServer::%s\n",__FUNCTION__);
	m_IoService.post(boost::bind(&CSapServer::_HandleStop,this));
}

void CSapServer::SetInterval(unsigned int dwRequestTimeout, int pHeartInterval, int pHeartDisconnectInterval)
{
    
}

CSapServer* CSapServer::Instance()
{
    static CSapServer *t_Instance = new CSapServer();
    return t_Instance;
}

void CSapServer::_StartAccept()
{
	SS_XLOG(XLOG_DEBUG,"CSapAgent::%s...\n",__FUNCTION__);
	
	
	SapConnection_ptr new_connection(new CSapConnection(this,CSapStack::Instance()->GetIoService()));
	m_Acceptor.async_accept(new_connection->Socket(),
		boost::bind(&CSapServer::_HandleAccept,this,new_connection,boost::asio::placeholders::error)
		);

}

void CSapServer::_HandleAccept(SapConnection_ptr new_connection,const boost::system::error_code &err)
{
	SS_XLOG(XLOG_DEBUG,"CSapServer::%s\n",__FUNCTION__);

	if (!err)
	{
		new_connection->SetRemoteAddr();
		//todo
		OnReceiveConnection(new_connection->Id(),new_connection->GetRemoteIp(),new_connection->GetRemotePort());
		new_connection->AsynRead();

		_StartAccept();
	}
	else if (m_Running)
	{
		SS_XLOG(XLOG_WARNING,"CSapAgent::HandleAccept, error:%s\n",err.message().c_str());
		m_Acceptor.async_accept(new_connection->Socket(),
				boost::bind(&CSapServer::_HandleAccept,this,new_connection,boost::asio::placeholders::error)
			);
	}
	else
	{
		SS_XLOG(XLOG_WARNING,"CSapAgent::HandleAccept, stop. error:%s\n",err.message().c_str());
	}
}

void CSapServer::_HandleStop()
{
	SS_XLOG(XLOG_TRACE,"CSapAgent::%s\n",__FUNCTION__);
	boost::system::error_code ec;
	m_Acceptor.close(ec);
	m_Running = false;
}

void CSapServer::OnConnectResult(int nId,int nResult)
{

}

void CSapServer::OnReceiveConnection(int nId,const std::string& strRemoteIp,unsigned int dwRemotePort)
{

}

void CSapServer::OnReceiveMessage(int nId,const void *pBuffer,int nLen,const std::string& strRemoteIp,unsigned int dwRemotePort)
{
	SS_XLOG(XLOG_DEBUG,"CSapAgent::%s,id[%d],buffer[%x],len[%d],addr[%s:%d]\n",__FUNCTION__,nId,pBuffer,nLen,strRemoteIp.c_str(),dwRemotePort);
	if (m_Callback != NULL)
	{
		m_Callback->OnReceiveMessage(nId,pBuffer,nLen,strRemoteIp,dwRemotePort);
	}
}

void CSapServer::OnPeerClose(int nId)
{

}

//    }
//}