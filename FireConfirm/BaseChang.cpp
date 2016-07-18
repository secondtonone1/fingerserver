#include "FireConfirmManager.h"
#include "../LogicSystem.h"
#include "StringConverter.h"
#include "../PlatformLib/SerializeType.h"
// #include "../CommonLib/PlayerData.h"
#include "../PersistSystem.h"
#include "../PlatformLib/Utilex.h"
#include "../CommonLib/CopyFinishTime.h"
#include "Gift.h"
#include "BaseChang.h"

using namespace Lynx;
// 
// void BaseChangManager::checkExp(const  ConnId& connId)
// {
// 	UInt32 level = 0;
// 	UInt64 maxLevel = gPlayerExpTable->mMap.getMax()->mKey;	
// 	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
// 	level = player->getVipLevel();
// 	PlayerExpTemplate *nextLevelExpTemplate = gPlayerExpTable->get(level + 1);
// 	UInt32 exp = player->getPlayerExp();
// 
// 	while(exp >= nextLevelExpTemplate->mExp)
// 	{
// 		exp -= nextLevelExpTemplate->mExp;
// 		level += 1;
// 
// 		levelUp(connId,level);
// 
// 
// 		if (level >= maxLevel)
// 		{
// 			break;
// 		}
// 		nextLevelExpTemplate = gPlayerExpTable->get(level + 1);
// 
// 	}
// 	player->setPlayerExp(exp);
// 
// 	player->getPersistManager().setDirtyBit(BASEDATABIT);
// 
// 	GCPlayerDetailResp detailInfoResp;
// 	detailInfoResp.mPacketID = BOC_PLAYER_DETAIL_RESP;
// 	detailInfoResp.mRespJsonStr = player->getPlayerInfoManager().convertDetailInfoToJson();
// 	NetworkSystem::getSingleton().sendMsg(detailInfoResp, connId);
// 
// 	
// 
// 
// 	//sendbasedate
// 
// 
// 
// }
// 
// void BaseChangManager::levelUp(const  ConnId& connId,UInt32 level)
// {
// 	Player* player = LogicSystem::getSingleton().getPlayerByConnId(connId);
// 
// 	player->setPlayerLeval(level);
// 
// 	//角色属性，技能改变
// 
// 
// }



