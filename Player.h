#ifndef __LYNX_GAME_SERVER_GAME_OBJECT_PLAYER_H__
#define __LYNX_GAME_SERVER_GAME_OBJECT_PLAYER_H__

#include "NetworkSystem.h"

#include "ItemManager.h"
#include "PlayerInfoManager.h"
#include "SkillManager.h"
#include "BuffManager.h"
#include "StageManager.h"
#include "EquipManager.h"
#include "FashionManager.h"
#include "FireConfirm/FireConfirmManager.h"
#include "PersistManager.h"
#include "RhymeManager.h"
#include "HoarStoneManager.h"
#include "HeroEquipManager.h"
#include "MaterialManager.h"
#include "GemManager.h"
#include "JewelryManager.h"
#include "AllItemManager.h"
#include "ServantManager.h"
#include "ChatManager.h"
#include "DetailInfoManager.h"
#include "FriendBlackManager.h"
#include "GMManager.h"
#include "EmailManager.h"
#include "CharactorManager.h"
#include "AchieveManager.h"
#include "ConsortManager.h"

#include "PlayerAttrData.h"
#include "../Ranking/RankingPersonal.h"

#include "../FireConfirm/Box.h"
namespace Lynx
{
	class Player : public MainThread
	{
	public:
		Player();
		virtual ~Player();

      
        friend class ItemManager;
        friend class PlayerInfoManager;
		friend class SkillManager;
		friend class BuffManager;
		friend class StageManager;
		friend class EquipManager;
		friend class FashionManager;
		friend class FireConfirmManager;
		friend class PersistManager;
		friend class RhymeManager;
		friend class HoarStoneManager;
		friend class HeroEquipManager;
		friend class MaterialManager;
		friend class GemManager;
		friend class JewelryManager;
		friend class AllItemManager;
		friend class ServantManager;
		friend class ChatManager;
	    friend class DetailInfoManager;
		friend class GMManager;
		friend class RankingPersonalManager;
		friend class FriendBlackManager;
		friend class EmailManager;
		friend class AchieveManager;
		friend class CharactorManager;
		friend class ConsortManager;

		bool operator < (const Player & player)
		{
			UInt64 otherUid = player.getPlayerGuid();
			return mPlayerData.mBaseData.m_nPlayerID < otherUid;

		}


		bool initial(const ConnId& connId, const PlayerData& playerData, 
             const UInt64& loginToken);
		void release();
		void update(const UInt64& accTime);
        
        void onClientConnectClosed();
        void onClientConnectReconnected(const ConnId& connId, const UInt64& loginToken, bool isLogin);
        const UInt64& getLoginToken() const { return mLoginToken; }

		const ConnId& getConnId() const { return mConnId; }
	

		const Guid& getPlayerGuid() const { return mPlayerData.mBaseData.m_nPlayerID; }
		const std::string& getPlayerName() const { return mPlayerData.mBaseData.m_strPlayerName; }
        const UInt32 getPlayerLeval() const { return mPlayerData.mBaseData.m_nLevel; }
		void setPlayerLeval(UInt32 num)  {  mPlayerData.mBaseData.m_nLevel = num ; }

		 const UInt64 getPlayerAccountID() const { return mPlayerData.mBaseData.m_accountID; }

        const UInt32 getPlayerExp() const { return mPlayerData.mBaseData.m_nLevelExp; }
		void setPlayerExp(UInt32 num)  {  mPlayerData.mBaseData.m_nLevelExp = num ; }


		

		const std::string& getPlayerTitle() const { return mPlayerData.mBaseData.m_strPlayerTitle; }
		void setPlayerTitle(std::string& num)  {  mPlayerData.mBaseData.m_strPlayerTitle = num ; }

		const UInt32 getVipLevel() const { return mPlayerData.mBaseData.m_nVipLevel; }
// 		void setVipLevel(UInt32 num)  {  mPlayerData.mBaseData.m_nVipLevel = num ; }

		 const UInt32 getPlayerVIPExp() const { return mPlayerData.mBaseData.m_nVipExp; }

		const PlayerChapterList getChapterList() const { return mPlayerData.mChapterListData; }
		void setchapterList(PlayerChapterList num)  {  mPlayerData.mChapterListData = num ; }

