#ifndef __LYNX_COMMON_LIB_ITEM_MSG_H__
#define __LYNX_COMMON_LIB_ITEM_MSG_H__

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
		ITME_LIST_RESP						  = sItemMsgBase + 9
    };

	//原游戏添加物品操作 sec
   /* struct AddItemNotify
    {
        AddItemNotify(): mPromp(false) {}

        ItemData mItemData;
        bool mPromp;

        LYNX_MESSAGE_2(ADD_ITEM_NOTIFY, AddItemNotify, ItemData, mItemData, bool, mPromp);
    };*/

	//原游戏删除条目的通知
  /*  struct DelItemNotify
    {
        DelItemNotify() : mItemGuid(0) {}

        Guid mItemGuid;

        LYNX_MESSAGE_1(DEL_ITEM_NOTIFY, DelItemNotify, Guid, mItemGuid);
    };*/

    struct UseItemReq
    {
        Guid mItemId;
        UInt32 mCount;

        UseItemReq(): mCount(0), mItemId(0) {}

        LYNX_MESSAGE_2(USE_ITEM_REQ, UseItemReq, Guid, mItemId, UInt32, mCount);
    };

    struct UseItemResp
    {
        ErrorId mErrorId;

        UseItemResp():mErrorId(0) {}

        LYNX_MESSAGE_1(USE_ITEM_RESP, UseItemResp, ErrorId, mErrorId);
    };
	
   /* struct UpdateItemNotify
    {
        Guid mItemGuid;
        UInt32 mCount;
        bool mPromp;

        UpdateItemNotify(): mCount(0), mItemGuid(0), mPromp(false) {}

        LYNX_MESSAGE_3(UPDATE_ITEM_NOTIFY, UpdateItemNotify, Guid, mItemGuid, UInt32, mCount, bool, mPromp);
    };*/

   /* struct SellItemReq
    {
        List<Guid> mSellList;

        SellItemReq() {}
		
        LYNX_MESSAGE_1(SELL_ITEM_REQ, SellItemReq, List<Guid>, mSellList);
    };*/

	struct SellItemReq
	{
		SellItemReq(){}
		std::string m_strRecvMsg;
		List<Guid> mSellList;
		
		void analyzeString()
		{
			std::stringstream mystream;
			std::string subString;
			int count = 0;
			//截取第一个“#”
			int index = m_strRecvMsg.find_first_of("#");
			assert(index != string::npos);
			if(index != string::npos)
			{
				//取出总个数
				std::string countStr = m_strRecvMsg.substr(0,index);
				mystream.clear();
				mystream.str("");
				mystream << countStr;
				mystream >> count;
			}


			//字符串索引偏移移位，移动到#后面的字符，然后从此处开始截取直到字符串的结尾 sec
			index ++;
			subString = m_strRecvMsg.substr(index,m_strRecvMsg.length()-index);
			//从剩余的字串中查找
			index = subString.find_first_of("#");
			while(index != string::npos)
			{
				std::string numStr = subString.substr(0,index);
				Guid num;
				mystream.clear();
				mystream.str("");
				mystream << numStr;
				mystream >> num;
				mSellList.insertTail(num);
				index++;
				subString = subString.substr(index,subString.length()-index);
				index = subString.find_first_of("#");
			}

			mystream.clear();
			mystream.str("");
			mystream << subString;
			Guid numLast;
			mystream >> numLast;
			mSellList.insertTail(numLast);
			assert(mSellList.size()==count);
		}
		
		LYNX_MESSAGE_1(SELL_ITEM_REQ, SellItemReq,std::string, m_strRecvMsg);

	};

    struct SellItemResp
    {
        ErrorId mErrorId;
        UInt32 mGetGold;

        SellItemResp(): mErrorId(0), mGetGold(0) {}

        LYNX_MESSAGE_2(SELL_ITEM_RESP, SellItemResp, ErrorId, mErrorId, UInt32, mGetGold);
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

	struct ItemListDataS2C
	{
		ItemListDataS2C(){}

		PlayerItemListS2C mItemListData;
		LYNX_MESSAGE_1(SERVER_2_CLIENT_ITEM_DATA, ItemListDataS2C, PlayerItemListS2C, mItemListData);
	};

	struct AddItemNotify
	{
		AddItemNotify(): m_strSendMsg("") {}

		ItemData2Client mItemData;
		
		std::string m_strSendMsg;
		
		std::string constructString(int num = 0)
		{
			m_strSendMsg = mItemData.constructString(num);
		}

		LYNX_MESSAGE_1(ADD_ITEM_NOTIFY, AddItemNotify, std::string, m_strSendMsg);
	};

	

} // namespace Lynx

#endif // __LYNX_COMMON_LIB_ITEM_MSG_H__