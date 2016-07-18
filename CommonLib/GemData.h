#ifndef __LYNX_COMMON_LIB_GEM_DATA_H__
#define __LYNX_COMMON_LIB_GEM_DATA_H__

#include "Definition.h"

namespace Lynx
{
	struct GemData
	{


		GemData() : m_nUid(0), m_nId(0),m_nCount(0),m_nDirtyType(NODIRTY){}

		//gemUid
		UInt64 m_nUid;

		//gemid ²ÄÁÏid
		UInt64 m_nId;		

		UInt32 m_nCount;

		UInt32 m_nDirtyType;

		LYNX_S11N_4(GemData, UInt64, m_nUid, UInt64, m_nId,UInt32, m_nCount,UInt32, m_nDirtyType);


	};
} // namespace Lynx

#endif //__LYNX_COMMON_LIB_GEM_DATA_H__
