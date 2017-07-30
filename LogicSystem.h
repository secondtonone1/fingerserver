#ifndef __LYNX_GAME_SERVER_LOGIC_SYSTEM_H__
#define __LYNX_GAME_SERVER_LOGIC_SYSTEM_H__

#include "MainThread.h"
#include "Player.h"

#include "Guild.h"

namespace Lynx
{
	
	class LogicSystem : public MainThread, public Singleton<LogicSystem>
	{
		friend class ConsortSystem;
	public:
        LogicSystem();
		virtual ~LogicSystem();

		bool initial();
		void release();
		void update(const UInt64& accTime);

		void  dailyRefresh(const UInt64& accTime);

		void  getDailyRefreshList(UInt32 refreshTime,List<UInt32>&refreshList);

		UInt32  getRefreshTime(UInt32 systemID);

		

		//成就系统日常更新，跨天通知， 默认是零点通知
		void dailyTaskRefresh(const UInt64& accTime);

        bool reloadTable(const String& tableName);
        bool reloadAllTable();

        void setPlayerGuid(const Guid& guidBase) { mPlayerGuidBase = guidBase; }

		const Guid& getPlayerGuid(){return mPlayerGuidBase;}
 
		Guid getItemGuid(){ return mItemGuidBase; }

		//新生成itemGuid
        Guid generateItemGuid() {++mItemGuidBase;
			//baseUidPersist();
			return mItemGuidBase; }
      		
		//设置全局唯一的itemUid
		void setItemGuid(const Guid& itemGuid)
		{
			mItemGuidBase = itemGuid;
		}

		void setLansquenetGuid(const Guid& lansquenetGuid)
		{
			mLansquenetGuid = lansquenetGuid;
		}

		void setGuildGuid(const Guid& guildGuid)
		{
		  mGuildGuid = guildGuid;
		}

		//新生成emailGuid
		Guid generateEmailGuid() {++mEmailGuid;
		return mEmailGuid; }

		void setEmailGuid(const Guid& emailGuid)
		{
			mEmailGuid = emailGuid;
		}

		//生成材料id
		Guid generateMaterialGuid() {++mMaterialGuid;
		//baseUidPersist();
		return mMaterialGuid; }

		void setMaterialGuid(const Guid& materialGuid)
		{
			mMaterialGuid = materialGuid;
		}

		//生成角色id，以及日常任务id
		Guid generateCharactorGuid(){++mCharactorGuid;
		//baseUidPersist();
		return mCharactorGuid;}

		void setCharactorGuid(const Guid& charactorGuid)
		{
			mCharactorGuid = charactorGuid;
		}

		Guid generateConsortGuid(){++mConsortGuid;
		//baseUidPersist();
		return mConsortGuid;}

		void setConsortGuid(const Guid& consortGuid)
		{
			mConsortGuid = consortGuid;
		}
	
		void setTicketGuid(const Guid& ticketGuid)
		{
			mTicketGuid = ticketGuid;
		}
	
		Guid generateTicketGuid(){++mTicketGuid;
		//baseUidPersist();
		return mTicketGuid;}



		//玩家计数存盘
		void baseUidPersist();

		Player* createPlayer(const ConnId& connId, const PlayerData& playerData, 
            const UInt64& loginToken);
		void destroyPlayerByGuid(const Guid& guid);
		void destroyPlayerByConnId(const ConnId& connId);
		void destroyPlayerByName(const String& playerName);
        void destroyPlayerByAccountId(const Guid& accountId);
        void destroyPlayerByAccountName(const String& accountName);
		Player* getPlayerByConnId(const ConnId& connId);
		Player* getPlayerByGuid(const Guid& guid);
		Player* getPlayerByName(const String& playerName);
        Player* getPlayerByAccountGuid(const Guid& guid);
        Player* getPlayerByAccountName(const String& accountName);

        const Map<ConnId, Player*>& getPlayerConnectionMap() const { return mPlayerConnectionMap; }

        void passportAuthSuccess(const ConnId& connId, const Guid& accountId, const String& accoutName);
        void passportAuthFail(const ConnId& connId, const String& accountName);
        
		void loadPlayerData(const ConnId& connId, const Guid& playerGuid);
        void loadPlayerDataSuccess(const ConnId& connId, const PlayerData& playerData);
        void loadPlayerDataFail(const ConnId& connId, const Guid& playerGuid);
        void reconnect(const ConnId& connId, const Guid& playerGuid, const UInt64& loginToken);

