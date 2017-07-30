#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"
#include "RobotTable.h"

using namespace Lynx;


bool 
RobotTable::loadFromDbc(const String& fileName)
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
RobotTable::reloadFromDbc(const String& fileName)
{
	RobotTableMap tmpMap = mMap;
	mMap.clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload robot table.");
		return false;
	}
	return true;
}



bool 
RobotTable::loadFromCsv(const String& filePath)
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
		
		RobotTableTemplate robotTabletemplate;

		if (!csvReader.bind("roleID", robotTabletemplate.roleID))
		{
			LOG_WARN("Failed to load robotTable.csv for [roleID]");
			return false;
		}
		if (!csvReader.bind("guild", robotTabletemplate.guild))
		{
			LOG_WARN("Failed to load robotTable.csv for [guild]");
			return false;
		}
		if (!csvReader.bind("title", robotTabletemplate.title))
		{
			LOG_WARN("Failed to load robotTable.csv for [title]");
			return false;
		}
		if (!csvReader.bind("name", robotTabletemplate.name))
		{
			LOG_WARN("Failed to load robotTable.csv for [name]");
			return false;
		}

		if (!csvReader.bind("level", robotTabletemplate.level))
		{
			LOG_WARN("Failed to load robotTable.csv for [level]");
			return false;
		}
		if (!csvReader.bind("power", robotTabletemplate.power))
		{
			LOG_WARN("Failed to load robotTable.csv for [power]");
			return false;
		}
		if (!csvReader.bind("vipLv", robotTabletemplate.vipLv))
		{
			LOG_WARN("Failed to load robotTable.csv for [vipLv]");
			return false;
		}
		if (!csvReader.bind("modelID", robotTabletemplate.modelID))
		{
			LOG_WARN("Failed to load robotTable.csv for [modelID]");
			return false;
		}
		if (!csvReader.bind("puonfashionID", robotTabletemplate.puonfashionID))
		{
			LOG_WARN("Failed to load robotTable.csv for [puonfashionID]");
			return false;
		}
		if (!csvReader.bind("equipID1", robotTabletemplate.equipID1))
		{
			LOG_WARN("Failed to load robotTable.csv for [equipID1]");
			return false;
		}
		if (!csvReader.bind("skill1", robotTabletemplate.skill1))
		{
			LOG_WARN("Failed to load robotTable.csv for [skill1]");
			return false;
		}
		if (!csvReader.bind("skill2", robotTabletemplate.skill2))
		{
			LOG_WARN("Failed to load robotTable.csv for [skill2]");
			return false;
		}
		if (!csvReader.bind("skill3", robotTabletemplate.skill3))
		{
			LOG_WARN("Failed to load robotTable.csv for [skill3]");
			return false;
		}
		if (!csvReader.bind("rhymeCount", robotTabletemplate.rhymeCount))
		{
			LOG_WARN("Failed to load robotTable.csv for [rhymeCount]");
			return false;
		}
