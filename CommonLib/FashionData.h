#ifndef __LYNX_COMMON_LIB_FASHION_DATA_H__
#define __LYNX_COMMON_LIB_FASHION_DATA_H__

#include "Definition.h"

namespace Lynx
{
	struct FashionData
	{


		FashionData() : m_nFahionID(0){}

		//buffID
		Guid m_nFahionID;
		
		LYNX_S11N_1(FashionData, Guid, m_nFahionID);


	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_BUFFER_DATA_H__