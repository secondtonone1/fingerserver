#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"
#include "Award.h"

using namespace Lynx;


bool 
AwardTable::loadFromDbc(const String& fileName)
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
AwardTable::reloadFromDbc(const String& fileName)
{
	mAwardList tmpList = mList;
	mList.clear();
	if (!loadFromDbc(fileName))
	{
		mList = tmpList;
		LOG_WARN("Failed to reload award table.");
		return false;
	}
	return true;
}



bool 
AwardTable::loadFromCsv(const String& filePath)
{
	mList.clear();
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
		AwardTemplate awardtemplate;
	
		if (!csvReader.bind("id", awardtemplate.id))
		{
			LOG_WARN("Failed to load Award.csv for [id]");
			return false;
		}
		char str1[32]={};
		char str2[32]={};
		char str3[32]={};
		
		for (int i=1;i<100;i++)
		{
			AwardItem awarditem;
			sprintf(str1, "%s%d","type",i);
			sprintf(str2, "%s%d","weight",i);
			sprintf(str3, "%s%d","awardcontent",i);
			
			if (!csvReader.bind(str1, awarditem.bigtype))
			{
				LOG_WARN("Failed to load Award.csv for [type]");
				break;
			}
			if (awarditem.bigtype == 0)
			{
				break;
			}
			
			if (!csvReader.bind(str2, awarditem.weight))
			{
				LOG_WARN("Failed to load Award.csv for [weight]");
				break;
			}
			if (!csvReader.bind(str3, awarditem.awardcontent))
			{
				LOG_WARN("Failed to load Award.csv for [awardcontent]");
				break;
			}
			awardtemplate.AwardItems.insertTail(awarditem);
			
		}	
	
		mList.insertTail(awardtemplate);
	}
	fileStream.close();
	return true;
}

bool AwardTable::saveToDbc(const String& filePath)
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
AwardCardTable::loadFromDbc(const String& fileName)
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
AwardCardTable::reloadFromDbc(const String& fileName)
{
	mAwardCardList tmpList = mCardList;
	mCardList.clear();
	if (!loadFromDbc(fileName))
	{
		mCardList = tmpList;
		LOG_WARN("Failed to reload award table.");
		return false;
	}
	return true;
}



bool 
AwardCardTable::loadFromCsv(const String& filePath)
{
	mCardList.clear();
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
		AwardCardTemplate awardcardtemplate;

		if (!csvReader.bind("id", awardcardtemplate.id))
		{
			LOG_WARN("Failed to load awardcard.csv for [id]");
			return false;
		}
		char str1[32]={};
		char str2[32]={};
		char str3[32]={};
		char str4[32]={};		
		char str5[32]={};
		
		for (int i=1;i<100;i++)
		{	
		
			Resource resource;
			sprintf(str1, "%s%d","weight",i);
			sprintf(str2, "%s%d","resourcestype",i);
			sprintf(str3, "%s%d","subtype",i);
			sprintf(str4, "%s%d","num",i);
			if (!csvReader.bind(str1, resource.weight))
			{
				LOG_WARN("Failed to load awardcard.csv for [weight]");
				break;
			}
			if (resource.weight == 0)
			{
				break;
			}

			if (!csvReader.bind(str2, resource.resourcestype))
			{
				LOG_WARN("Failed to load awardcard.csv for [resourcestype]");
				break;
			}
			
			if (!csvReader.bind(str3, resource.subtype))
			{
				LOG_WARN("Failed to load awardcard.csv for [subtype]");
				break;
			}
			if (!csvReader.bind(str4, resource.num))
			{
				LOG_WARN("Failed to load awardcard.csv for [num]");
				break;
			}
			
			awardcardtemplate.resourcesList.insertTail(resource);

		}	

		mCardList.insertTail(awardcardtemplate);
	}
	fileStream.close();
	return true;
}

