#include "ServantLvUpMaterialTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
ServantLvUpMaterialTable::loadFromDbc(const String& fileName)
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
ServantLvUpMaterialTable::reloadFromDbc(const String& fileName)
{
	ServantLvUpMaterialTemplateMap tmpMap = mMap;

	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;

		LOG_WARN("Failed to reload servantLvUpMaterial table.");
		return false;
	}
	return true;
}

bool 
ServantLvUpMaterialTable::saveToDbc(const String& filePath)
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
ServantLvUpMaterialTable::loadFromCsv(const String& filePath)
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
		ServantLvUpMaterialTemplate servantLvUpMaterialTemp;

		// ¹«ÓÃ
		if (!csvReader.bind("id", servantLvUpMaterialTemp.mId))
		{
			LOG_WARN("Failed to load servantlvupmaterial.csv for [id]");
			return false;
		}

		if (!csvReader.bind("expProvide", servantLvUpMaterialTemp.mExpProvide))
		{
			LOG_WARN("Failed to load servantlvupmaterial.csv for [expProvide]");
			return false;
		}

		mMap.insert(servantLvUpMaterialTemp.mId, servantLvUpMaterialTemp);


	}
	fileStream.close();
	return true;
}

