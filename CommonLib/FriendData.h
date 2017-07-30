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

		List<UInt64> mBeApplyList;

		LYNX_S11N_4(FriendData, UInt32 ,m_nFindBit,  List<UInt64>, mFriendList,
			List<UInt64>, mBlackList,List<UInt64>, mBeApplyList);


	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_FRIEND_DATA_H__