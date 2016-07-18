#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_ACHIEVE_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_ACHIEVE_MAMAGER_H__


#include "MainThread.h"
#include "FireConfirm/InlineActivity.h"


namespace Lynx
{
	class Player;

	class AchieveManager : public MainThread,public Singleton<AchieveManager>
	{
	public:
		AchieveManager();
		virtual ~AchieveManager();

		bool initial(Player* player);

		void release();

		bool acvArgMatch(UInt64 achieveId);

		AchieveData * getSameEventAcvData(UInt64 achieveId);

		std::string convertAchieveDataToJson();

		void updateAchieveData(UInt64 eventType,   UInt32 change);

		void updateDailyTaskData(UInt64 eventType,   UInt32 change);

		void dealAddDailyReq(UInt64 dailytaskId);

		void finishDailyTask(UInt64 dailytaskId);

		void getActiveAward(UInt64 dailytaskId);
		
		bool finishAchieve(UInt64 eventType, UInt64 achId);

		void postUpdateAchMsg(UInt64 playerUid,  const AchieveData & achData);

		void postUpdateDailyMsg(UInt64 playerUid,  const DailyTaskData & dailyTaskData);

		bool dealAchPlayerLv(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		bool dealAchBarrier(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		bool dealAchEliteBarrier(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		bool dealAchRhymeEnhance(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		bool dealAchHonStoneActive(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		bool dealgetAchServant(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		bool dealAchVipLv(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		bool dealAchPower(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		bool dealAch4StarServant(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		bool dealAchPurpleServant(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		bool dealAchAssistOpen(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		bool dealAchPurpleAssist(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		bool dealAchQualifySucCnt(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		bool dealAchAreaSucCnt(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		bool dealAchBuyCharactorCnt(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		bool dealCourageSucCnt(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		bool dealWXChallengeCnt(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		bool dealAchFriendCnt(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		bool dealAchGemMaxLv(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		

		bool dealAchLingRenCall(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		bool dealAchXibanCall(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		bool dealAchMingLingCall(UInt64 eventType, UInt64 achId, AchieveData * pAchData);

		void getContant(const List<AllItemEle> &contantList);

		void sendResp2Client(UInt64 oldAchId, UInt64 newAchId,  UInt64 event, UInt32 arg, UInt32 flag,
			const List<AllItemEle> &contantList);

		void sendDlyResp2Client(UInt64 dlyId , UInt64 event, UInt32 arg, UInt32 flag,
			const List<AllItemEle> &contantList);

		void sendDlyActiveResp(UInt32 index, const List<AllItemEle> &contantList);

	private:
		List<AchieveData>  * m_pAchieveList;
		List<DailyTaskData> * m_pDailyTaskList;
		Map<UInt64,  AchieveData *> m_achieveMap;
		Map<UInt64, DailyTaskData*> m_dailyTaskMap;
		Map<UInt64, DailyTaskData *> m_dlyEventMap; 
		Player* m_pPlayer;
		UInt64 m_nPlayerUid;
		PlayerDailyActiveData * m_pdailyActiveData; 
		

	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_ACHIEVE_MAMAGER_H__