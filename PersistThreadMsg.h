#ifndef __LYNX_GAME_SERVER_PERSIST_THREAD_MSG_H__
#define __LYNX_GAME_SERVER_PERSIST_THREAD_MSG_H__

#include "CommonLib.h"

namespace Lynx
{
	enum
	{
		PERSIST_LOAD_PLAYERDATA_REQ                = 1,         // 加载玩家数据请求
        PERSIST_LOAD_PLAYERDATA_RESP               = 2,         // 加载玩家数据回应
        PERSIST_SYNC_BASE_GUID_NOTIFY              = 3,         // 同步itemguid同步通知
        PERSIST_LOAD_PLAYER_GUID_REQ               = 4,         // 加载玩家guid请求
        PERSIST_LOAD_PLAYER_GUID_RESP              = 5,         // 加载玩家guid回应
        PERSIST_CREATE_PLAYER_REQ                  = 6,         // 创建玩家请求
        PERSIST_CREATE_PLAYER_RESP                 = 7,         // 创建玩家回应
        PERSIST_ADD_ACCOUNT_REQ                    = 8,         // 添加用户账户请求
        PERSIST_ADD_ACCOUNT_RESP                   = 9,         // 添加用户账户回应
        
        PERSIST_DEL_ITEM_NOTIFY                    = 10,        // 删除道具通知
        PERSIST_UPDATE_ITEM_NOTIFY                 = 11,        // 更新道具通知
        PERSIST_UPDATE_EQUIP_NOTIFY                = 12,        // 更新装备通知
        
        PERSIST_UPDATE_HERO_DEBRIS_NOTIFY          = 14,        // 更新英雄碎片通知
        PERSIST_INSERT_ITEM_NOTIFY                 = 15,        // 添加道具通知
        PERSIST_INSERT_EQUIP_NOTIFY                = 16,        // 添加装备通知
      
        PERSIST_INSERT_HERO_DEBRIS_NOTIFY          = 18,        // 添加英雄碎片通知
        PERSIST_DEL_EQUIP_NOTIFY                   = 19,        // 删除装备通知
       
        PERSIST_DEL_HERO_DEBRIS_NOTIFY             = 21,        // 删除英雄碎片通知
        PERSIST_INSERT_RUNE_NOTIFY                 = 22,        // 添加符文通知 
        PERSIST_UPDATE_RUNE_NOTIFY                 = 23,        // 更新符文通知
        PERSIST_DEL_RUNE_NOTIFY                    = 24,        // 删除符文通知
        PERSIST_INSERT_PET_DEBRIS_NOTIFY           = 25,        // 添加宠物碎片通知
        PERSIST_UPDATE_PET_DEBRIS_NOTIFY           = 26,        // 更新宠物碎片通知
        PERSIST_DEL_PET_DEBRIS_NOTIFY              = 27,        // 删除宠物碎片通知
        PERSIST_INSERT_EQUIP_DEBRIS_NOTIFY         = 28,        // 添加装备碎片通知
        PERSIST_UPDATE_EQUIP_DEBRIS_NOTIFY         = 29,        // 更新装备碎片通知
        PERSIST_DEL_EQUIP_DEBRIS_NOTIFY            = 30,        // 删除装备碎片通知

        PERSIST_INSERT_HERO_NOTIFY                 = 41,        // 添加英雄通知
        PERSIST_DEL_HERO_NOTIFY                    = 42,        // 删除英雄通知
        PERSIST_UPDATE_HERO_NOTIFY                 = 43,        // 更新英雄通知
        PERSIST_INSERT_PET_NOTIFY                  = 44,        // 添加宠物通知
        PERSIST_DEL_PET_NOTIFY                     = 45,        // 删除宠物通知
        PERSIST_UPDATE_PET_NOTIFY                  = 46,        // 更新宠物通知

		PERSIST_UPDATE_PLAYER_GUID				= 48,	//更新玩家新手引导
		PERSIST_UPDATE_PLAYER_GUIDGIFT				= 49,	//更新玩家新手引导所需材料初始化

        PERSIST_UPDATE_PLAYER_DATA_NOTIFY          = 61,        // 同步玩家基础数据通知
        PERSIST_UPDATE_PLAYER_ACCOUNT_DATA_NOTIFY  = 62,        // 同步玩家账户数据通知
        PERSIST_RESET_PLAYER_TOLLGATE_DATA_NOTIFY  = 63,        // 重置玩家任务数据通知
        PERSIST_UPDATE_PLAYER_TOLLGATE_DATA_NOTIFY = 64,        // 同步玩家任务数据通知
        PERSIST_INSERT_PLAYER_TOLLGATE_DATA_NOTIFY = 65,        // 添加任务数据
        PERSIST_UPDATE_PLAYER_TOLLGATE_AWARD_NOTIFY     = 66,   // 更新玩家副本领奖信息    
        PERSIST_UPDATE_PLAYER_PRODUCE_DATA_NOTIFY  = 71,        // 同步玩家产出数据通知
        PERSIST_UPDATE_PLAYER_SYSTEM_AWARD_NOTIFY  = 72,        // 更新玩家系统奖励数据通知
        PERSIST_UPDATE_PLAYER_ACTIVITY_DATA_NOTIFY = 73,        // 更新玩家活动数据
        PERSIST_UPDATE_PLAYER_ACHIEVEMENT_DATA_NOTIFY   = 74,   // 更新玩家成就数据
        PERSIST_UPDATE_PLAYER_TOWER_DATA_NOTIFY    = 75,        // 更新玩家爬塔数据

        PERSIST_INSERT_EMAIL_NOTIFY                = 81,        // 创建邮件请求
        PERSIST_DELETE_EMAIL_NOTIFY                = 82,        // 删除邮件通知
        PERSIST_UPDATE_EMAIL_NOTIFY                = 83,        // 更新邮件
		PERSIST_CLEAR_EMAIL_NOTIFY					 = 84,       // 清除所有邮件

        PERSIST_LOAD_OFFLINE_PLAYERBASEDATA_REQ    = 101,       // 加载离线玩家数据请求
        PERSIST_LOAD_OFFLINE_PLAYERBASEDATA_RESP   = 102,       // 加载离线玩家数据回应
        PERSIST_LOAD_OFFLINE_PLAYERDATA_REQ        = 103,       // 加载离线玩家数据请求
        PERSIST_LOAD_OFFLINE_PLAYERDATA_RESP       = 104,       // 加载离线玩家数据回应

        PERSIST_INSERT_CONSUME_RECORD_NOTIFY       = 121,       // 添加英雄消耗记录通知
        PERSIST_UPDATE_CONSUME_RECORD_NOTIFY       = 122,       // 更新英雄消耗记录通知
        PERSIST_DEL_CONSUME_RECORD_NOTIFY          = 123,       // 删除英雄消耗记录通知

        PERSIST_INSERT_STORE_ITEM_NOTIFY           = 141,       // 添加商城购物记录
        PERSIST_UPDATE_STORE_ITEM_NOTIFY           = 142,       // 更新商城购物记录
        PERSIST_DEL_STORE_ITEM_NOTIFY              = 143,       // 删除商城购物记录
        PERSIST_UPDATE_STORE_DATE_FLAG             = 144,       // 更新商城购物日期
        PERSIST_UPDATE_SECRET_STORE_DATA_NOTIFY    = 145,       // 更新特殊商店道具信息

        PERSIST_INSERT_EXCHANGE_ITEM_NOTIFY        = 161,       // 添加兑换记录
        PERSIST_UPDATE_EXCHANGE_ITEM_NOTIFY        = 162,       // 更新兑换记录
        PERSIST_DEL_EXCHANGE_ITEM_NOTIFY           = 163,       // 删除兑换记录
        PERSIST_UPDATE_EXCHANGE_DATE_FLAG          = 164,       // 更新兑换日期

        PERSIST_LOAD_ARENA_PLAYER_DATA_REQ         = 181,       // 加载竞技场玩家数据请求
        PERSIST_LOAD_ARENA_PLAYER_DATA_RESP        = 182,       // 加载竞技场玩家数据回应
        PERSIST_UPDATE_PLAYER_ARENA_BEST_RANK_NOTIFY    = 183,  // 更新玩家竞技场最好排名通知
        PERSIST_UPDATE_ARENA_HERO_NOTIFY           = 184,       // 更新玩家竞技场英雄通知
        PERSIST_UPDATE_ARENA_PET_NOTIFY            = 185,       // 更新玩家竞技场宠物通知
        PERSIST_RESET_ARENA_BATTLE_CNT_NOTIFY      = 186,       // 更新竞技场玩家战斗次数

        PERSIST_LOAD_RANK_DATA_REQ                 = 201,       // 加载排行榜数据请求
        PERSIST_LOAD_RANK_DATA_RESP                = 202,       // 加载排行榜数据回应
        PERSIST_UPDATE_PLAYER_RANK_BASE_NOTIFY     = 203,       // 更新玩家排行榜基础信息通知

        PERSIST_UPDATE_PLAYER_INFO_NOTIFY          = 211,       // 更新玩家信息通知
	
		PERSIST_LOAD_PLAYER_REQ_TEST			   = 212,		//sec 新添加测试玩家登陆消息通知

		//manager发给persistworker请求设置dirty位
		PERSIST_MANAGER_SETDIRTY_REQ			   = 213,		//

		PERSIST_MANAGER_SETDIRTY_RESP			   = 214,		//

		PERSIST_NOTIFY_WORKER_SAVEDB_REQ		   = 215,		//

		PERSIST_NOTIFY_WORKER_SAVEDB_RESP          = 216,		//

		PERSIST_OFF_LINE_SAVE_REQ				   = 217,		//

		PERSIST_FIRE_CONFIREM_SAVE_REQ			   = 300,		//战斗验证存档

		PERSIST_BASE_DATA_SAVE_REQ				   = 301,		//玩家基本信息存档

		PERSIST_RANKING_SAVE_REQ				   = 302,		//玩家基本信息存档

		PERSIST_OFFLINE_REQ						   = 303,		//玩家基本信息存档

		PERSIST_FIND_SIMILAR_POWER				   = 304,		//查询战力相似玩家

		PERSIST_FIND_SIMILAR_POWER_RESP            = 305,         // 查询战力相似玩家返回