		const UInt32 getSceneID() const { return mPlayerData.mChapterListData.m_SceneID; }
		void setSceneID(UInt32 num)  {  mPlayerData.mChapterListData.m_SceneID = num ; }

		const UInt32 getRoomID() const { return mPlayerData.mChapterListData.m_RoomID; }
		void setRoomID(UInt32 num)  {  mPlayerData.mChapterListData.m_RoomID = num ; }


		const PlayerShopData &getPlayerShopData() const { return mPlayerData.mPlayerShopData; }
		void setPlayerShopData(PlayerShopData num)  {  mPlayerData.mPlayerShopData = num ; }

		const PlayerOnlineDay &getPlayerOnlineDay() const { return mPlayerData.mPlayerOnlineDay; }
		void setPlayerOnlineDay(PlayerOnlineDay num)  {  mPlayerData.mPlayerOnlineDay = num ; }

		

		const PlayerJewelryData &getJewelryData() const { return mPlayerData.mJewelryData; }
		void setJewelryData(PlayerJewelryData num)  {  mPlayerData.mJewelryData = num ; }

		
		 List<KeyValue> getChapterUnlocked()  { return mPlayerData.mChapterListData.m_ChapterUnlocked; }
		 void setChapterUnlocked(List<KeyValue> num)  {  mPlayerData.mChapterListData.m_ChapterUnlocked = num ;} 

// 		List<KeyValue> getResetStages()  { return mPlayerData.mChapterListData.m_ResetStages; }
// 		void setResetStages(List<KeyValue> num)  {  mPlayerData.mChapterListData.m_ResetStages = num ; }


		const UInt32 getPlayerModelID() const { return mPlayerData.mBaseData.m_nModelID; }
		void setPlayerModelID(UInt32 num)  {  mPlayerData.mBaseData.m_nModelID = num ; }

        const UInt64 getPlayerCoin() const { return mPlayerData.mBaseData.m_nCoin; }

		//不能直接用它，要用能纪录log的接口
// 		void setPlayerCoin(UInt64 num)  {  mPlayerData.mBaseData.m_nCoin = num ; }

		const UInt64 getPlayerRhyme() const { return mPlayerData.mRhymeData.m_RhymeSoul; }
		void setPlayerRhyme(UInt64 num)  {  mPlayerData.mRhymeData.m_RhymeSoul = num ; }

		const PlayerRhymeData getPlayerRhymeData() const { return mPlayerData.mRhymeData; }
		void setPlayerRhymeData(PlayerRhymeData num)  {  mPlayerData.mRhymeData = num ; }

		const PlayerHoarStoneData getPlayerHoarStoneData() const { return mPlayerData.mHoarStoneData; }
		void setPlayerHoarStoneData(PlayerHoarStoneData num)  {  mPlayerData.mHoarStoneData = num ; }

// 		const UInt32 getPlayerstrength() const { return mPlayerData.mBaseData.m_nStrength; }
// 		void setPlayerstrength(UInt32 num)  {  mPlayerData.mBaseData.m_nStrength = num ; }

		
		const UInt32 getPlayerShengWang() const { return mPlayerData.mBaseData.m_nFame; }
		void setPlayerShengWang(UInt32 num)  {  mPlayerData.mBaseData.m_nFame = num ; }

		const UInt32 getPlayerScore() const { return mPlayerData.mTowerData.m_Score; }
		void setPlayerScore(UInt32 num)  {  mPlayerData.mTowerData.m_Score = num ; }		

		const UInt32 getPlayerPower() const { return mPlayerData.mBaseData.m_nPower; }
		void setPlayerPower(UInt32 num)  {  mPlayerData.mBaseData.m_nPower = num ; }

		const UInt64 getPlayerGold() const { return mPlayerData.mBaseData.m_nGold; }

		//不能用它，用有l能纪录log的接口
// 		void setPlayerGold(UInt64 num)  {  mPlayerData.mBaseData.m_nGold = num ; }

        const UInt64 getConnectCloseTime() const { return mConnectCloseTime; }
        const PlayerData& getPlayerData() const { return mPlayerData; }

