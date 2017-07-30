#include "HoarstoneBaseTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
HoarstoneBaseTable::loadFromDbc(const String& fileName)
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
HoarstoneBaseTable::reloadFromDbc(const String& fileName)
{
	HoarstoneBaseTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload HoarstoneBaseTemplate table.");
		return false;
	}
	return true;
}

bool 
HoarstoneBaseTable::saveToDbc(const String& filePath)
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
HoarstoneBaseTable::loadFromCsv(const String& filePath)
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
		HoarstoneBaseTemplate hoarStoneBaseTemp;
		// ¹«ÓÃ
		if (!csvReader.bind("id", hoarStoneBaseTemp.mId))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [id]");
			return false;
		}

		if (!csvReader.bind("name", hoarStoneBaseTemp.mName))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [name]");
			return false;
		}

		if (!csvReader.bind("maxLevel", hoarStoneBaseTemp.mMaxLv))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [maxLevel]");
			return false;
		}

		if (!csvReader.bind("mainIcon", hoarStoneBaseTemp.mMainIcon))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [mainIcon]");
			return false;
		}
		
		if (!csvReader.bind("pieceIcon", hoarStoneBaseTemp.mPieceIcon))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [pieceIcon]");
			return false;
		}

		if (!csvReader.bind("active1ConditionType", hoarStoneBaseTemp.mActiveConditionType1))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [active1ConditionType]");
			return false;
		}

		if (!csvReader.bind("active1ConditionParam1", hoarStoneBaseTemp.mActiveCondition1Param1))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [active1ConditionParam1]");
			return false;
		}

		if (!csvReader.bind("active1ConditionParam2", hoarStoneBaseTemp.mActiveCondition1Param2))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [active1ConditionParam2]");
			return false;
		}

		if (!csvReader.bind("active1ConditionDesc", hoarStoneBaseTemp.mActiveConditionDesc1))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [activeConditionDesc1]");
			return false;
		}

		if (!csvReader.bind("active2ConditionType", hoarStoneBaseTemp.mActiveConditionType2))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [active2ConditionType]");
			return false;
		}

		if (!csvReader.bind("active2ConditionParam1", hoarStoneBaseTemp.mActiveCondition2Param1))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [active2ConditionParam1]");
			return false;
		}

		if (!csvReader.bind("active2ConditionParam2", hoarStoneBaseTemp.mActiveCondition2Param2))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [active2ConditionParam2]");
			return false;
		}

		if (!csvReader.bind("active2ConditionDesc", hoarStoneBaseTemp.mActiveConditionDesc2))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [activeConditionDesc2]");
			return false;
		}
		
		if (!csvReader.bind("active3ConditionType", hoarStoneBaseTemp.mActiveConditionType3))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [active3ConditionType]");
			return false;
		}


		if (!csvReader.bind("active3ConditionParam1", hoarStoneBaseTemp.mActiveCondition3Param1))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [active3ConditionParam1]");
			return false;
		}

		if (!csvReader.bind("active3ConditionParam2", hoarStoneBaseTemp.mActiveCondition3Param2))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [active3ConditionParam2]");
			return false;
		}


		if (!csvReader.bind("active3ConditionDesc", hoarStoneBaseTemp.mActiveConditionDesc3))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [activeConditionDesc3]");
			return false;
		}

		if (!csvReader.bind("active4ConditionType", hoarStoneBaseTemp.mActiveConditionType4))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [active4ConditionType]");
			return false;
		}

		if (!csvReader.bind("active4ConditionParam1", hoarStoneBaseTemp.mActiveCondition4Param1))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [active4ConditionParam1]");
			return false;
		}

		if (!csvReader.bind("active4ConditionParam2", hoarStoneBaseTemp.mActiveCondition4Param2))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [active4ConditionParam2]");
			return false;
		}

		if (!csvReader.bind("active4ConditionDesc", hoarStoneBaseTemp.mActiveConditionDesc4))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [activeConditionDesc4]");
			return false;
		}

		if (!csvReader.bind("active5ConditionType", hoarStoneBaseTemp.mActiveConditionType5))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [active5ConditionType]");
			return false;
		}

		if (!csvReader.bind("active5ConditionParam1", hoarStoneBaseTemp.mActiveCondition5Param1))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [active5ConditionParam1]");
			return false;
		}

		if (!csvReader.bind("active5ConditionParam2", hoarStoneBaseTemp.mActiveCondition5Param2))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [active5ConditionParam2]");
			return false;
		}

		if (!csvReader.bind("active5ConditionDesc", hoarStoneBaseTemp.mActiveConditionDesc5))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [activeConditionDesc5]");
			return false;
		}

		if (!csvReader.bind("exchange", hoarStoneBaseTemp.mExchange))
		{
			LOG_WARN("Failed to load hoarstonebase.csv for [exchange]");
			return false;
		}


		mMap.insert(hoarStoneBaseTemp.mId, hoarStoneBaseTemp);
	}
	fileStream.close();
	return true;
}

