#include "ServantLuckTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
ServantLuckTable::loadFromDbc(const String& fileName)
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
ServantLuckTable::reloadFromDbc(const String& fileName)
{
	ServantLuckTemplateMap tmpMap = mMap;

	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;

		LOG_WARN("Failed to reload ServantLuckTable table.");
		return false;
	}
	return true;
}

bool 
ServantLuckTable::saveToDbc(const String& filePath)
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
ServantLuckTable::loadFromCsv(const String& filePath)
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
		ServantLuckTemplate servantLuckTemp;
		// ¹«ÓÃ
		if (!csvReader.bind("id", servantLuckTemp.mId))
		{
			LOG_WARN("Failed to load servantlucky.csv for [id]");
			return false;
		}

		if (!csvReader.bind("attr1", servantLuckTemp.mAttrType1))
		{
			LOG_WARN("Failed to load servantlucky.csv for [attr1]");
			return false;
		}

		if (!csvReader.bind("percent1", servantLuckTemp.mAttrValue1))
		{
			LOG_WARN("Failed to load servantlucky.csv for [percent1]");
			return false;
		}

		if (!csvReader.bind("attr2", servantLuckTemp.mAttrType2))
		{
			LOG_WARN("Failed to load servantlucky.csv for [attr2]");
			return false;
		}

		if (!csvReader.bind("percent2", servantLuckTemp.mAttrValue2))
		{
			LOG_WARN("Failed to load servantlucky.csv for [percent2]");
			return false;
		}

		if (!csvReader.bind("attr3", servantLuckTemp.mAttrType3))
		{
			LOG_WARN("Failed to load servantlucky.csv for [attr3]");
			return false;
		}

		if (!csvReader.bind("percent3", servantLuckTemp.mAttrValue3))
		{
			LOG_WARN("Failed to load servantlucky.csv for [percent3]");
			return false;
		}

		if (!csvReader.bind("servant1", servantLuckTemp.mServant1))
		{
			LOG_WARN("Failed to load servantlucky.csv for [servant1]");
			return false;
		}

		if (!csvReader.bind("servant2", servantLuckTemp.mServant2))
		{
			LOG_WARN("Failed to load servantlucky.csv for [servant2]");
			return false;
		}

		if (!csvReader.bind("servant3", servantLuckTemp.mServant3))
		{
			LOG_WARN("Failed to load servantlucky.csv for [servant3]");
			return false;
		}

		if (!csvReader.bind("servant4", servantLuckTemp.mServant4))
		{
			LOG_WARN("Failed to load servantlucky.csv for [servant4]");
			return false;
		}

		if (!csvReader.bind("servant5", servantLuckTemp.mServant5))
		{
			LOG_WARN("Failed to load servantlucky.csv for [servant5]");
			return false;
		}

		mMap.insert(servantLuckTemp.mId, servantLuckTemp);
	}
	fileStream.close();
	return true;
}

