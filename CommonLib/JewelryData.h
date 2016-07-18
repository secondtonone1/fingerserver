#ifndef __LYNX_COMMON_LIB_JEWELRY_DATA_H__
#define __LYNX_COMMON_LIB_JEWELRY_DATA_H__

#include "Definition.h"

namespace Lynx
{
	
	struct JewelryAttr
	{
		JewelryAttr():mKey(0),mValue(0),mRandomID(0){}
		UInt64 mRandomID;
		UInt32 mKey;
		UInt32 mValue;
		LYNX_S11N_3(JewelryAttr, UInt64, mRandomID,UInt32, mKey, UInt32, mValue);

	};
	
	struct JewelryData
	{

		JewelryData():m_nJewelryUid(0), m_nJewelryId(0),m_staticAttrKey(0),m_staticAttrValue(0),m_oldAttrIndex(0),m_purplePoints(0),
		m_purpleTimes(0),m_orangePoints(0),m_orangeTimes(0){}

		// Œ∆∑uid
		UInt64 m_nJewelryUid;

		// Œ∆∑id
		UInt64 m_nJewelryId;

		List<JewelryAttr> m_randomAttrList;

		UInt32 m_staticAttrKey;

		UInt32 m_staticAttrValue;

		UInt32 m_purplePoints;

		UInt32 m_purpleTimes;

		UInt32 m_orangePoints;

		UInt32 m_orangeTimes;

		JewelryAttr m_oldJewelryAttr;

		UInt32 m_oldAttrIndex;

		

		LYNX_S11N_11(JewelryData, UInt64, m_nJewelryUid, UInt64, m_nJewelryId, List<JewelryAttr>, m_randomAttrList, 
			UInt32, m_staticAttrKey, UInt32, m_staticAttrValue, UInt32, m_purplePoints, UInt32, m_purpleTimes, UInt32, m_orangePoints,
			UInt32, m_orangeTimes, JewelryAttr, m_oldJewelryAttr, UInt32, m_oldAttrIndex);


	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_JEWELRY_DATA_H__