		const PlayerBaseData& getPlayerBaseData() const { return mPlayerData.mBaseData; }
		const PlayerConsortData & getPlayerConsortData() const{return mPlayerData.mConsortData;}

		/*const UInt32 & getPlayerPower() const{return mPlayerData.mBaseData.m_nPower;}
		void  setPlayerPower(UInt32 power){mPlayerData.mBaseData.m_nPower = power;}*/
		
		void setPlayerBaseData(PlayerBaseData num)  {  mPlayerData.mBaseData = num ; }

		const PlayerDailyResetData& getPlayerDailyResetData() const { return mPlayerData.mDailyRestData; }
		void setPlayerDailyResetData(PlayerDailyResetData num)  {  mPlayerData.mDailyRestData = num ; }//player->getPersistManager().setDirtyBit(DAILYRESETBIT);
		

		const PlayerFireConfirmData& GetFireConfirmData() const { return mPlayerData.mFireConfirmData; }

		const void SetFireConfirmData(PlayerFireConfirmData value) {  mPlayerData.mFireConfirmData =value; }

		
		PlayerFireConfirmData& GetFireConfirmDataTest()  { return mPlayerData.mFireConfirmData; }


		void SetFireConfirmDataTest(const PlayerFireConfirmData &value) {  mPlayerData.mFireConfirmData =value; }

		const PlayerBuffListData & GetBufferData() const { return mPlayerData.mBuffListData; }

		const void SetBufferData(PlayerBuffListData value) {  mPlayerData.mBuffListData =value; }

		const PlayerSkillListData & GetSkillList() const { return mPlayerData.mSkillListData; }

		const void SetSkillList(PlayerSkillListData value) {  mPlayerData.mSkillListData =value; }

		const PlayerFashionData & GetFashionData() const { return mPlayerData.mFashionData; }

		const void SetFashionData(PlayerFashionData value) {  mPlayerData.mFashionData =value; }

		const PlayerGemData & GetPlayerGemData() const { return mPlayerData.mGemData; }

		const void SetPlayerGemData(PlayerGemData value) {  mPlayerData.mGemData =value; }

		const PlayerEquipData & GetPlayerEquipData() const { return mPlayerData.mEquipData; }

		const void SetPlayerEquipData(PlayerEquipData value) {  mPlayerData.mEquipData =value; }

		const PlayerLocalData getPlayerLocalData() const { return mPlayerData.mLocalData; }
		void setPlayerLocalData(PlayerLocalData num)  {  mPlayerData.mLocalData = num ; }

		const PlayerTowerData getPlayerTowerData() const { return mPlayerData.mTowerData; }
		void setPlayerTowerData(PlayerTowerData num)  {  mPlayerData.mTowerData = num ; }

		const PlayerCourageChallengeData getCourageChallengeData() const { return mPlayerData.mCourageChallengeData; }
		void setCourageChallengeData(PlayerCourageChallengeData num)  {  mPlayerData.mCourageChallengeData = num ; }

		const PlayerInlineActivityData getInlineActivityData() const { return mPlayerData.mInlineActivityData; }
		void setInlineActivityData(PlayerInlineActivityData num)  {  mPlayerData.mInlineActivityData = num ; }
		
		
		const PlayerServantData& GetPlayerServantData() const{return mPlayerData.mServantData; }
		void  setPlayerServantData(PlayerServantData values){mPlayerData.mServantData = values; }

		const PlayerJewelryData& getPlayerJewelryData(){return mPlayerData.mJewelryData;}

		const PlayerChatData& getPlayerChatData(){return  mPlayerData.mChatData;}

		const UInt64 & getPlayerLeaveTime(){return mPlayerData.mBaseData.m_nLeavTime;}

		void setPlayerLeaveTime(const UInt64 leaveTime){mPlayerData.mBaseData.m_nLeavTime = leaveTime;}

// 		const T_Character& GetCharacter() const { return mPlayerData.mCharacter; }
// 
// 		const void SetCharacter(T_Character value) {  mPlayerData.mCharacter =value; }

		

// 		const List<KeyValue>& GetCounter() const { return mPlayerData.mBaseData.m_Counter; }
// 		void setCounter(List<KeyValue> counter)  {  mPlayerData.mBaseData.m_Counter = counter ; }

