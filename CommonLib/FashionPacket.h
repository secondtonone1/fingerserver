#ifndef __LYNX_COMMON_LIB_FASHION_PACKET_H__
#define __LYNX_COMMON_LIB_FASHION_PACKET_H__
#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_PLAYER_FASHION_LIST_REQ					= sFashionMsgBase + 1,
		BOC_PLAYER_FASHION_LIST_RESP				= sFashionMsgBase + 2,
		BOC_PLAYER_FASHION_EQUIP_REQ				= sFashionMsgBase + 3,
		BOC_PLAYER_FASHION_EQUIP_RESP				= sFashionMsgBase + 4,
		BOC_PLAYER_FASHION_BUY_REQ					= sFashionMsgBase + 5,
		BOC_PLAYER_FASHION_BUY_RESP				= sFashionMsgBase + 6
	};

	//-----------------CG-------------------
	struct CGFashionEquipReq
	{
		CGFashionEquipReq(): mPacketID(BOC_PLAYER_FASHION_EQUIP_REQ), mReqJsonStr("") {}

		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_PLAYER_FASHION_EQUIP_REQ, CGFashionEquipReq, UInt16, mPacketID,std::string, mReqJsonStr);	
	};

	struct CGFashionBuyReq
	{
		CGFashionBuyReq(): mPacketID(BOC_PLAYER_FASHION_BUY_REQ), mReqJsonStr("") {}

		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_PLAYER_FASHION_BUY_REQ, CGFashionBuyReq, UInt16, mPacketID,std::string, mReqJsonStr);	
	};



	//-----------------GC-------------------
	struct GCFashionEquipResp
	{
		GCFashionEquipResp(): mPacketID(BOC_PLAYER_FASHION_EQUIP_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_FASHION_EQUIP_RESP, GCFashionEquipResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCFashionBuyResp
	{
		GCFashionBuyResp(): mPacketID(BOC_PLAYER_FASHION_BUY_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_FASHION_BUY_RESP, GCFashionBuyResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};
	

	struct GCPlayerFashionListResp
	{
		GCPlayerFashionListResp():mPacketID(BOC_PLAYER_FASHION_LIST_RESP),mRespJsonStr(""){}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_FASHION_LIST_RESP, GCPlayerFashionListResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

}// namespace Lynx

#endif // __LYNX_COMMON_LIB_FASHION_PACKET_H__

