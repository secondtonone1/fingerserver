#ifndef __LYNX_COMMON_LIB_RHYME_PACKET_H__
#define __LYNX_COMMON_LIB_RHYME_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_RHYME_REQ					= sRhymeMsgBase + 1,
		BOC_RHYME_RESP					= sRhymeMsgBase + 2,
		BOC_RHYME_ENHANCE_REQ           = sRhymeMsgBase + 3,
		BOC_RHYME_ENHANCE_RESP          = sRhymeMsgBase + 4,

		BOC_RHYME_ACUPOINTACTIVE_REQ    = sRhymeMsgBase + 7,
		BOC_RHYME_ACUPOINTACTIVE_RESP	= sRhymeMsgBase + 8,
		BOC_RHYME_SKILLSET_REQ			= sRhymeMsgBase + 9,
		BOC_RHYME_SKILLSET_RESP			= sRhymeMsgBase + 10,

		BOC_RHYME_STEP_REQ              = sRhymeMsgBase + 11,
		BOC_RHYME_STEP_RESP             = sRhymeMsgBase + 12
		
	};

	//-----------------CG-------------------
	struct CGRhymeReq
	{
		CGRhymeReq(): mPacketID(BOC_RHYME_REQ), mCharGUID(0) {}

		UInt16	mPacketID;
		UInt64	mCharGUID;
		
		LYNX_MESSAGE_2(BOC_RHYME_REQ, CGRhymeReq, UInt16, mPacketID, UInt64, mCharGUID);
	};

	//-----------------GC-------------------
	struct GCRhymeResp
	{
		GCRhymeResp(): mPacketID(BOC_RHYME_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_RHYME_RESP, GCRhymeResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct CGRhymeEnhanceReq
	{
		CGRhymeEnhanceReq():mPacketID(BOC_RHYME_ENHANCE_REQ),mReqJsonStr(""){}
		UInt16	mPacketID;
		std::string  mReqJsonStr;
		LYNX_MESSAGE_2(BOC_RHYME_ENHANCE_REQ, CGRhymeEnhanceReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct GCRhymeEnhanceResp
	{
		GCRhymeEnhanceResp():mPacketID(BOC_RHYME_ENHANCE_RESP),mRespJsonStr(""){}
		UInt16	mPacketID;
		std::string  mRespJsonStr;
		LYNX_MESSAGE_2(BOC_RHYME_ENHANCE_RESP, GCRhymeEnhanceResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	

	struct CGRhymeAcupointActiveReq
	{
		CGRhymeAcupointActiveReq():mPacketID(BOC_RHYME_ACUPOINTACTIVE_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_RHYME_ACUPOINTACTIVE_REQ, CGRhymeAcupointActiveReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct CGRhymeStepReq
	{
		CGRhymeStepReq():mPacketID(BOC_RHYME_STEP_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_RHYME_STEP_REQ, CGRhymeStepReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct GCRhymeAcupointActiveResp
	{
		GCRhymeAcupointActiveResp():mPacketID(BOC_RHYME_ACUPOINTACTIVE_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_RHYME_ACUPOINTACTIVE_RESP, GCRhymeAcupointActiveResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct CGRhymeSkillSetReq
	{
		CGRhymeSkillSetReq():mPacketID(BOC_RHYME_SKILLSET_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(BOC_RHYME_SKILLSET_REQ, CGRhymeSkillSetReq, UInt16, mPacketID, std::string, mReqJsonStr);		
	};

	struct GCRhymeSkillResp
	{
		GCRhymeSkillResp():mPacketID(BOC_RHYME_SKILLSET_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_RHYME_SKILLSET_RESP, GCRhymeSkillResp, UInt16, mPacketID, std::string , mRespJsonStr);

	};

	struct GCRhymeStepResp
	{
		GCRhymeStepResp():mPacketID(BOC_RHYME_STEP_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(BOC_RHYME_STEP_RESP, GCRhymeStepResp, UInt16, mPacketID, std::string , mRespJsonStr);

	};


}// namespace Lynx

#endif // __LYNX_COMMON_LIB_RHYME_PACKET_H__