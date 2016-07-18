#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_GIFT_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_GIFT_MANAGER_H__
#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "../CommonLib/CommonLib.h"


namespace Lynx
{
	class Player;

	enum RewardLottery
	{
		RL_ONCE_FREE = 1,//
		RL_ONCE_TICKET = 2,
		RL_ONCE = 3,		
		RL_TEN_FREE	= 4,//
		RL_TEN	= 5,
		RL_VIP	= 6,
		RL_TEN_TICKET	= 7,
// 		免费
// 		代金券
// 		一抽
// 		免费十抽
// 		十连抽
// 		vip抽
// 		十连抽券

	};

//1,2,3伶人
	//4,5,7戏班
	//6 vip只有一次

	
	struct GiftReq
	{
		GiftReq(): giftID(0),giftNum(0){}

		UInt32 giftID;
		UInt32 giftNum;
		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				giftID = root["giftID"].asUInt();
				giftNum = root["giftNum"].asUInt();
			}
		}
		LYNX_MESSAGE_1(OPEN_GIFT_REQ, GiftReq,std::string, strReceive);
	};

	struct BoxReq
	{
		BoxReq(): boxID(0),boxNum(0){}

		UInt32 boxID;
		UInt32 boxNum;
		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				boxID = root["boxID"].asUInt();
				boxNum = root["boxNum"].asUInt();
			}
		}

		LYNX_MESSAGE_1(OPEN_BOX_REQ, BoxReq,std::string, strReceive);

	};

	struct RewardLotteryMsgReq
	{
		RewardLotteryMsgReq(){}

		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
			}
		}

		LYNX_MESSAGE_1(OPEN_LOTTERY_MSG_REQ, RewardLotteryMsgReq,std::string, strReceive);

	};
	struct RewardLotteryMsgResp
	{
		RewardLotteryMsgResp(): onceLeftTime(0),tenLeftTime(0),couponOnceNum(0),couponTenNum(0){}

		UInt32 onceLeftTime;
		UInt32 tenLeftTime;
		UInt32 couponOnceNum;
		UInt32 couponTenNum;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["onceLeftTime"] = Json::Value(onceLeftTime);
			root["tenLeftTime"] = Json::Value(tenLeftTime);
			root["couponOnceNum"] = Json::Value(couponOnceNum);
			root["couponTenNum"] = Json::Value(couponTenNum);


			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	struct RewardLotteryReq
	{
		RewardLotteryReq():reqType(0){}

		UInt32 reqType;// 1	免费，2代金券，3一抽，4	免费十抽，5	十连抽，6	vip抽，7代金券十连抽，8 代金券vip抽


		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				reqType = root["reqType"].asUInt();
			}
		}

		LYNX_MESSAGE_1(OPEN_REWARD_LOTTERY_REQ, RewardLotteryReq,std::string, strReceive);

	};


	struct RewardLotteryResp
	{
		RewardLotteryResp(): reqType(0),result(0),onceLeftTime(0),tenLeftTime(0),couponOnceNum(0),couponTenNum(0),gold(0){}

		UInt32 reqType;
		UInt32 result;
		// 		UInt32 couponNum;
		List<UInt32> newServant;//1 获得新佣兵
		// 		UInt32 leftTime;
		UInt32 gold;
		UInt32 onceLeftTime;
		UInt32 tenLeftTime;
		UInt32 couponOnceNum;
		UInt32 couponTenNum;

		List<Award> awards;
		List<Goods> ends;
		Json::Value allAttr;//发送物品属性

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);
			root["onceLeftTime"] = Json::Value(onceLeftTime);
			root["tenLeftTime"] = Json::Value(tenLeftTime);		
			root["couponOnceNum"] = Json::Value(couponOnceNum);
			root["couponTenNum"] = Json::Value(couponTenNum);
			root["gold"] = Json::Value(gold);

			for(List<UInt32>::Iter *item = newServant.begin();item !=NULL;item = newServant.next(item) )
			{			
				root["newServant"].append(item->mValue);
			}


			for(List<Award>::Iter * it = awards.begin(); it != NULL; it = awards.next(it))			
			{
				Json::Value son;
				for(List<Goods>::Iter * iter = it->mValue.award.begin(); iter != NULL; iter = it->mValue.award.next(iter))
				{
					Json::Value leaf;
					leaf.append(iter->mValue.resourcestype);
					leaf.append(iter->mValue.subtype);
					leaf.append(iter->mValue.num);

					son["award"].append(leaf);
				}
				root["awards"].append(son);
			}

			for(List<Goods>::Iter * iter = ends.begin(); iter != NULL; iter = ends.next(iter))
			{
				Json::Value son;
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);

				root["ends"].append(son);
			}

			root["allAttr"] = Json::Value(allAttr);

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};


	struct LotteryDrawReq
	{
		LotteryDrawReq(): drawType(0){}

		UInt32 drawType;//1 2 3 单抽，十连抽，时间抽
		std::string strReceive;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				drawType = root["drawType"].asUInt();
			}
		}

		LYNX_MESSAGE_1(OPEN_LOTTERY_DRAW_REQ, LotteryDrawReq,std::string, strReceive);

	};
	struct LotteryDrawResp
	{
		LotteryDrawResp(): drawType(0){}

		UInt32 drawType;
		List<Goods> goodsList;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["drawType"] = Json::Value(drawType);

			for(List<Goods>::Iter * iter = goodsList.begin(); iter != NULL; iter = goodsList.next(iter))
			{
				root["resourcestype"].append(iter->mValue.resourcestype);
				root["subtype"].append(iter->mValue.subtype);
				root["num"].append(iter->mValue.num);
			}

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	struct GiftResp
	{
		GiftResp(): ID(0){}

		UInt32 ID;
		List<Goods> goodsList;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["ID"] = Json::Value(ID);

			for(List<Goods>::Iter * iter = goodsList.begin(); iter != NULL; iter = goodsList.next(iter))
			{
				root["resourcestype"].append(iter->mValue.resourcestype);
				root["subtype"].append(iter->mValue.subtype);
				root["num"].append(iter->mValue.num);
			}

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	struct AwardResp
	{
		AwardResp(): ID(0){}

		UInt32 ID;
		List<Goods> goodsList;
		List<Goods> CardsList;
		List<Goods> monsterDropList;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["ID"] = Json::Value(ID);			
			for(List<Goods>::Iter * iter = goodsList.begin(); iter != NULL; iter = goodsList.next(iter))
			{
				Json::Value son;	
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["goodsList"].append(son);
			}
			for(List<Goods>::Iter * iter = CardsList.begin(); iter != NULL; iter = CardsList.next(iter))
			{
				Json::Value son;	
				son.append(iter->mValue.resourcestype);
				son.append(iter->mValue.subtype);
				son.append(iter->mValue.num);
				root["CardsList"].append(son);			
			}
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	struct CGPropertyChange
	{
		CGPropertyChange(): sFlag(""),sKey(""),sValue(""){}

		String sFlag;
		String sKey;
		String sValue;
		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				sFlag = root["sFlag"].asCString();
				sKey  = root["sKey"].asCString();
				sValue = root["sValue"].asCString();
			}
		}

		LYNX_MESSAGE_1(PROPERTY_CHAGE_REQ, CGPropertyChange,std::string, jsonStr);
	};


	struct PropertyChange
	{
		PropertyChange(): sFlag(""),sKey(""),sValue(""){}

		String sFlag;
		String sKey;
		String sValue;
		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["sFlag"] = Json::Value(sFlag.c_str());
			root["sKey"] = Json::Value(sKey.c_str());
			root["sValue"] = Json::Value(sValue.c_str());
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};


	struct CGPropertysChange
	{
		CGPropertysChange(): type(0){}

		UInt32 type;//大类 1 购买 2 结算界面 3 开箱子
		List<Goods> goodsList;
		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				type = root["type"].asUInt();

			}
		}

		LYNX_MESSAGE_1(PROPERTYS_CHAGE_REQ, CGPropertyChange,std::string, jsonStr);
	};


	struct PropertysChange
	{
		PropertysChange(): type(0){}

		UInt32 type;//大类 1 购买 2 结算界面 3 开箱子
		List<Goods> goodsList;
		std::string convertDataToJson()
		{				
			Json::Value root;     	
			root["type"] = Json::Value(type);
			for(List<Goods>::Iter * iter = goodsList.begin(); iter != NULL; iter = goodsList.next(iter))
			{
				Json::Value son; 
				son["resourcestype"]= Json::Value(iter->mValue.resourcestype);
				son["subtype"]= Json::Value(iter->mValue.subtype);
				son["num"]= Json::Value(iter->mValue.num);
				root["goodsList"].append(son);
			}
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};
	class GiftManager : public Singleton<GiftManager>
	{
	public:
		static void onOpenGiftReq(const  ConnId& ,GiftReq & );		//开礼包 
		static void onOpenBoxReq(const  ConnId& ,BoxReq & );	//开箱子
		static void onqiyongReq(const  ConnId& ,LotteryDrawReq & );//
		static void onPropertyChang(const  ConnId& ,CGPropertyChange & );		//属性更改

		static void onRewardLotteryMsgReq(const  ConnId& ,RewardLotteryMsgReq & );	//抽奖 又名赏罚令

		static void onRewardLotteryReq(const  ConnId& ,RewardLotteryReq & );	//抽奖

		List<UInt32> getRewardLotteryIDs(UInt32 typeID);

		UInt32 getOpenLotteryID();

		UInt32 getRewardLotteryAwardID(Guid playerID,UInt32 id,UInt32 &awardID);

		UInt32 checkRewardLotteryTen(Guid playerID,UInt32 typeID);

		UInt32 checkRewardLotteryTenCount(Guid playerID,UInt32 typeID,UInt32 &count);

		void getLotteryLeftTime(Guid playerID,UInt32 &onceTime,UInt32 &tenTime);

		void getAllAttr(Guid playerID,UInt32 resType,UInt32 subType,Json::Value &jsonValue);

		void servanNeedChangeToPiece(Guid playerID, List<Award> &awards,List<Goods> &ends);

		void getGift(const ConnId& connId ,UInt32 id,ChapterEndResp &resp);//获取礼包

		UInt32 modifyCoin(Guid playerID,Int32 num);//铜钱
		UInt32 modifyExp(Guid playerID,Int32 num);//经验
		UInt32 modifyItem(Guid playerID,Guid templateID,Int32 num);//装备
		UInt32 modifyShengWang(Guid playerID,Int32 num);//声望
		UInt32 modifyRhyme(Guid playerID,Int32 num);//声望		 

		UInt32 modifyYuanBao(Guid playerID,Int32 num);//元宝
		UInt32 modifyMercenary(Guid playerID,Int32 num);//佣兵
		UInt32 updateCounter(Guid playerID,UInt32 boxID,Int32 num);//计数器
		UInt32 updateRewardCounter(Guid playerID,UInt32 boxID,Int32 num);//回馈计数器
		UInt32 modifyCounter(const ConnId& connId,UInt32 counterType,UInt32 counterID,Goods &goods);//增加回馈计数器
		UInt32 updateFood(Guid playerID,UInt32 boxID,Int32 num);//美食屋存档

		UInt32 updateStrength(Guid playerID,UInt32 boxID,Int32 num);//体力存档

		UInt32 updateStageDatas(Guid playerID,List<Goods> itemList);



		// 		 UInt32 updateCopyCounter(Guid playerID,UInt32 copyID,Int32 num);//活动副本
		// 		 UInt32 subGold(Guid playerID,Int32 num);//铜钱
		// 		 UInt32 subExp(Guid playerID,Int32 num);//经验
		// 		 UInt32 subItem(Guid playerID,Guid templateID,Int32 num);//装备
		// 		 UInt32 subStength(Guid playerID,Int32 num);//体力
		// 		 UInt32 subShengWang(Guid playerID,Int32 num);//声望
		// 		 UInt32 subYuanBao(Guid playerID,Int32 num);//元宝
		// 		 UInt32 subMercenary(Guid playerID,Int32 num);//佣兵

		UInt32 PlayerItemChangeResult(Guid playerID,UInt32 reFlashType,List<Goods> &itemList);
		UInt32 addToPlayer(Guid playerID,UInt32 reFlashType,List<Goods> itemList);
		void sendToPlayer(Guid playerID,UInt32 type,List<Goods> itemList,List<UInt64> newItemUids);
		void sendItemListChange(Guid playerID,List<UInt64> ItemUids);
		UInt32 subFromPlayer(Guid playerID,Goods goods);
		UInt32 getEmputyBagNum(Guid playerID);
		void getContentByID(UInt32 itemID,List<Goods> &ItemList);
		void getChapterAward(const  ConnId& connId,List<AwardMonsterDamage> awardMonsterList);
		void getAwardByID(UInt32 itemID,UInt32 num,List<Goods> &ItemList);
		void getAwardCardByID(UInt32 itemID,List<Goods> &ItemList);
		void updateSingleProperty(Guid playerID,String strFlag,String strData);
		void ChoiseNFromWeigthList(UInt32 num , List<Resource> inList,List<Goods> &outList);
		//宝库次数//佣兵抽奖	

		void combineSame( List<Goods> &goodsList);//合并相同

		void splitKind( List<Goods> &goodsList, List<Goods> &kindgoodsList,UInt32 kind);//拆分一类资源

		void checkJewelry(List<Goods> &itemList,PlayerFireConfirmData &fireConfirmData,UInt32 isFull);
	};

	struct StepContent
	{
		UInt32 step;
		UInt32 contentID;
	};
	struct Counter
	{
		UInt32 itemID;
		List<StepContent> stepContent;
	};

	enum Lottery
	{
		LOTTERY_TYPE_TO_ID1 = 61001,//抽奖类型计数使用ID
		LOTTERY_TYPE_TO_ID2 = 61002,
		LOTTERY_TYPE_TO_ID3 = 61003,		
		LOTTERY_INTERVAL	= 48*60*60,//时抽间隔时间
		LOTTERY_MAX_DRAW	= 10,
	};


}


