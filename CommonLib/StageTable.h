#ifndef __LYNX_COMMON_LIB_STAGE_TABLE_H__
#define __LYNX_COMMON_LIB_STAGE_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct StageTemplate 
	{
		UInt64 mStageId;

		UInt32 mChapterId;

		String mName;

		String mDesc;

		UInt32 mType;

		UInt32 mSceneID;

		String mSceneConfig;

		String mStrFinShowList;

		String mPoint;

		UInt32 mCapacity; 

		UInt32 mStageStepCount;

		UInt32 mPerBloodCount;

		UInt32 mStrengthCost;

		UInt32 mChallengetimesLimit;

		String mStartLimit1;

		String mStartLimit2;

		String mStartLimit3;

		String mStartLimit4;

		String mStartLimit5;

		String mIcon;

		String mBossShow;

		String mStarRequire1;

		String mStarRequire2;

		String mStarRequire3;

		String mStarRequire4;

		String mStarRequire5;

		UInt32 mAwardGold;

		UInt32 mAwardExp;

		String mAwardOther;

		String mAwardID;

		String mFirstAwardID;

		UInt32 awardCardID;

		String monsterAward;

		UInt32 threat;

		UInt32 limittime1;

		UInt32 limittime2;

		UInt32 mopuptype;

		UInt32 awardaddtype;

		UInt32 isBoss;

		String dialogue;


		LYNX_S11N_40(StageTemplate, UInt64, mStageId,UInt32, mChapterId,String, mName,
			String, mDesc,UInt32, mType,UInt32, mSceneID,
			String, mSceneConfig,String, mStrFinShowList,String, mPoint,
			UInt32, mCapacity,UInt32, mStageStepCount,UInt32, mPerBloodCount,
			UInt32, mStrengthCost,UInt32, mChallengetimesLimit,String, mStartLimit1,
			String, mStartLimit2,String, mStartLimit3,String, mStartLimit4,
			String, mStartLimit5,String, mIcon,String, mBossShow,
			String, mStarRequire1,String, mStarRequire2,String, mStarRequire3,String, mStarRequire4,String, mStarRequire5,
			UInt32, mAwardGold,UInt32, mAwardExp,String, mAwardOther,
			String, mAwardID,String, mFirstAwardID, UInt32 , awardCardID,String, monsterAward,
			UInt32, threat,	UInt32, limittime1,	UInt32, limittime2,UInt32, mopuptype,UInt32, awardaddtype,
			UInt32, isBoss,String, dialogue
			);

	};

	typedef Map<UInt64, StageTemplate> StageTemplateMap;


	struct StageTable
	{
		StageTemplateMap mMap;

		LYNX_S11N_1(StageTable, StageTemplateMap, mMap);

		StageTemplate* get(UInt32 id)
		{
			StageTemplateMap::Iter* iter = mMap.find(id);

			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		void clear()
		{
			mMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_STAGE_TABLE_H__