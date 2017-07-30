#ifndef __LYNX_COMMON_LIB_HOARSTONEBASE_TABLE_H__
#define __LYNX_COMMON_LIB_HOARSTONEBASE_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct HoarstoneBaseTemplate 
	{
		// 界石id
		UInt64 mId;

		// 界石名称
		String	   mName;

		//level
		UInt32	   mMaxLv;
		//图标
		String	   mMainIcon;

		//碎片图标
		String     mPieceIcon;

		UInt32     mActiveConditionType1;

		UInt64     mActiveCondition1Param1;

		UInt64     mActiveCondition1Param2;

		
		String     mActiveConditionDesc1;

		UInt32     mActiveConditionType2;

		UInt64     mActiveCondition2Param1;

		UInt64     mActiveCondition2Param2;

		String     mActiveConditionDesc2;

		UInt32     mActiveConditionType3;

		UInt64     mActiveCondition3Param1;

		UInt64     mActiveCondition3Param2;

		String     mActiveConditionDesc3;

		UInt32     mActiveConditionType4;

		UInt64     mActiveCondition4Param1;

		UInt64     mActiveCondition4Param2;

		String     mActiveConditionDesc4;

		UInt32     mActiveConditionType5;

		UInt64     mActiveCondition5Param1;

		UInt64     mActiveCondition5Param2;

		String     mActiveConditionDesc5;

		UInt32		mExchange;

		LYNX_S11N_26(HoarstoneBaseTemplate,UInt64, mId, String,mName,UInt32,mMaxLv,String,mMainIcon,
			String,mPieceIcon,UInt32, mActiveConditionType1, UInt64 ,mActiveCondition1Param1, UInt64 ,mActiveCondition1Param1,
			String,mActiveConditionDesc1,
			UInt32 ,mActiveConditionType2,UInt64 ,mActiveCondition2Param1, UInt64 ,mActiveCondition2Param2,
			String,mActiveConditionDesc2,
			UInt32 ,mActiveConditionType3,UInt64 ,mActiveCondition3Param1, UInt64 ,mActiveCondition3Param2,
			String,mActiveConditionDesc3,
			UInt32 ,mActiveConditionType4,UInt64 ,mActiveCondition4Param1, UInt64 ,mActiveCondition4Param2,
			String,mActiveConditionDesc4,
			UInt32 ,mActiveConditionType5,UInt64 ,mActiveCondition4Param1, UInt64 ,mActiveCondition4Param2,
			String,mActiveConditionDesc5,  UInt32, mExchange
			);

	};

	typedef Map<UInt64, HoarstoneBaseTemplate> HoarstoneBaseTemplateMap;


	struct HoarstoneBaseTable
	{
		HoarstoneBaseTemplateMap mMap;


		LYNX_S11N_1(HoarstoneBaseTable, HoarstoneBaseTemplateMap, mMap);

		HoarstoneBaseTemplate* get(UInt64 id)
		{
			HoarstoneBaseTemplateMap::Iter* iter = mMap.find(id);
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

#endif // __LYNX_COMMON_LIB_HOARSTONEBASE_TABLE_H__


