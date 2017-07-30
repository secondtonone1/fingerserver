
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
		LogicSystem::getSingleton().setForbidLogin(1);

#ifdef __linux__
		RobotManager::getSingleton().insertRobotDetail();
#endif

		serverPublicData.gServerRobotRefreshFlag = 1;
		ServerData::getSingleton().setServerPublicData(serverPublicData,true);

		PersistRobotDataReq req;
		req.flag = 1;
		PersistSystem::getSingleton().postThreadMsg(req,0);


	}
	return true;
	
}


void RobotManager::insertRobotDetail()
{
	
	std::string detailInfo;
	char dest[64] = {};
	String playerUidStr;

	for(Map<UInt64, RobotTableTemplate>::Iter *iter = gRobotTable->mMap.begin();iter != NULL;iter = gRobotTable->mMap.next(iter))
	{
		Json::Value root;
		dest[0] = '\0';
		root.append(iter->mValue.roleID);
		root.append(iter->mValue.name.c_str());
		root.append(iter->mValue.guild.c_str());
		root.append(iter->mValue.title.c_str());
		root.append(iter->mValue.level);
		root.append(iter->mValue.power);
		root.append(iter->mValue.vipLv);
		root.append(iter->mValue.modelID);
		root.append(iter->mValue.puonfashionID.c_str());
		root.append(iter->mValue.equipID1.c_str());
		root.append(iter->mValue.skill1.c_str());
		root.append(iter->mValue.skill2.c_str());
		root.append(iter->mValue.skill3.c_str());
		root.append(iter->mValue.rhymeCount);
// 		root.append(iter->mValue.rhymeskill1.c_str());
// 		root.append(iter->mValue.rhymeskill2.c_str());
// 		root.append(iter->mValue.rhymeskill3.c_str());
		root.append(iter->mValue.onstation1.c_str());
		root.append(iter->mValue.onstation2.c_str());
		root.append(iter->mValue.onstation3.c_str());
		root.append(iter->mValue.onstation4.c_str());
		root.append(iter->mValue.onstation5.c_str());

		root.append(iter->mValue.levelGrow.mMaxHP);
		root.append(iter->mValue.levelGrow.mMaxMP);
		root.append(iter->mValue.levelGrow.mMaxSP);
		root.append(iter->mValue.levelGrow.mPA);
		root.append(iter->mValue.levelGrow.mPF);
		root.append(iter->mValue.levelGrow.mMA);
		root.append(iter->mValue.levelGrow.mMF);
  		root.append(0);
		root.append(iter->mValue.levelGrow.mMPRecover);
		root.append(iter->mValue.levelGrow.mSPRecover);
		root.append(iter->mValue.levelGrow.mAttSpeed);
		root.append(iter->mValue.levelGrow.mCombinationAtt);
		root.append(iter->mValue.levelGrow.mMoveSpeed);
		root.append(iter->mValue.levelGrow.mJumpHeight);
		root.append(iter->mValue.levelGrow.mFlyTimes);
		root.append(iter->mValue.levelGrow.mSPDamage);
		root.append(iter->mValue.levelGrow.mAttPush);
		root.append(iter->mValue.levelGrow.mAttPushDef);
		root.append(iter->mValue.levelGrow.mAttRaise);
		root.append(iter->mValue.levelGrow.mAttRaiseDef);
		root.append(iter->mValue.levelGrow.mStiff);
		root.append(iter->mValue.levelGrow.mStiffDef);
		root.append(iter->mValue.levelGrow.mBaseCrit);
		root.append(iter->mValue.levelGrow.mCritRate);
		root.append(iter->mValue.levelGrow.mCritDef);
		root.append(iter->mValue.levelGrow.mStunDef);
		root.append(iter->mValue.levelGrow.mStunTimeDef);
		root.append(iter->mValue.levelGrow.mSlowDef);
		root.append(iter->mValue.levelGrow.mSlowTimeDef);
		root.append(iter->mValue.levelGrow.mPoisonDef);
		root.append(iter->mValue.levelGrow.mBloodDef);
		root.append(iter->mValue.levelGrow.mKODef);
		root.append(iter->mValue.levelGrow.mFloatVal);
		root.append(iter->mValue.levelGrow.mFloatDef);
		root.append(iter->mValue.levelGrow.mFall);
		root.append(iter->mValue.levelGrow.mFallDef);
		root.append(iter->mValue.score);
		
		
		Json::FastWriter writer;  
		detailInfo = writer.write(root);		
		sprintf(dest,"%d",iter->mValue.roleID);		
		playerUidStr = (String)dest;	
#ifdef __linux__
		RedisManager::getSingleton().set(playerUidStr.c_str(), detailInfo.c_str());
#endif
	
	}
}