#ifndef __LYNX_GAME_SERVER_PERSIST_WORKER_H__
#define __LYNX_GAME_SERVER_PERSIST_WORKER_H__

#include "CommonLib.h"
#include "PersistThreadMsg.h"
#include "DBInterface.h"
#include "PersistWorkerExt.h"

namespace Lynx
{
	struct PlayerData;
	struct HoarStoneSave
	{
		UInt32 mStoneLv;
		UInt32 mPieceCount;
		UInt32 mStoneStar;
		std::string mEquipStr;
	};
	struct ActivityData
	{
		ActivityData():playerID(0),type(0),count(0),key(0),value(0),gotIt(0){}
		Guid playerID;
		UInt32 type;
		UInt32 count;
		UInt32 key;
		UInt32 value;
		UInt32 gotIt;
	};
	class PersistWorker : public ThreadWorker,public Singleton<PersistWorker>
	{
	public:
		PersistWorker();
		virtual ~PersistWorker();

        bool initial(UInt32 index);
        void release();

		UInt16 loadStageRangking();

		UInt16 loadServerData();

		UInt16 loadResetStages();

		void setActivityData(PlayerData &playerData,List<ActivityData>activityDataList);

		void initActivityData(PlayerData &playerData,List<ActivityData>activityDataList);

		

	private:
		void onThreadStarted();
		void onThreadShutdown();
		void onPreUpdate();
		void onPostUpdate();
		void onDispatchMsg(ThreadMsg& threadMsg);

        void onPersistLoadPlayerDataReq(PersistLoadPlayerDataReq& msg);
        void onPersistSyncBaseGuidNotify(PersistSyncBaseGuidNotify& msg);
        void onPersistLoadPlayerGuidReq(PersistLoadPlayerGuidReq& msg);
  
        bool onInitSkillData(UInt64 &playerUid, UInt32 &modelId, UInt64& connId);

		bool onInitHeroEquipData(UInt64 &playerUid, UInt32 &modelId, UInt64& connId);

		bool onInitHeroJewelryData(UInt64 &playerUid, UInt32 &modelId, UInt64& connId);

		bool onInitServantData(UInt64 &playerUid, UInt32 &modelId, UInt64& connId);

		bool onInitFashionData(UInt64 &playerUid, UInt32 &modelId);

		bool onInitCharactorData(UInt64 &playerUid, UInt32 &modelId);

		bool onInitAchieveData(UInt64 &playerUid, UInt32 &modelId);
		bool onInitDailyTaskData(UInt64 &playerUid, UInt32 &modelId);

		bool initAchieveDb(const UInt64 &playerUid, const AchieveData& achdata);

		bool initDailyTaskDb(UInt64 &playerUid, const DailyTaskData & dlydata);
	
		bool updateDailyTaskDb(UInt64 &playerUid, const DailyTaskData & dlydata);

		bool updateAchieveDb(const UInt64 &playerUid, const AchieveData& achdata);

		bool updateAchBarrierDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateAchEliteBarrierDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateRhymeEnhanceDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateActRhymeSkillDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateActHonorstoneDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateActGetServantDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateActPlayerLvDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateActVipLvDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateActPowerDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateAct4StarServantDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateActPurpleServantDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateDlyBarrierCntDb(const UInt64 &playerUid,  const DailyTaskData& dailydata);

		bool updateDlyEliteBarrierCntDb(const UInt64 &playerUid,  const DailyTaskData& dailydata);

		bool updateDlyLingRenCntDb(const UInt64 &playerUid,  const DailyTaskData& dailydata);

		bool updateDlyXibanCntDb(const UInt64 &playerUid,  const DailyTaskData& dailydata);

		bool updateDlyMultiSucCntDb(const UInt64 &playerUid,  const DailyTaskData& dailydata);
		
		bool updateDlyTwelveSucDb(const UInt64 &playerUid,  const DailyTaskData& dailydata);

		bool updateDlyChallengeZhaofuMarket(const UInt64 &playerUid,  const DailyTaskData& dailydata);

		bool updateDlyDoCourage(const UInt64 &playerUid,  const DailyTaskData& dailydata);

		bool updateDlyDoWXChallenge(const UInt64 &playerUid,  const DailyTaskData& dailydata);

		bool updateDlyDoQualify(const UInt64 &playerUid,  const DailyTaskData& dailydata);

