//
//  main.cpp
//  mystack
//
//  Created by mike on 14-6-22.
//  Copyright (c) 2014年 km. All rights reserved.
//

#include <stdio.h>
#include <string>
#include <iostream>
#include "AdminSession.h"
#include "AdminLogHelper.h"
#include "SapLogHelper.h"
#include "ISapStack.h"
#include "ISapServer.h"
#include <stdlib.h>

int main(int argc, const char * argv[])
{

	/*arg valid*/
	if(argc<2)
	{
		printf("Usage: sapadmin port\n");
		return -1;
	}
	/*set log*/
	XLOG_INIT("log.properties",true);
	XLOG_REGISTER(SAP_STACK_MODULE,"sap_stack");
	XLOG_REGISTER(ADMIN_MODULE,"sap_bussiness");
	A_XLOG(XLOG_DEBUG,"test\n");


	/*stack start*/
	GetSapStackInstance()->Start(3);

	/*login server*/
	int nPort=atoi(argv[1]);
	CAdminSession *session=new CAdminSession;
	GetSapServerInstance()->SetCallback(session);
	session->Start();   

	/*starting...*/
	//session->OnLoginServer(argv[1],nPort);
	session->OnStartServer(nPort);
	while(1)
	{
		sleep(1);
		//A_XLOG(XLOG_DEBUG,"sleep\n");
	}
	
    return 0;
}

