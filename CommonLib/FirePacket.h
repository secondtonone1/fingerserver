#ifndef __LYNX_COMMON_LIB_FIRE_PACKET_H__
#define __LYNX_COMMON_LIB_FIRE_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{

		FORBIDDEN_LOGIN				=	sFireMsgBase+1,	//禁止登陆
		FIRE_CONDITION_RESP  		=   sFireMsgBase+2,	//发送玩家监控条件
		FIRE_CONFIRM_DATA_REQ		=   sFireMsgBase+4,	//监控数据
		FIRE_CONFIRM_DATA_RESP		=   sFireMsgBase+5,	//发送玩家监控结果

		WELFALE_MARKET_BEGIN_REQ			=   sWelfaleMarketBase+32,	//招福集市挑战
		WELFALE_MARKET_BEGIN_RESP			=   sWelfaleMarketBase+33,	//招福集市挑战返回
		WELFALE_MARKET_END_REQ				=   sWelfaleMarketBase+34,	//招福集市结束
		WELFALE_MARKET_END_RESP				=   sWelfaleMarketBase+35,	//招福集市结算

		TWELVE_PALACE_UNLOCK_REQ			=   sWelfaleMarketBase+60,	//十二宗宫解锁
		TWELVE_PALACE_UNLOCK_RESP			=   sWelfaleMarketBase+61,	//十二宗宫解锁返回
		TWELVE_PALACE_BEGIN_REQ				=   sWelfaleMarketBase+62,	//十二宗宫挑战
		TWELVE_PALACE_BEGIN_RESP			=   sWelfaleMarketBase+63,	//十二宗宫挑战返回
		TWELVE_PALACE_END_REQ				=   sWelfaleMarketBase+64,	//十二宗宫结束
		TWELVE_PALACE_END_RESP				=   sWelfaleMarketBase+65,	//十二宗宫结算

		TWELVE_PALACE_RANDOMENTRY_REQ				=   sWelfaleMarketBase+66,	//乱入者请求
		TWELVE_PALACE_RANDOMENTRY_RESP				=   sWelfaleMarketBase+67,	//乱入者返回

		

	};


}// namespace Lynx

#endif // __LYNX_COMMON_LIB_FIRE_PACKET_H__