		PERSIST_SHOP_SAVE_REQ					   = 306,		//商城信息存档

		PERSIST_SHOP_ITEM_SAVE_REQ				   = 307,		//商城商品信息存档

		PERSIST_CAT_COIN_SAVE_REQ				   = 308,		//猫币存档

		PERSIST_COIN_GROUP_SAVE_REQ				   = 309,		//猫币组合存档




		PERSIST_JEWELRY_ADD                        = 400,       //通知数据库增加饰品
		PERSIST_JEWELRY_DEL                        = 401,       //通知数据库删除饰品
		PERSIST_JEWELRY_UPDATE                     = 402,        //通知数据库更新饰品

		PERSIST_JEWELRYEQUIP_UPDATE                = 403,

		PERSIST_INLINE_ACTIVITY_POWER		   	   = 404,		//内置活动
		PERSIST_SERVERDATA_REQ				   	   = 405,		//公共数据
		PERSIST_ROBOTDATA_REQ				   	   = 406,		//机器人数据
		PERSIST_ROBOTDATA_RESP				   	   = 407,		//机器人数据

		PERSIST_SERVANT_INSERT                     = 500,       //通知数据库增加佣兵
		PERSIST_SERVANT_UPDATE                     = 501,       //通知数据库更新佣兵表
		PERSIST_SERVATN_DEL                        = 502,        //通知数据库删除佣兵
		

		PERSIST_SERVANTTREASURE_INSERT             = 600,		//数据库添加宝物
		PERSIST_SERVANTTREASURE_UPDATE             = 601,       //数据库更新宝物

		PERSIST_LOADPLAYER_BY_ACCOUNT             = 700 ,//通过账号加载玩家
		PERSIST_LOADPLAYER_BY_ACCOUNT_RESP    = 701, //通过账号加载回复

		PERSIST_FRIENDOFFLINE_UPDATE     = 800,      //离线好友更新数据库
		PERSIST_FRIENDDATA_UPDATE      = 801 , 		//好友数据更新
		PERSIST_LEAVETIME = 803	,
		PERSIST_FRIENDOFFLINE_ADD = 804,   //离线好友添加
		RERSIST_FRIENDOFFLINE_DEL  = 805,   //离线好友删除
		PERSIST_RESETALLPLAYERLEAVETIME = 806, //服务器刷新玩家所有离线时间
		PERSIST_FORBIDLOGINTIME_UPDATE = 807, //服务器禁止登陆时间更新
		PERSIST_FORBIDCHAT_UPDATE = 808,// 禁止聊天更新

		PERSIST_ONLINELEVELRANK_UPDATE = 900, //从数据库load数据更新在线玩家等级排行
		PERSIST_POWERRANK_UPDATE	 = 901, //从数据库load数据，所有玩家战力排行
		PERSIST_PLAYERCONSORT_UPDATE = 899, //从数据库load其他玩家的军团信息
		PERSIST_PLAYERCONSORT_SAVE = 898, //将玩家军团信息更新到数据库中
		PERSIST_CONSORT_SAVE = 897, // 军团信息更新到数据库
		PERSIST_CONSORT_CREATE = 896, //创建军团到数据库
		PERSIST_CONSORTAPPLY_INSERT = 895, //将玩家申请信息写入数据库
		PERSIST_CONSORTAPPLY_DEL = 894, //将玩家申请信息删除
		PERSIST_CONSORTAPPLY_CLEAR = 893, //清除玩家所有申请信息
		PERSIST_CONSORTAPPLY_ALLCLEAR = 892,//清除军团所有申请信息
		PERSIST_CONSORT_DEL = 891, //删除军团数据库信息
		PERSIST_CONSORTLOG_INSERT = 890, //军团日志写入数据库
		PERSIST_CONSORTSIGN_RESET = 889, //公会签到信息重置
		PERSIST_CONSORTSIGN_UPDATE = 888, //玩家签到信息更新
		PERSIST_CONSORTACTIVE_UPDATE = 887,//公会活跃度存盘
		PERSIST_CONSORTLOYAL_UPDATE = 886, //公会行侠仗义更新存盘
		PERSIST_CONSORTKITQUE_UPDATE = 885,//公会厨房次数更新
		PERSIST_BUSINESSCAT_UPDATE = 884, //公会行商人次数更新
		PERSIST_BUSINESSCATTIME_RESET = 883,  //每天重置公会商店
		PERSIST_EYEQUETIMES_UPDATE = 882,//公会眼力训练次数更新
		PERSIST_TICKETTIME_RESET = 881,//公会票友集会重置时间
		PERSIST_CONSORTTICKET_UPDATE = 880,//公会票友集会更新
		PERSIST_TICKETQUALITY_UPDATE = 879,//公会票友集会品质更新
		PERSIST_TICKETFRIEND_INIT = 878,//公会票友集会初始化
		PERSIST_TICKETALL_DEL = 877,//公会票友集会清空
		PERSIST_TICKETDATA_UPDATE = 876,//公会票友数据更新
		PERSIST_TICKETSUPPORT_UPDATE = 875,//公会赞助数据更新
		PERSIST_TICKETAWARD_INSERT = 874,//个人奖励插入
		PERSIST_TICKETAWARD_UPDATE = 873,//个人奖励更新
		PERSIST_TICKETTIMES_UPDATE = 872,//票友集会次数更新
		PERSIST_TICKETAWARD_DEL = 871,//个人奖励删除
		PERSIST_TICKETTIMES_RESET = 870,//重置票友集会次数
		PERSIST_LOYALTIMES_RESET = 869,//重置行侠仗义次数
		PERSIST_KITCHENTIMES_RESET = 868,//重置厨房特训次数
		PERSIST_EYETIMES_RESET = 867,//重置眼力训练次数
		PERSIST_ELOQUENCETIMES_UPDATE = 866,//口才训练次数更新
		PERSIST_ELOQUENCETIMES_RESET = 865,//口才训练重置次数
		PERSIST_WOODTIMES_RESET = 864,//木猫阵法重置次数
		PERSIST_WOODTOTAL_UPDATE = 863,//木猫总血量和赞助强化更新
		PERSIST_WOODTOTAL_RESET = 862,//木猫阵法重置共享数据
		PERSIST_WOODSELF_UPDATE = 861,//木猫阵法更新个人数据
	
		PERSIST_ACHIEVE_UPDATE = 902,  //更新成就到数据库
		PERSIST_OFFLINEFRIEND_UPDATE = 903,//更新离线玩家的好友数量
		PERSIST_DAILYTASK_UPDATE = 904,//日常任务添加请求存盘
		PERSIST_DAILYACTIVE_UPDATE = 905, //日常活跃度表存盘
		PERSIST_DAILYLASTTIME_UPDATE = 906, //日常活跃度上次清理时间更新消息

		PERSIST_FASHION_INSERT = 1000,

		PERSIST_CHARACTOR_UPDATE = 1001, //角色更新数据
		PERSIST_CHARACTOR_INSERT = 1002, //角色插入数据库
		PERSIST_SCORERANK_LOAD = 1003, //角色更新数据
// 		PERSIST_RANKGAME_REPORT_LOAD = 1004, //所有玩家排位赛积分排行
		PERSIST_RANKGAME_SCORE_SAVE = 1005, //所有玩家排位赛积分排行
		PERSIST_RANKGAME_REPORT_SAVE = 1006, //所有玩家排位赛积分排行
		PERSIST_DAY_ONLINE_SAVE = 1007,//7天连续在线
		PERSIST_BASE_DATA_SAVE = 1008, //所有玩家排位赛积分排行
		PERSIST_BE_APPLY_DATA_SAVE = 1009, //好友申请列表

			
		PERSIST_SERVER_DAILY_DATA_REQ				   	   = 109,		//服务器日常数据

		PERSIST_GM_RESETTONGBAO_REQ					= 2001, //通宝当铺重置存盘
		PERSIST_GM_RESETMENGCHU_REQ					= 2002, //猛畜生重置存盘
		PERSIST_GM_RESETYUSHIGF_REQ						= 2003,//玉石工坊重置
		PERSIST_GM_RESETSHIJIAB_REQ						= 2004,//市场集安保重置
		PERSIST_GM_RESETBASHANSL_REQ                 = 2005,//巴山试炼重置
		PERSIST_GM_RESETWUXIANTZ_REQ					= 2006,//无限挑战
		PERSIST_GM_RESETCOURAGE_REQ					= 2007,//勇气试炼
		PERSIST_GM_RESETPAIWEI_REQ						= 2008,//排位赛
		PERSIST_GM_RESETPAIWEIBUY_REQ					= 2009,//排位购买次数
		PERSIST_GM_RESET12HAOJIAO_REQ					= 2010,//十二宗宫号角
		PERSIST_GM_RESETFOODCOOK_REQ				= 2011,//美食屋烹饪次数
		PERSIST_GM_RESETSERVANTONCE_REQ			= 2012,//赏罚令免费召唤一次
		PERSIST_GM_RESETSERVANTTEN_REQ				= 2013,//赏罚令免费召唤十次
		PERSIST_GM_RESETDAILYTASK_REQ					= 2014,//重置所有每日任务
		PERSIST_GM_RESETMONTHSIGN_REQ				= 2015,//重置月签到
		PERSIST_GM_RESETSEVENDAY_REQ					= 2016,//七日登录重置
		PERSIST_GM_RESETSEVENT_REQ						= 2017,//七日讯
		PERSIST_GM_RESETGROWFOUND_REQ			= 2018,//成长基金删除
		PERSIST_GM_RESETFENSHI_REQ						= 2019,//分时奖励
		PERSIST_GM_RESETONLINEAWARD_REQ			= 2020,//在线奖励
		PERSIST_GM_RESETBAICAISH_REQ					= 2021,//重置拜财神
		PERSIST_GM_RESETSTRENGHT_REQ					= 2022,//购买体力
		PERSIST_GM_RESETLIANPU_REQ						= 2023,//脸谱上电
		PERSIST_GM_RESETTONGBAOS_REQ                  = 2024,//通宝商店


	
	};

	struct Pos
	{
		Pos():x(0),y(0){}
		UInt32 x;//x坐标
		UInt32 y;//y坐标
	};
    
