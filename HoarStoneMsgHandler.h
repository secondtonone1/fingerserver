#ifndef __LYNX_GAME_SERVER_HOARSTONE_MSG_HANDLER_H__
#define __LYNX_GAME_SERVER_HOARSTONE_MSG_HANDLER_H__

#include "CommonLib.h"

namespace Lynx
{
	struct HoarStoneMsgHandler
	{
		static void onHoarStoneActiveReq(const ConnId&, CGHoarStoneActiveReq&);
		static void onHoarStoneLvUpReq(const ConnId&, CGHoarStoneLvUpReq&);
		static void onHoarStoneStarUpReq(const ConnId&, CGHoarStoneStarUpReq&);
		static void onHoarStoneAddPieceReq(const ConnId&, CGHoarStoneAddPieceReq&);
		static void onHoarStoneAddRuneReq(const ConnId&, CGHoarStoneAddRuneReq&);
		static void onHoarStoneRuneEquipReq(const ConnId&, CGHoarStoneRuneEquipReq&);
		static void onHoarStoneRuneCombineReq(const ConnId&, CGHoarStoneRuneCombReq&);
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_HOARSTONE_MSG_HANDLER_H__