bool AwardCardTable::saveToDbc(const String& filePath)
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
LotteryTable::loadFromDbc(const String& fileName)
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
LotteryTable::reloadFromDbc(const String& fileName)
{
	List<LotteryTemplate> tmpList = mContentList;
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
LotteryTable::loadFromCsv(const String& filePath)
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
	String keyValue = "";
	while (csvReader.readLine())
	{
		LotteryTemplate awardcontenemplate;

		if (!csvReader.bind("id", awardcontenemplate.id))
		{
			LOG_WARN("Failed to load AwardContent.csv for [id]");
			return false;
		}
		if (!csvReader.bind("award", awardcontenemplate.firstAward))
		{
			LOG_WARN("Failed to load AwardContent.csv for [award]");
			return false;
		}

		char str1[32]={};
		char str2[32]={};


		for (int i=1;i<100;i++)
		{
			TypeNumValue skeyValue;
			sprintf(str1, "%s%d","award",i);
			sprintf(str2, "%s%d","conditions",i);


			if (!csvReader.bind(str1, skeyValue.uType))
			{
				LOG_WARN("Failed to load lottery.csv for [conditions]");
				break;
			}
// 			Vector<String> strVector;
// 			lynxStrSplit(keyValue, ";", strVector, true);
// 			if (strVector.size() == 2)
// 			{
// 				skeyValue.uType = atoi(strVector[0].c_str());
// 				skeyValue.uValue = atoi(strVector[1].c_str());
// 			}
// 			else
// 			{
// 				LOG_WARN("Failed to load AwardContent.csv for [conditions]");
// 				break;
// 			}

			if (skeyValue.uType == 0)
			{
				break;
			}
			if (!csvReader.bind(str2, skeyValue.uValue))
			{
				LOG_WARN("Failed to load lottery.csv for [award]");
				break;
			}
			awardcontenemplate.ContentList.insertTail(skeyValue);

		}	

		mContentList.insertTail(awardcontenemplate);
	}
	fileStream.close();
	return true;
}

bool LotteryTable::saveToDbc(const String& filePath)
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
BoxCounterTable::loadFromDbc(const String& fileName)
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
BoxCounterTable::reloadFromDbc(const String& fileName)
{
	List<BoxTemplate> tmpList = mContentList;
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
BoxCounterTable::loadFromCsv(const String& filePath)
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
		BoxTemplate awardcontenemplate;

		if (!csvReader.bind("id", awardcontenemplate.id))
		{
			LOG_WARN("Failed to load AwardContent.csv for [id]");
			return false;
		}

		char str1[32]={};
		char str2[32]={};	

		for (int i=1;i<100;i++)
		{
			KeyValue keyValue;
			sprintf(str1, "%s%d","conditions",i);
			sprintf(str2, "%s%d","awardid",i);// lottery.xlxs ²»Ò»Ñù award


			if (!csvReader.bind(str1, keyValue.key))
			{
				LOG_WARN("Failed to load AwardContent.csv for [conditions]");
				break;
			}

			if (keyValue.key == 0)
			{
				break;
			}
			if (!csvReader.bind(str2, keyValue.value))
			{
				LOG_WARN("Failed to load AwardContent.csv for [award]");
				break;
			}
			awardcontenemplate.ContentList.insertTail(keyValue);

		}	

		mContentList.insertTail(awardcontenemplate);
	}
	fileStream.close();
	return true;
}

bool BoxCounterTable::saveToDbc(const String& filePath)
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
AwardContentTable::loadFromDbc(const String& fileName)
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
AwardContentTable::reloadFromDbc(const String& fileName)
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
AwardContentTable::loadFromCsv(const String& filePath)
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

bool AwardContentTable::saveToDbc(const String& filePath)
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
SkeySvalueTable::loadFromDbc(const String& fileName)
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
SkeySvalueTable::reloadFromDbc(const String& fileName)
{
	List<SkeySvalueTemplate> tmpList = mSkeySvalue;
	mSkeySvalue.clear();
	if (!loadFromDbc(fileName))
	{
		mSkeySvalue = tmpList;
		LOG_WARN("Failed to reload award table.");
		return false;
	}
	return true;
}



bool 
SkeySvalueTable::loadFromCsv(const String& filePath)
{
	mSkeySvalue.clear();
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
		SkeySvalueTemplate awardcontenemplate;

		if (!csvReader.bind("name", awardcontenemplate.name))
		{
			LOG_WARN("Failed to load SkeySvalueTable for [id]");
			return false;
		}
		char str1[32]={};
		char str2[32]={};
		
		for (int i=1;i<100;i++)
		{
			SkeySvalue skeySvalue;

			sprintf(str1, "%s%d","skey",i);
			sprintf(str2, "%s%d","svalue",i);			
			if (!csvReader.bind(str1, skeySvalue.sKey))
			{
				LOG_WARN("Failed to load SkeySvalue.csv for [key]");
				break;
			}

			if (skeySvalue.sKey == "")
			{
				break;
			}
			if (!csvReader.bind(str2, skeySvalue.sValue))
			{
				LOG_WARN("Failed to load SkeySvalue.csv for [values]");
				break;
			}
			awardcontenemplate.SkeySvalueList.insertTail(skeySvalue);
		}	
		mSkeySvalue.insertTail(awardcontenemplate);

	}
	fileStream.close();
	return true;
}

bool SkeySvalueTable::saveToDbc(const String& filePath)
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
VipTable::loadFromDbc(const String& fileName)
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
VipTable::reloadFromDbc(const String& fileName)
{
	List<VipTemplate> tmpList = mVip;
// 	PlayerVIPTemplateMap tmpMap = mapVip;
	mVip.clear();
// 	mapVip.clear();
	if (!loadFromDbc(fileName))
	{
		mVip = tmpList;
// 		mapVip = tmpMap;
		LOG_WARN("Failed to reload award table.");
		return false;
	}
	return true;
}



