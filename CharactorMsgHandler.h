#ifndef __LYNX_GAME_SERVER_CHARACTOR_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_CHARACTOR_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct CharactorMsgHandler
	{
		static void onCharactorBuy(const ConnId&connId, CGCharactorBuyReq&msg);
		static void onCharactorEquipFashion(const ConnId&connId, CGFashionEquipReq&msg);
		static void onCharactorChange(const ConnId&connId, CGCharactorSwitchReq&msg);
		static void onPowerRank(const ConnId & connId, CGPowerRankReq& msg);
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_CHARACTOR_MSG_HANDLER_H__