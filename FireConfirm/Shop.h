#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_SHOP_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_SHOP_MANAGER_H__
#include "../NetworkLib/MsgHdr.h"
#include "../PersistThreadMsg.h"
#include "../CommonLib/MsgfireInfoToJson.h"
#include "../CommonLib/CommonLib.h"
#include "Gift.h"

namespace Lynx
{

	class Player;
	enum Shop
	{
		RefreshMaxTimes = 20,
		MysticalShop = 26,//神秘商店
		ServantShop = 27,//佣兵商店
		GiftShop = 28,//礼包商店
	};


	struct ShopReq
	{
		ShopReq(): reqType(0),shopType(0),needSend(1){}

		UInt32 reqType;
		UInt32 shopType;
		UInt32 needSend;//1需要发送,0不部需要发送

		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				reqType = root["reqType"].asUInt();
				shopType = root["shopType"].asUInt();				
			}
		}
		LYNX_MESSAGE_1(SHOP_INFO_REQ, ShopReq,std::string, jsonStr);
	};
	

	struct ShopResp	
	{
		ShopResp(): reqType(0),result(0),shopType(0),refreshTimes(0),gold(0),refreshNeedTime(0),refreshCost(0),refreshMaxTimes(0){}

		UInt32 reqType;
		UInt32 shopType;
		UInt32 result;
		UInt32 refreshTimes;//
		UInt32 gold;
		UInt32 refreshNeedTime;
		UInt32 refreshCost;
		UInt32 refreshMaxTimes;
		List<Item> items;	
		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);
			root["shopType"] = Json::Value(shopType);
			root["refreshTimes"] = Json::Value(refreshTimes);
			root["gold"] = Json::Value(gold);
			root["refreshNeedTime"] = Json::Value(refreshNeedTime);
			root["refreshCost"] = Json::Value(refreshCost);
			root["refreshMaxTimes"] = Json::Value(refreshMaxTimes);

			for(List<Item>::Iter * iter = items.begin(); iter != NULL; iter = items.next(iter))
			{
				Json::Value son;
				son["position"] = Json::Value(iter->mValue.position);
				son["buyTimes"] = Json::Value(iter->mValue.buyTimes);
				son["goods"].append(iter->mValue.goods.resourcestype);
				son["goods"].append(iter->mValue.goods.subtype);
				son["goods"].append(iter->mValue.goods.num);
			
				root["items"].append(son);
			}
			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};


	struct ShopBuyReq
	{
		ShopBuyReq(): shopType(0),num(0),position(0){}

		UInt32 shopType;
		UInt32 num;
		UInt32 position ;//普通关卡为星数，多人副本为完成时间
		List<AwardMonsterDamage> awardMonsterList;
		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				shopType = root["shopType"].asUInt();
				num = root["num"].asUInt();
				position  = root["position"].asUInt();
			}
		}
		LYNX_MESSAGE_1(SHOP_BUY_REQ, ShopBuyReq,std::string, jsonStr);
	};

	struct ShopBuyResp	
	{
		ShopBuyResp(): shopType(0),result(0),buyNum(0),position(0){}

		UInt32 shopType;
		UInt32 result;
		UInt32 buyNum;//
		UInt32 position ;//
		Json::Value allAttr;//
		List<Award> awards;
		List<Goods> ends;


		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["shopType"] = Json::Value(shopType);
			root["result"] = Json::Value(result);
			root["buyNum"] = Json::Value(buyNum);
			root["position"] = Json::Value(position);

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


	struct StrengthBuyReq
	{
		StrengthBuyReq(): reqType(0),num(0),needSend(1){}

		UInt32 reqType;
		UInt32 num;
		UInt32 needSend;//1需要发送,0不部需要发送

		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				reqType = root["reqType"].asUInt();
				num = root["num"].asUInt();				
			}
		}
		LYNX_MESSAGE_1(STRENGTH_BUY_REQ, StrengthBuyReq,std::string, jsonStr);
	};


	struct StrengthBuyResp	
	{
		StrengthBuyResp(): reqType(0),result(0),strength (0),buyNum(0),gold(0){}

		UInt32 reqType;
		UInt32 result;
		UInt32 strength ;
		UInt32 buyNum;//
		UInt32 gold;
	
		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);
			root["strength"] = Json::Value(strength);
			root["buyNum"] = Json::Value(buyNum);
			root["gold"] = Json::Value(gold);

			Json::FastWriter writer;  
			std::string strWrite = writer.write(root);
			return strWrite;
		}
	};

	struct CoinBuyReq
	{
		CoinBuyReq(): reqType(0),num(0),needSend(1){}

		UInt32 reqType;
		UInt32 num;
		UInt32 needSend;//1需要发送,0不部需要发送

		std::string jsonStr;
		void convertJsonToData(std::string jsonStr)
		{
			Json::Reader reader;    
			Json::Value root;    
			if (reader.parse(jsonStr, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素      
			{
				reqType = root["reqType"].asUInt();
				num = root["num"].asUInt();				
			}
		}
		LYNX_MESSAGE_1(COIN_BUY_REQ, CoinBuyReq,std::string, jsonStr);
	};


	struct CoinBuyResp	
	{
		CoinBuyResp(): reqType(0),result(0),buyNum(0),freeTimes(0),maxBuyTimes(0){}

		UInt32 reqType;
		UInt32 result;
		UInt32 buyNum;//
		UInt32 freeTimes;//客户端不用了，在ends里
		UInt32 maxBuyTimes;
		Json::Value allAttr;//
		List<Award> awards;
		List<Goods> ends;

		std::string convertDataToJson()
		{
			Json::Value root;     	
			root["reqType"] = Json::Value(reqType);
			root["result"] = Json::Value(result);
			root["buyNum"] = Json::Value(buyNum);
 			root["freeTimes"] = Json::Value(freeTimes);
			root["maxBuyTimes"] = Json::Value(maxBuyTimes);

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


	class ShopManager : public Singleton<ShopManager>
	{

	public:

		static void onShopReq(const  ConnId& connId, ShopReq & msg);

		static void onShopBuyReq(const  ConnId& connId, ShopBuyReq & msg);


		static void onCoinBuyReq(const  ConnId& connId, CoinBuyReq & msg);



		void getShopContentByID(UInt32 itemID,List<Goods> &ItemList);

		UInt32 setShopItems(Guid playerID, UInt32 type,UInt32 refreshTimes, UInt32 refreshTime, bool needSave);
			
		UInt32 setShopItem(Guid playerID, UInt32 type,UInt32 rank, Item item, bool needSave);
		
		ShopItems getShopItems(Guid playerID, UInt32 type);
		
		Item getShopItem(Guid playerID,UInt32 type,UInt32 rank);

		UInt32 dealMysticalshopbuy(Guid playerID,UInt32 shopType,UInt32 position,ShopBuyResp &resp);

		UInt32 dealGiftshopbuy(Guid playerID,UInt32 shopType,UInt32 position,ShopBuyResp &resp);

		UInt32 getMysticalShopInfo(Guid playerID,UInt32 refresh,ShopResp &resp);

		UInt32 getServantShopInfo(Guid playerID,UInt32 refresh,ShopResp &resp);

		UInt32 getGiftShopInfo(Guid playerID,UInt32 refresh,ShopResp &resp);

		void getShopNewItems(Guid playerID, UInt32 position,UInt32 acID,UInt32 shopType,Item &item);

		void getPlayerItem(Guid playerID,UInt32 shopType,UInt32 position,Item &item,UInt32 &refreshTime,UInt32 &refreshTimes);

		UInt32 getMysticalShopID(UInt32 position,UInt32 level,UInt32 vipLevel,UInt32 refreshTimes,UInt32 refresh);//获取大商品ID

		UInt32 getServantShopID(UInt32 position,UInt32 level,UInt32 vipLevel,UInt32 refreshTimes,UInt32 refresh);

		UInt32 getGiftShopID(UInt32 position,UInt32 level,UInt32 vipLevel,UInt32 refreshTimes,UInt32 refresh);

		UInt32 checkRefreshTime(Guid playerID,UInt32 shopType,UInt32 &refreshNeedTime);

		void getPremiumsContent(UInt32 nowBuyTimes,List<Goods>& itemList);

		UInt32  getCoinBuyMaxTimes(UInt32 vipLevel);

		UInt32  getStrengthBuyMaxTimes(UInt32 vipLevel);

	};

}

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_SHOP_MANAGER_H__
