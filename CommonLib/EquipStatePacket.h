#ifndef __LYNX_COMMON_LIB_EQUIP_STATE_PACKET_H__
#define __LYNX_COMMON_LIB_EQUIP_STATE_PACKET_H__

#include "Data.h"

namespace Lynx
{
	enum 
	{
		BOC_PLAYER_EQUIP_STATE_REQ					= sEquipMsgBase + 1,
		BOC_PLAYER_EQUIP_STATE_RESP					= sEquipMsgBase + 2
	};

	//-----------------CG-------------------


	//-----------------GC-------------------


	struct GCPlayerEquipStateResp
	{
		GCPlayerEquipStateResp():mPacketID(BOC_PLAYER_EQUIP_STATE_RESP),mRespJsonStr(""){}

		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(BOC_PLAYER_EQUIP_STATE_RESP, GCPlayerEquipStateResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

}// namespace Lynx

#endif //__LYNX_COMMON_LIB_EQUIP_STATE_PACKET_H__

