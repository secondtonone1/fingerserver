#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_CONSORT_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_CONSORT_MAMAGER_H__


#include "MainThread.h"


namespace Lynx
{
	class Player;
	struct RandomTask
	{
		RandomTask():taskId(0), tag(0), rate(0){}
		UInt32 taskId;
		UInt32 tag;
		UInt32 rate;
	};
	
	class ConsortManager : public MainThread ,public Singleton<ConsortManager>
	{
	public:
		ConsortManager();
		virtual ~ConsortManager();

		bool initial(Player* player);
		void release();

		std::string convertDataToJson();
		
		void syncRankConsortList();

		void syncConsortList();

		void findConsortById(UInt64 consortId);

		void quickJoinConsort();

		void joinConsort(UInt64 consortId);

		void createConsort(std::string name);

		void consortMemberSync(UInt64 consortId);

		void consortDetailSync(UInt64 consortId);

		void checkApply(UInt64 consortId);

		void dealApply(UInt64 consortId, UInt64 applyUid, UInt32 accept);

		void eraseApply(UInt64 consortId);

		bool judgeApplyExist(ConsortData * pConsortData, UInt64 applyUid);

		void dealApplyClear(UInt64 consortId);

		void setConsortInfo(UInt64 consortId, UInt32 needCheck, UInt64 powerlimit);

		void setDescs(UInt64 consortId, std::string descs);

		void kickMember(UInt64 consortId, UInt64 kickId);

		void destroyConsort(UInt64 consortId);

		void leaveConsort(UInt64 consortId);

		void updateConsortPower(UInt64 oldPower, UInt64 newPower);

		void setPlayerConsortInfo(UInt64 consortId, UInt32 consortJob = 0);

		void setViceLeader(UInt64 otherUid, UInt32 setUp);

		void getWoodCatData();


		PlayerConsortData * getPlayerConsortData();

		void addConsortLog(ConsortData * pConsortData, UInt32 tempId, UInt64 consortId, UInt64 playerUid, std::string  playerName,  std::string param1,
			std::string param2, UInt64 logTime);

		void logListSync(UInt64 time);

		//签到接口
		void consortSign(UInt32 signType);

		void consortSignAward(UInt32 awardIndex);

		void initialSignStruct();

		void notifyOtherOnlineConcortMember(UInt32 msgtype, UInt64 selfuid,  ConsortData * pConsortData, UInt32 jobflag = 0, UInt64 notuid = 0);
		//行侠仗义
		void getCatTask(UInt32 taskId);

		void getCatTaskAward(UInt32 taskId);
		
		void refreshCatTask();

		void beginKitchenQue();

		void endKitchenQue(UInt32 questionId, UInt32 score,UInt32 systemID);

		void businessCatTime();

		void businessCatReset();

		void businessCatBuy(UInt32 index);

		void updateGuidStr(std::string guidStr);

		void generateEyeQue();

		void getEyeAward(String  codeStr, UInt32 score);

		void ticketJoin(UInt32 ticketId, UInt32 sceneIndex,UInt64 servantId);

		void ticketSupport(UInt32 ticketId, UInt32 grade);

		void ticketGetAward();

		float getAwardOverride(UInt32 ticketId, UInt32 peaplecount);

		Map<UInt32, TicketAward *> * getAwardMap();

		void convertTicketDataToJson(Json::Value & root,  UInt32 ticketId);

		void getTicketAwardList(List<ReturnItemEle> &rtItemList, UInt64 awardId, float overrider);

		void getTicketData();

		void getTicketOneData(UInt32 ticketId);

		void beginEloquence();

		void getEloquenceAward(UInt32 win);

		void getWoodData();

		void beginWoodCat(UInt32 woodIndex);

		void endWoodCat(UInt32 woodIndex, UInt32 res);

		void awardJson(Json::Value & root,  UInt64 awardId);

		void enhanceWood(UInt32 type);

		void woodTotalAward();		

		void clearWoodBattle();

		void clearTicketAward();

	private:
				
		        void convertWoodData(Json::Value &root);

				void initCatTask();

				void initBusinessCat();

				//公会活动游戏等初始化，用于玩家加入公会或者被批准加入
				//playerInit表示要处理的玩家指针，如果玩家不在内存，那么仅需存盘 uidInit表示要处理的玩家uid
				void gameActiveInit(Player *  playerInit,  UInt64 uidInit);



			//生成行侠仗义任务，从任务池中根据rate返回指定个数的任务队列
			bool  generateCatTask(UInt32 taskCount, List<UInt32>& listtask);

			void clearApplyData(PlayerConsortData * playerConsortData);
			
			Player* m_pPlayer;
			PlayerConsortData * m_pPlayerConsortData;
			UInt32 m_nIndex;

			void clearApply(UInt64 playerUid);

		

			void clearPlayerConsortInfo();

			bool judgeActiveMatch(UInt32 awardIndex,  ConsortData * pConsortData, ConsortLvTemplate * signlvTemp);

			void  getSingAward(UInt32 awardIndex,  ConsortLvTemplate * signlvTemp ,UInt32 systemID);


			String m_strCode;
			UInt32 m_nEyeGroup;

			List<TicketAward> *  m_pTicketAwardList;
			Map<UInt32, TicketAward *> m_ticketAwardMap;
			
	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_CONSORT_MAMAGER_H__