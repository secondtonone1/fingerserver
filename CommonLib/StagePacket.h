#ifndef __LYNX_COMMON_LIB_STAGE_PACKET_H__
#define __LYNX_COMMON_LIB_STAGE_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_CHAPTER_LIST_REQ					= sStageMsgBase + 1,
		BOC_CHAPTER_LIST_RESP					= sStageMsgBase + 2,
		BOC_CHAPTER_LIST_CHANG_REQ				= sStageMsgBase + 3,
		BOC_CHAPTER_LIST_CHANG_RESP				= sStageMsgBase + 4,

		TWELVE_PALACE_LIST_REQ					= sStageMsgBase + 5,//十二宗宫列表
		TWELVE_PALACE_LIST_RESP					= sStageMsgBase + 6,//十二宗宫列表
		TWELVE_PALACE_LIST_CHANG_REQ			= sStageMsgBase + 7,//十二宗宫列表
		TWELVE_PALACE_LIST_CHANG_RESP			= sStageMsgBase + 8,//十二宗宫列表


		CHAPTER_AWARD_RESP			=   sStageMsgBase+9,	//返回副本结算奖励
		CHAPTER_START_REQ			=   sStageMsgBase+10,	// 挑战关卡
		CHAPTER_START_RESP			=   sStageMsgBase+11,	//挑战关卡回复
		CHAPTER_END_REQ				=   sStageMsgBase+12,	//关卡结束
		CHAPTER_END_RESP			=   sStageMsgBase+13,	//关卡结束回复
		CHAPTER_LEAVE_REQ			=   sStageMsgBase+14,	//离开关卡
		CHAPTER_LEAVE_RESP			=   sStageMsgBase+15,	//离开关卡回复

		CHAPTER_COUNTER_REQ			=   sStageMsgBase+16,	//关卡计数器
		CHAPTER_COUNTER_RESP		=   sStageMsgBase+17,	//关卡计数器回复
		CHAPTER_AWARD_CARD_REQ		=   sStageMsgBase+18,	//翻牌
		CHAPTER_AWARD_CARD_RESP		=   sStageMsgBase+19,	//翻牌回复
		CHAPTER_STAR_CHECK_REQ		=   sStageMsgBase+20,	//关卡星数
		CHAPTER_STAR_CHECK_RESP		=   sStageMsgBase+21,	//关卡星数
		CHAPTER_FOOD_REQ			=   sStageMsgBase+22,	//关卡美食屋
		CHAPTER_FOOD_RESP			=   sStageMsgBase+23,	//关卡美食屋返回

		RECIEVED_PACKET_REQ			=   sStageMsgBase+24,	//客户端接收到协议回复

		CHAPTER_RELIVE_REQ			=   sStageMsgBase+26,	//复活
		CHAPTER_RELIVE_RESP			=   sStageMsgBase+27,	//复活返回

		CHAPTER_MOP_UP_REQ			=   sStageMsgBase+40,	//关卡扫荡
		CHAPTER_MOP_UP_RESP			=   sStageMsgBase+41,	//关卡扫荡返回


		CHAPTER_RECORD_REQ			=   sStageMsgBase+50,	//破关纪录
		CHAPTER_RECORD_RESP			=   sStageMsgBase+51,	//破关纪录返回

		CHAPTER_TWELVE_PALACE_RECORD_REQ			=   sStageMsgBase+52,	//破关纪录十二宗宫
		CHAPTER_TWELVE_PALACE_RECORD_RESP			=   sStageMsgBase+53,	//破关纪录返回十二宗宫

		CHAPTER_BOX_OPEN_REQ			=   sStageMsgBase+60,	//开章节宝箱
		CHAPTER_BOX_OPEN_RESP			=   sStageMsgBase+61,	//开章节宝箱返回

		CHAPTER_STRENGTH_REQ			=   sStageMsgBase+70,	//体力请求
		CHAPTER_STRENGTH_RESP			=   sStageMsgBase+71,	//体力返回

		COURAGE_CHALLENGE_REQ			=   sStageMsgBase+80,	//勇气挑战请求
		COURAGE_CHALLENG_RESP			=   sStageMsgBase+81,	//勇气挑战返回
		CAT_COIN_REQ					=   sStageMsgBase+82,	//硬币盒请求
		CAT_COIN_RESP					=   sStageMsgBase+83,	//硬币盒返回
		COURAGE_CHALLENG_BEGIN_REQ			=   sStageMsgBase+84,	//勇气挑战开始
		COURAGE_CHALLENG_BEGIN_RESP			=   sStageMsgBase+85,	//勇气挑战开始返回
		COURAGE_CHALLENG_END_REQ			=   sStageMsgBase+86,	//勇气挑战结束
		COURAGE_CHALLENG_END_RESP			=   sStageMsgBase+87,	//勇气挑战返回

		BATTLE_START_REQ			=   sStageMsgBase+100,	//战斗开始
		BATTLE_START_RESP			=   sStageMsgBase+101,	//战斗开始回复

		CHAPTER_END_REQ1				=   sStageMsgBase+122,	//关卡结束
	};

	//-----------------CG-------------------
	struct CGStageListReq
	{
		CGStageListReq(): mPacketID(BOC_CHAPTER_LIST_REQ), mCharGUID(0) {}

		UInt16	mPacketID;
		UInt64	mCharGUID;

		LYNX_MESSAGE_2(BOC_CHAPTER_LIST_REQ, CGStageListReq, UInt16, mPacketID, UInt64, mCharGUID);
	};

	//-----------------GC-------------------
	struct GCStageListResp
	{
		GCStageListResp(): mPacketID(BOC_CHAPTER_LIST_RESP), mRespJsonStr("") {}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_CHAPTER_LIST_RESP, GCStageListResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

}// namespace Lynx

#endif // __LYNX_COMMON_LIB_STAGE_PACKET_H__