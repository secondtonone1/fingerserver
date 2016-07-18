#include "RhymeLevelTable.h"
#include "Table.h"
#include "ResourceGroupManager.h"
#include "CsvReader.h"

using namespace Lynx;


bool 
RhymeLevelTable::loadFromDbc(const String& fileName)
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
RhymeLevelTable::reloadFromDbc(const String& fileName)
{
	RhymeLevelTemplateMap tmpMap = mMap;
	RhymeReverseFindMap reversTmpMap = mReverseFindMap;
	RhymeStepMaxMap rhymeStepMaxMap = mRhymeStepMaxMap;
	clear();
	if (!loadFromDbc(fileName))
	{
		mMap = tmpMap;
		mReverseFindMap = reversTmpMap;
		mRhymeStepMaxMap = rhymeStepMaxMap;
		LOG_WARN("Failed to reload rhymeLevel table.");
		return false;
	}
	return true;
}

bool 
RhymeLevelTable::saveToDbc(const String& filePath)
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
RhymeLevelTable::loadFromCsv(const String& filePath)
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
		RhymeLevelTemplate rhymeLevelTemplate;

		if (!csvReader.bind("id", rhymeLevelTemplate.mRhymeID))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [id]");
			return false;
		}
		
		if (!csvReader.bind("step", rhymeLevelTemplate.mStep))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [step]");
			return false;
		}

		if (!csvReader.bind("level", rhymeLevelTemplate.mLevel))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [level]");
			return false;
		}
		
		if (!csvReader.bind("nextNeedExp", rhymeLevelTemplate.mNextNeedExp))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [nextNeedExp]");
			return false;
		}

		if (!csvReader.bind("nextLevelCostCoin", rhymeLevelTemplate.mNextNeedCoin))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [nextLevelCostCoin]");
			return false;
		}

		if (!csvReader.bind("nextLevelSoul", rhymeLevelTemplate.mNextNeedSoul))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [nextLevelSoul]");
			return false;
		}

		if (!csvReader.bind("getExp", rhymeLevelTemplate.mGetExp))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [getExp]");
			return false;
		}

		if (!csvReader.bind("levelUpToStep", rhymeLevelTemplate.mLevelUpToStep))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [levelUpToStep]");
			return false;
		}

		if (!csvReader.bind("levelUpToLevel", rhymeLevelTemplate.mLevelUpToLevel))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [levelUpToLevel]");
			return false;
		}

		if (!csvReader.bind("acupoint", rhymeLevelTemplate.mAcupoint))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [acupoint]");
			return false;
		}

		if (!csvReader.bind("critRate", rhymeLevelTemplate.critRate))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [critRate]");
			return false;
		}

		if (!csvReader.bind("power", rhymeLevelTemplate.mPower))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [power]");
			return false;
		}

	
		std::string critRateStr = rhymeLevelTemplate.critRate.c_str();
		std::string ::size_type  findIndex = critRateStr.find(";");
		std::stringstream mystream;

		
		while(findIndex != std::string::npos)
		{
			//;号之前的全部截取，放入到列表里
			std::string critRateEleStr = critRateStr.substr(0,findIndex);

			mystream.clear();
			mystream.str("");

			mystream << critRateEleStr;

			UInt64 criteRateEle = 0;
			mystream >> criteRateEle;

			rhymeLevelTemplate.mRateEnhanceVec.push_back(criteRateEle);

			critRateStr = critRateStr.substr(findIndex + 1);

			findIndex = critRateStr.find(';');

		}

		if(critRateStr != "")
		{
			mystream.clear();
			mystream.str("");

			mystream << critRateStr;

			UInt64 criteRateEle = 0;
			mystream >> criteRateEle;

			rhymeLevelTemplate.mRateEnhanceVec.push_back(criteRateEle);
		}

		if (!csvReader.bind("activeSkill", rhymeLevelTemplate.mActiveSkillIndex))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [activeSkill]");
			return false;
		}

		if (!csvReader.bind("MaxHP", rhymeLevelTemplate.mMaxHp))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [MaxHP]");
			return false;
		}

		if (!csvReader.bind("MaxMP", rhymeLevelTemplate.mMaxMp))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [MaxMP]");
			return false;
		}

		if (!csvReader.bind("MaxSP", rhymeLevelTemplate.mMaxSp))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [MaxSP]");
			return false;
		}

		if (!csvReader.bind("PA", rhymeLevelTemplate.mPA))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [PA]");
			return false;
		}

		if (!csvReader.bind("MA", rhymeLevelTemplate.mMA))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [MA]");
			return false;
		}

		if (!csvReader.bind("PF", rhymeLevelTemplate.mPF))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [PF]");
			return false;
		}

		if (!csvReader.bind("MF", rhymeLevelTemplate.mMF))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [MF]");
			return false;
		}

		if (!csvReader.bind("HPRecover", rhymeLevelTemplate.mHPRecover))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [HPRecover]");
			return false;
		}

		if (!csvReader.bind("MPRecover", rhymeLevelTemplate.mMPRecover))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [MPRecover]");
			return false;
		}

		if (!csvReader.bind("SPRecover", rhymeLevelTemplate.mSPRecover))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [SPRecover]");
			return false;
		}

		if (!csvReader.bind("MaxHPAdd", rhymeLevelTemplate.mMaxHPAdd))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [MaxHPAdd]");
			return false;
		}

		if (!csvReader.bind("MaxMPAdd", rhymeLevelTemplate.mMaxMPAdd))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [MaxMPAdd]");
			return false;
		}

		if (!csvReader.bind("MaxSPAdd", rhymeLevelTemplate.mMaxSPAdd))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [MaxSPAdd]");
			return false;
		}

		if (!csvReader.bind("PAAdd", rhymeLevelTemplate.mPAAdd))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [PAAdd]");
			return false;
		}

		if (!csvReader.bind("MAAdd", rhymeLevelTemplate.mMAAdd))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [MAAdd]");
			return false;
		}

		if (!csvReader.bind("PFAdd", rhymeLevelTemplate.mPFAdd))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [PFAdd]");
			return false;
		}

		if (!csvReader.bind("MFAdd", rhymeLevelTemplate.mMFAdd))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [MFAdd]");
			return false;
		}
	
		if (!csvReader.bind("HPRecoverAdd", rhymeLevelTemplate.mHPRecoverAdd))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [HPRecoverAdd]");
			return false;
		}

		if (!csvReader.bind("MPRecoverAdd", rhymeLevelTemplate.mMPRecoverAdd))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [MPRecoverAdd]");
			return false;
		}

		if (!csvReader.bind("SPRecoverAdd", rhymeLevelTemplate.mSPRecoverAdd))
		{
			LOG_WARN("Failed to load rhymeLevel.csv for [SPRecoverAdd]");
			return false;
		}

		mMap.insert(rhymeLevelTemplate.mRhymeID, rhymeLevelTemplate);
		RhymeReverseFindMap::Iter * mapFindIter = mReverseFindMap.find(rhymeLevelTemplate.mStep);
		
		if(mapFindIter == NULL)
		{
			//没找到，则插入第一个
			Map<UInt32, UInt64> mapFind;
			mapFind.insert(rhymeLevelTemplate.mLevel, rhymeLevelTemplate.mRhymeID);
			mReverseFindMap.insert(rhymeLevelTemplate.mStep,mapFind);
			
		}
		else
		{
			Map<UInt32, UInt64>&  mapFind = mapFindIter->mValue;
			mapFind.insert(rhymeLevelTemplate.mLevel, rhymeLevelTemplate.mRhymeID);
		}

		RhymeStepMaxMap::Iter * mapStepFindIter = mRhymeStepMaxMap.find(rhymeLevelTemplate.mStep);
		if(mapStepFindIter == NULL)
		{
			mRhymeStepMaxMap.insert(rhymeLevelTemplate.mStep,rhymeLevelTemplate);
		}
		else
		{
			if(rhymeLevelTemplate.mLevel >  (mapStepFindIter->mValue.mLevel) )
			{
				mapStepFindIter->mValue = rhymeLevelTemplate;
			}
		}
	}
	fileStream.close();
	return true;
}

