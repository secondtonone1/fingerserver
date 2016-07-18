#ifndef __LYNX_COMMON_LIB_HOARSTONE_PACKET_H__
#define __LYNX_COMMON_LIB_HOARSTONE_PACKET_H__
#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_PLAYER_HOARSTONE_LIST_REQ					= sHoarStoneBase + 1,
		BOC_PLAYER_HOARSTONE_LIST_RESP				    = sHoarStoneBase + 2,
		BOC_PLAYER_HOARSTONE_ACTIVE_REQ                 = sHoarStoneBase + 3,
		BOC_PLAYER_HOARSTONE_ACTIVE_RESP                = sHoarStoneBase + 4,
		BOC_PLAYER_HOARSTONE_LVUP_REQ					= sHoarStoneBase + 5,
		BOC_PLAYER_HOARSTONE_LVUP_RESP					= sHoarStoneBase + 6,
		BOC_PLAYER_HOARSTONE_STARUP_REQ					= sHoarStoneBase + 7,
		BOC_PLAYER_HOARSTONE_STARUP_RESP				= sHoarStoneBase + 8,
		BOC_PLAYER_HOARSTONE_ADDPIECE_REQ               = sHoarStoneBase + 9,
		BOC_PLAYER_HOARSTONE_ADDPIECE_RESP				= sHoarStoneBase + 10,
		BOC_PLAYER_HOARSTONE_ADDRUNE_REQ				= sHoarStoneBase + 11,
		BOC_PLAYER_HOARSTONE_ADDRUNE_RESP				= sHoarStoneBase + 12,
		BOC_PLAYER_HOARSTONE_RUNEEQUIP_REQ				= sHoarStoneBase + 13,
		BOC_PLAYER_HOARSTONE_RUNEEQUIP_RESP				= sHoarStoneBase + 14,
		BOC_PLAYER_HOARSTONE_RUNECOMBINE_REQ            = sHoarStoneBase + 15,
		BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP			= sHoarStoneBase + 16
	};

	//-----------------CG-------------------

	struct CGHoarStoneActiveReq
	{
		CGHoarStoneActiveReq():mPacketID(BOC_PLAYER_HOARSTONE_ACTIVE_REQ),mReqJsonStr(""){}

		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_HOARSTONE_ACTIVE_REQ,CGHoarStoneActiveReq,UInt16, mPacketID, std::string, mReqJsonStr);
	};
	
	struct CGHoarStoneLvUpReq
	{
		CGHoarStoneLvUpReq():mPacketID(BOC_PLAYER_HOARSTONE_LVUP_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_HOARSTONE_LVUP_REQ,CGHoarStoneLvUpReq,UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct CGHoarStoneStarUpReq
	{
		CGHoarStoneStarUpReq():mPacketID(BOC_PLAYER_HOARSTONE_STARUP_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_HOARSTONE_STARUP_REQ,CGHoarStoneStarUpReq,UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct CGHoarStoneAddPieceReq
	{
		CGHoarStoneAddPieceReq():mPacketID(BOC_PLAYER_HOARSTONE_ADDPIECE_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_HOARSTONE_ADDPIECE_REQ,CGHoarStoneAddPieceReq,UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct CGHoarStoneAddRuneReq
	{
		CGHoarStoneAddRuneReq():mPacketID(BOC_PLAYER_HOARSTONE_ADDRUNE_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_HOARSTONE_ADDRUNE_REQ,CGHoarStoneAddRuneReq,UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct CGHoarStoneRuneEquipReq
	{
		CGHoarStoneRuneEquipReq():mPacketID(BOC_PLAYER_HOARSTONE_RUNEEQUIP_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_HOARSTONE_RUNEEQUIP_REQ,CGHoarStoneRuneEquipReq,UInt16, mPacketID, std::string, mReqJsonStr);
	};
	
	struct CGHoarStoneRuneCombReq
	{
		CGHoarStoneRuneCombReq():mPacketID(BOC_PLAYER_HOARSTONE_RUNECOMBINE_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_HOARSTONE_RUNECOMBINE_REQ,CGHoarStoneRuneCombReq,UInt16, mPacketID, std::string, mReqJsonStr);
	};

	//-----------------GC-------------------

	struct GCPlayerHoarStoneListResp
	{
		GCPlayerHoarStoneListResp():mPacketID(BOC_PLAYER_HOARSTONE_LIST_RESP),mRespJsonStr(""){}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_HOARSTONE_LIST_RESP, GCPlayerHoarStoneListResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCHoarStoneActiveResp
	{
		GCHoarStoneActiveResp():mPacketID(BOC_PLAYER_HOARSTONE_ACTIVE_RESP),mRespJsonStr(""){}

		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_HOARSTONE_ACTIVE_RESP,GCHoarStoneActiveResp,UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCHoarStoneLvUpResp
	{
		GCHoarStoneLvUpResp():mPacketID(BOC_PLAYER_HOARSTONE_LVUP_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_HOARSTONE_LVUP_RESP,GCHoarStoneLvUpResp,UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCHoarStoneStarResp
	{
		GCHoarStoneStarResp():mPacketID(BOC_PLAYER_HOARSTONE_STARUP_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_HOARSTONE_STARUP_RESP,GCHoarStoneStarResp,UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCHoarStoneAddPieceResp
	{
		GCHoarStoneAddPieceResp():mPacketID(BOC_PLAYER_HOARSTONE_ADDPIECE_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_HOARSTONE_ADDPIECE_RESP,GCHoarStoneStarResp,UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCHoarStoneAddRuneResp
	{
		GCHoarStoneAddRuneResp():mPacketID(BOC_PLAYER_HOARSTONE_ADDRUNE_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_HOARSTONE_ADDRUNE_RESP,GCHoarStoneAddRuneResp,UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCHoarStoneRuneEquipResp
	{
		GCHoarStoneRuneEquipResp():mPacketID(BOC_PLAYER_HOARSTONE_RUNEEQUIP_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_HOARSTONE_RUNEEQUIP_RESP,GCHoarStoneRuneEquipResp,UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCHoarStoneRuneCombResp
	{
		GCHoarStoneRuneCombResp():mPacketID(BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_HOARSTONE_RUNECOMBINE_RESP,GCHoarStoneRuneCombResp,UInt16, mPacketID, std::string, mRespJsonStr);
	};

}// namespace Lynx

#endif // __LYNX_COMMON_LIB_HOARSTONE_PACKET_H__