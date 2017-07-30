#ifndef __LYNX_COMMON_LIB_SERVANT_PACKET_H__
#define __LYNX_COMMON_LIB_SERVANT_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_SERVANTDATA_LIST_REQ					= sServantMsgBase + 1,
		BOC_SERVANTDATA_LIST_RESP					= sServantMsgBase + 2,
		BOC_SERVANT_COMBINE_REQ				        = sServantMsgBase + 3,
		BOC_SERVANT_COMBINE_RESP				    = sServantMsgBase + 4,
		BOC_SERVANT_LVUP_REQ                        = sServantMsgBase + 5,
		BOC_SERVANT_LVUP_RESP						= sServantMsgBase + 6,
		BOC_SERVANT_STARUP_REQ                      = sServantMsgBase + 7,
		BOC_SERVANT_FLOORUP_REQ                     = sServantMsgBase + 8,
		BOC_SERVANT_FLOORUP_RESP                    = sServantMsgBase + 9,
		BOC_STREASURE_COMBINE_REQ                   = sServantMsgBase + 10,
		BOC_STREASURE_COMBINE_RESP                  = sServantMsgBase + 11,
		BOC_STREASURE_EQUIP_REQ                     = sServantMsgBase + 12,
		BOC_STREASURE_EQUIP_RESP                    = sServantMsgBase + 13,
		BOC_SERVANTBATTLE_OPEN_REQ                  = sServantMsgBase + 14,
		BOC_SERVANTBATTLE_OPEN_RESP                 = sServantMsgBase + 15,
		BOC_ASSISTBATTLE_ENHANCE_REQ                = sServantMsgBase + 16,
		BOC_ASSISTBATTLE_ENHANCE_RESP               = sServantMsgBase + 17,
		BOC_SERVANTBATTLE_SET_REQ                   = sServantMsgBase + 18,
		BOC_SERVANTBATTLE_SET_RESP                  = sServantMsgBase + 19,
		BOC_ASSISTBATTLE_ONSET_REQ                  = sServantMsgBase + 20,
		BOC_SERVANTGET_NOTIFY						= sServantMsgBase + 22,
		BOC_SERVANTSWITCH_REQ						= sServantMsgBase + 23,
		BOC_SERVANTSWITCH_RESP						= sServantMsgBase + 24,
		BOC_SERVANT_EQUIPONCE_REQ				= sServantMsgBase + 25,
		BOC_SERVANT_EQUIPONCE_RESP				= sServantMsgBase + 26,
		BOC_SERVANT_INFOLOCK_REQ					= sServantMsgBase + 31,
		BOC_SERVANT_INFOLOCK_RESP					= sServantMsgBase + 32
		
		
		
	};

	//-----------------CG-------------------
	struct CGServantDataListReq
	{
		CGServantDataListReq(): mPacketID(BOC_SERVANTDATA_LIST_REQ), mCharGUID(0) {}

		UInt16	mPacketID;
		UInt64	mCharGUID;

		LYNX_MESSAGE_2(BOC_SERVANTDATA_LIST_REQ, CGServantDataListReq, UInt16, mPacketID, UInt64, mCharGUID);
	};

	struct CGServantCombineReq
	{
		CGServantCombineReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_SERVANT_COMBINE_REQ, CGServantCombineReq,UInt16 ,mPacketID,std::string, mReqJsonStr);

	};

	struct CGServantLvUpReq
	{
		CGServantLvUpReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_SERVANT_LVUP_REQ, CGServantLvUpReq,UInt16 ,mPacketID,std::string, mReqJsonStr);

	};

	
	struct CGServantStarUpReq
	{
		CGServantStarUpReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_SERVANT_STARUP_REQ, CGServantStarUpReq,UInt16 ,mPacketID,std::string, mReqJsonStr);

	};
	
	struct CGServantFloorUpReq
	{
		CGServantFloorUpReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_SERVANT_FLOORUP_REQ, CGServantFloorUpReq,UInt16 ,mPacketID,std::string, mReqJsonStr);

	};

	struct CGTreasureCombineReq
	{
		CGTreasureCombineReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_STREASURE_COMBINE_REQ, CGTreasureCombineReq,UInt16 ,mPacketID,std::string, mReqJsonStr);

	};

	struct CGTreasureEquipReq
	{
		CGTreasureEquipReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_STREASURE_EQUIP_REQ, CGTreasureEquipReq,UInt16 ,mPacketID,std::string, mReqJsonStr);

	};
	
	struct CGServantBattleOpenReq
	{
		CGServantBattleOpenReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_SERVANTBATTLE_OPEN_REQ, CGServantBattleOpenReq,UInt16 ,mPacketID,std::string, mReqJsonStr);

	};

	struct CGAssistBattleEnhanceReq
	{
		CGAssistBattleEnhanceReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_ASSISTBATTLE_ENHANCE_REQ, CGAssistBattleEnhanceReq,UInt16 ,mPacketID,std::string, mReqJsonStr);

	};

	struct CGServantBattleSetReq
	{
		CGServantBattleSetReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_SERVANTBATTLE_SET_REQ, CGServantBattleSetReq,UInt16 ,mPacketID,std::string, mReqJsonStr);

	};

	struct CGAssistBattleOnSetReq
	{
		CGAssistBattleOnSetReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_ASSISTBATTLE_ONSET_REQ, CGAssistBattleOnSetReq,UInt16 ,mPacketID,std::string, mReqJsonStr);

	};
	  	
	struct CGServantSwitchReq
	{
		CGServantSwitchReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_SERVANTSWITCH_REQ, CGServantSwitchReq,UInt16 ,mPacketID,std::string, mReqJsonStr);

	};

	struct CGServantEquipOnceReq
	{
		CGServantEquipOnceReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_SERVANT_EQUIPONCE_REQ, CGServantEquipOnceReq,UInt16 ,mPacketID,std::string, mReqJsonStr);
	};

	struct CGServantInfoLockReq
	{
		CGServantInfoLockReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_SERVANT_INFOLOCK_REQ, CGServantInfoLockReq,UInt16 ,mPacketID,std::string, mReqJsonStr);
	};

	

	//-----------------GC-------------------
	struct GCServantDataListResp
	{
		GCServantDataListResp(): mPacketID(BOC_SERVANTDATA_LIST_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_SERVANTDATA_LIST_RESP, GCServantDataListResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCServantCombineResp
	{
		GCServantCombineResp():mPacketID(),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_SERVANT_COMBINE_RESP,GCServantCombineResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};

	struct GCServantLvUpResp
	{
		GCServantLvUpResp():mPacketID(),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_SERVANT_LVUP_RESP,GCServantLvUpResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};

	struct GCServantFloorUpResp
	{
		GCServantFloorUpResp():mPacketID(),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_SERVANT_FLOORUP_RESP,GCServantFloorUpResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};

	struct GCTreasureCombineResp
	{
		GCTreasureCombineResp():mPacketID(),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_STREASURE_COMBINE_RESP,GCTreasureCombineResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};

	struct GCTreasureEquipResp
	{
		GCTreasureEquipResp():mPacketID(),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_STREASURE_EQUIP_RESP,GCTreasureEquipResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};

	struct GCServantBattleOpenResp
	{
		GCServantBattleOpenResp():mPacketID(),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_SERVANTBATTLE_OPEN_RESP,GCServantBattleOpenResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};

    struct GCAssistBattleEnhanceResp
	{
		GCAssistBattleEnhanceResp():mPacketID(),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_ASSISTBATTLE_ENHANCE_RESP,GCAssistBattleEnhanceResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};

	struct GCServantBattleSetResp
	{
		GCServantBattleSetResp():mPacketID(),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_SERVANTBATTLE_SET_RESP,GCServantBattleSetResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};

	struct GCServantGetNotify
	{
		GCServantGetNotify():mPacketID(),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_SERVANTGET_NOTIFY,GCServantGetNotify,UInt16, mPacketID,std::string, mRespJsonStr);
	};


	struct GCServantSwitchResp
	{
		GCServantSwitchResp():mPacketID(),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_SERVANTSWITCH_RESP,GCServantSwitchResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};
	
	struct GCServantEquipOnceResp
	{
		GCServantEquipOnceResp():mPacketID(BOC_SERVANT_EQUIPONCE_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_SERVANT_EQUIPONCE_RESP,GCServantEquipOnceResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};

	struct GCServantInfoLockResp
	{
		GCServantInfoLockResp():mPacketID(BOC_SERVANT_INFOLOCK_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_SERVANT_INFOLOCK_RESP,GCServantInfoLockResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};

	

}// namespace Lynx

#endif // __LYNX_COMMON_LIB_SERVANT_PACKET_H__