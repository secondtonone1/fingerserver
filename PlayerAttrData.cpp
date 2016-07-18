#include "PlayerAttrData.h"

using namespace Lynx;

//目前人物属性涉及到战力的只有这些，故此处只存储这些数据结构用来计算属性和战力加成
//如有不够，后需添加
PlayerAttrData::PlayerAttrData(UInt32 modelId, UInt32 level)
{
	LevelGrowTemplate * levelGrowTemp = LEVELGROW_TABLE().reverseGet(modelId, level);
	if(levelGrowTemp)
	{
		m_nMaxHp = levelGrowTemp->mMaxHP;
		m_nMaxMp = levelGrowTemp->mMaxMP;
		m_nMaxSp = levelGrowTemp->mMaxSP;
		m_nPA = levelGrowTemp->mPA;
		m_nMA = levelGrowTemp->mMA;
		m_nPF = levelGrowTemp->mPF;
		m_nMF = levelGrowTemp->mMF;
		m_nHPRecover = 0;
		m_nMPRecover = levelGrowTemp->mMPRecover;
		m_nSPRecover = levelGrowTemp->mSPRecover;
		m_nSPDamage = levelGrowTemp->mSPDamage;
		m_nBaseCrit = levelGrowTemp->mBaseCrit;
		m_fCritRate = levelGrowTemp->mCritRate;
		m_nCritDef = levelGrowTemp->mCritDef;
	}
	else
	{
		m_nMaxHp = 0;
		m_nMaxMp = 0;
		m_nMaxSp = 0;
		m_nPA = 0;
		m_nMA = 0;
		m_nPF = 0;
		m_nMF = 0;
		m_nHPRecover = 0;
		m_nMPRecover = 0;
		m_nSPRecover = 0;
		m_nSPDamage = 0;
		m_nBaseCrit = 0;
		m_fCritRate = 0;
		m_nCritDef = 0;;

	}
}


PlayerAttrData::PlayerAttrData(const PlayerAttrData& playerAttrData)
{
	m_nMaxHp = playerAttrData.m_nMaxHp;
	m_nMaxMp = playerAttrData.m_nMaxMp;
	m_nMaxSp = playerAttrData.m_nMaxSp;
	m_nPA = playerAttrData.m_nPA;
	m_nMA = playerAttrData.m_nMA;
	m_nPF = playerAttrData.m_nPF;
	m_nMF = playerAttrData.m_nMF;
	m_nHPRecover = playerAttrData.m_nHPRecover;
	m_nMPRecover = playerAttrData.m_nMPRecover;
	m_nSPRecover = playerAttrData.m_nSPRecover;
	m_nSPDamage = playerAttrData.m_nSPDamage;
	m_nBaseCrit = playerAttrData.m_nBaseCrit;
	m_fCritRate = playerAttrData.m_fCritRate;
	m_nCritDef = playerAttrData.m_nCritDef;
}

void PlayerAttrData::addAttrTypeValue(UInt32 attrType, float attrValue)
{
	
	switch(attrType)
	{
	case MaxHP:
		{
			m_nMaxHp += attrValue;
		}
		break;
	case MaxMP:
		{
			m_nMaxMp += attrValue;
		}
		break;
	case MaxSP:
		{	
			m_nMaxSp += attrValue;
		}
		break;
	case PA:
		{
			m_nPA += attrValue;
		}
		break;
	case MA:
		{
			m_nMA += attrValue;
		}
		break;
	case PF:
		{
			m_nPF += attrValue;
		}
		break;
	case MF:
		{
			m_nMF += attrValue;
		}
		break;

	case MPRecover:
		{
			m_nMPRecover += attrValue;
		}
		break;
	case SPRecover:
		{
			m_nSPRecover += attrValue;
		}
		break;
	
	case BaseCrit:
		{
			m_nBaseCrit += attrValue;
		}
		break;
	case Critrate:
		{
			m_fCritRate += attrValue;
		}
		break;
	case BaseToughness:
		{
			m_nCritDef += attrValue;
		}
		break;
	case HPRecover:
		{
			m_nHPRecover += attrValue;
		}
		break;
	case SPDamage:
		{
			m_nSPDamage += attrValue;
		}
		break;

	default:
		break;

	}
}

PlayerAttrData & PlayerAttrData::operator = (const PlayerAttrData & playerAttrData)
{
	//检测自赋值
	if(this == &playerAttrData)
	{
		return * this;
	}
	
	m_nMaxHp = playerAttrData.m_nMaxHp;
	m_nMaxMp = playerAttrData.m_nMaxMp;
	m_nMaxSp = playerAttrData.m_nMaxSp;
	m_nPA = playerAttrData.m_nPA;
	m_nMA = playerAttrData.m_nMA;
	m_nPF = playerAttrData.m_nPF;
	m_nMF = playerAttrData.m_nMF;
	m_nHPRecover = playerAttrData.m_nHPRecover;
	m_nMPRecover = playerAttrData.m_nMPRecover;
	m_nSPRecover = playerAttrData.m_nSPRecover;
	m_nSPDamage = playerAttrData.m_nSPDamage;
	m_nBaseCrit = playerAttrData.m_nBaseCrit;
	m_fCritRate = playerAttrData.m_fCritRate;
	m_nCritDef = playerAttrData.m_nCritDef;

	return *this;
}


UInt32 PlayerAttrData::attrConverToPower()
{
	UInt32 power = 0;

	attrConverToCapacity(power, MaxHP,m_nMaxHp);
	attrConverToCapacity(power, MaxMP,m_nMaxMp);
	attrConverToCapacity(power, MaxSP,m_nMaxSp);
    attrConverToCapacity(power, PA, m_nPA);
	attrConverToCapacity(power, MA, m_nMA);
	attrConverToCapacity(power, PF, m_nPF);
	attrConverToCapacity(power, MF, m_nMF);
	attrConverToCapacity(power, HPRecover, m_nHPRecover);
	attrConverToCapacity(power, MPRecover, m_nMPRecover);
	attrConverToCapacity(power, SPRecover, m_nSPRecover);
	attrConverToCapacity(power, SPDamage, m_nSPDamage);
	attrConverToCapacity(power, BaseCrit, m_nBaseCrit);
	
	attrConverToCapacity(power, Critrate, m_fCritRate);

	attrConverToCapacity(power,BaseToughness ,m_nCritDef);

	return  power;
}


void PlayerAttrData::attrConverToCapacity(UInt32 & fightCapacity,UInt32 attrType, UInt32 attrValue)
{
		
	AttrPowerTemplate * attrPowerTemp = ATTRPOWER_TABLE().get(attrType);

	if(attrPowerTemp)
	{
		fightCapacity += attrValue * attrPowerTemp->mAttrValue;
	}
}
