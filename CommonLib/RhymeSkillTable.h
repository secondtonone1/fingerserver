#ifndef __LYNX_COMMON_LIB_RHYMESKILL_TABLE_H__
#define __LYNX_COMMON_LIB_RHYMESKILL_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct RhymeSkillTemplate 
	{

		UInt32 mRhymeSkillIndex;
		String mActiveCondition;
		
		String mName1;
		String mDesc1;
		String mIcon1;
		UInt32 mPowerNeed1;
		UInt64 mSkillId1;
		UInt32 mEffectId1;

		String mName2;
		String mDesc2;
		String mIcon2;
		UInt32 mPowerNeed2;
		UInt64 mSkillId2;
		UInt32 mEffectId2;

		String mName3;
		String mDesc3;
		String mIcon3;
		UInt32 mPowerNeed3;
		UInt64 mSkillId3;
		UInt32 mEffectId3;

		Vector<UInt32 > mConditionVec;

		LYNX_S11N_21(RhymeSkillTemplate, UInt32, mRhymeSkillIndex,String, mActiveCondition,
			String, mName1,String, mDesc1,String, mIcon1,UInt32, mPowerNeed1,UInt64, mSkillId1,UInt32, mEffectId1,
			String, mName2,String, mDesc2,String, mIcon2,UInt32, mPowerNeed2,UInt64, mSkillId2,UInt32, mEffectId2,
			String, mName3,String, mDesc3,String, mIcon3,UInt32, mPowerNeed3,UInt64, mSkillId3,UInt32, mEffectId3,
			Vector<UInt32 >, mConditionVec
			);

	};

	typedef Map<UInt32, RhymeSkillTemplate> RhymeSkillTemplateMap;
	
	struct RhymeSkillTable
	{
		RhymeSkillTemplateMap mMap;
		
		LYNX_S11N_1(RhymeSkillTable, RhymeSkillTemplateMap, mMap);

		RhymeSkillTemplate* get(UInt32 id)
		{
			RhymeSkillTemplateMap::Iter* iter = mMap.find(id);

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


#endif //__LYNX_COMMON_LIB_RHYMELEVEL_TABLE_H__