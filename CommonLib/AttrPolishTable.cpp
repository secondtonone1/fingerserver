#include "AttrPolishTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
AttrPolishTable::loadFromDbc(const String& fileName)
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
AttrPolishTable::reloadFromDbc(const String& fileName)
{
	AttrPolishTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload AttrPolish table.");
		return false;
	}
	return true;
}

bool 
AttrPolishTable::saveToDbc(const String& filePath)
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
AttrPolishTable::loadFromCsv(const String& filePath)
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
		AttrPolishTemplate attrPolishTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("lv", attrPolishTemplate.mLevel))
		{
			LOG_WARN("Failed to load attrPolishTemplate.csv for [lv]");
			assert(false);
			return false;
		}

		if (!csvReader.bind("white", attrPolishTemplate.mWhite))
		{
			LOG_WARN("Failed to load attrPolishTemplate.csv for [white]");
			assert(false);
			return false;
		}

		if (!csvReader.bind("green", attrPolishTemplate.mGreen))
		{
			LOG_WARN("Failed to load attrPolishTemplate.csv for [green]");
			assert(false);
			return false;
		}

		if (!csvReader.bind("blue", attrPolishTemplate.mBlue))
		{
			LOG_WARN("Failed to load attrPolishTemplate.csv for [blue]");
			assert(false);
			return false;
		}

		if (!csvReader.bind("purple", attrPolishTemplate.mPurple))
		{
			LOG_WARN("Failed to load attrPolishTemplate.csv for [purple]");
			assert(false);
			return false;
		}

		if (!csvReader.bind("orange", attrPolishTemplate.mOrange))
		{
			LOG_WARN("Failed to load attrPolishTemplate.csv for [orange]");
			assert(false);
			return false;
		}

		mMap.insert(attrPolishTemplate.mLevel, attrPolishTemplate);
	}
	fileStream.close();
	return true;
}