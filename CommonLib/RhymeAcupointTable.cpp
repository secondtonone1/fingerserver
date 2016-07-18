#include "RhymeAcupointTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
RhymeAcupointTable::loadFromDbc(const String& fileName)
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
RhymeAcupointTable::reloadFromDbc(const String& fileName)
{
	RhymeAcupointTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload rhymeLevel table.");
		return false;
	}
	return true;
}

bool 
RhymeAcupointTable::saveToDbc(const String& filePath)
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
RhymeAcupointTable::loadFromCsv(const String& filePath)
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
		RhymeAcupointTemplate rhymeAcupointTemplate;
		
		if (!csvReader.bind("id", rhymeAcupointTemplate.mId))
		{
			LOG_WARN("Failed to load rhymeAcupoint.csv for [id]");
			return false;
		}
		
		if (!csvReader.bind("name", rhymeAcupointTemplate.mName))
		{
			LOG_WARN("Failed to load rhymeAcupoint.csv for [name]");
			return false;
		}
		
		if (!csvReader.bind("activeNeedGrass", rhymeAcupointTemplate.mActiveNeedGrass))
		{
			LOG_WARN("Failed to load rhymeAcupoint.csv for [activeNeedGrass]");
			return false;
		}
		
		if (!csvReader.bind("activeNeedStep", rhymeAcupointTemplate.mActiveNeedStep))
		{
			LOG_WARN("Failed to load rhymeAcupoint.csv for [activeNeedStep]");
			return false;
		}

		if (!csvReader.bind("activeNeedLevel", rhymeAcupointTemplate.mActiveNeedLevel))
		{
			LOG_WARN("Failed to load rhymeAcupoint.csv for [activeNeedLevel]");
			return false;
		}

		if (!csvReader.bind("attr1", rhymeAcupointTemplate.mattr1))
		{
			LOG_WARN("Failed to load rhymeAcupoint.csv for [attr1]");
			return false;
		}

		if (!csvReader.bind("attr2", rhymeAcupointTemplate.mattr2))
		{
			LOG_WARN("Failed to load rhymeAcupoint.csv for [attr2]");
			return false;
		}

		if (!csvReader.bind("attr3", rhymeAcupointTemplate.mattr3))
		{
			LOG_WARN("Failed to load rhymeAcupoint.csv for [attr3]");
			return false;
		}

		if (!csvReader.bind("attr4", rhymeAcupointTemplate.mattr4))
		{
			LOG_WARN("Failed to load rhymeAcupoint.csv for [attr4]");
			return false;
		}

		if (!csvReader.bind("attr5", rhymeAcupointTemplate.mattr5))
		{
			LOG_WARN("Failed to load rhymeAcupoint.csv for [attr5]");
			return false;
		}

		if (!csvReader.bind("grassType", rhymeAcupointTemplate.mGrassType))
		{
			LOG_WARN("Failed to load rhymeAcupoint.csv for [grassType]");
			return false;
		}

		mMap.insert(rhymeAcupointTemplate.mId, rhymeAcupointTemplate);

	}
	fileStream.close();
	return true;
}