bool 
VipTable::loadFromCsv(const String& filePath)
{
	VipTemplate vipTemplate;
	mVip.clear();
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
		if (!csvReader.bind("id", vipTemplate.id))
		{
			LOG_WARN("Failed to load vip.csv for [id]");
			return false;
		}
		if (!csvReader.bind("charge", vipTemplate.charge))
		{
			LOG_WARN("Failed to load vip.csv for [charge]");
			return false;
		}
		if (!csvReader.bind("stageCard1", vipTemplate.stageCard1))
		{
			LOG_WARN("Failed to load vip.csv for [stageCard1]");
			return false;
		}
		if (!csvReader.bind("stageCard2", vipTemplate.stageCard2))
		{
			LOG_WARN("Failed to load vip.csv for [stageCard2]");
			return false;
		}
		if (!csvReader.bind("stageCard3", vipTemplate.stageCard3))
		{
			LOG_WARN("Failed to load vip.csv for [stageCard3]");
			return false;
		}
		if (!csvReader.bind("stageCard4", vipTemplate.stageCard4))
		{
			LOG_WARN("Failed to load vip.csv for [stageCard4]");
			return false;
		}
		if (!csvReader.bind("strengthPurchase", vipTemplate.strengthPurchase))
		{
			LOG_WARN("Failed to load vip.csv for [strengthPurchase]");
			return false;
		}
		if (!csvReader.bind("strengthPurchase", vipTemplate.strengthPurchase))
		{
			LOG_WARN("Failed to load vip.csv for [strengthPurchase]");
			return false;
		}
		if (!csvReader.bind("stageReset", vipTemplate.stageReset))
		{
			LOG_WARN("Failed to load vip.csv for [stageReset]");
			return false;
		}
		if (!csvReader.bind("stageCleanOut10", vipTemplate.stageCleanOut10))
		{
			LOG_WARN("Failed to load vip.csv for [stageCleanOut10]");
			return false;
		}
	
		
		if (!csvReader.bind("friend", vipTemplate.friends))
		{
			LOG_WARN("Failed to load vip.csv for [friend]");
			return false;
		}
		if (!csvReader.bind("love", vipTemplate.love))
		{
			LOG_WARN("Failed to load vip.csv for [love]");
			return false;
		}
		if (!csvReader.bind("foodBuy", vipTemplate.foodBuy))
		{
			LOG_WARN("Failed to load vip.csv for [foodBuy]");
			return false;
		}
		if (!csvReader.bind("welfaremarket", vipTemplate.welfaremarket))
		{
			LOG_WARN("Failed to load vip.csv for [welfaremarket]");
			return false;
		}		
		if (!csvReader.bind("trailtimes", vipTemplate.trailtimes))
		{
			LOG_WARN("Failed to load vip.csv for [trailtimes]");
			return false;
		}
		if (!csvReader.bind("trailopenrate", vipTemplate.trailopenrate))
		{
			LOG_WARN("Failed to load vip.csv for [trailopenrate]");
			return false;
		}
		if (!csvReader.bind("climtowertimes", vipTemplate.climtowertimes))
		{
			LOG_WARN("Failed to load vip.csv for [climtowertimes]");
			return false;
		}

		if (!csvReader.bind("twelvePalaceBuy", vipTemplate.twelvePalaceBuy))
		{
			LOG_WARN("Failed to load vip.csv for [twelvePalaceBuy]");
			return false;
		}
		if (!csvReader.bind("territorybuytimes", vipTemplate.territorybuytimes))
		{
			LOG_WARN("Failed to load vip.csv for [territorybuytimes]");
			return false;
		}

		mVip.insertTail(vipTemplate);
// 		mapVip.insert(vipTemplate.id,vipTemplate);
	}
	fileStream.close();
	return true;
}

bool VipTable::saveToDbc(const String& filePath)
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
WelfareMarketTable::loadFromDbc(const String& fileName)
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
WelfareMarketTable::reloadFromDbc(const String& fileName)
{
	List<WelfareMarketTemplate> tmpList = mWelfareMarket;
	mWelfareMarket.clear();
	if (!loadFromDbc(fileName))
	{
		mWelfareMarket = tmpList;
		LOG_WARN("Failed to reload WelfareMarketTable table.");
		return false;
	}
	return true;
}



