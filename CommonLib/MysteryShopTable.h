#ifndef __LYNX_COMMON_LIB_MYSTERY_SHOP_TABLE_H__
#define __LYNX_COMMON_LIB_MYSTERY_SHOP_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct Sell
	{
		UInt32 restype;
		UInt32 subtype;
		UInt32 num;
		String cost;
		LYNX_S11N_4(Sell, UInt32, restype,UInt32, subtype, UInt32, num,String, cost);
	};
	struct ShopsTemplate 
	{
		UInt32 id;

		UInt32 shoptype;
		
		List<Sell> sellList;
		
		LYNX_S11N_3(ShopsTemplate, UInt32, id,UInt32, shoptype,List<Sell> ,sellList);

	};

	typedef Map<UInt32, ShopsTemplate> ShopsTemplateMap;


	struct ShopsTable
	{
		ShopsTemplateMap mMap;


		LYNX_S11N_1(ShopsTable, ShopsTemplateMap, mMap);

		ShopsTemplate* get(UInt32 id)
		{
			ShopsTemplateMap::Iter* iter = mMap.find(id);

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

#endif // __LYNX_COMMON_LIB_MYSTERY_SHOP_TABLE_H__