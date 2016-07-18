#include "MaterialTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
MaterialTable::loadFromDbc(const String& fileName)
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
MaterialTable::reloadFromDbc(const String& fileName)
{
	MaterialTemplateMap tmpMap = mMap;
	
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		
		LOG_WARN("Failed to reload MaterialTable table.");
		return false;
	}
	return true;
}

bool 
MaterialTable::saveToDbc(const String& filePath)
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
MaterialTable::loadFromCsv(const String& filePath)
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
		MaterialTemplate materialTemp;
		// ¹«ÓÃ
		if (!csvReader.bind("id", materialTemp.mId))
		{
			LOG_WARN("Failed to load material.csv for [id]");
			return false;
		}
	
		if(!csvReader.bind("quality", materialTemp.mQuality))
		{
			LOG_WARN("Failed to load material.csv for [quality]");
		}
		
		if(!csvReader.bind("order", materialTemp.mOrder))
		{
			LOG_WARN("Failed to load material.csv for [order]");
		}

		if(!csvReader.bind("price", materialTemp.mPrice))
		{
			LOG_WARN("Failed to load material.csv for [quality]");
		}

		if(!csvReader.bind("produced", materialTemp.mProduced))
		{
			LOG_WARN("Failed to load material.csv for [produced]");
		}

	

		mMap.insert(materialTemp.mId, materialTemp);

		
	}
	fileStream.close();
	return true;
}

