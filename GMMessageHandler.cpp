#include "GMMessageHandler.h"
#include "LogicSystem.h"
#include "NetworkSystem.h"
#include "PersistSystem.h"
#include "../RedisManager.h"
#include "GameServer.h"

using namespace Lynx;

void GMMessageHandler::onMsgGMReq(const ConnId& connId, CGGMReq& msg)
{
	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
	if (!player)
	{
		LOG_WARN("Failed to get Player by ConnId: %llu", connId);
		return;
	}

	std::string reqJsonStr = msg.mReqJsonStr;
	Json::Reader reader;

	Json::Value root;
	if (reader.parse(reqJsonStr, root)) 
	{
		UInt64 playerUid = root["playerUid"].asUInt64();

		Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
		if (!player)
		{
			LOG_WARN("Failed to get Player by ConnId: %llu", connId);
			return;
		}

		UInt64 etype = root["etype"].asUInt64();
		UInt64 param1 = root["param1"].asUInt64();
		UInt64 param2 = root["param2"].asUInt64();
		std::string param3 = root["param3"].asString();
		std::string param4 = root["param4"].asString();

		player->getGMManager().dealGMRequest(etype,param1,param2, param3, param4);
	}
	else
	{
		LOG_INFO("empty message GMMessage!");
	}
}



void GMMessageHandler::onMsgGMForbidLoginReq(const ConnId& connId, CGGMForbidLoginReq& msg)
{
	
	std::string reqJsonStr = msg.mReqJsonStr;
	Json::Reader reader;

	Json::Value root;
	if (reader.parse(reqJsonStr, root)) 
	{
		UInt32 forbidlogin = root["forbidlogin"].asUInt();
		if(forbidlogin == 1)
		{
			RedisManager::getSingleton().set("forbidlogin","1");
			LogicSystem::getSingleton().setForbidLogin(1);
			LOG_INFO("-----------------------------");
			LOG_INFO("%u", forbidlogin);
		}
		else
		{
			RedisManager::getSingleton().set("forbidlogin","0");
			LogicSystem::getSingleton().setForbidLogin(0);
			LOG_INFO("++++++++++++++++++");
			LOG_INFO("%u", forbidlogin);
		}

	}
	else
	{
		LOG_INFO("empty message GMMessage!");
	}
}


void GMMessageHandler::onMsgGMKickPlayerReq(const ConnId& connId, CGGMKickPlayerReq& msg)
{
	std::string reqJsonStr = msg.mReqJsonStr;
	Json::Reader reader;

	Json::Value root;
	if (reader.parse(reqJsonStr, root)) 
	{

		UInt64 playeruid = root["playerUid"].asUInt64();
		UInt64 seconds = root["forbidlong"].asUInt64();
		UInt64 begintime =  root["begintime"].asUInt64();
		LOG_INFO("playeruid is = %llu",playeruid);
		LOG_INFO("seconds is = %llu",seconds);

	    Player * kickPlayer = LogicSystem::getSingleton().getPlayerByGuid(playeruid);

		PersistForbidLoginTimeUpdate forbidlogintime;
		forbidlogintime.playerUid = playeruid;
		forbidlogintime.forbidlogintime = begintime + seconds;
		forbidlogintime.forbidbegintime = begintime;

		PersistSystem::getSingleton().postThreadMsg(forbidlogintime, playeruid);


		if(!kickPlayer)
		{
			return;
		}

		PlayerBaseData playerBaseData = kickPlayer->getPlayerBaseData();
		playerBaseData.m_nForbidLoginTime= time(0) + seconds;
		LOG_INFO("forbidtime is = %llu", playerBaseData.m_nForbidLoginTime);
		LOG_INFO("time(0) is = %llu", time(0));
		kickPlayer->setPlayerBaseData(playerBaseData);

		const ConnId& connId = kickPlayer->getConnId();
		if(connId == 0)
		{
			return;
		}

		if(time(0)>= begintime && time(0) < begintime + seconds)
		{
				LogicSystem::getSingleton().closeConnection(kickPlayer);
		}

	}
	else
	{
		LOG_INFO("empty message GMMessage!");
	}
}

