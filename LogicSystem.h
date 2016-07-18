#ifndef __LYNX_GAME_SERVER_LOGIC_SYSTEM_H__
#define __LYNX_GAME_SERVER_LOGIC_SYSTEM_H__

#include "MainThread.h"
#include "Player.h"

#include "Guild.h"

namespace Lynx
{
	
	class LogicSystem : public MainThread, public Singleton<LogicSystem>
	{
	public:
        LogicSystem();
		virtual ~LogicSystem();

		bool initial();
		void release();
		void update(const UInt64& accTime);

		void  dailyRefresh(const UInt64& accTime);

        bool reloadTable(const String& tableName);
        bool reloadAllTable();

        void setPlayerGuid(const Guid& guidBase) { mPlayerGuidBase = guidBase; }

		const Guid& getPlayerGuid(){return mPlayerGuidBase;}
 
		//新生成itemGuid
        Guid generateItemGuid() {++mItemGuidBase;
			baseUidPersist();
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
		baseUidPersist();
		return mMaterialGuid; }

		void setMaterialGuid(const Guid& materialGuid)
		{
			mMaterialGuid = materialGuid;
		}

		//生成角色id，以及日常任务id
		Guid generateCharactorGuid(){++mCharactorGuid;
		baseUidPersist();
		return mCharactorGuid;}

		void setCharactorGuid(const Guid& charactorGuid)
		{
			mCharactorGuid = charactorGuid;
		}


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

		void updateDailyResetData(Player *player,UInt32 flag);

		void checkPlayerDataFromDB(Player& player);

		bool combinSendData(Guid playerID,UInt32 resType,Json::Value jvalue);

		bool combinSendDataTest(Guid playerID,UInt32 resType,const Json::Value &jvalue);

	public:
		//玩家信息
		std::string getDetailInfo(std::string playerUidStr); 

		//获取玩家概要信息
		bool getBaseInfo(UInt64 playerUid, BaseInfoData & baseInfoData);

		//获取baseinfomap
		const Map<UInt64, BaseInfoData> &getBaseInfoMap();

		void insertBaseInfo(const BaseInfoData & baseInfoData);
		//登陆或下线时更新玩家信息
		void updateBaseInfo(const PlayerBaseData &baseData);

		void updateBaseInfo(const BaseInfoData & baseInfoData);

		Guid listRandOne(List<Guid> playerIDList);

			
		void dealSimililarPower(Guid playerID,Guid otherPlayerID,UInt32 times);

		void kickOutAllPlayer();

	 public:
	
	public:
		Set<Player >   getPlayerLvSet(Player * selfPlayer);
		

    private:
        void loadPlayerDataFromMemery(const ConnId& connId, Player* player);

	public:
		//测试强制清除内存玩家功能，以后屏蔽此接口 其他人不要使用
		//王戊辰
		void debugTestDestroyPlayer(const Guid& guid);

		void closeConnection(Player* player);

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
		
		Guid mPlayerGuidBase;
		//sec 添加全局唯一的ItemUid
		Guid mItemGuidBase;

		Guid mLansquenetGuid;

		Guid mGuildGuid;

		Guid mEmailGuid;

		Guid mMaterialGuid;

		Guid mCharactorGuid;

		Map<UInt64, BaseInfoData> mPlayerBaseInfoMap;

		UInt32 mDailyResetFlag ;
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_LOGIC_SYSTEM_H__

