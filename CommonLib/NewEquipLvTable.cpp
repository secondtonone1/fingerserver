#include "NewEquipLvTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
NewEquipLvTable::loadFromDbc(const String& fileName)
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
NewEquipLvTable::reloadFromDbc(const String& fileName)
{
	NewEquipLvTemplateMap tmpMap = mMap;
	NewEquipLvReverseMap  reverseMap = mReverseMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		mReverseMap = reverseMap;
		LOG_WARN("Failed to reload NewEquipLvTable table.");
		return false;
	}
	return true;
}

bool 
NewEquipLvTable::saveToDbc(const String& filePath)
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
NewEquipLvTable::loadFromCsv(const String& filePath)
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
		NewEquipLvTemplate newEquipLvTemp;
		// ¹«ÓÃ
		if (!csvReader.bind("id", newEquipLvTemp.mId))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [id]");
			return false;
		}

		if (!csvReader.bind("equipId", newEquipLvTemp.mEquipId))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [equipId]");
			return false;
		}

		if (!csvReader.bind("level", newEquipLvTemp.mLv))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [level]");
			return false;
		}

		if (!csvReader.bind("name", newEquipLvTemp.mName))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [name]");
			return false;
		}

		if (!csvReader.bind("icon", newEquipLvTemp.mIcon))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [icon]");
			return false;
		}

		if (!csvReader.bind("quality", newEquipLvTemp.mQuality))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [quality]");
			return false;
		}

		if (!csvReader.bind("levelUpNeedCoin", newEquipLvTemp.mLvUpNeedCoin))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [levelUpNeedCoin]");
			return false;
		}

		if (!csvReader.bind("levelUpNeedResId1", newEquipLvTemp.mLvUpNeedResId1))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [levelUpNeedResId1]");
			return false;
		}

		if (!csvReader.bind("levelUpNeedResCnt1", newEquipLvTemp.mLvUpNeedResCnt1))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [levelUpNeedResCnt1]");
			return false;
		}
	
		if (!csvReader.bind("levelUpNeedResId2", newEquipLvTemp.mLvUpNeedResId2))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [levelUpNeedResId2]");
			return false;
		}

		if (!csvReader.bind("levelUpNeedResCnt2", newEquipLvTemp.mLvUpNeedResCnt2))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [levelUpNeedResCnt2]");
			return false;
		}

		if (!csvReader.bind("isStepUp", newEquipLvTemp.mIsStepUp))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [isStepUp]");
			return false;
		}

		if (!csvReader.bind("attrType", newEquipLvTemp.mAttrType))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [attrType]");
			return false;
		}

		if (!csvReader.bind("attrValue", newEquipLvTemp.mAttrValue))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [attrValue]");
			return false;
		}

		if (!csvReader.bind("nextLevelAttrType", newEquipLvTemp.mNextLevelAttrValue))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [nextLevelAttrType]");
			return false;
		}

		if (!csvReader.bind("nextLevelAttrValue", newEquipLvTemp.mNextLevelAttrValue))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [nextLevelAttrValue]");
			return false;
		}

		if (!csvReader.bind("equipPosition", newEquipLvTemp.mEquipPos))
		{
			LOG_WARN("Failed to load newequiplvtable.csv for [equipPosition]");
			return false;
		}

		mMap.insert(newEquipLvTemp.mId, newEquipLvTemp);

		NewEquipLvReverseMap::Iter * reverseMapIter = mReverseMap.find(newEquipLvTemp.mEquipId);
		
		if(reverseMapIter)
		{
			reverseMapIter->mValue.insert(newEquipLvTemp.mLv, newEquipLvTemp);
		}
		else
		{
			Map<UInt32, NewEquipLvTemplate> equipLvMap;
			equipLvMap.insert(newEquipLvTemp.mLv, newEquipLvTemp);
			mReverseMap.insert(newEquipLvTemp.mEquipId, equipLvMap);
			

		}
	}
	fileStream.close();
	return true;
}

