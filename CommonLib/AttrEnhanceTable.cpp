#include "AttrEnhanceTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
AttrEnhanceTable::loadFromDbc(const String& fileName)
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
AttrEnhanceTable::reloadFromDbc(const String& fileName)
{
	AttrEnhanceTemplateMap tmpMap = mMap;
	AttrEnhanceReverseMap tmpReverseMap = mReverseMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		mReverseMap = tmpReverseMap;
		LOG_WARN("Failed to reload AttrEnhanceTable table.");
		return false;
	}
	return true;
}

bool 
AttrEnhanceTable::saveToDbc(const String& filePath)
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
AttrEnhanceTable::loadFromCsv(const String& filePath)
{
	mMap.clear();
	mReverseMap.clear();

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
		AttrEnhanceTemplate attrEnhanceTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("level", attrEnhanceTemplate.mLevel))
		{
			LOG_WARN("Failed to load attrEnhanceTemplate.csv for [level]");
			return false;
		}

		if(!csvReader.bindFloat("addRate",attrEnhanceTemplate.mAddRate))
		{
			LOG_WARN("Failed to load attrEnhanceTemplate.csv for [addRate]");
			return false;
		}

		if (!csvReader.bind("costCoin", attrEnhanceTemplate.mCostCoin))
		{
			LOG_WARN("Failed to load attrEnhanceTemplate.csv for [costCoin]");
			return false;
		}

		if (!csvReader.bind("exp", attrEnhanceTemplate.mExp))
		{
			LOG_WARN("Failed to load attrEnhanceTemplate.csv for [exp]");
			return false;
		}

		if (!csvReader.bind("id", attrEnhanceTemplate.mID))
		{
			LOG_WARN("Failed to load attrEnhanceTemplate.csv for [id]");
			return false;
		}
	
		if (!csvReader.bind("star", attrEnhanceTemplate.mStar))
		{
			LOG_WARN("Failed to load attrEnhanceTemplate.csv for [star]");
			return false;
		}

		mMap.insert(attrEnhanceTemplate.mID, attrEnhanceTemplate);

		AttrEnhanceReverseMap::Iter * reverseMapIter = mReverseMap.find(attrEnhanceTemplate.mStar);

		if(reverseMapIter)
		{
			reverseMapIter->mValue.insert(attrEnhanceTemplate.mLevel,attrEnhanceTemplate);
		}
		else
		{
			Map<UInt32, AttrEnhanceTemplate> reverseMap;
			reverseMap.insert(attrEnhanceTemplate.mLevel,attrEnhanceTemplate);
			mReverseMap.insert(attrEnhanceTemplate.mStar, reverseMap);
		}
		
	}
	fileStream.close();
	return true;
}