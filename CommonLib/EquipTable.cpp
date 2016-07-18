#include "EquipTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
EquipTable::loadFromDbc(const String& fileName)
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
EquipTable::reloadFromDbc(const String& fileName)
{
	EquipTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload BuffTemplate table.");
		return false;
	}
	return true;
}

bool 
EquipTable::saveToDbc(const String& filePath)
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
EquipTable::loadFromCsv(const String& filePath)
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
		EquipTemplate equipTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("itemId", equipTemplate.mId))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [itemId]");
			return false;
		}

		if (!csvReader.bind("res", equipTemplate.mRes))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [res]");
			return false;
		}

		if (!csvReader.bind("levelRequire", equipTemplate.mLevelRequire))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [levelRequire]");
			return false;
		}

		if (!csvReader.bind("equipType", equipTemplate.mEquipType))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [equipType]");
			return false;
		}

		if (!csvReader.bind("equipPos", equipTemplate.mEquipPos))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [equipPos]");
			return false;
		}

		if (!csvReader.bind("equipHole", equipTemplate.mEquipHole))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [equipHole]");
			return false;
		}

		if (!csvReader.bind("maxStar", equipTemplate.mMaxStar))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [maxStar]");
			return false;
		}

		if (!csvReader.bind("effect", equipTemplate.mEffect))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [effect]");
			return false;
		}

		if (!csvReader.bind("equipSet", equipTemplate.mEquipSet))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [equipSet]");
			return false;
		}

		if (!csvReader.bind("materialExp", equipTemplate.mMaterialExp))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [materialExp]");
			return false;
		}

		if (!csvReader.bind("resID", equipTemplate.mResID))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [resID]");
			return false;
		}

		if (!csvReader.bind("staticAttrType1", equipTemplate.mStaticAttrType1))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [staticAttrType1]");
			return false;
		}

		if (!csvReader.bind("staticAttrValMin1", equipTemplate.mStaticAttrValMin1))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [staticAttrValMin1]");
			return false;
		}

		if (!csvReader.bind("staticAttrValMax1", equipTemplate.mStaticAttrValMax1))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [staticAttrValMax1]");
			return false;
		}

		if (!csvReader.bind("staticAttrType2", equipTemplate.mStaticAttrType2))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [staticAttrType2]");
			return false;
		}

		if (!csvReader.bind("staticAttrValMin2", equipTemplate.mStaticAttrValMin2))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [staticAttrValMin1]");
			return false;
		}

		if (!csvReader.bind("staticAttrValMax2", equipTemplate.mStaticAttrValMax2))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [staticAttrValMax2]");
			return false;
		}

		if (!csvReader.bind("staticAttrType3", equipTemplate.mStaticAttrType3))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [staticAttrValMin1]");
			return false;
		}

		if (!csvReader.bind("staticAttrValMin3", equipTemplate.mStaticAttrValMin3))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [staticAttrValMin3]");
			return false;
		}

		if (!csvReader.bind("staticAttrValMax3", equipTemplate.mStaticAttrValMax3))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [staticAttrValMax3]");
			return false;
		}

		if (!csvReader.bind("randomRuleId1", equipTemplate.mRandomRuleId1))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [randomRuleId1]");
			return false;
		}
		
		if (!csvReader.bind("randomRuleId2", equipTemplate.mRandomRuleId2))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [randomRuleId2]");
			return false;
		}

		if (!csvReader.bind("randomRuleId3", equipTemplate.mRandomRuleId3))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [randomRuleId3]");
			return false;
		}

		if (!csvReader.bind("randomRuleId4", equipTemplate.mRandomRuleId4))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [randomRuleId4]");
			return false;
		}

		if (!csvReader.bind("randomRuleId5", equipTemplate.mRandomRuleId5))
		{
			LOG_WARN("Failed to load equipTemplate.csv for [randomRuleId5]");
			return false;
		}

		if(!csvReader.bind("refreshcoin", equipTemplate.mRefreshcoin))
		{
			LOG_WARN("Failed to load equipTemplate.csv for[refreshcoin]");
			return false;
		}

		if(!csvReader.bind("refreshstone", equipTemplate.mRefreshstone))
		{
			LOG_WARN("Failed to load equipTemplate.csv for[refreshstone]");
			return false;
		}

		if(!csvReader.bind("quality", equipTemplate.mQuality))
		{
			LOG_WARN("Failed to load equipTemplate.csv for[quality]");
			return false;
		}

		mMap.insert(equipTemplate.mId, equipTemplate);
	}
	fileStream.close();
	return true;
}

