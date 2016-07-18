#include "ShopTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
CommodityTable::loadFromDbc(const String& fileName)
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
CommodityTable::reloadFromDbc(const String& fileName)
{
	CommodityTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload CommodityTable table.");
		return false;
	}
	return true;
}

bool 
CommodityTable::saveToDbc(const String& filePath)
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
CommodityTable::loadFromCsv(const String& filePath)
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

		CommodityTemplate commodityTemplate;
		// 公用
		if (!csvReader.bind("id", commodityTemplate.id))
		{
			LOG_WARN("Failed to load commodityTemplate.csv for [id]");
			return false;
		}
		if (!csvReader.bind("resourcesType1", commodityTemplate.resourcesType1))
		{
			LOG_WARN("Failed to load commodityTemplate.csv for [resourcesType1]");
			return false;
		}

		if (!csvReader.bind("subtype1", commodityTemplate.subtype1))
		{
			LOG_WARN("Failed to load commodityTemplate.csv for [subtype1]");
			return false;
		}
		if (!csvReader.bind("num1", commodityTemplate.num1))
		{
			LOG_WARN("Failed to load commodityTemplate.csv for [num1]");
			return false;
		}
		if (!csvReader.bind("costype1", commodityTemplate.costype1))
		{
			LOG_WARN("Failed to load commodityTemplate.csv for [costype1]");
			return false;
		}
		if (!csvReader.bind("costsubtype1", commodityTemplate.costsubtype1))
		{
			LOG_WARN("Failed to load commodityTemplate.csv for [costsubtype1]");
			return false;
		}
		if (!csvReader.bind("costnumber1", commodityTemplate.costnumber1))
		{
			LOG_WARN("Failed to load commodityTemplate.csv for [costnumber1]");
			return false;
		}

		mMap.insert(commodityTemplate.id, commodityTemplate);		
	}
	fileStream.close();
	return true;
}


bool 
GiftTable::loadFromDbc(const String& fileName)
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
GiftTable::reloadFromDbc(const String& fileName)
{
	GiftTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload GiftTable table.");
		return false;
	}
	return true;
}

bool 
GiftTable::saveToDbc(const String& filePath)
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
GiftTable::loadFromCsv(const String& filePath)
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

		GiftTemplate giftTemplate;
		// 公用

		if (!csvReader.bind("giftid", giftTemplate.giftid))
		{
			LOG_WARN("Failed to load giftTemplate.csv for [giftid]");
			return false;
		}
		if (!csvReader.bind("name", giftTemplate.name))
		{
			LOG_WARN("Failed to load giftTemplate.csv for [name]");
			return false;
		}

		if (!csvReader.bind("gifttype", giftTemplate.gifttype))
		{
			LOG_WARN("Failed to load giftTemplate.csv for [gifttype]");
			return false;
		}

		char str1[32]={};
		char str2[32]={};
		UInt32 num = 0;
		for (int i=1;i<50;i++)
		{
			num = 0;
			sprintf(str1, "%s%d","packageID",i);

			if (!csvReader.bind(str1,num))
			{
				LOG_WARN("Failed to load Gift.csv for [packageID]");
				break;
			}
			if (num != 0 )
			{
				giftTemplate.packageIDs.insertTail(num);
			}

		}
		for (int i=1;i<50;i++)
		{
			num = 0;
			sprintf(str2, "%s%d","awardcontentshopID",i);

			if (!csvReader.bind(str2,num))
			{
				LOG_WARN("Failed to load Gift.csv for [packageID]");
				break;
			}
			if (num != 0 )
			{
				giftTemplate.awardcontentshopIDs.insertTail(num);
			}

		}

		mMap.insert(giftTemplate.giftid, giftTemplate);		
	}
	fileStream.close();
	return true;
}


bool 
GiftShopTable::loadFromDbc(const String& fileName)
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
GiftShopTable::reloadFromDbc(const String& fileName)
{
	GiftShopMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload GiftShopTable table.");
		return false;
	}
	return true;
}