// 		if (!csvReader.bind("rhymeskill1", robotTabletemplate.rhymeskill1))
// 		{
// 			LOG_WARN("Failed to load robotTable.csv for [rhymeskill1]");
// 			return false;
// 		}
// 		if (!csvReader.bind("rhymeskill2", robotTabletemplate.rhymeskill2))
// 		{
// 			LOG_WARN("Failed to load robotTable.csv for [rhymeskill2]");
// 			return false;
// 		}
// 		if (!csvReader.bind("rhymeskill3", robotTabletemplate.rhymeskill3))
// 		{
// 			LOG_WARN("Failed to load robotTable.csv for [rhymeskill3]");
// 			return false;
// 		}
		if (!csvReader.bind("onstation1", robotTabletemplate.onstation1))
		{
			LOG_WARN("Failed to load robotTable.csv for [onstation1]");
			return false;
		}
		if (!csvReader.bind("onstation2", robotTabletemplate.onstation2))
		{
			LOG_WARN("Failed to load robotTable.csv for [onstation2]");
			return false;
		}
		if (!csvReader.bind("onstation3", robotTabletemplate.onstation3))
		{
			LOG_WARN("Failed to load robotTable.csv for [onstation3]");
			return false;
		}
		if (!csvReader.bind("onstation4", robotTabletemplate.onstation4))
		{
			LOG_WARN("Failed to load robotTable.csv for [onstation4]");
			return false;
		}
		if (!csvReader.bind("onstation5", robotTabletemplate.onstation5))
		{
			LOG_WARN("Failed to load robotTable.csv for [onstation5]");
			return false;
		}
		if (!csvReader.bind("MaxHP", robotTabletemplate.levelGrow.mMaxHP))
		{
			LOG_WARN("Failed to load robotTable.csv for [MaxHP]");
			return false;
		}
		if (!csvReader.bind("MaxMP", robotTabletemplate.levelGrow.mMaxMP))
		{
			LOG_WARN("Failed to load robotTable.csv for [MaxMP]");
			return false;
		}
		if (!csvReader.bind("MaxSP", robotTabletemplate.levelGrow.mMaxSP))
		{
			LOG_WARN("Failed to load robotTable.csv for [MaxSP]");
			return false;
		}
		if (!csvReader.bind("PA", robotTabletemplate.levelGrow.mPA))
		{
			LOG_WARN("Failed to load robotTable.csv for [PA]");
			return false;
		}
		if (!csvReader.bind("PF", robotTabletemplate.levelGrow.mPF))
		{
			LOG_WARN("Failed to load robotTable.csv for [PF]");
			return false;
		}
		if (!csvReader.bind("MA", robotTabletemplate.levelGrow.mMA))
		{
			LOG_WARN("Failed to load robotTable.csv for [MA]");
			return false;
		}
		if (!csvReader.bind("MF", robotTabletemplate.levelGrow.mMF))
		{
			LOG_WARN("Failed to load robotTable.csv for [MF]");
			return false;
		}
