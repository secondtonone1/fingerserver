#ifndef __LYNX_COMMON_LIB_CHARACTOR_DATA_H__
#define __LYNX_COMMON_LIB_CHARACTOR_DATA_H__

#include "Definition.h"

namespace Lynx
{
	struct CharactorData
	{


		CharactorData() :  m_nCharactorId(0),  m_nEquipFashion(0){}

		
		
		//CharactorID
		Guid m_nCharactorId;
		//´©´÷µÄÊ±×°
		Guid m_nEquipFashion;		


		LYNX_S11N_2(CharactorData,  Guid, m_nCharactorId, Guid ,m_nEquipFashion);


	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_CHARACTOR_DATA_H__