bool 
GiftShopTable::saveToDbc(const String& filePath)
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
GiftShopTable::loadFromCsv(const String& filePath)
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

		GiftShopTemplate giftShopTemplate;
		// 公用
		if (!csvReader.bind("ID", giftShopTemplate.ID))
		{
			LOG_WARN("Failed to load giftShopTemplate.csv for [ID]");
			return false;
		}
		if (!csvReader.bind("name", giftShopTemplate.name))
		{
			LOG_WARN("Failed to load giftShopTemplate.csv for [name]");
			return false;
		}

		if (!csvReader.bind("giftID", giftShopTemplate.giftID))
		{
			LOG_WARN("Failed to load giftShopTemplate.csv for [giftID]");
			return false;
		}
		if (!csvReader.bind("giftquality", giftShopTemplate.giftquality))
		{
			LOG_WARN("Failed to load giftShopTemplate.csv for [giftquality]");
			return false;
		}
		if (!csvReader.bind("gifttype", giftShopTemplate.gifttype))
		{
			LOG_WARN("Failed to load giftShopTemplate.csv for [gifttype]");
			return false;
		}
		if (!csvReader.bind("canbuyNum", giftShopTemplate.canbuyNum))
		{
			LOG_WARN("Failed to load giftShopTemplate.csv for [canbuyNum]");
			return false;
		}
		if (!csvReader.bind("qualification", giftShopTemplate.qualification))
		{
			LOG_WARN("Failed to load giftShopTemplate.csv for [qualification]");
			return false;
		}
		if (!csvReader.bind("program1", giftShopTemplate.program1))
		{
			LOG_WARN("Failed to load giftShopTemplate.csv for [program1]");
			return false;
		}
		if (!csvReader.bind("costtype", giftShopTemplate.costtype))
		{
			LOG_WARN("Failed to load giftShopTemplate.csv for [costtype]");
			return false;
		}
		if (!csvReader.bind("secondarytype", giftShopTemplate.secondarytype))
		{
			LOG_WARN("Failed to load giftShopTemplate.csv for [secondarytype]");
			return false;
		}
		if (!csvReader.bind("costNum", giftShopTemplate.costNum))
		{
			LOG_WARN("Failed to load giftShopTemplate.csv for [costNum]");
			return false;
		}
		if (!csvReader.bind("giftshow", giftShopTemplate.giftshow))
		{
			LOG_WARN("Failed to load giftShopTemplate.csv for [giftshow]");
			return false;
		}
		if (!csvReader.bind("favorable", giftShopTemplate.favorable))
		{
			LOG_WARN("Failed to load giftShopTemplate.csv for [favorable]");
			return false;
		}
		if (!csvReader.bind("whichday", giftShopTemplate.whichday))
		{
			LOG_WARN("Failed to load giftShopTemplate.csv for [whichday]");
			return false;
		}
		if (!csvReader.bind("starttime", giftShopTemplate.starttime))
		{
			LOG_WARN("Failed to load giftShopTemplate.csv for [starttime]");
			return false;
		}
		if (!csvReader.bind("endtime", giftShopTemplate.endtime))
		{
			LOG_WARN("Failed to load giftShopTemplate.csv for [endtime]");
			return false;
		}

		mMap.insert(giftShopTemplate.ID, giftShopTemplate);		
	}
	fileStream.close();
	return true;
}


bool 
MysticalshopTable::loadFromDbc(const String& fileName)
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
MysticalshopTable::reloadFromDbc(const String& fileName)
{
	MysticalshopTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload MysticalshopTable table.");
		return false;
	}
	return true;
}

bool 
MysticalshopTable::saveToDbc(const String& filePath)
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
MysticalshopTable::loadFromCsv(const String& filePath)
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

		MysticalshopTemplate mysticalshopTemplate;
		// 公用
		if (!csvReader.bind("ID", mysticalshopTemplate.ID))
		{
			LOG_WARN("Failed to load mysticalshopTemplate.csv for [ID]");
			return false;
		}
		if (!csvReader.bind("shopposition", mysticalshopTemplate.shopposition))
		{
			LOG_WARN("Failed to load mysticalshopTemplate.csv for [shopposition]");
			return false;
		}
		if (!csvReader.bind("vipLv", mysticalshopTemplate.vipLv))
		{
			LOG_WARN("Failed to load mysticalshopTemplate.csv for [vipLv]");
			return false;
		}
		if (!csvReader.bind("refreshTime", mysticalshopTemplate.refreshTime))
		{
			LOG_WARN("Failed to load mysticalshopTemplate.csv for [refreshTime]");
			return false;
		}
		if (!csvReader.bind("playerLv", mysticalshopTemplate.playerLv))
		{
			LOG_WARN("Failed to load mysticalshopTemplate.csv for [playerLv]");
			return false;
		}
		if (!csvReader.bind("timeawardcontentshop", mysticalshopTemplate.timeawardcontentshop))
		{
			LOG_WARN("Failed to load mysticalshopTemplate.csv for [timeawardcontentshop]");
			return false;
		}


		char str1[32]={};
		UInt32 num = 0;
		for (int i=1;i<50;i++)
		{
			num = 0;
			sprintf(str1, "%s%d","awardcontentshop",i);

			if (!csvReader.bind(str1,num))
			{
				LOG_WARN("Failed to load mysticalshop.csv for [awardcontentshop]");
				break;
			}
			if (num != 0 )
			{
				mysticalshopTemplate.awardcontentshops.insertTail(num);
			}
		}

		mMap.insert(mysticalshopTemplate.ID, mysticalshopTemplate);		
	}
	fileStream.close();
	return true;
}



