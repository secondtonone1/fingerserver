#include "HoarstoneLvTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
HoarstoneLvTable::loadFromDbc(const String& fileName)
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
HoarstoneLvTable::reloadFromDbc(const String& fileName)
{
	HoarstoneLvTemplateMap tmpMap = mMap;
	HoarstoneLvReversMap tmpReversMap = mReversMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		mReversMap = tmpReversMap;
		LOG_WARN("Failed to reload HoarstoneLvTemplate table.");
		return false;
	}
	return true;
}

bool 
HoarstoneLvTable::saveToDbc(const String& filePath)
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
HoarstoneLvTable::loadFromCsv(const String& filePath)
{
	mMap.clear();
	mReversMap.clear();

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
		HoarstoneLvTemplate hoarstoneLvTemp;
		// ¹«ÓÃ
		if (!csvReader.bind("id", hoarstoneLvTemp.mId))
		{
			LOG_WARN("Failed to load hoarstoneLv.csv for [id]");
			return false;
		}

		if (!csvReader.bind("idx", hoarstoneLvTemp.mIdx))
		{
			LOG_WARN("Failed to load hoarstoneLv.csv for [idx]");
			return false;
		}

		if (!csvReader.bind("level", hoarstoneLvTemp.mLevel))
		{
			LOG_WARN("Failed to load hoarstoneLv.csv for [level]");
			return false;
		}
		
		
		if (!csvReader.bind("power", hoarstoneLvTemp.mPower))
		{
				LOG_WARN("Failed to load hoarstoneLv.csv for [power]");
				return false;
		}

		if (!csvReader.bind("nextLevelAddPower", hoarstoneLvTemp.mNextLvAddPower))
		{
			LOG_WARN("Failed to load hoarstoneLv.csv for [nextLevelAddPower]");
			return false;
		}

		if (!csvReader.bind("costCoin", hoarstoneLvTemp.mCostCoin))
		{
			LOG_WARN("Failed to load hoarstoneLv.csv for [costCoin]");
			return false;
		}

		if (!csvReader.bind("costPiece", hoarstoneLvTemp.mCostPiece))
		{
			LOG_WARN("Failed to load hoarstoneLv.csv for [costPiece]");
			return false;
		}

		if (!csvReader.bind("attrType1", hoarstoneLvTemp.mAttrType1))
		{
			LOG_WARN("Failed to load hoarstoneLv.csv for [attrType1]");
			return false;
		}

		if (!csvReader.bind("attrValue1", hoarstoneLvTemp.mAttrValue1))
		{
			LOG_WARN("Failed to load hoarstoneLv.csv for [attrValue1]");
			return false;
		}

		if (!csvReader.bind("attrType2", hoarstoneLvTemp.mAttrType2))
		{
			LOG_WARN("Failed to load hoarstoneLv.csv for [attrType2]");
			return false;
		}

		if (!csvReader.bind("attrValue2", hoarstoneLvTemp.mAttrValue2))
		{
			LOG_WARN("Failed to load hoarstoneLv.csv for [attrValue2]");
			return false;
		}

		if (!csvReader.bind("attrType3", hoarstoneLvTemp.mAttrType3))
		{
			LOG_WARN("Failed to load hoarstoneLv.csv for [attrType3]");
			return false;
		}

		if (!csvReader.bind("attrValue3", hoarstoneLvTemp.mAttrValue3))
		{
			LOG_WARN("Failed to load hoarstoneLv.csv for [attrValue3]");
			return false;
		}

		if (!csvReader.bind("attrType4", hoarstoneLvTemp.mAttrType4))
		{
			LOG_WARN("Failed to load hoarstoneLv.csv for [attrType4]");
			return false;
		}

		if (!csvReader.bind("attrValue4", hoarstoneLvTemp.mAttrValue4))
		{
			LOG_WARN("Failed to load hoarstoneLv.csv for [attrValue4]");
			return false;
		}

		if (!csvReader.bind("attrType5", hoarstoneLvTemp.mAttrType5))
		{
			LOG_WARN("Failed to load hoarstoneLv.csv for [attrType5]");
			return false;
		}

		if (!csvReader.bind("attrValue5", hoarstoneLvTemp.mAttrValue5))
		{
			LOG_WARN("Failed to load hoarstoneLv.csv for [attrValue5]");
			return false;
		}


		mMap.insert(hoarstoneLvTemp.mId, hoarstoneLvTemp);
		HoarstoneLvReversMap::Iter * reversIter = mReversMap.find(hoarstoneLvTemp.mIdx);
		
		if(reversIter)
		{
			reversIter->mValue.insert(hoarstoneLvTemp.mLevel,hoarstoneLvTemp);
		}
		else
		{
			Map<UInt32, HoarstoneLvTemplate> lvMap;
			lvMap.insert(hoarstoneLvTemp.mLevel,hoarstoneLvTemp);
			mReversMap.insert(hoarstoneLvTemp.mIdx, lvMap);

		}

	}
	fileStream.close();
	return true;
}

