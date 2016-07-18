#include "ServantAssistBattleTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
ServantAssistBattleTable::loadFromDbc(const String& fileName)
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
ServantAssistBattleTable::reloadFromDbc(const String& fileName)
{
	ServantAssistBattleTemplateMap tmpMap = mMap;

	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;

		LOG_WARN("Failed to reload servantassistbattle table.");
		return false;
	}
	return true;
}

bool 
ServantAssistBattleTable::saveToDbc(const String& filePath)
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
ServantAssistBattleTable::loadFromCsv(const String& filePath)
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
		ServantAssistBattleTemplate servantAssistBattleTemp;

		// ¹«ÓÃ
		if (!csvReader.bind("id", servantAssistBattleTemp.mId))
		{
			LOG_WARN("Failed to load servantassistbattle.csv for [id]");
			return false;
		}

		if (!csvReader.bind("pos", servantAssistBattleTemp.mPos))
		{
			LOG_WARN("Failed to load servantassistbattle.csv for [pos]");
			return false;
		}

		if (!csvReader.bind("level", servantAssistBattleTemp.mLevel))
		{
			LOG_WARN("Failed to load servantassistbattle.csv for [level]");
			return false;
		}

		if (!csvReader.bind("quality", servantAssistBattleTemp.mQuality))
		{
			LOG_WARN("Failed to load servantassistbattle.csv for [quality]");
			return false;
		}

		if (!csvReader.bind("costcount", servantAssistBattleTemp.mCost))
		{
			LOG_WARN("Failed to load servantassistbattle.csv for [costcount]");
			return false;
		}

		if (!csvReader.bind("attr", servantAssistBattleTemp.mAttrType))
		{
			LOG_WARN("Failed to load servantassistbattle.csv for [attr]");
			return false;
		}

		if (!csvReader.bind("attrvalue", servantAssistBattleTemp.mAttrValue))
		{
			LOG_WARN("Failed to load servantassistbattle.csv for [attrvalue]");
			return false;
		}

		if (!csvReader.bind("power", servantAssistBattleTemp.mPower))
		{
			LOG_WARN("Failed to load servantassistbattle.csv for [power]");
			return false;
		}

		mMap.insert(servantAssistBattleTemp.mId, servantAssistBattleTemp);

		ReverseAssistBattleTemplateMap::Iter *  reverseIter = mReverseMap.find(servantAssistBattleTemp.mPos);
		if(reverseIter)
		{
			reverseIter->mValue.insert(servantAssistBattleTemp.mLevel, servantAssistBattleTemp);
		}
		else
		{
			Map<UInt32 , ServantAssistBattleTemplate> servantAssistBattleTempMap;
			servantAssistBattleTempMap.insert(servantAssistBattleTemp.mLevel, servantAssistBattleTemp);
			mReverseMap.insert(servantAssistBattleTemp.mPos, servantAssistBattleTempMap);
		}



	}
	fileStream.close();
	return true;
}

