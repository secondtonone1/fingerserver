#include "TowersTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
LotteryActivityTable::loadFromDbc(const String& fileName)
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
LotteryActivityTable::reloadFromDbc(const String& fileName)
{
	LotteryActivityTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload LotteryActivityTable table.");
		return false;
	}
	return true;
}

bool 
LotteryActivityTable::saveToDbc(const String& filePath)
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
LotteryActivityTable::loadFromCsv(const String& filePath)
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
		
		LotteryActivityTemplate LotteryActivityTemplate;
		// 公用
		if (!csvReader.bind("id", LotteryActivityTemplate.id))
		{
			LOG_WARN("Failed to load LotteryActivityTemplate.csv for [id]");
			return false;
		}
		if (!csvReader.bind("starttime", LotteryActivityTemplate.starttime))
		{
			LOG_WARN("Failed to load LotteryActivityTemplate.csv for [starttime]");
			return false;
		}

		if (!csvReader.bind("closetime", LotteryActivityTemplate.closetime))
		{
			LOG_WARN("Failed to load LotteryActivityTemplate.csv for [closetime]");
			return false;
		}
		if (!csvReader.bind("lotteryid", LotteryActivityTemplate.lotteryid))
		{
			LOG_WARN("Failed to load LotteryActivityTemplate.csv for [lotteryid]");
			return false;
		}
		
		mMap.insert(LotteryActivityTemplate.id, LotteryActivityTemplate);		
	}
	fileStream.close();
	return true;
}



bool 
RewardLotteryTable::loadFromDbc(const String& fileName)
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
RewardLotteryTable::reloadFromDbc(const String& fileName)
{
	RewardLotteryTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload RewardLotteryTable table.");
		return false;
	}
	return true;
}

bool 
RewardLotteryTable::saveToDbc(const String& filePath)
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
RewardLotteryTable::loadFromCsv(const String& filePath)
{
	char dest[64] = {};	
	char dest2[64] = {};
	String strData1;	
	String strData2;

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

		RewardLotteryTemplate RewardLotteryTemplate;
		// 公用
		if (!csvReader.bind("id", RewardLotteryTemplate.id))
		{
			LOG_WARN("Failed to load RewardLotteryTemplate.csv for [id]");
			return false;
		}
		if (!csvReader.bind("type", RewardLotteryTemplate.type))
		{
			LOG_WARN("Failed to load RewardLotteryTemplate.csv for [type]");
			return false;
		}
		if (!csvReader.bind("refreshtime", RewardLotteryTemplate.refreshtime))
		{
			LOG_WARN("Failed to load RewardLotteryTemplate.csv for [refreshtime]");
			return false;
		}
		if (!csvReader.bind("opentime", RewardLotteryTemplate.opentime))
		{
			LOG_WARN("Failed to load RewardLotteryTemplate.csv for [opentime]");
			return false;

		}
		if (!csvReader.bind("cost", RewardLotteryTemplate.cost))
		{
			LOG_WARN("Failed to load RewardLotteryTemplate.csv for [cost]");
			return false;
		}
		if (!csvReader.bind("vipneed", RewardLotteryTemplate.vipneed))
		{
			LOG_WARN("Failed to load RewardLotteryTemplate.csv for [vipneed]");
			return false;
		}
		if (!csvReader.bind("normalaward", RewardLotteryTemplate.normalaward))
		{
			LOG_WARN("Failed to load RewardLotteryTemplate.csv for [normalaward]");
			return false;
		}
		if (!csvReader.bind("nextbestneedtimes", RewardLotteryTemplate.nextbestneedtimes))
		{
			LOG_WARN("Failed to load RewardLotteryTemplate.csv for [nextbestneedtimes]");
			return false;

		}
		if (!csvReader.bind("bestaward", RewardLotteryTemplate.bestaward))
		{
			LOG_WARN("Failed to load RewardLotteryTemplate.csv for [bestaward]");
			return false;
		}


		for (UInt32 i = 1;i<51;i++)
		{
			KeyValue keyValue;
			sprintf(dest,"times%d",i);
			strData1 = (String)dest;
			sprintf(dest2,"award%d",i);
			strData2 = (String)dest2;
			if (!csvReader.bind(strData1, keyValue.key))
			{
				LOG_WARN("Failed to load RewardLotteryTemplate.csv for [times]");
				break;
			}
			if (keyValue.key == 0)
			{
				break;
			}
			if (!csvReader.bind(strData2, keyValue.value))
			{
				LOG_WARN("Failed to load RewardLotteryTemplate.csv for [award]");
				break;
			}
			RewardLotteryTemplate.keyValues.insertTail(keyValue);		
		}	

		mMap.insert(RewardLotteryTemplate.id, RewardLotteryTemplate);		
	}
	fileStream.close();
	return true;
}
