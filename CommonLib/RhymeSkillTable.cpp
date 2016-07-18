#include "RhymeSkillTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
RhymeSkillTable::loadFromDbc(const String& fileName)
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
RhymeSkillTable::reloadFromDbc(const String& fileName)
{
	RhymeSkillTemplateMap tmpMap = mMap;
	
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		
		LOG_WARN("Failed to reload rhymeSkill table.");
		return false;
	}
	return true;
}

bool 
RhymeSkillTable::saveToDbc(const String& filePath)
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
RhymeSkillTable::loadFromCsv(const String& filePath)
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
		RhymeSkillTemplate rhymeSkillTemplate;

		if (!csvReader.bind("id", rhymeSkillTemplate.mRhymeSkillIndex))
		{
			LOG_WARN("Failed to load rhymeSkill.csv for [id]");
			return false;
		}

		if (!csvReader.bind("activeNeedLevel", rhymeSkillTemplate.mActiveCondition))
		{
			LOG_WARN("Failed to load rhymeSkill.csv for [activeNeedLevel]");
			return false;
		}

		if (!csvReader.bind("name1", rhymeSkillTemplate.mName1))
		{
			LOG_WARN("Failed to load rhymeSkill.csv for [name1]");
			return false;
		}
	
		if (!csvReader.bind("desc1", rhymeSkillTemplate.mDesc1))
		{
			LOG_WARN("Failed to load rhymeSkill.csv for [desc1]");
			return false;
		}

		if (!csvReader.bind("icon1", rhymeSkillTemplate.mIcon1))
		{
			LOG_WARN("Failed to load rhymeSkill.csv for [icon1]");
			return false;
		}

		if (!csvReader.bind("skillId1", rhymeSkillTemplate.mSkillId1))
		{
			LOG_WARN("Failed to load rhymeSkill.csv for [skillId1]");
			return false;
		}

		if (!csvReader.bind("powerNeed1", rhymeSkillTemplate.mPowerNeed1))
		{
			LOG_WARN("Failed to load rhymeSkill.csv for [powerNeed1]");
			return false;
		}

		if (!csvReader.bind("effectId1", rhymeSkillTemplate.mEffectId1))
		{
			LOG_WARN("Failed to load rhymeSkill.csv for [effectId1]");
			return false;
		}


		if (!csvReader.bind("name2", rhymeSkillTemplate.mName2))
		{
			LOG_WARN("Failed to load rhymeSkill.csv for [name2]");
			return false;
		}

		if (!csvReader.bind("desc2", rhymeSkillTemplate.mDesc2))
		{
			LOG_WARN("Failed to load rhymeSkill.csv for [desc2]");
			return false;
		}

		if (!csvReader.bind("icon2", rhymeSkillTemplate.mIcon2))
		{
			LOG_WARN("Failed to load rhymeSkill.csv for [icon2]");
			return false;
		}

		if (!csvReader.bind("skillId2", rhymeSkillTemplate.mSkillId2))
		{
			LOG_WARN("Failed to load rhymeSkill.csv for [skillId2]");
			return false;
		}

		if (!csvReader.bind("powerNeed2", rhymeSkillTemplate.mPowerNeed2))
		{
			LOG_WARN("Failed to load rhymeSkill.csv for [powerNeed2]");
			return false;
		}

		if (!csvReader.bind("effectId2", rhymeSkillTemplate.mEffectId2))
		{
			LOG_WARN("Failed to load rhymeSkill.csv for [effectId2]");
			return false;
		}


		if (!csvReader.bind("name3", rhymeSkillTemplate.mName3))
		{
			LOG_WARN("Failed to load rhymeSkill.csv for [name3]");
			return false;
		}

		if (!csvReader.bind("desc3", rhymeSkillTemplate.mDesc3))
		{
			LOG_WARN("Failed to load rhymeSkill.csv for [desc3]");
			return false;
		}

		if (!csvReader.bind("icon3", rhymeSkillTemplate.mIcon3))
		{
			LOG_WARN("Failed to load rhymeSkill.csv for [icon3]");
			return false;
		}

		if (!csvReader.bind("skillId3", rhymeSkillTemplate.mSkillId3))
		{
			LOG_WARN("Failed to load rhymeSkill.csv for [skillId3]");
			return false;
		}

		if (!csvReader.bind("powerNeed3", rhymeSkillTemplate.mPowerNeed3))
		{
			LOG_WARN("Failed to load rhymeSkill.csv for [powerNeed3]");
			return false;
		}

		if (!csvReader.bind("effectId3", rhymeSkillTemplate.mEffectId3))
		{
			LOG_WARN("Failed to load rhymeSkill.csv for [effectId3]");
			return false;
		}

		std::string conditionStr = rhymeSkillTemplate.mActiveCondition.c_str();
		std::string ::size_type  findIndex = conditionStr.find(";");
		std::stringstream mystream;


		while(findIndex != std::string::npos)
		{
			//;号之前的全部截取，放入到列表里
			std::string conditionEleStr = conditionStr.substr(0,findIndex);

			mystream.clear();
			mystream.str("");

			mystream << conditionEleStr;

			UInt32 conditionEle = 0;
			mystream >> conditionEle;

			rhymeSkillTemplate.mConditionVec.push_back(conditionEle);

			conditionStr = conditionStr.substr(findIndex + 1);

			findIndex = conditionStr.find(';');

		}

		if(conditionStr != "")
		{
			mystream.clear();
			mystream.str("");

			mystream << conditionStr;

			UInt64 conditionEle = 0;
			mystream >> conditionEle;

			rhymeSkillTemplate.mConditionVec.push_back(conditionEle);
		}

	
	}
	fileStream.close();
	return true;
}