void GMMessageHandler::onMsgGMForbidChatReq(const ConnId & connId, CGGMForbidChatReq & msg)
{
	std::string reqJsonStr = msg.mReqJsonStr;
	Json::Reader reader;

	Json::Value root;
	if (reader.parse(reqJsonStr, root)) 
	{

		UInt64 forbiduid = root["playerUid"].asUInt64();
		UInt32 timelong = root["timelong"].asUInt();
		UInt64 begintime = 	root["begintime"].asUInt64();
		LOG_INFO("forbiduid is=%llu", forbiduid);
		LOG_INFO("timelong is =%llu", timelong);


	    Player * forbidPlayer = LogicSystem::getSingleton().getPlayerByGuid(forbiduid);
		if(!forbidPlayer)
		{
			//设置数据库
			PersistForbidChatUpdate forbidChatUpdate;
			forbidChatUpdate.playerUid = forbiduid;
			forbidChatUpdate.disableflag = 1;
			forbidChatUpdate.begintime = begintime;
			forbidChatUpdate.timelong = timelong;

			PersistSystem::getSingleton().postThreadMsg(forbidChatUpdate, forbiduid);



			return;
		}
		//修改内存并修改数据库
	
		forbidPlayer->getChatManager().clientForbidChat( forbiduid, timelong, begintime);




	}
	else
	{
		LOG_INFO("empty message GMMessage!");
	}
}

void GMMessageHandler::onMsgGMSendEmailReq(const ConnId & connId, CGGMSendEmailReq & msg)
{
	std::string reqJsonStr = msg.mReqJsonStr;
	Json::Reader reader;

	Json::Value root;
	if (reader.parse(reqJsonStr, root)) 
	{

		UInt64 forbiduid = root["playerUid"].asUInt64();
		UInt64 tempid = root["tempid"].asUInt64();
		std::string desc = root["desc"].asString();
		std::string contantStr = root["contant"].asString();
		std::string titleStr = root["title"].asString();
		std::string fromStr = root["from"].asString();
		LogicSystem::getSingleton().GMEmailSend(tempid, forbiduid, desc, contantStr,titleStr, fromStr);
	


	}
	else
	{
		LOG_INFO("empty message GMMessage!");
	}
}

void GMMessageHandler::onMsgGMKickAllReq(const ConnId & connId, CGGMKickAllReq & msg)
{
	std::string reqJsonStr = msg.mReqJsonStr;
	Json::Reader reader;

	Json::Value root;
	if (reader.parse(reqJsonStr, root)) 
	{

		RedisManager::getSingleton().set("forbidlogin","1");
		LogicSystem::getSingleton().setForbidLogin(1);
		LogicSystem::getSingleton().kickOutAllPlayer();
		TimeUtil::sleep(10000);
		//GameServer::getSingleton().gameExit();
// 		exit(0);

	}
	else
	{
		LOG_INFO("empty message GMMessage!");
	}
}


void GMMessageHandler::onMsgGMNoticeReq(const ConnId & connId, CGGMNoticeReq & msg)
{
	std::string reqJsonStr = msg.mReqJsonStr;
	Json::Reader reader;

	Json::Value root;
	if (reader.parse(reqJsonStr, root)) 
	{
		std::string msg = root["msg"].asString();
	
		LogicSystem::getSingleton().sendGMSystemMsg(msg);

	}
	else
	{
		LOG_INFO("empty message GMMessage!");
	}

}


