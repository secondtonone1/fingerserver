#include "AchievementTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
AchievementTable::loadFromDbc(const String& fileName)
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
AchievementTable::reloadFromDbc(const String& fileName)
{
	AchievementTemplateMap tmpMap = mMap;
	FirstAchievementMap tmpFirstAchMap = mFirstAchMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
	    mFirstAchMap = tmpFirstAchMap;
		LOG_WARN("Failed to reload Achievement table.");
		return false;
	}
	return true;
}

bool 
AchievementTable::saveToDbc(const String& filePath)
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
AchievementTable::loadFromCsv(const String& filePath)
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
		AchievementTemplate achievementTemp;
		// 公用
		if (!csvReader.bind("id", achievementTemp.mId))
		{
			LOG_WARN("Failed to load achievement.csv for [id]");
			return false;
		}

		if (!csvReader.bind("needevent", achievementTemp.mEvent))
		{
			LOG_WARN("Failed to load achievement.csv for [needevent]");
			return false;
		}

		if (!csvReader.bind("needarg", achievementTemp.mArg))
		{
			LOG_WARN("Failed to load achievement.csv for [needarg]");
			return false;
		}

		if (!csvReader.bind("award", achievementTemp.mAward))
		{
			LOG_WARN("Failed to load achievement.csv for [award]");
			return false;
		}

		if (!csvReader.bind("nextid", achievementTemp.mNextId))
		{
			LOG_WARN("Failed to load achievement.csv for [nextid]");
			return false;
		}

		if (!csvReader.bind("preid", achievementTemp.mPreId))
		{
			LOG_WARN("Failed to load achievement.csv for [preid]");
			return false;
		}

		
		
	
		std::string ::size_type findIndex = achievementTemp.mAward.find(';');
		std::string awardTotalStr = achievementTemp.mAward.c_str();
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
	
			achievementTemp.mItemList.insertTail(allItemEle);

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
	
			achievementTemp.mItemList.insertTail(allItemEle);

		}


		mMap.insert(achievementTemp.mId, achievementTemp);

		if(!achievementTemp.mPreId)
		{
			mFirstAchMap.insert(achievementTemp.mEvent, achievementTemp);
		}
	

	}
	fileStream.close();
	return true;
}