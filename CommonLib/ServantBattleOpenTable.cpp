#include "ServantBattleOpenTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
ServantBattleOpenTable::loadFromDbc(const String& fileName)
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
ServantBattleOpenTable::reloadFromDbc(const String& fileName)
{
	ServantBattleOpenTemplateMap tmpMap = mMap;
	
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		
		LOG_WARN("Failed to reload servantbattleopen table.");
		return false;
	}
	return true;
}

bool 
ServantBattleOpenTable::saveToDbc(const String& filePath)
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
ServantBattleOpenTable::loadFromCsv(const String& filePath)
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
		ServantBattleOpenTemplate servantBattleOpenTemp;

		// ¹«ÓÃ
		if (!csvReader.bind("pos", servantBattleOpenTemp.mPos))
		{
			LOG_WARN("Failed to load servantbattleopen.csv for [pos]");
			return false;
		}

		if (!csvReader.bind("level", servantBattleOpenTemp.mLevel))
		{
			LOG_WARN("Failed to load servantbattleopen.csv for [level]");
			return false;
		}

		if (!csvReader.bind("cost", servantBattleOpenTemp.mCost))
		{
			LOG_WARN("Failed to load servantbattleopen.csv for [cost]");
			return false;
		}
	
		mMap.insert(servantBattleOpenTemp.mPos, servantBattleOpenTemp);


	}
	fileStream.close();
	return true;
}

