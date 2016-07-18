#include "AttrRandomTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
AttrRandomTable::loadFromDbc(const String& fileName)
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
AttrRandomTable::reloadFromDbc(const String& fileName)
{
	AttrRandomTemplateMap tmpMap = mMap;
	AttrRandomLvQualityMap tmpLvQMap = mLvQualityMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		mLvQualityMap = tmpLvQMap;
		LOG_WARN("Failed to reload ItemTemplate table.");
		return false;
	}
	return true;
}

bool 
AttrRandomTable::saveToDbc(const String& filePath)
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
AttrRandomTable::loadFromCsv(const String& filePath)
{
	mMap.clear();
	mLvQualityMap.clear();
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
		AttrRandomTemplate attrRandomTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("id", attrRandomTemplate.mId))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [id]");
			return false;
		}

		if(!csvReader.bind("level",attrRandomTemplate.mLevel))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [level]");
			return false;
		}

		if (!csvReader.bind("quality", attrRandomTemplate.mQuality))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [quality]");
			return false;
		}

		if (!csvReader.bind("PAMin", attrRandomTemplate.mPAMin))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [PAMin]");
			return false;
		}

		if (!csvReader.bind("PAMax", attrRandomTemplate.mPAMax))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [PAMax]");
			return false;
		}

		if (!csvReader.bind("PARate", attrRandomTemplate.mPARate))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [PARate]");
			return false;
		}

		if (!csvReader.bind("MAMin", attrRandomTemplate.mMAMin))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [MAMin]");
			return false;
		}


		if (!csvReader.bind("MAMax", attrRandomTemplate.mMAMax))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [MAMax]");
			return false;
		}


		if (!csvReader.bind("MARate", attrRandomTemplate.mMARate))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [MARate]");
			return false;
		}

		if (!csvReader.bind("PFMin", attrRandomTemplate.mPFMin))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [PFMin]");
			return false;
		}

		if (!csvReader.bind("PFMax", attrRandomTemplate.mPFMax))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [PFMax]");
			return false;
		}

		if (!csvReader.bind("PFRate", attrRandomTemplate.mPFRate))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [PFRate]");
			return false;
		}

		if (!csvReader.bind("MFMin", attrRandomTemplate.mMFMin))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [MFMin]");
			return false;
		}

		if (!csvReader.bind("MFMax", attrRandomTemplate.mMFMax))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [MFMax]");
			return false;
		}

		if (!csvReader.bind("MFRate", attrRandomTemplate.mMFRate))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [MFRate]");
			return false;
		}

		if (!csvReader.bind("MaxHPMin", attrRandomTemplate.mMaxHPMin))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [mMaxHPMin]");
			return false;
		}

		if (!csvReader.bind("MaxHPMax", attrRandomTemplate.mMaxHPMax))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [mMaxHPMax]");
			return false;
		}

		if (!csvReader.bind("MaxHPRate", attrRandomTemplate.mMaxHPRate))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [MaxHPRate]");
			return false;
		}

		if (!csvReader.bind("MaxMPMin", attrRandomTemplate.mMaxMPMin))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [MaxMPMin]");
			return false;
		}

		if (!csvReader.bind("MaxMPMax", attrRandomTemplate.mMaxMPMax))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [MaxMPMax]");
			return false;
		}
	
		if (!csvReader.bind("MaxMPRate", attrRandomTemplate.mMaxMPRate))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [MaxMPRate]");
			return false;
		}

		if (!csvReader.bind("MaxSPMin", attrRandomTemplate.mMaxSPMin))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [MaxSPMin]");
			return false;
		}

		if (!csvReader.bind("MaxSPMax", attrRandomTemplate.mMaxSPMax))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [MaxSPMax]");
			return false;
		}

		if (!csvReader.bind("MaxSPRate", attrRandomTemplate.mMaxSPRate))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [MaxSPRate]");
			return false;
		}

		if (!csvReader.bind("CritRateMin", attrRandomTemplate.mCritRateMin))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [CritRateMin]");
			return false;
		}

		if (!csvReader.bind("CritRateMax", attrRandomTemplate.mCritRateMax))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [mCritRateMax]");
			return false;
		}

		if (!csvReader.bind("CritRateRate", attrRandomTemplate.mCritRateRate))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [CritRateRate]");
			return false;
		}

		if (!csvReader.bind("CritDamageMin", attrRandomTemplate.mCritDamageMin))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [CritDamageMin]");
			return false;
		}

		if (!csvReader.bind("CritDamageMax", attrRandomTemplate.mCritDamageMax))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [CritDamageMax]");
			return false;
		}

		if (!csvReader.bind("CritDamageRate", attrRandomTemplate.mCritDamageRate))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [CritDamageRate]");
			return false;
		}

		if (!csvReader.bind("CritDefMin", attrRandomTemplate.mCritDefMin))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [CritDefMin]");
			return false;
		}

		if (!csvReader.bind("CritDefMax", attrRandomTemplate.mCritDefMax))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [CritDefMax]");
			return false;
		}

		if (!csvReader.bind("CritDefRate", attrRandomTemplate.mCritDefRate))
		{
			LOG_WARN("Failed to load attrRandomTemplate.csv for [CritDefRate]");
			return false;
		}

		mMap.insert(attrRandomTemplate.mId, attrRandomTemplate);
		AttrRandomLvQualityMap::Iter* qualityMapIter = mLvQualityMap.find(attrRandomTemplate.mLevel);
		
		if(qualityMapIter)
		{
			qualityMapIter->mValue.insert(attrRandomTemplate.mQuality,attrRandomTemplate.mId);
		}
		else
		{
			Map<UInt32,UInt64> qualityMap;
			qualityMap.insert(attrRandomTemplate.mQuality,attrRandomTemplate.mId);
			mLvQualityMap.insert(attrRandomTemplate.mLevel,qualityMap);
		}
	
	}
	fileStream.close();
	return true;
}