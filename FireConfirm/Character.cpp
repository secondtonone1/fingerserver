
#include "BattleCalc.h"
#include "../LogicSystem.h"
#include "Character.h"
#include "Scene.h"
#include "math.h"
#include "PVPSystem.h"
using namespace Lynx;



Character::Character()
{
}
Character::~Character()
{
}

void Character::initCharacter(Guid playerID)			
{
	UInt32 id;
	UInt32 level;
	Player *player;

	setPosX(FIRST_POSX);
	setPosY(GRUOND_Y);

	player = LogicSystem::getSingleton().getPlayerByGuid(playerID);


	id = player->getPlayerModelID();
	level = player->getPlayerLeval();

	m_character.playerID  = player->getPlayerGuid();
	m_character.level = level;

	initLevelAttr(playerID,level);//角色初始属性与等级增加属性

	initSkill(playerID);
	initBuff(playerID);


	initEquipAttr(playerID);
	initFashionAttr(playerID);


	// 	self.TotalRecordList = UserDataManager.UM_TotalRecord


	// 	self:getCharacter():setHP(self:getCharacter():getAttr(GameDefine.CHAR_ATTR_KEY.MaxHP))
	// 	self:getCharacter():setMP(self:getCharacter():getAttr(GameDefine.CHAR_ATTR_KEY.MaxMP))
	// 	self:getCharacter():setSP(self:getCharacter():getAttr(GameDefine.CHAR_ATTR_KEY.MaxSP))
	// 
	LastP lastP;
	lastP.hp =0;
	lastP.mp =0;
	lastP.sp =0;
	lastP.maxhp =0;
	lastP.maxmp =0;
	lastP.maxsp =0;


	setNowState(CHAR_STATE_IDLE);
	setLastState(CHAR_STATE_IDLE);


	// 	self.updateRecoverHandler = scheduler.scheduleGlobal(handler(self,self.update_recover), 1)
	// 		self.updateHeightHandler = scheduler.scheduleUpdateGlobal(handler(self,self.update_height))

}

void Character::release()			
{
}

void Character::initAttr(UInt32 ids)
{
	CharacterData characterData;

}

float  getVal(Vector<String> strVector,UInt32 level)
{

	float tpVal = 0;
	if (atoi(strVector[0].c_str()) == GROW_TYPE_LINE)
	{
		tpVal = level * lynxAtof(strVector[1].c_str());
	}
	else if (atoi(strVector[0].c_str()) == GROW_TYPE_PARABOLIC)
	{
		tpVal = level*lynxAtof(strVector[1].c_str()) + level*level*lynxAtof(strVector[2].c_str());
	}
	return tpVal;
}

void Character::initLevelAttr(Guid playerID,UInt32 level)//角色 等级属性
{
//	Vector<String> strVector;
//	Player *player;
//	UInt32 modleID;
//
//	
//
//	player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
//	modleID = player->getPlayerModelID();
//
//// 	HeroTemplate heroTemplate ;
//// 	heroTemplate = *gHeroTable->get(modleID);
//// 
//// 	*m_character.heroTemplate = heroTemplate;//marksend
//
//	m_character.heroTemplate = *gHeroTable->get(modleID);//marksend
//
//
//	LevelGrowTemplate *levelGrowTemplate;
//	levelGrowTemplate = gLevelGrowTable->get(modleID);
//
//	lynxStrSplit(levelGrowTemplate->mMaxHP, ";", strVector, true);
//	m_character.heroTemplate.mMaxHP += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mMaxMP, ";", strVector, true);
//	m_character.heroTemplate.mMaxMP += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mMaxSP, ";", strVector, true);
//	m_character.heroTemplate.mMaxSP += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mPA, ";", strVector, true);
//	m_character.heroTemplate.mPA += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mPF, ";", strVector, true);
//	m_character.heroTemplate.mPF += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mMA, ";", strVector, true);
//	m_character.heroTemplate.mMA += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mMF, ";", strVector, true);
//	m_character.heroTemplate.mMF += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mMPRecover, ";", strVector, true);
//	m_character.heroTemplate.mMPRecover += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mSPRecover, ";", strVector, true);
//	m_character.heroTemplate.mSPRecover += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mAttSpeed, ";", strVector, true);
//	m_character.heroTemplate.mAttackSpeed += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mCombinationAtt, ";", strVector, true);
//	m_character.heroTemplate.mCombinationAtt += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mMoveSpeed, ";", strVector, true);
//	m_character.heroTemplate.mMoveSpeed += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mJumpHeight, ";", strVector, true);
//	m_character.heroTemplate.mJumpHeight += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mFlyTimes, ";", strVector, true);
//	m_character.heroTemplate.mFlyTimes += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mSPDamage, ";", strVector, true);
//	m_character.heroTemplate.mSPDamage += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mAttPush, ";", strVector, true);
//	m_character.heroTemplate.mAttPush += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mAttPushDef, ";", strVector, true);
//	m_character.heroTemplate.mAttPushDef += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mAttRaise, ";", strVector, true);
//	m_character.heroTemplate.mAttRaise += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mAttRaiseDef, ";", strVector, true);
//	m_character.heroTemplate.mAttRaiseDef += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mStiff, ";", strVector, true);
//	m_character.heroTemplate.mStiff += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mStiffDef, ";", strVector, true);
//	m_character.heroTemplate.mStiffDef += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mBaseCrit, ";", strVector, true);
//	m_character.heroTemplate.mBaseCrit += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mCritRate, ";", strVector, true);
//	m_character.heroTemplate.mCritRate += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mCritDef, ";", strVector, true);
//	m_character.heroTemplate.mCritDef += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mStunDef, ";", strVector, true);
//	m_character.heroTemplate.mStunDef += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mStunTimeDef, ";", strVector, true);
//	m_character.heroTemplate.mStunTimeDef += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mSlowDef, ";", strVector, true);
//	m_character.heroTemplate.mSlowDef += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mSlowTimeDef, ";", strVector, true);
//	m_character.heroTemplate.mSlowTimeDef += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mPoisonDef, ";", strVector, true);
//	m_character.heroTemplate.mPoisonDef += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mBloodDef, ";", strVector, true);
//	m_character.heroTemplate.mBloodDef += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mKODef, ";", strVector, true);
//	m_character.heroTemplate.mKODef += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mFloatVal, ";", strVector, true);
//	m_character.heroTemplate.mFloatVal += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mFall, ";", strVector, true);
//	m_character.heroTemplate.mFall += getVal(strVector,level);
//	strVector.clear();
//
//	lynxStrSplit(levelGrowTemplate->mFallDef, ";", strVector, true);
//	m_character.heroTemplate.mFallDef += getVal(strVector,level);
//	strVector.clear();

}

void Character::initSkill(UInt32 playerID)
{
	Player * player;
	SkillBaseTemplate *skillBaseTemplate;
	player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	PlayerSkillListData skillList = player->GetSkillList();
	for(List<SkillData>::Iter * iter = skillList.m_listSkills.begin();iter != NULL;iter = skillList.m_listSkills.next(iter))//marksend
	{		
		skillBaseTemplate = gSkillBaseTable->get(iter->mValue.m_nID);
		SkillLevelTemplate * skillLvTemplate = SKILLLEVEL_TABLE().reverseGetNextLv(iter->mValue.m_nID,iter->mValue.m_nLevel);

		m_character.skillBaseData.skillMap.insert(iter->mValue.m_nID,skillBaseTemplate);
	}

}


