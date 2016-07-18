#include "DailyActiveTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
DailyActiveTable::loadFromDbc(const String& fileName)
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
DailyActiveTable::reloadFromDbc(const String& fileName)
{
	DailyActiveTemplateMap tmpMap = mMap;

	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;

		LOG_WARN("Failed to reload active table.");
		return false;
	}
	return true;
}

bool 
DailyActiveTable::saveToDbc(const String& filePath)
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
DailyActiveTable::loadFromCsv(const String& filePath)
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
		DailyActiveTemplate dailyActiveTemp;
		// 公用
		if (!csvReader.bind("active", dailyActiveTemp.mActive))
		{
			LOG_WARN("Failed to load dailyactive.csv for [active]");
			return false;
		}

		if(!csvReader.bind("id", dailyActiveTemp.mId))
		{
			LOG_WARN("Failed to load dailyactive.csv for [id]");
			return false;
		}

		if (!csvReader.bind("award", dailyActiveTemp.mAward))
		{
			LOG_WARN("Failed to load dailyactive.csv for [award]");
			return false;
		}

		std::string ::size_type findIndex = dailyActiveTemp.mAward.find(';');
		std::string awardTotalStr = dailyActiveTemp.mAward.c_str();
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

			dailyActiveTemp.mItemList.insertTail(allItemEle);

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

			dailyActiveTemp.mItemList.insertTail(allItemEle);

		}


		mMap.insert(dailyActiveTemp.mId, dailyActiveTemp);



	}
	fileStream.close();
	return true;
}