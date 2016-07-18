#include "EquipGemRuleTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
EquipGemRuleTable::loadFromDbc(const String& fileName)
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
EquipGemRuleTable::reloadFromDbc(const String& fileName)
{
	EquipGemRuleTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		
		LOG_WARN("Failed to reload EquipGemRule table.");
		return false;
	}
	return true;
}

bool 
EquipGemRuleTable::saveToDbc(const String& filePath)
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
EquipGemRuleTable::loadFromCsv(const String& filePath)
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
		
		EquipGemRuleTemplate equipGemRuleTemp;
		// ¹«ÓÃ
		if (!csvReader.bind("equipid", equipGemRuleTemp.mEquipId))
		{
			LOG_WARN("Failed to load equip_gem_rule.csv for [equipid]");
			return false;
		}

		if (!csvReader.bind("gemLevel1", equipGemRuleTemp.mGemLevel1))
		{
			LOG_WARN("Failed to load equip_gem_rule.csv for [gemLevel1]");
			return false;
		}
		
		if (!csvReader.bind("condition1", equipGemRuleTemp.mCondition1))
		{
			LOG_WARN("Failed to load equip_gem_rule.csv for [condition1]");
			return false;
		}

		dealCondition(equipGemRuleTemp.mCondKey1,equipGemRuleTemp.mCondValue1,equipGemRuleTemp.mCondition1.c_str());
		
		
		 
		
		

		if (!csvReader.bind("gemLevel2", equipGemRuleTemp.mGemLevel2))
		{
			LOG_WARN("Failed to load equip_gem_rule.csv for [gemLevel2]");
			return false;
		}

		if (!csvReader.bind("condition2", equipGemRuleTemp.mCondition2))
		{
			LOG_WARN("Failed to load equip_gem_rule.csv for [condition2]");
			return false;
		}


		dealCondition(equipGemRuleTemp.mCondKey2,equipGemRuleTemp.mCondValue2,equipGemRuleTemp.mCondition2.c_str());

		if (!csvReader.bind("gemLevel3", equipGemRuleTemp.mGemLevel3))
		{
			LOG_WARN("Failed to load equip_gem_rule.csv for [gemLevel3]");
			return false;
		}

		if (!csvReader.bind("condition3", equipGemRuleTemp.mCondition3))
		{
			LOG_WARN("Failed to load equip_gem_rule.csv for [condition3]");
			return false;
		}

		dealCondition(equipGemRuleTemp.mCondKey3,equipGemRuleTemp.mCondValue3,equipGemRuleTemp.mCondition3.c_str());

		if (!csvReader.bind("gemLevel4", equipGemRuleTemp.mGemLevel4))
		{
			LOG_WARN("Failed to load equip_gem_rule.csv for [gemLevel4]");
			return false;
		}

		if (!csvReader.bind("condition4", equipGemRuleTemp.mCondition4))
		{
			LOG_WARN("Failed to load equip_gem_rule.csv for [condition4]");
			return false;
		}

		dealCondition(equipGemRuleTemp.mCondKey4,equipGemRuleTemp.mCondValue4,equipGemRuleTemp.mCondition4.c_str());

		if (!csvReader.bind("gemLevel5", equipGemRuleTemp.mGemLevel5))
		{
			LOG_WARN("Failed to load equip_gem_rule.csv for [gemLevel5]");
			return false;
		}

		if (!csvReader.bind("condition5", equipGemRuleTemp.mCondition5))
		{
			LOG_WARN("Failed to load equip_gem_rule.csv for [condition5]");
			return false;
		}

		dealCondition(equipGemRuleTemp.mCondKey5,equipGemRuleTemp.mCondValue5,equipGemRuleTemp.mCondition5.c_str());

		if (!csvReader.bind("gemLevel6", equipGemRuleTemp.mGemLevel6))
		{
			LOG_WARN("Failed to load equip_gem_rule.csv for [gemLevel6]");
			return false;
		}

		if (!csvReader.bind("condition6", equipGemRuleTemp.mCondition6))
		{
			LOG_WARN("Failed to load equip_gem_rule.csv for [condition6]");
			return false;
		}

		dealCondition(equipGemRuleTemp.mCondKey6,equipGemRuleTemp.mCondValue6,equipGemRuleTemp.mCondition6.c_str());

		mMap.insert(equipGemRuleTemp.mEquipId, equipGemRuleTemp);

		
	}
	fileStream.close();
	return true;
}

void EquipGemRuleTable::dealCondition(UInt32 & condKey, UInt32 & condValue, std::string condStr)
{
	
	
		std::string ::size_type  findIndex = condStr.find(';');
		if(findIndex != std::string::npos)
		{
			std::string condKeyStr = condStr.substr(0,findIndex);
			
			std::stringstream mystream;

			mystream.clear();
			mystream.str("");

			mystream << condKeyStr;

			UInt32 tempKey;
			mystream >> tempKey;
		
			condKey = tempKey;

			mystream.clear();
			mystream.str("");

			std::string condValueStr = condStr.substr(findIndex + 1);
			mystream << condValueStr;
			UInt32 tempValue;
			mystream >> tempValue;

			condValue = tempValue;
			
		}
		else
		{
			condKey = 0;
			condValue = 0;
		}
	
}