	struct PersistLoadPlayerGuidReq
	{
		Guid mAccountId;
		ConnId mConnId;
		String mAccountName;

		PersistLoadPlayerGuidReq(): mAccountId(0), mConnId(0), mAccountName("") {}

		LYNX_MESSAGE_3(PERSIST_LOAD_PLAYER_GUID_REQ, PersistLoadPlayerGuidReq, Guid, mAccountId,
			ConnId, mConnId, String, mAccountName);
	};

    struct PersistLoadPlayerDataResp 
    {
        PersistLoadPlayerDataResp() : mConnId(0), mPlayerGuid(0), merrorId(0), mAccountName("") {}

        ConnId mConnId;
        errorId merrorId;
        Guid mPlayerGuid;
        String mAccountName;
        PlayerData mPlayerData;

        LYNX_MESSAGE_4(PERSIST_LOAD_PLAYERDATA_RESP, PersistLoadPlayerDataResp,
            ConnId, mConnId, errorId, merrorId, Guid, mPlayerGuid, 
            PlayerData, mPlayerData);
    };

	struct PersistFindSimilarPowerResp 
	{
		PersistFindSimilarPowerResp() : playerID(0),times(0),initialValue(0){}

		Guid playerID;
		UInt32 times;//1 十二宗宫，10001 多人副本标识
		UInt32 initialValue;
		List<GuidValue> keyValueList;

		LYNX_MESSAGE_4(PERSIST_FIND_SIMILAR_POWER_RESP, PersistFindSimilarPowerResp,
			Guid, playerID,UInt32, times,UInt32, initialValue,List<GuidValue>, keyValueList);
	};

	
	struct PersistLoadOfflineDataResp 
	{
// 		PersistLoadOffLineDataResp() :{}

		PlayerBaseData mBaseData;


		LYNX_MESSAGE_1(PERSIST_OFFLINE_REQ, PersistLoadOfflineDataResp,PlayerBaseData, mBaseData);
	};

    struct PersistSyncBaseGuidNotify
    {
        PersistSyncBaseGuidNotify() : mPlayerGuid(0),mItemGuid(0),mLansquenetGuid(0),mGuildGuid(0),mEmailGuid(0),mMaterialGuid(0),
		mCharactorGuid(0), mConsortGuid(0), mTicketGuid(0){}

        UInt64 mPlayerGuid;

		UInt64 mItemGuid;

		UInt64 mLansquenetGuid;

		UInt64 mGuildGuid;

		UInt64 mEmailGuid;

		UInt64 mMaterialGuid;

		UInt64 mCharactorGuid;

		UInt64 mConsortGuid;

		UInt64 mTicketGuid;

        LYNX_MESSAGE_9(PERSIST_SYNC_BASE_GUID_NOTIFY, PersistSyncBaseGuidNotify,
            UInt64, mPlayerGuid,UInt64, mItemGuid,UInt64, mLansquenetGuid,UInt64, mGuildGuid,UInt64, mEmailGuid, UInt64, mMaterialGuid,
			UInt64, mCharactorGuid, UInt64, mConsortGuid,UInt64, mTicketGuid);
    };

    struct PersistLoadPlayerGuidResp
    {
        Guid mPlayerGuid;
        Guid mAccountId;
        ConnId mConnId;
        errorId merrorId;
        String mAccountName;

        PersistLoadPlayerGuidResp(): merrorId(0), mPlayerGuid(0), mConnId(0),
            mAccountName(""), mAccountId(0) {}

        LYNX_MESSAGE_5(PERSIST_LOAD_PLAYER_GUID_RESP, PersistLoadPlayerGuidResp, Guid, mPlayerGuid,
            errorId, merrorId, ConnId, mConnId, String, mAccountName, Guid, mAccountId);
    };

   	
	struct PersistLoadPlayerReqTest
	{
		std::string mPlayerName;
		ConnId mConnId;
		LYNX_MESSAGE_2(PERSIST_LOAD_PLAYER_REQ_TEST, PersistLoadPlayerReqTest, std::string, mPlayerName,ConnId,mConnId);
		
	};

//=======================================================================================================================================================================>
	//新游戏BOC加载请求

	struct PersistLoadPlayerDataReq 
	{
		PersistLoadPlayerDataReq() : mConnId(0), mPlayerGuid(0){}

		ConnId mConnId;
		Guid mPlayerGuid;
		

		LYNX_MESSAGE_2(PERSIST_LOAD_PLAYERDATA_REQ, PersistLoadPlayerDataReq,
			ConnId, mConnId, Guid, mPlayerGuid);
	};

	struct PersistCreatePlayerReq
	{
		PersistCreatePlayerReq():mConnId(0),mPlayerModelId(0),mName(""),mAccountId(0){}

		ConnId mConnId;
		UInt32 mPlayerModelId;
		String mName;
		UInt64 mAccountId;

		LYNX_MESSAGE_4(PERSIST_CREATE_PLAYER_REQ, PersistCreatePlayerReq,
			ConnId, mConnId, UInt32, mPlayerModelId,	String, mName, UInt64, mAccountId);
	};

	struct PersistCreatePlayerResp
	{
		PersistCreatePlayerResp():mRes(0),mPlayerModelId(0),mPlayerUid(0){}
		UInt16 mRes;
		UInt64 mPlayerUid;
		ConnId mConnId;
		String mName;
		UInt32 mPlayerModelId;
		LYNX_MESSAGE_5(PERSIST_CREATE_PLAYER_RESP, PersistCreatePlayerResp,
			UInt16, mRes, UInt64, mPlayerUid, ConnId, mConnId, UInt32, mPlayerModelId,String,mName);
		
	};

	struct PersistInsertRobotResp
	{
		PersistInsertRobotResp():flag(0){}
		UInt32 flag;
		LYNX_MESSAGE_1(PERSIST_ROBOTDATA_RESP, PersistInsertRobotResp,UInt32, flag);

	};

	struct PersistLoadPlayerByAccount
	{
		PersistLoadPlayerByAccount():mConnId(0),mAccountId(0){}
	
		UInt64 mAccountId;
		ConnId mConnId;
		LYNX_MESSAGE_2(PERSIST_LOADPLAYER_BY_ACCOUNT, PersistLoadPlayerByAccount,
			UInt64 ,mAccountId, ConnId, mConnId);
		
	};

	struct PersistLoadPlayerByAccountResp
	{
		PersistLoadPlayerByAccountResp():mConnId(0),mAccountId(0),mErrorId(0),mPlayerUid(0){}
		errorId mErrorId;
		UInt64 mAccountId;
		ConnId mConnId;
		UInt64 mPlayerUid;
		LYNX_MESSAGE_4(PERSIST_LOADPLAYER_BY_ACCOUNT_RESP, PersistLoadPlayerByAccountResp,
			UInt64 ,mAccountId, ConnId, mConnId, errorId ,mErrorId,UInt64,mPlayerUid);

	};

	//persistmanager 通知persistsystem dirtybit
	struct PersistManagerSetDirtyReq
	{
		PersistManagerSetDirtyReq():mPlayerUid(0),mDirtyBit(0){}
		UInt64 mPlayerUid;
		UInt32 mDirtyBit;

		LYNX_MESSAGE_2(PERSIST_MANAGER_SETDIRTY_REQ, PersistManagerSetDirtyReq,
			UInt64, mPlayerUid, UInt32, mDirtyBit);

	};

	struct PersistManagerSetDirtyResp
	{
		PersistManagerSetDirtyResp():mPlayerUid(0),mDirtyBit(0){}
		UInt64 mPlayerUid;
		UInt32 mDirtyBit;

		LYNX_MESSAGE_2(PERSIST_MANAGER_SETDIRTY_RESP, PersistManagerSetDirtyResp,
			UInt64, mPlayerUid, UInt32, mDirtyBit);
	};

	struct PersistNotifyWorkerSaveDbReq
	{
		PersistNotifyWorkerSaveDbReq():mPlayerUid(0),mDirtyBit(0){}
		UInt64 mPlayerUid;
		UInt32 mDirtyBit;

		LYNX_MESSAGE_2(PERSIST_NOTIFY_WORKER_SAVEDB_REQ, PersistNotifyWorkerSaveDbReq,
			UInt64, mPlayerUid, UInt32, mDirtyBit);
	};

	struct PersistNotifyWorkerSaveDbResp
	{
		PersistNotifyWorkerSaveDbResp():mPlayerUid(0),mDirtyBit(0),merrorId(0){}
		UInt64 mPlayerUid;
		UInt32 mDirtyBit;
		errorId merrorId;

		LYNX_MESSAGE_3(PERSIST_NOTIFY_WORKER_SAVEDB_RESP, PersistNotifyWorkerSaveDbResp,
			UInt64, mPlayerUid, UInt32, mDirtyBit,errorId, merrorId);
	};

	struct PersistDelItemNotify
	{
		Guid mItemGuid;
		Guid mPlayerGuid;
		// 道具栏位(普通,装备,装备碎片,英雄碎片)
		UInt8 mField;

		PersistDelItemNotify(): mItemGuid(0), mPlayerGuid(0), mField(0) {}

		LYNX_MESSAGE_3(PERSIST_DEL_ITEM_NOTIFY, PersistDelItemNotify, Guid, mItemGuid,
			Guid, mPlayerGuid, UInt8, mField);
	};


	struct PersistUpdateItemNotify
	{
		ItemData mItemData;
		Guid mPlayerGuid;

		PersistUpdateItemNotify(): mPlayerGuid(0) {}

		LYNX_MESSAGE_2(PERSIST_UPDATE_ITEM_NOTIFY, PersistUpdateItemNotify, ItemData, mItemData, Guid, mPlayerGuid);
	};

	struct PersistInsertItemNotify
	{
		ItemData mItemData;
		Guid mPlayerGuid;

		PersistInsertItemNotify(): mPlayerGuid(0) {} 

		LYNX_MESSAGE_2(PERSIST_INSERT_ITEM_NOTIFY, PersistInsertItemNotify, ItemData, mItemData, Guid, mPlayerGuid);
	};

	
	struct PersistOffLineSaveReq
	{
		
		UInt64 mPlayerUid;
		
