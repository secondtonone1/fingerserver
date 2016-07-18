#ifndef __LYNX_COMMON_LIB_BUFFER_DATA_2_CLIENT_H__
#define __LYNX_COMMON_LIB_BUFFER_DATA_2_CLIENT_H__

#include "Definition.h"

namespace Lynx
{
	struct BufferData2Client
	{
		
		BufferData2Client() : m_nBufferID(0), m_nGetTime(0){}

		//buffID
		UInt64 m_nBufferID;
		//buff 获得时间戳
		UInt64 m_nGetTime;		
	
		std::string constructString(int num = 0)
		{
			std::stringstream mystream;
			mystream.clear();
			mystream.str("");

			mystream << m_nBufferID;
			std::string bufferIDStr;
			mystream >> bufferIDStr;

			mystream.clear();
			mystream.str("");

			mystream << m_nGetTime;
			std::string GetTimeStr;
			mystream >> GetTimeStr;

			m_strMsgSend = bufferIDStr + connectSymbols[num] + GetTimeStr  ;

			return m_strMsgSend;
		}

		std::string m_strMsgSend;
		LYNX_S11N_1(BufferData2Client, std::string , m_strMsgSend);


	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_BUFFER_DATA_2_CLIENT_H__
