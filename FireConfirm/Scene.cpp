#include "FireConfirmManager.h"
#include "../LogicSystem.h"
#include "StringConverter.h"
#include "../PlatformLib/SerializeType.h"
#include "../PersistSystem.h"
#include "../PlatformLib/Utilex.h"
#include "../CommonLib/CopyFinishTime.h"
#include "Gift.h"
#include "Scene.h"
#include "PVPSystem.h"

using namespace Lynx;


void SceneManager::addPlayer(Guid playerID,UInt32 sceneID)
{
	PlayerChapterList chapterList;
	Player * player;
	Character *character;
	
	character = CharacterPool.top();
	CharacterPool.pop();

	character->initCharacter(playerID);

	m_SceneTempelete->playerPool.insertTail(playerID);
	m_SceneTempelete->sceneCharacter.CharacterMap.insert(playerID,character);

	

	player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	chapterList = player->getChapterList();
	chapterList.m_StageID = m_SceneTempelete->stageID;
	chapterList.m_SceneID = sceneID;
	player->setchapterList(chapterList);

	PVPSyncResp resp;
	resp.typeID = 7;
	resp.result = LynxErrno::None;
	resp.charDatas.insertTail(character->getCharacterData()) ;
	std::string jsonStr;
	jsonStr = resp.convertDataToJson();
	senderSceneAll(PVP_SYNC_RESP,jsonStr);

}

void SceneManager::removeScenePlayer(UInt32 playerID)
{
	UInt32 sceneID = 0;
	List<Guid> playerPool;
	PlayerChapterList chapterList;
	Player * player;


	player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	if (player != NULL)
	{
		chapterList = player->getChapterList();

		sceneID = chapterList.m_SceneID;
		chapterList.m_StageID = 0;
		chapterList.m_SceneID = 0;
		player->setchapterList(chapterList);
	}
	


	for(Map<Guid,Character*>::Iter * item = m_SceneTempelete->sceneCharacter.CharacterMap.begin();item != NULL;item = m_SceneTempelete->sceneCharacter.CharacterMap.next(item))
	{
		if (item->mKey == playerID)
		{
			CharacterPool.push(item->mValue);
			m_SceneTempelete->sceneCharacter.CharacterMap.erase(item);
		}
	}

	for(List<Guid>::Iter * it = m_SceneTempelete->playerPool.begin();it != NULL;it = m_SceneTempelete->playerPool.next(it))
	{
		if (it->mValue == playerID)
		{
			m_SceneTempelete->playerPool.erase(it);

		}	
		if (m_SceneTempelete->playerPool.size() == 0)
		{
			PVPSystem::getSingleton().removeScene(sceneID);
		}
	}

}

void SceneManager::updateHit()
{

}

void SceneManager::update()
{
	while(true)
	{
		UInt64 t = TimeUtil::getTimeMilliSec()/10000;
		if(lastUpdateTime - t <10000)
		{
 			TimeUtil::sleep(1000);
		}
		if (m_SceneTempelete = NULL)
		{
			return;
		}
		//todo
// 		for(Map<Guid,Character*>::Iter *iter = m_SceneTempelete->sceneCharacter.CharacterMap.begin();iter != NULL;iter = m_SceneTempelete->sceneCharacter.CharacterMap.next(iter))
// 		{
// 			
// 			iter->mValue->updateHeight(t);
// 			iter->mValue->updateMove(t);
// 		}
 		TimeUtil::sleep(1000);
	}

// 	Map<UInt32,SceneTempelete*>::Iter *it = SceneMap.find(sceneId);
	// 	sceneTempelete->PlayerPool;
	// 	for (Map<UInt32,Guid>::Iter * iter = sceneTempelete->PlayerPool.begin();iter != NULL; iter = sceneTempelete->PlayerPool.next(iter))
	// 	{
	// 		player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	// 		playerChapterList = player->getChapterData();
	// // 		heroSpeed = T_Character::getSingleton().getMoveSpeed();
	// // 		heroSpeed = playerChapterList.m_listChapterData
	// 	}
//iter->mValue->getHeroTemplate().mMoveSpeed

}

