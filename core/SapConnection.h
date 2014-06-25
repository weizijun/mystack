#ifndef mystack_SapConnection_h
#define mystack_SapConnection_h

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio/basic_deadline_timer.hpp>
#include <boost/thread/mutex.hpp>
#include <string>
#include <deque>
#include "SapServer.h"
#include "common/detail/_time.h"
#include "SapBuffer.h"

class CSapServer;
class CSapConnection : public boost::enable_shared_from_this<CSapConnection> ,
	private boost::noncopyable
{
public:
	CSapConnection(CSapServer *pServer,boost::asio::io_service &io_service);
	~CSapConnection();

	static void SetInterval(int nHeartIntervl,int nHeartDisconnectInterval)
	{
		m_nHeartInterval=nHeartIntervl;
		m_nTimeoutInterval=nHeartDisconnectInterval;
	}

	void SetId(int nId){m_nId=nId;}
	int Id() const {return m_nId;}

	void SetRemoteAddr();
	const std::string & GetRemoteIp() const { return m_strRemoteIp;}
	unsigned int GetRemotePort() const { return m_dwRemotePort;}

	boost::asio::ip::tcp::socket& Socket() { return m_socket; }

	void AsyncConnect(const std::string &strIp,unsigned int dwPort,int nTimeout,unsigned int dwLocalPort=0);
	void AsynRead();
	void AsyncWrite(const void *pBuffer,int nLen,unsigned int dwTimeout=0);
	void Close();

private:
	void _HandleRead(const boost::system::error_code& err,std::size_t dwTransferred);

private:
	CSapServer *m_pServer;
	static int m_nHeartInterval;
	static int m_nTimeoutInterval;
	int m_nId;

	boost::asio::ip::tcp::socket m_socket;
	boost::asio::ip::tcp::resolver m_resolver;
	boost::asio::deadline_timer m_timer;

	bool m_bRunning;

	bool m_isConnected;
	std::string m_strRemoteIp;
	unsigned int m_dwRemotePort;

	struct timeval_a m_HeartBeatPoint;
	struct timeval_a m_AlivePoint;

	CSapBuffer m_Buffer;
};

typedef boost::shared_ptr<CSapConnection> SapConnection_ptr;

#endif