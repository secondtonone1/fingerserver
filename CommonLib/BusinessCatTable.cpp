#include "BusinessCatTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
BusinessCatTable::loadFromDbc(const String& fileName)
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
BusinessCatTable::reloadFromDbc(const String& fileName)
{
	BusinessCatTemplateMap tmpMap = mMap;
	
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
	  
		LOG_WARN("Failed to reload businesscat table.");
		return false;
	}
	return true;
}

bool 
BusinessCatTable::saveToDbc(const String& filePath)
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
BusinessCatTable::loadFromCsv(const String& filePath)
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
		BusinessCatTemplate businessCatTemp;

		// ¹«ÓÃ
		if (!csvReader.bind("id", businessCatTemp.mId))
		{
			LOG_WARN("Failed to load businesscat.csv for [id]");
			return false;
		}

		if (!csvReader.bind("position1", businessCatTemp.mAwardStr1))
		{
			LOG_WARN("Failed to load businesscat.csv for [position1]");
			return false;
		}

		if (!csvReader.bind("position1price", businessCatTemp.mCostStr1))
		{
			LOG_WARN("Failed to load businesscat.csv for [position1price]");
			return false;
		}

		createList(&businessCatTemp, businessCatTemp.mAwardStr1.c_str(), businessCatTemp.mCostStr1.c_str() );

		if (!csvReader.bind("position2", businessCatTemp.mAwardStr2))
		{
			LOG_WARN("Failed to load businesscat.csv for [position2]");
			return false;
		}

		if (!csvReader.bind("position2price", businessCatTemp.mCostStr2))
		{
			LOG_WARN("Failed to load businesscat.csv for [position2price]");
			return false;
		}

		createList(&businessCatTemp, businessCatTemp.mAwardStr2.c_str(), businessCatTemp.mCostStr2.c_str() );

		if (!csvReader.bind("position3", businessCatTemp.mAwardStr3))
		{
			LOG_WARN("Failed to load businesscat.csv for [position3]");
			return false;
		}

		if (!csvReader.bind("position3price", businessCatTemp.mCostStr3))
		{
			LOG_WARN("Failed to load businesscat.csv for [position3price]");
			return false;
		}

		createList(&businessCatTemp, businessCatTemp.mAwardStr3.c_str(), businessCatTemp.mCostStr3.c_str() );

		if (!csvReader.bind("position4", businessCatTemp.mAwardStr4))
		{
			LOG_WARN("Failed to load businesscat.csv for [position4]");
			return false;
		}

		if (!csvReader.bind("position4price", businessCatTemp.mCostStr4))
		{
			LOG_WARN("Failed to load businesscat.csv for [position4price]");
			return false;
		}
	createList(&businessCatTemp, businessCatTemp.mAwardStr4.c_str(), businessCatTemp.mCostStr4.c_str() );

		if (!csvReader.bind("position5", businessCatTemp.mAwardStr5))
		{
			LOG_WARN("Failed to load businesscat.csv for [position5]");
			return false;
		}

		if (!csvReader.bind("position5price", businessCatTemp.mCostStr5))
		{
			LOG_WARN("Failed to load businesscat.csv for [position5price]");
			return false;
		}
	
		createList(&businessCatTemp, businessCatTemp.mAwardStr5.c_str(), businessCatTemp.mCostStr5.c_str() );

		if (!csvReader.bind("position6", businessCatTemp.mAwardStr6))
		{
			LOG_WARN("Failed to load businesscat.csv for [position6]");
			return false;
		}

		if (!csvReader.bind("position6price", businessCatTemp.mCostStr6))
		{
			LOG_WARN("Failed to load businesscat.csv for [position6price]");
			return false;
		}

		createList(&businessCatTemp, businessCatTemp.mAwardStr6.c_str(), businessCatTemp.mCostStr6.c_str() );

		if (!csvReader.bind("position7", businessCatTemp.mAwardStr7))
		{
			LOG_WARN("Failed to load businesscat.csv for [position7]");
			return false;
		}

		if (!csvReader.bind("position7price", businessCatTemp.mCostStr7))
		{
			LOG_WARN("Failed to load businesscat.csv for [position7price]");
			return false;
		}

		createList(&businessCatTemp, businessCatTemp.mAwardStr7.c_str(), businessCatTemp.mCostStr7.c_str() );

		if (!csvReader.bind("position8", businessCatTemp.mAwardStr8))
		{
			LOG_WARN("Failed to load businesscat.csv for [position8]");
			return false;
		}

		if (!csvReader.bind("position8price", businessCatTemp.mCostStr8))
		{
			LOG_WARN("Failed to load businesscat.csv for [position8price]");
			return false;
		}

		createList(&businessCatTemp, businessCatTemp.mAwardStr8.c_str(), businessCatTemp.mCostStr8.c_str() );

		if (!csvReader.bind("position9", businessCatTemp.mAwardStr9))
		{
			LOG_WARN("Failed to load businesscat.csv for [position9]");
			return false;
		}

		if (!csvReader.bind("position9price", businessCatTemp.mCostStr9))
		{
			LOG_WARN("Failed to load businesscat.csv for [position9price]");
			return false;
		}

		createList(&businessCatTemp, businessCatTemp.mAwardStr9.c_str(), businessCatTemp.mCostStr9.c_str() );

		if (!csvReader.bind("position10", businessCatTemp.mAwardStr10))
		{
			LOG_WARN("Failed to load businesscat.csv for [position10]");
			return false;
		}

		if (!csvReader.bind("position10price", businessCatTemp.mCostStr10))
		{
			LOG_WARN("Failed to load businesscat.csv for [position10price]");
			return false;
		}

		createList(&businessCatTemp, businessCatTemp.mAwardStr10.c_str(), businessCatTemp.mCostStr10.c_str() );

		if (!csvReader.bind("position11", businessCatTemp.mAwardStr11))
		{
			LOG_WARN("Failed to load businesscat.csv for [position11]");
			return false;
		}

		if (!csvReader.bind("position11price", businessCatTemp.mCostStr11))
		{
			LOG_WARN("Failed to load businesscat.csv for [position11price]");
			return false;
		}

		createList(&businessCatTemp, businessCatTemp.mAwardStr11.c_str(), businessCatTemp.mCostStr11.c_str() );

		if (!csvReader.bind("position12", businessCatTemp.mAwardStr12))
		{
			LOG_WARN("Failed to load businesscat.csv for [position12]");
			return false;
		}

		if (!csvReader.bind("position12price", businessCatTemp.mCostStr12))
		{
			LOG_WARN("Failed to load businesscat.csv for [position12price]");
			return false;
		}

		createList(&businessCatTemp, businessCatTemp.mAwardStr12.c_str(), businessCatTemp.mCostStr12.c_str() );

		if (!csvReader.bind("position13", businessCatTemp.mAwardStr13))
		{
			LOG_WARN("Failed to load businesscat.csv for [position13]");
			return false;
		}

		if (!csvReader.bind("position13price", businessCatTemp.mCostStr13))
		{
			LOG_WARN("Failed to load businesscat.csv for [position13price]");
			return false;
		}

		createList(&businessCatTemp, businessCatTemp.mAwardStr13.c_str(), businessCatTemp.mCostStr13.c_str() );

		if (!csvReader.bind("position14", businessCatTemp.mAwardStr14))
		{
			LOG_WARN("Failed to load businesscat.csv for [position14]");
			return false;
		}

		if (!csvReader.bind("position14price", businessCatTemp.mCostStr14))
		{
			LOG_WARN("Failed to load businesscat.csv for [position14price]");
			return false;
		}

		createList(&businessCatTemp, businessCatTemp.mAwardStr14.c_str(), businessCatTemp.mCostStr14.c_str() );

		if (!csvReader.bind("position15", businessCatTemp.mAwardStr15))
		{
			LOG_WARN("Failed to load businesscat.csv for [position15]");
			return false;
		}

		if (!csvReader.bind("position15price", businessCatTemp.mCostStr15))
		{
			LOG_WARN("Failed to load businesscat.csv for [position15price]");
			return false;
		}

		createList(&businessCatTemp, businessCatTemp.mAwardStr15.c_str(), businessCatTemp.mCostStr15.c_str() );

		if (!csvReader.bind("position16", businessCatTemp.mAwardStr16))
		{
			LOG_WARN("Failed to load businesscat.csv for [position16]");
			return false;
		}

		if (!csvReader.bind("position16price", businessCatTemp.mCostStr16))
		{
			LOG_WARN("Failed to load businesscat.csv for [position16price]");
			return false;
		}

		createList(&businessCatTemp, businessCatTemp.mAwardStr16.c_str(), businessCatTemp.mCostStr16.c_str() );

		mMap.insert(businessCatTemp.mId, businessCatTemp);

	}

	fileStream.close();
	return true;
}


