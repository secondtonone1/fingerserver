#include "DailyTaskTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
DailyTaskTable::loadFromDbc(const String& fileName)
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
DailyTaskTable::reloadFromDbc(const String& fileName)
{
	DailyTaskTempMap tmpMap = mMap;
	
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		
		LOG_WARN("Failed to reload DailyTask table.");
		return false;
	}
	return true;
}

bool 
DailyTaskTable::saveToDbc(const String& filePath)
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
DailyTaskTable::loadFromCsv(const String& filePath)
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
		DailyTaskTemplate dailyTaskTemp;
		// 公用
		if (!csvReader.bind("id", dailyTaskTemp.mId))
		{
			LOG_WARN("Failed to load dailytask.csv for [id]");
			return false;
		}

		if (!csvReader.bind("needevent", dailyTaskTemp.mEvent))
		{
			LOG_WARN("Failed to load dailytask.csv for [needevent]");
			return false;
		}

		if (!csvReader.bind("needarg", dailyTaskTemp.mArg))
		{
			LOG_WARN("Failed to load dailytask.csv for [needarg]");
			return false;
		}

		if (!csvReader.bind("award", dailyTaskTemp.mAward))
		{
			LOG_WARN("Failed to load dailytask.csv for [award]");
			return false;
		}

		if (!csvReader.bind("active", dailyTaskTemp.mActive))
		{
			LOG_WARN("Failed to load dailytask.csv for [active]");
			return false;
		}

		if (!csvReader.bind("needlv", dailyTaskTemp.mNeedLv))
		{
			LOG_WARN("Failed to load dailytask.csv for [needlv]");
			return false;
		}



		std::string ::size_type findIndex = dailyTaskTemp.mAward.find(';');
		std::string awardTotalStr = dailyTaskTemp.mAward.c_str();
		std::stringstream mystream;
		while(findIndex != std::string::npos)
		{
			//;号之前的全部截取，放入到列表里
			std::string awardEleStr = awardTotalStr.substr(0,findIndex);
			std::string ::size_type typeIndex = awardEleStr.find(',');
			std::string typeStr = awardEleStr.substr(0,typeIndex);

			AllItemEle allItemEle;

			mystream.clear();
			mystream.str("");

			mystream << typeStr;
			mystream >> allItemEle.resType;


			awardEleStr = awardEleStr.substr(typeIndex+1);
			std::string::size_type subTypeIndex = awardEleStr.find(',');
			std::string subTypeStr = awardEleStr.substr(0,subTypeIndex);

			mystream.clear();
			mystream.str("");

			mystream << subTypeStr;
			mystream >> allItemEle.subType;

			std::string countStr = awardEleStr.substr(subTypeIndex+1);

			mystream.clear();
			mystream.str("");

			mystream << countStr;
			mystream >> allItemEle.count;

			dailyTaskTemp.mItemList.insertTail(allItemEle);

			awardTotalStr = awardTotalStr.substr(findIndex + 1);

			findIndex = awardTotalStr.find(';');

		}

		if(awardTotalStr != "")
		{
			std::string awardEleStr = awardTotalStr;
			std::string ::size_type typeIndex = awardEleStr.find(',');
			std::string typeStr = awardEleStr.substr(0,typeIndex);

			AllItemEle allItemEle;

			mystream.clear();
			mystream.str("");

			mystream << typeStr;
			mystream >> allItemEle.resType;


			awardEleStr = awardEleStr.substr(typeIndex+1);
			std::string::size_type subTypeIndex = awardEleStr.find(',');
			std::string subTypeStr = awardEleStr.substr(0,subTypeIndex);

			mystream.clear();
			mystream.str("");

			mystream << subTypeStr;
			mystream >> allItemEle.subType;

			std::string countStr = awardEleStr.substr(subTypeIndex+1);

			mystream.clear();
			mystream.str("");

			mystream << countStr;
			mystream >> allItemEle.count;

			dailyTaskTemp.mItemList.insertTail(allItemEle);

		}


		mMap.insert(dailyTaskTemp.mId, dailyTaskTemp);

		

	}
	fileStream.close();
	return true;
}



bool 
SevenDayTrainningTable::loadFromDbc(const String& fileName)
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
SevenDayTrainningTable::reloadFromDbc(const String& fileName)
{
	SevenDayTrainningTempMap tmpMap = mMap;

	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;

		LOG_WARN("Failed to reload DailyTask table.");
		return false;
	}
	return true;
}

bool 
SevenDayTrainningTable::saveToDbc(const String& filePath)
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
SevenDayTrainningTable::loadFromCsv(const String& filePath)
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
		SevenDayTrainningTemplate sevenDayTrainningTemplate;
		// 公用
		if (!csvReader.bind("id", sevenDayTrainningTemplate.id))
		{
			LOG_WARN("Failed to load sevenDayTrainning.csv for [id]");
			return false;
		}

		if (!csvReader.bind("link", sevenDayTrainningTemplate.link))
		{
			LOG_WARN("Failed to load sevenDayTrainning.csv for [link]");
			return false;
		}

		if (!csvReader.bind("needevent", sevenDayTrainningTemplate.needevent))
		{
			LOG_WARN("Failed to load sevenDayTrainning.csv for [needevent]");
			return false;
		}

		if (!csvReader.bind("needarg", sevenDayTrainningTemplate.needarg))
		{
			LOG_WARN("Failed to load sevenDayTrainning.csv for [needarg]");
			return false;
		}

		if (!csvReader.bind("award", sevenDayTrainningTemplate.award))
		{
			LOG_WARN("Failed to load sevenDayTrainning.csv for [award]");
			return false;
		}


		mMap.insert(sevenDayTrainningTemplate.id, sevenDayTrainningTemplate);



	}
	fileStream.close();
	return true;
}