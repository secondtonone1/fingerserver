#include "TowersTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;

bool 
TowerTypeTable::loadFromDbc(const String& fileName)
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
TowerTypeTable::reloadFromDbc(const String& fileName)
{
	TowerTypeTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload TowerTypeTable table.");
		return false;
	}
	return true;
}

bool 
TowerTypeTable::saveToDbc(const String& filePath)
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
TowerTypeTable::loadFromCsv(const String& filePath)
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
		TowerTypeTemplate towerTypeTemplate;
		// 公用
		if (!csvReader.bind("id", towerTypeTemplate.id))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [id]");
			return false;
		}
		if (!csvReader.bind("name", towerTypeTemplate.name))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [name]");
			return false;
		}
		if (!csvReader.bind("desc", towerTypeTemplate.desc))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [desc]");
			return false;
		}
		if (!csvReader.bind("towerheight", towerTypeTemplate.towerheight))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [towerheight]");
			return false;

		}

		if (!csvReader.bind("attackpower", towerTypeTemplate.attackpower))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [attackpower]");
			return false;
		}
		if (!csvReader.bind("hp", towerTypeTemplate.hp))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [hp]");
			return false;
		}
		if (!csvReader.bind("strength", towerTypeTemplate.strength))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [strength]");
			return false;
		}
		if (!csvReader.bind("startscore", towerTypeTemplate.startscore))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [startscore]");
			return false;

		}
		if (!csvReader.bind("mopupcost", towerTypeTemplate.mopupcost))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [mopupcost]");
			return false;
		}
		if (!csvReader.bind("operatetlimittime", towerTypeTemplate.operatetlimittime))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [operatetlimittime]");
			return false;
		}
		if (!csvReader.bind("strengthreduce", towerTypeTemplate.strengthreduce))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [strengthreduce]");
			return false;
		}
// 		if (!csvReader.bind("hprecover", towerTypeTemplate.hprecover))
// 		{
// 			LOG_WARN("Failed to load towerTypeTemplate.csv for [hprecover]");
// 			return false;
// 		}
		if (!csvReader.bind("roulettemaxtimes", towerTypeTemplate.roulettemaxtimes))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [roulettemaxtimes]");
			return false;
		}	

		if (!csvReader.bind("rouletteid", towerTypeTemplate.rouletteid))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [rouletteid]");
			return false;
		}
	
		if (!csvReader.bind("exchange", towerTypeTemplate.exchange))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [exchange]");
			return false;
		}
		if (!csvReader.bind("roulette", towerTypeTemplate.roulette))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [roulette]");
			return false;
		}
		if (!csvReader.bind("scorerate", towerTypeTemplate.scorerate))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [scorerate]");
			return false;
		}
// 		if (!csvReader.bind("awardshow", towerTypeTemplate.awardshow))
// 		{
// 			LOG_WARN("Failed to load towerTypeTemplate.csv for [awardshow]");
// 			return false;
// 		}
							
		if (!csvReader.bind("mopupaward1", towerTypeTemplate.mopupaward1))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [mopupaward1]");
			return false;
		}

		if (!csvReader.bind("mopupawardrate1", towerTypeTemplate.mopupawardrate1))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [mopupawardrate1]");
			return false;
		}


		if (!csvReader.bind("mopupaward2", towerTypeTemplate.mopupaward2))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [mopupaward2]");
			return false;
		}


		if (!csvReader.bind("mopupawardrate2", towerTypeTemplate.mopupawardrate2))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [mopupawardrate2]");
			return false;
		}

		if (!csvReader.bind("mopupaward3", towerTypeTemplate.mopupaward3))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [mopupaward3]");
			return false;
		}

		if (!csvReader.bind("mopupawardrate3", towerTypeTemplate.mopupawardrate3))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [mopupawardrate3]");
			return false;
		}

		if (!csvReader.bind("mopupaward4", towerTypeTemplate.mopupaward4))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [mopupaward4]");
			return false;
		}

		if (!csvReader.bind("mopupawardrate4", towerTypeTemplate.mopupawardrate4))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [mopupawardrate4]");
			return false;
		}
		if (!csvReader.bind("strengthrecover", towerTypeTemplate.strengthrecover))
		{
			LOG_WARN("Failed to load towerTypeTemplate.csv for [strengthrecover]");
			return false;
		}

		mMap.insert(towerTypeTemplate.id, towerTypeTemplate);
	}
	fileStream.close();
	return true;
}


bool 
TowerMonsterTable::loadFromDbc(const String& fileName)
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
TowerMonsterTable::reloadFromDbc(const String& fileName)
{
	TowerMonsterTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload TowerMonsterTable table.");
		return false;
	}
	return true;
}

