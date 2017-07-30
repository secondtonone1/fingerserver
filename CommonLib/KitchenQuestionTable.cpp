#include "KitchenQuestionTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
KitchenQuestionTable::loadFromDbc(const String& fileName)
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
KitchenQuestionTable::reloadFromDbc(const String& fileName)
{
	KitchenQuestionTemMap tmpMap = mMap;
	
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
	
		LOG_WARN("Failed to reload cattask table.");
		return false;
	}
	return true;
}

bool 
KitchenQuestionTable::saveToDbc(const String& filePath)
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
KitchenQuestionTable::loadFromCsv(const String& filePath)
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
		KitchenQuestionTemplate kitchenQuestionTemp;
        	std::stringstream mystream;

		// ¹«ÓÃ
		if (!csvReader.bind("id", kitchenQuestionTemp.mId))
		{
			LOG_WARN("Failed to load kitchenquestion.csv for [id]");
			return false;
		}
	
		if (!csvReader.bind("time", kitchenQuestionTemp.mTime))
		{
			LOG_WARN("Failed to load kitchenquestion.csv for [time]");
			return false;
		}
	
		if (!csvReader.bind("perfectAward", kitchenQuestionTemp.mAward1))
		{
			LOG_WARN("Failed to load kitchenquestion.csv for [perfectAward]");
			return false;
		}

		if (!csvReader.bind("excellent", kitchenQuestionTemp.mAward2))
		{
			LOG_WARN("Failed to load kitchenquestion.csv for [excellent]");
			return false;
		}

		
		if (!csvReader.bind("normalAward", kitchenQuestionTemp.mAward3))
		{
			LOG_WARN("Failed to load kitchenquestion.csv for [normalAward]");
			return false;
		}

		if (!csvReader.bind("badAward", kitchenQuestionTemp.mAward4))
		{
			LOG_WARN("Failed to load kitchenquestion.csv for [badAward]");
			return false;
		}

		if (!csvReader.bind("failAward", kitchenQuestionTemp.mAward5))
		{
			LOG_WARN("Failed to load kitchenquestion.csv for [failAward]");
			return false;
		}


		mMap.insert(kitchenQuestionTemp.mId, kitchenQuestionTemp);
	


	}

	fileStream.close();
	return true;
}