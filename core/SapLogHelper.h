//
//  SapLogHelper.h
//  mystack
//
//  Created by mike on 14-6-22.
//  Copyright (c) 2014年 km. All rights reserved.
//

#ifndef mystack_SapLogHelper_h
#define mystack_SapLogHelper_h

#ifdef __MACOS__

#define SS_XLOG(...) ""
#define XLOG_DEBUG ""
#define XLOG_INFO ""
#define XLOG_ERROR ""
#define XLOG_TRACE ""
#define XLOG_WARNING ""

#else

#include "LogManager.h"

const int SAP_STACK_MODULE=7;
DEFINE_MODULE_XLOG(SAP_STACK_MODULE,SS_XLOG)
#define SS_SLOG(Level,Event) SLOG(SAP_STACK_MODULE,Level,Event)

const int SAPPER_STACK_MODULE=21;
DEFINE_MODULE_XLOG(SAPPER_STACK_MODULE,SP_XLOG)
#define SP_SLOG(Level,Event) SLOG(SAPPER_STACK_MODULE,Level,Event)

inline bool IsEnableLevel(int nModule,const XLOG_LEVEL level)
{
	sdo::common::ILog *pImp=sdo::common::LogManager::instance()->get_log_module(nModule);
	if(pImp!=NULL&&pImp->is_enable_level(level))
		return true;
	else 
		return false;
}

#endif



#endif