void Character::initBuff(UInt32 playerID)
{

	Player * player;
	BuffTemplate *buffTemplate;
	player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	PlayerBuffListData buffList = player->GetBufferData();
	for(List<BufferData>::Iter *iter = buffList.m_listBuffers.begin();iter != NULL;iter = buffList.m_listBuffers.next(iter))
	{
		buffTemplate = gBuffTable->get(iter->mValue.m_nBufferID);
		m_character.buffData.buffMap.insert(iter->mValue.m_nBufferID,buffTemplate);
	}

}

void Character::initEquipAttr(UInt32 playerID)
{
	Player *player;
	player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	PlayerItemListData itemListData;

	Map<UInt64, ItemData*> MapUidEquipType;/* = player->getItemManager().m_mapUidEquipType;*/
	// 	MapUidEquipType = player->getItemManager().m_mapUidEquipType;
	// 	for(Map<UInt64, ItemData*>::Iter * iter = MapUidEquipType->begin();iter != NULL;iter = MapUidEquipType->next(iter))
	{
		//todo 增加装备 韵文属性

	}

}
void Character::initFashionAttr(UInt32 playerID)
{
	Player *player;
	player = LogicSystem::getSingleton().getPlayerByGuid(playerID);
	PlayerFashionData mFashionData;
	FahionTemplate *fahionTemplate;

	mFashionData = player->GetFashionData();
	mFashionData.m_listFashionDatas;
	for(List<FashionData>::Iter * iter = mFashionData.m_listFashionDatas.begin();iter != NULL;iter = mFashionData.m_listFashionDatas.next(iter) )
	{
		fahionTemplate = gFashionTable->get(iter->mValue.m_nFahionID);//todo 属性加入 herotemplate

	}


}
void Character::update_skill()
{

}

void initEquipAttr(UInt32 playerID)
{

}
void initFashionAttr(UInt32 playerID)
{

}
void Character::updateHeight(UInt64 t)
{
	// 	if self==nil or actorInStage==nil then
	// 		return
	// 		end 
	// 
	// 		if display:getRunningScene().calc.isQTE == true 
	// 			and self:getClient():getCharacter():getCharType() ~= GameDefine.CHARACTER_TYPE.Player then
	// 			return
	// 			end   
	// 
	// 			if self.isBlock ==true then
	// 				-- self.isBlock = false
	// 				return
	// 				end
	// 	setPosX(0);//todo
	// 	setPosY(0);

	Int32 MyGravity =GRAVITY;
	if(getNowState() != CHAR_STATE_JUMP)
	{
		if (getNowState() != CHAR_STATE_FALL)
		{
			if(getNowState() == CHAR_STATE_ATTACK)
			{
				MyGravity = MyGravity * GravityDeclineRate;
			}
			else
			{
				return;
			}
		}
	}
	if (getOnGroundFlag() == true)
	{
		return;
	}
	double inv = (t - m_character.jumpStartTime);
	float rt = inv/1000;
	Int32 lastSpeed = getJumpSpeed();
	Int32 nowSpeed = lastSpeed + GRAVITY* rt;
	if (nowSpeed < 0)
	{
		nowSpeed = 0;
	}

	setJumpSpeed(nowSpeed);
	if (getGravityFlag() == true)
	{
		float distance = /*getJumpSpeed() * rt*/lastSpeed*rt + GRAVITY*rt*rt/2;
		UInt32 nowHeight = getPosY() + distance;
		if (nowHeight < GRUOND_Y)
		{
			nowHeight = GRUOND_Y;
		}
		if (getHaveAttachMaxYFlag() == false)
		{
			setJumpSpeed(getJumpSpeed() + MyGravity * rt);
			if (getJumpSpeed() <= 0)
			{
				setHaveAttachMaxYFlag(true) ;	
				setJumpSpeed(0) ;
				if (getFallFlag() == false &&getAttackFlag() == false)
				{
					playAnimation(CHAR_STATE_JUMP,0);
				}
			}
			else if (getAttackFlag() == true)
			{
				setHaveAttachMaxYFlag(true);
				setJumpSpeed(0);
			}
			setPosY(nowHeight);
			refreshPosition(nowHeight);
		}
		else
		{
			if (getAttackFlag() == true)
			{
				if (getSkillMoveYFlag() == false)
				{
					setJumpSpeed(0);
					setSkillMoveYFlag(true);	
					distance = 0;
				}
			}
			setPosY(getPosY()+distance);
			if (getPosY() <= GRUOND_Y)
			{
				setPosY(GRUOND_Y);
				refreshPosition(nowHeight);					
				onJumpEnd();
				if (getFallFlag() == false&&getAttackFlag() == false)
				{
					playAnimation(CHAR_STATE_JUMP,0);
				}
			}
			refreshPosition(getPosY() + distance);
			setJumpSpeed((getJumpSpeed() + MyGravity * rt));
		}
		if(CHAR_SHADOW ==1)
		{
			// 			shadowNode = m_character.shadowNode;
			UInt32 dis = GRUOND_Y-getPosY();
			if ( dis > CHAR_SHADOW_CONFIG_OFFSETY)
			{
				dis = CHAR_SHADOW_CONFIG_OFFSETY;
			}
			// 			setPositionY(dis);
		}
	}

}

void Character::refreshPosition(UInt32 val)
{
	// 	setPositionY(self:getPosY())
}

void Character::update_recover(UInt32 t)
{
	checkIsDie();
}

UInt32 Character::getMoveSpeed()
{	
	return getHeroTemplate().mMoveSpeed;
}

bool Character::checkCanRun()
{
	if (getBuffStateByIdx(BUFF_STATE_SUB_TYPE_VERTIGO)== true)
	{
		return false;
	}
	if (getNowState() == CHAR_STATE_IDLE)
	{
		return true;
	}
	else if (getNowState() == CHAR_STATE_RUN)
	{
		return true;
	}
	else if (getNowState() == CHAR_STATE_JUMP)
	{
		return true;
	}
	else if (getNowState() == CHAR_STATE_ATTACK)
	{
		// 		SkillData currSkill = getSkillById(getCurSkillID());
		// 		currSkill.
		// 
		// 		local currSkill = self:getSkillById(self:getCurSkillID())
		// 			if currSkill.skillBaseData.canTurnFace == true then
		// 				return true
		// 				end
		// 				if currSkill:isNowAllowPlayerInterruptSkill() == true then
		// 					return true
	}
	return false;

}



bool Character::checkCanTurn()
{
	if (getNowState() == CHAR_STATE_IDLE)
	{
		return true;
	}
	else if (getNowState() == CHAR_STATE_RUN)
	{
		return true;
	}
	else if (getNowState() == CHAR_STATE_JUMP)
	{
		return true;
	}
	else if (getNowState() == CHAR_STATE_ATTACK)
	{	
		if (getSkillBaseById(getCurSkillID())->mCanTurnFace == 1)
		{
			return true;
		}
		if (isNowAllowPlayerInterruptSkill(getCurSkillID()) == true)
		{
			return true;
		}
	}
	return false;
}


bool Character::checkCanJump()
{
	if (getBuffStateByIdx(BUFF_STATE_SUB_TYPE_VERTIGO)== true)
	{
		return false;
	}
	if (getNowState() == CHAR_STATE_IDLE )
	{
		return true;
	}
	else if (getNowState() == CHAR_STATE_RUN)
	{
		return true;
	}
	return false;
}

