
#include "../LogicSystem.h"
#include "../PersistSystem.h"
#include "../ServerData.h"
#include "../RedisManager.h"
#include "Robot.h"

using namespace Lynx;

bool RobotManager::initial()
{
	ServerPublicData  serverPublicData = ServerData::getSingleton().getServerPublicData();

 	if (serverPublicData.gServerRobotRefreshFlag == 0)
	{
		serverPublicData.gServerRobotRefreshFlag = 1;
		ServerData::getSingleton().setServerPublicData(serverPublicData);

		for(Map<UInt64, RobotTableTemplate>::Iter *iter = gRobotTable->mMap.begin();iter != NULL;iter = gRobotTable->mMap.next(iter))
		{
			PersistRobotDataReq req;
			req.uuid = iter->mValue.roleID;
			req.uid = iter->mValue.modelID;
 			req.name = iter->mValue.name;
			req.level = iter->mValue.level;
			req.viplevel = iter->mValue.vipLv;
			req.power = iter->mValue.power;
			PersistSystem::getSingleton().postThreadMsg(req,0);

		}
#ifdef __linux__
		RobotManager::getSingleton().insertRobotDetail();
#endif
	}
	return true;
	
}

void RobotManager::insertRobotDetail()
{
	Json::Value root;
	std::string detailInfo;
	char dest[64] = {};
	String playerUidStr;

	for(Map<UInt64, RobotTableTemplate>::Iter *iter = gRobotTable->mMap.begin();iter != NULL;iter = gRobotTable->mMap.next(iter))
	{
		dest[0] = '\0';
		root["roleID"] = Json::Value(iter->mValue.roleID);
		root["name"] = Json::Value(iter->mValue.name.c_str());
		root["guild"] = Json::Value(iter->mValue.guild.c_str());
		root["title"] = Json::Value(iter->mValue.title.c_str());
		root["level"] = Json::Value(iter->mValue.level);
		root["power"] = Json::Value(iter->mValue.power);
		root["vipLv"] = Json::Value(iter->mValue.vipLv);
		root["modelID"] = Json::Value(iter->mValue.modelID);
		root["puonfashionID"] = Json::Value(iter->mValue.puonfashionID.c_str());
		root["equipID1"] = Json::Value(iter->mValue.equipID1.c_str());
		root["skill1"] = Json::Value(iter->mValue.skill1.c_str());
		root["skill2"] = Json::Value(iter->mValue.skill2.c_str());
		root["skill3"] = Json::Value(iter->mValue.skill3.c_str());
		root["rhymeskill1"] = Json::Value(iter->mValue.rhymeskill1.c_str());
		root["rhymeskill2"] = Json::Value(iter->mValue.rhymeskill2.c_str());
		root["rhymeskill3"] = Json::Value(iter->mValue.rhymeskill3.c_str());
		root["onstation1"] = Json::Value(iter->mValue.onstation1.c_str());
		root["onstation2"] = Json::Value(iter->mValue.onstation2.c_str());
		root["onstation3"] = Json::Value(iter->mValue.onstation3.c_str());
		root["onstation4"] = Json::Value(iter->mValue.onstation4.c_str());
		root["onstation5"] = Json::Value(iter->mValue.onstation5.c_str());

		root["MaxHP"] = Json::Value(iter->mValue.levelGrow.mMaxHP);
		root["MaxMP"] = Json::Value(iter->mValue.levelGrow.mMaxMP);
		root["MaxSP"] = Json::Value(iter->mValue.levelGrow.mMaxSP);
		root["PA"] = Json::Value(iter->mValue.levelGrow.mPA);
		root["PF"] = Json::Value(iter->mValue.levelGrow.mPF);
		root["MA"] = Json::Value(iter->mValue.levelGrow.mMA);
		root["MF"] = Json::Value(iter->mValue.levelGrow.mMF);
// 		root["HPRecover"] = Json::Value(iter->mValue.levelGrow.mHPRecover);
		root["MPRecover"] = Json::Value(iter->mValue.levelGrow.mMPRecover);
		root["SPRecover"] = Json::Value(iter->mValue.levelGrow.mSPRecover);
		root["AttSpeed"] = Json::Value(iter->mValue.levelGrow.mAttSpeed);
		root["CombinationAtt"] = Json::Value(iter->mValue.levelGrow.mCombinationAtt);
		root["MoveSpeed"] = Json::Value(iter->mValue.levelGrow.mMoveSpeed);
		root["JumpHeight"] = Json::Value(iter->mValue.levelGrow.mJumpHeight);
		root["FlyTimes"] = Json::Value(iter->mValue.levelGrow.mFlyTimes);
		root["SPDamage"] = Json::Value(iter->mValue.levelGrow.mSPDamage);
		root["AttPush"] = Json::Value(iter->mValue.levelGrow.mAttPush);
		root["AttPushDef"] = Json::Value(iter->mValue.levelGrow.mAttPushDef);
		root["AttRaise"] = Json::Value(iter->mValue.levelGrow.mAttRaise);
		root["AttRaiseDef"] = Json::Value(iter->mValue.levelGrow.mAttRaiseDef);
		root["Stiff"] = Json::Value(iter->mValue.levelGrow.mStiff);
		root["StiffDef"] = Json::Value(iter->mValue.levelGrow.mStiffDef);
		root["BaseCrit"] = Json::Value(iter->mValue.levelGrow.mBaseCrit);
		root["CritRate"] = Json::Value(iter->mValue.levelGrow.mCritRate);
		root["CritDef"] = Json::Value(iter->mValue.levelGrow.mCritDef);
		root["StunDef"] = Json::Value(iter->mValue.levelGrow.mStunDef);
		root["StunTimeDef"] = Json::Value(iter->mValue.levelGrow.mStunTimeDef);
		root["SlowDef"] = Json::Value(iter->mValue.levelGrow.mSlowDef);
		root["SlowDef"] = Json::Value(iter->mValue.levelGrow.mSlowDef);
		root["SlowTimeDef"] = Json::Value(iter->mValue.levelGrow.mSlowTimeDef);
		root["PoisonDef"] = Json::Value(iter->mValue.levelGrow.mPoisonDef);
		root["BloodDef"] = Json::Value(iter->mValue.levelGrow.mBloodDef);
		root["KODef"] = Json::Value(iter->mValue.levelGrow.mKODef);
		root["FloatVal"] = Json::Value(iter->mValue.levelGrow.mFloatVal);
		root["FloatDef"] = Json::Value(iter->mValue.levelGrow.mFloatDef);
		root["Fall"] = Json::Value(iter->mValue.levelGrow.mFall);
		root["FallDef"] = Json::Value(iter->mValue.levelGrow.mFallDef);
		
		
		Json::FastWriter writer;  
		detailInfo = writer.write(root);		
		sprintf(dest,"%d",iter->mValue.roleID);		
		playerUidStr = (String)dest;	
#ifdef __linux__
		RedisManager::getSingleton().set(playerUidStr.c_str(), detailInfo.c_str());
#endif

	}
}