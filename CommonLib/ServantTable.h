#ifndef __LYNX_COMMON_LIB_SERVANT_TABLE_H__
#define __LYNX_COMMON_LIB_SERVANT_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct ServantTemplate 
	{
		UInt64 mId;
		UInt64 mMonsterid;
		UInt32 mQuality;
		UInt64 mSkillID1;
		UInt64 mSkillID2;
		//Íò·Ö±È£¬1200/10000
		UInt32 mRate;
		UInt32 mCurStar;
		UInt32 mCurFloor;
		UInt32 mMaxStar;
		UInt32 mMaxFloor;
		UInt32 mPieceType;
		UInt32 mPieceCount;
		String mLuckyStr;
		List<UInt64> luckyList;
		UInt32 mServantSwitch;

		LYNX_S11N_15(ServantTemplate, UInt64, mId,UInt64, mMonsterid,UInt32, mQuality, UInt64, mSkillID1,UInt64, mSkillID2,
			                         UInt32, mRate,UInt32, mCurStar, UInt32, mCurFloor, UInt32, mMaxStar, UInt32, mMaxFloor, 
									 UInt32, mPieceType, UInt32, mPieceCount,String, mLuckyStr, List<UInt64>, luckyList,UInt32, mServantSwitch);

	};

	typedef Map<UInt64, ServantTemplate> ServantTemplateMap;


	struct ServantTable
	{
		ServantTemplateMap mMap;

		LYNX_S11N_1(ServantTable, ServantTemplateMap, mMap);

		ServantTemplate* get(UInt64 id)
		{
			ServantTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_SERVANT_TABLE_H__