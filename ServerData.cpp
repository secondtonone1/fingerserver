#include "ServerData.h"
#include "PersistSystem.h"
#include "RedisManager.h"
#include "PassportThreadMsg.h"
#include "PassportSystem.h"
#include "ConfigParser.h"
using namespace Lynx;

void ServerData::serverDatainit()
{
	if (mServerPublicData.gServerID ==0)
	{
		PassportServerIDReq req;
		req.serverIP = ConfigParser::getSingleton().getMainListenIp().c_str() ;
		req.port = ConfigParser::getSingleton().getMainListenPort() ;
			
		PassportSystem::getSingleton().postThreadMsg(req,0);
	}

	

}
ServerPublicData &ServerData::getServerPublicData()
{
	return mServerPublicData;
}

ServerDailyData &ServerData::getServerDailyData()
{
	return mServerDailyData;
}




void ServerData::setServerPublicData(ServerPublicData &serverPublicData,bool neeSave = false)
{
	mServerPublicData = serverPublicData;

	if (neeSave == true)
	{
		PersistServerDataReq req;
		req.gServerID = serverPublicData.gServerID;
		req.gRechargeNum = serverPublicData.gRechargeNum;
		req.gServerRobotRefreshFlag = serverPublicData.gServerRobotRefreshFlag;		
		req.gRankGameRewardresetTime = serverPublicData.gRankGameRewardresetTime;
		req.gServerFirstOpenTime = serverPublicData.gServerFirstOpenTime;
		req.gServerSpare5 = serverPublicData.gServerSpare5;	
		
		PersistSystem::getSingleton().postThreadMsg(req,0);
	}
}


void ServerData::setServerDailyData(ServerDailyData &serverDailyData,bool neeSave = false)
{
	mServerDailyData = serverDailyData;

	if (neeSave == true)
	{
		PersistServerDailyDataReq req;
		req.gServerOnlineNum= serverDailyData.gServerOnlineNum;
		req.gServerOnlineMaxNum= serverDailyData.gServerOnlineMaxNum;
		req.gServerOnlineNumSetTime= serverDailyData.gServerOnlineNumSetTime;
		req.gSSpare1= serverDailyData.gSSpare1;
		req.gSSpare2= serverDailyData.gSSpare2;
		req.gSSpare3= serverDailyData.gSSpare3;
	

		PersistSystem::getSingleton().postThreadMsg(req,0);
	}
}

//SAVE_INV_TIME 5000*120 = 10min
static UInt32 gCount = 0;
void ServerData::saveOnlinePlayerGuid(Map<Guid, Player*> &mPlayerGuidMap)
{
	gCount ++;
	if (gCount < 120)
	{
		
		return;
	}
	gCount = 0;
	
	char tmp[8];
	char tmpNum[8];
	std::string str = "";
	std::string strNum = "";
	int num = 0;
	int count = 0;

	num = mPlayerGuidMap.size();
	
	for (Map<Guid, Player*>::Iter* iter = mPlayerGuidMap.begin();
		iter != NULL; iter = mPlayerGuidMap.next(iter))
	{
		count ++ ;
		if (count > 10000)
		{
			continue;
		}
		snprintf(tmp, sizeof(tmp),"%llu",iter->mKey);
		str += (std::string)tmp + ",";
	}

	sprintf(tmpNum,"%d",num);
	strNum = (std::string)tmpNum;

	RedisManager::getSingleton().set("OnlinePlayerGuids", str.c_str());
	RedisManager::getSingleton().set("OnlinePlayerNum", strNum.c_str());

	LOG_INFO("save saveOnlinePlayerGuid guids =  %s ",str.c_str());
}

void ServerData::checkMaxOnlineNum()
{

}
void ServerData::addOnlineNum()
{
	UInt32 nowTime  = TimeUtil::getTimeSec();
	UInt32 nowDayBeiginTime =  TimeUtil::getTimeSec() - TimeUtil::getSecOfToday();
	UInt32 oldDayBeiginTime =mServerDailyData.gServerOnlineNumSetTime - TimeUtil::getSecOfToThatday(mServerDailyData.gServerOnlineNumSetTime);
	
	if (nowDayBeiginTime != oldDayBeiginTime)
	{
		char dest1[1024]={};
		snprintf(dest1,sizeof(dest1),"%d.%d",mServerDailyData.gServerOnlineMaxNum,mServerDailyData.gServerOnlineNumSetTime);
		LogicSystem::getSingleton().write_server_log(LogType60,dest1,LogInfo);

		mServerDailyData.gServerOnlineNum = 0;
		mServerDailyData.gServerOnlineMaxNum = 0;
		mServerDailyData.gServerOnlineNumSetTime = nowTime;
		//save
		ServerData::setServerDailyData(mServerDailyData,true);		
	}

	mServerDailyData.gServerOnlineNum++;


	if (mServerDailyData.gServerOnlineNum > mServerDailyData.gServerOnlineMaxNum)
	{
		mServerDailyData.gServerOnlineMaxNum = mServerDailyData.gServerOnlineNum ;
		//save
		ServerData::setServerDailyData(mServerDailyData,true);
	}

}
void ServerData::subOnlineNum()
{

}