#include "AdminSession.h"
#include "SapServer.h"
#include "AdminLogHelper.h"
#include "MemoryPool.h"
#include <iostream>

using namespace sdo::common;

void dump_buffer(const string& strPacketInfo,const void *pBuffer,unsigned int nPacketlen)
{
	string strPacket(strPacketInfo);
	unsigned char *pChar=(unsigned char *)pBuffer;
	int nLine=nPacketlen>>3;
	int nLast=(nPacketlen&0x7);
	int i=0;
	char szBuf[128]={0};
	for(i=0;(i<nLine)&&(i<40);i++)
	{            
		unsigned char * pBase=pChar+(i<<3);
		snprintf(szBuf, sizeof(szBuf),"                [%2d]    %02x %02x %02x %02x    %02x %02x %02x %02x    ......\n",
			i,*(pBase),*(pBase+1),*(pBase+2),*(pBase+3),*(pBase+4),*(pBase+5),*(pBase+6),*(pBase+7));
		strPacket.append(szBuf);
	}

	unsigned char * pBase=pChar+(i<<3);
	if(nLast>0)
	{
		for(int j=0;j<8;j++)
		{
			char szBuf[128]={0};
			if(j==0)
			{
				sprintf(szBuf,"                [%2d]    %02x ",i,*(pBase+j));
				strPacket+=szBuf;
			}
			else if(j<nLast&&j==3)
			{
				sprintf(szBuf,"%02x    ",*(pBase+j));
				strPacket+=szBuf;
			}
			else if(j>=nLast&&j==3)
			{
				strPacket+="      ";
			}
			else if(j<nLast)
			{
				sprintf(szBuf,"%02x ",*(pBase+j));
				strPacket+=szBuf;
			}
			else
			{
				strPacket+="   ";
			}

		}
		strPacket+="   ......\n";
	}

	A_XLOG(XLOG_DEBUG,strPacket.c_str());
}


CAdminSession::CAdminSession()
{
    m_funcMap[ADMIN_LOGIN_MSG]=&CAdminSession::DoAdminLoginMsg;
    m_funcMap[ADMIN_CONNECT_RESULT_MSG]=&CAdminSession::DoConnectResultMsg;
    m_funcMap[ADMIN_RECEIVE_MESSAGE_MSG]=&CAdminSession::DoReceiveMsg;
    m_funcMap[ADMIN_PEER_CLOSE_MSG]=&CAdminSession::DoPeerCloseMsg;
    m_funcMap[ADMIN_SEND_REQUEST_MSG]=&CAdminSession::DoRequestMsg;
	m_funcMap[ADMIN_SERVER_START_MSG] = &CAdminSession::DoStartServerMsg;
}

void CAdminSession::OnLoginServer(const string &strIp,int nPort)
{
    A_XLOG(XLOG_DEBUG,"CAdminSession::OnLoginServer,addr[%s:%d]\n",
        strIp.c_str(),nPort);
    
    SAdminLoginMsg *pMsg=new SAdminLoginMsg;
    pMsg->m_strIp=strIp;
    pMsg->m_nPort=nPort;

    PutQ(pMsg);
}

void CAdminSession::OnStartServer(int nPort)
{
	A_XLOG(XLOG_DEBUG,"CAdminSession::%s,addr[%d]\n",__FUNCTION__,nPort);

	SAdminServerStartMsg *pMsg=new SAdminServerStartMsg;
	pMsg->m_nPort=nPort;

	PutQ(pMsg);
}

void CAdminSession::OnSendRequest(const string &strRequest)
{
    A_XLOG(XLOG_DEBUG,"CAdminSession::OnSendRequest,request[%s]\n",strRequest.c_str());
    SAdminRequestMsg *pMsg=new SAdminRequestMsg;
    pMsg->m_strRequest=strRequest;

    PutQ(pMsg);
}

void CAdminSession::OnConnectResult(int nId,int nResult)
{
    
    A_XLOG(XLOG_DEBUG,"CAdminSession::OnConnectResult,id[%d],result[%d]\n",
            nId,nResult);

    SAdminConnectResultMsg *pMsg=new SAdminConnectResultMsg;
    pMsg->m_nId=nId;
    pMsg->m_nResult=nResult;

    PutQ(pMsg);
}
void CAdminSession::OnReceiveMessage(int nId,const void *pBuffer, int nLen, const string &strRemoteIp,unsigned int dwRemotPort)
{
    A_XLOG(XLOG_DEBUG,"CAdminSession::OnReceiveMessage,id[%d]\n",nId);
    
    SAdminReceiveMsg *pMsg=new SAdminReceiveMsg;
    pMsg->m_nId=nId;
    pMsg->m_pBuffer=CMemoryPool::Instance()->Malloc(nLen);
    memcpy(pMsg->m_pBuffer,pBuffer,nLen);
    pMsg->m_nLen=nLen;

    PutQ(pMsg);
}
void CAdminSession::OnPeerClose(int nId)
{
    A_XLOG(XLOG_DEBUG,"CAdminSession::OnPeerClose,id[%d]\n",nId);
    SdminPeerCloseMsg *pMsg=new SdminPeerCloseMsg;
    pMsg->m_nId=nId;
    PutQ(pMsg);
}

