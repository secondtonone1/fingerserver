
#include "BattleCalc.h"
#include "BattleData.h"

using namespace Lynx;
BattleCalc::BattleCalc()
{

}
BattleCalc::~BattleCalc()
{

}

bool BattleCalc::checkCanAttackByCampID(Character &attacker,Character &defencer)
{
	UInt32 attackerwhichOneInBg = attacker.getWhichOneInBg();
	UInt32 defencerwhichOneInBg = defencer.getWhichOneInBg();
	if (attackerwhichOneInBg != defencerwhichOneInBg)
	{
		return false;
	}
	UInt32 camp_att = attacker.getCampID();
	UInt32 camp_def = defencer.getCampID();
	if (camp_att != camp_def)
	{
		return true;
	}
	return false;

}

void BattleCalc::cleanTargets()
{

}
bool BattleCalc::checkInTargets(Guid uid )
{
	return true;

}

void BattleCalc::addToTargets(UInt64 playerID)
{

}

Damages BattleCalc::calcDamage(Character &attacker,Character &defencer,AttParams attParams)
{

	Damages finalDamages;

	float skillIdRate = attParams.damage.dValue;

	finalDamages.spcost = attacker.getHeroTemplate()->mSPDamage * (-1) * (skillIdRate);

	finalDamages.push = calcAttackPush(attacker, defencer, attParams.attrSkillAdd);
	finalDamages.raise = calcAttackRaise(attacker, defencer, attParams.notstand, attParams.attrSkillAdd);
	finalDamages.fall =calcAttackFall(attacker, defencer, attParams.notstand);
	finalDamages.fly =calcAttackFloat(attacker, defencer, attParams.notstand);
	finalDamages.stiff = calcAttackStiff(attacker, defencer, attParams.attrSkillAdd);
	finalDamages.buff = calcBuffState(attacker, defencer, attParams.addBuff);

	float critRate =  getCritRate( attacker, defencer );
	Damage damage = getDamage( attacker, defencer, attParams.damage);
	if (((rand()%100)/100) <= critRate)
	{
		finalDamages.isCrit= true;			
		finalDamages.damage =getCritValue(attacker, damage);
	}
	else
	{
		finalDamages.isCrit = false;			
		finalDamages.damage =  damage;
	}

	finalDamages.hurtType = attParams.damage.hurtType;// or GameDefine.HURT_TYPE.Default

	//-- dump(finalDamages)		
	return finalDamages;
}

float BattleCalc::getCritRate( Character &attacker,Character &defencer )
{
	float rate = 0;
	float critAtt = attacker.getHeroTemplate()->mBaseCrit;
	float critDef = defencer.getHeroTemplate()->heroEx.mBaseToughness;


	float rateAtt = critAtt / (100 + 8*attacker.getLevel() + critAtt);
	float rateDef = critDef / (100 + 8*defencer.getLevel() + critDef);

	rate = rateAtt * (1 - rateDef);

	return rate;
}

Damage BattleCalc::getCritValue( Character &attacker,Damage damage )
{
	float critRate = attacker.getHeroTemplate()->mCritRate;
	damage.dType *= critRate;
	damage.dValue *= critRate;
	damage.hurtType *= critRate;
	return damage;
}

Damage BattleCalc::getDamage(Character &attacker,Character defencer,Damage damageContent)
{
	//--易伤buff加成
	float blood = 0;
	if( defencer.getBuffStateByIdx(BUFF_STATE_SUB_TYPE_BLOOD) == true)
	{
		blood = 0.5 ;
	}

	float att = 0;
	float def = 0;
	if( damageContent.dType == 0 )
	{
		//--技能伤害
		att = attacker.getHeroTemplate()->mMA;
		def = defencer.getHeroTemplate()->mMF;
	}
	else if( damageContent.dType == 1)
	{
		//--普通伤害
		att = attacker.getHeroTemplate()->mPA;
		def = defencer.getHeroTemplate()->mPF;

	}

	Damage damage;
	UInt32 randomNum = rand()%100;
	float critRate = att * att / (att + def) * (1 + blood) * (1 + ((randomNum-50)/1000)) ;
	if (damageContent.dValue != 0 )
	{
		damage.dType *= critRate;
		damage.dValue *= critRate;
		damage.hurtType *= critRate;
	}
	return damage;
}