bool 
WelfareMarketTable::loadFromCsv(const String& filePath)
{
	WelfareMarketTemplate WelfareMarketTemplate;
	mWelfareMarket.clear();
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
		if (!csvReader.bind("id", WelfareMarketTemplate.id))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [id]");
			return false;
		}
		if (!csvReader.bind("dailyBaseTimes", WelfareMarketTemplate.dailyBaseTimes))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [dailyBaseTimes]");
			return false;
		}
		if (!csvReader.bind("sweepCostRmbEachTimes", WelfareMarketTemplate.sweepCostRmbEachTimes))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [sweepCostRmbEachTimes]");
			return false;
		}
		if (!csvReader.bindFloat("awardChangeDifficulty1", WelfareMarketTemplate.difficulty1.awardChangeDifficulty))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [awardChangeDifficulty1]");
			return false;
		}		
		if (!csvReader.bindFloat("awardChangeDifficulty2", WelfareMarketTemplate.difficulty2.awardChangeDifficulty))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [awardChangeDifficulty2]");
			return false;
		}
		if (!csvReader.bindFloat("awardChangeDifficulty3", WelfareMarketTemplate.difficulty3.awardChangeDifficulty))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [awardChangeDifficulty3]");
			return false;
		}
		if (!csvReader.bindFloat("awardChangeDifficulty4", WelfareMarketTemplate.difficulty4.awardChangeDifficulty))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [awardChangeDifficulty4]");
			return false;
		}
		if (!csvReader.bindFloat("awardChangeDifficulty5", WelfareMarketTemplate.difficulty5.awardChangeDifficulty))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [awardChangeDifficulty5]");
			return false;
		}
		if (!csvReader.bindFloat("awardChangeDifficulty6", WelfareMarketTemplate.difficulty6.awardChangeDifficulty))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [awardChangeDifficulty6]");
			return false;
		}
		if (!csvReader.bind("levelRequireDifficulty1", WelfareMarketTemplate.difficulty1.levelRequireDifficulty))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [levelRequireDifficulty1]");
			return false;
		}		
		if (!csvReader.bind("levelRequireDifficulty2", WelfareMarketTemplate.difficulty2.levelRequireDifficulty))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [levelRequireDifficulty2]");
			return false;
		}
		if (!csvReader.bind("levelRequireDifficulty3", WelfareMarketTemplate.difficulty3.levelRequireDifficulty))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [levelRequireDifficulty3]");
			return false;
		}
		if (!csvReader.bind("levelRequireDifficulty4", WelfareMarketTemplate.difficulty4.levelRequireDifficulty))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [levelRequireDifficulty4]");
			return false;
		}
		if (!csvReader.bind("levelRequireDifficulty5", WelfareMarketTemplate.difficulty5.levelRequireDifficulty))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [levelRequireDifficulty5]");
			return false;
		}
		if (!csvReader.bind("levelRequireDifficulty6", WelfareMarketTemplate.difficulty6.levelRequireDifficulty))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [levelRequireDifficulty6]");
			return false;
		}
		if (!csvReader.bind("awardDifficulty1", WelfareMarketTemplate.difficulty1.awardDifficulty))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [awardDifficulty1]");
			return false;
		}		
		if (!csvReader.bind("awardDifficulty2", WelfareMarketTemplate.difficulty2.awardDifficulty))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [awardDifficulty2]");
			return false;
		}
		if (!csvReader.bind("awardDifficulty3", WelfareMarketTemplate.difficulty3.awardDifficulty))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [awardDifficulty3]");
			return false;
		}
		if (!csvReader.bind("awardDifficulty4", WelfareMarketTemplate.difficulty4.awardDifficulty))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [awardDifficulty4]");
			return false;
		}
		if (!csvReader.bind("awardDifficulty5", WelfareMarketTemplate.difficulty5.awardDifficulty))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [awardDifficulty5]");
			return false;
		}
		if (!csvReader.bind("awardDifficulty6", WelfareMarketTemplate.difficulty6.awardDifficulty))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [awardDifficulty6]");
			return false;
		}
		if (!csvReader.bind("Monday", WelfareMarketTemplate.Monday))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [Monday]");
			return false;
		}
		if (!csvReader.bind("Tuesday", WelfareMarketTemplate.Tuesday))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [Tuesday]");
			return false;
		}
		if (!csvReader.bind("Wednesday", WelfareMarketTemplate.Wednesday))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [Wednesday]");
			return false;
		}
		if (!csvReader.bind("Thursday", WelfareMarketTemplate.Thursday))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [Thursday]");
			return false;
		}
		if (!csvReader.bind("Friday", WelfareMarketTemplate.Friday))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [Friday]");
			return false;
		}

		if (!csvReader.bind("Saturday", WelfareMarketTemplate.Saturday))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [Saturday]");
			return false;
		}
		if (!csvReader.bind("Sunday", WelfareMarketTemplate.Sunday))
		{
			LOG_WARN("Failed to load WelfareMarket.csv for [Sunday]");
			return false;
		}

		mWelfareMarket.insertTail(WelfareMarketTemplate);
	}
	fileStream.close();
	return true;
}

bool WelfareMarketTable::saveToDbc(const String& filePath)
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
