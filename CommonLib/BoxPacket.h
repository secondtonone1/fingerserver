#ifndef __LYNX_COMMON_LIB_BOX_PACKET_H__
#define __LYNX_COMMON_LIB_BOX_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		OPEN_GIFT_REQ                          = sItemMsgBase + 200,//开礼包
		OPEN_GIFT_RESP                         = sItemMsgBase + 201,//返回开礼包结果
		OPEN_BOX_REQ                           = sItemMsgBase + 202,//开箱子
		OPEN_BOX_RESP                          = sItemMsgBase + 203,//返回开箱子结果


		OPEN_LOTTERY_DRAW_REQ                  =	sItemMsgBase + 206,//抽奖
		OPEN_LOTTERY_DRAW_RESP                 =	sItemMsgBase + 207,//抽奖
		PROPERTY_CHAGE_REQ					   =   sItemMsgBase + 210,	//属性改变
		PROPERTY_CHAGE_RESP					   =   sItemMsgBase + 211,	//属性改变回复
		PROPERTYS_CHAGE_REQ					   =   sItemMsgBase + 212,	//属性改变
		PROPERTYS_CHAGE_RESP				   =   sItemMsgBase + 213,	//属性改变回复

		OPEN_LOTTERY_MSG_REQ                  =	sItemMsgBase + 220,//抽奖一次
		OPEN_LOTTERY_MSG_RESP                 =	sItemMsgBase + 221,//抽奖一次返回
		OPEN_REWARD_LOTTERY_REQ                 =	sItemMsgBase + 222,//抽奖一次
		OPEN_REWARD_LOTTERY_RESP                =	sItemMsgBase + 223,//抽奖一次返回
	


	};

	enum 
	{
		

		TOWER_INFO_REQ                 =	sClimTowerMsgBase + 0,	//爬塔信息
		TOWER_INFO_RESP                =	sClimTowerMsgBase + 1,	//爬塔信息返回
		TOWER_BEGIN_REQ				   =    sClimTowerMsgBase + 2,	//爬塔开始
		TOWER_BEGIN_RESP			   =    sClimTowerMsgBase + 3,	//爬塔开始返回
		IN_TOWER_CHOICE_REQ			   =    sClimTowerMsgBase + 4,	//爬塔中的选择
		IN_TOWER_CHOICE_RESP		   =    sClimTowerMsgBase + 5,	//爬塔中的选择
		TOWER_FLOOR_END_REQ			   =    sClimTowerMsgBase + 6,	//爬塔一层结束
		TOWER_FLOOR_END_RESP		   =    sClimTowerMsgBase + 7,	//爬塔一层结束返回
		TOWER_END_REQ				   =    sClimTowerMsgBase + 8,	//爬塔结束
		TOWER_END_RESP				   =    sClimTowerMsgBase + 9,	//爬塔结束返回
		TOWER_RELIVE_REQ			   =    sClimTowerMsgBase + 10,	//爬塔复活请求
		TOWER_RELIVE_RESP			   =    sClimTowerMsgBase + 11,	//爬塔复活返回
		TOWER_MOPUP_REQ				 =    sClimTowerMsgBase + 12,	//爬塔扫荡请求
		TOWER_MOPUP_RESP			   =    sClimTowerMsgBase + 13,	//爬塔扫荡返回

		TOWER_BUFF_REQ				   =    sClimTowerMsgBase + 112,	//爬塔复活返回
		TOWER_BUFF_RESP				   =    sClimTowerMsgBase + 113,	//爬塔复活返回

	};

	enum 
	{
		SHOP_INFO_REQ                 =		sShopBase + 0,	//商城信息
		SHOP_INFO_RESP                =		sShopBase + 1,	//商城信息返回
		SHOP_BUY_REQ				  =     sShopBase + 2,	//商城购买
		SHOP_BUY_RESP				  =     sShopBase + 3,	//商城购买返回	

		STRENGTH_BUY_REQ              =		sShopBase + 4,	//体力购买
		STRENGTH_BUY_RESP             =		sShopBase + 5,	//体力购买返回

		COIN_BUY_REQ                  =		sShopBase + 6,	//金币购买
		COIN_BUY_RESP                 =		sShopBase + 7,	//金币购买返回


		COURAGE_SHOP_INFO_REQ                 =		sShopBase + 10,	//勇气商店信息
		COURAGE_SHOP_INFO_RESP                =		sShopBase + 11,	//勇气商店信息返回
		COURAGE_SHOP_BUY_REQ				  =     sShopBase + 12,	//勇气商店购买
		COURAGE_SHOP_BUY_RESP				  =     sShopBase + 13,	//勇气商店购买返回	
	};
	
	enum 
	{
// 		GROWTH_FOUND_REQ                 =		sInLineActivityBase + 0,	//成长基金
// 		GROWTH_FOUND_RESP                =		sInLineActivityBase + 1,	//成长基金
// 
// 		MONTH_SIGN_REQ					 =     sInLineActivityBase + 2,		//月签到
// 		MONTH_SIGN_RESP			  		 =     sInLineActivityBase + 3,		//月签到	
// 
// 		ONLINE_WELFARE_REQ              =		sInLineActivityBase + 4,	//在线奖励
// 		ONLINE_WELFARE_RESP             =		sInLineActivityBase + 5,	//在线奖励
// 
// 		PEOPLE_WELFARE_REQ              =		sInLineActivityBase + 6,	//全民福利
// 		PEOPLE_WELFARE_RESP             =		sInLineActivityBase + 7,	//全民福利
// 
// 		SEVENDAY_LOGIN_AWARD_REQ       =		sInLineActivityBase + 8,	//7天登录奖励
// 		SEVENDAY_LOGIN_AWARD_RESP      =		sInLineActivityBase + 9,	//7天登录奖励
// 
// 		SEVENDAY_TASK_REQ              =		sInLineActivityBase + 10,	//7天任务奖励
// 		SEVENDAY_TASK_RESP             =		sInLineActivityBase + 11,	//7天任务奖励
// 
// 		TIME_AWARD_REQ                 =		sInLineActivityBase + 12,	//分时段奖励
// 		TIME_AWARD_RESP                =		sInLineActivityBase + 13,	//分时段奖励

		SEND_ACTIVITY_RESP                =		sInLineActivityBase + 15,	//登录时发活动数据包

		ONE_ACTIVITY_REQ					       =		sInLineActivityBase + 16,	//活动请求
		ONE_ACTIVITY_RESP					       =		sInLineActivityBase + 17,	//分时段奖励

	};



	enum 
	{


		RANKGAME_INFO_REQ              =	sRankGameBase + 0,	//排位赛信息
		RANKGAME_INFO_RESP             =	sRankGameBase + 1,	//排位赛信息返回
		RANKGAME_RANKING_REQ		   =    sRankGameBase + 2,	//排位赛排行榜
		RANKGAME_RANKING_RESP			=    sRankGameBase + 3,	//排位赛排行榜返回
		RANKGAME_REPORT_REQ			   =    sRankGameBase + 4,	//排位赛战报
		RANKGAME_REPORT_RESP		   =    sRankGameBase + 5,	//排位赛战报返回	
		RANKGAME_SHOP_REQ			   =    sRankGameBase + 6,	//排位赛商店信息
		RANKGAME_SHOP_RESP			   =    sRankGameBase + 7,	//排位赛商店信息返回
		RANKGAME_SHOPBUY_REQ			=    sRankGameBase + 8,	//排位赛商店购买
		RANKGAME_SHOPBUY_RESP		   =    sRankGameBase + 9,	//排位赛商店购买返回
		RANKGAME_BEGIN_REQ				=    sRankGameBase + 10,	//排位赛开始
		RANKGAME_BEGIN_RESP			   =    sRankGameBase + 11,	//排位赛开始返回
		RANKGAME_END_REQ			   =    sRankGameBase + 12,	//排位赛结束
		RANKGAME_END_RESP			   =    sRankGameBase + 13,	//
		RANKGAME_BUYTIMES_REQ			   =    sRankGameBase + 14,	//排位赛购买次数
		RANKGAME_BUYTIMES_RESP			   =    sRankGameBase + 15,	//排位赛购买次数返回

		

	};


}// namespace Lynx

#endif // __LYNX_COMMON_LIB_BOX_PACKET_H__
