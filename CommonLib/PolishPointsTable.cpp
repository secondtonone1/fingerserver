#include "PolishPointsTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
PolishPointsTable::loadFromDbc(const String& fileName)
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
PolishPointsTable::reloadFromDbc(const String& fileName)
{
	PolishPointsTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload polishPoints table.");
		return false;
	}
	return true;
}

bool 
PolishPointsTable::saveToDbc(const String& filePath)
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
PolishPointsTable::loadFromCsv(const String& filePath)
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
		PolishPointsTemplate polishPointsTemplate;

		if (!csvReader.bind("lv", polishPointsTemplate.mLevel))
		{
			LOG_WARN("Failed to load polishPoints.csv for [lv]");
			return false;
		}
	
		if (!csvReader.bind("point1", polishPointsTemplate.mPurplePoints))
		{
			LOG_WARN("Failed to load polishPoints.csv for [point1]");
			return false;
		}
	
		if(!csvReader.bind("point2", polishPointsTemplate.mOrangePoints))
		{
			LOG_WARN("Failed to load polishPoints.csv for [point2]");
			return false;
		}

		mMap.insert(polishPointsTemplate.mLevel, polishPointsTemplate);
	}
	fileStream.close();
	return true;
}

