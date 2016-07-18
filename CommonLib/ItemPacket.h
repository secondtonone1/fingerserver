#ifndef __LYNX_COMMON_LIB_ITEM_PACKET_H__
#define __LYNX_COMMON_LIB_ITEM_PACKET_H__

#include "Data.h"

namespace Lynx
{
    enum
    {
        ADD_ITEM_NOTIFY                       = sItemMsgBase + 1,
        DEL_ITEM_NOTIFY                       = sItemMsgBase + 2,
        USE_ITEM_REQ                          = sItemMsgBase + 3,
        USE_ITEM_RESP                         = sItemMsgBase + 4,
        UPDATE_ITEM_NOTIFY                    = sItemMsgBase + 5,
        SELL_ITEM_REQ                         = sItemMsgBase + 6,
        SELL_ITEM_RESP                        = sItemMsgBase + 7,
		ITME_LIST_REQ						  = sItemMsgBase + 8,
		ITME_LIST_RESP						  = sItemMsgBase + 9,
		EQUIP_ENHANCE_REQ					  = sItemMsgBase + 10,
		EQUIP_ENHANCE_RESP					  = sItemMsgBase + 11,
		EQUIP_POLISH_REQ					  = sItemMsgBase + 12,
		EQUIP_POLISH_RESP					  = sItemMsgBase + 13,
		EQUIP_POLISH_RESET_REQ				  = sItemMsgBase + 14,
		EQUIP_POLISH_RESET_RESP				  = sItemMsgBase + 15,
		GEM_COMBINE_REQ						  = sItemMsgBase + 16,
		GEM_COMBINE_RESP                      = sItemMsgBase + 17,
		GEM_COMBINE_ONCE_REQ				  = sItemMsgBase + 18,
		GEM_SET_REQ							  = sItemMsgBase + 19,
		GEM_SET_RESP						  = sItemMsgBase + 20,
		EQUIP_GEM_COMBINE_REQ				  = sItemMsgBase + 21,
		EQUIP_GEM_COMBINE_RESP				  = sItemMsgBase + 22,
		GEM_UNLOAD_REQ						  = sItemMsgBase + 23,
		GEM_UNLOAD_RESP						  = sItemMsgBase + 24,

		ITME_LIST_CHANGE_REQ						  = sItemMsgBase + 40,
		ITME_LIST_CHANGE_RESP						  = sItemMsgBase + 41,
    };

	
    struct UseItemReq
    {
        Guid mItemId;
        UInt32 mCount;

        UseItemReq(): mCount(0), mItemId(0) {}

        LYNX_MESSAGE_2(USE_ITEM_REQ, UseItemReq, Guid, mItemId, UInt32, mCount);
    };

    struct UseItemResp
    {
        errorId merrorId;

        UseItemResp():merrorId(0) {}

        LYNX_MESSAGE_1(USE_ITEM_RESP, UseItemResp, errorId, merrorId);
    };
	
	struct SellItemReq
	{
		SellItemReq(){}

		UInt16 mPacketID;
		std::string mReqJsonStr;
		
		LYNX_MESSAGE_2(SELL_ITEM_REQ, SellItemReq , UInt16, mPacketID, std::string, mReqJsonStr);

	};

    struct SellItemResp
    {
        errorId merrorId;
        UInt32 mGetGold;

        SellItemResp(): merrorId(0), mGetGold(0) {}

        LYNX_MESSAGE_2(SELL_ITEM_RESP, SellItemResp, errorId, merrorId, UInt32, mGetGold);
    };


	struct DelItemNotify
	{
		DelItemNotify() : m_nItemUuid(0) {}

		Guid m_nItemUuid;
		
		LYNX_MESSAGE_1(DEL_ITEM_NOTIFY, DelItemNotify, Guid, m_nItemUuid);
	};

	struct UpdateItemNotify
    {
		UpdateItemNotify(): m_nItemUuid(0), m_nCount(0){}       
		Guid m_nItemUuid;
        UInt32 m_nCount;
		
		
		LYNX_MESSAGE_2(UPDATE_ITEM_NOTIFY, UpdateItemNotify, Guid, m_nItemUuid,  UInt32, m_nCount);
    };

	

	struct AddItemNotify
	{
		
	};

	struct ItemListReq
	{
		ItemListReq():mPacketID(),mCharGUID(0){}
		UInt16	mPacketID;
		UInt64	mCharGUID;

