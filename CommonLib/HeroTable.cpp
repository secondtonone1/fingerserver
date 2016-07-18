#include "HeroTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
HeroTable::loadFromDbc(const String& fileName)
{
	StreamBuffer streamBuffer(8192 - sizeof(StreamBuffer::Node));
	Int32 length = RESOURCE_GROUP_MANAGER().loadFileToStream(fileName, 
		streamBuffer, "Data");
	if (length <= 0)
	{
		LOG_WARN("Failed to load file %s", fileName.c_str());
		return false;
	}
	if (!unserialize(*this, streamBuffer, 0))
	{
		LOG_WARN("Failed to unserialize file %s", fileName.c_str());
		return false;
	}
	return true;
}

bool 
HeroTable::reloadFromDbc(const String& fileName)
{
	HeroTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload HeroTemplateMap table.");
		return false;
	}
	return true;
}

bool 
HeroTable::saveToDbc(const String& filePath)
{
	StreamBuffer streamBuffer(8192 - sizeof(StreamBuffer::Node));
	serialize(*this, streamBuffer, 0);
	if (!FileUtil::saveFile(filePath, streamBuffer))
	{
		LOG_WARN("Failed to save file %s.", filePath.c_str());
		return false;
	}
	return true;
}

bool 
HeroTable::loadFromCsv(const String& filePath)
{
	mMap.clear();
	std::ifstream fileStream(filePath.c_str());
	if (fileStream.fail())
	{
		LOG_WARN("Failed to open %s file.", filePath.c_str());
		return false;
	}
	CsvReader csvReader(fileStream, ",");
	csvReader.initTitle();
	while (csvReader.readLine())
	{
		HeroTemplate heroTemplate;
		// 公用
		if (!csvReader.bind("id", heroTemplate.mId))
		{
			LOG_WARN("Failed to load hero.csv for [id]");
			return false;
		}

		if (!csvReader.bind("name", heroTemplate.mName))
		{
			LOG_WARN("Failed to load hero.csv for [name]");
			return false;
		}

		if (!csvReader.bind("desc", heroTemplate.mDesc))
		{
			LOG_WARN("Failed to load hero.csv for [desc]");
			return false;
		}

		if (!csvReader.bind("res", heroTemplate.mRes))
		{
			LOG_WARN("Failed to load hero.csv for [res]");
			return false;
		}

		if (!csvReader.bind("skillList", heroTemplate.mStrSkillList))
		{
			LOG_WARN("Failed to load hero.csv for [skillList]");
			return false;
		}

		if (!csvReader.bind("skillListOnCreate", heroTemplate.mStrSkillListOnCreate))
		{
			LOG_WARN("Failed to load hero.csv for [skillListOnCreate]");
			return false;
		}

		/*if (!csvReader.bind("PA", heroTemplate.mPA))
		{
			LOG_WARN("Failed to load hero.csv for [PA]");
			return false;
		}

		if (!csvReader.bind("PF", heroTemplate.mPF))
		{
			LOG_WARN("Failed to load hero.csv for [PF]");
			return false;
		}

		if (!csvReader.bind("MA", heroTemplate.mMA))
		{
			LOG_WARN("Failed to load hero.csv for [MA]");
			return false;
		}

		if (!csvReader.bind("MF", heroTemplate.mMF))
		{
			LOG_WARN("Failed to load hero.csv for [MF]");
			return false;
		}

		if (!csvReader.bind("MaxHP", heroTemplate.mMaxHP))
		{
			LOG_WARN("Failed to load hero.csv for [MaxHP]");
			return false;
		}

		if (!csvReader.bind("MaxMP", heroTemplate.mMaxMP))
		{
			LOG_WARN("Failed to load hero.csv for [MaxMP]");
			return false;
		}

		if (!csvReader.bind("MaxSP", heroTemplate.mMaxSP))
		{
			LOG_WARN("Failed to load hero.csv for [MaxSP]");
			return false;
		}

		if (!csvReader.bind("MPRecover", heroTemplate.mMPRecover))
		{
			LOG_WARN("Failed to load hero.csv for [MPRecover]");
			return false;
		}

		if (!csvReader.bind("SPRecover", heroTemplate.mSPRecover))
		{
			LOG_WARN("Failed to load hero.csv for [SPRecover]");
			return false;
		}

		if (!csvReader.bind("AttackSpeed", heroTemplate.mAttackSpeed))
		{
			LOG_WARN("Failed to load hero.csv for [MaxSP]");
			return false;
		}

		if (!csvReader.bind("CombinationAtt", heroTemplate.mCombinationAtt))
		{
			LOG_WARN("Failed to load hero.csv for [CombinationAtt]");
			return false;
		}

		if (!csvReader.bind("MoveSpeed", heroTemplate.mMoveSpeed))
		{
			LOG_WARN("Failed to load hero.csv for [MoveSpeed]");
			return false;
		}

		if (!csvReader.bind("JumpHeight", heroTemplate.mJumpHeight))
		{
			LOG_WARN("Failed to load hero.csv for [JumpHeight]");
			return false;
		}

		if (!csvReader.bind("FlyTimes", heroTemplate.mFlyTimes))
		{
			LOG_WARN("Failed to load hero.csv for [FlyTimes]");
			return false;
		}

		if (!csvReader.bind("SPDamage", heroTemplate.mSPDamage))
		{
			LOG_WARN("Failed to load hero.csv for [SPDamage]");
			return false;
		}

		if (!csvReader.bind("AttPush", heroTemplate.mAttPush))
		{
			LOG_WARN("Failed to load hero.csv for [AttPush]");
			return false;
		}

		if (!csvReader.bind("AttPushDef", heroTemplate.mAttPushDef))
		{
			LOG_WARN("Failed to load hero.csv for [AttPushDef]");
			return false;
		}

		if (!csvReader.bind("AttRaise", heroTemplate.mAttRaise))
		{
			LOG_WARN("Failed to load hero.csv for [AttRaise]");
			return false;
		}

		if (!csvReader.bind("AttRaiseDef", heroTemplate.mAttRaiseDef))
		{
			LOG_WARN("Failed to load hero.csv for [AttRaiseDef]");
			return false;
		}

		if (!csvReader.bind("Stiff", heroTemplate.mStiff))
		{
			LOG_WARN("Failed to load hero.csv for [Stiff]");
			return false;
		}

		if (!csvReader.bind("StiffDef", heroTemplate.mStiffDef))
		{
			LOG_WARN("Failed to load hero.csv for [StiffDef]");
			return false;
		}

		if (!csvReader.bind("BaseCrit", heroTemplate.mBaseCrit))
		{
			LOG_WARN("Failed to load hero.csv for [BaseCrit]");
			return false;
		}

		if (!csvReader.bind("CritRate", heroTemplate.mCritRate))
		{
			LOG_WARN("Failed to load hero.csv for [CritRate]");
			return false;
		}

		if (!csvReader.bind("CritDef", heroTemplate.mCritDef))
		{
			LOG_WARN("Failed to load hero.csv for [CritDef]");
			return false;
		}

		if (!csvReader.bind("StunDef", heroTemplate.mStunDef))
		{
			LOG_WARN("Failed to load hero.csv for [StunDef]");
			return false;
		}

		if (!csvReader.bind("StunTimeDef", heroTemplate.mStunTimeDef))
		{
			LOG_WARN("Failed to load hero.csv for [StunTimeDef]");
			return false;
		}

		if (!csvReader.bind("SlowDef", heroTemplate.mSlowDef))
		{
			LOG_WARN("Failed to load hero.csv for [SlowDef]");
			return false;
		}

		if (!csvReader.bind("SlowTimeDef", heroTemplate.mSlowTimeDef))
		{
			LOG_WARN("Failed to load hero.csv for [SlowTimeDef]");
			return false;
		}

		if (!csvReader.bind("PoisonDef", heroTemplate.mPoisonDef))
		{
			LOG_WARN("Failed to load hero.csv for [PoisonDef]");
			return false;
		}

		if (!csvReader.bind("BloodDef", heroTemplate.mBloodDef))
		{
			LOG_WARN("Failed to load hero.csv for [BloodDef]");
			return false;
		}

		if (!csvReader.bind("KODef", heroTemplate.mKODef))
		{
			LOG_WARN("Failed to load hero.csv for [KODef]");
			return false;
		}

		if (!csvReader.bind("FloatVal", heroTemplate.mFloatVal))
		{
			LOG_WARN("Failed to load hero.csv for [FloatVal]");
			return false;
		}

		if (!csvReader.bind("FloatDef", heroTemplate.mFloatDef))
		{
			LOG_WARN("Failed to load hero.csv for [FloatDef]");
			return false;
		}

		if (!csvReader.bind("Fall", heroTemplate.mFall))
		{
			LOG_WARN("Failed to load hero.csv for [Fall]");
			return false;
		}

		if (!csvReader.bind("FallDef", heroTemplate.heroEx.mFallDef))
		{
			LOG_WARN("Failed to load hero.csv for [FallDef]");
			return false;
		}

		if (!csvReader.bind("DefenceAreaStand", heroTemplate.heroEx.mDefenceAreaStand))
		{
			LOG_WARN("Failed to load hero.csv for [DefenceAreaStand]");
			return false;
		}

		if (!csvReader.bind("DefenceAreaLie", heroTemplate.heroEx.mDefenceAreaLie))
		{
			LOG_WARN("Failed to load hero.csv for [DefenceAreaLie]");
			return false;
		}*/

		if (!csvReader.bind("playerlevel", heroTemplate.mNeedLv))
		{
			LOG_WARN("Failed to load hero.csv for [playerlevel]");
			return false;
		}

		if (!csvReader.bind("viplevel", heroTemplate.mNeedVipLv))
		{
			LOG_WARN("Failed to load hero.csv for [viplevel]");
			return false;
		}

		if (!csvReader.bind("cost", heroTemplate.mCost))
		{
			LOG_WARN("Failed to load hero.csv for [cost]");
			return false;
		}

		
		std::string originSkillStr = heroTemplate.mStrSkillListOnCreate.c_str();

		std::string ::size_type  findIndex = originSkillStr.find(';');
		std::stringstream mystream;
		while(findIndex != std::string::npos)
		{
			//;号之前的全部截取，放入到列表里
			std::string originEleStr = originSkillStr.substr(0,findIndex);

			mystream.clear();
			mystream.str("");

			mystream << originEleStr;

			UInt64 originEle = 0;
			mystream >> originEle;

			heroTemplate.m_listOriginSkills.insertTail(originEle);
			
			originSkillStr = originSkillStr.substr(findIndex + 1);

			findIndex = originSkillStr.find(';');

		}

		if(originSkillStr != "")
		{
			mystream.clear();
			mystream.str("");

			mystream << originSkillStr;

			UInt64 originEle = 0;
			mystream >> originEle;
			heroTemplate.m_listOriginSkills.insertTail(originEle);
		}

		
		Vector<UInt64> totalSkillVec;
		std::string totalSkillStr = heroTemplate.mStrSkillList.c_str();
		findIndex = totalSkillStr.find(';');
		while(findIndex != std::string::npos)
		{
			//;号之前的全部截取，放入到列表里
			std::string totalEleStr = totalSkillStr.substr(0,findIndex);

			mystream.clear();
			mystream.str("");

			mystream << totalEleStr;

			UInt64 totalEle = 0;
			mystream >> totalEle;

			totalSkillVec.push_back(totalEle);

			totalSkillStr = totalSkillStr.substr(findIndex + 1);

			findIndex = totalSkillStr.find(';');

		}

		if(totalSkillStr != "")
		{
			mystream.clear();
			mystream.str("");

			mystream << totalSkillStr;

			UInt64 originEle = 0;
			mystream >> originEle;
			totalSkillVec.push_back(originEle);
		}

		for(UInt32 i = 0; i < totalSkillVec.size(); i++)
		{
			List<UInt64>::Iter* findIter = heroTemplate.m_listOriginSkills.find(totalSkillVec[i],heroTemplate.m_listOriginSkills.begin(),
				heroTemplate.m_listOriginSkills.end());
			
			if(!findIter)
			{
				heroTemplate.m_listLearnSkills.insertTail(totalSkillVec[i]);
			}
			
		}

		

		mMap.insert(heroTemplate.mId, heroTemplate);
	}
	fileStream.close();
	return true;
}

