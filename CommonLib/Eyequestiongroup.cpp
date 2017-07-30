#include "Eyequestiongroup.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
EyeQuestionTable::loadFromDbc(const String& fileName)
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
EyeQuestionTable::reloadFromDbc(const String& fileName)
{
	EyeQuestionTemMap tmpMap = mMap;
	
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
	
		LOG_WARN("Failed to reload eyequestion table.");
		return false;
	}
	return true;
}

bool 
EyeQuestionTable::saveToDbc(const String& filePath)
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
EyeQuestionTable::loadFromCsv(const String& filePath)
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
		EyeQuestionTemplate eyeQuestionTemp;
        	std::stringstream mystream;

		// ¹«ÓÃ
		if (!csvReader.bind("groupid", eyeQuestionTemp.mId))
		{
			LOG_WARN("Failed to load memoryquestiongroup.csv for [groupid]");
			return false;
		}
	
		if (!csvReader.bind("perfectAward", eyeQuestionTemp.mAward1))
		{
			LOG_WARN("Failed to load memoryquestiongroup.csv for [perfectAward]");
			return false;
		}

		if (!csvReader.bind("excellentAward", eyeQuestionTemp.mAward2))
		{
			LOG_WARN("Failed to load memoryquestiongroup.csv for [excellentAward]");
			return false;
		}

		
		if (!csvReader.bind("normalAward", eyeQuestionTemp.mAward3))
		{
			LOG_WARN("Failed to load memoryquestiongroup.csv for [normalAward]");
			return false;
		}

		if (!csvReader.bind("badAward", eyeQuestionTemp.mAward4))
		{
			LOG_WARN("Failed to load memoryquestiongroup.csv for [badAward]");
			return false;
		}

		if (!csvReader.bind("failAward", eyeQuestionTemp.mAward5))
		{
			LOG_WARN("Failed to load kitchenquestion.csv for [failAward]");
			return false;
		}


		mMap.insert(eyeQuestionTemp.mId, eyeQuestionTemp);
	


	}

	fileStream.close();
	return true;
}