		LYNX_MESSAGE_2(ITME_LIST_REQ, ItemListReq, UInt16, mPacketID, UInt64, mCharGUID);
	};

	struct ItemListResp
	{
		ItemListResp():mPacketID(ITME_LIST_RESP),mRespJsonStr(""){}
		UInt16	mPacketID;
		std::string	mRespJsonStr;
		LYNX_MESSAGE_2(ITME_LIST_RESP, ItemListResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct EquipEhanceReq
	{
		EquipEhanceReq():mPacketID(EQUIP_ENHANCE_REQ),mReqJsonStr(""){}

		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(EQUIP_ENHANCE_REQ, EquipEhanceReq, UInt16, mPacketID, std::string, mReqJsonStr);
		
	};

	struct EquipEhanceResp
	{
		EquipEhanceResp():mPacketID(EQUIP_ENHANCE_RESP),mRespJsonStr(""){}

		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(EQUIP_ENHANCE_RESP, EquipEhanceResp, UInt16, mPacketID, std::string, mRespJsonStr);

	};

	
	struct EquipPolishReq
	{
		EquipPolishReq():mPacketID(EQUIP_POLISH_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(EQUIP_POLISH_REQ, EquipPolishReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct EquipPolishResp
	{
		EquipPolishResp():mPacketID(EQUIP_POLISH_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(EQUIP_POLISH_RESP, EquipPolishResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	
	struct EquipPolishResetReq
	{
		EquipPolishResetReq():mPacketID(EQUIP_POLISH_RESET_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(EQUIP_POLISH_RESET_REQ, EquipPolishResetReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct EquipPolishResetResp
	{
		EquipPolishResetResp():mPacketID(EQUIP_POLISH_RESET_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(EQUIP_POLISH_RESET_RESP, EquipPolishResetResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};
	
	struct GemCombineReq
	{
		GemCombineReq():mPacketID(GEM_COMBINE_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(GEM_COMBINE_REQ, GemCombineReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};
	
	struct GemCombineResp
	{
		GemCombineResp():mPacketID(GEM_COMBINE_RESP),mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(GEM_COMBINE_RESP, GemCombineResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};

	struct GemCombineOnceReq
	{
		GemCombineOnceReq():mPacketID(GEM_COMBINE_ONCE_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(GEM_COMBINE_ONCE_REQ, GemCombineOnceReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};

	struct GemSetReq
	{
		GemSetReq():mPacketID(GEM_SET_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(GEM_SET_REQ, GemSetReq, UInt16, mPacketID, std::string, mReqJsonStr);
		
	};

	struct GemSetResp
	{
		GemSetResp():mPacketID(GEM_SET_RESP), mRespJsonStr(""){}
		
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(GEM_SET_RESP, GemSetResp, UInt16, mPacketID, std::string, mRespJsonStr);

	};
	
	struct GemUnloadReq
	{
		GemUnloadReq():mPacketID(GEM_UNLOAD_REQ), mReqJsonStr(""){}

		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(GEM_UNLOAD_REQ, GemUnloadReq, UInt16, mPacketID, std::string, mReqJsonStr);

	};

	struct GemUnloadResp
	{
		GemUnloadResp():mPacketID(GEM_UNLOAD_RESP), mRespJsonStr(""){}

		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(GEM_UNLOAD_RESP, GemUnloadResp, UInt16, mPacketID, std::string, mRespJsonStr);

	};
		

	struct EquipGemCombineReq
	{
		EquipGemCombineReq():mPacketID(EQUIP_GEM_COMBINE_REQ),mReqJsonStr(""){}
		UInt16 mPacketID;
		std::string mReqJsonStr;
		LYNX_MESSAGE_2(EQUIP_GEM_COMBINE_REQ, EquipGemCombineReq, UInt16, mPacketID, std::string, mReqJsonStr);
	};
	
	struct EquipGemCombineResp
	{
		EquipGemCombineResp():mPacketID(EQUIP_GEM_COMBINE_RESP), mRespJsonStr(""){}
		UInt16 mPacketID;
		std::string mRespJsonStr;
		LYNX_MESSAGE_2(EQUIP_GEM_COMBINE_RESP, EquipGemCombineResp, UInt16, mPacketID, std::string, mRespJsonStr);
	};
		

} // namespace Lynx

#endif // __LYNX_COMMON_LIB_ITEM_MSG_H__