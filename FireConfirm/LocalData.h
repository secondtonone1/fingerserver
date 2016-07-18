#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_LOCAL_DATA_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_LOCAL_DATA_MANAGER_H__
#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "../CommonLib/CommonLib.h"

namespace Lynx
{

	
	class LocalDataManager: public Singleton<LocalDataManager>
	{
	
	public:
		void setTwelvePalaceLocalData(ConnId connId,UInt32 key,UInt32 value);
		
	};









}

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_LOCAL_DATA_MANAGER_H__
