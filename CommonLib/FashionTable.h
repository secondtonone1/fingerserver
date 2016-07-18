#ifndef __LYNX_COMMON_LIB_FASHION_TABLE_H__
#define __LYNX_COMMON_LIB_FASHION_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct FahionTemplate 
	{
		UInt64 mId;

		String mName;

		String mDesc;

		UInt32 mBelong;

		String mIcon;

		UInt32 mQuality;

		UInt32 mResID;

		UInt32 mLimitVip;

		UInt32 mLimitRMB;

		

		UInt32 mAttrPA;

		UInt32 mAttrPF;

		UInt32 mAttrMA;

		UInt32 mAttrMF;

		UInt32 mAttrMaxHP;

		UInt32 mAttrMaxMP;

		UInt32 mAttrMaxSP;

		UInt32 mAttrCrit;
		

		LYNX_S11N_17(FahionTemplate, UInt64, mId,String, mName,String, mDesc,
			UInt32 ,mBelong,String, mIcon,UInt32, mQuality,
			UInt32, mResID,UInt32, mLimitVip,UInt32, mLimitRMB,
			UInt32, mAttrPA,UInt32, mAttrPF,UInt32 ,mAttrMA,
			UInt32, mAttrMF,UInt32, mAttrMaxHP,
			UInt32 ,mAttrMaxMP,UInt32, mAttrMaxSP,UInt32,mAttrCrit
			);

	};

	typedef Map<UInt64, FahionTemplate> FahionTemplateMap;


	struct FashionTable
	{
		FahionTemplateMap mMap;


		LYNX_S11N_1(FashionTable, FahionTemplateMap, mMap);

		FahionTemplate* get(UInt32 id)
		{
			FahionTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_EQUIP_TABLE_H__