		PersistOffLineSaveReq():mPlayerUid(0){}

		LYNX_MESSAGE_1(PERSIST_OFF_LINE_SAVE_REQ, PersistOffLineSaveReq,  UInt64, mPlayerUid);
	};


	struct PersistFireConfirmSaveReq
	{
		Guid playerGuid;

		PlayerFireConfirmData FireConfirmData;

		PersistFireConfirmSaveReq():playerGuid(0){}

		LYNX_MESSAGE_2(PERSIST_FIRE_CONFIREM_SAVE_REQ, PersistFireConfirmSaveReq,  Guid, playerGuid,PlayerFireConfirmData, FireConfirmData);
	};


	struct PersistPlayerSinglePropertySaveReq
	{

		Guid playerGuid;
		String strFlag;
		String strData;

 		PersistPlayerSinglePropertySaveReq():playerGuid(0),strFlag(""),strData(""){}

		LYNX_MESSAGE_3(PERSIST_BASE_DATA_SAVE_REQ, PersistPlayerSinglePropertySaveReq,  Guid, playerGuid,String, strFlag, String, strData);
	};

	struct PersistFindSimilarPowerReq
	{

		Guid playerGuid;
		UInt32 times;
		UInt32 high;
		UInt32 low;
		UInt32 initialValue;

		PersistFindSimilarPowerReq():playerGuid(0),times(0),high(0),low(0),initialValue(0){}

		LYNX_MESSAGE_5(PERSIST_FIND_SIMILAR_POWER, PersistFindSimilarPowerReq,  Guid, playerGuid,UInt32, times,UInt32, high,UInt32, low,UInt32, initialValue);
	};

	
	struct PersistServerDataReq
	{
		UInt32 gServerID ;//充值人数
		UInt32 gRechargeNum ;//充值人数
		UInt32 gServerRobotRefreshFlag ;//0重新导入机器人		
		UInt32 gRankGameRewardresetTime ;//
		UInt32 gServerFirstOpenTime ;//充值人数
		UInt32 gServerSpare5 ;//充值人数

		PersistServerDataReq():gRechargeNum(0),gServerRobotRefreshFlag(0),gServerID(0),gRankGameRewardresetTime(0),gServerFirstOpenTime(0),gServerSpare5(0){}

		LYNX_MESSAGE_6(PERSIST_SERVERDATA_REQ, PersistServerDataReq,UInt32,gRechargeNum,UInt32, gServerRobotRefreshFlag,UInt32, gServerID,UInt32, gRankGameRewardresetTime,UInt32, gServerFirstOpenTime,UInt32, gServerSpare5);
	};

	struct PersistServerDailyDataReq
	{
		UInt32 gServerOnlineNum ;//当前在线人数
		UInt32 gServerOnlineMaxNum ;//当天最高在线人数
		UInt32 gServerOnlineNumSetTime ;//当天最高人数设置时间
		UInt32 gSSpare1 ;//
		UInt32 gSSpare2 ;//
		UInt32 gSSpare3 ;//

		PersistServerDailyDataReq():gServerOnlineMaxNum(0),gServerOnlineNumSetTime(0),gServerOnlineNum(0),gSSpare1(0),gSSpare2(0),gSSpare3(0){}

		LYNX_MESSAGE_6(PERSIST_SERVER_DAILY_DATA_REQ, PersistServerDailyDataReq,UInt32,gServerOnlineMaxNum,UInt32, gServerOnlineNumSetTime,UInt32, gServerOnlineNum,UInt32, gSSpare1,UInt32, gSSpare2,UInt32, gSSpare3);
	};