// 		if (!csvReader.bind("HPRecover", robotTabletemplate.levelGrow.mHPRecover))
// 		{
// 			LOG_WARN("Failed to load robotTable.csv for [HPRecover]");
// 			return false;
// 		}
		if (!csvReader.bind("MPRecover", robotTabletemplate.levelGrow.mMPRecover))
		{
			LOG_WARN("Failed to load robotTable.csv for [MPRecover]");
			return false;
		}
		if (!csvReader.bind("SPRecover", robotTabletemplate.levelGrow.mSPRecover))
		{
			LOG_WARN("Failed to load robotTable.csv for [SPRecover]");
			return false;
		}
		if (!csvReader.bind("AttSpeed", robotTabletemplate.levelGrow.mAttSpeed))
		{
			LOG_WARN("Failed to load robotTable.csv for [AttSpeed]");
			return false;
		}
		if (!csvReader.bind("CombinationAtt", robotTabletemplate.levelGrow.mCombinationAtt))
		{
			LOG_WARN("Failed to load robotTable.csv for [CombinationAtt]");
			return false;
		}
		if (!csvReader.bind("MoveSpeed", robotTabletemplate.levelGrow.mMoveSpeed))
		{
			LOG_WARN("Failed to load robotTable.csv for [MoveSpeed]");
			return false;
		}
		if (!csvReader.bind("JumpHeight", robotTabletemplate.levelGrow.mJumpHeight))
		{
			LOG_WARN("Failed to load robotTable.csv for [JumpHeight]");
			return false;
		}
		if (!csvReader.bind("FlyTimes", robotTabletemplate.levelGrow.mFlyTimes))
		{
			LOG_WARN("Failed to load robotTable.csv for [FlyTimes]");
			return false;
		}

		if (!csvReader.bind("SPDamage", robotTabletemplate.levelGrow.mSPDamage))
		{
			LOG_WARN("Failed to load robotTable.csv for [SPDamage]");
			return false;
		}
		if (!csvReader.bind("AttPush", robotTabletemplate.levelGrow.mAttPush))
		{
			LOG_WARN("Failed to load robotTable.csv for [AttPush]");
			return false;
		}
		if (!csvReader.bind("AttPushDef", robotTabletemplate.levelGrow.mAttPushDef))
		{
			LOG_WARN("Failed to load robotTable.csv for [AttPushDef]");
			return false;
		}
		if (!csvReader.bind("AttRaise", robotTabletemplate.levelGrow.mAttRaise))
		{
			LOG_WARN("Failed to load robotTable.csv for [AttRaise]");
			return false;
		}
		if (!csvReader.bind("AttRaiseDef", robotTabletemplate.levelGrow.mAttRaiseDef))
		{
			LOG_WARN("Failed to load robotTable.csv for [AttRaiseDef]");
			return false;
		}
		if (!csvReader.bind("Stiff", robotTabletemplate.levelGrow.mStiff))
		{
			LOG_WARN("Failed to load robotTable.csv for [Stiff]");
			return false;
		}
		if (!csvReader.bind("StiffDef", robotTabletemplate.levelGrow.mStiffDef))
		{
			LOG_WARN("Failed to load robotTable.csv for [StiffDef]");
			return false;
		}
		if (!csvReader.bind("BaseCrit", robotTabletemplate.levelGrow.mBaseCrit))
		{
			LOG_WARN("Failed to load robotTable.csv for [BaseCrit]");
			return false;
		}
		if (!csvReader.bind("CritRate", robotTabletemplate.levelGrow.mCritRate))
		{
			LOG_WARN("Failed to load robotTable.csv for [CritRate]");
			return false;
		}
		if (!csvReader.bind("CritDef", robotTabletemplate.levelGrow.mCritDef))
		{
			LOG_WARN("Failed to load robotTable.csv for [CritDef]");
			return false;
		}
		if (!csvReader.bind("StunDef", robotTabletemplate.levelGrow.mStunDef))
		{
			LOG_WARN("Failed to load robotTable.csv for [StunDef]");
			return false;
		}
		if (!csvReader.bind("StunTimeDef", robotTabletemplate.levelGrow.mStunTimeDef))
		{
			LOG_WARN("Failed to load robotTable.csv for [StunTimeDef]");
			return false;
		}
		if (!csvReader.bind("SlowDef", robotTabletemplate.levelGrow.mSlowDef))
		{
			LOG_WARN("Failed to load robotTable.csv for [SlowDef]");
			return false;
		}
		if (!csvReader.bind("SlowTimeDef", robotTabletemplate.levelGrow.mSlowTimeDef))
		{
			LOG_WARN("Failed to load robotTable.csv for [SlowTimeDef]");
			return false;
		}
		if (!csvReader.bind("PoisonDef", robotTabletemplate.levelGrow.mPoisonDef))
		{
			LOG_WARN("Failed to load robotTable.csv for [PoisonDef]");
			return false;
		}
		if (!csvReader.bind("BloodDef", robotTabletemplate.levelGrow.mBloodDef))
		{
			LOG_WARN("Failed to load robotTable.csv for [BloodDef]");
			return false;
		}
		if (!csvReader.bind("KODef", robotTabletemplate.levelGrow.mKODef))
		{
			LOG_WARN("Failed to load robotTable.csv for [KODef]");
			return false;
		}
		if (!csvReader.bind("FloatVal", robotTabletemplate.levelGrow.mFloatVal))
		{
			LOG_WARN("Failed to load robotTable.csv for [FloatVal]");
			return false;
		}
		if (!csvReader.bind("FloatDef", robotTabletemplate.levelGrow.mFloatDef))
		{
			LOG_WARN("Failed to load robotTable.csv for [FloatDef]");
			return false;
		}
		if (!csvReader.bind("Fall", robotTabletemplate.levelGrow.mFall))
		{
			LOG_WARN("Failed to load robotTable.csv for [Fall]");
			return false;
		}	
		if (!csvReader.bind("FallDef", robotTabletemplate.levelGrow.mFallDef))
		{
			LOG_WARN("Failed to load robotTable.csv for [FallDef]");
			return false;
		}
		if (!csvReader.bind("score", robotTabletemplate.score))
		{
			LOG_WARN("Failed to load robotTable.csv for [score]");
			return false;
		}



		mMap.insert(robotTabletemplate.roleID,robotTabletemplate);		
	}
	fileStream.close();
	return true;
}

bool RobotTable::saveToDbc(const String& filePath)
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