    // 系统广播接口
        void broadcaseSystemNotice(const String& contents, UInt8 priority);
        void sendSystemNoticeToPlayer(const Guid& playerGuid, const String& contents, UInt8 priority);

		//发邮件
		void GMEmailSend(UInt64 tempid, UInt64 toPlayer, std::string des, std::string contant, std::string title, std::string from);

    // 公会接口
    public:
        Guild* createGuild(const Guid& playerGuid, const String& guildName);
        void destroyGuildByGuid(const Guid& guildGuid);
        void destroyGuildByName(const String& guildName);
        Guild* getGuildByGuid(const Guid& guildGuid);
        Guild* getGuildByName(const String& guildName);

	public:
		//生成随机数
		List<UInt32> getRandomList(int randomCount, int randomMax);

		void updateDailyResetData(Player *player,UInt32 flag,List<UInt32> &refreshIDs);

		void checkPlayerDataFromDB(Player& player);

		bool combinSendData(Guid playerID,UInt32 resType,Json::Value jvalue);

		bool combinSendDataTest(Guid playerID,UInt32 resType,const Json::Value &jvalue);

	public:
		//玩家信息
		std::string getDetailInfo(std::string playerUidStr); 

		void  setDetailInfo(UInt64 playerID, std::string detailInfo);

		//获取玩家概要信息
		bool getBaseInfo(UInt64 playerUid, BaseInfoData & baseInfoData);

		//获取baseinfomap
		const Map<UInt64, BaseInfoData> &getBaseInfoMap();

		void insertBaseInfo(const BaseInfoData & baseInfoData);
		//登陆或下线时更新玩家信息
		void updateBaseInfo(const PlayerBaseData &baseData);

		void updateBaseInfo(const BaseInfoData & baseInfoData);

		//登录或下线更新玩家帮会信息
		void updateConsortInfo(UInt64 playerUid, const PlayerConsortData & playerConsortData);

		//定时更新玩家军团数据
		void updateConsortInfo(UInt64 playerUid, const ConsortInfoData& consortInfoData);

		//获取玩家帮会信息
		ConsortInfoData  * getConsortInfo(UInt64 playerUid);
		

		//更新公会缓存，目前保留使用
		//void updateConsortInfo(const ConsortInfoData & consortInfoData);

		Guid listRandOne(Guid playerID,List<GuidValue> keyValueList,UInt32 flag,UInt32 initialValue);

			
		void dealSimililarPower(Guid playerID,Guid otherPlayerID,UInt32 times);

		void kickOutAllPlayer();

		void logPlayerLogin(PlayerBaseData baseData);

		void logPlayerLogout(PlayerBaseData baseData);

		void  updateSevenDayTask(Guid playerID,UInt32 eventType,UInt32 change);

	
		void initRoleInfoReq(UInt64 playerID,std::string name,UInt32 modleID);

		void write_log(UInt32 logType,UInt64 playerID, char * dest,UInt32 logLevel);

		void write_server_log(UInt32 logType,char * dest,UInt32 logLevel);


	 public:
	
		 //下限存盘
		 void onPersistOffLineSaveReq(UInt64 playerUid);
	public:
		Set<Player >   getPlayerLvSet(Player * selfPlayer);
		
	public:
			//留给别的接口处理成就，奖励等系统消息
		void sendSystemMsg(UInt64 tempId, std::string param1, UInt32 param2);
		//发送GM消息
		void sendGMSystemMsg(std::string msg);

		void sendGMAwardBag(UInt64 playeruid,UInt64 awardId);

		void setForbidLogin(UInt32 i);

		void sendGMMsg(std::string msg,  UInt64 endtime, UInt32 term);

        //GM重置系统玩法次数
		//重置通宝当铺次数
		void gmResetTongBao(UInt64 playeruid);
		//重置猛畜生市场
		void gmResetMengChu(UInt64 playeruid);
       //重置玉石
		void gmResetYushiGF(UInt64 playeruid);
		//集市安保
		void gmResetJiShiAB(UInt64 playeruid);
		//巴山试炼
		void gmResetBashanSL(UInt64 playeruid);
		//无限挑战
		void gmResetWuxianTZ(UInt64 playeruid);
       //勇气试炼
		void gmResetCourage(UInt64 playeruid);
		//排位赛
         void gmResetPaiWei(UInt64 playeruid);
		 //排位赛购买
		  void gmResetPaiWeiBuy(UInt64 playeruid);
		  //十二宗宫号角
		 void gmReset12HaoJiao(UInt64 playeruid);
		//烹饪次数
		 void gmResetFoodsCook(UInt64 playeruid);
		 //赏罚令免费抽一次
		 void gmResetServantCall1(UInt64 playeruid);
		 //赏罚令免费抽十次
		 void gmResetServantCall10(UInt64 playeruid);
		 //每日任务进度清空
		 void gmResetDailyTask(UInt64 playeruid);
		 //每月签到当天签到清除
		 void gmResetMonthToday(UInt64 playeruid);
		  //七日登录清除为没登录
		 void gmResetSevenDay(UInt64 playeruid);
		 //七日讯清除所有记录
		 void gmResetSevenTrain(UInt64 playeruid);

