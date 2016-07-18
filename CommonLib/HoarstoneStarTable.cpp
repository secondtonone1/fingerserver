#include "HoarstoneStarTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
HoarstoneStarTable::loadFromDbc(const String& fileName)
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
HoarstoneStarTable::reloadFromDbc(const String& fileName)
{
	HoarstoneStarTemplateMap tmpMap = mMap;
	HoarStoneReverseMap tempReverseMap = mReverseMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		mReverseMap = tempReverseMap;
		LOG_WARN("Failed to reload HoarstoneStar table.");
		return false;
	}
	return true;
}

bool 
HoarstoneStarTable::saveToDbc(const String& filePath)
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
HoarstoneStarTable::loadFromCsv(const String& filePath)
{
	mMap.clear();
	mReverseMap.clear();

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
		HoarstoneStarTemplate hoarstoneStarTemp;
		// ¹«ÓÃ
		if (!csvReader.bind("id", hoarstoneStarTemp.mId))
		{
			LOG_WARN("Failed to load hoarstoneStar.csv for [id]");
			return false;
		}

		if (!csvReader.bind("idx", hoarstoneStarTemp.mIdx))
		{
			LOG_WARN("Failed to load hoarstoneStar.csv for [idx]");
			return false;
		}


		if (!csvReader.bind("starlevel", hoarstoneStarTemp.mStarlevel))
		{
			LOG_WARN("Failed to load hoarstoneStar.csv for [starlevel]");
			return false;
		}

		if (!csvReader.bind("needEquipId1", hoarstoneStarTemp.needEquipId1))
		{
			LOG_WARN("Failed to load hoarstoneStar.csv for [needEquipId1]");
			return false;
		}

		if (!csvReader.bind("needEquipId2", hoarstoneStarTemp.needEquipId2))
		{
			LOG_WARN("Failed to load hoarstoneStar.csv for [needEquipId2]");
			return false;
		}

		if (!csvReader.bind("needEquipId3", hoarstoneStarTemp.needEquipId3))
		{
			LOG_WARN("Failed to load hoarstoneStar.csv for [needEquipId3]");
			return false;
		}

		if (!csvReader.bind("needEquipId4", hoarstoneStarTemp.needEquipId4))
		{
			LOG_WARN("Failed to load hoarstoneStar.csv for [needEquipId4]");
			return false;
		}

		if (!csvReader.bind("needEquipId5", hoarstoneStarTemp.needEquipId5))
		{
			LOG_WARN("Failed to load hoarstoneStar.csv for [needEquipId5]");
			return false;
		}

		if (!csvReader.bind("needEquipId6", hoarstoneStarTemp.needEquipId6))
		{
			LOG_WARN("Failed to load hoarstoneStar.csv for [needEquipId6]");
			return false;
		}

		if (!csvReader.bind("attrType1", hoarstoneStarTemp.mAttrType1))
		{
			LOG_WARN("Failed to load hoarstoneStar.csv for [attrType1]");
			return false;
		}
	
		if (!csvReader.bind("attrValue1", hoarstoneStarTemp.mAttrValue1))
		{
			LOG_WARN("Failed to load hoarstoneStar.csv for [attrValue1]");
			return false;
		}

		if (!csvReader.bind("attrType2", hoarstoneStarTemp.mAttrType2))
		{
			LOG_WARN("Failed to load hoarstoneStar.csv for [attrType2]");
			return false;
		}

		if (!csvReader.bind("attrValue2", hoarstoneStarTemp.mAttrValue2))
		{
			LOG_WARN("Failed to load hoarstoneStar.csv for [attrValue2]");
			return false;
		}

		if (!csvReader.bind("attrType3", hoarstoneStarTemp.mAttrType3))
		{
			LOG_WARN("Failed to load hoarstoneStar.csv for [attrType3]");
			return false;
		}

		if (!csvReader.bind("attrValue3", hoarstoneStarTemp.mAttrValue3))
		{
			LOG_WARN("Failed to load hoarstoneStar.csv for [attrValue3]");
			return false;
		}

		if (!csvReader.bind("attrType4", hoarstoneStarTemp.mAttrType4))
		{
			LOG_WARN("Failed to load hoarstoneStar.csv for [attrType4]");
			return false;
		}

		if (!csvReader.bind("attrValue4", hoarstoneStarTemp.mAttrValue4))
		{
			LOG_WARN("Failed to load hoarstoneStar.csv for [attrValue4]");
			return false;
		}

		if (!csvReader.bind("attrType5", hoarstoneStarTemp.mAttrType5))
		{
			LOG_WARN("Failed to load hoarstoneStar.csv for [attrType5]");
			return false;
		}

		if (!csvReader.bind("attrValue5", hoarstoneStarTemp.mAttrValue5))
		{
			LOG_WARN("Failed to load hoarstoneStar.csv for [attrValue5]");
			return false;
		}

		mMap.insert(hoarstoneStarTemp.mId, hoarstoneStarTemp);

		HoarStoneReverseMap::Iter * reverseMapIter = mReverseMap.find(hoarstoneStarTemp.mIdx);
		
		if(reverseMapIter)
		{
			reverseMapIter->mValue.insert(hoarstoneStarTemp.mStarlevel, hoarstoneStarTemp);
		}
		else
		{
			Map<UInt32, HoarstoneStarTemplate > lvHoarStoneMap;
			lvHoarStoneMap.insert(hoarstoneStarTemp.mStarlevel, hoarstoneStarTemp);
			mReverseMap.insert(hoarstoneStarTemp.mIdx, lvHoarStoneMap);
		}

	}
	fileStream.close();
	return true;
}