void CAdminSession::Deal(void *pMsg)
{
    SAdminMsg *pAdminMsg=(SAdminMsg *)pMsg;
    (this->*(m_funcMap[pAdminMsg->m_enType]))(pAdminMsg);
    delete pAdminMsg;
}
void CAdminSession::DoAdminLoginMsg(SAdminMsg * pMsg)
{
    //SAdminLoginMsg *pDetail=(SAdminLoginMsg *)pMsg;
    //A_XLOG(XLOG_TRACE,"CAdminSession::DoAdminLoginMsg,addr[%s:%d],login[%s:%s]\n",
    //    pDetail->m_strIp.c_str(),pDetail->m_nPort,pDetail->m_strName.c_str(),pDetail->m_strPass.c_str());

    //m_strName=pDetail->m_strName;
    //m_strPass=pDetail->m_strPass;
    //m_nId=CSapAgent::Instance()->DoConnect(pDetail->m_strIp,pDetail->m_nPort,5);
}

void CAdminSession::DoStartServerMsg(SAdminMsg * pMsg)
{
	SAdminServerStartMsg *pDetail=(SAdminServerStartMsg *)pMsg;
	A_XLOG(XLOG_TRACE,"CAdminSession::%s,addr[%d]\n",__FUNCTION__,pDetail->m_nPort);

	m_nId=CSapServer::Instance()->StartServer(pDetail->m_nPort);
}

void CAdminSession::DoConnectResultMsg(SAdminMsg * pMsg)
{
    //SAdminConnectResultMsg *pDetail=(SAdminConnectResultMsg *)pMsg;
    //A_XLOG(XLOG_TRACE,"CAdminSession::DoConnectResultMsg,id[%d],result[%d]\n",
    //    m_nId,pDetail->m_nResult);

    //if(pDetail->m_nResult!=0)
    //{
    //    A_XLOG(XLOG_ERROR,"connect refused!\n");
    //    exit(-1);
    //}

    //CSapEncoder msg(SAP_PACKET_REQUEST,2,1);
    //msg.CreateAndSetSequence();
    //msg.SetValue(1,3);
    //msg.SetValue(2,5);

    //CSapAgent::Instance()->DoSendMessage(m_nId,msg.GetBuffer(),msg.GetLen());
}
void CAdminSession::DoReceiveMsg(SAdminMsg * pMsg)
{
    SAdminReceiveMsg *pDetail=(SAdminReceiveMsg *)pMsg;
    A_XLOG(XLOG_TRACE,"CAdminSession::DoReceiveMsg,id[%d],len[%d]\n",m_nId,pDetail->m_nLen);

	dump_buffer("receive data\n",pDetail->m_pBuffer,pDetail->m_nLen);

 //   CSapDecoder msgDecode(pDetail->m_pBuffer,pDetail->m_nLen);
	//msgDecode.DecodeBodyAsTLV();

	//const char * buffer = (char *)msgDecode.GetBuffer();
	//int t_len = msgDecode.GetLen();
	//A_XLOG(XLOG_TRACE,"CAdminSession::DoReceiveMsg,id[%d],server_id[%u],message_id[%u],sequeue_id[%u],code[%u]\n",m_nId,msgDecode.GetServiceId(),msgDecode.GetMsgId(),msgDecode.GetSequence(),msgDecode.GetCode());
 //   
	//std::string t_Data1;
	//unsigned int t_Data2 = 0;
	//unsigned int t_Data3 = 0;
	//std::string t_Data4;
	//msgDecode.GetValue(1,t_Data1);
	//msgDecode.GetValue(2,&t_Data2);
	//msgDecode.GetValue(3,&t_Data3);
	//msgDecode.GetValue(4,t_Data4);

	//A_XLOG(XLOG_TRACE,"CAdminSession::%s data1[%s],2[%d],3[%d],4[%s]\n",__FUNCTION__,t_Data1.c_str(),t_Data2,t_Data3,t_Data4.c_str());

 //   CMemoryPool::Instance()->Free(pDetail->m_pBuffer,pDetail->m_nLen);

	//CSapEncoder msg(SAP_PACKET_REQUEST,2,1);
	//msg.CreateAndSetSequence();
	//msg.SetValue(1,t_Data1);
	//msg.SetValue(2,pDetail->m_nId);

	//CSapAgent::Instance()->DoSendMessage(t_Data2,msg.GetBuffer(),msg.GetLen());
}
void CAdminSession::DoPeerCloseMsg(SAdminMsg * pMsg)
{
    SdminPeerCloseMsg *pDetail=(SdminPeerCloseMsg *)pMsg;
    A_XLOG(XLOG_TRACE,"CAdminSession::DoPeerCloseMsg,id[%d]\n",m_nId);
    
    A_XLOG(XLOG_ERROR,"peer close connection!\n");
    //exit(-1);
}
void CAdminSession::DoRequestMsg(SAdminMsg * pMsg)
{
 //   SAdminRequestMsg *pDetail=(SAdminRequestMsg *)pMsg;
	//A_XLOG(XLOG_TRACE,"CAdminSession::DoRequestMsg,id[%d],data:%s\n",m_nId,pDetail->m_strRequest.c_str());

	//CSapEncoder msg(SAP_PACKET_REQUEST,2,1);
	//msg.CreateAndSetSequence();
	//msg.SetValue(1,pDetail->m_strRequest);
	//msg.SetValue(2,5);

	//CSapAgent::Instance()->DoSendMessage(m_nId,msg.GetBuffer(),msg.GetLen());
}

