#include "CourageContentTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
CoinTable::loadFromDbc(const String& fileName)
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
CoinTable::reloadFromDbc(const String& fileName)
{
	CoinTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload CoinTemplate table.");
		return false;
	}
	return true;
}

bool 
CoinTable::saveToDbc(const String& filePath)
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
CoinTable::loadFromCsv(const String& filePath)
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
		CoinTemplate coinTemplate;
		// 公用
		if (!csvReader.bind("id", coinTemplate.id))
		{
			LOG_WARN("Failed to load Coin.csv for [id]");
			return false;
		}

		if (!csvReader.bind("monsterid", coinTemplate.monsterid))
		{
			LOG_WARN("Failed to load Coin.csv for [monsterid]");
			return false;
		}

		if (!csvReader.bind("light", coinTemplate.light))
		{
			LOG_WARN("Failed to load Coin.csv for [light]");
			return false;
		}

		if (!csvReader.bind("quality", coinTemplate.quality))
		{
			LOG_WARN("Failed to load Coin.csv for [quality]");
			return false;
		}

		if (!csvReader.bind("territory", coinTemplate.territory))
		{
			LOG_WARN("Failed to load Coin.csv for [territory]");
			return false;
		}

		mMap.insert(coinTemplate.id, coinTemplate);
	}
	fileStream.close();
	return true;
}



bool 
CoinGroupTable::loadFromDbc(const String& fileName)
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
CoinGroupTable::reloadFromDbc(const String& fileName)
{
	CoinGroupTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload CoinGroupTemplate table.");
		return false;
	}
	return true;
}

bool 
CoinGroupTable::saveToDbc(const String& filePath)
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
CoinGroupTable::loadFromCsv(const String& filePath)
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
		CoinGroupTemplate CoinGroupTemplate;
		// 公用
		if (!csvReader.bind("id", CoinGroupTemplate.id))
		{
			LOG_WARN("Failed to load CoinGroup.csv for [id]");
			return false;
		}

		if (!csvReader.bind("territoryID", CoinGroupTemplate.territoryID))
		{
			LOG_WARN("Failed to load CoinGroup.csv for [territoryID]");
			return false;
		}

		if (!csvReader.bind("des", CoinGroupTemplate.des))
		{
			LOG_WARN("Failed to load CoinGroup.csv for [des]");
			return false;
		}

		if (!csvReader.bind("attr1", CoinGroupTemplate.attr1))
		{
			LOG_WARN("Failed to load CoinGroup.csv for [attr1]");
			return false;
		}

		if (!csvReader.bind("percent1", CoinGroupTemplate.percent1))
		{
			LOG_WARN("Failed to load CoinGroup.csv for percent1]");
			return false;
		}

		if (!csvReader.bind("coin1", CoinGroupTemplate.coin1))
		{
			LOG_WARN("Failed to load CoinGroup.csv for [coin1]");
			return false;
		}

		if (!csvReader.bind("num1", CoinGroupTemplate.num1))
		{
			LOG_WARN("Failed to load CoinGroup.csv for [num1]");
			return false;
		}

		if (!csvReader.bind("coin2", CoinGroupTemplate.coin2))
		{
			LOG_WARN("Failed to load CoinGroup.csv for [coin2]");
			return false;
		}

		if (!csvReader.bind("num2", CoinGroupTemplate.num2))
		{
			LOG_WARN("Failed to load CoinGroup.csv for [num2]");
			return false;
		}

		if (!csvReader.bind("coin3", CoinGroupTemplate.coin3))
		{
			LOG_WARN("Failed to load CoinGroup.csv for [coin3]");
			return false;
		}

		if (!csvReader.bind("num3", CoinGroupTemplate.num3))
		{
			LOG_WARN("Failed to load CoinGroup.csv for [num3]");
			return false;
		}


		mMap.insert(CoinGroupTemplate.id, CoinGroupTemplate);
	}
	fileStream.close();
	return true;
}

