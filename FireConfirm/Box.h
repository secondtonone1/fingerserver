#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_BOX_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_BOX_MANAGER_H__
// #include "../NetworkLib/MsgHdr.h"
// #include "../PersistThreadMsg.h"
// #include "../CommonLib/MsgfireInfoToJson.h"
// #include "../CommonLib/CommonLib.h"

//#include "../Rapidjson/include/rapidjson/filestream.h"
#include "../Rapidjson/include/rapidjson/document.h"
#include "../Rapidjson/include/rapidjson/prettywriter.h"
#include "../Rapidjson/include/rapidjson/filereadstream.h"
#include "../Rapidjson/include/rapidjson/filewritestream.h"
#include "../Rapidjson/include/rapidjson/stringbuffer.h"
#include "../Rapidjson/include/rapidjson/rapidjson.h"

using namespace rapidjson;

namespace Lynx
{

	class Player;
	
	class BoxManager: public Singleton<BoxManager>
	{
	
	public:
		void testJson();
		
	};



}

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_BOX_MANAGER_H__
