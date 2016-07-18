#include "PowerValueTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
PowerValueTable::loadFromDbc(const String& fileName)
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
PowerValueTable::reloadFromDbc(const String& fileName)
{
	PowerValueTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload PowerValueTable table.");
		return false;
	}
	return true;
}

bool 
PowerValueTable::saveToDbc(const String& filePath)
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
PowerValueTable::loadFromCsv(const String& filePath)
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
		PowerValueTemplate PowerValueTemplate;
		// ¹«ÓÃ
// 		if (!csvReader.bind("id", PowerValueTemplate.mId))
// 		{
// 			LOG_WARN("Failed to load PowerValue.csv for [id]");
// 			return false;
// 		}
		
		if (!csvReader.bindFloat("MaxHP", PowerValueTemplate.mMaxHP))
		{
			LOG_WARN("Failed to load PowerValue.csv for [MaxHP]");
			return false;
		}

		if (!csvReader.bindFloat("MaxMP", PowerValueTemplate.mMaxMP))
		{
			LOG_WARN("Failed to load PowerValue.csv for [MaxMP]");
			return false;
		}

		if (!csvReader.bindFloat("MaxSP", PowerValueTemplate.mMaxSP))
		{
			LOG_WARN("Failed to load PowerValue.csv for [MaxSP]");
			return false;
		}

		if (!csvReader.bindFloat("PA", PowerValueTemplate.mPA))
		{
			LOG_WARN("Failed to load PowerValue.csv for [PA]");
			return false;
		}

		if (!csvReader.bindFloat("PF", PowerValueTemplate.mPF))
		{
			LOG_WARN("Failed to load PowerValue.csv for [PF]");
			return false;
		}

		if (!csvReader.bindFloat("MA", PowerValueTemplate.mMA))
		{
			LOG_WARN("Failed to load PowerValue.csv for [MA]");
			return false;
		}

		if (!csvReader.bindFloat("MF", PowerValueTemplate.mMF))
		{
			LOG_WARN("Failed to load PowerValue.csv for [MF]");
			return false;
		}
		
		if (!csvReader.bindFloat("MPRecover", PowerValueTemplate.mMPRecover))
		{
			LOG_WARN("Failed to load PowerValue.csv for [MPRecover]");
			return false;
		}

		if (!csvReader.bindFloat("SPRecover", PowerValueTemplate.mSPRecover))
		{
			LOG_WARN("Failed to load PowerValue.csv for [SPRecover]");
			return false;
		}

		if (!csvReader.bindFloat("AttSpeed", PowerValueTemplate.mAttSpeed))
		{
			LOG_WARN("Failed to load PowerValue.csv for [AttSpeed]");
			return false;
		}

		if (!csvReader.bindFloat("CombinationAtt", PowerValueTemplate.mCombinationAtt))
		{
			LOG_WARN("Failed to load PowerValue.csv for [CombinationAtt]");
			return false;
		}

		if (!csvReader.bindFloat("MoveSpeed", PowerValueTemplate.mMoveSpeed))
		{
			LOG_WARN("Failed to load PowerValue.csv for [MoveSpeed]");
			return false;
		}

		if (!csvReader.bindFloat("JumpHeight", PowerValueTemplate.mJumpHeight))
		{
			LOG_WARN("Failed to load PowerValue.csv for [JumpHeight]");
			return false;
		}

		if (!csvReader.bindFloat("FlyTimes", PowerValueTemplate.mFlyTimes))
		{
			LOG_WARN("Failed to load PowerValue.csv for [FlyTimes]");
			return false;
		}

		if (!csvReader.bindFloat("SPDamage", PowerValueTemplate.mSPDamage))
		{
			LOG_WARN("Failed to load PowerValue.csv for [SPDamage]");
			return false;
		}

		if (!csvReader.bindFloat("AttPush", PowerValueTemplate.mAttPush))
		{
			LOG_WARN("Failed to load PowerValue.csv for [AttPush]");
			return false;
		}

		if (!csvReader.bindFloat("AttPushDef", PowerValueTemplate.mAttPushDef))
		{
			LOG_WARN("Failed to load PowerValue.csv for [AttPushDef]");
			return false;
		}

		if (!csvReader.bindFloat("AttRaise", PowerValueTemplate.mAttRaise))
		{
			LOG_WARN("Failed to load PowerValue.csv for [AttRaise]");
			return false;
		}

		if (!csvReader.bindFloat("AttRaiseDef", PowerValueTemplate.mAttRaiseDef))
		{
			LOG_WARN("Failed to load PowerValue.csv for [AttRaiseDef]");
			return false;
		}

		if (!csvReader.bindFloat("Stiff", PowerValueTemplate.mStiff))
		{
			LOG_WARN("Failed to load PowerValue.csv for [Stiff]");
			return false;
		}
	
		if (!csvReader.bindFloat("StiffDef", PowerValueTemplate.mStiffDef))
		{
			LOG_WARN("Failed to load PowerValue.csv for [StiffDef]");
			return false;
		}

		if (!csvReader.bindFloat("BaseCrit", PowerValueTemplate.mBaseCrit))
		{
			LOG_WARN("Failed to load PowerValue.csv for [BaseCrit]");
			return false;
		}

		if (!csvReader.bindFloat("CritRate", PowerValueTemplate.mCritRate))
		{
			LOG_WARN("Failed to load PowerValue.csv for [CritRate]");
			return false;
		}