		bool updateDlyDoArea(const UInt64 &playerUid,  const DailyTaskData& dailydata);

		bool updateDlyRhymeEnhance(const UInt64 &playerUid,  const DailyTaskData& dailydata);

		bool updateDlyHoarStone(const UInt64 &playerUid,  const DailyTaskData& dailydata);

		bool updateDlyBaiCaiShen(const UInt64 &playerUid,  const DailyTaskData& dailydata);

		bool updateBuyEnergy(const UInt64 &playerUid,  const DailyTaskData& dailydata);

		bool updateAssistUnlockDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateOwnGreenAssisDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateOwnBlueAssisDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateOwnPurpleAssisDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateQualifyDanlvDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateQualifySucCntDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateAreaSucCntDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateBuyCharactorDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateCourageSucDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateWxChallengeDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateUseRhymeSkillDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateUseRhymeSpeedDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateBatCallServantDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateFriendCntDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateLv5GemCntDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateLv10GemCntDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateLv15GemCntDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateLRCallServantDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateXBCallServantDb(const UInt64 &playerUid,  const AchieveData& achdata);

		bool updateMLCallServantDb(const UInt64 &playerUid, const AchieveData& achdata);

		//更新玩家成就到数据库
		void onPersistUpdateAchDb(AchieveUpdateMsg & msg );

		//更新玩家每日信息到数据库
		void onPersistUpdateDailyTaskDb(PersistDailyTaskUpdateMsg & msg);

		//更新玩家日常活跃度表
		void onPersistUpdateDailyActiveDb(PersistDailyActiveUpdateMsg & msg);

		void onPersistUpdateActiveTimeDb(PersistDailyLastTimeUpdateMsg &msg);

		//创建玩家请求
        void onPersistCreatePlayerReq(PersistCreatePlayerReq& msg);

	   //根据玩家账号加载玩家数据
		void onLoadPlayerByAccount(PersistLoadPlayerByAccount & msg);
  
		//玩家被监控数据存盘
		void onPersistUpdataFireConfirmData(PersistFireConfirmSaveReq& msg);
		//玩家属性存盘
		void onPersistUpdateSingleProperty(PersistPlayerSinglePropertySaveReq& msg);
        
		void onPersistManagerSetDirtyReq(PersistManagerSetDirtyReq & msg);

		void onPersistJewelryDelReq(PersistDelJewelryNotify & msg);

		void onPersistJewelryAddReq(PersistAddJewelryNotify & msg);

		void onPerisitJewelryUpdateReq(PersistUpdateJewelryNotify & msg);

		void onPerisitJewelryEquipUpdateReq(PersistUpdateJewelryEquipNotify & msg);

		void onPersistServantAddReq(PersistAddServantNotify & msg);

		void onPersistServantUpdateReq(PersistUpdateServantNotify & msg);

		void onPersistServantDelReq(PersistDelServantNotify & msg);

		void onPersistServantTreasureAddReq(PersistAddServantTreasureNotify & msg);

		void onPersistServantTreasureUpdateReq(PersistUpdateServantTreasureNotify & msg);

		

		void initHeroEquipLock(Vector<HeroEquipData> &heroEquipVec, UInt64 modelId);

		void initJewelryRdAttr(JewelryData& jewelryData,std::string &jewelryStr, std::stringstream &mystream);

		void initNoneEquip(Vector<HeroEquipData> &heroEquipVec);

		void initEffectEquip(Vector<HeroEquipData> &heroEquipVec, UInt64 & equipId);

		void initEquipGemBit(HeroEquipData &heroEquipData,  UInt32& level,UInt32 & condKey, UInt32 gemSlotBit);

		void onPersistNotifyWorkerSaveDbReq(PersistNotifyWorkerSaveDbReq & msg);
    
		void onPersistOffLineSaveReq(PersistOffLineSaveReq& msg);

		bool dealSkillSaveDb(UInt64 playerUid);

		bool dealItemSaveDb(UInt64 playerUid);

		bool dealBaseDataSaveDb(UInt64 playerUid);

		bool dealDailyResetDataSaveDb(UInt64 playerUid);

		bool dealGemDataSaveDb(UInt64 playerUid);

		bool dealServantCostSaveDb(UInt64 playerUid);

		bool dealServantBattleSaveDb(UInt64 playerUid);

		bool dealChatSaveDb(UInt64 playerUid);