bool 
ServantshopTable::loadFromDbc(const String& fileName)
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
ServantshopTable::reloadFromDbc(const String& fileName)
{
	ServantshopTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload ServantshopTable table.");
		return false;
	}
	return true;
}

bool 
ServantshopTable::saveToDbc(const String& filePath)
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
ServantshopTable::loadFromCsv(const String& filePath)
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

		ServantshopTemplate servantshopTemplate;
		// 公用
		if (!csvReader.bind("ID", servantshopTemplate.ID))
		{
			LOG_WARN("Failed to load servantshopTemplate.csv for [ID]");
			return false;
		}
		if (!csvReader.bind("shopposition", servantshopTemplate.shopposition))
		{
			LOG_WARN("Failed to load servantshopTemplate.csv for [shopposition]");
			return false;
		}

		if (!csvReader.bind("vipLv", servantshopTemplate.vipLv))
		{
			LOG_WARN("Failed to load servantshopTemplate.csv for [vipLv]");
			return false;
		}
// 		if (!csvReader.bind("playerLv", servantshopTemplate.playerLv))
// 		{
// 			LOG_WARN("Failed to load servantshopTemplate.csv for [playerLv]");
// 			return false;
// 		}
		if (!csvReader.bind("refreshTime", servantshopTemplate.refreshTime))
		{
			LOG_WARN("Failed to load servantshopTemplate.csv for [refreshTime]");
			return false;
		}
		if (!csvReader.bind("timeawardcontentshop", servantshopTemplate.timeawardcontentshop))
		{
			LOG_WARN("Failed to load servantshopTemplate.csv for [timeawardcontentshop]");
			return false;
		}


		char str1[32]={};
		UInt32 num = 0;
		for (int i=1;i<50;i++)
		{
			num = 0;
			sprintf(str1, "%s%d","awardcontentshop",i);

			if (!csvReader.bind(str1,num))
			{
				LOG_WARN("Failed to load Servantshop.csv for [awardcontentshop]");
				break;
			}
			if (num != 0 )
			{
				servantshopTemplate.awardcontentshops.insertTail(num);
			}
		}

		mMap.insert(servantshopTemplate.ID, servantshopTemplate);		
	}
	fileStream.close();
	return true;
}



bool 
AwardContentShopTable::loadFromDbc(const String& fileName)
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
AwardContentShopTable::reloadFromDbc(const String& fileName)
{
	mAwardContentList tmpList = mContentList;
	mContentList.clear();
	if (!loadFromDbc(fileName))
	{
		mContentList = tmpList;
		LOG_WARN("Failed to reload award table.");
		return false;
	}
	return true;
}



bool 
AwardContentShopTable::loadFromCsv(const String& filePath)
{
	mContentList.clear();
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
		AwardContentTemplate awardcontenemplate;

		if (!csvReader.bind("id", awardcontenemplate.id))
		{
			LOG_WARN("Failed to load AwardContent.csv for [id]");
			return false;
		}
		char str1[32]={};
		char str2[32]={};
		char str3[32]={};
		char str4[32]={};
		char str5[32]={};

		for (int i=1;i<100;i++)
		{
			Content contern;

			sprintf(str1, "%s%d","partType",i);
			sprintf(str2, "%s%d","weight",i);
			sprintf(str3, "%s%d","resourcestype",i);
			sprintf(str4, "%s%d","subtype",i);
			sprintf(str5, "%s%d","num",i);

			if (!csvReader.bind(str1, contern.contenttype))
			{
				LOG_WARN("Failed to load AwardContent.csv for [contenttype]");
				break;
			}

			if (contern.contenttype == 0)
			{
				break;
			}
			if (!csvReader.bind(str2, contern.weight))
			{
				LOG_WARN("Failed to load AwardContent.csv for [weight]");
				break;
			}


			if (!csvReader.bind(str3, contern.resourcestype))
			{
				LOG_WARN("Failed to load AwardContent.csv for [resourcestype]");
				break;
			}
			if (!csvReader.bind(str4, contern.subtype))
			{
				LOG_WARN("Failed to load AwardContent.csv for [subtype]");
				break;
			}
			if (!csvReader.bind(str5, contern.num))
			{
				LOG_WARN("Failed to load AwardContent.csv for [num]");
				break;
			}
			awardcontenemplate.ContentList.insertTail(contern);

		}	

		mContentList.insertTail(awardcontenemplate);
	}
	fileStream.close();
	return true;
}

