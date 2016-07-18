#include "HoarstoneRuneTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
HoarstoneRuneTable::loadFromDbc(const String& fileName)
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
HoarstoneRuneTable::reloadFromDbc(const String& fileName)
{
	HoarstoneRuneTemplateMap tmpMap = mMap;
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
HoarstoneRuneTable::saveToDbc(const String& filePath)
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
HoarstoneRuneTable::loadFromCsv(const String& filePath)
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
		HoarstoneRuneTemplate hoarstoneRuneTemp;
		// ¹«ÓÃ
		if (!csvReader.bind("id", hoarstoneRuneTemp.mId))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [id]");
			return false;
		}

		if (!csvReader.bind("name", hoarstoneRuneTemp.mName))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [name]");
			return false;
		}
		
		if (!csvReader.bind("res", hoarstoneRuneTemp.mRes))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [res]");
			return false;
		}

		if (!csvReader.bind("attrType1", hoarstoneRuneTemp.mAttrType1))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [attrType1]");
			return false;
		}

		if (!csvReader.bind("attrValue1", hoarstoneRuneTemp.mAttrValue1))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [attrValue1]");
			return false;
		}

		if (!csvReader.bind("attrType2", hoarstoneRuneTemp.mAttrType2))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [attrType2]");
			return false;
		}


		if (!csvReader.bind("attrValue2", hoarstoneRuneTemp.mAttrValue2))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [attrValue2]");
			return false;
		}

		if (!csvReader.bind("attrType3", hoarstoneRuneTemp.mAttrType3))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [attrType3]");
			return false;
		}


		if (!csvReader.bind("attrValue3", hoarstoneRuneTemp.mAttrValue3))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [attrValue3]");
			return false;
		}

		if (!csvReader.bind("attrType4", hoarstoneRuneTemp.mAttrType4))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [attrType4]");
			return false;
		}


		if (!csvReader.bind("attrValue4", hoarstoneRuneTemp.mAttrValue4))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [attrValue4]");
			return false;
		}

		if (!csvReader.bind("attrType5", hoarstoneRuneTemp.mAttrType5))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [attrType5]");
			return false;
		}


		if (!csvReader.bind("attrValue5", hoarstoneRuneTemp.mAttrValue5))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [attrValue5]");
			return false;
		}

		if (!csvReader.bind("quality", hoarstoneRuneTemp.mQuality))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [quality]");
			return false;
		}

		if (!csvReader.bind("combine1", hoarstoneRuneTemp.mCombine1))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [combine1]");
			return false;
		}

		if (!csvReader.bind("amount1", hoarstoneRuneTemp.mAmount1))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [amount1]");
			return false;
		}

		if (!csvReader.bind("combine2", hoarstoneRuneTemp.mCombine2))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [combine2]");
			return false;
		}

		if (!csvReader.bind("amount2", hoarstoneRuneTemp.mAmount2))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [amount2]");
			return false;
		}

		if (!csvReader.bind("combine3", hoarstoneRuneTemp.mCombine3))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [combine3]");
			return false;
		}

		if (!csvReader.bind("amount3", hoarstoneRuneTemp.mAmount3))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [amount3]");
			return false;
		}

		if (!csvReader.bind("combine4", hoarstoneRuneTemp.mCombine4))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [combine4]");
			return false;
		}

		if (!csvReader.bind("amount4", hoarstoneRuneTemp.mAmount4))
		{
			LOG_WARN("Failed to load hoarstoneEquip.csv for [amount4]");
			return false;
		}

		mMap.insert(hoarstoneRuneTemp.mId, hoarstoneRuneTemp);
	}
	fileStream.close();
	return true;
}

