#include "CatTaskTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
CatTaskTable::loadFromDbc(const String& fileName)
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
CatTaskTable::reloadFromDbc(const String& fileName)
{
	CatTaskTemplateMap tmpMap = mMap;
	LvTaskTemplateMap tmpMapLv = mLvMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		mLvMap	= tmpMapLv;
		LOG_WARN("Failed to reload cattask table.");
		return false;
	}
	return true;
}

bool 
CatTaskTable::saveToDbc(const String& filePath)
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
CatTaskTable::loadFromCsv(const String& filePath)
{
	mMap.clear();
	mLvMap.clear();
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
		CatTaskTemplate catTaskTemp;
        	std::stringstream mystream;

		// ¹«ÓÃ
		if (!csvReader.bind("id", catTaskTemp.mId))
		{
			LOG_WARN("Failed to load cattask.csv for [id]");
			return false;
		}

		if (!csvReader.bind("guildlevel", catTaskTemp.mGuildLv))
		{
			LOG_WARN("Failed to load cattask.csv for [guildlevel]");
			return false;
		}

		if (!csvReader.bind("time", catTaskTemp.mTime))
		{
			LOG_WARN("Failed to load cattask.csv for [time]");
			return false;
		}

		if (!csvReader.bind("rate", catTaskTemp.mRate))
		{
			LOG_WARN("Failed to load cattask.csv for [rate]");
			return false;
		}

		if (!csvReader.bind("award1", catTaskTemp.mAward1))
		{
			LOG_WARN("Failed to load cattask.csv for [award1]");
			return false;
		}

		if(! ( catTaskTemp.mAward1.empty() ))
		{
				std::string awardstr = catTaskTemp.mAward1.c_str();
				std::string ::size_type  findIndex = awardstr.find(';');
				if(findIndex == std::string::npos)
				{
					return false;
				}

				std::string typeStr = awardstr.substr(0,findIndex);
				awardstr = awardstr.substr(findIndex + 1);
				mystream.clear();
				mystream.str("");
				mystream << typeStr ;
				mystream >> catTaskTemp.mType1;

				findIndex = awardstr.find(';');
				if(findIndex == std::string::npos)
				{
					return false;
				}
				std::string subtypeStr = awardstr.substr(0,findIndex);
				awardstr = awardstr.substr(findIndex + 1);
				mystream.clear();
				mystream.str("");
				mystream <<  subtypeStr;
				mystream >> catTaskTemp.mSubType1;

				mystream.clear();
				mystream.str("");
				mystream <<  awardstr;
				mystream >> catTaskTemp.mCount1;

		}

		if (!csvReader.bind("award2", catTaskTemp.mAward2))
		{
			LOG_WARN("Failed to load cattask.csv for [award2]");
			return false;
		}

		if(! ( catTaskTemp.mAward2.empty() ))
		{
				std::string awardstr = catTaskTemp.mAward2.c_str();
				std::string ::size_type  findIndex = awardstr.find(';');
				if(findIndex == std::string::npos)
				{
					return false;
				}

				std::string typeStr = awardstr.substr(0,findIndex);
				awardstr = awardstr.substr(findIndex + 1);
				mystream.clear();
				mystream.str("");
				mystream << typeStr ;
				mystream >> catTaskTemp.mType2;

				findIndex = awardstr.find(';');
				if(findIndex == std::string::npos)
				{
					return false;
				}
				std::string subtypeStr = awardstr.substr(0,findIndex);
				awardstr = awardstr.substr(findIndex + 1);
				mystream.clear();
				mystream.str("");
				mystream <<  subtypeStr;
				mystream >> catTaskTemp.mSubType2;

				mystream.clear();
				mystream.str("");
				mystream <<  awardstr;
				mystream >> catTaskTemp.mCount2;

		}



		mMap.insert(catTaskTemp.mId, catTaskTemp);
		Map<UInt32, List<CatTaskTemplate > >::Iter * findIter = mLvMap.find(catTaskTemp.mGuildLv);
		if(findIter != NULL)
		{
			findIter->mValue.insertTail(catTaskTemp);
		}
		else
		{
			List<CatTaskTemplate> taskList;
			taskList.insertTail(catTaskTemp);
			mLvMap.insert(catTaskTemp.mGuildLv, taskList);

		}


	}

	fileStream.close();
	return true;
}