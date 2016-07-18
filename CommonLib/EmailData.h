#ifndef __LYNX_COMMON_LIB_EMAIL_DATA_H__
#define __LYNX_COMMON_LIB_EMAIL_DATA_H__

#include "Definition.h"

namespace Lynx
{
	struct EmailData
	{

		EmailData() : m_nEmailUid(0), m_nTempId(0), m_nPlayerUid(0), m_nOpenState(0), m_nGetState(0), m_strDes(""),
		m_strContent(""){}

		UInt64 m_nEmailUid;
		UInt64 m_nTempId;
		UInt64 m_nPlayerUid;
		UInt32 m_nOpenState;
		UInt32 m_nGetState;
		String  m_strDes;
		String  m_strContent;
		String  m_strFrom;
		String  m_strTittle;
		UInt64 m_nTime;

		LYNX_S11N_10(EmailData, UInt64, m_nEmailUid, UInt64, m_nTempId, UInt64, m_nPlayerUid,
			UInt32 ,m_nOpenState, UInt32, m_nGetState, String,  m_strDes, String,  m_strContent, String,  m_strFrom,
			String, m_strTittle, UInt64, m_nTime);

		
	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_EMAIL_DATA_H__