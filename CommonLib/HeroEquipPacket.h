#ifndef __LYNX_COMMON_LIB_HEROEQUIP_PACKET_H__
#define __LYNX_COMMON_LIB_HEROEQUIP_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_HERO_EQUIPLIST_REQ					= sHeroEquipMsgBase + 1,
		BOC_HERO_EQUIPLIST_RESP					= sHeroEquipMsgBase + 2,
		BOC_HERO_EQUIPENHANCE_REQ				= sHeroEquipMsgBase + 3,
		BOC_HERO_EQUIPENHANCE_RESP				= sHeroEquipMsgBase + 4,
		BOC_HERO_EQUIPGEM_SET_REQ			    = sHeroEquipMsgBase + 5,
		BOC_HERO_EQUIPGEM_SET_RESP				= sHeroEquipMsgBase + 6,
		BOC_HERO_EQUIPGEM_SETONCE_REQ			= sHeroEquipMsgBase + 7,
		BOC_HERO_EQUIPGEM_SETONCE_RESP          = sHeroEquipMsgBase + 8,
		BOC_HERO_EQUIPGEM_MANUAL_ACTIVE_REQ     = sHeroEquipMsgBase + 9,
		BOC_HERO_EQUIPGEM_MANUAL_ACTIVE_RESP	= sHeroEquipMsgBase + 10,
		BOC_HERO_EQUIPGEM_LVUP_REQ			    = sHeroEquipMsgBase + 11,
		BOC_HERO_EQUIPGEM_LVUP_RESP             = sHeroEquipMsgBase + 12,
		BOC_HERO_EQUIPGEM_UNLOAD_REQ            = sHeroEquipMsgBase + 13,
		BOC_HERO_EQUIPGEM_UNLOAD_RESP			= sHeroEquipMsgBase + 14

	};

	//-----------------CG-------------------
	struct CGHeroEquipListReq
	{
		CGHeroEquipListReq(): mPacketID(BOC_HERO_EQUIPLIST_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_HERO_EQUIPLIST_REQ, CGHeroEquipListReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct CGHeroEquipEnhanceReq
	{
		CGHeroEquipEnhanceReq(): mPacketID(BOC_HERO_EQUIPENHANCE_REQ), mReqJsonStr("") {}

		UInt16	mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_HERO_EQUIPENHANCE_REQ, CGHeroEquipEnhanceReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct CGHeroEquipGemSetReq
	{
		CGHeroEquipGemSetReq():mPacketID(BOC_HERO_EQUIPGEM_SET_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_HERO_EQUIPGEM_SET_REQ, CGHeroEquipGemSetReq, UInt16, mPacketID,std::string, mReqJsonStr);
	};

	struct CGHeroEquipGemSetOnceReq
	{
		CGHeroEquipGemSetOnceReq():mPacketID(BOC_HERO_EQUIPGEM_SETONCE_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_HERO_EQUIPGEM_SETONCE_REQ, CGHeroEquipGemSetOnceReq, UInt16, mPacketID,std::string, mReqJsonStr);
	};

	struct CGHeroEquipGemManualActiveReq
	{
		CGHeroEquipGemManualActiveReq():mPacketID(BOC_HERO_EQUIPGEM_MANUAL_ACTIVE_REQ),mReqJsonStr(""){}
	    UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_HERO_EQUIPGEM_MANUAL_ACTIVE_REQ, CGHeroEquipGemManualActiveReq, UInt16, mPacketID,std::string, mReqJsonStr);
	};

	struct CGHeroEquipGemUnloadReq
	{
		CGHeroEquipGemUnloadReq():mPacketID(BOC_HERO_EQUIPGEM_UNLOAD_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_HERO_EQUIPGEM_UNLOAD_REQ, CGHeroEquipGemUnloadReq, UInt16, mPacketID,std::string, mReqJsonStr);
	};

	struct CGHeroEquipGemLvUpReq
	{
		CGHeroEquipGemLvUpReq():mPacketID(BOC_HERO_EQUIPGEM_LVUP_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_HERO_EQUIPGEM_LVUP_REQ, CGHeroEquipGemLvUpReq, UInt16, mPacketID,std::string, mReqJsonStr);
	};

	//-----------------GC-------------------
	struct GCHeroEquipListResp
	{
		GCHeroEquipListResp(): mPacketID(BOC_HERO_EQUIPLIST_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_HERO_EQUIPLIST_RESP, GCHeroEquipListResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCHeroEquipEnhanceResp
	{
		GCHeroEquipEnhanceResp():mPacketID(BOC_HERO_EQUIPENHANCE_RESP), mRespJsonStr(""){}
		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_HERO_EQUIPENHANCE_RESP, GCHeroEquipEnhanceResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCHeroEquipGemSetResp
	{
		GCHeroEquipGemSetResp():mPacketID(BOC_HERO_EQUIPGEM_SET_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_HERO_EQUIPGEM_SET_RESP, GCHeroEquipGemSetResp, UInt16, mPacketID,std::string, mRespJsonStr);
	};

	struct GCHeroEquipGemSetOnceResp
	{
		GCHeroEquipGemSetOnceResp():mPacketID(BOC_HERO_EQUIPGEM_SETONCE_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_HERO_EQUIPGEM_SETONCE_RESP, GCHeroEquipGemSetOnceResp, UInt16, mPacketID,std::string, mRespJsonStr);
	};


	struct GCHeroEquipGemManualActiveResp
	{
		GCHeroEquipGemManualActiveResp():mPacketID(BOC_HERO_EQUIPGEM_MANUAL_ACTIVE_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_HERO_EQUIPGEM_MANUAL_ACTIVE_RESP, GCHeroEquipGemManualActiveResp, UInt16, mPacketID,std::string, mRespJsonStr);
	};

	struct GCHeroEquipGemLvUpResp
	{
		GCHeroEquipGemLvUpResp():mPacketID(BOC_HERO_EQUIPGEM_LVUP_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_HERO_EQUIPGEM_LVUP_RESP, GCHeroEquipGemLvUpResp, UInt16, mPacketID,std::string, mRespJsonStr);
	};

	struct GCHeroEquipGemUnloadResp
	{
		GCHeroEquipGemUnloadResp():mPacketID(BOC_HERO_EQUIPGEM_UNLOAD_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_HERO_EQUIPGEM_UNLOAD_RESP, GCHeroEquipGemUnloadResp, UInt16, mPacketID,std::string, mRespJsonStr);
	};

}// namespace Lynx

#endif // __LYNX_COMMON_LIB_HEROEQUIP_PACKET_H__