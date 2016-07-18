#include "MaterialPointsTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
MaterialPointsTable::loadFromDbc(const String& fileName)
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
MaterialPointsTable::reloadFromDbc(const String& fileName)
{
	MaterialPointsTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload MaterialPoints table.");
		return false;
	}
	return true;
}

bool 
MaterialPointsTable::saveToDbc(const String& filePath)
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
MaterialPointsTable::loadFromCsv(const String& filePath)
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
		MaterialPointsTemplate materialPointsTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("lv", materialPointsTemplate.mLevel))
		{
			LOG_WARN("Failed to load materialPoints.csv for [level]");
			return false;
		}

		if(!csvReader.bind("bluePoints", materialPointsTemplate.mBluePoints))
		{
			LOG_WARN("Failed to load materialPoints.csv for [bluePoints]");
			return false;
		}
	
		if(!csvReader.bind("purplePoints", materialPointsTemplate.mPurplePoints))
		{
			LOG_WARN("Failed to load materialPoints.csv for [purplePoints]");
			return false;
		}

		if(!csvReader.bind("orangePoints", materialPointsTemplate.mOrangePoints))
		{
			LOG_WARN("Failed to load materialPoints.csv for [orangePoints]");
			return false;
		}

		mMap.insert(materialPointsTemplate.mLevel, materialPointsTemplate);
	}
	fileStream.close();
	return true;
}