		 //成长基金清除
		 void gmResetGrowBuy(UInt64 playeruid);
		 //分时奖励
		 void gmResetFenShi(UInt64 playeruid);
		 //在线奖励
		 void gmResetOnlineAward(UInt64 playeruid);
		 //拜财神
		 void gmResetBaiCaiShen(UInt64 playeruid);
		 //购买体力
		 void gmResetStrengthBuy(UInt64 playeruid);

		 //重置脸谱上电
		 void gmResetLianPu(UInt64 playeruid);
		 //通宝商店重置
		 void gmResetTongBaoS(UInt64 playeruid);
		 //珍宝阁商店重置
		 void gmResetZBG(UInt64 playeruid);

		 //京剧猫商店重置
		 void gmResetServantShop(UInt64 playeruid);

		 const Vector<AchieveData>& getAchieveVec();

		 const Vector<DailyTaskData> & getDailyTaskVec();
    private:

		void initAllAchieve();	

		void initAllDailyTask();

		void addAchieveVec(const FirstAchievementMap& firstAchMap, UInt64 eventType);

		void addDailyTaskVec(DailyTaskTempMap* dailyTaskMap, UInt64 eventType);

        void loadPlayerDataFromMemery(const ConnId& connId, Player* player);

	public:
		//测试强制清除内存玩家功能，以后屏蔽此接口 其他人不要使用
		//王戊辰
		void debugTestDestroyPlayer(const Guid& guid);

		void closeConnection(Player* player);

		void insertCreateSet(String name);

		bool isInCreateSet(String name);

		void eraseCreateSet(String name);

		void setLoginTime(UInt64 timemill);

		UInt64 getLoinTime();

		


	//本地数据转化格式，客户端能识别的string类型的数据结构
	private:

	private:
		void onClientConnectionClosed(const Event& ev);
		void onClientConnectionNotExist(const Event& ev);

		//检查每日清理数据
	private:
		//检查每日清理数据，如果不是同一天，那么进行数据清理
		//是同一天，返回false
		//不是同一天，返回true，用这个结果判断是否给客户端发送信息
		bool checkReset(UInt64 time, Player * player);

	private:
       
        ServerId mServerId;
		Map<ConnId, Player*> mPlayerConnectionMap;
		Map<Guid, Player*> mPlayerGuidMap;
		StringMap<Player*> mPlayerNameMap;
        Map<Guid, Player*> mAccountGuidMap;
        StringMap<Player*> mAccountNameMap;
        UInt64 mLastSyncGuidBaseTime;

		UInt64 mLastRestGuidBaseTime;

		UInt64 mLastCheckOnlineWelfareTime;

		UInt64 mLastGetOnlineNumTime;

		UInt64 mForbidLogin;
		
		Guid mPlayerGuidBase;
		//sec 添加全局唯一的ItemUid
		Guid mItemGuidBase;

		Guid mLansquenetGuid;

		Guid mGuildGuid;

		Guid mEmailGuid;

		Guid mMaterialGuid;

		Guid mCharactorGuid;

		Guid mConsortGuid;

		Guid mTicketGuid;

		//玩家基本信息共享，仅用于读取，不可存盘 wwc
		Map<UInt64, BaseInfoData> mPlayerBaseInfoMap;
		//玩家公会信息共享，仅用于读取，不可存盘 wwc
		Map<UInt64, ConsortInfoData> mPlayerConsortInfoMap;

		UInt32 mDailyResetFlag ;

		List<GMMsg> mGMMsgList;

		Set<String> mCreatePlayerNameSet;

		Vector<AchieveData> mAllAchiveInit;

		//for test
		UInt64 gLoginTime;

		Vector<DailyTaskData> mAllDailyTaskInit;
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_LOGIC_SYSTEM_H__

