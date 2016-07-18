#include "LevelGrowTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
LevelGrowTable::loadFromDbc(const String& fileName)
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
LevelGrowTable::reloadFromDbc(const String& fileName)
{
	LevelGrowTemplateMap tmpMap = mMap;
	LevelGrowReverseFindMap reverseMap = mReverseMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		mReverseMap = reverseMap;
		LOG_WARN("Failed to reload LevelGrowTable table.");
		return false;
	}
	return true;
}

bool 
LevelGrowTable::saveToDbc(const String& filePath)
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
LevelGrowTable::loadFromCsv(const String& filePath)
{
	mMap.clear();
	mReverseMap.clear();
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
		LevelGrowTemplate levelGrowTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("id", levelGrowTemplate.mId))
		{
			LOG_WARN("Failed to load levelGrow.csv for [id]");
			return false;
		}
		
		if (!csvReader.bind("modelId", levelGrowTemplate.mModelID))
		{
			LOG_WARN("Failed to load levelGrow.csv for [imodelIdd]");
			return false;
		}

		if (!csvReader.bind("level", levelGrowTemplate.mLevel))
		{
			LOG_WARN("Failed to load levelGrow.csv for [level]");
			return false;
		}

		if (!csvReader.bind("power", levelGrowTemplate.mPower))
		{
			LOG_WARN("Failed to load levelGrow.csv for [power]");
			return false;
		}

		if (!csvReader.bind("MaxHP", levelGrowTemplate.mMaxHP))
		{
			LOG_WARN("Failed to load levelGrow.csv for [MaxHP]");
			return false;
		}

		if (!csvReader.bind("MaxMP", levelGrowTemplate.mMaxMP))
		{
			LOG_WARN("Failed to load levelGrow.csv for [MaxMP]");
			return false;
		}

		if (!csvReader.bind("MaxSP", levelGrowTemplate.mMaxSP))
		{
			LOG_WARN("Failed to load levelGrow.csv for [MaxSP]");
			return false;
		}

		if (!csvReader.bind("PA", levelGrowTemplate.mPA))
		{
			LOG_WARN("Failed to load levelGrow.csv for [PA]");
			return false;
		}

		if (!csvReader.bind("PF", levelGrowTemplate.mPF))
		{
			LOG_WARN("Failed to load levelGrow.csv for [PF]");
			return false;
		}

		if (!csvReader.bind("MA", levelGrowTemplate.mMA))
		{
			LOG_WARN("Failed to load levelGrow.csv for [MA]");
			return false;
		}

		if (!csvReader.bind("MF", levelGrowTemplate.mMF))
		{
			LOG_WARN("Failed to load levelGrow.csv for [MF]");
			return false;
		}
		
		if (!csvReader.bind("MPRecover", levelGrowTemplate.mMPRecover))
		{
			LOG_WARN("Failed to load levelGrow.csv for [MPRecover]");
			return false;
		}

		if (!csvReader.bind("SPRecover", levelGrowTemplate.mSPRecover))
		{
			LOG_WARN("Failed to load levelGrow.csv for [SPRecover]");
			return false;
		}

		if (!csvReader.bind("AttSpeed", levelGrowTemplate.mAttSpeed))
		{
			LOG_WARN("Failed to load levelGrow.csv for [AttSpeed]");
			return false;
		}

		if (!csvReader.bind("CombinationAtt", levelGrowTemplate.mCombinationAtt))
		{
			LOG_WARN("Failed to load levelGrow.csv for [CombinationAtt]");
			return false;
		}

		if (!csvReader.bind("MoveSpeed", levelGrowTemplate.mMoveSpeed))
		{
			LOG_WARN("Failed to load levelGrow.csv for [MoveSpeed]");
			return false;
		}

		if (!csvReader.bind("JumpHeight", levelGrowTemplate.mJumpHeight))
		{
			LOG_WARN("Failed to load levelGrow.csv for [JumpHeight]");
			return false;
		}

		if (!csvReader.bind("FlyTimes", levelGrowTemplate.mFlyTimes))
		{
			LOG_WARN("Failed to load levelGrow.csv for [FlyTimes]");
			return false;
		}

		if (!csvReader.bind("SPDamage", levelGrowTemplate.mSPDamage))
		{
			LOG_WARN("Failed to load levelGrow.csv for [SPDamage]");
			return false;
		}

		if (!csvReader.bind("AttPush", levelGrowTemplate.mAttPush))
		{
			LOG_WARN("Failed to load levelGrow.csv for [AttPush]");
			return false;
		}

		if (!csvReader.bind("AttPushDef", levelGrowTemplate.mAttPushDef))
		{
			LOG_WARN("Failed to load levelGrow.csv for [AttPushDef]");
			return false;
		}

		if (!csvReader.bind("AttRaise", levelGrowTemplate.mAttRaise))
		{
			LOG_WARN("Failed to load levelGrow.csv for [AttRaise]");
			return false;
		}

		if (!csvReader.bind("AttRaiseDef", levelGrowTemplate.mAttRaiseDef))
		{
			LOG_WARN("Failed to load levelGrow.csv for [AttRaiseDef]");
			return false;
		}

		if (!csvReader.bind("Stiff", levelGrowTemplate.mStiff))
		{
			LOG_WARN("Failed to load levelGrow.csv for [Stiff]");
			return false;
		}
	
		if (!csvReader.bind("StiffDef", levelGrowTemplate.mStiffDef))
		{
			LOG_WARN("Failed to load levelGrow.csv for [StiffDef]");
			return false;
		}

		if (!csvReader.bind("BaseCrit", levelGrowTemplate.mBaseCrit))
		{
			LOG_WARN("Failed to load levelGrow.csv for [BaseCrit]");
			return false;
		}

		if (!csvReader.bindFloat("CritRate", levelGrowTemplate.mCritRate))
		{
			LOG_WARN("Failed to load levelGrow.csv for [CritRate]");
			return false;
		}

		if (!csvReader.bind("CritDef", levelGrowTemplate.mCritDef))
		{
			LOG_WARN("Failed to load levelGrow.csv for [CritDef]");
			return false;
		}

		if (!csvReader.bindFloat("StunDef", levelGrowTemplate.mStunDef))
		{
			LOG_WARN("Failed to load levelGrow.csv for [StunDef]");
			return false;
		}
	
		if (!csvReader.bind("StunTimeDef", levelGrowTemplate.mStunTimeDef))
		{
			LOG_WARN("Failed to load levelGrow.csv for [StunTimeDef]");
			return false;
		}

		if (!csvReader.bind("SlowDef", levelGrowTemplate.mSlowDef))
		{
			LOG_WARN("Failed to load levelGrow.csv for [SlowDef]");
			return false;
		}

		if (!csvReader.bind("SlowTimeDef", levelGrowTemplate.mSlowTimeDef))
		{
			LOG_WARN("Failed to load levelGrow.csv for [SlowTimeDef]");
			return false;
		}

		if (!csvReader.bind("PoisonDef", levelGrowTemplate.mPoisonDef))
		{
			LOG_WARN("Failed to load levelGrow.csv for [PoisonDef]");
			return false;
		}

		if (!csvReader.bind("BloodDef", levelGrowTemplate.mBloodDef))
		{
			LOG_WARN("Failed to load levelGrow.csv for [BloodDef]");
			return false;
		}

		if (!csvReader.bind("KODef", levelGrowTemplate.mKODef))
		{
			LOG_WARN("Failed to load levelGrow.csv for [KODef]");
			return false;
		}

		if (!csvReader.bind("FloatVal", levelGrowTemplate.mFloatVal))
		{
			LOG_WARN("Failed to load levelGrow.csv for [FloatVal]");
			return false;
		}

		if (!csvReader.bind("FloatDef", levelGrowTemplate.mFloatDef))
		{
			LOG_WARN("Failed to load levelGrow.csv for [FloatDef]");
			return false;
		}

		if (!csvReader.bind("Fall", levelGrowTemplate.mFall))
		{
			LOG_WARN("Failed to load levelGrow.csv for [Fall]");
			return false;
		}

		if (!csvReader.bind("FallDef", levelGrowTemplate.mFallDef))
		{
			LOG_WARN("Failed to load levelGrow.csv for [FallDef]");
			return false;
		}


		LevelGrowReverseFindMap::Iter * reverseIter = mReverseMap.find(levelGrowTemplate.mModelID);
		if(reverseIter)
		{
			reverseIter->mValue.insert(levelGrowTemplate.mLevel, levelGrowTemplate);
		}
		else
		{

			Map<UInt32, LevelGrowTemplate>  levelMap;
			levelMap.insert(levelGrowTemplate.mLevel, levelGrowTemplate);

			mReverseMap.insert(levelGrowTemplate.mModelID,levelMap);

		}
			

		mMap.insert(levelGrowTemplate.mId, levelGrowTemplate);
	}
	fileStream.close();
	return true;
}

