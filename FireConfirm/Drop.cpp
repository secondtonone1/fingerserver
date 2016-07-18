// #include "FireConfirmManager.h"
#include "../LogicSystem.h"
#include "StringConverter.h"
#include "../PlatformLib/SerializeType.h"
// #include "../CommonLib/PlayerData.h"
#include "../PersistSystem.h"
#include "../PlatformLib/Utilex.h"
#include "../CommonLib/CopyFinishTime.h"
#include "Drop.h"

using namespace Lynx;


// 
// self:init()
// 		 end
// 
// 		 function T_Drop:init()
// 		 if self.canFly == false then
// 			 return
// 			 end
// 
// 			 self.beforeDelayTime = GameConfig.DROP_BEFORE_FLY_DELAY_BASE_TIME + math.random(-2, 2)/10
// 			 self.waitTime = 0
// 			 end
// 
// 
// 
UInt32 DropManager::runPath(Guid playerID)
{
	Drop drop;
	drop.totalTime = 1;
	Pos nowPos;

	Character mCharacter;
	Player* player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	// 	mCharacter = player->GetCharacter();
	nowPos = mCharacter.getPos();
	Pos controlPos1;
	if (drop.targetPos.x > nowPos.x )
	{
		controlPos1.x = nowPos.x + (drop.targetPos.x - nowPos.x) * (rand()%(4)+4)/10;
	}
	else if (drop.targetPos.x < nowPos.x )
	{

		controlPos1.x = nowPos.x - (drop.targetPos.x - nowPos.x) * (rand()%(4)+4)/10;
	}
	Pos tarPos;
	tarPos.x = drop.targetPos.x;
	tarPos.y = drop.targetPos.y +50;
	List<Pos>controlPoints;
	controlPoints.insertTail(nowPos);
	controlPoints.insertTail(controlPos1);
	controlPoints.insertTail(tarPos);
	
	// 			 local bezierToAni = cc.BezierTo:create(self.totalTime, controlPoints)
	// 				 if bezierToAni ~= nil then
	// 					 local aniEndCB = cca.callFunc(function ()
	// self:moveEnd()
	// 	 end)
	// 	 local seq = cc.Sequence:create(bezierToAni, aniEndCB)
	// 	 self.showNode:runAction(seq)
	// 	 end
	// 	 end
	
	return 1;
}



// 			 function T_Drop:runPath()
// 			 self.totalTime = GameConfig.DROP_FLY_TIME
// 			 -- --start points
// 			 local nowPos = {}
// 		 nowPos.x, nowPos.y = self.showNode:getPosition()
// 
// 			 --control points 1
// 			 local controlPos1 = cc.p(0,0)
// 			 if self.targetPos.x > nowPos.x then
// 				 controlPos1.x = nowPos.x + (self.targetPos.x - nowPos.x) * math.random(4, 8)/10
// 				 elseif self.targetPos.x < nowPos.x then 
// 				 controlPos1.x = nowPos.x - (nowPos.x - self.targetPos.x) * math.random(4, 8)/10
// 			 else
// 			 controlPos1.x = nowPos.x - 20 + math.random(0, 40)
// 			 end
// 			 controlPos1.y = nowPos.y + GameConfig.DROP_FLY_OFFSETY
// 
// 
// 			 local tarPos = {}
// 			 tarPos.x = self.targetPos.x
// 				 tarPos.y = self.targetPos.y + 50
// 				 --ani
// 				 local controlPoints = {
// 					 nowPos,
// 					 controlPos1,
// 					 tarPos,
// 			 }
// 
// 			 local bezierToAni = cc.BezierTo:create(self.totalTime, controlPoints)
// 				 if bezierToAni ~= nil then
// 					 local aniEndCB = cca.callFunc(function ()
// self:moveEnd()
// 	 end)
// 	 local seq = cc.Sequence:create(bezierToAni, aniEndCB)
// 	 self.showNode:runAction(seq)
// 	 end
// 	 end

Pos DropManager::getPlayerPositon(Guid playerID)
{

	Character mCharacter;
	Player* player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
// 	mCharacter = player->GetCharacter();
	Pos playerPos = mCharacter.getPos();
	return playerPos;

	
}


