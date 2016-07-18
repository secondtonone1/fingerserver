#ifndef __LYNX_COMMON_LIB_BAG_MSG_H__
#define __LYNX_COMMON_LIB_BAG_MSG_H__

#include "Data.h"

namespace Lynx
{
	enum
	{
		ITEM_SALE_REQ						 = sBagMsgBase + 1;
		ITEM_SALE_RESP						 = sBagMsgBase + 2;
		FASHION_BUY_REQ						 = sBagMsgBase + 3;
		FASHION_BUY_RESP					 = sBagMsgBase + 4;
	};

	
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_BAG_MSG_H__