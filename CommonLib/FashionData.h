#ifndef __LYNX_COMMON_LIB_FASHION_DATA_H__
#define __LYNX_COMMON_LIB_FASHION_DATA_H__

#include "Definition.h"

namespace Lynx
{
	struct FashionData
	{


		FashionData() : m_nFahionID(0), m_nFashionUid(0){}

		//buffID
		Guid m_nFahionID;
		Guid m_nFashionUid;
		LYNX_S11N_2(FashionData, Guid, m_nFahionID, Guid, m_nFashionUid);


	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_BUFFER_DATA_H__