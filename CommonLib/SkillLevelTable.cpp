#include "SkillLevelTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
SkillLevelTable::loadFromDbc(const String& fileName)
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
SkillLevelTable::reloadFromDbc(const String& fileName)
{
	SkillLevelTemplateMap tmpMap = mMap;
	SkillLevelReverseMap tmpReverseMap = mReverseMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		mReverseMap = tmpReverseMap;
		LOG_WARN("Failed to reload rhymeSkill table.");
		return false;
	}
	return true;
}

bool 
SkillLevelTable::saveToDbc(const String& filePath)
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
SkillLevelTable::loadFromCsv(const String& filePath)
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
		SkillLevelTemplate skillLevelTemplate;

		// 公用
		if (!csvReader.bind("id", skillLevelTemplate.mTitleID))
		{
			LOG_WARN("Failed to load SkillLevel.csv for [id]");
			return false;
		}

		if (!csvReader.bind("skill", skillLevelTemplate.mSkillID))
		{
			LOG_WARN("Failed to load SkillLevel.csv for [skill]");
			return false;
		}

		if(!csvReader.bind("level", skillLevelTemplate.mLevel))
		{
			LOG_WARN("Failed to load SkillLevel.csv for [level]");
			return false;
		}

		if(!csvReader.bind("cd", skillLevelTemplate.mCD1))
		{
			LOG_WARN("Failed to load SkillLevel.csv for [cd1]");
			return false;
		}
		
		if(!csvReader.bind("mp", skillLevelTemplate.mMP1))
		{
			LOG_WARN("Failed to load SkillLevel.csv for [mp1]");
			return false;
		}

		if(!csvReader.bind("attr", skillLevelTemplate.mAttr1))
		{
			LOG_WARN("Failed to load SkillLevel.csv for [attr1]");
			return false;
		}

		if(!csvReader.bind("state", skillLevelTemplate.mState1))
		{
			LOG_WARN("Failed to load SkillLevel.csv for [state1]");
			return false;
		}

		if(!csvReader.bind("buff", skillLevelTemplate.mBuff1))
		{
			LOG_WARN("Failed to load SkillLevel.csv for [buff1]");
			return false;
		}

		if(!csvReader.bind("learnLevel", skillLevelTemplate.mLearnLevel))
		{
			LOG_WARN("Failed to load SkillLevel.csv for [learnLevel]");
			return false;
		}

		if(!csvReader.bind("learnCoin", skillLevelTemplate.mLearnCoin))
		{
			LOG_WARN("Failed to load SkillLevel.csv for [learnCoin]");
			return false;
		}

		if(!csvReader.bind("power", skillLevelTemplate.mPower))
		{
			LOG_WARN("Failed to load SkillLevel.csv for [power]");
			return false;
		}

		if(!csvReader.bind("equip", skillLevelTemplate.mEquip))
		{
			LOG_WARN("Failed to load SkillLevel.csv for [equip]");
			return false;
		}
		

	mMap.insert(skillLevelTemplate.mTitleID, skillLevelTemplate);

	SkillLevelReverseMap::Iter * levelMapIter = mReverseMap.find(skillLevelTemplate.mSkillID);

	if(!levelMapIter)
	{
		//反向map里没有
		Map<UInt32 , SkillLevelTemplate> map;
		map.insert(skillLevelTemplate.mLevel, skillLevelTemplate);
		mReverseMap.insert(skillLevelTemplate.mSkillID,map);
	}
	else
	{
		levelMapIter->mValue.insert(skillLevelTemplate.mLevel, skillLevelTemplate);
	}

	
	}
	fileStream.close();
	return true;
}

