#ifndef __LYNX_COMMON_LIB_HOARSTONE_DATA_H__
#define __LYNX_COMMON_LIB_HOARSTONE_DATA_H__

#include "Definition.h"

namespace Lynx
{
	struct HoarStoneData
	{
		UInt64 mHoarStoneId;
		UInt32 mStoneLv;
		UInt32 mPieceCount;
		UInt32 mStoneStar;
	
		List<UInt64> mEquipList;

		LYNX_S11N_5(HoarStoneData,UInt64, mHoarStoneId,UInt32,mStoneLv,UInt32, mPieceCount,UInt32, mStoneStar,List<UInt64>, mEquipList);

	};

	struct runePosData
	{
		UInt32 runePos;
		UInt64 runeID;
	};

} // namespace Lynx

#endif // __LYNX_COMMON_LIB_HOARSTONE_DATA_H__