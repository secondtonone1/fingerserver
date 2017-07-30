#include "MysteryShopTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
ShopsTable::loadFromDbc(const String& fileName)
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
ShopsTable::reloadFromDbc(const String& fileName)
{
	ShopsTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload ShopsTable table.");
		return false;
	}
	return true;
}

bool 
ShopsTable::saveToDbc(const String& filePath)
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
ShopsTable::loadFromCsv(const String& filePath)
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
		ShopsTemplate shopsTemplate;

		if (!csvReader.bind("id", shopsTemplate.id))
		{
			LOG_WARN("Failed to load shopsTemplate.csv for [id]");
			return false;
		}

		if (!csvReader.bind("shoptype", shopsTemplate.shoptype))
		{
// 			LOG_WARN("Failed to load shopsTemplate.csv for [name]");
// 			return false;
		}

		char str1[32]={};
		char str2[32]={};
		char str3[32]={};
		char str4[32]={};
		for (int i=1;i<50;i++)
		{
			Sell sell;
			sprintf(str1, "%s%d","restype",i);
			sprintf(str2, "%s%d","subtype",i);
			sprintf(str3, "%s%d","num",i);
			sprintf(str4, "%s%d","cost",i);


			if (!csvReader.bind(str1, sell.restype))
			{
				LOG_WARN("Failed to load shopsTemplate.csv for [type]");
				break;
			}
			if (sell.restype == 0)
			{
				break;
			}

			if (!csvReader.bind(str2, sell.subtype))
			{
				LOG_WARN("Failed to load shopsTemplate.csv for [subtype]");
				break;
			}
			if (!csvReader.bind(str3, sell.num))
			{
				LOG_WARN("Failed to load shopsTemplate.csv for [num]");
				break;
			}
			if (!csvReader.bind(str4, sell.cost))
			{
				LOG_WARN("Failed to load shopsTemplate.csv for [cost]");
				break;
			}

			shopsTemplate.sellList.insertTail(sell);

		}	
		
		mMap.insert(shopsTemplate.id, shopsTemplate);
	}
	fileStream.close();
	return true;
}

