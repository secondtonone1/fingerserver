#ifndef __LYNX_COMMON_LIB_FRIEND_DATA_H__
#define __LYNX_COMMON_LIB_FRIEND_DATA_H__

#include "Definition.h"

namespace Lynx
{
	struct FriendData
	{
		FriendData() :  m_nFindBit(0){}

		//标记内存中是否有该数据
		UInt32 m_nFindBit;

		List<UInt64> mFriendList;
		List<UInt64> mBlackList;

		LYNX_S11N_3(FriendData, UInt32 ,m_nFindBit,  List<UInt64>, mFriendList,
			List<UInt64>, mBlackList);


	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_FRIEND_DATA_H__