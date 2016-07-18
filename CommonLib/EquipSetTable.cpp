#include "EquipSetTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
EquipSetTable::loadFromDbc(const String& fileName)
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
EquipSetTable::reloadFromDbc(const String& fileName)
{
	EquipSetTemplateMap tmpMap = mMap;
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
EquipSetTable::saveToDbc(const String& filePath)
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
EquipSetTable::loadFromCsv(const String& filePath)
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
		EquipSetTemplate equipSetTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("setId", equipSetTemplate.mSetId))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [setId]");
			return false;
		}

		if (!csvReader.bind("name", equipSetTemplate.mName))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [name]");
			return false;
		}

		if (!csvReader.bind("desc", equipSetTemplate.mDesc))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [desc]");
			return false;
		}

		if (!csvReader.bind("list1", equipSetTemplate.mList1))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [list1]");
			return false;
		}

		if (!csvReader.bind("list2", equipSetTemplate.mList2))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [list2]");
			return false;
		}
	
		if (!csvReader.bind("list3", equipSetTemplate.mList3))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [list3]");
			return false;
		}

		if (!csvReader.bind("list4", equipSetTemplate.mList4))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [list4]");
			return false;
		}

		if (!csvReader.bind("list5", equipSetTemplate.mList5))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [list5]");
			return false;
		}

		if (!csvReader.bind("list6", equipSetTemplate.mList6))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [list6]");
			return false;
		}

		if (!csvReader.bind("list7", equipSetTemplate.mList7))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [list7]");
			return false;
		}

		if (!csvReader.bind("list8", equipSetTemplate.mList8))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [list8]");
			return false;
		}

		if (!csvReader.bind("list9", equipSetTemplate.mList9))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [list9]");
			return false;
		}

		if (!csvReader.bind("attrRequireEquipCount1", equipSetTemplate.mAttrRequireEquipCount1))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [attrRequireEquipCount1]");
			return false;
		}

		if (!csvReader.bind("attrType1", equipSetTemplate.mAttrType1))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [attrType1]");
			return false;
		}

		if (!csvReader.bind("attrValue1", equipSetTemplate.mAttrValue1))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [attrValue1]");
			return false;
		}

		if (!csvReader.bind("attrRequireEquipCount2", equipSetTemplate.mAttrRequireEquipCount2))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [attrRequireEquipCount2]");
			return false;
		}

		if (!csvReader.bind("attrType2", equipSetTemplate.mAttrType2))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [attrType2]");
			return false;
		}

		if (!csvReader.bind("attrValue2", equipSetTemplate.mAttrValue2))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [attrValue2]");
			return false;
		}

		if (!csvReader.bind("attrRequireEquipCount3", equipSetTemplate.mAttrRequireEquipCount3))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [attrRequireEquipCount3]");
			return false;
		}

		if (!csvReader.bind("attrType3", equipSetTemplate.mAttrType3))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [attrType3]");
			return false;
		}

		if (!csvReader.bind("attrValue3", equipSetTemplate.mAttrValue3))
		{
			LOG_WARN("Failed to load equipSetTemplate.csv for [attrValue3]");
			return false;
		}

		

		mMap.insert(equipSetTemplate.mSetId, equipSetTemplate);
	}
	fileStream.close();
	return true;
}

