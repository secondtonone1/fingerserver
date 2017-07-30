
#include "../LogicSystem.h"
#include "Code.h"
#include "GlobalValue.h"
#include "InlineActivity.h"

using namespace Lynx;

void CodeManager::onCodeReq(const  ConnId& connId,CodeReq & req)
{

	CodeResp resp;
	PassportCodeReq passpReq;
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	req.convertJsonToData(req.strReceive);

	passpReq.reqType = req.reqType;
	passpReq.code = req.code;
	passpReq.playerID = player->getPlayerGuid();
	PassportSystem::getSingleton().postThreadMsg(passpReq, 0);

}

void CodeManager::codeResp(PassportCodeResp msg)
{
	List<Goods> tmpItemList;
	List<Goods> itemList;
	UInt32 nowTime = TimeUtil::getTimeSec();

	CodeResp resp;
	PassportCodeReq passpReq;
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(msg.PlayerID);	
	if (player == NULL)
	{
		return;
	}
	
	if (msg.errorId != LynxErrno::None)
	{
		resp.result = msg.errorId;
		std::string jsonStr = resp.convertDataToJson();
		NetworkSystem::getSingleton().sender( player->getConnId(),BOC_CODE_RESP,jsonStr);
		return;
	}
	if (nowTime < msg.beginTime &&nowTime > msg.endTime)
	{
		resp.result = LynxErrno::TimeNotRight;
	}

	if (resp.result == LynxErrno::None)
	{

		GiftManager::getSingleton().getAwardByID(msg.awardID,0,tmpItemList);
		for(List<Goods>::Iter *it = tmpItemList.begin();it!= NULL;it = tmpItemList.next(it))
		{
			GiftManager::getSingleton().getContentByID(it->mValue.subtype,itemList);
		}
		GiftManager::getSingleton().combineSame(itemList);

		char  des[2048] = {};
		char dest[2048] = {};
		String str1;
		String str2;
		char tmp[64] = {};
		sprintf(des,"兑换码奖励;系统;你获得的奖励有：;");
		for(List<Goods>::Iter * item = itemList.begin();item!=NULL;item = itemList.next(item))
		{
			sprintf(tmp,"(%d,%d,%d);",item->mValue.resourcestype,item->mValue.subtype,item->mValue.num);
			strcat(dest,tmp);
		}
		str1 = (String)des;
		str2 = (String)dest;

		str1 = InlineActivityManager::getSingleton().stringToUtf(str1);
		//sdasd;asdsadasd;asdasdasdas;####(1,2,100);(1,1,1000):(1,2,1000)
		//发送人;标题;正文;奖励用四个####隔开，后面（1,2,100）你知道的格式		

		GMManager::getSingleton().GMEmailSend(0, player->getPlayerGuid(), str1.c_str(),str2.c_str());


		PassportCodeReq passpReq;
		passpReq.reqType = msg.reqType;
		passpReq.code = msg.code;
		passpReq.playerID = player->getPlayerGuid();
		passpReq.isSave = 1;
		PassportSystem::getSingleton().postThreadMsg(passpReq, 0);
	}

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( player->getConnId(),BOC_CODE_RESP,jsonStr);
}



void CodeManager::onFishEatReq(const  ConnId& connId,FishEatReq & req)
{

	FishEatResp resp;
	UInt32 coin = 0;
	Goods goods;
	List<Goods> itemList;

	Player *player = LogicSystem::getSingleton().getPlayerByConnId(connId);	
	if (player == NULL)
	{
		return;
	}
	GlobalValue globalValue = GlobalValueManager::getSingleton().getGlobalValue();
	PlayerDailyResetData dailyResetData= player->getPlayerDailyResetData();
	req.convertJsonToData(req.strReceive);

	if (dailyResetData.m_nFishEatTimes <= 0)
	{
		resp.result = LynxErrno::FishEatTimesNotEnough;
	}
	if (req.kickCount > globalValue.uFISHEATmaxpresscount)
	{
		resp.result = LynxErrno::FishEatTimesNotEnough;
	}
	if(resp.result == LynxErrno::None &&req.kickCount != 0)
	{
		coin = req.kickCount * globalValue.uFISHEATgivecoin + req.fishEat* globalValue.uFISHEATeatfish;
		dailyResetData.m_nFishEatTimes --;
		player->setPlayerDailyResetData(dailyResetData);
		player->getPersistManager().setDirtyBit(DAILYRESETBIT);
		goods.resourcestype =AWARD_BASE;
		goods.subtype = AWARD_BASE_COIN;
		goods.num = coin;
		itemList.insertTail(goods);
		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,itemList,MiniLog137);
		
	}
	resp.coin = player->getPlayerCoin();
	resp.fishEatTimes = dailyResetData.m_nFishEatTimes;
	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( connId,BOC_FISHEAT_RESP,jsonStr);

}