void BusinessCatTable::createList(BusinessCatTemplate * pTemp, std::string awardStr,  std::string costStr)
{
	std::stringstream mystream;

	
	AllItemEle awardEle;
	std::string ::size_type  findIndex = awardStr.find(';');
	if(findIndex != std::string::npos)
	{
		std::string typeStr = awardStr.substr(0,findIndex);
		mystream.clear();
		mystream.str("");
		mystream << typeStr ;
		mystream >> awardEle.resType;
		awardStr = awardStr.substr(findIndex +1);
	}
	
	findIndex = awardStr.find(';');
	if(findIndex != std::string::npos)
	{
		std::string subtypeStr = awardStr.substr(0,findIndex);
		mystream.clear();
		mystream.str("");
		mystream << subtypeStr ;
		mystream >> awardEle.subType;
		awardStr = awardStr.substr(findIndex +1);

	}

	mystream.clear();
	mystream.str("");
	mystream << awardStr;
	mystream >> awardEle.count;

	pTemp->mAwardList.insertTail(awardEle);


	//////////////////////////////////////////////////////////

	AllItemEle costEle;
	  findIndex = costStr.find(';');
	if(findIndex != std::string::npos)
	{
		std::string typeStr = costStr.substr(0,findIndex);
		mystream.clear();
		mystream.str("");
		mystream << typeStr ;
		mystream >> costEle.resType;
		costStr = costStr.substr(findIndex +1);
	}
	
	findIndex = costStr.find(';');
	if(findIndex != std::string::npos)
	{
		std::string subtypeStr = costStr.substr(0,findIndex);
		mystream.clear();
		mystream.str("");
		mystream << subtypeStr ;
		mystream >> costEle.subType;
		costStr = costStr.substr(findIndex +1);

	}

	mystream.clear();
	mystream.str("");
	mystream << costStr;
	mystream >> costEle.count;

	pTemp->mCostList.insertTail(costEle);






}