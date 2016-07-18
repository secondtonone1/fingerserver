#include "ChapterTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
ChapterTable::loadFromDbc(const String& fileName)
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
ChapterTable::reloadFromDbc(const String& fileName)
{
	ChapterTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload BuffTemplate table.");
		return false;
	}
	return true;
}

bool 
ChapterTable::saveToDbc(const String& filePath)
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
ChapterTable::loadFromCsv(const String& filePath)
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
		ChapterTemplate chapterTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("id", chapterTemplate.mId))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [Id]");
			return false;
		}

		if (!csvReader.bind("name", chapterTemplate.mName))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [name]");
			return false;
		}

		if (!csvReader.bind("desc", chapterTemplate.mDesc))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [desc]");
			return false;
		}

		if (!csvReader.bind("bgPicture", chapterTemplate.mBgPicture))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [bgPicture]");
			return false;
		}

		if (!csvReader.bind("starAwardCount1", chapterTemplate.mStarAwardCount1))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [starAwardCount1]");
			return false;
		}

		if (!csvReader.bind("starAwardType1", chapterTemplate.mStarAwardType1))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [starAwardType1]");
			return false;
		}


		if (!csvReader.bind("starAwardContent1", chapterTemplate.mStarAwardContent1))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [starAwardContent1]");
			return false;
		}


		if (!csvReader.bind("starAwardCount2", chapterTemplate.mStarAwardCount2))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [starAwardType2]");
			return false;
		}


		if (!csvReader.bind("starAwardType2", chapterTemplate.mStarAwardType2))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [starAwardType2]");
			return false;
		}


		if (!csvReader.bind("starAwardContent2", chapterTemplate.mStarAwardContent2))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [starAwardContent2]");
			return false;
		}


		if (!csvReader.bind("starAwardCount3", chapterTemplate.mStarAwardCount3))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [starAwardCount3]");
			return false;
		}

		if (!csvReader.bind("starAwardType3", chapterTemplate.mStarAwardType3))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [starAwardType3]");
			return false;
		}

		if (!csvReader.bind("starAwardContent3", chapterTemplate.mStarAwardContent3))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [starAwardContent3]");
			return false;
		}

		
		if (!csvReader.bind("stageList", chapterTemplate.mStrStageList))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [stageList]");
			return false;
		}

		if (!csvReader.bind("starTotalCount", chapterTemplate.mStarTotalCount))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [starTotalCount]");
			return false;
		}

		mMap.insert(chapterTemplate.mId, chapterTemplate);
	}
	fileStream.close();
	return true;
}