		void insertItemToDb(List<ItemData>::Iter* iter, bool &exeFailed,UInt64 playerUid);

		void updateItemToDb(List<ItemData>::Iter* iter, bool &exeFailed,UInt64 playerUid);

		bool delItemToDb(List<ItemData>::Iter* iter, bool &exeFailed,UInt64 playerUid);

		bool dealRhymeSaveDb(UInt64 playerUid);

		bool dealHoarStoneSaveDb(UInt64 playerUid);

		bool dealMaterialSaveDb(UInt64 playerUid);

		bool dealHeroEquipSaveDb(UInt64 playerUid);

		void updateMaterialToDb(List<MaterialData>::Iter* iter, bool &exeFailed,UInt64 playerUid);

		void updateGemToDb(List<GemData>::Iter* iter, bool &exeFailed,UInt64 playerUid);

		//以下为 load playerData 

		//加载玩家基础信息
		UInt16 loadPlayerBaseData(PlayerData& playerData,UInt64 playerUid);
		//加载玩家日常恢复信息
		UInt16 loadPlayerDailyResetData(PlayerData& playerData,UInt64 playerUid);

		//加载技能列表
		UInt16 loadPlayerSkillList(PlayerData& playerData,UInt64 playerUid);
		//加载buff列表
		UInt16 loadPlayerBuffList(PlayerData& playerData,UInt64 playerUid);
		//加载时装列表
		UInt16 loadPlayerFashionList(PlayerData& playerData,UInt64 playerUid);
		//加载物品列表
		UInt16 loadPlayerItemList(PlayerData& playerData,UInt64 playerUid);

		UInt16 loadPlayerStageData(PlayerData& playerData,UInt64 playerUid);

		//加载界石列表
		UInt16 loadPlayerHoarStoneData(PlayerData& playerData,UInt64 playerUid);


		//加载玩家韵文系统
		UInt16 loadPlayerRhymeData(PlayerData& playerData, UInt64 playerUid);
		
		//加载监控等级
		UInt16 loadPlayerFireConfirmData(PlayerData& playerData,UInt64 playerUid);

		UInt16 loadPlayerEquipData(PlayerData& playerData,UInt64 playerUid);

		UInt16 loadPlayerMaterialData(PlayerData& playerData,UInt64 playerUid);

		UInt16 loadPlayerGemData(PlayerData& playerData,UInt64 playerUid);

		UInt16 loadPlayerJewelryData(PlayerData& playerData,UInt64 playerUid);

		UInt16 loadPlayerJewelryEquipData(PlayerData& playerData,UInt64 playerUid);

		UInt16 loadPlayerServantData(PlayerData& playerData,UInt64 playerUid);

		UInt16 loadServantFood(PlayerData& playerData,UInt64 playerUid);

		UInt16 loadServantTreasure(PlayerData& playerData,UInt64 playerUid);

		UInt16 loadServantList(PlayerData& playerData,UInt64 playerUid);

		UInt16 loadServantBattleData(PlayerData& playerData,UInt64 playerUid);

		UInt16 loadChatData(PlayerData& playerData,UInt64 playerUid);

		UInt16 loadFriendData(PlayerData& playerData,UInt64 playerUid);

		UInt16 loadEmailData(PlayerData& playerData, UInt64 playerUid);

		UInt16 loadCharactorData(PlayerData& playerData, UInt64 playerUid);

		UInt16 loadAchieveData(PlayerData& playerData, UInt64 playerUid);

		UInt16 loadDailyTaskData(PlayerData& playerData, UInt64 playerUid);

		UInt16 loadDailyActiveData(PlayerData& playerData, UInt64 playerUid);

		//加载计数器
		UInt16 loadCounter(PlayerData& playerData,UInt64 playerUid);

		//加载副本计数器
		UInt16 loadCopyCounter(PlayerData& playerData,UInt64 playerUid);

		//回馈计数器
		UInt16 loadRewardCounter(PlayerData& playerData,UInt64 playerGuid);

		//回馈计数器
		UInt16 loadFoods(PlayerData& playerData,UInt64 playerGuid);

		UInt16 loadLottery(PlayerData& playerData,UInt64 playerGuid);

		UInt16 loadStrength(PlayerData& playerData,UInt64 playerGuid);


		UInt16 loadShop(PlayerData& playerData,UInt64 playerGuid);

		UInt16 loadShopItem(PlayerData& playerData,UInt64 playerGuid);


