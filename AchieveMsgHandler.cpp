#include "AchieveMsgHandler.h"
#include "LogicSystem.h"
#include "GameServer.h"

using namespace Lynx;

void AchieveMsgHandler::onAchieveFinishReq(const ConnId&connId, CGAchieveFinishReq&msg)
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

		UInt64 achId = root["achievementid"].asUInt64();
		UInt64 eventId = root["event"].asUInt64();

	   selfplayer->getAchieveManager().finishAchieve(eventId,achId);



	}
	else
	{
		LOG_INFO("empty message finish req!");
	}
}


void AchieveMsgHandler::onDailyTaskAddReq(const ConnId&connId, CGDailyTaskAddReq &msg)
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

		UInt64 dailytaskId = root["dailyid"].asUInt64();
		

		selfplayer->getAchieveManager().dealAddDailyReq(dailytaskId);



	}
	else
	{
		LOG_INFO("empty message add dailytask req!");
	}
}


void AchieveMsgHandler::onDailyTaskFinishReq(const ConnId&connId, CGDailyTaskFinishReq &msg)
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

		UInt64 dailytaskId = root["dailyid"].asUInt64();


		selfplayer->getAchieveManager().finishDailyTask(dailytaskId);



	}
	else
	{
		LOG_INFO("empty message add dailytask req!");
	}
}

void AchieveMsgHandler::onActiveAwardGetReq(const ConnId& connId, CGActiveAwardGetReq &msg)
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

		UInt64 dailytaskId = root["index"].asUInt64();


		selfplayer->getAchieveManager().getActiveAward(dailytaskId);



	}
	else
	{
		LOG_INFO("empty message add dailytask req!");
	}
}