float BattleCalc::calcAttackPush(Character &attacker,Character &defencer,Map<UInt32, AttrSkillAdd*> attrSkillAdd)
{

	UInt32 isGet =0;
	float baseAttPush = attacker.getHeroTemplate()->mAttPush;
	float baseAttPushDef = defencer.getHeroTemplate()->mAttPushDef;
	//--击退抗性百分比=击退抗性/（100+8*LV+击退抗性）
	float attPushDefRate = baseAttPushDef / (100+8*defencer.getLevel()+baseAttPushDef);
	//--击退位移=基础击退力*（1+技能加成）*（1-击退抗性百分比）
	UInt32 pushKeyStr = CHAR_ATTR_AttPush;
	float pushOffsetX = 0;
	for(Map<UInt32, AttrSkillAdd*>::Iter *iter = attrSkillAdd.begin();iter!=NULL;iter = attrSkillAdd.next(iter))
	{
		if (pushKeyStr == iter->mKey)
		{
			if (iter->mValue->affectType == VALUE_EFFECT_TYPE_numrate)
			{
				pushOffsetX = baseAttPush *(1+iter->mValue->affectValue)*(1-attPushDefRate);
			}
			else if (iter->mValue->affectType == VALUE_EFFECT_TYPE_numval)
			{
				pushOffsetX = (baseAttPush + iter->mValue->affectValue)*(1-attPushDefRate);
			}
			isGet =1;
			break;
		}
	}

	if (isGet ==0)
	{
		pushOffsetX = baseAttPush *(1-attPushDefRate);
	}
	return pushOffsetX;
}

float BattleCalc::calcAttackRaise(Character &attacker,Character &defencer,float notstand,Map<UInt32, AttrSkillAdd*> attrSkillAdd)
{
	UInt32 isGet =0;
	float floatHeight = 0;	
	if (notstand !=  BeHurtedState_float)
	{
		return 0;
	}

	float raiseKeyStr = CHAR_ATTR_AttRaise;
	float floatForce;
	float attRaise = attacker.getHeroTemplate()->mAttRaise;
	for(Map<UInt32, AttrSkillAdd*>::Iter *iter = attrSkillAdd.begin();iter!=NULL;iter = attrSkillAdd.next(iter))
	{
		if (raiseKeyStr == iter->mKey)
		{
			if (iter->mValue->affectType == VALUE_EFFECT_TYPE_numrate)
			{
				floatForce = attRaise *(1+iter->mValue->affectValue);
			}
			else if (iter->mValue->affectType == VALUE_EFFECT_TYPE_numval)
			{
				floatForce = (attRaise + iter->mValue->affectValue);
			}
			isGet = 1;
		}
	}
	if (isGet == 0)
	{
		floatForce = attRaise;
	}
	if (floatForce == 0)
	{
		return 0;
	}
	return floatHeight;

	floatHeight = floatForce - defencer.getHeroTemplate()->mAttRaiseDef;
	if (floatHeight < 0)
	{
		return 0;
	}
	return floatHeight;
}

//--倒地
float BattleCalc::calcAttackFall(Character &attacker,Character &defencer,float notstand)
{
	if( notstand != BeHurtedState_fall)
	{
		return false;
	}
	float attckerfall =  attacker.getHeroTemplate()->mFall; 
	float defencerfalldef =  defencer.getHeroTemplate()->mFallDef;
	if (attckerfall < defencerfalldef)
	{
		return false;
	}
	return true;
}

//击飞
float BattleCalc::calcAttackFloat(Character &attacker,Character &defencer,float notstand)
{
	if( notstand != BeHurtedState_floatX)
	{
		return false;
	}
	float attckerfall =  attacker.getHeroTemplate()->mFloatVal; 
	float defencerfalldef =  defencer.getHeroTemplate()->mFloatDef;
	if (attckerfall < defencerfalldef)
	{
		return false;
	}
	return true;
}


