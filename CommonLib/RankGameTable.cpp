#include "RankGameTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
RankedShopTable::loadFromDbc(const String& fileName)
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
RankedShopTable::reloadFromDbc(const String& fileName)
{
	RankedShopTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload RankedShopTemplate table.");
		return false;
	}
	return true;
}

bool 
RankedShopTable::saveToDbc(const String& filePath)
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
RankedShopTable::loadFromCsv(const String& filePath)
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
		RankedShopTemplate RankedShopTemplate;
		// 公用
		if (!csvReader.bind("id", RankedShopTemplate.id))
		{
			LOG_WARN("Failed to load RankedShop.csv for [id]");
			return false;
		}

		if (!csvReader.bind("resType", RankedShopTemplate.resType))
		{
			LOG_WARN("Failed to load RankedShop.csv for [resType]");
			return false;
		}

		if (!csvReader.bind("subType", RankedShopTemplate.subType))
		{
			LOG_WARN("Failed to load RankedShop.csv for [subType]");
			return false;
		}

		if (!csvReader.bind("num", RankedShopTemplate.num))
		{
			LOG_WARN("Failed to load RankedShop.csv for [num]");
			return false;
		}

		if (!csvReader.bind("costsubType", RankedShopTemplate.costsubType))
		{
			LOG_WARN("Failed to load RankedShop.csv for [costsubType]");
			return false;
		}
		if (!csvReader.bind("costresType", RankedShopTemplate.costresType))
		{
			LOG_WARN("Failed to load RankedShop.csv for [costresType]");
			return false;
		}

		if (!csvReader.bind("costnum", RankedShopTemplate.costnum))
		{
			LOG_WARN("Failed to load RankedShop.csv for [costnum]");
			return false;
		}


		mMap.insert(RankedShopTemplate.id, RankedShopTemplate);
	}
	fileStream.close();
	return true;
}



bool 
RankRewardTable::loadFromDbc(const String& fileName)
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
RankRewardTable::reloadFromDbc(const String& fileName)
{
	RankRewardTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload RankRewardTemplate table.");
		return false;
	}
	return true;
}

bool 
RankRewardTable::saveToDbc(const String& filePath)
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
RankRewardTable::loadFromCsv(const String& filePath)
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
		RankRewardTemplate RankRewardTemplate;
		// 公用
		if (!csvReader.bind("id", RankRewardTemplate.id))
		{
			LOG_WARN("Failed to load RankReward.csv for [id]");
			return false;
		}

		if (!csvReader.bind("coin", RankRewardTemplate.coin))
		{
			LOG_WARN("Failed to load RankReward.csv for [coin]");
			return false;
		}

		if (!csvReader.bind("gold", RankRewardTemplate.gold))
		{
			LOG_WARN("Failed to load RankReward.csv for [gold]");
			return false;
		}

		if (!csvReader.bind("score", RankRewardTemplate.score))
		{
			LOG_WARN("Failed to load RankReward.csv for [score]");
			return false;
		}

		if (!csvReader.bind("min", RankRewardTemplate.min))
		{
			LOG_WARN("Failed to load RankReward.csv for [min]");
			return false;
		}
		if (!csvReader.bind("max", RankRewardTemplate.max))
		{
			LOG_WARN("Failed to load RankReward.csv for [max]");
			return false;
		}

		if (!csvReader.bind("type", RankRewardTemplate.type))
		{
			LOG_WARN("Failed to load RankReward.csv for [type]");
			return false;
		}
		if (!csvReader.bind("descIndex", RankRewardTemplate.descIndex))
		{
			LOG_WARN("Failed to load RankReward.csv for [descIndex]");
			return false;
		}

		mMap.insert(RankRewardTemplate.id, RankRewardTemplate);
	}
	fileStream.close();
	return true;
}