		UInt16 loadCatCoin(PlayerData& playerData,UInt64 playerGuid);
		UInt16 loadCoinGroup(PlayerData& playerData,UInt64 playerGuid);

		UInt16 loadCourageChallengeData(PlayerData& playerData,UInt64 playerGuid);


		UInt16 loadPlayerChapterData(PlayerData& playerData,UInt64 playerGuid);

		UInt16 loadPlayerLocalData(PlayerData& playerData,UInt64 playerGuid);
		
		UInt32 getAttrQuality( const RandomAttribute &randomAttr, const ItemData & itemData);

		UInt32 getPAQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr );

		UInt32 getMAQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr );

		UInt32 getPFQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getMFQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getMaxHPQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getMaxMPQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getMaxSPQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getCritRateQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getCirtDamageQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);

		UInt32 getBaseToughnessQuality(const UInt32 &equipLv, const RandomAttribute &randomAttr);
		
		//更新关卡信息
		bool updateStageData(UInt64 playerUid,UInt32 chapterID,UInt32 stageID);

		bool updateRewardCounterData(UInt64 playerUid,UInt32 chapterID,UInt32 stageID);

		UInt16 loadPersonalRangking(PlayerData &playerData,UInt64 playerUid);

		UInt16 loadTowerData(PlayerData &playerData,UInt64 playerUid);

		UInt16 loadInlineActivityList(PlayerData &playerData,UInt64 playerUid);

		UInt16 loadSevenDayTask(PlayerData &playerData,UInt64 playerUid);

		UInt16 loadInlineActivity(PlayerData &playerData,UInt64 playerUid);
		
		void onPersistUpdateStageRanking(PersistRankingSaveReq& msg );

		void onCatCoinSaveReq(CatCoinSaveReq& msg );
		void onPersistUpdateShopItem(PersistShopItemSaveReq& msg );

		void onPersistUpdateShop(PersistShopSaveReq& msg );
		void onCoinGroupSaveReq(CoinGroupSaveReq& msg );

		void onPersistLoadOfflineData( PersistOfflineDataReq &msg);

		void onPersistFindSimilarPower( PersistFindSimilarPowerReq &msg);

		void onPersistServerDataReq( PersistServerDataReq &msg);

		void onPersistRobotDataReq( PersistRobotDataReq &msg);

		void onPersistInlineActivityPower( PersistInlineActivityReq &msg);

		void onPersistFriendOffLineUpdate(PersistUpdateFriendOffLineNotify & msg);

		void onPersistFriendOffLineAdd(PersistAddFriendOffLineNotify & msg);

		void onPersistFriendOffLineDel(PersistDelFriendOffLineNotify &msg);

		void onPersistLeaveTime(PersistUpdateLeaveTime &msg);

		void onPersistFriendUpdate(PersistUpdateFriendNotify & msg);

		void onLoadPlayerOnlineByLv(PerisistUpdateOnlineLvRank & msg);

		void onLoadPlayerPowerRank(PersistGetPowerRank & msg);

		void updateAchOfflineFriendCnt(OffLineFriendUpdateMsg& msg);

		bool updateLocalDataToDb(UInt64 playerUid);

		bool updateBoxCounterToDb(UInt64 playerUid);

		bool updateChapterDataToDb(UInt64 playerUid);

		bool updateTowerDataToDb(UInt64 playerUid);

		bool updateStageDataToDb(UInt64 playerUid);

		bool updateFoodDataToDb(UInt64 playerUid);

		bool updateLotteryDataToDb(UInt64 playerUid);

		bool updateCourageChallengeDataToDb(UInt64 playerUid);

		bool updateStrengthDataToDb(UInt64 playerUid);

		bool updateResetStagesToDb();

		UInt16 deleteResetStages();
		
		void  onAddEmail(PersistAddEmail &msg);

		void onDelEmail(PersistDelEmail &msg);

		void onUpdateEmail(PersistUpdateEmail &msg);

		void onClearEmail(PersistClearEmail &msg);

		void onFashionAdd(FashionAdd &msg);

		void loadAllPlayerTest();

		void onCharactorInsert(CharactorAdd & msg);

		void onCharactorUpdate(CharactorUpdate & msg);

    public:
        void textDb();

	private:
		ThreadMsgHandler mThreadMsgHandler;
        DBInterface mDBInterface;
	};
} // namespace Lynx

#endif // __LYNX_GAME_SERVER_PERSIST_WORKER_H__

