#ifndef __LYNX_COMMON_LIB_BUFFER_DATA_H__
#define __LYNX_COMMON_LIB_BUFFER_DATA_H__

#include "Definition.h"

namespace Lynx
{
	struct BufferData
    {
       

		BufferData() : m_nBufferID(0), m_nGetTime(0){}

		//buffID
		Guid m_nBufferID;
		//buff 获得的时间
		Guid m_nGetTime;		
		
			
		LYNX_S11N_2(BufferData, Guid, m_nBufferID, Guid, m_nGetTime);


    };
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_BUFFER_DATA_H__