#include "HeroEquipRuleTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
HeroEquipRuleTable::loadFromDbc(const String& fileName)
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
HeroEquipRuleTable::reloadFromDbc(const String& fileName)
{
	HeroEquipRuleTemplateMap tmpMap = mMap;
	
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		
		LOG_WARN("Failed to reload HeroEquipRuleTable table.");
		return false;
	}
	return true;
}

bool 
HeroEquipRuleTable::saveToDbc(const String& filePath)
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
HeroEquipRuleTable::loadFromCsv(const String& filePath)
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
		HeroEquipRuleTemplate heroEquipRuleTemp;
		// ¹«ÓÃ
		if (!csvReader.bind("heroId", heroEquipRuleTemp.mHeroId))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [heroId]");
			return false;
		}

		if (!csvReader.bind("equipId1", heroEquipRuleTemp.mEquipId1))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [equipId1]");
			return false;
		}

		if (!csvReader.bind("equipId2", heroEquipRuleTemp.mEquipId2))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [equipId2]");
			return false;
		}

		if (!csvReader.bind("equipId3", heroEquipRuleTemp.mEquipId3))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [equipId3]");
			return false;
		}

		if (!csvReader.bind("equipId4", heroEquipRuleTemp.mEquipId4))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [equipId4]");
			return false;
		}

		if (!csvReader.bind("equipId5", heroEquipRuleTemp.mEquipId5))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [equipId5]");
			return false;
		}
		
		if (!csvReader.bind("equipId6", heroEquipRuleTemp.mEquipId6))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [equipId6]");
			return false;
		}
		
		if (!csvReader.bind("equip1UnlockLevel", heroEquipRuleTemp.mEquip1UnlockLv))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [equip1UnlockLevel]");
			return false;
		}
		
		if (!csvReader.bind("equip2UnlockLevel", heroEquipRuleTemp.mEquip2UnlockLv))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [equip2UnlockLevel]");
			return false;
		}
		
		if (!csvReader.bind("equip3UnlockLevel", heroEquipRuleTemp.mEquip3UnlockLv))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [equip3UnlockLevel]");
			return false;
		}

		if (!csvReader.bind("equip4UnlockLevel", heroEquipRuleTemp.mEquip4UnlockLv))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [equip4UnlockLevel]");
			return false;
		}

		if (!csvReader.bind("equip5UnlockLevel", heroEquipRuleTemp.mEquip5UnlockLv))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [equip5UnlockLevel]");
			return false;
		}

		if (!csvReader.bind("equip6UnlockLevel", heroEquipRuleTemp.mEquip6UnlockLv))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [equip6UnlockLevel]");
			return false;
		}

		
		if (!csvReader.bind("jewelry1UnlockLevel", heroEquipRuleTemp.mJewelry1UnlockLv))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [jewelry1UnlockLevel]");
			return false;
		}

		if (!csvReader.bind("jewelry2UnlockLevel", heroEquipRuleTemp.mJewelry2UnlockLv))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [jewelry2UnlockLevel]");
			return false;
		}

		if (!csvReader.bind("jewelry3UnlockLevel", heroEquipRuleTemp.mJewelry3UnlockLv))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [jewelry3UnlockLevel]");
			return false;
		}

		if (!csvReader.bind("jewelry4UnlockLevel", heroEquipRuleTemp.mJewelry4UnlockLv))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [jewelry4UnlockLevel]");
			return false;
		}
		
		if (!csvReader.bind("jewelry5UnlockLevel", heroEquipRuleTemp.mJewelry5UnlockLv))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [jewelry5UnlockLevel]");
			return false;
		}

		if (!csvReader.bind("jewelry6UnlockLevel", heroEquipRuleTemp.mJewelry6UnlockLv))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [jewelry6UnlockLevel]");
			return false;
		}

		mMap.insert(heroEquipRuleTemp.mHeroId,heroEquipRuleTemp);
	}
	fileStream.close();
	return true;
}

