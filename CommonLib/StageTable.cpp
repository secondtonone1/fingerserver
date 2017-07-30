#include "StageTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
StageTable::loadFromDbc(const String& fileName)
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
StageTable::reloadFromDbc(const String& fileName)
{
	StageTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload StageTemplate table.");
		return false;
	}
	return true;
}

bool 
StageTable::saveToDbc(const String& filePath)
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
StageTable::loadFromCsv(const String& filePath)
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
		StageTemplate stageTemplate;
		// 公用
		if (!csvReader.bind("stageId", stageTemplate.mStageId))
		{
			LOG_WARN("Failed to load Stage.csv for [Id]");
			return false;
		}

		if (!csvReader.bind("chapterId", stageTemplate.mChapterId))
		{
			LOG_WARN("Failed to load Stage.csv for [chapterId]");
			return false;
		}

		if (!csvReader.bind("name", stageTemplate.mName))
		{
			LOG_WARN("Failed to load Stage.csv for [name]");
			return false;
		}

		if (!csvReader.bind("desc", stageTemplate.mDesc))
		{
			LOG_WARN("Failed to load Stage.csv for [desc]");
			return false;
		}

		if (!csvReader.bind("type", stageTemplate.mType))
		{
			LOG_WARN("Failed to load Stage.csv for [type]");
			return false;
		}

		if (!csvReader.bind("sceneID", stageTemplate.mSceneID))
		{
			LOG_WARN("Failed to load Stage.csv for [sceneID]");
			return false;
		}

		if (!csvReader.bind("sceneConfig", stageTemplate.mSceneConfig))
		{
			LOG_WARN("Failed to load Stage.csv for [sceneConfig]");
			return false;
		}

		if (!csvReader.bind("finShowList", stageTemplate.mStrFinShowList))
		{
			LOG_WARN("Failed to load Stage.csv for [finShowList]");
			return false;
		}

		if (!csvReader.bind("point", stageTemplate.mPoint))
		{
			LOG_WARN("Failed to load Stage.csv for [point]");
			return false;
		}

		if (!csvReader.bind("capacity", stageTemplate.mCapacity))
		{
			LOG_WARN("Failed to load Stage.csv for [capacity]");
			return false;
		}

		if (!csvReader.bind("stageStepCount", stageTemplate.mStageStepCount))
		{
			LOG_WARN("Failed to load Stage.csv for [stageStepCount]");
			return false;
		}

// 		if (!csvReader.bind("perBloodCount", stageTemplate.mPerBloodCount))
// 		{
// 			LOG_WARN("Failed to load Stage.csv for [perBloodCount]");
// 			return false;
// 		}


		if (!csvReader.bind("strengthCost", stageTemplate.mStrengthCost))
		{
			LOG_WARN("Failed to load Stage.csv for [strengthCost]");
			return false;
		}

		if (!csvReader.bind("challengetimesLimit", stageTemplate.mChallengetimesLimit))
		{
			LOG_WARN("Failed to load Stage.csv for [challengetimesLimit]");
			return false;
		}

		if (!csvReader.bind("startLimit1", stageTemplate.mStartLimit1))
		{
			LOG_WARN("Failed to load Stage.csv for [startLimit1]");
			return false;
		}

		if (!csvReader.bind("startLimit2", stageTemplate.mStartLimit2))
		{
			LOG_WARN("Failed to load Stage.csv for [startLimit2]");
			return false;
		}

		if (!csvReader.bind("startLimit3", stageTemplate.mStartLimit3))
		{
			LOG_WARN("Failed to load Stage.csv for [startLimit3]");
			return false;
		}

		if (!csvReader.bind("startLimit4", stageTemplate.mStartLimit4))
		{
			LOG_WARN("Failed to load Stage.csv for [startLimit4]");
			return false;
		}

		if (!csvReader.bind("startLimit5", stageTemplate.mStartLimit5))
		{
			LOG_WARN("Failed to load Stage.csv for [startLimit5]");
			return false;
		}

