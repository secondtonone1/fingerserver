#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_BASECHANG_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_BASECHANG_MANAGER_H__
#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "FireConfirmManager.h"
#include "RankGame.h"

namespace Lynx
{

	class Player;


	
	class BaseChangManager : public Singleton<BaseChangManager>
	{
	
	public:
		void checkStringToValue(UInt32 &dest,Json::Value &src);
		void checkStringToValue(UInt64 &dest,Json::Value &src);
// 		void CheckStringToValue(int dest,String src);
// 		void CheckStringToValue(double dest,String src);
// 		void CheckStringToValue(float dest,String src);

		void cGChapterEndconvertJsonToData(CGChapterEnd &chapterEnd);
		void rankGameEndconvertJsonToData(RankGameEndReq &rankGameEndReq);

		
	};









}

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_BASECHANG_MANAGER_H__
