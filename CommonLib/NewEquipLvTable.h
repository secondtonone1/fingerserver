#ifndef __LYNX_COMMON_LIB_NEWEQUIPLV_TABLE_H__
#define __LYNX_COMMON_LIB_NEWEQUIPLV_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct NewEquipLvTemplate 
	{
		UInt64 mId;
		UInt64 mEquipId;
		UInt32 mLv;

		String mName;
		String mIcon;
		UInt32 mQuality;

		UInt32 mLvUpNeedCoin;
		UInt32 mLvUpNeedResId1;
		UInt32 mLvUpNeedResCnt1;

		UInt32 mLvUpNeedResId2;
		UInt32 mLvUpNeedResCnt2;
		UInt32 mIsStepUp;

		UInt32 mAttrType;

		UInt32 mAttrValue;

		UInt32 mNextLevelAttrType;

		UInt32 mNextLevelAttrValue;

		UInt32 mEquipPos;

		LYNX_S11N_17(NewEquipLvTemplate, UInt64, mId,UInt64, mEquipId,UInt32,mLv, String, mName,String, mIcon,UInt32, mQuality,
			UInt32, mLvUpNeedCoin,UInt32, mLvUpNeedResId1,UInt32, mLvUpNeedResCnt1,UInt32, mLvUpNeedResId2,UInt32, mLvUpNeedResCnt2,
			UInt32, mIsStepUp, UInt32, mAttrType, UInt32, mAttrValue, UInt32, mNextLevelAttrType, UInt32, mNextLevelAttrValue, UInt32, mEquipPos);

	};

	typedef Map<UInt64, NewEquipLvTemplate> NewEquipLvTemplateMap;
	typedef Map<UInt64, Map<UInt32, NewEquipLvTemplate> > NewEquipLvReverseMap;
	

	struct NewEquipLvTable
	{
		NewEquipLvTemplateMap mMap;
		NewEquipLvReverseMap mReverseMap;

		LYNX_S11N_2(NewEquipLvTable, NewEquipLvTemplateMap, mMap, NewEquipLvReverseMap, mReverseMap);

		NewEquipLvTemplate* get(UInt64 id)
		{
			NewEquipLvTemplateMap::Iter* iter = mMap.find(id);

			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		NewEquipLvTemplate * reverseGet(UInt64 id, UInt32 level)
		{
			NewEquipLvReverseMap::Iter * mapLvEquipIter = mReverseMap.find(id);
			
			if(mapLvEquipIter)
			{
				Map<UInt32, NewEquipLvTemplate>::Iter * newEquipLvIter = mapLvEquipIter->mValue.find(level);
				
				if(newEquipLvIter)
				{
					return &newEquipLvIter->mValue;
				}
				else
				{
					return NULL;
				}
			}
			else
			{
				return NULL;
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
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_NEWEQUIPLV_TABLE_H__