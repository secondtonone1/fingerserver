#include "ConsortWoodTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
ConsortWoodTable::loadFromDbc(const String& fileName)
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
ConsortWoodTable::reloadFromDbc(const String& fileName)
{
	ConsortWoodTempMap tmpMap = mMap;
	
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
	  
		LOG_WARN("Failed to reload Consortwood table.");
		return false;
	}
	return true;
}

bool 
ConsortWoodTable::saveToDbc(const String& filePath)
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
ConsortWoodTable::loadFromCsv(const String& filePath)
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
		ConsortWoodTemplate consortWoodTemp;

		// ¹«ÓÃ
		if (!csvReader.bind("levelID", consortWoodTemp.mLv))
		{
			LOG_WARN("Failed to load woodencat.csv for [levelID]");
			return false;
		}

		if (!csvReader.bind("targethp", consortWoodTemp.mHp))
		{
			LOG_WARN("Failed to load woodencat.csv for [targethp]");
			return false;
		}
	
		if (!csvReader.bind("challengetimes", consortWoodTemp.mTimes))
		{
			LOG_WARN("Failed to load woodencat.csv for [challengetimes]");
			return false;
		}

		if (!csvReader.bind("targetaward", consortWoodTemp.mTargetAward))
		{
			LOG_WARN("Failed to load woodencat.csv for [mTargetAward]");
			return false;
		}

		if (!csvReader.bind("limit1", consortWoodTemp.mLimit1))
		{
			LOG_WARN("Failed to load woodencat.csv for [limit1]");
			return false;
		}

		if (!csvReader.bind("limit2", consortWoodTemp.mLimit2))
		{
			LOG_WARN("Failed to load woodencat.csv for [limit2]");
			return false;
		}

		if (!csvReader.bind("limit3", consortWoodTemp.mLimit3))
		{
			LOG_WARN("Failed to load woodencat.csv for [limit3]");
			return false;
		}

		if (!csvReader.bind("limit4", consortWoodTemp.mLimit4))
		{
			LOG_WARN("Failed to load woodencat.csv for [limit4]");
			return false;
		}

		if (!csvReader.bind("limit5", consortWoodTemp.mLimit5))
		{
			LOG_WARN("Failed to load woodencat.csv for [limit5]");
			return false;
		}

		if (!csvReader.bind("subtract1", consortWoodTemp.mDelHp1))
		{
			LOG_WARN("Failed to load woodencat.csv for [subtract1]");
			return false;
		}

		if (!csvReader.bind("subtract2", consortWoodTemp.mDelHp2))
		{
			LOG_WARN("Failed to load woodencat.csv for [subtract2]");
			return false;
		}

		if (!csvReader.bind("subtract3", consortWoodTemp.mDelHp3))
		{
			LOG_WARN("Failed to load woodencat.csv for [subtract3]");
			return false;
		}

		if (!csvReader.bind("subtract4", consortWoodTemp.mDelHp4))
		{
			LOG_WARN("Failed to load woodencat.csv for [subtract4]");
			return false;
		}

		if (!csvReader.bind("subtract5", consortWoodTemp.mDelHp5))
		{
			LOG_WARN("Failed to load woodencat.csv for [subtract5]");
			return false;
		}

		if (!csvReader.bind("winaward1", consortWoodTemp.mWinAward1))
		{
			LOG_WARN("Failed to load woodencat.csv for [winaward1]");
			return false;
		}

		if (!csvReader.bind("winaward2", consortWoodTemp.mWinAward2))
		{
			LOG_WARN("Failed to load woodencat.csv for [winaward2]");
			return false;
		}

		if (!csvReader.bind("winaward3", consortWoodTemp.mWinAward3))
		{
			LOG_WARN("Failed to load woodencat.csv for [winaward3]");
			return false;
		}

		if (!csvReader.bind("winaward4", consortWoodTemp.mWinAward4))
		{
			LOG_WARN("Failed to load woodencat.csv for [winaward4]");
			return false;
		}

		if (!csvReader.bind("winaward5", consortWoodTemp.mWinAward5))
		{
			LOG_WARN("Failed to load woodencat.csv for [winaward5]");
			return false;
		}

		if (!csvReader.bind("loseaward1", consortWoodTemp.mLoseAward1))
		{
			LOG_WARN("Failed to load woodencat.csv for [loseaward1]");
			return false;
		}

		if (!csvReader.bind("loseaward2", consortWoodTemp.mLoseAward2))
		{
			LOG_WARN("Failed to load woodencat.csv for [loseaward2]");
			return false;
		}

		if (!csvReader.bind("loseaward3", consortWoodTemp.mLoseAward3))
		{
			LOG_WARN("Failed to load woodencat.csv for [loseaward3]");
			return false;
		}

		if (!csvReader.bind("loseaward4", consortWoodTemp.mLoseAward4))
		{
			LOG_WARN("Failed to load woodencat.csv for [loseaward4]");
			return false;
		}

		if (!csvReader.bind("loseaward5", consortWoodTemp.mLoseAward5))
		{
			LOG_WARN("Failed to load woodencat.csv for [loseaward5]");
			return false;
		}


		mMap.insert(consortWoodTemp.mLv, consortWoodTemp);

	}

	fileStream.close();
	return true;
}