// 		if (!csvReader.bind("icon", stageTemplate.mIcon))
// 		{
// 			LOG_WARN("Failed to load Stage.csv for [icon]");
// 			return false;
// 		}

		if (!csvReader.bind("bossShow", stageTemplate.mBossShow))
		{
			LOG_WARN("Failed to load Stage.csv for [bossShow]");
			return false;
		}

		if (!csvReader.bind("StarRequire1", stageTemplate.mStarRequire1))
		{
			LOG_WARN("Failed to load Stage.csv for [StarRequire1]");
			return false;
		}

		if (!csvReader.bind("StarRequire2", stageTemplate.mStarRequire2))
		{
			LOG_WARN("Failed to load Stage.csv for [StarRequire2]");
			return false;
		}

		if (!csvReader.bind("StarRequire3", stageTemplate.mStarRequire3))
		{
			LOG_WARN("Failed to load Stage.csv for [StarRequire3]");
			return false;
		}

		if (!csvReader.bind("StarRequire4", stageTemplate.mStarRequire4))//加载两个表，有的表没该字段
		{
			stageTemplate.mStarRequire4 ="";
// 			LOG_WARN("Failed to load Stage.csv for [StarRequire4]");
// 			return false;
		}

		if (!csvReader.bind("StarRequire5", stageTemplate.mStarRequire5))
		{
			stageTemplate.mStarRequire5 ="";
			// 			LOG_WARN("Failed to load Stage.csv for [StarRequire4]");
			// 			return false;
		}


		if (!csvReader.bind("awardGold", stageTemplate.mAwardGold))
		{
			LOG_WARN("Failed to load Stage.csv for [awardGold]");
			return false;

		}


		if (!csvReader.bind("awardExp", stageTemplate.mAwardExp))
		{
			LOG_WARN("Failed to load Stage.csv for [awardExp]");
			return false;

		}


		if (!csvReader.bind("awardOther", stageTemplate.mAwardOther))
		{
			LOG_WARN("Failed to load Stage.csv for [awardOther]");
			return false;

		}


		if (!csvReader.bind("awardID", stageTemplate.mAwardID))
		{
			LOG_WARN("Failed to load Stage.csv for [awardID]");
			return false;

		}
		if (!csvReader.bind("firstAwardID", stageTemplate.mFirstAwardID))
		{
			LOG_WARN("Failed to load Stage.csv for [firstAwardID]");
			return false;
		}

		if (!csvReader.bind("awardCardID", stageTemplate.awardCardID))
		{
			LOG_WARN("Failed to load Stage.csv for [awardCardID]");
			return false;

		}
		if (!csvReader.bind("monsterAward", stageTemplate.monsterAward))
		{
			LOG_WARN("Failed to load Stage.csv for [monsterAward]");
			return false;

		}
		if (!csvReader.bind("threat", stageTemplate.threat))
		{
			LOG_WARN("Failed to load Stage.csv for [threat]");
			return false;

		}
		if (!csvReader.bind("limittime1", stageTemplate.limittime1))
		{
			LOG_WARN("Failed to load Stage.csv for [limittime1]");
			return false;

		}
		if (!csvReader.bind("limittime2", stageTemplate.limittime2))
		{
			LOG_WARN("Failed to load Stage.csv for [limittime2]");
			return false;

		}

		if (!csvReader.bind("mopuptype", stageTemplate.mopuptype))
		{
			stageTemplate.mopuptype =0;
// 			LOG_WARN("Failed to load Stage.csv for [mopuptype]");
// 			return false;
		}

		if (!csvReader.bind("awardaddtype", stageTemplate.awardaddtype))
		{
			stageTemplate.awardaddtype =0;
// 			LOG_WARN("Failed to load Stage.csv for [awardaddtype]");
// 			return false;
		}

		if (!csvReader.bind("isBoss", stageTemplate.isBoss))
		{
			stageTemplate.isBoss =0;
			// 			LOG_WARN("Failed to load Stage.csv for [mopuptype]");
			// 			return false;
		}

		if (!csvReader.bind("dialogue", stageTemplate.dialogue))
		{
			stageTemplate.dialogue ="";
			// 			LOG_WARN("Failed to load Stage.csv for [awardaddtype]");
			// 			return false;
		}


		mMap.insert(stageTemplate.mStageId, stageTemplate);
	}
	fileStream.close();
	return true;
}

