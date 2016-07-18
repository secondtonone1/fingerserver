#include "CharactorMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"
#include "RankSystem.h"
using namespace Lynx;

void CharactorMsgHandler::onCharactorBuy(const ConnId&connId, CGCharactorBuyReq&msg)
{
	Player* selfplayer = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!selfplayer)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;

	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{

		UInt64 modelId = root["modelid"].asUInt64();

	   selfplayer->getCharactorManager().buyCharactor(modelId);



	}
	else
	{
		LOG_INFO("empty message charactor buy!");
	}
}


void CharactorMsgHandler::onCharactorEquipFashion(const ConnId&connId, CGFashionEquipReq&msg)
{
	Player* selfplayer = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!selfplayer)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;

	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{

		UInt64 modelId = root["modelid"].asUInt64();
		UInt64 fashionid = root["fashionid"].asUInt64();

	   selfplayer->getCharactorManager().equipFashion(modelId, fashionid);



	}
	else
	{
		LOG_INFO("empty message fashionequip buy!");
	}
	
}


void CharactorMsgHandler::onCharactorChange(const ConnId&connId, CGCharactorSwitchReq&msg)
{

	Player* selfplayer = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!selfplayer)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;

	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{

		UInt64 modelId = root["modelid"].asUInt64();
	

	   selfplayer->getCharactorManager().changeCharactor(modelId);
	}
	else
	{
		LOG_INFO("empty message charactor change!");
	}
	
}


void CharactorMsgHandler::onPowerRank(const ConnId & connId, CGPowerRankReq& msg)
{
	Player* selfplayer = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!selfplayer)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	Json::Value root;

	Json::Reader reader;

	if(reader.parse(msg.mReqJsonStr,root))
	{

		UInt64 playeruid = root["playeruid"].asUInt64();

		const std::string &respstr = RankSystem::getSingleton().sendPowerRankJson(playeruid);

		GCPowerRankResp rankResp;
		rankResp.mRespJsonStr = respstr;
		rankResp.mPacketID = BOC_POWERRANK_RESP;
		NetworkSystem::getSingleton().sendMsg(rankResp, connId);

		cout << respstr << endl;
	
	}
	else
	{
		LOG_INFO("empty message power rank !");
	}

}