// 	 function T_Drop:getPlayerPositon(player_uuid)
// 	 local player = self.server:getPlayerFromSceneByUUID(player_uuid)
// 	 if player ~= nil then
// 		 local tarPos = {}
// 	 tarPos.x = player:getCharacter():getClient():getCharacter():getMainPicNode():getPositionX()
// 		 tarPos.y = player:getCharacter():getClient():getCharacter():getMainPicNode():getPositionY()
// 		 return tarPos.x, tarPos.y
// 		 end
// 
// 		 return nil, nil
// 		 end
// 
// 
// UInt32 DropManager::update(UInt64 t)
// {
// 	Drop drop;
// 
// 	T_Character mCharacter;
// 	Player* player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	// 	mCharacter = player->GetCharacter();
// 	Pos playerPos = mCharacter.getPos();
// 
// 	if (drop.canFly == FALSE)
// 	{
// 		return 2;
// 	}
// 	if (drop.flagEnd == FALSE)
// 	{
// 		return 3;
// 	}
// 
// 	if (drop.haveRun == true)
// 	{
// 		drop.waiteTime += t;
// 		if ( drop.waiteTime > drop.beforeDelayTime)
// 		{
// 			drop.pos = mCharacter.getPos();
// 		}
// 		drop.totalTime = 1;
// 		runPath();
// 		drop.haveRun = true;
// 		return 1;
// 	}


// 	return playerPos;


	// 		 if self.canFly == false then
	// 			 return
	// 			 end
	// 
	// 			 if self.flagEnd == true then
	// 				 return
	// 				 end
	// 
	// 				 if self.haveRun == false then
	// 					 self.waitTime = self.waitTime + t
	// 					 if self.waitTime > self.beforeDelayTime then
	// 						 self.targetPos.x, self.targetPos.y = self:getPlayerPositon(UserDataManager.UM_UUID)
	// 						 if (self.targetPos.x == nil) or (self.targetPos.y == nil) then
	// 							 self.canFly = false
	// 							 return
	// 							 end
	// 
	// 							 self.totalTime = GameConfig.DROP_FLY_TIME
	// self:runPath()
	// 	 self.haveRun = true
	// 	 end
	// 	 end
	// 	 end


// }
// 		 function T_Drop:update(t)
// 		 if self.canFly == false then
// 			 return
// 			 end
// 
// 			 if self.flagEnd == true then
// 				 return
// 				 end
// 
// 				 if self.haveRun == false then
// 					 self.waitTime = self.waitTime + t
// 					 if self.waitTime > self.beforeDelayTime then
// 						 self.targetPos.x, self.targetPos.y = self:getPlayerPositon(UserDataManager.UM_UUID)
// 						 if (self.targetPos.x == nil) or (self.targetPos.y == nil) then
// 							 self.canFly = false
// 							 return
// 							 end
// 
// 							 self.totalTime = GameConfig.DROP_FLY_TIME
// self:runPath()
// 	 self.haveRun = true
// 	 end
// 	 end
// 	 end

// 	 function T_Drop:moveEnd()
// 	 self.flagEnd = true
// 	 self.totalTime = 0
// 	 if self.dropType == "coin" then
// 		 self.server.recvAwardList.coin = self.server.recvAwardList.coin + self.value
// 		 self.server.parent.UiNode:setCoinLabel(self.server.recvAwardList.coin)
// 		 self.server.parent.UiNode:PlayGetAwardAnimation(self.dropType)
// 		 elseif self.dropType == "box" then
// 		 table.insert(self.server.recvAwardList, self.value)
// 		 self.server.parent.UiNode:setchestLabel(table.nums(self.server.recvAwardList))
// 		 self.server.parent.UiNode:PlayGetAwardAnimation(self.dropType)
// 		 end
// 
// 		 -- release node
// 		 self.showNode:removeSelf()
// 
// 		 -- release self
// 		 self.server:RemoveDrop(self.id)
// 		 end
// 
// 		 return T_Drop

void DropManager::moveEnd()
	 {

// 		 T_Character mCharacter;
// 		 Player* player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
// 		 mCharacter = player->GetCharacter();

	 }


