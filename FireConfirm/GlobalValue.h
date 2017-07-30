#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_GLOBLEVALUE_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_GLOBLEVALUE_MANAGER_H__
#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "../CommonLib/CommonLib.h"
#include "../PlatformLib/PlatformLib.h"




namespace Lynx
{

	struct GlobalValue
	{
		UInt32 id;
		UInt32 typeID;
		List<UInt32> ids;

		//meishiwu

		UInt32 uMSWweight1;
		UInt32 uMSWweight2;
		UInt32 uMSWweight3;
		UInt32 uMSWmax;
		UInt32 uMSWrecover;
		UInt32 uMSWeachBuy;
		UInt32 uMSWprice;
		UInt32 uMSWtime;
		UInt32 uMSWrate1;
		UInt32 uMSWrate2;
		UInt32 uMSWrate3;

		//saodang
		UInt32 uSDrate;
		UInt32 uSDsweeptimes;
		UInt32 uSDsweepfreetimes;
		UInt32 uSDamount;
		List<UInt32> uSDprices;
		//specialrelive		
		UInt32 uSRLdayfreetimes;
		UInt32 uSRLmaxtimes;
		List<UInt32>uSRLcosts;
		
		//trail
		UInt32 uTRIALratecosts;
		UInt32 uTRIALFriendRate;
		UInt32 uTRIALFastRate;
		UInt32 uTRIALGainsRate;
		UInt32 uTRIALVipNeed;




		//twelvepalace
		UInt32 uTPdailyreset;
		UInt32 uTPneeditemid;
		UInt32 uTPcanresetitem;
		UInt32 uTPresetlowerlimit;
		UInt32 uTPbuycost;
		UInt32 uTPbuybosscost;
		UInt32 uTPhornbuycost;
		UInt32 uTPeachdaybuytimes;
		

		//climbtower
		UInt32 uCLIMBTdailyreset;
		UInt32 uCLIMBTneeditemid;
		UInt32 uCLIMBTcanresetitem;
		UInt32 uCLIMBTresetlowerlimit;
		UInt32 uCLIMBTbuycost;


		//strength
		UInt32 uSTmax;
		UInt32 uSTtime;
		List<UInt32> uSTprices;
		//mysticalshopcost
		List<UInt32> uMSHOPrefreshtimes;
		List<UInt32> uSSHOPrefreshtimes;
		List<UInt32> uSSHOPbuytimes;

		List<UInt32> uCOINSHOPrefreshtimes;
		List<UInt32> uCOURAGESHOPrefreshtimes;

		//territorylucky
		List<UInt32> uTERRITORYLrefreshs;		
		List<UInt32> uTERRITORYLcosts;
		List<UInt32> uTERRITORYLbuytimess;
		UInt32 uTERRITORYLmaxtimes;
		UInt32 uTERRITORYLrecovertime;
		UInt32 uTERRITORYLchallage;
		UInt32 uTERRITORYLfreerefreshtimes;
		UInt32 uTERRITORYLfreerefresh;	
		UInt32 uTERRITORYLrmbrefresh;

		//gainwealth
		UInt32 uGAINWEALTHfreetimes;


		//growthfound
		UInt32 GROWTHFOUNDviplvneed;
		UInt32 GROWTHFOUNDcost;

		//复活玩家参数（战役、十二宗宫、勇气试炼、多人副本）
		UInt32	uReliveBaseCost;
		UInt32	uReliveRatio;
		UInt32	uReliveMaxCost;

		//复活多人副本助手参数（单次复活最大花费160元宝）
		UInt32	uRelivePlayer2BaseCost;
		UInt32	uRelivePlayer2Ratio;
		UInt32	uRelivePlayer2MaxCost;

		//进度条金币赠送
		UInt32 uFISHEATmaxtimes;
		UInt32	uFISHEATgivecoin;
		UInt32	uFISHEATeatfish;
		UInt32 uFISHEATmaxpresscount;


		//排名赛
		UInt32 uRankGameTimes;

		List<UInt32> uRankGameBuyTimesCost;
		List<UInt32>	uRankGameNum;
		UInt32 uRankGamewinScore;
		UInt32	uRankGameFailScoreCost;
		UInt32	uRankGameReportNum;
		UInt32 uRankGameTime;
		UInt32 uRankGameShopRefreshTimes;
		UInt32 uRankGameShopRefreshTime;
		UInt32 uRankGameShopRefreshCost;
		UInt32 uRankGameOpenLevel;
		UInt32 uRankGameRewardResetTime;
		UInt32 uRankGameMaxFinishTime;

		 
		//行侠仗义
		UInt32 uCatTaskInitGold;
		UInt32 uCatTaskAddGold;
		UInt32 uCatTaskMaxGold;

		//行商猫
		UInt32 uBusinessCatInitGold;
		UInt32 uBusinessCatAddGold;
		UInt32 uBusinessCatMaxGold;
	
		//公会玩法开启等级（依据公会等级）
		
		//猫员外开启等级
		UInt32 uGuildOpenBattle;
		//猫员外开启等级
		UInt32 uGuildOpenYuanWai; 
		//行商猫外开启等级
		UInt32 uGuildOpenBusinessCat;
		//厨房训练
		UInt32 uGuildOpenKitchen ;
		//票友集会
		UInt32 uGuildOpenTicket ;
		//签到开启等级
		UInt32 uGuildOpenSign ;

		//行侠仗义
		UInt32 uGuildOpenCatTask; 

		//中括号内的填写方法为[时间,id,时间,id]
		List<UInt32> uSystemRefreshTime;

		//玩家初始详情
		std::string sRole1; 
		UInt32 uRolePower1; 
		std::string sRole2; 
		UInt32 uRolePower2; 
		std::string sRole3; 
		UInt32 uRolePower3; 

		//关卡完成时间增加的误差值
		UInt32 uCopyFinishTimeDeviation; 

		//公会玩法刷新时间
		
		//签到
		UInt32 uConsortSignReset ;
		
		//票友集会
		UInt32 uTicketFriendReset;
		//眼力训练
		UInt32 uEyesightReset ;
		//厨房特训
		UInt32 uKitchenReset ;
		//行侠仗义
		UInt32 uXingXiaReset ;
		//口才训练
		UInt32 uEloquenceReset;
		//木猫阵法重置
		UInt32 uWoodencatReset;

		//赏罚令，单抽最大次数
		UInt32 uRewardLotteryFreeOnceMaxTimes;

		//木猫阵法购买buff花费
		//攻击buff花费
		UInt32 uWoodencatattackbuffcost;

		//防御buff花费
		UInt32 uWoodencatdefensebuffcost ;
		//暴击buff花费
		UInt32 uWoodencatCritbuffcost ;


	};
	
	class GlobalValueManager: public Singleton<GlobalValueManager>
	{
	
	public:
		bool initGlobalValue();
		bool checkUTFStringIsSame(String uString,String str);
		void getListFromString(String str,List<UInt32> &list);


		GlobalValue &getGlobalValue();
		GlobalValue *getGlobalValueData();




	
		
		
	};

	static GlobalValue gGlobalValueTemplate;




	 



}

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_GLOBLEVALUE_MANAGER_H__