bool 
TowerMonsterTable::saveToDbc(const String& filePath)
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
TowerMonsterTable::loadFromCsv(const String& filePath)
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
		TowerMonsterTemplate towerMonsterTemplate;
		// 公用
		if (!csvReader.bind("id", towerMonsterTemplate.id))
		{
			LOG_WARN("Failed to load towerMonsterTemplate.csv for [id]");
			return false;
		}
		if (!csvReader.bind("name", towerMonsterTemplate.name))
		{
			LOG_WARN("Failed to load towerMonsterTemplate.csv for [name]");
			return false;
		}
		if (!csvReader.bind("desc", towerMonsterTemplate.desc))
		{
			LOG_WARN("Failed to load towerMonsterTemplate.csv for [desc]");
			return false;
		}
		if (!csvReader.bind("res", towerMonsterTemplate.res))
		{
			LOG_WARN("Failed to load towerMonsterTemplate.csv for [res]");
			return false;

		}


		if (!csvReader.bind("attackpower", towerMonsterTemplate.attackpower))
		{
			LOG_WARN("Failed to load towerMonsterTemplate.csv for [attackpower]");
			return false;
		}
		if (!csvReader.bind("HP", towerMonsterTemplate.HP))
		{
			LOG_WARN("Failed to load towerMonsterTemplate.csv for [HP]");
			return false;
		}
		if (!csvReader.bind("briberycost", towerMonsterTemplate.briberycost))
		{
			LOG_WARN("Failed to load towerMonsterTemplate.csv for [briberycost]");
			return false;
		}
		if (!csvReader.bind("score", towerMonsterTemplate.score))
		{
			LOG_WARN("Failed to load towerMonsterTemplate.csv for [score]");
			return false;

		}


		mMap.insert(towerMonsterTemplate.id, towerMonsterTemplate);
	}
	fileStream.close();
	return true;
}


bool 
TowerEventTable::loadFromDbc(const String& fileName)
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
TowerEventTable::reloadFromDbc(const String& fileName)
{
	TowerEventTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload TowerEventTable table.");
		return false;
	}
	return true;
}

bool 
TowerEventTable::saveToDbc(const String& filePath)
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
TowerEventTable::loadFromCsv(const String& filePath)
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
		TowerEventTemplate towerEventTemplate;
		// 公用
		if (!csvReader.bind("id", towerEventTemplate.id))
		{
			LOG_WARN("Failed to load towerEventTemplate.csv for [id]");
			return false;
		}
		if (!csvReader.bind("type", towerEventTemplate.type))
		{
			LOG_WARN("Failed to load towerEventTemplate.csv for [type]");
			return false;
		}
		if (!csvReader.bind("param", towerEventTemplate.param))
		{
			LOG_WARN("Failed to load towerEventTemplate.csv for [param]");
			return false;
		}

		mMap.insert(towerEventTemplate.id, towerEventTemplate);
	}
	fileStream.close();
	return true;
}


bool 
TowerBuffTable::loadFromDbc(const String& fileName)
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
TowerBuffTable::reloadFromDbc(const String& fileName)
{
	TowerBuffTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload TowerBuffTable table.");
		return false;
	}
	return true;
}

bool 
TowerBuffTable::saveToDbc(const String& filePath)
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
TowerBuffTable::loadFromCsv(const String& filePath)
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
		TowerBuffTemplate towerBuffTemplate;
		// 公用
		if (!csvReader.bind("id", towerBuffTemplate.id))
		{
			LOG_WARN("Failed to load towerBuffTemplate.csv for [id]");
			return false;
		}
		if (!csvReader.bind("attr", towerBuffTemplate.attr))
		{
			LOG_WARN("Failed to load towerBuffTemplate.csv for [attr]");
			return false;
		}
		if (!csvReader.bind("num", towerBuffTemplate.num))
		{
			LOG_WARN("Failed to load towerBuffTemplate.csv for [num]");
			return false;
		}
		if (!csvReader.bind("keeptime", towerBuffTemplate.keeptime))
		{
// 			LOG_WARN("Failed to load towerBuffTemplate.csv for [keeptime]");
// 			return false;
		}
		if (!csvReader.bind("keepfloor", towerBuffTemplate.keepfloor))
		{
			LOG_WARN("Failed to load towerBuffTemplate.csv for [keepfloor]");
			return false;
		}
		mMap.insert(towerBuffTemplate.id, towerBuffTemplate);
	}
	fileStream.close();
	return true;
}

bool 
TowerAttrTable::loadFromDbc(const String& fileName)
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
TowerAttrTable::reloadFromDbc(const String& fileName)
{
	TowerAttrTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload TowerAttrTable table.");
		return false;
	}
	return true;
}

bool 
TowerAttrTable::saveToDbc(const String& filePath)
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
TowerAttrTable::loadFromCsv(const String& filePath)
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
		TowerAttrTemplate towerAttrTemplate;
		// 公用
		if (!csvReader.bind("id", towerAttrTemplate.id))
		{
			LOG_WARN("Failed to load towerAttrTemplate.csv for [id]");
			return false;
		}
		if (!csvReader.bind("attr", towerAttrTemplate.attr))
		{
			LOG_WARN("Failed to load towerAttrTemplate.csv for [attr]");
			return false;
		}
		//策划去掉直接读商店那个表的值