bool Character::checkIsDie()
{
	if (getHP() >0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Character::checkNeedPlayHurtAni()
{
	if (getSP()  >0)
	{
		return false;
	}
	else
	{
		return true;
	}
	return true;
}


bool Character::checkNeedExplode()//玩家不会被击碎
{
	return false;

}
bool Character::checkSkill(UInt32 skillId)
{
	if (getBuffStateByIdx(BUFF_STATE_SUB_TYPE_VERTIGO) == true )
	{
		return false;
	}

	if (getNowState() == CHAR_STATE_DIE )
	{
		return false;
	}
	else if (getNowState() == CHAR_STATE_FALL )
	{
		return false;
	}
	else if (getNowState() == CHAR_STATE_JUMP )
	{	
		if (skillId != SkillEquip_jump_attack &&getOnGroundFlag()==false)//todo
		{
			return false;
		}		
	}

	UInt32 lastSkillId =  getCurSkillID();
	if (lastSkillId != SkillEquip_normal_attack&& lastSkillId != SkillEquip_jump_attack)
	{
		if(isNowAllowPlayerInterruptSkill(skillId) == false)
		{
			return false;
		}
	}

	SkillLevelTemplate* skillLevelTemplate = getSkillLevelTemplate(skillId);
	if (skillLevelTemplate == NULL)
	{
		return false;
	}

	if (getMP() < skillLevelTemplate->mMP1)
	{
		return false;
	}

	if (getSkillCD(skillId) == false)
	{
		return false;
	}
	return true;
}
bool Character::getSkillCD(UInt32 skillID)
{

	SkillLevelTemplate* skillLevelTemplate = getSkillLevelTemplate(skillID);
	SkillData skillData =  getskillDatabyID(skillID);
	if (skillData.m_nCD == 0)
	{
		return true;
	}
	if (((skillData.m_nCD - TimeUtil::getTimeMilliSec()) - skillLevelTemplate->mCD1)>0)
	{
		return false;
	}
	return true;
}
void Character::setSkillCD(UInt32 skillID,UInt64 startTime)
{
	for(List<SkillData>::Iter * iterSkill = m_character.skillData.m_listSkills.begin();iterSkill!= NULL;iterSkill = m_character.skillData.m_listSkills.next(iterSkill) )
	{
		if (iterSkill->mValue.m_nID == skillID)
		{
			iterSkill->mValue.m_nCD = startTime;
		}
	}
}
SkillData Character::getskillDatabyID(UInt32 skillID)
{

	SkillData skillData;
	for(List<SkillData>::Iter * iterSkill = m_character.skillData.m_listSkills.begin();iterSkill!= NULL;iterSkill = m_character.skillData.m_listSkills.next(iterSkill) )
	{
		if (iterSkill->mValue.m_nID == skillID)
		{
			return iterSkill->mValue;
		}
	}
	return skillData;
}
void Character::changeHP(Int32 val)
{
	setHP((getHP() + val));
	if (getHP()<0)
	{
		setHP(0);
	}
	if ( getHP() > m_character.lastP.maxhp)
	{
		setHP(m_character.lastP.maxhp);
	}	
}


void Character::changeMP(Int32 val)
{
	setMP(getMP() + val);
	if (getMP()<0)
	{
		setMP(0);
	}
	if ( getMP() > m_character.lastP.maxmp)
	{
		setMP(m_character.lastP.maxmp);
	}
}
void Character::changeSP(Int32 val)
{
	if(getBuffStateByIdx(BUFF_STATE_SUB_TYPE_ENDURE) == true)
	{
		return;
	}
	if(getBuffStateByIdx(BUFF_STATE_SUB_TYPE_GOD) == true)
	{
		return;
	}

	setSP(getSP() + val);
	if (getSP()<0)
	{
		setSP(0);
	}
	if ( getSP() > m_character.lastP.maxsp)
	{
		setSP(m_character.lastP.maxsp);
	}
}




void Character::playHurtEffect(Damages damages)
{
	UInt32 hurtEffectId = 0;

}
void Character::addBuff(UInt32 val)
{

}

void Character::addHit()
{
	m_character.hit++;//todo
	if (m_character.hit > m_character.totalRecordList.maxhit)
	{
		m_character.totalRecordList.maxhit = m_character.hit;
	}
}

void Character::onHurt(Guid attackerID,Damages damages,Pos p)
{
	if( getCanBeAttack() == false )
	{
		return;
	}
	if (getBuffStateByIdx(BUFF_STATE_SUB_TYPE_GOD)== true)
	{
		return;
	}
	bool isHasSp = getSP()>0;
	addHit();

	bool alreadyDie = false;
	if (checkIsDie() == true )
	{		
		alreadyDie = true;		
	}

	UInt32 lastHp =getHP();


	// 	playEffectBlood(damages);
	changeHP(damages.damageval * (-1));
	changeSP(damages.spcost);

	// 	if self.son.uuid ~= nil and self.son.monsterType == GameDefine.MonsterType.boss then
	// 		self.server.parent.UiNode:setBossBlood(self.son.uuid)
	// 		elseif self.son.monsterType == GameDefine.MonsterType.monster then
	// self:getClient():setBloodProgress()
	// 	 end
	playHurtEffect(damages);

	// 	 damages.addBuff = checktable(damages.addBuff);
	for (List<UInt32>::Iter * iter = damages.addBuff.begin();iter != NULL;iter = damages.addBuff.next(iter))
	{
		addBuff(iter->mValue);
	}


	BufferData bufferData;
	for (List<UInt32>::Iter *it = damages.buff.begin();it != NULL;it = damages.buff.next(it))
	{
		addBuff(it->mValue, bufferData,1111111);
	}
	stopPreferPushMoveX();


	if (checkNeedExplode() == true) 
	{
		// 		display:getRunningScene().calc.rhymePower = display:getRunningScene().calc.rhymePower + self.son.RhymeEnergy
		// 		self:setCanBeAttack(false)
		// 		self:getClient():getCharacter():playExplode()
		// 			 if alreadyDie == false then
		// 				 self.son:dropAward()
		// 				 end
		// 				 return
		// 				 end

	}

	if (checkIsDie() == true )
	{
		if (alreadyDie == false )
		{
			// 			changeCharacterColorToGray();
			// 			dropAward();
		}
	}

	if (damages.fly == true&& isHasSp !=true )
	{
		bool dirR = true;
		if (p.x > getPosX())
		{
			dirR = false;
		}
		else if (getPosX() >getPosX())
		{
			dirR = false;
		}
		flyToWall(dirR);
		return;
	}
	UInt32 pushMoveXTimeByFall = 0;

	if(checkIsDie() == true)
	{
		setAttackFlag(false);
		if(getOnGroundFlag() == false)
		{
			if (damages.raise > 0 || (getKeepFallCnt() <= m_character.heroTemplate.mFlyTimes))
		 {
			 pushMoveXTimeByFall = fall(damages.raise, damages.stiff);
			 setKeepFallCnt(getKeepFallCnt() + 1);
		 }
			else
		 {
			 pushMoveXTimeByFall = fall(0, damages.stiff);
			}	
		}
		else 
		{
			die(false);
		}

	}
	else if (checkNeedPlayHurtAni() == true)
	{
		setAttackFlag(false);

		if (damages.raise > 0)
		{
			if (getKeepFallCnt() <= m_character.heroTemplate.mFlyTimes )
		 {
			 pushMoveXTimeByFall =fall(damages.raise, damages.stiff);
			 setKeepFallCnt(getKeepFallCnt() + 1);
		 }
			else		
		 {
			 pushMoveXTimeByFall = fall(0, damages.stiff);		
		 }
		}
		else if (damages.fall == true)
		{
			pushMoveXTimeByFall = fall(0, damages.stiff);
		}
		else
		{
			setLastState(getNowState());
			setNowState(CHAR_STATE_HURT);
			if(getOnGroundFlag() == false)
		 {
			 setJumpSpeed(HURT_AIR_FALL_SPEED);
		 }
			// 		 self:getClient():playAnimation(GameDefine.CHAR_STATE.hurt,damages.stiff, damages.hurtAni)

		}

		// 		local character = self:getClient():getCharacter()
		// 			if character:getCharType() == GameDefine.CHARACTER_TYPE.Player then
		// 				self.server.parent.UiNode:shakeBlood(GameDefine.CHARACTER_TYPE.Player)
		// 				elseif character:getCharType() == GameDefine.CHARACTER_TYPE.Monster then
		// 				if self.son.monsterType == GameDefine.MonsterType.boss then
		// 					self.server.parent.UiNode:shakeBlood(GameDefine.CHARACTER_TYPE.Monster)
		// 					end
		// 					end

	}

	if (isHasSp == true)
	{
		return;
	}
	UInt32 pushMoveTime =  HURT_PUSH_MOVE_TIME;
	if (pushMoveXTimeByFall > 0)
	{
		pushMoveTime = pushMoveXTimeByFall * 2;
	}


}

UInt32 Character::getKeepFallCnt()
{
	return 1;

}
void Character::setKeepFallCnt(UInt32 val)
{
	return ;

}

void Character::stopPreferPushMoveX()
{

}

void Character::flyToWall(UInt32 dirIsRight)
{
	setFallFlag(true);
	setFlyingState(CHARACTER_FLYING_STATE_startfly);

	turnFace( dirIsRight);//todo

	setLastState(getNowState());
	setNowState(CHAR_STATE_FALL);
	// 	getClient():playAnimation(CHAR_STATE.fall);

	setHaveAttachMaxYFlag(true);
	setOnGroundFlag(true);
	setJumpSpeed(0);
	setSkillMoveYFlag(false);
	setCanBeAttack(false);

}
void Character::turnFace(bool faceR)
{
	setFaceR(true);

}

UInt32 Character::fall(UInt32 height,UInt32 delay)
{
	setFallFlag(true);

	UInt32 t = 0;

	if (height > 0) 
	{
		if (m_character.bGravity == 0)
		{
			return 0;
		}
		onJumpStart(height);
	}
	else
	{
		onJumpStart(1);
	}
	setLastState(getNowState());
	setNowState(CHAR_STATE_FALL);
	// self:getClient():playAnimation(GameDefine.CHAR_STATE.fall, delay)

	double dtmp = 2*height/(m_character.bGravity*(-1));
	t = sqrt(dtmp);
	return t;

}

void Character::die(bool flag)
{
	if (getLastState() == CHAR_STATE_DIE )
	{
		return;
	}

	setLastState(getNowState());
	setNowState(CHAR_STATE_DIE);
	if (flag != true)
	{
		playAnimation(CHAR_STATE_DIE,0);
	}

	removeAllBuff();
	m_character.deadTimes++;
	setPosY(0);
	//sendclient
}


// 
void Character::victory()
{
	if (getLastState() == CHAR_STATE_DIE || getLastState() == CHAR_STATE_VICTORY)
	{
		return;
	}

	setLastState( getNowState());
	setNowState(CHAR_STATE_VICTORY);
	//sendclient
	// 	getClient():playAnimation(GameDefine.CHAR_STATE.victory)
}

SkillBaseTemplate *Character::getSkillBaseById( UInt32 skillId)
{
	SkillBaseTemplate *skillBaseTemplate = NULL;
	if ( (skillId <= 0))
	{
		return skillBaseTemplate;
	}

	for(Map<UInt32, SkillBaseTemplate*>::Iter *iter = m_character.skillBaseData.skillMap.begin();iter != NULL;iter = m_character.skillBaseData.skillMap.next(iter))
	{
		if ( iter->mValue->mSkillID == skillId)
		{
			return iter->mValue;
		}

	}
	return skillBaseTemplate;
}

UInt32 Character::getSkillIdByType(UInt32 skillType)
{
	return 1;
}

UInt32 Character::getSkillCnt()
{
	return m_character.skillBaseData.skillMap.size();
}

bool Character::addSkill(UInt32 skillId,SkillData skillData)
{
	if ( (skillId <= 0))
	{
		return false;
	}
	SkillBaseTemplate *skillBaseTemplate;

	for(Map<UInt32, SkillBaseTemplate*>::Iter *iter = m_character.skillBaseData.skillMap.begin();iter != NULL;iter = m_character.skillBaseData.skillMap.next(iter))
	{
		if ( iter->mKey == skillId)
		{
			m_character.skillBaseData.skillMap.erase(iter);
		}

	}
	m_character.skillBaseData.skillMap.insert(skillId,skillBaseTemplate);

	onChangeSkill(skillId, true);
	return true;

}

bool Character::removeSkill(UInt32 skillId)
{
	if ( (skillId <= 0))
	{
		return false;
	}

	for(Map<UInt32, SkillBaseTemplate*>::Iter *iter = m_character.skillBaseData.skillMap.begin();iter != NULL;iter = m_character.skillBaseData.skillMap.next(iter))
	{
		if ( iter->mKey == skillId)
		{
			m_character.skillBaseData.skillMap.erase(iter);
			return true;
		}
	}

	return false;
}

void Character::onChangeSkill(UInt32 skillId,UInt32 bIsAdd)
{
}

void Character::onSkillInterrupt(UInt32 skillID)
{
	stopAnimation(skillID);

}

void Character::stopAnimation(UInt32 skillID)
{

	// 	self.curAniFrameIdx = 0

	onSkillFinish(skillID);
}

void Character::onSkillFinish(UInt32 skillID)
{
	setAttackFlag(false);
	skillEnd(skillID);
}

void Character::skillStart(UInt32 skillId)
{
	UInt32 lastSkillID = getCurSkillID();
	if (lastSkillID > 0) 
	{
		if (lastSkillID != skillId )
		{
// 			SkillBaseTemplate *lastTSkill = getSkillBaseById(lastSkillID);		
			onSkillInterrupt(lastSkillID);
		}  
		else
		{
			return;
		}				
	}
	setCurSkillID(skillId);
	setSkillBeginTime(TimeUtil::getTimeMilliSec());

	setCurMultiIdx(1);
	setAttackFlag(true);

	setLastState(getNowState());
	setNowState(CHAR_STATE_ATTACK);

	changeMP((getSkillMP(skillId)*(-1)));
	playAnimation(CHAR_STATE_ATTACK,0);
	onSkillStart();
}
UInt32 Character::getSkillMP(UInt32 skillId)
{
	for(List<SkillData>::Iter * iterSkill = m_character.skillData.m_listSkills.begin();iterSkill!= NULL;iterSkill = m_character.skillData.m_listSkills.next(iterSkill) )
	{
		if (iterSkill->mValue.m_nID == skillId)
		{
			SkillLevelTemplate * skillNextLvTemplate = SKILLLEVEL_TABLE().reverseGetNextLv(skillId,iterSkill->mValue.m_nLevel);
			if (skillNextLvTemplate == NULL)
			{
				return 0;
			}
			return skillNextLvTemplate->mMP1;
		}		
	}
	return 0;
}
SkillLevelTemplate *Character::getSkillLevelTemplate(UInt32 skillId)
{
	for(List<SkillData>::Iter * iterSkill = m_character.skillData.m_listSkills.begin();iterSkill!= NULL;iterSkill = m_character.skillData.m_listSkills.next(iterSkill) )
	{
		if (iterSkill->mValue.m_nID == skillId)
		{
			return SKILLLEVEL_TABLE().reverseGetNextLv(skillId,iterSkill->mValue.m_nLevel);
		}
	}
	return NULL;

}

void Character::skillEnd(Guid playerID)
{
	setCurSkillID(0);
	setCurMultiIdx(0);

	AniComplete(CHAR_STATE_ATTACK,0);
}

void Character::AniComplete(UInt32 aniType,UInt32 id)
{
	if (aniType == CHAR_STATE_DIE )
	{
		onDead();
	}

	else if (aniType == CHAR_STATE_FALL)
	{
		// --Do nothing, don't to idle
	}

	else if( aniType == CHAR_STATE_HURT )
	{
		// 	playHurtRecover(aniType);
	}
	else 
	{
		toIdle();
	}

	return;
}

UInt32 Character::getTargetsCnt()
{
	return 1;
}


void Character::addToTargets(Guid playerID)
{
	return;
}

void Character::cleanTargets()
{

}

Area  rectIntersection(Area attRect,Area defRect)
{
	Area damageRect;
	return damageRect;
}
bool rectIntersectsRect(Area rect1,Area rect2 )
{


	if( rect1.x > rect2.x + rect2.w &&
					   rect1.x + rect1.w < rect2.x  &&
					   rect1.y > rect2.y + rect2.h   &&
					   rect1.y + rect1.h < rect2.y )
	{
		return true;
	}
	return false;

}
bool Character::skillDamage(Character *defencer, AttParams attParams)
{
	//--check actor is dead and on ground 
	if (getNowState() == CHAR_STATE_DIE && getOnGroundFlag()==true) 
	{
		return false;
	}

	Area attRect = getScreenAttackArea(attParams.attackArea);
	Area defRect = getScreenDefenceArea();
	if (rectIntersectsRect(attRect, defRect) == false )
	{
		return false;
	}

	Damages damages = BattleCalc::getSingleton().calcDamage(*this, *defencer, attParams);
	// if (damages == NULL)
	// {
	// 	return false;
	// }


	//--damage rect
	Area damageRect = rectIntersection(attRect, defRect);

	damages.hurtPoint.x = damageRect.x+damageRect.w/2;
	damages.hurtPoint.y = damageRect.y+damageRect.h/2;

	if( (defRect.y+defRect.h/2) <= damages.hurtPoint.y )
	{
		damages.hurtAni = 1;
	}
	else
	{
		damages.hurtAni = 0;
	}

	// -- damages.hurtPoint.x = damageRect.x + damageRect.width/2 - defRect.x
	// -- damages.hurtPoint.x = damageRect.x + damageRect.width/2 - defRect.x
	// -- if self:getClient():getCharacter():getFaceR() == false then
	// --     damages.hurtPoint.x = defRect.width - damages.hurtPoint.x
	// -- end
	// -- damages.hurtPoint.y = damageRect.y + damageRect.height/2 - defRect.y

	damages.addBuff =attParams.addBuff;
	Pos p;
	Guid playerID =0;
	onHurt( playerID, damages,p);

	return true;
}

bool Character::EventAttack(AttParams params,bool isKeep)
{

	AttParams attParams;
	attParams.damage = params.damage;	// {dType=0, dValue=100, hurtType=1}    dType: 0:法术伤害, 1:物理伤害
	attParams.attackArea = params.attackArea; // {x=0, y=0, w=0, h=0}
	attParams.attrSkillAdd = params.attrSkillAdd; //	{["GameDefine.CHAR_ATTR.AttPush"] = {affectType=1, affectValue=100}, ["GameDefine.CHAR_ATTR.AttRaise"] = {affectType=1, affectValue=100}, ...}
	attParams.addBuff = params.addBuff;	//{1, 2}
	attParams.notstand = params.notstand;		// 0:无效果, 1:浮空, 2:击倒, 3:击飞 
	if (isKeep == true )
	{
		if ((params.isAttacksBegin == 0))
		{
			attParams.isAttacksBegin = false ;
		}
		else if (params.isAttacksBegin == 1)
		{
			attParams.isAttacksBegin = true;
		}

	}

	m_character.hitCnt = skillAttack(attParams, isKeep);
	return true;
}

bool Character::skillAttack(AttParams attParams, UInt32 isAttacks)
{
	// 		--统计需要击打的数量
	UInt32 hitCharCnt = 0;
	Player *player;
	SceneManager* sceneManager ;

	player = LogicSystem::getSingleton().getPlayerByGuid(m_character.playerID);
	UInt32 sceneID = player->getSceneID();
	
	sceneManager = PVPSystem::getSingleton().getScene(sceneID);

	for(Map<Guid,Character*>::Iter *iter = sceneManager->m_SceneTempelete->sceneCharacter.CharacterMap.begin();iter != NULL;iter= sceneManager->m_SceneTempelete->sceneCharacter.CharacterMap.next(iter))
	{
		if (iter->mKey != m_character.playerID)
		{
			if (BattleCalc::getSingleton().checkCanAttackByCampID(*this, *iter->mValue) == true)
			{
				if (isAttacks == false)
				{
					if (skillDamage(iter->mValue,attParams) == true)
					{
						hitCharCnt ++;
					}
				}
				else
				{
					if (attParams.isAttacksBegin == 1)
					{
						cleanTargets();

					}
					if (BattleCalc::getSingleton().checkInTargets(iter->mValue->m_character.playerID) == false )
					{
						if (skillDamage(iter->mValue,attParams) == true)
						{
							hitCharCnt ++;
							addToTargets(iter->mValue->m_character.playerID);
						}
					}
				}
			}
		}
	}
	return hitCharCnt;
}

Area Character::getScreenAttackArea(Area rectData)
{
	Area attackRect ;
	Area rect = rectData;
	Pos posInMap ;
	posInMap.x = getPosX();
	posInMap.y = getPosY();

	if (getFaceR() == true)
	{
		attackRect.x = posInMap.x + rect.x;
		attackRect.y = posInMap.y + rect.y;
		attackRect.w = rect.w;
		attackRect.h = rect.h;
	}
	else
	{
		attackRect.x = posInMap.x -( rect.x + rect.w );
		attackRect.y = posInMap.y + rect.y;
		attackRect.w = rect.w;
		attackRect.h = rect.h;
	}
	// self:getClient():getCharacter():showAttackAreaForDebug(cc.rect(rect.x,rect.y,rect.width,rect.height))
	return attackRect;
}

Area Character::getScreenDefenceArea()
{
	Area defenceRect;
	Area rect ;
	if (getFallFlag() == true )
	{
		rect = getDefenceAreaFall();
	}
	else
	{
		rect = getDefenceAreaStand();
	}

	Pos posInMap ;
	posInMap.x = getPosX();
	posInMap.y = getPosY();
	 		
	if (getFaceR() == true )
	{
		defenceRect.x = posInMap.x + rect.x;
		defenceRect.y = posInMap.y + rect.y;
		defenceRect.w = rect.w;
		defenceRect.h = rect.h;
	}
	else
	{
		defenceRect.x = posInMap.x -( rect.x +rect.w);
		defenceRect.y = posInMap.y + rect.y;
		defenceRect.w = rect.w;
		defenceRect.h = rect.h;
	}

	// self:getClient():getCharacter():showBeattackedAreaForDebug(cc.rect(rect.x,rect.y,rect.w,rect.h))
	return defenceRect;
}

BuffTemplate* Character::getBuffByID(UInt32 buffId )
{
	BuffTemplate *buffTemplate = NULL;


	for(Map<UInt32, BuffTemplate*>::Iter * iter = m_character.buffData.buffMap.begin();iter != NULL; iter = m_character.buffData.buffMap.next(iter))
	{
		if (iter->mKey == buffId)
		{
			return iter->mValue;
		}
	}
	return buffTemplate;
}

UInt32 Character::getBuffCnt()
{
	return m_character.buffData.buffMap.mIterCount;
}

UInt32 Character::getBuffIdByType(UInt32 buffbigType,UInt32 buffSubType)
{

	BuffType buffType;
	for(Map<UInt32, BuffTemplate*>::Iter * iter = m_character.buffData.buffMap.begin();iter != NULL;iter = m_character.buffData.buffMap.next(iter) )
	{

		buffType = getBuffTypeByID(iter->mKey);

		if (buffType.buffBigType == buffbigType && buffType.buffSubType == buffSubType)
		{
			return iter->mKey;
		}
	}
	return 0;
}

BuffType Character::getBuffTypeByID(UInt32 buffId)
{
	BuffTemplate *buffTemplate;
	buffTemplate = gBuffTable->get(buffId);
	BuffType buffType;
	buffType.buffBigType = buffTemplate->mType;
	buffType.buffSubType = buffTemplate->mSubType;
	return buffType;

}

bool Character::addBuff(UInt32 buffId,BufferData buffObj,Int32 keepTime)
{
	if (buffId <= 0)
	{
		return false;
	}
	BuffType bufferType;
	bufferType = getBuffTypeByID(buffId);
	// 	if (bufferType == NULL)
	// 	{
	// 		return false;
	// 	}

	if (getBuffIdByType(BUFF_TYPE_STATE,BUFF_STATE_SUB_TYPE_GOD) )
	{
		return false;
	}
	return true;

}

void Character::removeBuff(UInt32 buffId )
{
	if (buffId <= 0)
	{
		return;
	}
	for(Map<UInt32, BuffTemplate*>::Iter * iter = m_character.buffData.buffMap.begin();iter != NULL;iter = m_character.buffData.buffMap.next(iter) )
	{
		if (iter->mKey == buffId)
		{
			// 			self.buffList[buffId]:onBuffEnd()
			m_character.buffData.buffMap.erase(iter);
			onChangeBuff(buffId, false);
		}
	}	
}

void Character::removeAllBuff()
{
	for(Map<UInt32, BuffTemplate*>::Iter * iter = m_character.buffData.buffMap.begin();iter != NULL;iter = m_character.buffData.buffMap.next(iter) )
	{
		removeBuff(iter->mKey);
		iter = m_character.buffData.buffMap.prev(iter);
	}
}

void Character::onChangeBuff(UInt32 buffId,UInt32 bIsAdd)
{
	return;
}

void Character::onJumpStart(UInt32 height)
{

	setGravityFlag(true);
	setNowState( CHAR_STATE_JUMP);
	setOnGroundFlag(false);

	float dTmp =((float) height * 2 ) / (GRAVITY*(-1));	
	float t = sqrt(dTmp);	
	setJumpSpeed(t * GRAVITY * (-1));//v=at=sqrt(2as)
	m_character.jumpStartTime = TimeUtil::getTimeMilliSec();

}

void Character::onJumpEnd()
{
	if(getFallFlag() == true)
	{
		setCanBeAttack(false);
		if (getJumpSpeed() < BOUNCE_MIN_SPEED)
		{
			setJumpSpeed(getJumpSpeed() * BOUNCE_DECREASE_RATE * (-1));
			setHaveAttachMaxYFlag(false);
			return;
		}
	}

	setKeepFallCnt(0);
	setOnGroundFlag(true);
	setJumpSpeed(0);
	setSkillMoveYFlag(false);
}

void Character::fallGroundDelay( UInt32 t)
{
	// 	if( m_character.isQTE == true)
	// 	{
	// 		return
	// 	}
	setFallFlag(false);
	if (checkIsDie() == false)
	{
		bool  bIsFly = false;
		if (getFlyingState() > CHARACTER_FLYING_STATE_none )
		{
			bIsFly = true;
		}
		// 		playStandup(bIsFly);
		// 		addBuff(5);//, nil, GameConfig.FALL_TO_UP_TIME
	}
	else
	{
		die(true);
		onDead();
	}


}

void Character::onDead()
{
	// 	m_character.deadTimes;


	revive();
	//layer:init(params)//显示用
}

void Character::toIdle()
{

	if (getOnGroundFlag() == true)
	{
		setHaveAttachMaxYFlag(false);
		setCurSkillID(0);

		setLastState(getNowState());
		setNowState(CHAR_STATE_IDLE);
		setAttackFlag(false);
		setFlyingState(CHARACTER_FLYING_STATE_none);
		setCanBeAttack(true);
		// 		playanimation(CHAR_STATE_IDLE);//todo
	}
	else
	{
		if (getNowState() == CHAR_STATE_FALL)
		{
			return;
		}
		
			setAttackFlag(false);
			setFlyingState(CHARACTER_FLYING_STATE_none);
			setCurSkillID(0);

			setLastState(getNowState());
			setNowState(CHAR_STATE_JUMP);
			setCanBeAttack(true);

			setHaveAttachMaxYFlag(true);
			setJumpSpeed(0);
			// 			playAnimation(GameDefine.CHAR_STATE.jump);

	}
}

void Character::revive()
{
	setNowState(CHAR_STATE_IDLE);
	toIdle();
	setHP(m_character.lastP.maxhp);
	setMP(m_character.lastP.maxmp);
	setSP(m_character.lastP.maxsp);

	// self:getClient():getCharacter():ClearFiter() 
}
bool Character::heroLeftMove(UInt32 speed)
{
	m_character.nowSpeed = 0;
	m_character.nowSpeed -= speed;
	m_character.moveStartTime = TimeUtil::getTimeMilliSec();

	return true;
}

bool Character::heroRightMove(UInt32 speed)
{
	m_character.nowSpeed = speed;
	m_character.moveStartTime = TimeUtil::getTimeMilliSec();
	return true;
}

bool Character::updateMove(UInt64 nowTime)
{
	Int32 nowPosx;
	double invTime;
	// 	if (getNowState()!= CHAR_STATE_RUN)//有可能是技能
	// 	{
	// 		return false;
	// 	}
	if (m_character.nowSpeed == 0)
	{
		return false;
	}
	invTime = (nowTime - m_character.moveStartTime)/1000;
	// 	if (invTime < 1000)
	// 	{
	// 		return false;
	// 	}
	nowPosx = m_character.nowSpeed * invTime + m_character.pos.x;//todo 速度是/ms?
	if (nowPosx > SCENE_EDGEX )//SCENE_EDGEX
	{
		m_character.moveStartTime =0;
		m_character.nowSpeed =0;
		nowPosx = SCENE_EDGEX;//SCENE_EDGEX
		setNowState(CHAR_STATE_IDLE);
	}
	else if (nowPosx < 0)
	{
		m_character.moveStartTime =0;
		m_character.nowSpeed =0;
		nowPosx = 0;
		setNowState(CHAR_STATE_IDLE);
	}
	else
	{
		m_character.moveStartTime = nowTime;
	}
	m_character.pos.x = nowPosx;
	return true;

}

// bool Character::checkJumpUpdate()
// {
// 	Int32 nowPosx;
// 	UInt64 invTime;
// 	UInt64 nowTime;
// 
// 	if (m_character.jump_speed == 0)
// 	{
// 		return false;
// 	}
// 
// 	nowTime =TimeUtil::getTimeMilliSec()/10000 ;
// 
// 
// }

void Character::stopAllAction(bool isStop)
{
	// 	bool isStop =  true;
	UInt32 curSkillId = getCurSkillID();
	if( curSkillId > 0 )
	{
		SkillBaseTemplate * skillBaseTemplate = getSkillBaseById(curSkillId);
		onSkillInterrupt(curSkillId);
	}
}


void Character:: playAnimation(UInt32 aniType,UInt32 delaytime)
{

	if(aniType == CHAR_STATE_IDLE )
	{


		// 		stopAllActions();
		// 		playIdle(aniType);
	}
	else if (aniType ==CHAR_STATE_RUN)
	{
		// 		playRun(aniType)
	}
	else if (aniType == CHAR_STATE_JUMP )
	{
		// 		playJump(aniType);
	}
	else if (aniType == CHAR_STATE_DIE)
	{		
		die(true);
		// 		playDie(aniType);
	}
	else if (aniType == CHAR_STATE_HURT)
	{
		// 		playHurt(aniType ,delaytime, hurtAni,true);
	}
	else if (aniType == CHAR_STATE_FALL)
	{
		// 		playFall(aniType ,delaytime);
	}
	else if (aniType == CHAR_STATE_VICTORY)
	{
		// 		playVictory(aniType);
	}
	else if (aniType == CHAR_STATE_ATTACK)
	{
		// 		playAttack(aniType,delaytime);
	}
	else if (aniType == CHAR_STATE_SHOW)
	{
		// 		playShowStart(aniType,delaytime);
	}

}

bool Character::playSkill()
{
	if (beforePlaySkill() == false )
		return false;

	if (procSkill() == false )
		return false;

	return true;
}

bool Character::beforePlaySkill()
{
	setAttackFlag(true);

	return true;
}

bool Character::procSkill()
{
	onSkillStart();//todo
	return true;
}
bool Character::onSkillStart()
{
	setflagAllowInterrupt(false);



	// 		if actorNode ~= nil then
	// 			self.effectNode = actorNode.pictures[GameDefine.CHAR_NODE_PRIORITY.EFFECT]
	// 		self.effectNode:setScale(self.skillData.skillBaseData.effectScale or 1)
	// 			local anchorPoint = self.skillData:getAniAnchorPoint()
	// 			if anchorPoint ~= 0 then
	// actorNode:setAnch(anchorPoint)
	// 		  end
	// 		  end
	// 		  self.loopNum = self.skillData.skillBaseData.loopNum or 1


	// 	playAnimation();
	UInt64 t = TimeUtil::getTimeMilliSec();
	updateSkill(t);
	return true;

}
bool Character::updateSkill(UInt64 t)
{
	UInt32 eventType = 0;
	AttParams eventParams;
	startFrameEvent( eventType, eventParams);
	return true;

}
bool Character::startFrameEvent(UInt32 eventType,AttParams eventParams)
{
	EventAttack( eventParams, false);
	return true;

}
bool Character::isNowAllowPlayerInterruptSkill(UInt32 skillID)
{
// 	SkillBaseTemplate *skillBaseTemplate ;
// 	skillBaseTemplate = gSkillBaseTable->get(skillID);
// 	return skillBaseTemplate->allowInterrupt;
	return m_character.flagAllowInterrupt;

}

bool Character::isNeedCD(UInt32 skillID)
{
	UInt32 level =0;
	SkillLevelTemplate *skillLevelTemplate;
	for(List<SkillData>::Iter *iter =  m_character.skillData.m_listSkills.begin();iter != NULL;iter = m_character.skillData.m_listSkills.next(iter))
	{
		if (iter->mValue.m_nID == skillID)
		{
			level = iter->mValue.m_nLevel;
		}
	}
	if (level ==  0)
	{
		return false;
	}

	skillLevelTemplate = SKILLLEVEL_TABLE().reverseGetNextLv(skillID,level);

	if (skillLevelTemplate->mCD1 > 0 )
		return true;

	return false;
}

bool Character::skillEventMove(Pos moveDistance,Pos moveSpeed)
{
	Pos moveTime;
	moveTime.x =0;
	moveTime.y =0;
	if (moveDistance.x != 0 && moveSpeed.x != 0)
	{
		moveTime.x = moveDistance.x / moveSpeed.x;
	}
	if (moveDistance.y != 0 && moveSpeed.y != 0)
	{
		moveTime.y = moveDistance.y / moveSpeed.y;
	}
	UInt32 giveSpeed = 0;
	if (moveTime.y != 0)
	{
		giveSpeed = moveDistance.y / moveTime.y;
	}


	if(giveSpeed != 0)
	{
		setSkillMoveYFlag(true);
		setJumpSpeed(giveSpeed);
		setOnGroundFlag(false);
	}
	return true;
}
bool Character::bulletUpdateMove(UInt64 t)
{
	//T_Bullet:update_move(t)
	return true;


}
bool Character::bulletStart(UInt64 t)
{	
	m_character.bulletStartTime = TimeUtil::getTimeMilliSec();
	m_character.bulletPos.x = getPosX();
	m_character.bulletPos.y = getPosY();
	m_character.bulletDirection = true;
	return true;
}
template<class TypeName>

UInt32 calcPropertytoPower(TypeName &values,PowerValueTemplate* powerValueTemplate)
{
	UInt32 power =0;

// 	power += values.mMaxHP * powerValueTemplate->mMaxHP ;

	power += values.mMaxMP * powerValueTemplate->mMaxMP ;

	power += values.mMaxSP * powerValueTemplate->mMaxSP ;

	power += values.mPA * powerValueTemplate->mPA ;

	power += values.mPF * powerValueTemplate->mPF ;

	power += values.mMA * powerValueTemplate->mMA ;

	power += values.mMF * powerValueTemplate->mMF ;

	power += values.mMPRecover * powerValueTemplate->mMPRecover ;

	power += values.mSPRecover * powerValueTemplate->mSPRecover ;

// 	power += values.mAttackSpeed * powerValueTemplate->mAttackSpeed ;

	power += values.mCombinationAtt * powerValueTemplate->mCombinationAtt ;

	power += values.mMoveSpeed * powerValueTemplate->mMoveSpeed ;

	power += values.mJumpHeight * powerValueTemplate->mJumpHeight ;

	power += values.mFlyTimes * powerValueTemplate->mFlyTimes ;

	power += values.mSPDamage * powerValueTemplate->mSPDamage ;

	power += values.mAttPush * powerValueTemplate->mAttPush ;

	power += values.mAttPushDef * powerValueTemplate->mAttPushDef ;

	power += values.mAttRaise * powerValueTemplate->mAttRaise ;

	power += values.mAttRaiseDef * powerValueTemplate->mAttRaiseDef ;

	power += values.mStiff * powerValueTemplate->mStiff ;

	power += values.mStiffDef * powerValueTemplate->mStiffDef ;

	power += values.mBaseCrit * powerValueTemplate->mBaseCrit ;

	power += values.mCritRate * powerValueTemplate->mCritRate ;

// 	power += values.mCritDef * powerValueTemplate-> mCritDef;
// 
// 	power += values.mStunDef * powerValueTemplate->mStunDef ;
// 
// 	power += values.mStunTimeDef * powerValueTemplate->mStunTimeDef ;
// 
// 	power += values.mSlowDef * powerValueTemplate->mSlowDef ;
// 
// 	power += values.mSlowTimeDef * powerValueTemplate->mSlowTimeDef ;
// 
// 	power += values.mPoisonDef * powerValueTemplate->mPoisonDef ;
// 
// 	power += values.mBloodDef * powerValueTemplate->mBloodDef ;
// 
// 	power += values.mKODef * powerValueTemplate->mKODef ;
// 
// 	power += values.mFloatVal * powerValueTemplate->mFloatVal ;
// 
// 	power += values.mFall * powerValueTemplate->mFall ;
// 
// 	power += values.mFallDef * powerValueTemplate->mFallDef ;



	return power;
}

UInt32 Character::calcPower(Guid playerID,UInt32 which)
{
	Player *player = LogicSystem::getSingleton().getPlayerByGuid(playerID);

	UInt32 maxNum = 0;
	UInt32 power = 0;
	UInt32 power1 = 0;
	UInt32 power2 = 0;
	UInt32 power3 = 0;

	if (which = 1 || (which ==10))//player
	{
		power1 =0;
		power2 =0;
		power3 =0;

		initLevelAttr(playerID,player->getPlayerLeval());
		PowerValueTemplate* powerValueTemplate = gPowerValueTable->get(1);
		power1 += calcPropertytoPower(m_character.heroTemplate,powerValueTemplate);

		if (which ==1)
		{
			power += power1;
		}
		else if (which ==10)
		{
			return power1;
		}
	}
	if (which = 1 || (which ==20))//skill
	{
		power1 =0;
		power2 =0;
		power3 =0;


		PlayerSkillListData mSkillListData = player->GetSkillList();
		for(List<SkillData>::Iter *iter = mSkillListData.m_listSkills.begin(); iter!=NULL;iter = mSkillListData.m_listSkills.next(iter))
		{
// 			power1 += gSkillLevelTable->get(iter->mValue.m_nID*1000+iter->mValue.m_nLevel)->mPower;
		}
		if (which ==1)
		{
			power += power1;
		}
		else if (which ==40)
		{
			return power1;
		}

	}
	if (which = 1 || (which ==30))//equip
	{
		power1 =0;
		power2 =0;
		power3 =0;

		PlayerEquipData mEquipData = player->GetPlayerEquipData();
		for(List<HeroEquipData>::Iter *iter = mEquipData.mHeroEquipLists.begin(); iter!=NULL;iter = mEquipData.mHeroEquipLists.next(iter))
		{
// 			power1 += gEquipTable->get(iter->mValue.m_nEquipId * 1000 +iter->mValue.m_nEquipLv)->mPower;
		}


		if (which ==1)
		{
			power += power1;
		}
		else if (which ==30)
		{
			return power1;
		}

	}
	if (which = 1 || (which ==40))//gem
	{
		power1 =0;
		power2 =0;
		power3 =0;

		PlayerGemData mGemData = player->GetPlayerGemData();
		for(List<GemData>::Iter *iter = mGemData.mGemLists.begin(); iter!=NULL;iter = mGemData.mGemLists.next(iter))
		{
// 			power1 = gGemTable->get(iter->mValue.m_nId)->mPower;
		}
		
		// 		power1 += gFashionTable->get(mFashionData.m_nEquipFashionID)->mPower;

		if (which ==1)
		{
			power += power1;
		}
		else if (which ==40)
		{
			return power1;
		}
	}
	if (which = 1 || (which ==50))//fashion
	{
		
		power1 =0;
		power2 =0;
		power3 =0;

		PlayerFashionData mFashionData = player->GetFashionData();
// 		power1 += gFashionTable->get(mFashionData.m_nEquipFashionID)->mPower;

		if (which ==1)
		{
			power += power1;
		}
		else if (which ==50)
		{
			return power1;
		}

	}
	if (which = 1 || (which ==60))//jewellery
	{
		power1 =0;
		power2 =0;
		power3 =0;

		JewelryEquipData mJewelryEquipData;

		PlayerJewelryData mJewelryData;
		PlayerFashionData mFashionData = player->GetFashionData();
		// 		power1 += gFashionTable->get(mFashionData.m_nEquipFashionID)->mPower;

		if (which ==1)
		{
			power += power1;
		}
		else if (which ==60)
		{
			return power1;
		}
	}

	if (which = 1 || (which >=70 && which <= 73))
	{
		power1 =0;
		power2 =0;
		power3 =0;

		PlayerHoarStoneData mHoarStoneData = player->getPlayerHoarStoneData();
		for (List<HoarStoneData>::Iter *iter = mHoarStoneData.mHoarStoneList.begin();iter!=NULL;iter = mHoarStoneData.mHoarStoneList.next(iter) )
		{
// 			power1 += gHoarstoneLvTable->get(iter->mValue.mStoneLv)->mPower;
// 			power2 += gHoarstoneStarTable->get(iter->mValue.mStoneStar)->mPower;
			for (List<UInt64>::Iter* it = iter->mValue.mEquipList.begin();it != NULL;it = iter->mValue.mEquipList.next(it))
			{
// 				power3 += gHoarstoneRuneTable->get(it->mValue)->mPower;
			}
		}
		if (which ==1)
		{
			power += power1 + power2 + power3;
		}
		else if (which ==70)
		{
			return power1 + power2 + power3;
		}
		else if (which == 71)
		{
			return power1;
		}
		else if (which == 72)
		{
			return power2;
		}
		else if (which == 72)
		{
			return power3;
		}
	}

	if (which = 1 || (which >=80 && which <= 82))
	{
		power1 =0;
		power2 =0;
		power3 =0;
		PlayerRhymeData mRhymeData = player->getPlayerRhymeData();
// 		power1 += gRhymeLevelTable->get(mRhymeData.m_RhymeStep *1000 + mRhymeData.m_RhymeLv)->mPower;
		for (List<UInt64>::Iter *iter = mRhymeData.m_RhymeAcupointList.begin();iter!=NULL;iter = mRhymeData.m_RhymeAcupointList.next(iter) )
		{
// 			power2 += gRhymeAcupointTable->get(iter->mValue)->mPower;
		}
		if (which ==1)
		{
			power += power1 + power2;
		}
		else if (which ==80)
		{
			return power1 + power2;
		}
		else if (which == 81)
		{
			return power1;
		}
		else if (which == 82)
		{
			return power2;
		}
	}
	
	return power;

}