		const List<KeyValue>& GetBoxCounter() const { return mPlayerData.mCounterData.m_BoxCounter; }
		void setBoxCounter(List<KeyValue> counter)  {  mPlayerData.mCounterData.m_BoxCounter = counter ; }

		const List<KeyValue>& GetCopyCounter() const { return mPlayerData.mCounterData.m_CopyCounter; }
		void setCopyCounter(List<KeyValue> counter)  {  mPlayerData.mCounterData.m_CopyCounter = counter ; }

		const List<UInt32>& GetRewardCounter() const { return mPlayerData.mCounterData.m_RewardCounter; }
		void setRewardCounter(List<UInt32> counter)  {  mPlayerData.mCounterData.m_RewardCounter = counter ; }

		const PlayerCounterData GetPlayerCounterData() const { return mPlayerData.mCounterData; }
		void setPlayerCounterData(PlayerCounterData& counter)  {  mPlayerData.mCounterData = counter ; }

		

		const PlayerStrengthData& getStrengthData() const { return mPlayerData.mStrengthData; }
		void setStrengthData(PlayerStrengthData value)  {  mPlayerData.mStrengthData = value ; }

		const PlayerFoodsData& getFoodsData() const { return mPlayerData.mFoodsData; }
		void setFoodsData(PlayerFoodsData value)  {  mPlayerData.mFoodsData = value ; }

		const PlayerRankGameData& getRankGameData() const { return mPlayerData.mRankGameData; }
		void setRankGameData(PlayerRankGameData value)  {  mPlayerData.mRankGameData = value ; }

		const PlayerBuyCoinData& getBuyCoinData() const { return mPlayerData.mPlayerBuyCoinData; }
		void setBuyCoinData(PlayerBuyCoinData value)  {  mPlayerData.mPlayerBuyCoinData = value ; }

		const RecordsData& getRecordsData() const { return mPlayerData.mRecordsData; }
		void setRecordsData(RecordsData value)  {  mPlayerData.mRecordsData = value ; }

		 
		PlayerDailyTaskData& getDailyTaskData() {return mPlayerData.mDailyTaskData;}

		void setDailyTaskData(PlayerDailyTaskData value) {mPlayerData.mDailyTaskData = value;}

		PlayerDailyActiveData &getDailyActiveData() {return mPlayerData.mDailyAcvData;}

		void setDailyActiveData(PlayerDailyActiveData  value){mPlayerData.mDailyAcvData = value;}
		
// 
// 		const Character& getCharacter() const { return mCharacter; }
// 		void setCharacter(Character value)  {  mCharacter = value ; }

  
        void setPlayerDataDirty() { mPlayerData.mBaseData.mDirtyFlag = true; }
      

        UInt32 getLastChatTime() { return mLastChatTime; }
        void updateLastChatTime();

		 void ResetFireConfirmData() ;
       
        ItemManager& getItemManager() { return mItemManager; }

		EmailManager& getEmailManager(){return mEmailManager;}

		PlayerInfoManager& getPlayerInfoManager(){ return mPlayerInfoManager;}

		SkillManager& getSkillManager(){return mSkillManager;}

		BuffManager& getBuffManager(){return mBuffManager;}
       
		StageManager& getStageManager(){return mStageManager;}

		EquipManager& getEquipManager(){return mEquipManager;}

		FashionManager& getFashionManager(){return mFashionManager;}

		FireConfirmManager& getFireConfirmManager(){ return mFireConfirmManager;}

		PersistManager& getPersistManager(){return mPersistManager;}

		RhymeManager & getRhymeManager(){return mRhymeManager;}

		HoarStoneManager & getHoarStoneManager(){return mHoarStoneManager;}

		HeroEquipManager & getHeroEquipManager(){return mHeroEquipManager;}

		MaterialManager & getMaterialManager(){return mMaterialManager; }

		GemManager & getGemManager(){return mGemManager;}

		JewelryManager & getJewelryManager(){return mJewelryManager;}

		AllItemManager & getAllItemManager(){return mAllItemManager;}
		