#endif // __LYNX_GAME_SERVER_GAME_MANAGER_GIFT_MANAGER_H__




// resType	subType		
// 1	1	铜钱	角色资源
// 1	2	元宝	
// 1	3	体力	
// 1	4	经验	
// 1	5	竞技点	
// 1	6	韵魂	
// 2	1	薄荷草	韵纹系统
// 2	2	荆棘草	
// 2	3	神仙草	
// 3	1	美食烧制次数	美食屋
// 3	2	全鱼火锅	
// 3	3	糖稀鱼丸	
// 3	4	鱼尾烧麦	
// 5	1	界石碎片1类	界石系统
// 5	2	界石碎片2类	
// 5	3	界石碎片3类	
// 5	4	界石碎片4类	
// 5	5	界石碎片5类	
// 5	6	界石碎片6类	
// 5	7	界石碎片7类	
// 5	8	界石碎片8类	
// 5	9	界石碎片9类	
// 5	10	界石碎片10类	
// 5	11	界石碎片11类	
// 5	12	界石碎片12类	
// 6	1	佣兵升级道具1	佣兵升级
// 6	2	佣兵升级道具2	
// 6	3	佣兵升级道具3	
// 6	4	佣兵升级道具4	
// 6	5	佣兵升级道具5	
// 7	id	佣兵	佣兵
// 8	id	佣兵碎片	佣兵
// 9	id	佣兵装备	佣兵
// 10	1	生命之光	佣兵兑换
// 11	id	material	装备材料
// 12	id	new_gem	宝石
// 13	id	new_jewelry	饰品
// 14	id	fashion	时装
// 16	id	hoarstoneequip	界石符文
// 99	1	通宝当铺次数	招福集市
// 99	2	猛禽市场次数	
// 99	3	玉石工坊次数	
// 99	4	市集安保次数	
// 99	5	巴山试炼	巴山试炼
// 99	6	十二宗宫	十二宗宫
// 99	7	无限挑战难度1	原初锣楼
// 99	8	无限挑战难度2	
// 99	9	无限挑战难度3	
// 99	10	勇气试炼	
// 99	11	排位赛	竞技场
// 99	12	斗技场	
// 99	13	大乱斗	
// 99	14	召唤boss	万象幻境
// 99	15	限时召唤boss	
// 99	16	世界boss	
