#include "ConsortLvTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
ConsortLvTable::loadFromDbc(const String& fileName)
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
ConsortLvTable::reloadFromDbc(const String& fileName)
{
	ConsortLvTemplateMap tmpMap = mMap;
	
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
	  
		LOG_WARN("Failed to reload ConsortLv table.");
		return false;
	}
	return true;
}

bool 
ConsortLvTable::saveToDbc(const String& filePath)
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
ConsortLvTable::loadFromCsv(const String& filePath)
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
		ConsortLvTemplate consortLvTemp;

		consortLvTemp.mActiveAward1 = 0;
		consortLvTemp.mActiveAward2 = 0;
		consortLvTemp.mActiveAward3 = 0;
		consortLvTemp.mActiveAward4 = 0;
	

		// ¹«ÓÃ
		if (!csvReader.bind("id", consortLvTemp.mId))
		{
			LOG_WARN("Failed to load guildlv.csv for [id]");
			return false;
		}

		if (!csvReader.bind("exp", consortLvTemp.mExp))
		{
			LOG_WARN("Failed to load guildlv.csv for [exp]");
			return false;
		}

		if (!csvReader.bind("num", consortLvTemp.mCount))
		{
			LOG_WARN("Failed to load guildlv.csv for [num]");
			return false;
		}

		if (!csvReader.bind("activity", consortLvTemp.mActive))
		{
			LOG_WARN("Failed to load guildlv.csv for [activity]");
			return false;
		}

		std::stringstream mystream;

		if (!csvReader.bind("activityaward1", consortLvTemp.mStrActiveAward1))
		{
			LOG_WARN("Failed to load guildlv.csv for [activityaward1]");
			return false;
		}
		
		if(!( consortLvTemp.mStrActiveAward1.empty()))
		{
				std::string activeaward1 = consortLvTemp.mStrActiveAward1.c_str();
				std::string ::size_type  findIndex = activeaward1.find(';');
				if(findIndex == std::string::npos)
				{
					return false;
				}

				std::string pointStr = activeaward1.substr(0,findIndex);
				std::string awardStr = activeaward1.substr(findIndex + 1);
				mystream.clear();
				mystream.str("");
				mystream << pointStr ;
				mystream >> consortLvTemp.mActivePoint1;
				
				mystream.clear();
				mystream.str("");
				mystream << awardStr;
				mystream>> consortLvTemp.mActiveAward1;

		}
	


		if (!csvReader.bind("activityaward2", consortLvTemp.mStrActiveAward2))
		{
			LOG_WARN("Failed to load guildlv.csv for [activityaward2]");
			return false;
		}

		if(! (consortLvTemp.mStrActiveAward2.empty() ))
		{
				std::string activeaward2 = consortLvTemp.mStrActiveAward2.c_str();
				std::string ::size_type  findIndex = activeaward2.find(';');
				if(findIndex == std::string::npos)
				{
					return false;
				}

				std::string pointStr = activeaward2.substr(0,findIndex);
				std::string awardStr = activeaward2.substr(findIndex+1);
				mystream.clear();
				mystream.str("");
				mystream << pointStr ;
				mystream >> consortLvTemp.mActivePoint2 ;
				mystream.clear();
				mystream.str("");
				mystream <<  awardStr;
				mystream>> consortLvTemp.mActiveAward2;

		}


		if (!csvReader.bind("activityaward3", consortLvTemp.mStrActiveAward3))
		{
			LOG_WARN("Failed to load guildlv.csv for [activityaward3]");
			return false;
		}

		if( ! (consortLvTemp.mStrActiveAward3.empty() ))
		{
				std::string activeaward3 = consortLvTemp.mStrActiveAward3.c_str();
				std::string ::size_type  findIndex = activeaward3.find(';');
				if(findIndex == std::string::npos)
				{
					return false;
				}

				std::string pointStr = activeaward3.substr(0,findIndex);
				std::string awardStr = activeaward3.substr(findIndex + 1);
				mystream.clear();
				mystream.str("");
				mystream << pointStr ;
				mystream >> consortLvTemp.mActivePoint3;
				mystream.clear();
				mystream.str("");
				mystream <<  awardStr;
				mystream >>consortLvTemp.mActiveAward3;
				

		}


		if (!csvReader.bind("activityaward4", consortLvTemp.mStrActiveAward4))
		{
			LOG_WARN("Failed to load guildlv.csv for [activityaward4]");
			return false;
		}

		if(! (consortLvTemp.mStrActiveAward4.empty() ))
		{
				std::string activeaward4 = consortLvTemp.mStrActiveAward4.c_str();
				std::string ::size_type  findIndex = activeaward4.find(';');
				if(findIndex == std::string::npos)
				{
					return false;
				}

				std::string pointStr = activeaward4.substr(0,findIndex);
				std::string awardStr = activeaward4.substr(findIndex + 1);
				mystream.clear();
				mystream.str("");
				mystream << pointStr ;
				mystream >> consortLvTemp.mActivePoint4;
				mystream.clear();
				mystream.str("");
				mystream <<  awardStr;
				mystream >> consortLvTemp.mActiveAward4;

		}

		if (!csvReader.bind("activityaward5", consortLvTemp.mStrActiveAward5))
		{
			LOG_WARN("Failed to load guildlv.csv for [activityaward5]");
			return false;
		}

		

		if (!csvReader.bind("cattasktimes", consortLvTemp.mXingManDo))
		{
			LOG_WARN("Failed to load guildlv.csv for [cattasktimes]");
			return false;
		}

		if (!csvReader.bind("cattaskstage", consortLvTemp.mXingXiaStage))
		{
			LOG_WARN("Failed to load guildlv.csv for [cattaskstage]");
			return false;
		}

		if (!csvReader.bind("kitchentimes", consortLvTemp.mKitchentimes))
		{
			LOG_WARN("Failed to load guildlv.csv for [kitchentimes]");
			return false;
		}

		if (!csvReader.bind("kitchenquestion", consortLvTemp.mStrKitchenQueStr))
		{
			LOG_WARN("Failed to load guildlv.csv for [kitchenquestion]");
			return false;
		}

		if(! (consortLvTemp.mStrKitchenQueStr.empty() ))
		{
				std::string questionStr = consortLvTemp.mStrKitchenQueStr.c_str();
				std::string ::size_type  findIndex = questionStr.find(',');
				while(findIndex != std::string::npos)
				{
					UInt32 question;
					std::string eleStr = questionStr.substr(0,findIndex);
					mystream.clear();
					mystream.str("");
					mystream << eleStr ;
					mystream >> question;
					consortLvTemp.kitchenQueList.insertTail(question);
					questionStr = questionStr.substr(findIndex + 1);
					 findIndex = questionStr.find(',');
				}
				
		}

		if (!csvReader.bind("businessid", consortLvTemp.mBusinessId))
		{
			LOG_WARN("Failed to load guildlv.csv for [businessid]");
			return false;
		}

		if (!csvReader.bind("businesstimes", consortLvTemp.mBusinessTimes))
		{
			LOG_WARN("Failed to load guildlv.csv for [businesstimes]");
			return false;
		}
	

		if (!csvReader.bind("memoryquestion", consortLvTemp.mStrEyeQuestr))
		{
			LOG_WARN("Failed to load guildlv.csv for [memoryquestion]");
			return false;
		}

		if(! (consortLvTemp.mStrEyeQuestr.empty() ))
		{
				std::string questionStr = consortLvTemp.mStrEyeQuestr.c_str();
				std::string ::size_type  findIndex = questionStr.find(',');
				while(findIndex != std::string::npos)
				{
					UInt32 question;
					std::string eleStr = questionStr.substr(0,findIndex);
					mystream.clear();
					mystream.str("");
					mystream << eleStr ;
					mystream >> question;
					consortLvTemp.mEyeQueList.insertTail(question);
					questionStr = questionStr.substr(findIndex + 1);
					 findIndex = questionStr.find(',');
				}
				
				UInt32 question;
				mystream.clear();
				mystream.str("");
				mystream << questionStr ;
				mystream >> question;
				consortLvTemp.mEyeQueList.insertTail(question);

		}

		if (!csvReader.bind("tickettimes", consortLvTemp.mTicketTimes))
		{
			LOG_WARN("Failed to load guildlv.csv for [tickettimes]");
			return false;
		}
		
		if (!csvReader.bind("friendlytimes", consortLvTemp.mfriendlyTimes))
		{
			LOG_WARN("Failed to load guildlv.csv for [friendlytimes]");
			return false;
		}

		if (!csvReader.bind("eloquencewin", consortLvTemp.mEloquencewin))
		{
			LOG_WARN("Failed to load guildlv.csv for [eloquencewin]");
			return false;
		}

		if (!csvReader.bind("eloquencelose", consortLvTemp.mEloquencelose))
		{
			LOG_WARN("Failed to load guildlv.csv for [eloquencelose]");
			return false;
		}
		
		if (!csvReader.bind("sighttimes", consortLvTemp.mEyeSightTimes))
		{
			LOG_WARN("Failed to load guildlv.csv for [sighttimes]");
			return false;
		}

		if (!csvReader.bind("eloquencetimes", consortLvTemp.mEloquenTimes))
		{
			LOG_WARN("Failed to load guildlv.csv for [eloquencetimes]");
			return false;
		}

		mMap.insert(consortLvTemp.mId, consortLvTemp);

	}

	fileStream.close();
	return true;
}