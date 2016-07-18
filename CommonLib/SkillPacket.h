#ifndef __LYNX_COMMON_LIB_SKILL_PACKET_H__
#define __LYNX_COMMON_LIB_SKILL_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_SKILL_LIST_REQ					= sKillMsgBase + 1,
		BOC_SKILL_LIST_RESP					= sKillMsgBase + 2,
		BOC_SKILL_LEVELUP_REQ				= sKillMsgBase + 3,
		BOC_SKILL_LEVELUP_RESP				= sKillMsgBase + 4,
		BOC_SKILL_EQUIPSET_REQ				= sKillMsgBase + 5
	};

	//-----------------CG-------------------
	struct CGSkillListReq
	{
		CGSkillListReq(): mPacketID(BOC_SKILL_LIST_REQ), mCharGUID(0) {}

		UInt16	mPacketID;
		UInt64	mCharGUID;

		LYNX_MESSAGE_2(BOC_SKILL_LIST_REQ, CGSkillListReq, UInt16, mPacketID, UInt64, mCharGUID);
	};

	struct CGSkillLevelUpReq
	{
		CGSkillLevelUpReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_1(BOC_SKILL_LEVELUP_REQ, CGSkillLevelUpReq,std::string, mReqJsonStr);

	};

	struct CGSkillEquipSetReq
	{
		CGSkillEquipSetReq():mPacketID(0),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;

		LYNX_MESSAGE_2(BOC_SKILL_EQUIPSET_REQ,CGSkillEquipSetReq,UInt16, mPacketID,std::string, mReqJsonStr);
	};

	//-----------------GC-------------------
	struct GCSkillListResp
	{
		GCSkillListResp(): mPacketID(BOC_SKILL_LIST_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_SKILL_LIST_RESP, GCSkillListResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCSkillLevelUpResp
	{
		GCSkillLevelUpResp():mPacketID(),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_SKILL_LEVELUP_RESP,GCSkillLevelUpResp,UInt16, mPacketID,std::string, mRespJsonStr);
	};
	


	

		
}// namespace Lynx

#endif // __LYNX_COMMON_LIB_SKILL_PACKET_H__