		ServantManager & getServantManager(){return mServantManager;}

		CharactorManager & getCharactorManager(){return mCharactorManager;}

		ChatManager & getChatManager(){return mChatManager;}

		AchieveManager & getAchieveManager(){return mAchieveManager;}

		DetailInfoManager &getDetailInfoManager(){return mDetailInfoManager;}

		GMManager & getGMManager(){return mGMManager;}

		RankingPersonalManager & getRankingPersonalManager(){return mRankingPersonalManager;}

		FriendBlackManager & getFriendBlackManager(){return mFriendBlackManager;}

		ConsortManager & getConsortManager(){return mConsortManager;}

		

		//激活条件类型，激活条件值，激活的界石索引
		bool conditionRes(UInt32 conditionType, UInt64 conditionValue1, UInt32 conditionValue2);

		UInt32  getExpAdd(UInt32 count,UInt32 &addStrength);

		UInt32  getVipExpAdd(UInt32 count,UInt32 &change);

		void levelUpcheck(UInt64 playerID);
		
		//计算玩家角色等级裸体属性
		void calNakedAttr();

		//获取 玩家该角色等级的裸体属性
		const PlayerAttrData &getLevelNakedAttr();

		void setDelDirty();

		void clearDelDirty();

		bool isToDel();

    public:
        void checkLvActiveConditon();
		void checkGuidInit();

    public:
       
    public:
		template <typename MsgType >
		void sendMsgWithAck(const MsgType& msg)
		{
			StreamBuffer* ostream = XNEW(StreamBuffer)();

			MsgHdr hdr;
			hdr.mAckId = mAckIndex++;
			hdr.mId = MessageId<MsgType >::sMsgId;
			hdr.mBodyLen = serializeSize(msg, *ostream);
			if (!hdr.serialize(*ostream))
			{
				LOG_WARN("Failed to serialize msg header [%u]", hdr.mId);
				XDELETE(ostream);
				return;
			}
			serialize(msg, *ostream, hdr.mId);
			NetworkSystem::getSingleton().sendMsg(*ostream, mConnId);

			mWaitAckMsgMap.insert(hdr.mAckId, ostream);
		}

		template <typename MsgType >
		void sendMsg(const MsgType& msg)
		{
			NetworkSystem::getSingleton().sendMsg(msg, mConnId);
		}

		void onMsgAckNotify(UInt32 ackId);

		UInt16 getLastPostState();

		void setLastPostState(UInt16 postState);

	private:
		void updatePlayerInfo();

	private:
		ConnId mConnId;
        UInt64 mLoginToken;
        UInt64 mConnectCloseTime;
		PlayerData mPlayerData;
       // PlayerPresentData mPlayerPresentData;
		Map<UInt32, StreamBuffer*> mWaitAckMsgMap;
		UInt32 mAckIndex;
        UInt32 mLastChatTime;

      //管理类
        ItemManager mItemManager;
		PlayerInfoManager mPlayerInfoManager;
		SkillManager mSkillManager;
		BuffManager mBuffManager;
		StageManager mStageManager;
		EquipManager mEquipManager;
		FashionManager mFashionManager;
 		FireConfirmManager mFireConfirmManager;

		PersistManager mPersistManager;
		
		RhymeManager mRhymeManager;

		HoarStoneManager mHoarStoneManager;

		HeroEquipManager mHeroEquipManager; 

		MaterialManager mMaterialManager;

		GemManager mGemManager;

		JewelryManager mJewelryManager;

		AllItemManager mAllItemManager;

		ServantManager mServantManager;

		ChatManager mChatManager;

		DetailInfoManager mDetailInfoManager;

		GMManager mGMManager;

		RankingPersonalManager mRankingPersonalManager;

		FriendBlackManager mFriendBlackManager;

		EmailManager mEmailManager;

		CharactorManager mCharactorManager;

		AchieveManager mAchieveManager;

		ConsortManager mConsortManager;

		//是否投递最后保存的消息，0，未投递，1，已投递
		UInt16	mLastPostState;

		PlayerAttrData m_playerAttrData;

		UInt32 mNeedDel;
        
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_GAME_OBJECT_PLAYER_H__

