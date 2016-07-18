#include "ItemTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
ItemTable::loadFromDbc(const String& fileName)
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
ItemTable::reloadFromDbc(const String& fileName)
{
    ItemTemplateMap tmpMap = mMap;
    clear();
    if (!loadFromDbc(fileName))
    {
        mMap = tmpMap;
        LOG_WARN("Failed to reload ItemTemplate table.");
        return false;
    }
    return true;
}

bool 
ItemTable::saveToDbc(const String& filePath)
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
ItemTable::loadFromCsv(const String& filePath)
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
		ItemTemplate itemTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("itemId", itemTemplate.mTemplateId))
		{
			LOG_WARN("Failed to load ItemTemplate.csv for [itemId]");
			return false;
		}

		if(!csvReader.bind("bagTab",itemTemplate.mBagTab))
		{
			LOG_WARN("Failed to load ItemTemplate.csv for [tag]");
			return false;
		}

        if (!csvReader.bind("type", itemTemplate.mTypeId))
        {
            LOG_WARN("Failed to load ItemTemplate.csv for [type]");
            return false;
        }

        if (!csvReader.bind("subType", itemTemplate.mSubTypeId))
        {
            LOG_WARN("Failed to load ItemTemplate.csv for [subType]");
            return false;
        }

        if (!csvReader.bind("name", itemTemplate.mName))
        {
            LOG_WARN("Failed to load ItemTemplate.csv for [ItemName]");
            return false;
        }

        if (!csvReader.bind("desc", itemTemplate.mDes))
        {
            LOG_WARN("Failed to load ItemTemplate.csv for [Des]");
            return false;
        }

        if (!csvReader.bind("icon", itemTemplate.mIcon))
        {
            LOG_WARN("Failed to load ItemTemplate.csv for [icon]");
            return false;
        }

        if (!csvReader.bind("quality", itemTemplate.mQuality))
        {
            LOG_WARN("Failed to load ItemTemplate.csv for [quality]");
            return false;
        }

		if (!csvReader.bind("beginTime", itemTemplate.mBeginTime))
		{
			LOG_WARN("Failed to load ItemTemplate.csv for [beginTime]");
			return false;
		}

        if (!csvReader.bind("invalid", itemTemplate.mInvalid))
        {
            LOG_WARN("Failed to load ItemTemplate.csv for [invalid]");
            return false;
        }

        if (!csvReader.bind("useful", itemTemplate.mUseful))
        {
            LOG_WARN("Failed to load ItemTemplate.csv for [useful]");
            return false;
        }

        if (!csvReader.bind("useLimit1", itemTemplate.mUseLimit1))
        {
            LOG_WARN("Failed to load ItemTemplate.csv for [useLimit1]");
            return false;
        }

		if (!csvReader.bind("useLimit2", itemTemplate.mUseLimit2))
		{
			LOG_WARN("Failed to load ItemTemplate.csv for [useLimit2]");
			return false;
		}

		if (!csvReader.bind("useLimit3", itemTemplate.mUseLimit3))
		{
			LOG_WARN("Failed to load ItemTemplate.csv for [useLimit3]");
			return false;
		}

		if (!csvReader.bind("useLimit4", itemTemplate.mUseLimit4))
		{
			LOG_WARN("Failed to load ItemTemplate.csv for [useLimit4]");
			return false;
		}

		if (!csvReader.bind("useLimit5", itemTemplate.mUseLimit5))
		{
			LOG_WARN("Failed to load ItemTemplate.csv for [useLimit5]");
			return false;
		}

        if (!csvReader.bind("amountLimit", itemTemplate.mAmountLimit))
        {
            LOG_WARN("Failed to load ItemTemplate.csv for [amountLimit]");
            return false;
        }

        if (!csvReader.bind("useTime", itemTemplate.mUseful))
        {
            LOG_WARN("Failed to load ItemTemplate.csv for [useTime]");
            return false;
        }

        if (!csvReader.bind("power", itemTemplate.mPower))
        {
            LOG_WARN("Failed to load ItemTemplate.csv for [power]");
            return false;
        }

        if (!csvReader.bind("cd", itemTemplate.mCD))
        {
            LOG_WARN("Failed to load ItemTemplate.csv for [CD]");
            return false;
        }

        if (!csvReader.bind("stackAmount", itemTemplate.mStackAmount))
        {
            LOG_WARN("Failed to load ItemTemplate.csv for [stackAmount]");
            return false;
        }

        if (!csvReader.bind("order", itemTemplate.mOrder))
        {
            LOG_WARN("Failed to load ItemTemplate.csv for [order]");
            return false;
        }

        if (!csvReader.bind("sale", itemTemplate.mSale))
        {
            LOG_WARN("Failed to load ItemTemplate.csv for [sale]");
            return false;
        }

		if (!csvReader.bind("price", itemTemplate.mSale))
		{
			LOG_WARN("Failed to load ItemTemplate.csv for [price]");
			return false;
		}

		if (!csvReader.bind("content", itemTemplate.mContent))
		{
			LOG_WARN("Failed to load ItemTemplate.csv for [content]");
			return false;
		}


		mMap.insert(itemTemplate.mTemplateId, itemTemplate);
	}
	fileStream.close();
	return true;
}

