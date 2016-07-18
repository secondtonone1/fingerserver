#include "ServantTreasureTable.h"

#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
ServantTreasureTable::loadFromDbc(const String& fileName)
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
ServantTreasureTable::reloadFromDbc(const String& fileName)
{
	ServantTreasureTemplateMap tmpMap = mMap;

	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;

		LOG_WARN("Failed to reload ServantTreasureTable table.");
		return false;
	}
	return true;
}

bool 
ServantTreasureTable::saveToDbc(const String& filePath)
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
ServantTreasureTable::loadFromCsv(const String& filePath)
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
		ServantTreasureTemplate servantTreasureTemp;
		// ¹«ÓÃ
		if (!csvReader.bind("id", servantTreasureTemp.mId))
		{
			LOG_WARN("Failed to load servanttreasure.csv for [id]");
			return false;
		}

		if (!csvReader.bind("quality", servantTreasureTemp.mQuality))
		{
			LOG_WARN("Failed to load servanttreasure.csv for [quality]");
			return false;
		}

		if (!csvReader.bind("attr1", servantTreasureTemp.mAttrType1))
		{
			LOG_WARN("Failed to load servanttreasure.csv for [attr1]");
			return false;
		}

		if (!csvReader.bind("attr1value", servantTreasureTemp.mAttrValue1))
		{
			LOG_WARN("Failed to load servanttreasure.csv for [attr1value]");
			return false;
		}

		if (!csvReader.bind("attr2", servantTreasureTemp.mAttrType2))
		{
			LOG_WARN("Failed to load servanttreasure.csv for [attr2]");
			return false;
		}

		if (!csvReader.bind("attr2value", servantTreasureTemp.mAttrValue2))
		{
			LOG_WARN("Failed to load servanttreasure.csv for [attr2value]");
			return false;
		}

		if (!csvReader.bind("attr2", servantTreasureTemp.mAttrType2))
		{
			LOG_WARN("Failed to load servanttreasure.csv for [attr2]");
			return false;
		}

		if (!csvReader.bind("attr2value", servantTreasureTemp.mAttrValue2))
		{
			LOG_WARN("Failed to load servanttreasure.csv for [attr2value]");
			return false;
		}

		if (!csvReader.bind("attr3", servantTreasureTemp.mAttrType3))
		{
			LOG_WARN("Failed to load servanttreasure.csv for [attr2]");
			return false;
		}

		if (!csvReader.bind("attr3value", servantTreasureTemp.mAttrValue3))
		{
			LOG_WARN("Failed to load servanttreasure.csv for [attr3value]");
			return false;
		}

		if (!csvReader.bind("power", servantTreasureTemp.mPower))
		{
			LOG_WARN("Failed to load servanttreasure.csv for [power]");
			return false;
		}

		if (!csvReader.bind("combine1", servantTreasureTemp.mCombineId1))
		{
			LOG_WARN("Failed to load servanttreasure.csv for [combine1]");
			return false;
		}
	
		if (!csvReader.bind("count1", servantTreasureTemp.mCombineCount1))
		{
			LOG_WARN("Failed to load servanttreasure.csv for [count1]");
			return false;
		}

		if (!csvReader.bind("combine2", servantTreasureTemp.mCombineId2))
		{
			LOG_WARN("Failed to load servanttreasure.csv for [combine2]");
			return false;
		}

		if (!csvReader.bind("count2", servantTreasureTemp.mCombineCount2))
		{
			LOG_WARN("Failed to load servanttreasure.csv for [count2]");
			return false;
		}

		if (!csvReader.bind("combine3", servantTreasureTemp.mCombineId3))
		{
			LOG_WARN("Failed to load servanttreasure.csv for [combine3]");
			return false;
		}

		if (!csvReader.bind("count3", servantTreasureTemp.mCombineCount3))
		{
			LOG_WARN("Failed to load servanttreasure.csv for [count3]");
			return false;
		}

		mMap.insert(servantTreasureTemp.mId, servantTreasureTemp);
		

	}
	fileStream.close();
	return true;
}

