#include "ServantTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
ServantTable::loadFromDbc(const String& fileName)
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
ServantTable::reloadFromDbc(const String& fileName)
{
	ServantTemplateMap tmpMap = mMap;

	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;

		LOG_WARN("Failed to reload ServantTable table.");
		return false;
	}
	return true;
}

bool 
ServantTable::saveToDbc(const String& filePath)
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
ServantTable::loadFromCsv(const String& filePath)
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
		ServantTemplate servantTemp;
		// 公用
		if (!csvReader.bind("id", servantTemp.mId))
		{
			LOG_WARN("Failed to load servant.csv for [id]");
			return false;
		}

		if (!csvReader.bind("monsterid", servantTemp.mMonsterid))
		{
			LOG_WARN("Failed to load servant.csv for [monsterid]");
			return false;
		}

		if (!csvReader.bind("quality", servantTemp.mQuality))
		{
			LOG_WARN("Failed to load servant.csv for [quality]");
			return false;
		}

		if (!csvReader.bind("skillid1", servantTemp.mSkillID1))
		{
			LOG_WARN("Failed to load servant.csv for [skillid1]");
			return false;
		}
		
		if (!csvReader.bind("skillid2", servantTemp.mSkillID2))
		{
			LOG_WARN("Failed to load servant.csv for [skillid2]");
			return false;
		}

		if (!csvReader.bind("rate", servantTemp.mRate))
		{
			LOG_WARN("Failed to load servant.csv for [rate]");
			return false;
		}

		if (!csvReader.bind("curstar", servantTemp.mCurStar))
		{
			LOG_WARN("Failed to load servant.csv for [curstar]");
			return false;
		}

		if (!csvReader.bind("curflor", servantTemp.mCurFloor))
		{
			LOG_WARN("Failed to load servant.csv for [curflor]");
			return false;
		}

		if (!csvReader.bind("maxstar", servantTemp.mMaxStar))
		{
			LOG_WARN("Failed to load servant.csv for [maxstar]");
			return false;
		}

		if (!csvReader.bind("maxfloor", servantTemp.mMaxFloor))
		{
			LOG_WARN("Failed to load servant.csv for [maxfloor]");
			return false;
		}

		if (!csvReader.bind("piece", servantTemp.mPieceType))
		{
			LOG_WARN("Failed to load servant.csv for [piece]");
			return false;
		}
		
		if (!csvReader.bind("count", servantTemp.mPieceCount))
		{
			LOG_WARN("Failed to load servant.csv for [count]");
			return false;
		}

		if (!csvReader.bind("lucky", servantTemp.mLuckyStr))
		{
			LOG_WARN("Failed to load servant.csv for [lucky]");
			return false;
		}

		
			if (!csvReader.bind("lifelight", servantTemp.mServantSwitch))
			{
				LOG_WARN("Failed to load servant.csv for [lifelight]");
				return false;
			}


		std::string luckyStr;
		luckyStr = servantTemp.mLuckyStr.c_str();

		
		std::string ::size_type findIndex = luckyStr.find(';');

		std::stringstream mystream;
		while(findIndex != std::string::npos)
		{
			//;号之前的全部截取，放入到列表里
			std::string equipEleStr = luckyStr.substr(0,findIndex);

			mystream.clear();
			mystream.str("");

			mystream << equipEleStr;

			UInt64 equipEle = 0;
			mystream >> equipEle;

			servantTemp.luckyList.insertTail(equipEle);

			luckyStr = luckyStr.substr(findIndex + 1);

			findIndex = luckyStr.find(';');

		}

		if(luckyStr != "")
		{
			mystream.clear();
			mystream.str("");

			mystream << luckyStr;

			UInt64 equipEle = 0;
			mystream >> equipEle;
			servantTemp.luckyList.insertTail(equipEle);
		}



		

		mMap.insert(servantTemp.mId, servantTemp);


	}
	fileStream.close();
	return true;
}