void SceneManager::switchJoystickSkillContent()
{

}
void SceneManager::onEvent(PVPTreadReq& msg )//这里的PVPTreadReq是传参过来，不是协议
{

	PVPSyncResp resp;
	std::string jsonStr;
	NameVal nameVal;

	nameVal.name = msg.typeID;
	nameVal.value = msg.value;
	if(m_SceneTempelete->isQTE == true)
	{
		return;
	}
	Player *player = LogicSystem::getSingleton().getPlayerByConnId(msg.connId);
	if (player == NULL)
	{
		LOG_WARN("player not found!!");
		return;
	}
	Character *character;
	SkillBaseTemplate *currSkill;


	for(Map<Guid,Character*>::Iter *iter = m_SceneTempelete->sceneCharacter.CharacterMap.begin();iter != NULL;iter = m_SceneTempelete->sceneCharacter.CharacterMap.next(iter))
	{
		if (iter->mKey == player->getPlayerGuid())
		{		
			character = iter->mValue;
			UInt32 skillID = character->getCurSkillID();
			SkillMap skillMap = character->getSkillMap();
			for(Map<UInt32, SkillBaseTemplate*>::Iter * it = skillMap.skillMap.begin();it != NULL;it = skillMap.skillMap.next(it))
			{
				if (skillID == it->mKey)
				{
					currSkill = it->mValue;
					break;
				}
			}
			character->setCanSkillControl(false);
			break;
		}
	}


	for(Map<Guid,Character*>::Iter *iter = m_SceneTempelete->sceneCharacter.CharacterMap.begin();iter != NULL;iter = m_SceneTempelete->sceneCharacter.CharacterMap.next(iter))
	{
		if (iter->mKey == player->getPlayerGuid())
		{
			if(nameVal.name == ButtonClick_direction)
			{
				//--按住了"上"按键

				if (nameVal.value == JOYSTICKDIR_UP )
				{	
					iter->mValue->setjoystick_updown(1);
				}
				else if (nameVal.value == JOYSTICKDIR_down)
				{
					if (iter->mValue->getjoystick_updown() != 2)
					{
						iter->mValue->setjoystick_updown(2);
						switchJoystickSkillContent();
					}
				}
				else if (nameVal.value == JOYSTICKDIR_right)
				{

					iter->mValue->setCanRealMove(true);
					iter->mValue->setjoystick_updown(0);
					if (iter->mValue->checkCanTurn())
					{
						iter->mValue->turnFace(true);
					}

					if (iter->mValue->getallowTurnFaceCommand() == true)
					{
						bool faceR = iter->mValue->getFaceR();
						if (faceR == false)
						{
							iter->mValue->setallowTurnFaceCommand(false);
							iter->mValue->sethaveRecvTurnFaceCommand(false);
							iter->mValue->turnFace(true);
						}
					}
					if(iter->mValue->checkCanRun() ==false)
					{
						iter->mValue->setCanRealMove(false);
					}

					if ((iter->mValue->getPosX() + iter->mValue->getDefenceAreaStand().w/2) >= m_SceneTempelete->sceneEdge.x )//半身+位置到边界？
					{
						iter->mValue->setCanRealMove(false);
					}

					if (iter->mValue->getNowState() == CHAR_STATE_ATTACK && currSkill->mControl==1)
					{

						if (iter->mValue->getCanRealMove() == true )
						{
							iter->mValue->setCanSkillControl(true);
							UInt32 speed =0;
							if (currSkill->mControlSpeed > iter->mValue->getHeroTemplate().mMoveSpeed)
							{
								speed = currSkill->mControlSpeed;
							}
							else
							{
								speed = iter->mValue->getHeroTemplate().mMoveSpeed;
							}
							iter->mValue->heroRightMove(speed);
						}
					}
					else if (iter->mValue->getNowState() == CHAR_STATE_JUMP)
					{
						if (iter->mValue->getCanRealMove() == true )
						{
							iter->mValue->heroRightMove(iter->mValue->getHeroTemplate().mMoveSpeed);
						}
					}
					else
					{
						if (iter->mValue->getNowState() != CHAR_STATE_RUN)
						{
							playAnimation(CHAR_STATE_RUN);
						}
						iter->mValue->setLastState(iter->mValue->getNowState());
						iter->mValue->setNowState(CHAR_STATE_RUN);

						if (iter->mValue->getCanRealMove() == true )
						{
							iter->mValue->heroRightMove(iter->mValue->getHeroTemplate().mMoveSpeed);
						}
					}
					
					if(iter->mValue->getCanRealMove() == true)
					{
						CharacterData charData;
						resp.typeID = msg.typeID;
						resp.result = 1;
						charData.canRealMove = true;
						charData.eNowState = CHAR_STATE_RUN;
						resp.charDatas.insertTail(charData);

						jsonStr = resp.convertDataToJson();
						senderSceneAll(PVP_SYNC_RESP,jsonStr);

					}
					else
					{
						resp.typeID = msg.typeID;
						resp.result = 2;
						jsonStr = resp.convertDataToJson();
						NetworkSystem::getSingleton().sender(msg.connId,1,jsonStr);
					}
				}
				else if (nameVal.value == JOYSTICKDIR_left)
				{
					iter->mValue->setCanRealMove(true);
					iter->mValue->setjoystick_updown(0);
					if (iter->mValue->checkCanTurn() == true)
					{
						iter->mValue->turnFace(false);
					}
					if (iter->mValue->getallowTurnFaceCommand() == true)
					{
						bool faceR = iter->mValue->getFaceR();
						if (faceR == true)
						{
							iter->mValue->setallowTurnFaceCommand(false);
							iter->mValue->sethaveRecvTurnFaceCommand(false);
							iter->mValue->turnFace(false);
						}
						return;
					}
					if (iter->mValue->checkCanTurn() == false)
					{
						iter->mValue->setCanRealMove(false);
					}
					if ( iter->mValue->getPosX() <=0 )
					{
						iter->mValue->setCanRealMove(false);
					}

					if (iter->mValue->getNowState() == CHAR_STATE_ATTACK && currSkill->mControl==1)
					{

						if (iter->mValue->getCanRealMove() == true )
						{
							iter->mValue->setCanSkillControl(true);
							UInt32 speed =0;
							if (currSkill->mControlSpeed > iter->mValue->getHeroTemplate().mMoveSpeed)
							{
								speed = currSkill->mControlSpeed;
							}
							else
							{
								speed = iter->mValue->getHeroTemplate().mMoveSpeed;
							}
							iter->mValue->heroLeftMove(speed);
						}
					}
					else if (iter->mValue->getNowState() == CHAR_STATE_JUMP)
					{
						if (iter->mValue->getCanRealMove() == true )
						{
							iter->mValue->heroLeftMove(iter->mValue->getHeroTemplate().mMoveSpeed);
						}
					}
					else
					{
						if (iter->mValue->getNowState() == CHAR_STATE_RUN)
						{
							playAnimation(CHAR_STATE_RUN);
						}					

						iter->mValue->setLastState(iter->mValue->getNowState());
						iter->mValue->setNowState(CHAR_STATE_RUN);
						if (iter->mValue->getCanRealMove() == true )
						{
							iter->mValue->heroLeftMove(iter->mValue->getHeroTemplate().mMoveSpeed);
						}
					}
				}
			}
		}

		else if (nameVal.name == ButtonClick_skill1)//技能
		{	
			UInt32 skillID;
			skillID = nameVal.value;
			if (skillID >4)
			{
				if(iter->mValue->checkSkill(skillID) == true )
				{
					iter->mValue->skillStart(skillID);
				}
			}
			else if (nameVal.value == ButtonClick_attack)//normal_attack
			{
				if (iter->mValue->getOnGroundFlag() == true)
				{
					skillID = SkillEquip_normal_attack;
				}
				else
				{
					skillID = SkillEquip_jump_attack;
				}
				if (iter->mValue->getjoystick_updown() == 1)
				{
					skillID = SkillEquip_up;
				}

				if(iter->mValue->checkSkill(skillID) == true )
				{
					if (iter->mValue->getjoystick_updown() == 0)
					{
						if (iter->mValue->getallowTurnFaceCommand() == true)
						{
							iter->mValue->sethaveRecvTurnFaceCommand(true);
							if (iter->mValue->getAttackFlag() == true)
							{
								return;
							}
						}
					}
					
						iter->mValue->skillStart(skillID);
						changeAttackBtnScale();
					
				}

			}
			else if (nameVal.value == ButtonClick_jump)//jump	
			{
				if (iter->mValue->checkCanJump() == true)
				{
					iter->mValue->setLastState(iter->mValue->getNowState());
					iter->mValue->setNowState(CHAR_STATE_JUMP);
					playAnimation(CHAR_STATE_JUMP);
				}

			}
			else if (nameVal.value ==  ButtonClick_pause)//stop
			{

				iter->mValue->setjoystick_updown(0);
				if (iter->mValue->getNowState() == CHAR_STATE_RUN)
				{
					iter->mValue->toIdle();
				}

			}	
			break;
		}		
	}
}