float BattleCalc::calcAttackStiff(Character &attacker,Character &defencer,Map<UInt32, AttrSkillAdd*> attrSkillAdd)
{
	UInt32 isGet =0;
	float baseAttPush = attacker.getHeroTemplate()->mStiff;
	float baseAttPushDef = defencer.getHeroTemplate()->mStiffDef;
	//--击退抗性百分比=击退抗性/（100+8*LV+击退抗性）
	float attPushDefRate = baseAttPushDef / (100+8*defencer.getLevel()+baseAttPushDef);
	//--击退位移=基础击退力*（1+技能加成）*（1-击退抗性百分比）
	UInt32 pushKeyStr = CHAR_ATTR_Stiff;
	UInt32 pushOffsetX = 0;
	for(Map<UInt32, AttrSkillAdd*>::Iter *iter = attrSkillAdd.begin();iter!=NULL;iter = attrSkillAdd.next(iter))
	{
		if (pushKeyStr == iter->mKey)
		{
			if (iter->mValue->affectType == VALUE_EFFECT_TYPE_numrate)
			{
				pushOffsetX = baseAttPush *(1+iter->mValue->affectValue)*(1-attPushDefRate);
			}
			else if (iter->mValue->affectType == VALUE_EFFECT_TYPE_numval)
			{
				pushOffsetX = (baseAttPush + iter->mValue->affectValue)*(1-attPushDefRate);
			}
			isGet =1;
			break;
		}
	}

	if (isGet ==0)
	{
		pushOffsetX = baseAttPush *(1-attPushDefRate);
	}
	return pushOffsetX;
}
List<UInt32> BattleCalc::calcBuffState(Character &attacker,Character &defencer,List<UInt32> buffArray)
{
	List<UInt32> buffs;

	for(List<UInt32>::Iter *iter = buffArray.begin();iter!=NULL;iter = buffArray.next(iter))
	{
		if(BuffManager::getSingleton().checkBuffExistById(iter->mValue)==true)
		{
			buffs.insertTail(iter->mValue);
		}
	}
	return buffs;
}



//--晕眩
VertigoVal BattleCalc::getEffectValOfVertigo( Character &attacker,Character &defencer,UInt32 skillId,UInt32 Multiidx )
{
	VertigoVal vertigoVal;

	float vertigoDef = defencer.getHeroTemplate()->heroEx.mVertigoDef;
	float t = vertigoDef / (100 + 8*defencer.getLevel() + vertigoDef);
	float vertigoAtt = 1; //--TODO ----通过buff实现
	float vertigoRate = vertigoAtt * (1 - t);
	vertigoVal.rate = vertigoRate;

	//-- time
	float vertigoDef2 = defencer.getHeroTemplate()->heroEx.mVertigoTimeDef; 
	float t2 = vertigoDef2 / (100 + 8*defencer.getLevel() + vertigoDef2);
	float vertigoAtt2 = 3; //--TODO ----通过buff实现
	float vertigoTime = vertigoAtt2 * (1 - t2);
	vertigoVal.time = vertigoTime;

	return vertigoVal;

}



VertigoVal BattleCalc::getEffectValOfSlow( Character &attacker,Character &defencer,UInt32 skillId,UInt32 Multiidx )
{
	VertigoVal vertigoVal;

	float vertigoDef =defencer.getHeroTemplate()->mSlowDef;
	float t = vertigoDef / (100 + 8*defencer.getLevel() + vertigoDef);
	float vertigoAtt = 1; //--TODO ----通过buff实现
	float vertigoRate = vertigoAtt * (1 - t);
	vertigoVal.rate = vertigoRate;

	//-- time
	float vertigoDef2 = defencer.getHeroTemplate()->mSlowTimeDef; 
	float t2 = vertigoDef2 / (100 + 8*defencer.getLevel() + vertigoDef2);
	float vertigoAtt2 = 3; //--TODO ----通过buff实现
	float vertigoTime = vertigoAtt2 * (1 - t2);
	vertigoVal.time = vertigoTime;
	vertigoVal.val = 0.5;

	return vertigoVal;
}

float BattleCalc::getEffectValOfPoison( Character &attacker,Character &defencer,UInt32 skillId,UInt32 Multiidx )

{
	float PoisonDef = defencer.getHeroTemplate()->mPoisonDef; 
	float t = PoisonDef / (100 + 8*defencer.getLevel() + PoisonDef);
	float attackerAtt = attacker.getHeroTemplate()->mPA;
	float attackerRate = 1;

	return (attackerAtt * attackerRate * (1 - t));	
}

float BattleCalc::getEffectValOfBlood( Character &attacker,Character &defencer,UInt32 skillId,UInt32 Multiidx )
{
	float BloodDef = defencer.getHeroTemplate()->mBloodDef;
	float t = BloodDef / (100 + 8*defencer.getLevel() + BloodDef);
	float BloodAtt = 1;// ------通过buff实现

	return (BloodAtt * (1 - t));
}
float BattleCalc::getEffectValOfKO( Character &attacker,Character &defencer,UInt32 skillId,UInt32 Multiidx )
{
	float KODef =defencer.getHeroTemplate()->mKODef; 
	float t = KODef / (100 + 8*defencer.getLevel() + KODef);

	double KOAtt = 0.3 ;//------通过buff实现

	return (KOAtt * (1 - t));
}