bool 
CourageContentTable::loadFromDbc(const String& fileName)
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
CourageContentTable::reloadFromDbc(const String& fileName)
{
	mAwardContentList tmpMap =  mContentList;
	mContentList.clear();
	if (!loadFromDbc(fileName))
	{
		mContentList = tmpMap;
		LOG_WARN("Failed to reload CourageContentTemplate table.");
		return false;
	}
	return true;
}

bool 
CourageContentTable::saveToDbc(const String& filePath)
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
CourageContentTable::loadFromCsv(const String& filePath)
{
	mContentList.clear();
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
		AwardContentTemplate awardContentTemplate;
		// 公用
		if (!csvReader.bind("couragemonsterID", awardContentTemplate.id))
		{	
			LOG_WARN("Failed to load AwardContent.csv for [couragemonsterID]");
			return false;
		}
		char str1[32]={};
		char str2[32]={};
		char str3[32]={};
		char str4[32]={};

		for (int i=1;i<100;i++)
		{
			Content content;

			sprintf(str1, "%s%d","partType",i);
			sprintf(str2, "%s%d","weight",i);
			sprintf(str3, "%s%d","monsterID",i);
			sprintf(str4, "%s%d","Award",i);

			if (!csvReader.bind(str1, content.contenttype))
			{
				LOG_WARN("Failed to load couragecontent.csv for [partType]");
				break;
			}

			if (content.contenttype == 0)
			{
				break;
			}
			if (!csvReader.bind(str2, content.weight))
			{
				LOG_WARN("Failed to load couragecontent.csv for [partType]");
				break;
			}
			content.resourcestype = 10000;
			
			if (!csvReader.bind(str3, content.subtype))
			{
				LOG_WARN("Failed to load couragecontent.csv for [monsterID]");
				break;
			}
			if (!csvReader.bind(str4, content.num))
			{
				LOG_WARN("Failed to load couragecontent.csv for [awardcontent]");
				break;
			}
			awardContentTemplate.ContentList.insertTail(content);
		}
		mContentList.insertTail(awardContentTemplate);
	}
	fileStream.close();
	return true;
}


bool 
TerritoryTable::loadFromDbc(const String& fileName)
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
TerritoryTable::reloadFromDbc(const String& fileName)
{
	TerritoryTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload TerritoryTemplate table.");
		return false;
	}
	return true;
}

bool 
TerritoryTable::saveToDbc(const String& filePath)
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
TerritoryTable::loadFromCsv(const String& filePath)
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
		TerritoryTemplate territoryTemplate;
		// 公用
		if (!csvReader.bind("id", territoryTemplate.id))
		{
			LOG_WARN("Failed to load Territory.csv for [id]");
			return false;
		}

		if (!csvReader.bind("name", territoryTemplate.name))
		{
			LOG_WARN("Failed to load Territory.csv for [name]");
			return false;
		}

		if (!csvReader.bind("condition1", territoryTemplate.condition1))
		{
			LOG_WARN("Failed to load Territory.csv for [condition1]");
			return false;
		}

		if (!csvReader.bind("condition2", territoryTemplate.condition2))
		{
			LOG_WARN("Failed to load Territory.csv for [condition2]");
			return false;
		}

// 		if (!csvReader.bind("couragemonster", territoryTemplate.couragemonster))
// 		{
// 			LOG_WARN("Failed to load Territory.csv for [couragemonster]");
// 			return false;
// 		}

		char str1[32]={};
		char str2[32]={};

		for (int i=1;i<100;i++)
		{
			KeyValue keyValue;

			sprintf(str1, "%s%d","limit",i);
			sprintf(str2, "%s%d","couragemonster",i);


			if (!csvReader.bind(str1, keyValue.key))
			{
				LOG_WARN("Failed to load Territory.csv for [limit]");
				break;
			}

			if (keyValue.key == 0)
			{
				break;
			}

			if (!csvReader.bind(str2, keyValue.value))
			{
				LOG_WARN("Failed to load Territory.csv for [limit]");
				break;
			}
			territoryTemplate.conditionMonsters.insertTail(keyValue);			
		}

		mMap.insert(territoryTemplate.id, territoryTemplate);
	}
	fileStream.close();
	return true;
	
}
