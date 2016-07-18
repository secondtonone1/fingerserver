#include "EffectTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
EffectTable::loadFromDbc(const String& fileName)
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
EffectTable::reloadFromDbc(const String& fileName)
{
	EffectTemplateMap tmpMap = mMap;
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
EffectTable::saveToDbc(const String& filePath)
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
EffectTable::loadFromCsv(const String& filePath)
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
		EffectTemplate effectTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("id", effectTemplate.mId))
		{
			LOG_WARN("Failed to load effectTemplate.csv for [Id]");
			return false;
		}

		if (!csvReader.bind("resType", effectTemplate.mResType))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [resType]");
			return false;
		}

		if (!csvReader.bind("type", effectTemplate.mType))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [type]");
			return false;
		}
	
		if (!csvReader.bind("playType", effectTemplate.mPlayType))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [playType]");
			return false;
		}

		if (!csvReader.bind("anchorPoint", effectTemplate.mAnchorPoint))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [anchorPoint]");
			return false;
		}

		if (!csvReader.bind("reference", effectTemplate.mReference))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [reference]");
			return false;
		}

		if (!csvReader.bind("offset", effectTemplate.mOffset))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [offset]");
			return false;
		}

		if (!csvReader.bind("playSpeed", effectTemplate.mPlaySpeed))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [playSpeed]");
			return false;
		}

		if (!csvReader.bind("frameStart", effectTemplate.mFrameStart))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [frameStart]");
			return false;
		}

		if (!csvReader.bind("frameCount", effectTemplate.mFrameCount))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [frameCount]");
			return false;
		}

		if (!csvReader.bind("res", effectTemplate.mRes))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [res]");
			return false;
		}

		if (!csvReader.bind("interval", effectTemplate.mInterval))
		{
			LOG_WARN("Failed to load chapterTemplate.csv for [interval]");
			return false;
		}

		mMap.insert(effectTemplate.mId, effectTemplate);
	}
	fileStream.close();
	return true;
}