void SceneManager::changeAttackBtnScale()
{

}

void SceneManager:: playAnimation(UInt32 aniType)
{

// 	UInt32 delaytime;
// 	UInt32 hurtAni;
// 	if (aniType == CHAR_STATE_IDLE )
// 	{
// 		stopAllActions();
// 		playIdle(aniType);
// 	}
// 	else if aniType ==CHAR_STATE_RUN)
// 	{
// 		playRun(aniType)
// 	}
// 	else if aniType == CHAR_STATE_JUMP )
// 	{
// 		playJump(aniType);
// 	}
// 	else if aniType == CHAR_STATE_DIE)
// 	{
// 		playDie(aniType);
// 	}
// 	else if aniType == CHAR_STATE_HURT)
// 	{
// 		playHurt(aniType ,delaytime, hurtAni,true);
// 	}
// 	else if aniType == CHAR_STATE_FALL)
// 	{
// 		playFall(aniType ,delaytime);
// 	}
// 	else if aniType == CHAR_STATE_VICTORY)
// 	{
// 		playVictory(aniType);
// 	}
// 	else if aniType == CHAR_STATE_ATTACK)
// 	{
// 		playAttack(aniType,delaytime);
// 	}
// 	else if aniType == CHAR_STATE_SHOW)
// 	{
// 		playShowStart(aniType,delaytime);
// 	}

}



