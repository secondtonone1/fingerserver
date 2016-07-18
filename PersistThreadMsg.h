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

		PERSIST_ONLINELEVELRANK_UPDATE = 900, //从数据库load数据更新在线玩家等级排行
		PERSIST_POWERRANK_UPDATE	 = 901, //从数据库load数据，所有玩家战力排行

		PERSIST_ACHIEVE_UPDATE = 902,  //更新成就到数据库
		PERSIST_OFFLINEFRIEND_UPDATE = 903,//更新离线玩家的好友数量
		PERSIST_DAILYTASK_UPDATE = 904,//日常任务添加请求存盘
		PERSIST_DAILYACTIVE_UPDATE = 905, //日常活跃度表存盘
		PERSIST_DAILYLASTTIME_UPDATE = 906, //日常活跃度上次清理时间更新消息

		PERSIST_FASHION_INSERT = 1000,

		PERSIST_CHARACTOR_INSERT = 1002, //角色插入数据库
		PERSIST_CHARACTOR_UPDATE = 1001 //角色更新数据
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
		PersistFindSimilarPowerResp() : playerID(0),times(0){}

		Guid playerID;
		UInt32 times;//10000多人副本标识
		List<Guid> playerIDList;

		LYNX_MESSAGE_3(PERSIST_FIND_SIMILAR_POWER_RESP, PersistFindSimilarPowerResp,
			Guid, playerID,UInt32, times,List<Guid>, playerIDList);
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
		mCharactorGuid(0){}

        UInt64 mPlayerGuid;

		UInt64 mItemGuid;

		UInt64 mLansquenetGuid;

		UInt64 mGuildGuid;

		UInt64 mEmailGuid;

		UInt64 mMaterialGuid;

		UInt64 mCharactorGuid;

        LYNX_MESSAGE_7(PERSIST_SYNC_BASE_GUID_NOTIFY, PersistSyncBaseGuidNotify,
            UInt64, mPlayerGuid,UInt64, mItemGuid,UInt64, mLansquenetGuid,UInt64, mGuildGuid,UInt64, mEmailGuid, UInt64, mMaterialGuid,
			UInt64, mCharactorGuid);
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
		PersistCreatePlayerResp():mRes(0),mPlayerUid(0){}
		UInt16 mRes;
		UInt64 mPlayerUid;
		ConnId mConnId;
		LYNX_MESSAGE_3(PERSIST_CREATE_PLAYER_RESP, PersistCreatePlayerResp,
			UInt16, mRes, UInt64, mPlayerUid, ConnId, mConnId);
		
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

		PersistFindSimilarPowerReq():playerGuid(0),times(1),high(0),low(0){}

		LYNX_MESSAGE_4(PERSIST_FIND_SIMILAR_POWER, PersistFindSimilarPowerReq,  Guid, playerGuid,UInt32, times,UInt32, high,UInt32, low);
	};

	struct PersistServerDataReq
	{
		UInt32 gRechargeNum ;//充值人数
		UInt32 gServerRobotRefreshFlag ;//0重新导入机器人
		UInt32 gServerSpare2 ;//充值人数
		UInt32 gServerSpare3 ;//充值人数
		UInt32 gServerSpare4 ;//充值人数
		UInt32 gServerSpare5 ;//充值人数

		PersistServerDataReq():gRechargeNum(1),gServerRobotRefreshFlag(0),gServerSpare2(0),gServerSpare3(0),gServerSpare4(0),gServerSpare5(0){}

		LYNX_MESSAGE_6(PERSIST_SERVERDATA_REQ, PersistServerDataReq,UInt32,gRechargeNum,UInt32, gServerRobotRefreshFlag,UInt32, gServerSpare2,UInt32, gServerSpare3,UInt32, gServerSpare4,UInt32, gServerSpare5);
	};
	struct PersistRobotDataReq
	{
		Guid uuid ;//
		UInt32 uid ;//
		String name ;//
		UInt32 level ;//
		UInt32 viplevel ;//
		UInt32 power ;//
// INSERT INTO robot(uuid,uid,name,level,viplevel,power)VALUES(1111,1,"1111",33,4,1312)
		PersistRobotDataReq():uuid(0),uid(0),name(""),level(0),viplevel(0),power(0){}

		LYNX_MESSAGE_6(PERSIST_ROBOTDATA_REQ, PersistRobotDataReq,Guid,uuid,UInt32, uid,String, name,UInt32, level,UInt32, viplevel,UInt32, power);
	};


	struct PersistInlineActivityReq
	{
		PersistInlineActivityReq():playerGuid(0),type(0),m_GrowFoundBuyFlag(0),m_MonthSignCount(0),m_MonthSignTime(0),m_OnlineWelFareOnlineTime(0),m_PeopleWelfareRechargeNum(0),
			m_FirstLoginTime(0), m_SevenLoginGotCount(0),m_LastGetTime(0),m_LoginTime(0),m_TimeAwardRefreshTime(0){}		

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


		LYNX_MESSAGE_18(PERSIST_INLINE_ACTIVITY_POWER,PersistInlineActivityReq,UInt32, type,Guid, playerGuid,UInt32, m_GrowFoundBuyFlag,List<UInt32>, m_GrowFoundGotList, UInt32, m_MonthSignCount, UInt32, m_MonthSignTime,
			UInt32, m_OnlineWelFareOnlineTime,List<UInt32>, m_OnlineWelFareGotList,UInt32, m_PeopleWelfareRechargeNum,List<UInt32>, m_PeopleWelfareGotList,
			UInt32, m_FirstLoginTime,UInt32, m_LastGetTime, UInt32, m_SevenLoginGotCount, List<KeyValue>, m_SevenDayTaskGotList, UInt32, m_TimeAwardRefreshTime,
			List<UInt32>, m_TimeAwardGotList,UInt32, m_LoginTime,List<KeyList>, m_SevenDayTaskFinishList);
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
		FashionAdd():mPlayerUid(0), mFashionUid(0), mFashionId(0){}
		UInt64 mPlayerUid;
		UInt64 mFashionUid;
		UInt64 mFashionId;
		LYNX_MESSAGE_3(PERSIST_FASHION_INSERT,FashionAdd, UInt64, mPlayerUid,UInt64, mFashionUid,UInt64, mFashionId);
		
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
		CharactorUpdate():charactorUid(0), fashionId(0){}
		UInt64 charactorUid;
		UInt64 fashionId;
		LYNX_MESSAGE_2(PERSIST_CHARACTOR_UPDATE,CharactorUpdate, UInt64, charactorUid,UInt64, fashionId);
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
	
	

	

} // namespace Lynx

#endif // __LYNX_GAME_SERVER_PERSIST_THREAD_MSG_H__
