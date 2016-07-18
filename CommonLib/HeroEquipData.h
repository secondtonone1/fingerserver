#ifndef __LYNX_COMMON_LIB_HEROEQUIP_DATA_H__
#define __LYNX_COMMON_LIB_HEROEQUIP_DATA_H__

#include "Definition.h"

namespace Lynx
{
	struct HeroEquipData
	{

		HeroEquipData() : m_nEquipId(0), m_nEquipLv(0),m_nActiveBit(AllGemLock),m_nStep(0){}

		//装备id
		UInt64 m_nEquipId;
			
		//装备的宝石槽位，没有或者无效的用0或-1（转化成无符号64位来处理）,装备宝石
		List<UInt64> m_nGemsList;
		
		//装备lv
		UInt32 m_nEquipLv;
		
		//32位数表示是否激活
		UInt32 m_nActiveBit;

		UInt32 m_nStep;

		LYNX_S11N_5(HeroEquipData, UInt64, m_nEquipId, UInt32, m_nEquipLv, List<UInt64>, m_nGemsList, UInt32, m_nActiveBit,UInt32, m_nStep);


	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_HEROEQUIP_DATA_H__