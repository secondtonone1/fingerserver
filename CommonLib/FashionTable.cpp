#include "FashionTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
FashionTable::loadFromDbc(const String& fileName)
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
FashionTable::reloadFromDbc(const String& fileName)
{
	FahionTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload FashionTemplate table.");
		return false;
	}
	return true;
}

bool 
FashionTable::saveToDbc(const String& filePath)
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
FashionTable::loadFromCsv(const String& filePath)
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
		FahionTemplate fashionTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("id", fashionTemplate.mId))
		{
			LOG_WARN("Failed to load fashionTemplate.csv for [id]");
			return false;
		}

		if (!csvReader.bind("name", fashionTemplate.mName))
		{
			LOG_WARN("Failed to load fashionTemplate.csv for [name]");
			return false;
		}
	
		if (!csvReader.bind("desc", fashionTemplate.mDesc))
		{
			LOG_WARN("Failed to load fashionTemplate.csv for [desc]");
			return false;
		}
	
		if (!csvReader.bind("belong", fashionTemplate.mBelong))
		{
			LOG_WARN("Failed to load fashionTemplate.csv for [belong]");
			return false;
		}

		if (!csvReader.bind("icon", fashionTemplate.mIcon))
		{
			LOG_WARN("Failed to load fashionTemplate.csv for [icon]");
			return false;
		}

		if (!csvReader.bind("quality", fashionTemplate.mQuality))
		{
			LOG_WARN("Failed to load fashionTemplate.csv for [quality]");
			return false;
		}

		if (!csvReader.bind("resID", fashionTemplate.mResID))
		{
			LOG_WARN("Failed to load fashionTemplate.csv for [resID]");
			return false;
		}


		if (!csvReader.bind("limitVip", fashionTemplate.mLimitVip))
		{
			LOG_WARN("Failed to load fashionTemplate.csv for [limitVip]");
			return false;
		}

		if (!csvReader.bind("limitRMB", fashionTemplate.mLimitRMB))
		{
			LOG_WARN("Failed to load fashionTemplate.csv for [limitRMB]");
			return false;
		}

		

		if (!csvReader.bind("attrPA", fashionTemplate.mAttrPA))
		{
			LOG_WARN("Failed to load fashionTemplate.csv for [attrPA]");
			return false;
		}

		if (!csvReader.bind("attrPF", fashionTemplate.mAttrPF))
		{
			LOG_WARN("Failed to load fashionTemplate.csv for [attrPF]");
			return false;
		}

		if (!csvReader.bind("attrMA", fashionTemplate.mAttrMA))
		{
			LOG_WARN("Failed to load fashionTemplate.csv for [attrMA]");
			return false;
		}

		if (!csvReader.bind("attrMF", fashionTemplate.mAttrMF))
		{
			LOG_WARN("Failed to load fashionTemplate.csv for [attrMF]");
			return false;
		}

		if (!csvReader.bind("attrMaxHP", fashionTemplate.mAttrMaxHP))
		{
			LOG_WARN("Failed to load fashionTemplate.csv for [attrMaxHP]");
			return false;
		}

		
		if (!csvReader.bind("attrMaxMP", fashionTemplate.mAttrMaxMP))
		{
			LOG_WARN("Failed to load fashionTemplate.csv for [attrMaxMP]");
			return false;
		}

		if (!csvReader.bind("attrMaxSP", fashionTemplate.mAttrMaxSP))
		{
			LOG_WARN("Failed to load fashionTemplate.csv for [attrMaxSP]");
			return false;
		}

		if (!csvReader.bind("attrCrit", fashionTemplate.mAttrCrit))
		{
			LOG_WARN("Failed to load fashionTemplate.csv for [attrCrit]");
			return false;
		}


		mMap.insert(fashionTemplate.mId, fashionTemplate);
	}
	fileStream.close();
	return true;
}

