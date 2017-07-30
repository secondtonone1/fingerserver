#ifndef __LYNX_GAME_SERVER_CONSORTSYSTEM_H__
#define __LYNX_GAME_SERVER_CONSORTSYSTEM_H__

#include "../MainThread.h"
#include "../Guild.h"
#include "../DBInterface.h"
#include "../CommonLib/ConsortData.h"
//王戊辰
namespace Lynx
{
	class ConsortInfoData;
	class FriendBlackManager;
	class FriendBlackInfo;
	class ConsortSystem : public MainThread, public Singleton < ConsortSystem >
	{
	public:

		ConsortSystem();
		virtual ~ConsortSystem();

		bool initial();
		void release();

		void update(const UInt64& accTime);
	
		ConsortData * getConsortDataById(UInt64 consortId);

		void destroyConsort(UInt64 consortId);
		
		void updateConsortRankList();

		const List<ConsortRankData > &getConsortRankList();

		const List<ConsortData >& getConsortList();

		ConsortRankData * getConsortRankData(UInt64 consortId);

		Map<UInt64, ConsortData *>  *getConsortMap(void);

		ConsortData * createConsort(std::string name, Player *  player);
	
		//判断同名公会
		bool judgeSameNameConsort(std::string name);

		UInt32 getConsortLastRank();

		void sendConsortMsg(UInt32 tempId, UInt64 consortId, UInt64 playerUid, std::string  playerName,  std::string param1,
			std::string param2, UInt64 logTime);
	
		//根据公会id清除某个玩家的申请
		bool eraseApplyFromCheckList(UInt64 consortId, UInt64 applyUid);

		const DBInterface & getDBInterface();
	
		//辅助聊天列表
		void  chatListJson(Json::Value & root, UInt64 playeruid, UInt64 lastchattime);

		//从DB获取base信息
		bool getBaseInfoFromDb(UInt64 playerUid, BaseInfoData & baseInfoData);

		//从DB获取其他玩家信息
		bool getOtherFriendInfo(UInt64 otherUid, FriendBlackInfo & friendblackInfo);

		bool getOtherBeApplyGuidList(UInt64 otherUid, List<UInt64> &friendBeApplyGuidList);

		void saveTicketFriends(UInt64 consortid, UInt32 ticketid);

		void saveTicketGivens(UInt64 consortid, UInt32 ticketid);

		UInt64 getAwardActiveTime();

		//票有集会生成题库并存盘
		void ticketQuestionInit(UInt64 consortId);

		UInt64 getWoodBeginTime();

		UInt64 getWoodEndTime();

		void clearWoodBattle(UInt64 consortId, UInt64 playeruid);

	private:

		//第二天重置一部分活动，根据不同的时间重置不同的
		////curTime为秒级别
		void resetRefresh(UInt64 curTime);

	//刷新一部分半天需求
		void resetRefreshHalfDay(UInt64 curTime);

	//重置票友集会
		void resetticketfriend(UInt64 curTime);

		//计算票友集会有效节点时间戳
		void calculateTicketActive(time_t curTime);

		//更新玩家内存数据，并发送刷新通知，此接口用于半天类型刷新
		void memNotifyHalfDay();

		//票友集会
		void ticketRefresh();

		


		//生成1~10级公会票友集会题库
		void generateTicketQuesions();
		
		void generateTicketQuestion(UInt32 lv);

		List<UInt32>* getTicketQuestion(UInt32 lv);
	
		void convertTicketStr2List(List<TicketFriend>* ticketList, std::string ticketStr);

		void convertGivenStr2List(List<GivenData>* givenList, std::string givenStr);

		std::string convertTicketList2Str(List<TicketFriend>* ticketList);

		std::string converGivenList2Str(List<GivenData>* givenList);

		void calculateWoodBegin(time_t curtime);

	

        //定时器上次打点时间,毫秒级
		UInt64 m_nLastSyncTime;
		UInt64 m_nLastRefreshTime;
		DBInterface mDBInterface;
		//每日重置签到活跃度的时间戳 ,秒级别
		UInt64 m_nActiveResetTime;

		//每12个小时行商猫商店刷新的时间戳，秒级别
		UInt64 m_nBusinessCatResetTime;

		//每日眼力训练重置时间戳，秒级别
		UInt64 m_nEyeSightResetTime;

		//票友集会题库重置时间戳，秒级别
		UInt64 m_nTicketFriendTime;

		//票友集会重置票友集会次数的时间戳
		UInt64 m_nTicketTimesTime;

		//公户行侠仗义次数重置时间戳
		UInt64 m_nLoyalTime;

		//公会厨房特训次数重置时间戳
		UInt64 m_nKitchenTime;

		//工会口才训练重置时间戳
		UInt64 m_nEloquenceTime;

		//木猫阵法重置时间戳
		UInt64 m_nWoodCatTime;


		//内存中维护的consort map，用于关联consortList中数据
		Map<UInt64, ConsortData *> m_consortMap;
		//consortList数据，内存中进行变更，用于更新数据库和计算
		List<ConsortData > m_consortList;
		//定时器从数据库中load consort数据，进行排序后的list ，
		//仅用于发送给客户端排行信息
		List<ConsortRankData > m_consortRankList;
		//关联数据，仅用于发送给客户端排行或单独查找，不用于数据库更新
		Map<UInt64, ConsortRankData *> m_consortRankMap;

		Map<UInt32, List<UInt32> >m_ticketfriendMap; 

		//当前票友集会结束时间戳或当前票友集会结束，下个票友集会结束时间戳
		UInt64 m_nTicketAcitve;

		//木猫阵法开始时间戳
		UInt64 m_nWoodCatBegin;
		//木猫阵法结束时间戳
		UInt64 m_nWoodCatEnd;
		
	
	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_RANKSYSTEM_H__