UInt32 SceneManager::getScreenEdgePosX(bool bRight)
{

	//客户端自己处理
	return 0;
}

void SceneManager::rightMoveMapLayer(UInt32 playerID,UInt32 speed) 	//--地图右移
{

	m_SceneTempelete->parent;
	UInt32 mapNOde = m_SceneTempelete->parent.MapNode;

	Character character;
// 	character = SceneManager.get(playerID);
// 
// 	UInt32 playerx = character.getPos().x;

	// UInt32 mapX = m_SceneTempelete->map mapNode.bgNode.tiledMap:getPositionX()//todo 客户端自己处理

}

void SceneManager::leftMoveMapLayer(UInt32 playerID,UInt32 speed) 	//--地图右移
{

	m_SceneTempelete->parent;
	UInt32 mapNOde = m_SceneTempelete->parent.MapNode;

	Character character;
// 	character = SceneManager.get(playerID);
// 
// 	UInt32 playerx = character.getPos().x;

	// UInt32 mapX = m_SceneTempelete->map mapNode.bgNode.tiledMap:getPositionX()//todo 客户端自己处理

}

void SceneManager::lockScene(UInt32 x1,UInt32 x2)
{
//-- body
	m_SceneTempelete->isFire = true;
	m_SceneTempelete->x1 = x1;
	m_SceneTempelete->x2 = x2;

}