// 		if (!csvReader.bindFloat("CritDef", PowerValueTemplate.mCritDef))
// 		{
// 			LOG_WARN("Failed to load PowerValue.csv for [CritDef]");
// 			return false;
// 		}
// 
// 		if (!csvReader.bindFloat("StunDef", PowerValueTemplate.mStunDef))
// 		{
// 			LOG_WARN("Failed to load PowerValue.csv for [StunDef]");
// 			return false;
// 		}
// 	
// 		if (!csvReader.bindFloat("StunTimeDef", PowerValueTemplate.mStunTimeDef))
// 		{
// 			LOG_WARN("Failed to load PowerValue.csv for [StunTimeDef]");
// 			return false;
// 		}
// 
// 		if (!csvReader.bindFloat("SlowDef", PowerValueTemplate.mSlowDef))
// 		{
// 			LOG_WARN("Failed to load PowerValue.csv for [SlowDef]");
// 			return false;
// 		}
// 
// 		if (!csvReader.bindFloat("SlowTimeDef", PowerValueTemplate.mSlowTimeDef))
// 		{
// 			LOG_WARN("Failed to load PowerValue.csv for [SlowTimeDef]");
// 			return false;
// 		}
// 
// 		if (!csvReader.bindFloat("PoisonDef", PowerValueTemplate.mPoisonDef))
// 		{
// 			LOG_WARN("Failed to load PowerValue.csv for [PoisonDef]");
// 			return false;
// 		}
// 
// 		if (!csvReader.bindFloat("BloodDef", PowerValueTemplate.mBloodDef))
// 		{
// 			LOG_WARN("Failed to load PowerValue.csv for [BloodDef]");
// 			return false;
// 		}
// 
// 		if (!csvReader.bindFloat("KODef", PowerValueTemplate.mKODef))
// 		{
// 			LOG_WARN("Failed to load PowerValue.csv for [KODef]");
// 			return false;
// 		}
// 
// 		if (!csvReader.bindFloat("FloatVal", PowerValueTemplate.mFloatVal))
// 		{
// 			LOG_WARN("Failed to load PowerValue.csv for [FloatVal]");
// 			return false;
// 		}
// 
// 		if (!csvReader.bindFloat("FloatDef", PowerValueTemplate.mFloatDef))
// 		{
// 			LOG_WARN("Failed to load PowerValue.csv for [FloatDef]");
// 			return false;
// 		}
// 
// 		if (!csvReader.bindFloat("Fall", PowerValueTemplate.mFall))
// 		{
// 			LOG_WARN("Failed to load PowerValue.csv for [Fall]");
// 			return false;
// 		}
// 
// 		if (!csvReader.bindFloat("FallDef", PowerValueTemplate.mFallDef))
// 		{
// 			LOG_WARN("Failed to load PowerValue.csv for [FallDef]");
// 			return false;
// 		}

		mMap.insert(1, PowerValueTemplate);
	}
	fileStream.close();
	return true;
}