void GMMessageHandler::onMsgGMReset(const ConnId & connId, CGGMResetReq & msg)
{
	std::string reqJsonStr = msg.mReqJsonStr;
	Json::Reader reader;

	Json::Value root;
	if (reader.parse(reqJsonStr, root)) 
	{
		UInt32 resettype = root["resetType"].asUInt();
		UInt64 playeruid = root["playerUid"].asUInt64();
		
		switch(resettype)
		{
		case 1:
			LogicSystem::getSingleton().gmResetTongBao(playeruid);
			break;
		case 2:
			LogicSystem::getSingleton().gmResetMengChu(playeruid);
			break;
		case 3:
			LogicSystem::getSingleton().gmResetYushiGF(playeruid);
			break;
		case 4:
			LogicSystem::getSingleton().gmResetJiShiAB(playeruid);	
			break;
		case 5:
			{
					LogicSystem::getSingleton().gmResetTongBao(playeruid);
					LogicSystem::getSingleton().gmResetMengChu(playeruid);
					LogicSystem::getSingleton().gmResetYushiGF(playeruid);
					LogicSystem::getSingleton().gmResetJiShiAB(playeruid);	

			}
	
			break;
		case 6:
			LogicSystem::getSingleton().gmResetBashanSL(playeruid);
		case 7:
			LogicSystem::getSingleton().gmResetWuxianTZ(playeruid);
			break;
		case 8:
			LogicSystem::getSingleton().gmResetCourage(playeruid);
			break;
		case 9:
			LogicSystem::getSingleton().gmResetPaiWei(playeruid);
			break;
		case 10:
			LogicSystem::getSingleton().gmResetPaiWeiBuy(playeruid);
			break;
		case 11:
			LogicSystem::getSingleton().gmReset12HaoJiao(playeruid);
			break;
			case 12:
			LogicSystem::getSingleton().gmResetFoodsCook(playeruid);
			break;
			case 13:
			LogicSystem::getSingleton().gmResetServantCall1(playeruid);
			break;
			case 14:
			LogicSystem::getSingleton().gmResetServantCall10(playeruid);
			break;
			case 15:
				LogicSystem::getSingleton().gmResetMonthToday(playeruid);
				break;
					case 16:
				LogicSystem::getSingleton().gmResetSevenDay(playeruid);
				break;
					case 17:
				LogicSystem::getSingleton().gmResetSevenTrain(playeruid);
				break;
					case 18:
				LogicSystem::getSingleton().gmResetGrowBuy(playeruid);
				break;
					case 19:
				LogicSystem::getSingleton().gmResetFenShi(playeruid);
				break;
						case 20:
				LogicSystem::getSingleton().gmResetOnlineAward(playeruid);
				break;
					
			case 21:
			LogicSystem::getSingleton().gmResetDailyTask(playeruid);
			break;

			case 22:
			LogicSystem::getSingleton().gmResetBaiCaiShen(playeruid);
			break;
			case 23:
			LogicSystem::getSingleton().gmResetStrengthBuy(playeruid);
			break;
			case 24:
			LogicSystem::getSingleton().gmResetLianPu(playeruid);
			break;
				case 25:
			LogicSystem::getSingleton().gmResetTongBaoS(playeruid);
			break;
				case 26:
			LogicSystem::getSingleton().gmResetZBG(playeruid);
			break;
				case 27:
			LogicSystem::getSingleton().gmResetServantShop(playeruid);
			break;
		}
		
        
	}
	else
	{
		LOG_INFO("empty message GMMessage!");
	}
}


void GMMessageHandler::onMsgGMAwardBag(const ConnId & connId, CGGAwardBagReq & msg)
 {
	 std::string reqJsonStr = msg.mReqJsonStr;
	 Json::Reader reader;

	 Json::Value root;
	 if (reader.parse(reqJsonStr, root)) 
	 {
		 UInt64 playeruid = root["playeruid"].asUInt64();
		 UInt64 awardid = root["awardid"].asUInt64();

		 LogicSystem::getSingleton().sendGMAwardBag(playeruid, awardid);

	 }
	 else
	 {
		 LOG_INFO("empty message GMMessage!");
	 }
 }

