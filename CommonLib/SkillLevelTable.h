#ifndef __LYNX_COMMON_LIB_SKILLLEVEL_TABLE_H__
#define __LYNX_COMMON_LIB_SKILLLEVEL_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct SkillLevelTemplate 
	{
		UInt64 mTitleID;
		UInt64 mSkillID;
		UInt32 mLevel;
		UInt32 mCD1;
		UInt32 mMP1;
		String mAttr1;
		String mState1;
		String mBuff1;
		UInt32 mLearnLevel;
        UInt32 mLearnCoin;
		UInt32 mPower;
		UInt32 mEquip;
		LYNX_S11N_12(SkillLevelTemplate, UInt64, mTitleID,UInt64, mSkillID,UInt32, mLevel,
				UInt32, mCD1,UInt32, mMP1,String, mAttr1,
				String, mState1,String, mBuff1,UInt32, mLearnLevel,UInt32, mLearnCoin,UInt32, mPower, UInt32 , mEquip
			);

	};

	typedef Map<UInt64, SkillLevelTemplate> SkillLevelTemplateMap;
	typedef Map<UInt64, Map<UInt32 , SkillLevelTemplate> > SkillLevelReverseMap;

	struct SkillLevelTable
	{
		SkillLevelTemplateMap mMap;
		SkillLevelReverseMap mReverseMap;

		LYNX_S11N_2(SkillLevelTable, SkillLevelTemplateMap, mMap, SkillLevelReverseMap, mReverseMap);

		SkillLevelTemplate* get(UInt32 id)
		{
			SkillLevelTemplateMap::Iter* iter = mMap.find(id);

			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		SkillLevelTemplate * reverseGet(UInt64 skillId, UInt32 level)
		{
			SkillLevelReverseMap::Iter * reverseMapIter = mReverseMap.find(skillId);
			
			if(!reverseMapIter)
			{
				return NULL;
			}
			else
			{
				Map<UInt32 , SkillLevelTemplate>::Iter * skillLevelIter = reverseMapIter->mValue.find(level);
				
				if(skillLevelIter)
				{
					return &skillLevelIter->mValue;
				}
				else
				{
					return NULL;
				}
			}
		}

		SkillLevelTemplate * reverseGetNextLv(UInt64 skillId, UInt32 level)
		{
			SkillLevelReverseMap::Iter * reverseMapIter = mReverseMap.find(skillId);

			if(!reverseMapIter)
			{
				return NULL;
			}
			else
			{
				Map<UInt32 , SkillLevelTemplate>::Iter * skillLevelIter = reverseMapIter->mValue.find(level+1);

				if(skillLevelIter)
				{
					return &skillLevelIter->mValue;
				}
				else
				{
					return NULL;
				}
			}
		}

		void clear()
		{
			mMap.clear();
			mReverseMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};

// 	"5":{        
// 		"4":[
// 		{
// 			"type":1,
// 				"params":[
// 			{"x":100, "y":0},
// 			{"x":1000, "y":0}
// 				]
// 		},
// 		{
// 			"type":5,
// 				"params":[
// 			{"dType":1, "dValue":2},
// 			{"x":-50, "y":0, "w":230, "h":270},
// 			{"17":{"affectType":1, "affectValue":10}, "19":{"affectType":1, "affectValue":200},"21":{"affectType":1, "affectValue":0.1}},
// 			{},
// 			1,
// 			1
// 				]
// 		}
// 		],

} // namespace Lynx

#endif