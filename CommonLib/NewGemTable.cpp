#include "NewGemTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
NewGemTable::loadFromDbc(const String& fileName)
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
NewGemTable::reloadFromDbc(const String& fileName)
{
	NewGemTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload NewGem table.");
		return false;
	}
	return true;
}

bool 
NewGemTable::saveToDbc(const String& filePath)
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
NewGemTable::loadFromCsv(const String& filePath)
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
		NewGemTemplate gemTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("gemId", gemTemplate.mGemId))
		{
			LOG_WARN("Failed to load new_gem.csv for [gemId]");
			return false;
		}

		if (!csvReader.bind("level", gemTemplate.mLevel))
		{
			LOG_WARN("Failed to load new_gem.csv for [level]");
			return false;
		}

		if (!csvReader.bind("quality", gemTemplate.mQuality))
		{
			LOG_WARN("Failed to load new_gem.csv for [quality]");
			return false;
		}
		
		if (!csvReader.bind("equipType", gemTemplate.mEquipType))
		{
			LOG_WARN("Failed to load new_gem.csv for [equipType]");
			return false;
		}

		if (!csvReader.bind("levelRequire", gemTemplate.mLvRequire))
		{
			LOG_WARN("Failed to load new_gem.csv for [levelRequire]");
			return false;
		}

		if (!csvReader.bind("attr1Key", gemTemplate.mAttrType1))
		{
			LOG_WARN("Failed to load new_gem.csv for [attr1Key]");
			return false;
		}

		if (!csvReader.bind("attr1Value", gemTemplate.mAttrValue1))
		{
			LOG_WARN("Failed to load new_gem.csv for [attr1Value]");
			return false;
		}

		if (!csvReader.bind("attr2Key", gemTemplate.mAttrType2))
		{
			LOG_WARN("Failed to load new_gem.csv for [attr2Key]");
			return false;
		}

		if (!csvReader.bind("attr2Value", gemTemplate.mAttrValue2))
		{
			LOG_WARN("Failed to load new_gem.csv for [attr2Value]");
			return false;
		}

		if (!csvReader.bind("attr3Key", gemTemplate.mAttrType3))
		{
			LOG_WARN("Failed to load new_gem.csv for [attr3Key]");
			return false;
		}

		if (!csvReader.bind("attr3Value", gemTemplate.mAttrValue3))
		{
			LOG_WARN("Failed to load new_gem.csv for [attr3Value]");
			return false;
		}

		if (!csvReader.bind("attr4Key", gemTemplate.mAttrType4))
		{
			LOG_WARN("Failed to load new_gem.csv for [attr4Key]");
			return false;
		}

		if (!csvReader.bind("attr4Value", gemTemplate.mAttrValue4))
		{
			LOG_WARN("Failed to load new_gem.csv for [attr4Value]");
			return false;
		}

		if (!csvReader.bind("attr5Key", gemTemplate.mAttrType5))
		{
			LOG_WARN("Failed to load new_gem.csv for [attr5Key]");
			return false;
		}

		if (!csvReader.bind("attr5Value", gemTemplate.mAttrValue5))
		{
			LOG_WARN("Failed to load new_gem.csv for [attr5Value]");
			return false;
		}

		if (!csvReader.bind("power", gemTemplate.mPower))
		{
			LOG_WARN("Failed to load new_gem.csv for [power]");
			return false;
		}


		mMap.insert(gemTemplate.mGemId, gemTemplate);
	}
	fileStream.close();
	return true;
}

