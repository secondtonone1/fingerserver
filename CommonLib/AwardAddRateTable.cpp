#include "AwardAddRateTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
AwardAddRateTable::loadFromDbc(const String& fileName)
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
AwardAddRateTable::reloadFromDbc(const String& fileName)
{
	AwardAddRateTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload AwardAddRateTable table.");
		return false;
	}
	return true;
}

bool 
AwardAddRateTable::saveToDbc(const String& filePath)
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
AwardAddRateTable::loadFromCsv(const String& filePath)
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
		

		AwardAddRateTemplate AwardAddRateTemplate;
		// ¹«ÓÃ
		if (!csvReader.bind("awardaddtype", AwardAddRateTemplate.awardaddtype))
		{
			LOG_WARN("Failed to load AwardAddRateTemplate.csv for [awardaddtype]");
			return false;
		}

		if (!csvReader.bind("needcondition", AwardAddRateTemplate.needcondition))
		{
			LOG_WARN("Failed to load AwardAddRateTemplate.csv for [needcondition]");
			return false;
		}

	
		

		char str1[32]={};
		char str2[32]={};
		char str3[32]={};
		for (int i=1;i<50;i++)
		{
			RateOfGood rateOfGood;
			sprintf(str1, "%s%d","resourcesType",i);
			sprintf(str2, "%s%d","subtype",i);
			sprintf(str3, "%s%d","rate",i);

			if (!csvReader.bind(str1, rateOfGood.resourcesType))
			{
				LOG_WARN("Failed to load Award.csv for [type]");
				break;
			}
			if (rateOfGood.resourcesType == 0)
			{
				break;
			}

			if (!csvReader.bind(str2, rateOfGood.subtype))
			{
				LOG_WARN("Failed to load Award.csv for [weight]");
				break;
			}
			if (!csvReader.bind(str3, rateOfGood.rate))
			{
				LOG_WARN("Failed to load Award.csv for [awardcontent]");
				break;
			}
			AwardAddRateTemplate.rateOfGoods.insertTail(rateOfGood);

		}	
		
		
		mMap.insert(AwardAddRateTemplate.awardaddtype, AwardAddRateTemplate);
	}
	fileStream.close();
	return true;
}