bool AwardContentShopTable::saveToDbc(const String& filePath)
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
GainStrengthTable::loadFromDbc(const String& fileName)
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
GainStrengthTable::reloadFromDbc(const String& fileName)
{
	List<GainStrengthTemplate> tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload CommodityTable table.");
		return false;
	}
	return true;
}

bool 
GainStrengthTable::saveToDbc(const String& filePath)
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
GainStrengthTable::loadFromCsv(const String& filePath)
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

		GainStrengthTemplate gainStrengthTemplate;


		if (!csvReader.bind("viplevel", gainStrengthTemplate.viplevel))
		{
			LOG_WARN("Failed to load AwardContent.csv for [viplevel]");
			return false;
		}
		if (!csvReader.bind("buytimes", gainStrengthTemplate.buytimes))
		{
			LOG_WARN("Failed to load AwardContent.csv for [buytimes]");
			return false;
		}
		if (!csvReader.bind("cost", gainStrengthTemplate.cost))
		{
			LOG_WARN("Failed to load AwardContent.csv for [cost]");
			return false;
		}
		if (!csvReader.bind("addstrength", gainStrengthTemplate.addstrength))
		{
			LOG_WARN("Failed to load AwardContent.csv for [addstrength]");
			return false;
		}
		mMap.insertTail(gainStrengthTemplate);		

	}
	fileStream.close();
	return true;
}














bool 
GainWealthTable::loadFromDbc(const String& fileName)
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
GainWealthTable::reloadFromDbc(const String& fileName)
{
	List<GainWealthTemplate> tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload CommodityTable table.");
		return false;
	}
	return true;
}

bool 
GainWealthTable::saveToDbc(const String& filePath)
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
GainWealthTable::loadFromCsv(const String& filePath)
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

		GainWealthTemplate gainWealthTemplate;


		if (!csvReader.bind("VIPlevel", gainWealthTemplate.VIPlevel))
		{
			LOG_WARN("Failed to load AwardContent.csv for [VIPlevel]");
			return false;
		}
		if (!csvReader.bind("times", gainWealthTemplate.times))
		{
			LOG_WARN("Failed to load AwardContent.csv for [times]");
			return false;
		}
		if (!csvReader.bind("cost", gainWealthTemplate.cost))
		{
			LOG_WARN("Failed to load AwardContent.csv for [cost]");
			return false;
		}
		if (!csvReader.bind("gaincoin", gainWealthTemplate.gaincoin))
		{
			LOG_WARN("Failed to load AwardContent.csv for [gaincoin]");
			return false;
		}
		mMap.insertTail(gainWealthTemplate);		

	}
	fileStream.close();
	return true;
}














bool 
PremiumsTable::loadFromDbc(const String& fileName)
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
PremiumsTable::reloadFromDbc(const String& fileName)
{
	PremiumsTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload CommodityTable table.");
		return false;
	}
	return true;
}

bool 
PremiumsTable::saveToDbc(const String& filePath)
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
PremiumsTable::loadFromCsv(const String& filePath)
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

		PremiumsTemplate premiumsTemplate;


		if (!csvReader.bind("ID", premiumsTemplate.ID))
		{
			LOG_WARN("Failed to load AwardContent.csv for [ID]");
			return false;
		}
		if (!csvReader.bind("type", premiumsTemplate.type))
		{
			LOG_WARN("Failed to load AwardContent.csv for [type]");
			return false;
		}
		if (!csvReader.bind("contentID", premiumsTemplate.contentID))
		{
			LOG_WARN("Failed to load AwardContent.csv for [contentID]");
			return false;
		}
	
		mMap.insert(premiumsTemplate.ID, premiumsTemplate);		

	}
	fileStream.close();
	return true;
}