   struct PersistGMResetTongbao
   {
		UInt64 playeruid;
		UInt16 reset; 
		PersistGMResetTongbao():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETTONGBAO_REQ, PersistGMResetTongbao, UInt64, playeruid, UInt16, reset);
	
   };

   struct PersistGMResetMengchu
   {
		UInt64 playeruid;
		UInt16 reset; 
		PersistGMResetMengchu():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETMENGCHU_REQ, PersistGMResetMengchu, UInt64, playeruid, UInt16, reset);
	
   };
	
   struct PersistGMResetYushiGF
   {
		UInt64 playeruid;
		UInt16 reset; 
		PersistGMResetYushiGF():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETYUSHIGF_REQ, PersistGMResetYushiGF, UInt64, playeruid, UInt16, reset);
   };
  				

    struct PersistGMResetJiShiAB
   {
		UInt64 playeruid;
		UInt16 reset; 
		PersistGMResetJiShiAB():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETSHIJIAB_REQ, PersistGMResetJiShiAB, UInt64, playeruid, UInt16, reset);
   };

	struct PersistGMResetBashanSL
   {
		UInt64 playeruid;
		UInt16 reset; 
		PersistGMResetBashanSL():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETBASHANSL_REQ, PersistGMResetBashanSL, UInt64, playeruid, UInt16, reset);
   };

	struct PersistGMResetWXTZ
   {
		UInt64 playeruid;
		UInt16 reset; 
		PersistGMResetWXTZ():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETWUXIANTZ_REQ, PersistGMResetWXTZ, UInt64, playeruid, UInt16, reset);
   };



	struct PersistGMCourage
	{
		UInt64 playeruid;
		UInt16 reset; 
		PersistGMCourage():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETCOURAGE_REQ, PersistGMCourage, UInt64, playeruid, UInt16, reset);
	
	};

	struct PersistGMPaiWei
	{
		UInt64 playeruid;
		UInt16 reset; 
		PersistGMPaiWei():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETPAIWEI_REQ, PersistGMPaiWei, UInt64, playeruid, UInt16, reset);
	
	};

	struct PersistGMPaiWeiBuy
	{
		UInt64 playeruid;
		UInt16 reset; 
		PersistGMPaiWeiBuy():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETPAIWEIBUY_REQ, PersistGMPaiWeiBuy, UInt64, playeruid, UInt16, reset);
	
	};

	
	struct PersistGM12HaoJiao
	{
		UInt64 playeruid;
		UInt16 reset; 
		PersistGM12HaoJiao():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESET12HAOJIAO_REQ, PersistGM12HaoJiao, UInt64, playeruid, UInt16, reset);
	
	};

	struct PersistGMCookFood
	{
		UInt64 playeruid;
		UInt16 reset; 
		PersistGMCookFood():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETFOODCOOK_REQ, PersistGMCookFood, UInt64, playeruid, UInt16, reset);
	
	};

	struct PersistGMServantOnce
	{
		UInt64 playeruid;
		UInt16 reset; 
		PersistGMServantOnce():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETSERVANTONCE_REQ, PersistGMServantOnce, UInt64, playeruid, UInt16, reset);
	
	};


	struct PersistGMServantTen
	{
		UInt64 playeruid;
		UInt16 reset; 
		PersistGMServantTen():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETSERVANTTEN_REQ, PersistGMServantTen, UInt64, playeruid, UInt16, reset);
	
	};

	struct PersistGMResetDailyTask
	{
		UInt64 playeruid;
		UInt16 reset; 
		PersistGMResetDailyTask():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETDAILYTASK_REQ, PersistGMResetDailyTask, UInt64, playeruid, UInt16, reset);
	
	};

	struct PersistGMResetMonthSign
	{
		UInt64 playeruid;
		UInt64 reset; 
		PersistGMResetMonthSign():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETMONTHSIGN_REQ, PersistGMResetMonthSign, UInt64, playeruid, UInt64, reset);
	
	};

	struct PersistGMResetSevenday
	{
		UInt64 playeruid;
		UInt64 reset; 
		PersistGMResetSevenday():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETSEVENDAY_REQ, PersistGMResetSevenday, UInt64, playeruid, UInt64, reset);
	
	};

	struct PersistGMResetSeventrain
	{
		UInt64 playeruid;
		UInt64 reset; 
		PersistGMResetSeventrain():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETSEVENT_REQ, PersistGMResetSeventrain, UInt64, playeruid, UInt64, reset);
	
	};

	struct PersistGMResetGrowfound
	{
		UInt64 playeruid;
		UInt64 reset; 
		PersistGMResetGrowfound():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETGROWFOUND_REQ, PersistGMResetGrowfound, UInt64, playeruid, UInt64, reset);
	
	};

	struct PersistGMResetFenshi
	{
		UInt64 playeruid;
		UInt64 reset; 
		PersistGMResetFenshi():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETFENSHI_REQ, PersistGMResetFenshi, UInt64, playeruid, UInt64, reset);
	
	};

	struct PersistGMResetOnlineAward
	{
		UInt64 playeruid;
		UInt64 reset; 
		PersistGMResetOnlineAward():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETONLINEAWARD_REQ, PersistGMResetOnlineAward, UInt64, playeruid, UInt64, reset);
	
	};

	struct PersistGMResetBaiCaiShen
	{
		UInt64 playeruid;
		UInt64 reset; 
		PersistGMResetBaiCaiShen():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETBAICAISH_REQ, PersistGMResetBaiCaiShen, UInt64, playeruid, UInt64, reset);
	
	};

	struct PersistGMResetBuyStrength
	{
		UInt64 playeruid;
		UInt64 reset; 
		PersistGMResetBuyStrength():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETSTRENGHT_REQ, PersistGMResetBuyStrength, UInt64, playeruid, UInt64, reset);
	
	};

	struct PersistGMResetLianPu
	{
		UInt64 playeruid;
		UInt64 reset; 
		PersistGMResetLianPu():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETLIANPU_REQ, PersistGMResetLianPu, UInt64, playeruid, UInt64, reset);
	
	};
	

	struct PersistGMResetTB
	{
		UInt64 playeruid;
		UInt64 reset; 
		PersistGMResetTB():playeruid(0), reset(0){}
		LYNX_MESSAGE_2(PERSIST_GM_RESETTONGBAOS_REQ, PersistGMResetTB, UInt64, playeruid, UInt64, reset);
	
	};
	


	struct PersistRobotDataReq
	{
		UInt32 flag ;//

		PersistRobotDataReq():flag(0){}

		LYNX_MESSAGE_1(PERSIST_ROBOTDATA_REQ, PersistRobotDataReq,UInt32, flag);
	};


	struct PersistInlineActivityReq
	{
		PersistInlineActivityReq():playerGuid(0),type(0),m_GrowFoundBuyFlag(0),m_MonthSignCount(0),m_MonthSignTime(0),m_OnlineWelFareOnlineTime(0),m_PeopleWelfareRechargeNum(0),
			m_FirstLoginTime(0), m_SevenLoginGotCount(0),m_LastGetTime(0),m_LoginTime(0),m_LogoutTime(0),m_onlieFinishTime(0),m_TimeAwardRefreshTime(0){}		

		Guid playerGuid;
		UInt32 type;
		//成长基金
		UInt32 m_GrowFoundBuyFlag;//1已经购买，0未购买
		List<UInt32> m_GrowFoundGotList;//已领取的奖励

		//月签到
		UInt32 m_MonthSignCount;//签到第几个
		UInt32 m_MonthSignTime;//签到的时间

		//在线时长奖励
		UInt32 m_OnlineWelFareOnlineTime;//在线时长
		List<UInt32> m_OnlineWelFareGotList;//已领取的奖励

		//全民福利奖励
		UInt32 m_PeopleWelfareRechargeNum ;//充值人数
		List<UInt32> m_PeopleWelfareGotList;//已领取的奖励

		//7天登录
		UInt32 m_FirstLoginTime;//首次登陆时间
		UInt32 m_LastGetTime;//上一次领奖时间
		UInt32 m_SevenLoginGotCount;//已领取个数
		//7天活动
		List<KeyValue> m_SevenDayTaskGotList;//已领取的奖励
		List<KeyList> m_SevenDayTaskFinishList;//已领取的奖励

		//分段时间
		UInt32 m_TimeAwardRefreshTime;//更新时间
		List<UInt32> m_TimeAwardGotList;//已领取的奖励

		//登录
		UInt32 m_LoginTime;//登陆时间
		UInt32 m_LogoutTime;//离线时间
		UInt32 m_onlieFinishTime;//离线时间


		LYNX_MESSAGE_20(PERSIST_INLINE_ACTIVITY_POWER,PersistInlineActivityReq,UInt32, type,Guid, playerGuid,UInt32, m_GrowFoundBuyFlag,List<UInt32>, m_GrowFoundGotList, UInt32, m_MonthSignCount, UInt32, m_MonthSignTime,
			UInt32, m_OnlineWelFareOnlineTime,List<UInt32>, m_OnlineWelFareGotList,UInt32, m_PeopleWelfareRechargeNum,List<UInt32>, m_PeopleWelfareGotList,
			UInt32, m_FirstLoginTime,UInt32, m_LastGetTime, UInt32, m_SevenLoginGotCount, List<KeyValue>, m_SevenDayTaskGotList, UInt32, m_TimeAwardRefreshTime,
			List<UInt32>, m_TimeAwardGotList,UInt32, m_LoginTime,UInt32, m_LogoutTime,UInt32, m_onlieFinishTime,List<KeyList>, m_SevenDayTaskFinishList);
	};


	struct PersistAddJewelryNotify
	{
		
		PersistAddJewelryNotify():m_nPlayerUid(0){}

		JewelryData m_jewelryData;
		UInt64 m_nPlayerUid;
		
		
		LYNX_MESSAGE_2(PERSIST_JEWELRY_ADD,PersistAddJewelryNotify, UInt64, m_nPlayerUid,JewelryData ,m_jewelryData);
		
	};
	
	struct PersistRankingSaveReq
	{
		
		UInt32 type;
		UInt32 rank;
		Guid playerID;
		UInt32 val ;

		PersistRankingSaveReq():type(0),rank(0),playerID(0),val(0){}

		LYNX_MESSAGE_4(PERSIST_RANKING_SAVE_REQ, PersistRankingSaveReq, UInt32, type,UInt32, rank,Guid, playerID, UInt32, val);
	};

	struct PersistShopSaveReq
	{
		UInt32 shopType;
		UInt32 refreshTimes;
		Guid playerID;
		UInt32 refreshTime;
	
		PersistShopSaveReq():shopType(0),refreshTimes(0),playerID(0),refreshTime(0){}

		LYNX_MESSAGE_4(PERSIST_SHOP_SAVE_REQ, PersistShopSaveReq, UInt32, shopType,UInt32, refreshTimes,Guid, playerID,UInt32, refreshTime);
	};

	struct CatCoinSaveReq
	{
		UInt32 catCoinID;
		UInt32 num;
		Guid playerID;

		CatCoinSaveReq():catCoinID(0),num(0),playerID(0){}

		LYNX_MESSAGE_3(PERSIST_CAT_COIN_SAVE_REQ, CatCoinSaveReq, UInt32, catCoinID,UInt32, num,Guid, playerID);
	};

	struct CoinGroupSaveReq
	{
		UInt32 groupID;
		Guid playerID;

		CoinGroupSaveReq():groupID(0),playerID(0){}

		LYNX_MESSAGE_2(PERSIST_COIN_GROUP_SAVE_REQ, CoinGroupSaveReq, UInt32, groupID,Guid, playerID);
	};


	struct PersistShopItemSaveReq
	{

		UInt32 shopType;
		UInt32 position;
		Guid playerID;
		UInt32 buyTime;
		UInt32 buyTimes;
		UInt32 resType;
		UInt32 subType;
		UInt32 num;

		PersistShopItemSaveReq():shopType(0),position(0),playerID(0),buyTime(0),buyTimes(0),resType(0),subType(0),num(0){}

		LYNX_MESSAGE_8(PERSIST_SHOP_ITEM_SAVE_REQ, PersistShopItemSaveReq, UInt32, shopType,UInt32, position,Guid, playerID,UInt32, buyTime,UInt32, buyTimes,UInt32, resType,UInt32, subType,UInt32, num);
	};


	struct PersistOfflineDataReq
	{
		Guid playerID;
	
		PersistOfflineDataReq():playerID(0){}

		LYNX_MESSAGE_1(PERSIST_OFFLINE_REQ, PersistOfflineDataReq,Guid, playerID);
	};

	struct PersistDelJewelryNotify
	{
		PersistDelJewelryNotify(): m_nPlayerUid(0), m_nJewelryUid(0) {}

		UInt64 m_nPlayerUid;
		UInt64 m_nJewelryUid;
		
		LYNX_MESSAGE_2(PERSIST_JEWELRY_DEL, PersistDelJewelryNotify, UInt64, m_nPlayerUid,
			UInt64, m_nJewelryUid);
	};

	struct PersistUpdateJewelryNotify
	{

		PersistUpdateJewelryNotify():m_nPlayerUid(0){}

		JewelryData m_jewelryData;
		UInt64 m_nPlayerUid;


		LYNX_MESSAGE_2(PERSIST_JEWELRY_UPDATE,PersistUpdateJewelryNotify, UInt64, m_nPlayerUid,JewelryData ,m_jewelryData);

	};

	struct PersistUpdateJewelryEquipNotify
	{
		PersistUpdateJewelryEquipNotify():m_nPlayerUid(0){}
	    JewelryEquipData m_jewelryEquipData;
		UInt64 m_nPlayerUid;
		LYNX_MESSAGE_2(PERSIST_JEWELRYEQUIP_UPDATE,PersistUpdateJewelryEquipNotify, UInt64, m_nPlayerUid,JewelryEquipData ,m_jewelryEquipData);
	};

	  
	struct PersistAddServantNotify
	{
		PersistAddServantNotify():m_nPlayerUid(0){}
		ServantData m_servantData;
		UInt64 m_nPlayerUid;
		LYNX_MESSAGE_2(PERSIST_SERVANT_INSERT,PersistAddServantNotify, UInt64, m_nPlayerUid,ServantData ,m_servantData);
	};

	struct PersistUpdateServantNotify
	{
		PersistUpdateServantNotify():m_nPlayerUid(0){}
		ServantData m_servantData;
		UInt64 m_nPlayerUid;
		LYNX_MESSAGE_2(PERSIST_SERVANT_UPDATE,PersistUpdateServantNotify, UInt64, m_nPlayerUid,ServantData ,m_servantData);
	};

	struct PersistDelServantNotify
	{
		PersistDelServantNotify():m_nPlayerUid(0),m_nServantUid(0){}
		UInt64 m_nServantUid;
		UInt64 m_nPlayerUid;
		LYNX_MESSAGE_2(PERSIST_SERVATN_DEL,PersistDelServantNotify, UInt64, m_nServantUid,UInt64 ,m_nPlayerUid);
	};

	struct PersistAddServantTreasureNotify
	{
		PersistAddServantTreasureNotify():m_nPlayerUid(0){}
		ServantTreasure m_servantTreasure;
		UInt64 m_nPlayerUid;
		LYNX_MESSAGE_2(PERSIST_SERVANTTREASURE_INSERT,PersistAddServantTreasureNotify, UInt64, m_nPlayerUid,ServantTreasure ,m_servantTreasure);
	};
	
	struct PersistUpdateServantTreasureNotify
	{
		PersistUpdateServantTreasureNotify():m_nPlayerUid(0){}
		ServantTreasure m_servantTreasure;
		UInt64 m_nPlayerUid;
		LYNX_MESSAGE_2(PERSIST_SERVANTTREASURE_UPDATE,PersistUpdateServantTreasureNotify, UInt64, m_nPlayerUid,ServantTreasure ,m_servantTreasure);
	};

	struct PersistUpdateFriendOffLineNotify
	{
		PersistUpdateFriendOffLineNotify():m_nPlayerUid(0), m_nType(0),m_nMyselfUid(0){}
		UInt32 m_nType; //类型,0表示增加好友，1表示删除好友
		UInt64 m_nPlayerUid; //要删除好友的uid
		UInt64 m_nMyselfUid; //自己的uid

		LYNX_MESSAGE_3(PERSIST_FRIENDOFFLINE_UPDATE, PersistUpdateFriendOffLineNotify, UInt64, m_nPlayerUid,UInt32, m_nType,
			UInt64 ,m_nMyselfUid);
	};

	struct PersistUpdateFriendNotify
	{
		PersistUpdateFriendNotify():m_nPlayerUid(0){}
		FriendData m_friendData;
		UInt64 m_nPlayerUid;
		LYNX_MESSAGE_2(PERSIST_FRIENDDATA_UPDATE, PersistUpdateFriendNotify, UInt64, m_nPlayerUid, FriendData ,m_friendData);
	};

	struct PersistUpdateLeaveTime
	{
			PersistUpdateLeaveTime():m_nPlayerUid(0), m_nLeaveTime(0){}
			UInt64 m_nPlayerUid;
			UInt64 m_nLeaveTime;
			LYNX_MESSAGE_2(PERSIST_LEAVETIME, PersistUpdateLeaveTime, UInt64, m_nPlayerUid, UInt64 ,m_nLeaveTime);
		
	};

	struct PersistResetAllLeaveTime
	{
		PersistResetAllLeaveTime():m_nLeaveTime(0){}
		UInt64 m_nLeaveTime;
		LYNX_MESSAGE_1(	PERSIST_RESETALLPLAYERLEAVETIME, PersistResetAllLeaveTime, UInt64 ,m_nLeaveTime);
	};



	struct PersistRankGameScoreSave
	{
		PersistRankGameScoreSave():m_nPlayerUid(0), m_score(0), m_time(0), m_lastIndex(0), m_lastTime(0), m_addWinCount(0){}
		UInt64 m_nPlayerUid;
		UInt32 m_score;
		UInt32 m_time;
		UInt32 m_lastIndex;
		UInt32 m_lastTime;
		UInt32 m_addWinCount;

		LYNX_MESSAGE_6(PERSIST_RANKGAME_SCORE_SAVE, PersistRankGameScoreSave, UInt64, m_nPlayerUid, UInt32 ,m_score, UInt32 ,m_time, UInt32 ,m_lastIndex, UInt32 ,m_lastTime, UInt32 ,m_addWinCount);

	};


	struct PersistUpdateFriendBeApplyOffLineDataToDb
	{
		PersistUpdateFriendBeApplyOffLineDataToDb():m_nPlayerUid(0){}
		UInt64 m_nPlayerUid;
		List<UInt64> friendBeApplyGuidList;

		LYNX_MESSAGE_2(PERSIST_BE_APPLY_DATA_SAVE, PersistUpdateFriendBeApplyOffLineDataToDb,UInt64, m_nPlayerUid, List<UInt64>, friendBeApplyGuidList);

	};


	struct PersistRankGameReportSave
	{
		PersistRankGameReportSave():m_nPlayerUid(0),deleteID(0),saveID(0){}
		UInt64 m_nPlayerUid;
		UInt32 deleteID;
		UInt32 saveID;
		ReportData reportData;

		LYNX_MESSAGE_4(PERSIST_RANKGAME_REPORT_SAVE, PersistRankGameReportSave, UInt64, m_nPlayerUid,UInt32, deleteID,UInt32, saveID,ReportData, reportData);

	};

	struct PersistOnlineDaySave
	{
		// 	及时存
		PersistOnlineDaySave():m_nPlayerUid(0),dayID(0){}
		UInt64 m_nPlayerUid;
		UInt32 dayID;

		LYNX_MESSAGE_2(PERSIST_DAY_ONLINE_SAVE, PersistOnlineDaySave, UInt64, m_nPlayerUid,UInt32, dayID);
	};

	struct PersistBaseDateSave
	{
		// 	及时存
		PersistBaseDateSave():m_nPlayerUid(0){}
		UInt64 m_nPlayerUid;

		LYNX_MESSAGE_1(PERSIST_BASE_DATA_SAVE, PersistBaseDateSave, UInt64, m_nPlayerUid);
	};



	struct PersistAddFriendOffLineNotify
	{
		PersistAddFriendOffLineNotify():m_nPlayerUid(0),m_nMyselfUid(0),friendStr(""),blackStr(""){}	
		UInt64 m_nPlayerUid; //要添加好友的uid
		UInt64 m_nMyselfUid; //自己的uid
		String friendStr;
		String blackStr;
		UInt32 friendCount;
		LYNX_MESSAGE_5(PERSIST_FRIENDOFFLINE_ADD, PersistAddFriendOffLineNotify, UInt64, m_nPlayerUid,
			UInt64 ,m_nMyselfUid, String, friendStr, String, blackStr, UInt32, friendCount);
	};
		
	struct PersistDelFriendOffLineNotify
	{
		PersistDelFriendOffLineNotify():m_nPlayerUid(0),m_nMyselfUid(0),friendStr(""),blackStr(""){}	
		UInt64 m_nPlayerUid; //要删除好友的uid
		UInt64 m_nMyselfUid; //自己的uid
		String friendStr;
		String blackStr;
		UInt32 friendCount;
		LYNX_MESSAGE_5(RERSIST_FRIENDOFFLINE_DEL, PersistDelFriendOffLineNotify, UInt64, m_nPlayerUid,
			UInt64 ,m_nMyselfUid, String, friendStr, String, blackStr, UInt32, friendCount);
	};

	struct PerisistUpdateOnlineLvRank
	{
		PerisistUpdateOnlineLvRank():m_nTime(0){}
		UInt64 m_nTime;
		LYNX_MESSAGE_1(PERSIST_ONLINELEVELRANK_UPDATE, PerisistUpdateOnlineLvRank, UInt64 ,m_nTime);
	};

	struct PersistGetPowerRank
	{
		PersistGetPowerRank():m_nTime(0){}
		UInt64 m_nTime;
		LYNX_MESSAGE_1(PERSIST_POWERRANK_UPDATE, PersistGetPowerRank, UInt64 ,m_nTime);
	};

	struct PersistGetScoreRank
	{
		PersistGetScoreRank():m_nTime(0){}
		UInt64 m_nTime;
		LYNX_MESSAGE_1(PERSIST_SCORERANK_LOAD, PersistGetScoreRank, UInt64 ,m_nTime);
	};

