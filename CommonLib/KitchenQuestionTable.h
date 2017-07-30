#ifndef  __LYNX_COMMON_LIB_KITCHENQUESTION_TABLE_H__
#define __LYNX_COMMON_LIB_KITCHENQUESTION_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct KitchenQuestionTemplate 
	{
		UInt32 mId;
		
		UInt32 mTime;
		

		UInt32 mAward1;
		UInt32 mAward2;
		UInt32 mAward3;
		UInt32 mAward4;
		UInt32 mAward5;
		
		LYNX_S11N_7(KitchenQuestionTemplate, UInt32, mId,  UInt32 ,mTime,
			UInt32, mAward1,UInt32, mAward2, UInt32, mAward3, UInt32, mAward4,
			UInt32, mAward5
		);

	};

	typedef Map<UInt32, KitchenQuestionTemplate> KitchenQuestionTemMap;


	struct KitchenQuestionTable
	{
		KitchenQuestionTemMap mMap;
		

		LYNX_S11N_1(KitchenQuestionTable, KitchenQuestionTemMap, mMap);

		KitchenQuestionTemplate* get(UInt32 id)
		{
			KitchenQuestionTemMap::Iter* iter = mMap.find(id);
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




#endif //__LYNX_COMMON_LIB_KITCHENQUESTION_TABLE_H__