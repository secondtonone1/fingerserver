#include "ConsortSignTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
ConsortSignTable::loadFromDbc(const String& fileName)
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
ConsortSignTable::reloadFromDbc(const String& fileName)
{
	ConsortSignTemplateMap tmpMap = mMap;
	
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
	  
		LOG_WARN("Failed to reload guildsign table.");
		return false;
	}
	return true;
}

bool 
ConsortSignTable::saveToDbc(const String& filePath)
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
ConsortSignTable::loadFromCsv(const String& filePath)
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
		ConsortSignTemplate consortSignTemp;
		// ¹«ÓÃ
		if (!csvReader.bind("id", consortSignTemp.mId))
		{
			LOG_WARN("Failed to load guildsign.csv for [id]");
			return false;
		}

		if (!csvReader.bind("cost", consortSignTemp.mCost))
		{
			LOG_WARN("Failed to load guildsign.csv for [cost]");
			return false;
		}

		if (!csvReader.bind("activity", consortSignTemp.mActivity))
		{
			LOG_WARN("Failed to load guildsign.csv for [activity]");
			return false;
		}
	
		if (!csvReader.bind("guildexp", consortSignTemp.mConsortExp))
		{
			LOG_WARN("Failed to load guildsign.csv for [guildexp]");
			return false;
		}

		if (!csvReader.bind("honor", consortSignTemp.mHonor))
		{
			LOG_WARN("Failed to load guildsign.csv for [honor]");
			return false;
		}

		mMap.insert(consortSignTemp.mId, consortSignTemp);

	}

	fileStream.close();
	return true;
}