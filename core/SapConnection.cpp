#include "SapConnection.h"
#include "SapLogHelper.h"
#include <boost/bind.hpp>

CSapConnection::CSapConnection(CSapServer *pServer,boost::asio::io_service &io_service) :
m_pServer(pServer),m_socket(io_service),m_resolver(io_service),m_timer(io_service)
{

}

CSapConnection::~CSapConnection()
{
	SS_XLOG(XLOG_TRACE,"CSapConnection::%s,id[%d]\n",__FUNCTION__,m_nId);
}

void CSapConnection::AsyncConnect(const std::string &strIp,unsigned int dwPort,int nTimeout,unsigned int dwLocalPort)
{
	SS_XLOG(XLOG_TRACE,"CSapConnection::async_connect,id[%d],addr[%s:%d],timeout[%d]\n",m_nId,strIp.c_str(),dwPort,nTimeout);


}

void CSapConnection::AsynRead()
{
	SS_XLOG(XLOG_DEBUG,"CSapConnection::%s...,id[%d]\n",__FUNCTION__,m_nId);

	gettimeofday_a(&m_HeartBeatPoint,NULL);
	gettimeofday_a(&m_AlivePoint,NULL);

	m_socket.async_read_some(boost::asio::buffer(m_Buffer.top(),m_Buffer.capacity()),
		boost::bind(&CSapConnection::_HandleRead,shared_from_this(),
		boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
}

void CSapConnection::AsyncWrite(const void *pBuffer,int nLen,unsigned int dwTimeout)
{
	SS_XLOG(XLOG_DEBUG,"CSapConnection::%s...\n",__FUNCTION__);
}

void CSapConnection::Close()
{
	SS_XLOG(XLOG_DEBUG,"CSapConnection::%s...\n",__FUNCTION__);
}

void CSapConnection::_HandleRead(const boost::system::error_code& err,std::size_t dwTransferred)
{
	SS_XLOG(XLOG_DEBUG,"CSapConnection::%s,id[%d],bytes_transferred[%d]\n",__FUNCTION__,m_nId,dwTransferred);
	if (err)
	{
		SS_XLOG(XLOG_WARNING,"CSapConnection::%s,id[%d],error:%s\n",__FUNCTION__,m_nId,err.message().c_str());
		//todo
		//StopConnection();
		return ;
	}

	gettimeofday_a(&m_AlivePoint,NULL);
	m_Buffer.inc_loc(dwTransferred);
	unsigned char* t_Data = m_Buffer.base();
	m_pServer->OnReceiveMessage(m_nId,t_Data,dwTransferred,m_strRemoteIp,m_dwRemotePort);

	m_socket.async_read_some(boost::asio::buffer(m_Buffer.top(),m_Buffer.capacity()),
		boost::bind(&CSapConnection::_HandleRead,shared_from_this(),
		boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));

	//while(m_Buffer.top() >= 0)
	//{

	//}
}

void CSapConnection::SetRemoteAddr()
{
	try{
		m_strRemoteIp=m_socket.remote_endpoint().address().to_string();
		m_dwRemotePort=m_socket.remote_endpoint().port();
	}
	catch(boost::system::system_error & ec){
		SS_XLOG(XLOG_WARNING,"CSapConnection::SetRemoteAddr,code:%s\n",ec.what());
		m_strRemoteIp="";
		m_dwRemotePort=0;
		return;
	}
}