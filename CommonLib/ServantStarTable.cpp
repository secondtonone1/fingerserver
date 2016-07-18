#include "ServantStarTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
ServantStarTable::loadFromDbc(const String& fileName)
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
ServantStarTable::reloadFromDbc(const String& fileName)
{
	ServantStarTemplateMap tmpMap = mMap;
	ServantStarReverseMap tmpReversMap = mReverseMap;

	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
        mReverseMap = tmpReversMap;
		LOG_WARN("Failed to reload ServantStarTable table.");
		return false;
	}
	return true;
}

bool 
ServantStarTable::saveToDbc(const String& filePath)
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
ServantStarTable::loadFromCsv(const String& filePath)
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
		ServantStarTemplate servantStarTemp;
		// ¹«ÓÃ
		if (!csvReader.bind("id", servantStarTemp.mId))
		{
			LOG_WARN("Failed to load servantstar.csv for [id]");
			return false;
		}

		if (!csvReader.bind("servant", servantStarTemp.mServantId))
		{
			LOG_WARN("Failed to load servantstar.csv for [servant]");
			return false;
		}

		if (!csvReader.bind("star", servantStarTemp.mStar))
		{
			LOG_WARN("Failed to load servantstar.csv for [star]");
			return false;
		}

		if (!csvReader.bind("cost", servantStarTemp.mCost))
		{
			LOG_WARN("Failed to load servantstar.csv for [cost]");
			return false;
		}

		if (!csvReader.bind("count", servantStarTemp.mCount))
		{
			LOG_WARN("Failed to load servantstar.csv for [count]");
			return false;
		}

		if (!csvReader.bind("power", servantStarTemp.mPower))
		{
			LOG_WARN("Failed to load servantstar.csv for [power]");
			return false;
		}

		if (!csvReader.bind("attr", servantStarTemp.mAttrType))
		{
			LOG_WARN("Failed to load servantstar.csv for [attr]");
			return false;
		}

		if (!csvReader.bind("value", servantStarTemp.mAttrValue))
		{
			LOG_WARN("Failed to load servantstar.csv for [value]");
			return false;
		}

		
		mMap.insert(servantStarTemp.mId, servantStarTemp);
		ServantStarReverseMap::Iter *  starMapIter = mReverseMap.find(servantStarTemp.mServantId);
		if(starMapIter)
		{
			starMapIter->mValue.insert(servantStarTemp.mStar, servantStarTemp);
		}
		else
		{
			Map<UInt32 , ServantStarTemplate> servantStarMap;
			servantStarMap.insert(servantStarTemp.mStar, servantStarTemp);
			mReverseMap.insert(servantStarTemp.mServantId, servantStarMap);
		}

	}
	fileStream.close();
	return true;
}

