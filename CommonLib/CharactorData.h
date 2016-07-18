#ifndef __LYNX_COMMON_LIB_CHARACTOR_DATA_H__
#define __LYNX_COMMON_LIB_CHARACTOR_DATA_H__

#include "Definition.h"

namespace Lynx
{
	struct CharactorData
	{


		CharactorData() : m_nCharactorUid(0), m_nCharactorId(0),  m_nEquipFashion(0){}

		//CharactorUID
		Guid m_nCharactorUid;
		//CharactorID
		Guid m_nCharactorId;
		//´©´÷µÄÊ±×°
		Guid m_nEquipFashion;		


		LYNX_S11N_3(CharactorData, Guid, m_nCharactorUid, Guid, m_nCharactorId, Guid ,m_nEquipFashion);


	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_CHARACTOR_DATA_H__