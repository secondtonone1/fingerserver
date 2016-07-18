#include "NewJewelryTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
NewJewelryTable::loadFromDbc(const String& fileName)
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
NewJewelryTable::reloadFromDbc(const String& fileName)
{
	NewJewelryTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload NewJewelry table.");
		return false;
	}
	return true;
}


bool 
NewJewelryTable::saveToDbc(const String& filePath)
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
NewJewelryTable::loadFromCsv(const String& filePath)
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
		NewJewelryTemplate jewelryTemplate;
		// 公用
		if (!csvReader.bind("jewelryId", jewelryTemplate.mJewelryId))
		{
			LOG_WARN("Failed to load new_jewelry.csv for [jewelryId]");
			return false;
		}

		// 公用
		if (!csvReader.bind("quality", jewelryTemplate.mQuality))
		{
			LOG_WARN("Failed to load new_jewelry.csv for [quality]");
			return false;
		}

		if (!csvReader.bind("equipSlotIdx", jewelryTemplate.mEquipSlotIdx))
		{
			LOG_WARN("Failed to load new_jewelry.csv for [equipSlotIdx]");
			return false;
		}

		if (!csvReader.bind("levelRequire", jewelryTemplate.mLvRequire))
		{
			LOG_WARN("Failed to load new_jewelry.csv for [levelRequire]");
			return false;
		}

		if (!csvReader.bind("randomAttr1", jewelryTemplate.mRandomAttr1))
		{
			LOG_WARN("Failed to load new_jewelry.csv for [randomAttr1]");
			return false;
		}

		if (!csvReader.bind("randomAttr2", jewelryTemplate.mRandomAttr2))
		{
			LOG_WARN("Failed to load new_jewelry.csv for [randomAttr2]");
			return false;
		}

		if (!csvReader.bind("randomAttr3", jewelryTemplate.mRandomAttr3))
		{
			LOG_WARN("Failed to load new_jewelry.csv for [randomAttr3]");
			return false;
		}

		if (!csvReader.bind("randomAttr4", jewelryTemplate.mRandomAttr4))
		{
			LOG_WARN("Failed to load new_jewelry.csv for [randomAttr4]");
			return false;
		}

		if (!csvReader.bind("randomAttr5", jewelryTemplate.mRandomAttr5))
		{
			LOG_WARN("Failed to load new_jewelry.csv for [randomAttr5]");
			return false;
		}
	
		if (!csvReader.bind("staticAttr1Key", jewelryTemplate.mStaticAttr1Key))
		{
			LOG_WARN("Failed to load new_jewelry.csv for [staticAttr1Key]");
			return false;
		}

		if (!csvReader.bind("staticAttr1ValueMin", jewelryTemplate.mStaticAttr1ValueMin))
		{
			LOG_WARN("Failed to load new_jewelry.csv for [staticAttr1ValueMin]");
			return false;
		}

		if (!csvReader.bind("staticAttr1ValueMax", jewelryTemplate.mStaticAttr1ValueMax))
		{
			LOG_WARN("Failed to load new_jewelry.csv for [staticAttr1ValueMax]");
			return false;
		}

		if (!csvReader.bind("polishCount", jewelryTemplate.mPolishCount))
		{
			LOG_WARN("Failed to load new_jewelry.csv for [polishCount]");
			return false;
		}
		
		if (!csvReader.bind("price", jewelryTemplate.mPrice))
		{
			LOG_WARN("Failed to load new_jewelry.csv for [price]");
			return false;
		}

		if (!csvReader.bind("refreshCoin", jewelryTemplate.mRefreshCoin))
		{
			LOG_WARN("Failed to load new_jewelry.csv for [refreshCoin]");
			return false;
		}

		mMap.insert(jewelryTemplate.mJewelryId, jewelryTemplate);
	}
	fileStream.close();
	return true;
}

