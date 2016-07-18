#include "ServantFloorTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
ServantFloorTable::loadFromDbc(const String& fileName)
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
ServantFloorTable::reloadFromDbc(const String& fileName)
{
	ServantFloorTemplateMap tmpMap = mMap;
	ServantFloorReverseMap tmpReversMap = mReverseMap;

	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		mReverseMap = tmpReversMap;
		LOG_WARN("Failed to reload servantfloor table.");
		return false;
	}
	return true;
}

bool 
ServantFloorTable::saveToDbc(const String& filePath)
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
ServantFloorTable::loadFromCsv(const String& filePath)
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
		ServantFloorTemplate servantFloorTemp;
		// ¹«ÓÃ
		if (!csvReader.bind("id", servantFloorTemp.mId))
		{
			LOG_WARN("Failed to load servantfloor.csv for [id]");
			return false;
		}

		if (!csvReader.bind("servantid", servantFloorTemp.mServantId))
		{
			LOG_WARN("Failed to load servantfloor.csv for [servantid]");
			return false;
		}

		if (!csvReader.bind("floor", servantFloorTemp.mFloor))
		{
			LOG_WARN("Failed to load servantfloor.csv for [floor]");
			return false;
		}

		if (!csvReader.bind("slot1", servantFloorTemp.mSlot1))
		{
			LOG_WARN("Failed to load servantfloor.csv for [slot1]");
			return false;
		}

		if (!csvReader.bind("slot2", servantFloorTemp.mSlot2))
		{
			LOG_WARN("Failed to load servantfloor.csv for [slot2]");
			return false;
		}

		if (!csvReader.bind("slot3", servantFloorTemp.mSlot3))
		{
			LOG_WARN("Failed to load servantfloor.csv for [slot3]");
			return false;
		}

		if (!csvReader.bind("slot4", servantFloorTemp.mSlot4))
		{
			LOG_WARN("Failed to load servantfloor.csv for [slot4]");
			return false;
		}

		if (!csvReader.bind("attr1", servantFloorTemp.mAttrType1))
		{
			LOG_WARN("Failed to load servantfloor.csv for [attr1]");
			return false;
		}

		if (!csvReader.bind("value1", servantFloorTemp.mAttrValue1))
		{
			LOG_WARN("Failed to load servantfloor.csv for [value1]");
			return false;
		}

		if (!csvReader.bind("attr2", servantFloorTemp.mAttrType2))
		{
			LOG_WARN("Failed to load servantfloor.csv for [attr2]");
			return false;
		}

		if (!csvReader.bind("value2", servantFloorTemp.mAttrValue2))
		{
			LOG_WARN("Failed to load servantfloor.csv for [value2]");
			return false;
		}


		if (!csvReader.bind("attr3", servantFloorTemp.mAttrType3))
		{
			LOG_WARN("Failed to load servantfloor.csv for [attr3]");
			return false;
		}

		if (!csvReader.bind("value3", servantFloorTemp.mAttrValue3))
		{
			LOG_WARN("Failed to load servantfloor.csv for [value3]");
			return false;
		}

		if (!csvReader.bind("attr4", servantFloorTemp.mAttrType4))
		{
			LOG_WARN("Failed to load servantfloor.csv for [attr4]");
			return false;
		}

		if (!csvReader.bind("value4", servantFloorTemp.mAttrValue4))
		{
			LOG_WARN("Failed to load servantfloor.csv for [value4]");
			return false;
		}

		if (!csvReader.bind("attr5", servantFloorTemp.mAttrType5))
		{
			LOG_WARN("Failed to load servantfloor.csv for [attr5]");
			return false;
		}

		if (!csvReader.bind("value5", servantFloorTemp.mAttrValue5))
		{
			LOG_WARN("Failed to load servantfloor.csv for [value5]");
			return false;
		}

		if (!csvReader.bind("power", servantFloorTemp.mPower))
		{
			LOG_WARN("Failed to load servantfloor.csv for [power]");
			return false;
		}

		mMap.insert(servantFloorTemp.mId, servantFloorTemp);
		ServantFloorReverseMap::Iter *  floorMapIter = mReverseMap.find(servantFloorTemp.mServantId);
		if(floorMapIter)
		{
			floorMapIter->mValue.insert(servantFloorTemp.mFloor, servantFloorTemp);
		}
		else
		{
			Map<UInt32 , ServantFloorTemplate> servantFloorMap;
			servantFloorMap.insert(servantFloorTemp.mFloor, servantFloorTemp);
			mReverseMap.insert(servantFloorTemp.mServantId, servantFloorMap);
		}

	}
	fileStream.close();
	return true;
}

