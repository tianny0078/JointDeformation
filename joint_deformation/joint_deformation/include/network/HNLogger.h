#pragma once
#include <iostream>
#define HNLOGGER_LEVEL4
#ifndef __FUNCDNAME__
	#define __FUNCDNAME__ ""
#endif
#ifdef HNLOG_LOG_FILENAME
	#define __HNLOG_SHOWFILE() << " " << __FILE__ << " "
#else
	#define __HNLOG_SHOWFILE()
#endif
#ifdef HNLOG_LOG_LINENO
	#define __HNLOG_SHOWLINE() << "+" << __LINE__ << " "
#else
	#define __HNLOG_SHOWLINE()
#endif
#ifdef HNLOG_LOG_FUNCNAME
	#define __HNLOG_SHOWFUNC() << " " << __FUNCDNAME__ << " "
#else
	#define __HNLOG_SHOWFUNC()
#endif

#ifdef HNLOG_LOG_TIME
	#define __HNLOG_SHOWFUNC() << " " << __DATE__ << __TIME__ << " "
#else
	#define __HNLOG_SHOWFUNC()
#endif

#define __HNLOG_WRITE__(msg, type) std::cout<< __HNLOG_SHOWFUNC() type __HNLOG_SHOWFILE() __HNLOG_SHOWLINE() << msg << std::endl;

#ifdef HNLOGGER_LEVEL0
	#define HNLOGGER_LEVEL1
	#ifdef HNLOG_LOG_FUNCNAME
		#define HNLOGGER_ENTERED { __HNLOG_WRITE__("", "ENTERED")
	#else
		#define HNLOGGER_ENTERED { __HNLOG_WRITE__(__FUNCDNAME__, "ENTERED")
	#endif
#else
	#define HNLOGGER_ENTERED { 
#endif

#ifdef HNLOGGER_LEVEL1
	#define HNLOGGER_LEVEL2
	#define HNLOG1(msg)  __HNLOG_WRITE__(msg, "INFO: ")
	#define HNLOGGER_IFELOG1(msg) {  HNLOG1(msg) } else 
	#define HNLOGGER_RBLOG1(msg,retVal) {  HNLOG1(msg) return retVal; }
#else
	#define HNLOG1(msg) ;
	#define HNLOGGER_IFELOG1(msg) {  } else 
	#define HNLOGGER_RBLOG1(msg,retVal) { return retVal; }
#endif

#ifdef HNLOGGER_LEVEL2
	#define HNLOGGER_LEVEL3
	#define HNLOG2(msg) __HNLOG_WRITE__(msg, "NOTIFY: ")
	#define HNLOGGER_ELOG2(msg) else {  HNLOG2(msg) }
	#define HNLOGGER_IFELOG2(msg) {  HNLOG2(msg) } else 
	#define HNLOGGER_RBLOG2(msg,retVal) {  HNLOG2(msg) return retVal; }
#else
	#define HNLOG2(msg) ;
	#define HNLOGGER_ELOG2(msg) 
	#define HNLOGGER_IFELOG2(msg) {  } else 
	#define HNLOGGER_RBLOG2(msg,retVal) { return retVal; }
#endif


#ifdef HNLOGGER_LEVEL3
	#define HNLOGGER_LEVEL4
	#define HNLOG3(msg)  __HNLOG_WRITE__(msg, "WARNING: ")
	#define HNLOGGER_ELOG3(msg) else {  HNLOG3(msg) }
	#define HNLOGGER_IFELOG3(msg) {  HNLOG3(msg) } else 
	#define HNLOGGER_RBLOG3(msg,retVal) {  HNLOG3(msg) return retVal; }
#else
	#define HNLOG3(msg) ;
	#define HNLOGGER_ELOG3(msg)
	#define HNLOGGER_IFELOG3(msg) {  } else 
	#define HNLOGGER_RBLOG3(msg,retVal) { return retVal; }
#endif

#ifdef HNLOGGER_LEVEL4
	#define HNLOG4(msg)  __HNLOG_WRITE__(msg, "ERROR: ")
	#define HNLOGGER_RBLOG4(msg,retVal) {  HNLOG4(msg) return retVal; }
	#define HNLOGGER_ELOG4(msg) else {  HNLOG4(msg) }
	#define HNLOGGER_IFELOG4(msg) {  HNLOG4(msg) } else 
#else
	#define HNLOG4(msg) ;
	#define HNLOGGER_ELOG4(msg) 
	#define HNLOGGER_IFELOG4(msg) {  } else 
	#define HNLOGGER_RBLOG4(msg,retVal) { return retVal; }
#endif


