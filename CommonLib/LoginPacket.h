#ifndef __LYNX_COMMON_LIB_LOGIN_PACKET_H__
#define __LYNX_COMMON_LIB_LOGIN_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_GAME_LOGIN_REQ					= sLoginMsgBase + 1,//2001
		BOC_GAME_LOGIN_RESP					= sLoginMsgBase + 2,
		BOC_GAME_CREATEPLAYER_REQ           = sLoginMsgBase + 3,
		BOC_GAME_CREATEPLAYER_RESP			= sLoginMsgBase + 4,
		BOC_ACCOUNT_LOGIN_REQ					= sLoginMsgBase + 5,
		BOC_ACCOUNT_LOGIN_RESP                = sLoginMsgBase + 6,


		OTHER_PLAYER_ATTR_REQ          = sLoginMsgBase + 10,
		OTHER_PLAYER_ATTR_RESP         = sLoginMsgBase + 11,

	};

	//-----------------CG-------------------
	struct CGGameLoginReq
	{
		CGGameLoginReq(): mPacketID(BOC_GAME_LOGIN_REQ), mStrGUID("") {}

		UInt16	mPacketID;
		//UInt64	mCharGUID;
		std::string mStrGUID;
		LYNX_MESSAGE_2(BOC_GAME_LOGIN_REQ, CGGameLoginReq, UInt16, mPacketID, std::string, mStrGUID);
	};

	struct CGGameCreatePlayerReq
	{
		CGGameCreatePlayerReq():mPacketID(),mReqStr(""){}
		
		UInt16 mPacketID;
		std::string mReqStr;
		LYNX_MESSAGE_2(BOC_GAME_CREATEPLAYER_REQ,CGGameCreatePlayerReq,UInt16, mPacketID,std::string, mReqStr);
	};

	struct CGAccountLoginReq
	{
		CGAccountLoginReq():mPacketID(),mReqStr(""){}

		UInt16 mPacketID;
		std::string mReqStr;
		LYNX_MESSAGE_2(BOC_ACCOUNT_LOGIN_REQ,CGAccountLoginReq,UInt16, mPacketID,std::string, mReqStr);
	};
	
	

	//-----------------GC-------------------
	struct GCGameLoginResp
	{
		GCGameLoginResp(): mPacketID(BOC_GAME_LOGIN_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_GAME_LOGIN_RESP, GCGameLoginResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GCGameCreatePlayerResp
	{
		GCGameCreatePlayerResp():mPacketID(),mRespStr(""){}

		UInt16 mPacketID;
		std::string mRespStr;
		LYNX_MESSAGE_2(BOC_GAME_CREATEPLAYER_RESP,GCGameCreatePlayerResp,UInt16, mPacketID,std::string, mRespStr);
	};

	struct GCAccountLoginResp
	{
		GCAccountLoginResp():mPacketID(),mRespStr(""){}

		UInt16 mPacketID;
		std::string mRespStr;
		LYNX_MESSAGE_2(BOC_ACCOUNT_LOGIN_RESP,GCAccountLoginResp,UInt16, mPacketID,std::string, mRespStr);
	};
	



	struct CGOterPlayerAttr
	{
		CGOterPlayerAttr(): playerID(0){}

		UInt32 playerID;

		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				playerID = root["playerID"].asUInt();				
			}
		}
		LYNX_MESSAGE_1(OTHER_PLAYER_ATTR_REQ, CGOterPlayerAttr,std::string, jsonStr);
	};

	struct oterPlayerAttrResp	
	{
		oterPlayerAttrResp():str("") {}

		std::string str;

		std::string convertDataToJson()
		{
// 			Json::Value root;     	
// 			root["typeID"] = Json::Value(typeID);
// 			
// 			Json::FastWriter writer;  
// 			std::string strWrite = writer.write(root);
			return str;
		}
	};



}// namespace Lynx

#endif // __LYNX_COMMON_LIB_LOGIN_PACKET_H__