void SceneManager::unlockScene()
{
	m_SceneTempelete->isFire =false;
	m_SceneTempelete->x1 = 0;
	m_SceneTempelete->x2 =0;

}

void SceneManager::setChapterLabel(UInt32 x)
{

}



List<Guid> SceneManager::findAttackTargets( Guid playerID,bool isPlayer,UInt32 skillId,UInt32 Multiidx)
{

	List<Guid> targets ;
	Character character;
	Character *characterOther;

	Pos pos = character.getPos();
	Pos posOther;
	SkillMap skillMap;

	SkillBaseTemplate *skillBaseTemplate;
	for (Map<UInt32, SkillBaseTemplate*>::Iter *iter = skillMap.skillMap.begin();iter != NULL;iter = skillMap.skillMap.next(iter))
	{
		if (iter->mKey == skillId)
		{
			skillBaseTemplate = iter->mValue;
		}
	}

	for(Map<Guid,Character*>::Iter * iter = m_SceneTempelete->sceneCharacter.CharacterMap.begin();iter != NULL; iter = m_SceneTempelete->sceneCharacter.CharacterMap.next(iter))
	{	
		if (iter->mKey == playerID)
		{
			continue;
		}
		characterOther = iter->mValue;
		posOther = characterOther->getPos();
		if (posOther.x >= (pos.x- skillBaseTemplate->width) && posOther.x <= (pos.x + skillBaseTemplate->width)
			&& posOther.y >= (pos.y- skillBaseTemplate->height) && posOther.y <= (pos.y + skillBaseTemplate->height))
		{
			targets.insertTail(iter->mKey);
		}		
	}
	return targets;	
}


void SceneManager::addHit()
{

	// 	getPlayerFromSceneByUUID(playerID):addHit();
	m_SceneTempelete->watchdog_hitRecordTime = HIT_RECORD_MIN_TIME;
	m_SceneTempelete->watchdog_hitChangeTime =HIT_CHANGE_MAX_TIME;

}

Character *  SceneManager::getPlayerFromSceneByUUID(Guid playerID)
{
	Player *player;
	UInt32 sceneID;
	Character * character = NULL;
	player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	sceneID = player->getSceneID();
	for(Map<Guid,Character*>::Iter * it =m_SceneTempelete->sceneCharacter.CharacterMap.begin();it != NULL; it = m_SceneTempelete->sceneCharacter.CharacterMap.next(it))
	{
		if (it->mKey == playerID)
		{
			return it->mValue;
		}
	}
	return character;
}

Character *SceneManager::getMonsterFromSceneByUUID(Guid playerID)
{
	Character *character =NULL;
	return character;

}
void SceneManager::addMonsterDrop(SceneData sceneData)
{


}

void SceneManager::AddDrop(String dropType,UInt32 value,Pos pos,bool bFly)
{
	UInt32 drop_id = m_SceneTempelete->DropIdBase;
	
}

void SceneManager::RemoveDrop(UInt32 drop_id )
{

	for(List<DropContent>::Iter* iter = m_SceneTempelete->dropList.begin();iter != NULL;iter = m_SceneTempelete->dropList.next(iter))
	{
		if (iter->mValue.drop_id == drop_id)
		{
			m_SceneTempelete->dropList.erase(iter);
		}
	}
}

void SceneManager::senderSceneAll(UInt16 packetID,std::string jsonStr)
{

	Player *player;
	for(Map<Guid,Character*>::Iter * it =m_SceneTempelete->sceneCharacter.CharacterMap.begin();it != NULL; it = m_SceneTempelete->sceneCharacter.CharacterMap.next(it))
	{
		player = LogicSystem::getSingleton().getPlayerByGuid(it->mKey);
		if(player == NULL)
		{
			return;
		}
		NetworkSystem::getSingleton().sender(player->getConnId(),packetID,jsonStr);
	}
}