// 	struct PersistGetRankGameReport
// 	{
// 		PersistGetRankGameReport():m_nTime(0){}
// 		UInt64 m_nTime;
// 		LYNX_MESSAGE_1(PERSIST_RANKGAME_REPORT_LOAD, PersistGetRankGameReport, UInt64 ,m_nTime);
// 	};



	struct PersistPlayerConsortUpdate
	{
		PersistPlayerConsortUpdate():m_nTime(0){}
		UInt64 m_nTime;
		LYNX_MESSAGE_1(PERSIST_PLAYERCONSORT_UPDATE, PersistPlayerConsortUpdate, UInt64 ,m_nTime);
	};

	struct PersistPlayerConsortSave
	{
		PersistPlayerConsortSave():m_nPlayerUid(0), m_nConsortId(0), m_nCurContribute(0),m_nTotalContribute(0),
		m_nConsortJob(0),m_nLeaveTime(0){}
		UInt64 m_nPlayerUid;
		UInt64 m_nConsortId;
		UInt32 m_nCurContribute;
		UInt32 m_nTotalContribute;
		UInt32 m_nConsortJob;
		UInt64 m_nLeaveTime;
		LYNX_MESSAGE_6(PERSIST_PLAYERCONSORT_SAVE, PersistPlayerConsortSave, UInt64, m_nPlayerUid,
			UInt64, m_nConsortId, UInt32, m_nCurContribute,UInt32, m_nTotalContribute, UInt32, m_nConsortJob, UInt64 ,m_nLeaveTime);
	};

	struct PersistConsortSave
	{
		PersistConsortSave():m_nConsortId(0), m_nConsortLv(0), m_strConsortName(""),
		m_nCount(0),m_nPower(0),m_strDescs(""),m_nRes(0), m_nExp(0),m_nCheck(0),m_nPowerLimit(0){}
		
		UInt64 m_nConsortId;
		UInt32 m_nConsortLv;
		String  m_strConsortName;

		UInt32 m_nCount;
		UInt64 m_nPower;
		String  m_strDescs;

		UInt32 m_nRes;
		UInt32 m_nExp;
		UInt32 m_nCheck;

		UInt64 m_nPowerLimit;

		LYNX_MESSAGE_10(PERSIST_CONSORT_SAVE, PersistConsortSave, UInt64, m_nConsortId,
			UInt32, m_nConsortLv, String , m_strConsortName, UInt32, m_nCount,UInt64 ,m_nPower,
			String , m_strDescs, UInt32, m_nRes, UInt32 ,m_nExp, UInt32, m_nCheck, UInt64, m_nPowerLimit);
	};
		
	struct PersistConsortCreate
	{
		PersistConsortCreate():m_nConsortId(0), m_nConsortLv(0), m_strConsortName(""),
		m_nCount(0),m_nPower(0),m_strDescs(""),m_nRes(0), m_nExp(0),m_nCheck(0),m_nPowerLimit(0){}
		
		UInt64 m_nConsortId;
		UInt32 m_nConsortLv;
		String  m_strConsortName;

		UInt32 m_nCount;
		UInt64 m_nPower;
		String  m_strDescs;

		UInt32 m_nRes;
		UInt32 m_nExp;
		UInt32 m_nCheck;

		UInt64 m_nPowerLimit;
		UInt64 m_nLeader;
		LYNX_MESSAGE_11(PERSIST_CONSORT_CREATE, PersistConsortCreate, UInt64, m_nConsortId,
			UInt32, m_nConsortLv, String , m_strConsortName, UInt32, m_nCount,UInt64 ,m_nPower,
			String , m_strDescs, UInt32, m_nRes, UInt32 ,m_nExp, UInt32, m_nCheck, UInt64, m_nPowerLimit,UInt64 ,m_nLeader);
	};
	
	struct PersistConsortApplyInsert
	{
		PersistConsortApplyInsert():m_nPlayerUid(0),m_nConsortId(0), m_nTime(0){}
		
		UInt64 m_nPlayerUid;
		UInt64 m_nConsortId;
		UInt64 m_nTime;

		LYNX_MESSAGE_3(PERSIST_CONSORTAPPLY_INSERT, PersistConsortApplyInsert, UInt64, m_nPlayerUid,
			UInt64, m_nConsortId, UInt64, m_nTime);
	};
	

	struct PersistConsortApplyDel
	{
		PersistConsortApplyDel():m_nPlayerUid(0),m_nConsortId(0){}
		
		UInt64 m_nPlayerUid;
		UInt64 m_nConsortId;
	

		LYNX_MESSAGE_2(PERSIST_CONSORTAPPLY_DEL, PersistConsortApplyDel, UInt64, m_nPlayerUid,
			UInt64, m_nConsortId);
	};
	
	struct PersistConsortApplyClear
	{
		PersistConsortApplyClear():m_nPlayerUid(0){}
		
		UInt64 m_nPlayerUid;
		
	

		LYNX_MESSAGE_1(PERSIST_CONSORTAPPLY_CLEAR, PersistConsortApplyClear, UInt64, m_nPlayerUid);
	};

	struct PersistConsortApplyAllClear
	{
		PersistConsortApplyAllClear():m_nConsortId(0){}
		
		UInt64 m_nConsortId;
		
	

		LYNX_MESSAGE_1(PERSIST_CONSORTAPPLY_ALLCLEAR, PersistConsortApplyAllClear, UInt64, m_nConsortId);
	};

	struct PersistConsortDel
	{
		PersistConsortDel():m_nConsortId(0){}
		
		UInt64 m_nConsortId;
		
		LYNX_MESSAGE_1(PERSIST_CONSORT_DEL, PersistConsortDel, UInt64, m_nConsortId);
	};

	struct PersistConsortLogInsert
	{
		PersistConsortLogInsert():m_nTempId(0),m_nPlayerUid(0), m_nTime(0),m_nConsortId(0), m_strPlayerName(""),
			m_strParam1(""), m_strParam2(""){}
		
		UInt64 m_nTempId;
		UInt64 m_nPlayerUid;
		UInt64 m_nTime;
		UInt64 m_nConsortId;
		String m_strPlayerName;
		String m_strParam1;
		String m_strParam2;

		LYNX_MESSAGE_7(PERSIST_CONSORTLOG_INSERT, PersistConsortLogInsert,UInt64, m_nTempId, 
			UInt64, m_nPlayerUid, UInt64,  m_nTime, UInt64, m_nConsortId, String, m_strPlayerName,
			String, m_strParam1,String ,m_strParam2);
	};
	
	struct PersistConsortSignReset
	{
		PersistConsortSignReset():m_nResetTime(0){}

		UInt64 m_nResetTime;

		LYNX_MESSAGE_1(PERSIST_CONSORTSIGN_RESET, PersistConsortSignReset,UInt64, m_nResetTime );

	};

	struct PersistConsortSignUpdate
	{
		PersistConsortSignUpdate():m_nSign(0), m_nPlayerUid(0){}

		UInt64 m_nPlayerUid;
		List<UInt32> m_nSignAwards;
		UInt32 m_nSign;

		LYNX_MESSAGE_3(PERSIST_CONSORTSIGN_UPDATE, PersistConsortSignUpdate,List<UInt32>, m_nSignAwards,
			UInt32, m_nSign, UInt64, m_nPlayerUid);
	};

	struct PersistConsortActiveUpdate
	{
		PersistConsortActiveUpdate():m_nConsortId(0), m_nActive(0){}

		UInt64 m_nConsortId;
		
		UInt32 m_nActive;

		LYNX_MESSAGE_2(PERSIST_CONSORTACTIVE_UPDATE, PersistConsortActiveUpdate,UInt64, m_nConsortId,
			UInt32 ,m_nActive);
	};

	struct PersistConsortLoyalUpdate
	{
		PersistConsortLoyalUpdate():m_nPlayerUid(0), m_nRefreshTimes(0){}

		UInt64 m_nPlayerUid;
		UInt32 m_nRefreshTimes;
		List<XingxiaTask>  m_listXingxiaTasks; 

		LYNX_MESSAGE_3(PERSIST_CONSORTLOYAL_UPDATE, PersistConsortLoyalUpdate,UInt64, m_nPlayerUid,UInt32 ,m_nRefreshTimes,
			List<XingxiaTask>,  m_listXingxiaTasks);
	};

	struct PersistConsortKitQueUpdate
	{
		PersistConsortKitQueUpdate():m_nPlayerUid(0), m_nRefreshTimes(0){}

		UInt64 m_nPlayerUid;
		UInt32 m_nRefreshTimes;
		
		LYNX_MESSAGE_2(PERSIST_CONSORTKITQUE_UPDATE, PersistConsortKitQueUpdate,UInt64, m_nPlayerUid,UInt32 ,m_nRefreshTimes);
	};

	struct PersistBusinessCatUpdate
	{
		PersistBusinessCatUpdate():m_nPlayerUid(0), m_nRefreshTimes(0){}

		UInt64 m_nPlayerUid;
		UInt32 m_nRefreshTimes;
		List<UInt32> m_nBuyList;
		
		LYNX_MESSAGE_3(PERSIST_BUSINESSCAT_UPDATE, PersistBusinessCatUpdate,UInt64, m_nPlayerUid,UInt32 ,m_nRefreshTimes,
			List<UInt32>, m_nBuyList);
	};

	struct PersistBusinessCatTimeReset
	{
		PersistBusinessCatTimeReset(): m_nResetTime(0){}

		
		UInt64 m_nResetTime;
		
		LYNX_MESSAGE_1(PERSIST_BUSINESSCATTIME_RESET, PersistBusinessCatTimeReset,UInt64 ,m_nResetTime);
	};
	 
	struct PersistEyeTimesUpdate
	{
		PersistEyeTimesUpdate(): m_nEyeTimes(0){}

		UInt64 m_nPlayeruid;
		UInt32 m_nEyeTimes;
		
		LYNX_MESSAGE_2(PERSIST_EYEQUETIMES_UPDATE, PersistEyeTimesUpdate,UInt64, m_nPlayeruid,UInt32 ,m_nEyeTimes);
	};
	 
	struct PersistTicketTimeReset
	{
		PersistTicketTimeReset(): m_nResetTime(0){}

		UInt64 m_nResetTime;
		
		LYNX_MESSAGE_1(PERSIST_TICKETTIME_RESET, PersistTicketTimeReset , UInt64, m_nResetTime);
	};

	struct PersistTicketFriendUpdate
	{
		PersistTicketFriendUpdate(): m_strData(""), m_nConsortuid(0),m_nTicketid(0){}

		UInt64 m_nConsortuid;
		UInt32 m_nTicketid;
		String m_strData;
		
		LYNX_MESSAGE_3(PERSIST_CONSORTTICKET_UPDATE, PersistTicketFriendUpdate , UInt64, m_nConsortuid,
			UInt32, m_nTicketid,String, m_strData);
	};

	struct PersistTicketQualityUpdate
	{
		PersistTicketQualityUpdate(): m_nQuality(0), m_nConsortuid(0),m_nTicketid(0){}

		UInt64 m_nConsortuid;
		UInt32 m_nTicketid;
		UInt32 m_nQuality;
		
		LYNX_MESSAGE_3(PERSIST_TICKETQUALITY_UPDATE, PersistTicketQualityUpdate , UInt64, m_nConsortuid,
			UInt32, m_nTicketid, UInt32, m_nQuality);
	};

	struct PersistTicketFriendInit
	{
		PersistTicketFriendInit(): m_strExe(""){}

		String m_strExe;
		
		LYNX_MESSAGE_1(PERSIST_TICKETFRIEND_INIT, PersistTicketFriendInit , String, m_strExe);
	};

	struct PersistTicketAllDel
	{
		PersistTicketAllDel():m_nDel(0){}
		UInt64 m_nDel;
		LYNX_MESSAGE_1(PERSIST_TICKETALL_DEL, PersistTicketAllDel,UInt64, m_nDel );
	};

	struct PersistTicketDataUpdate
	{
		PersistTicketDataUpdate():m_strData(""), m_nConsortId(0),m_nTicketId(0) {}
		UInt64 m_nConsortId;
		UInt32 m_nTicketId;
		String  m_strData;
		LYNX_MESSAGE_3(PERSIST_TICKETDATA_UPDATE, PersistTicketDataUpdate,UInt64, m_nConsortId ,
			UInt32 ,m_nTicketId,String,  m_strData);
	};

	struct PersistTicketSupport
	{
		PersistTicketSupport():m_nQuality(0), m_strFriends(""),m_nConsortId(0), m_nTicketId(0){}
	    UInt32 m_nQuality;
		String  m_strFriends;
		UInt64 m_nConsortId;
		UInt32 m_nTicketId;
		LYNX_MESSAGE_4(PERSIST_TICKETSUPPORT_UPDATE, PersistTicketSupport,UInt32, m_nQuality ,
			String,  m_strFriends,UInt64,m_nConsortId,UInt32, m_nTicketId);
	};

	struct PersistTicketAwardAdd
	{
		PersistTicketAwardAdd():m_nTicketId(0), m_nAwardId(0),m_nPeapleCnt(0),
			m_nEndTime(0), m_nPlayeruid(0){}
	    UInt32 m_nTicketId;
		UInt64 m_nAwardId;  
		UInt32 m_nPeapleCnt;
		UInt64 m_nEndTime;
		UInt64 m_nPlayeruid;
		LYNX_MESSAGE_5(PERSIST_TICKETAWARD_INSERT, PersistTicketAwardAdd, UInt32 ,m_nTicketId,
			UInt64 ,m_nAwardId, UInt32 ,m_nPeapleCnt, UInt64,  m_nEndTime, UInt64, m_nPlayeruid);
	};

	struct PersistTicketAwardUpdate
	{
		PersistTicketAwardUpdate():m_nTicketId(0), m_nAwardId(0),m_nPeapleCnt(0),
			m_nEndTime(0), m_nPlayeruid(0){}
	    UInt32 m_nTicketId;
		UInt64 m_nAwardId;  
		UInt32 m_nPeapleCnt;
		UInt64 m_nEndTime;
		UInt64 m_nPlayeruid;
		LYNX_MESSAGE_5(PERSIST_TICKETAWARD_UPDATE, PersistTicketAwardUpdate, UInt32 ,m_nTicketId,
			UInt64 ,m_nAwardId, UInt32 ,m_nPeapleCnt, UInt64,  m_nEndTime, UInt64, m_nPlayeruid);
	};
		 
	struct PersistTicketTimesUpdate
	{
		PersistTicketTimesUpdate():m_nPlayeruid(0),m_nTimes(0){}
	    UInt32 m_nTimes;
		UInt64 m_nPlayeruid;
		LYNX_MESSAGE_2(PERSIST_TICKETTIMES_UPDATE, PersistTicketTimesUpdate, UInt32 ,m_nTimes, UInt64, m_nPlayeruid);
	};
	
	struct PersistTicketAwardDel
	{
		PersistTicketAwardDel():m_nPlayeruid(0){}
	   
		UInt64 m_nPlayeruid;
		LYNX_MESSAGE_1(PERSIST_TICKETAWARD_DEL, PersistTicketAwardDel, UInt64, m_nPlayeruid);
	};
		


	struct PersistTicketTimesReset
	{
		PersistTicketTimesReset():resettime(0){}
	   
		UInt64 resettime;
		LYNX_MESSAGE_1(PERSIST_TICKETTIMES_RESET, PersistTicketTimesReset, UInt64, resettime);
	};

	struct PersistLoyalTimesReset
	{
		PersistLoyalTimesReset():resettime(0){}
	   
		UInt64 resettime;
		LYNX_MESSAGE_1(PERSIST_LOYALTIMES_RESET, PersistLoyalTimesReset, UInt64, resettime);
	};

	struct PersistKitchenTimesReset
	{
		PersistKitchenTimesReset():resettime(0){}
	   
		UInt64 resettime;
		LYNX_MESSAGE_1(PERSIST_KITCHENTIMES_RESET, PersistKitchenTimesReset, UInt64, resettime);
	};
		
	struct PersistEyeTimesReset
	{
		PersistEyeTimesReset():resettime(0){}
	   
		UInt64 resettime;
		LYNX_MESSAGE_1(PERSIST_EYETIMES_RESET, PersistEyeTimesReset, UInt64, resettime);
	};
		
	struct PersistEloquenceTimesUpdate
	{
		PersistEloquenceTimesUpdate():playeruid(0),times(0){}
	    UInt64 playeruid;
		UInt64 times;
		LYNX_MESSAGE_2(PERSIST_ELOQUENCETIMES_UPDATE, PersistEloquenceTimesUpdate, UInt64, playeruid,
			UInt64 ,times);
	};

	struct PersistEloquenceTimesReset
	{
		PersistEloquenceTimesReset():resettime(0){}
	   
		UInt64 resettime;
		LYNX_MESSAGE_1(PERSIST_ELOQUENCETIMES_RESET, PersistEloquenceTimesReset,UInt64, resettime);
	};

	struct PersistWoodTimesReset
	{
		PersistWoodTimesReset():resettime(0){}
	   
		UInt64 resettime;
		LYNX_MESSAGE_1(PERSIST_WOODTIMES_RESET, PersistWoodTimesReset,UInt64, resettime);
	};

	struct PersistWoodTotalUpdate
	{
		PersistWoodTotalUpdate():consortid(0), woodleft(0), enhance1(0), enhance2(0),enhance3(0){}
	   
		UInt64 consortid;
		UInt32 woodleft;
		UInt32 enhance1;
		UInt32 enhance2;
		UInt32 enhance3;

		
		LYNX_MESSAGE_5(PERSIST_WOODTOTAL_UPDATE, PersistWoodTotalUpdate,UInt64, consortid, 
			UInt32,  woodleft, UInt32 ,enhance1, UInt32 , enhance2, UInt32, enhance3
			);
	};
	
	struct PersistWoodTotalReset
	{
		PersistWoodTotalReset(){}
	   
		LYNX_MESSAGE_0(PERSIST_WOODTOTAL_RESET, PersistWoodTotalReset);
	};
	
	struct PersistWoodSelfUpdate
	{
		PersistWoodSelfUpdate():playeruid(0), awardflag(0), times(0){}
	   UInt64 playeruid;
	   UInt32 awardflag;
	   UInt32 times;
		LYNX_MESSAGE_3(PERSIST_WOODSELF_UPDATE, PersistWoodSelfUpdate, UInt64, playeruid,
			UInt32, awardflag, UInt32, times);
	};

	

	struct PersistAddEmail
	{
		PersistAddEmail(){}
		EmailData mEmailData;
		LYNX_MESSAGE_1(PERSIST_INSERT_EMAIL_NOTIFY,PersistAddEmail, EmailData,mEmailData);
		
	};

	struct PersistDelEmail
	{
		PersistDelEmail(): mEmailUid(0){}
		UInt64 mEmailUid;
		LYNX_MESSAGE_1(PERSIST_DELETE_EMAIL_NOTIFY,PersistDelEmail, UInt64,mEmailUid);

	};

	struct PersistUpdateEmail
	{
		PersistUpdateEmail(){}
		EmailData mEmailData;
		LYNX_MESSAGE_1(PERSIST_UPDATE_EMAIL_NOTIFY,PersistUpdateEmail, EmailData,mEmailData);

	};

	struct PersistClearEmail
	{
		PersistClearEmail(){}
		UInt64 mPlayerUid;
		LYNX_MESSAGE_1(PERSIST_CLEAR_EMAIL_NOTIFY,PersistClearEmail, UInt64, mPlayerUid);
		
	};

	struct FashionAdd
	{
		FashionAdd():mPlayerUid(0),mFashionId(0){}
		UInt64 mPlayerUid;
		
		UInt64 mFashionId;
		LYNX_MESSAGE_2(PERSIST_FASHION_INSERT,FashionAdd, UInt64, mPlayerUid,UInt64, mFashionId);
		
	};

	struct CharactorAdd
	{
		CharactorAdd():charactorUid(0), charactorId(0), fashionId(0), playerUid(0){}
		UInt64 charactorUid;
		UInt64 charactorId;
		UInt64 fashionId;
		UInt64 playerUid;
		LYNX_MESSAGE_4(PERSIST_CHARACTOR_INSERT,CharactorAdd, UInt64, charactorUid,UInt64, charactorId,UInt64, fashionId,UInt64, playerUid);
	};

	struct CharactorUpdate
	{
		CharactorUpdate():charactorid(0), fashionId(0),playeruid(0){}
		UInt64 charactorid;
		UInt64 fashionId;
		UInt64 playeruid;
		LYNX_MESSAGE_3(PERSIST_CHARACTOR_UPDATE,CharactorUpdate, UInt64, charactorid,UInt64, fashionId,
			UInt64 ,playeruid);
	};

	struct AchieveUpdateMsg
	{
		AchieveUpdateMsg():playerUid(0){}
		UInt64 playerUid;
		AchieveData achieveData;
		LYNX_MESSAGE_2(PERSIST_ACHIEVE_UPDATE,AchieveUpdateMsg, UInt64, playerUid, AchieveData, achieveData);
	};
	//更新离线好友数量成就
	struct OffLineFriendUpdateMsg
	{
		OffLineFriendUpdateMsg():playerUid(0){}
		UInt64 playerUid;
		UInt32 count;
		LYNX_MESSAGE_2(PERSIST_OFFLINEFRIEND_UPDATE,OffLineFriendUpdateMsg, UInt64, playerUid, UInt32, count);
		
	};

	struct PersistDailyTaskUpdateMsg
	{
		PersistDailyTaskUpdateMsg():playerUid(0){}
		UInt64 playerUid;
		DailyTaskData dailyTask;
		LYNX_MESSAGE_2(PERSIST_DAILYTASK_UPDATE,PersistDailyTaskUpdateMsg, UInt64, playerUid, DailyTaskData, dailyTask);

	};

	struct PersistDailyActiveUpdateMsg
	{
		PersistDailyActiveUpdateMsg():playerUid(0){}
		UInt64 playerUid;
		PlayerDailyActiveData dailyacvData;
		LYNX_MESSAGE_2(PERSIST_DAILYACTIVE_UPDATE,PersistDailyActiveUpdateMsg, UInt64, playerUid, 	PlayerDailyActiveData, dailyacvData);

	};


	struct PersistDailyLastTimeUpdateMsg
	{
		PersistDailyLastTimeUpdateMsg():playerUid(0){}
		UInt64 playerUid;
		 UInt64 time;
		LYNX_MESSAGE_2(PERSIST_DAILYLASTTIME_UPDATE,PersistDailyLastTimeUpdateMsg, UInt64, playerUid, 	UInt64, time);

	};
	
	//新手引导消息存盘
	struct PersistGuidStepUpdateMsg
	{
		PersistGuidStepUpdateMsg():playerUid(0),guidStr(""){}
		UInt64 playerUid;
		 String guidStr;
		LYNX_MESSAGE_2(PERSIST_UPDATE_PLAYER_GUID,PersistGuidStepUpdateMsg, UInt64, playerUid, 	 String, guidStr);

	};
	
	//新手引导所需材料标记存盘
	struct PersistGuidFlagUpdateMsg
	{
		PersistGuidFlagUpdateMsg():playerUid(0),giftflag(0){}
		UInt64 playerUid;
		 UInt32  giftflag;
		LYNX_MESSAGE_2(PERSIST_UPDATE_PLAYER_GUIDGIFT,PersistGuidFlagUpdateMsg, UInt64, playerUid, UInt32, giftflag);

	};
	
	struct PersistForbidLoginTimeUpdate
	{
		PersistForbidLoginTimeUpdate():playerUid(0),forbidlogintime(0),forbidbegintime(0){}
		UInt64 playerUid;
		 UInt64  forbidlogintime;
		 UInt64  forbidbegintime;
		LYNX_MESSAGE_3(PERSIST_FORBIDLOGINTIME_UPDATE,PersistForbidLoginTimeUpdate, UInt64, playerUid,  
			UInt64,  forbidlogintime, UInt64 , forbidbegintime);

	};

	struct PersistForbidChatUpdate
	{
		PersistForbidChatUpdate():playerUid(0),disableflag(0), begintime(0), timelong(0){}
		UInt64 playerUid;
		UInt32  disableflag;
        UInt64  begintime;
		UInt64  timelong;
		LYNX_MESSAGE_4(PERSIST_FORBIDCHAT_UPDATE,PersistForbidChatUpdate, UInt64, playerUid, UInt32, disableflag,
			UInt64 , begintime, UInt64,  timelong);

	};

	
		


} // namespace Lynx

#endif // __LYNX_GAME_SERVER_PERSIST_THREAD_MSG_H__