// 		if (!csvReader.bind("num", towerAttrTemplate.num))
// 		{
// 			LOG_WARN("Failed to load towerAttrTemplate.csv for [num]");
// 			return false;
// 		}

		mMap.insert(towerAttrTemplate.id, towerAttrTemplate);
	}
	fileStream.close();
	return true;
}



bool 
TowersTable::loadFromDbc(const String& fileName)
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
TowersTable::reloadFromDbc(const String& fileName)
{
	TowerTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload TowerTable table.");
		return false;
	}
	return true;
}

bool 
TowersTable::saveToDbc(const String& filePath)
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
TowersTable::loadFromCsv(const String& filePath)
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
		TowerTemplate towerTemplate;
		// 公用
		if (!csvReader.bind("stageId", towerTemplate.stageId))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [stageId]");
			return false;
		}
		if (!csvReader.bind("chapterId", towerTemplate.chapterId))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [chapterId]");
			return false;
		}
		if (!csvReader.bind("name", towerTemplate.name))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [name]");
			return false;
		}
		if (!csvReader.bind("desc", towerTemplate.desc))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [desc]");
			return false;
		}
		if (!csvReader.bind("finShowList", towerTemplate.finShowList))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [finShowList]");
			return false;

		}
		if (!csvReader.bind("point", towerTemplate.point))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [point]");
			return false;
		}
		if (!csvReader.bind("stageStepCount", towerTemplate.stageStepCount))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [stageStepCount]");
			return false;
		}
		if (!csvReader.bind("icon", towerTemplate.icon))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [icon]");
			return false;
		}
		if (!csvReader.bind("awardGold", towerTemplate.awardGold))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [awardGold]");
			return false;

		}
		if (!csvReader.bind("awardExp", towerTemplate.awardExp))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [awardExp]");
			return false;
		}
		if (!csvReader.bind("awardOther", towerTemplate.awardOther))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [awardOther]");
			return false;
		}
		if (!csvReader.bind("awardID", towerTemplate.awardID))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [awardID]");
			return false;
		}
		if (!csvReader.bind("firstAwardID", towerTemplate.firstAwardID))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [firstAwardID]");
			return false;
		}
		if (!csvReader.bind("mopuptype", towerTemplate.mopuptype))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [mopuptype]");
			return false;
		}		

		if (!csvReader.bind("event1", towerTemplate.event1))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [event1]");
			return false;
		}
		if (!csvReader.bind("eventrate1", towerTemplate.eventrate1))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [eventrate1]");
			return false;
		}
		if (!csvReader.bind("event2", towerTemplate.event2))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [event2]");
			return false;
		}
		if (!csvReader.bind("eventrate2", towerTemplate.eventrate2))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [eventrate2]");
			return false;
		}
		if (!csvReader.bind("event3", towerTemplate.event3))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [event3]");
			return false;
		}
		if (!csvReader.bind("eventrate3", towerTemplate.eventrate3))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [eventrate3]");
			return false;
		}
		if (!csvReader.bind("event4", towerTemplate.event4))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [event4]");
			return false;
		}

		if (!csvReader.bind("eventrate4", towerTemplate.eventrate4))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [eventrate4]");
			return false;
		}
		if (!csvReader.bind("event5", towerTemplate.event5))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [event5]");
			return false;
		}
		if (!csvReader.bind("eventrate5", towerTemplate.eventrate5))
		{
			LOG_WARN("Failed to load towerTemplate.csv for [eventrate5]");
			return false;
		}

		mMap.insert(towerTemplate.stageId, towerTemplate);
	}
	fileStream.close();
	return true;
}


bool 
TowerBoxTable::loadFromDbc(const String& fileName)
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
TowerBoxTable::reloadFromDbc(const String& fileName)
{
	TowerBoxTemplateMap tmpMap = mMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		LOG_WARN("Failed to reload TowerBoxTable table.");
		return false;
	}
	return true;
}

bool 
TowerBoxTable::saveToDbc(const String& filePath)
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
TowerBoxTable::loadFromCsv(const String& filePath)
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
			TowerBoxTemplate towerBoxTemplate;
			// 公用
			if (!csvReader.bind("id", towerBoxTemplate.id))
			{
				LOG_WARN("Failed to load towerBoxTemplate.csv for [id]");
				return false;
			}
			if (!csvReader.bind("awardcontent", towerBoxTemplate.awardcontent))
			{
				LOG_WARN("Failed to load towerBoxTemplate.csv for [awardcontent]");
				return false;
			}
			if (!csvReader.bind("attackpower", towerBoxTemplate.attackpower))
			{
				LOG_WARN("Failed to load towerBoxTemplate.csv for [attackpower]");
				return false;
			}

			if (!csvReader.bind("HP", towerBoxTemplate.HP))
			{
				LOG_WARN("Failed to load towerBoxTemplate.csv for [HP]");
				return false;
			}

			if (!csvReader.bind("scorecost", towerBoxTemplate.scorecost))
			{
				LOG_WARN("Failed to load towerBoxTemplate.csv for [scorecost]");
				return false;
			}

			mMap.insert(towerBoxTemplate.id, towerBoxTemplate);
		}
		fileStream.close();
		return true;
}

