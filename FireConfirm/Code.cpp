
#include "../LogicSystem.h"
#include "Code.h"

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

void CodeManager::codeResp(PassporCodeResp msg)
{
	List<Goods> tmpItemList;
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
		NetworkSystem::getSingleton().sender( player->getConnId(),CAT_COIN_RESP,jsonStr);
	}
	if (nowTime < msg.beginTime &&nowTime > msg.endTime)
	{
		resp.result = LynxErrno::TimeNotRight;
	}
// 	if (msg.awardID == LynxErrno::None)
// 	{
// 		resp.result = LynxErrno::NotFound;
// 	}

	if (resp.result == LynxErrno::None)
	{

		GiftManager::getSingleton().getAwardByID(msg.awardID,0,tmpItemList);
		for(List<Goods>::Iter *it = tmpItemList.begin();it!= NULL;it = tmpItemList.next(it))
		{
			GiftManager::getSingleton().getContentByID(it->mValue.subtype,resp.ends);
		}
		GiftManager::getSingleton().combineSame(resp.ends);

		Award award;
		award.award = resp.ends;
		resp.awards.insertTail(award);

		GiftManager::getSingleton().addToPlayer(player->getPlayerGuid(),REFLASH_AWARD,resp.ends);

		GiftManager::getSingleton().PlayerItemChangeResult(player->getPlayerGuid(),0,resp.ends);


		PassportCodeReq passpReq;
		passpReq.reqType = msg.reqType;
		passpReq.code = msg.code;
		passpReq.playerID = player->getPlayerGuid();
		passpReq.isSave = 1;
		PassportSystem::getSingleton().postThreadMsg(passpReq, 0);
	}

	std::string jsonStr = resp.convertDataToJson();
	NetworkSystem::getSingleton().sender( player->getConnId(),PASSPORT_CODE_RESP,jsonStr);
}


