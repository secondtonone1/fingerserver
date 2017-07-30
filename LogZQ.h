#ifndef __LYNX_GAME_SERVER_LOGZQ_SYSTEM_H__
#define __LYNX_GAME_SERVER_LOGZQ_SYSTEM_H__

#include "MainThread.h"
namespace Lynx
 {


	class LogZQ : public MainThread, public Singleton<LogZQ>
	{
	public:

		void write_log();
	
// 		void LogZQ::Ourpalm_Init();
// 		public String Ourpalm_getServiceCode(final String gameResVer) ;
// 		void Ourpalm_ServiceCode_Entry.getInstance(this).Ourpalm_getServiceCode(""); 
// 		public void SetLogs(boolean log) ;
// 		void Ourpalm_ServiceCode_Entry.getInstance(this).SetLogs(true);

	

	};
	
} 

#endif  //__LYNX_GAME_SERVER_LOGZQ_SYSTEM_H__

