#ifndef __LYNX_COMMON_LIB_TICKETFRIEND_TABLE_H__
#define __LYNX_COMMON_LIB_TICKETFRIEND_TABLE_H__

#include "Data.h"
#include "PlatformLib.h"

namespace Lynx
{
	struct TicketFriendTemplate 
	{
		UInt32 mId;
		UInt32 mLv;
		UInt32 mRate;
		UInt32 mTag;

		UInt64 mAward1;
		UInt64 mAward2;
		UInt64 mAward3;
		UInt64 mAward4;
		UInt64 mAward5;
		List<UInt64> mAwardList;

		
		UInt32 mSupport1;
		UInt32 mSupport2;
		UInt32 mSupport3;
		UInt32 mSupport4;
		List<UInt32> mSupportList;

		UInt32 mPeapleCount1;
		UInt32 mPeapleCount2;
		UInt32 mPeapleCount3;
		List<UInt32> mPeapleCountList;
		
		UInt32 mJoinCount1;
		UInt32  mJoinCount2;
		UInt32 mJoinCount3;

		LYNX_S11N_21(TicketFriendTemplate,UInt32, mId, UInt32, mLv,
			UInt32, mRate, UInt32, mTag, UInt64, mAward1,UInt64, mAward2,UInt64, mAward3,
			UInt64 ,mAward4, UInt64, mAward5,List<UInt64>, mAwardList, UInt32, mSupport1,
			UInt32, mSupport2, UInt32, mSupport3, List<UInt32>, mSupportList,UInt32, mPeapleCount1,
			UInt32, mPeapleCount2,UInt32, mPeapleCount3,List<UInt32>, mPeapleCountList,UInt32, mJoinCount1,
			UInt32,  mJoinCount2, UInt32 ,mJoinCount3
			);

	};

	typedef Map<UInt32, TicketFriendTemplate> TicketFriendTemplateMap;
	typedef Map<UInt32, List<TicketFriendTemplate> >TicketFriendLvMap;

	struct TicketFriendTable
	{
		TicketFriendTemplateMap mMap;
		TicketFriendLvMap mLvMap;

		LYNX_S11N_2(TicketFriendTable, TicketFriendTemplateMap, mMap,TicketFriendLvMap, mLvMap);

		TicketFriendTemplate* get(UInt32 id)
		{
			TicketFriendTemplateMap::Iter* iter = mMap.find(id);

			if (iter == mMap.end())
			{
				return NULL;
			}
			return &iter->mValue;
		}

		 List<TicketFriendTemplate> * getLvList(UInt32 lv)
		 {
			 TicketFriendLvMap::Iter * iter = mLvMap.find(lv);
			 if(iter == mLvMap.end() )
			 {
				return NULL;
			 }
			
			 return &iter->mValue;
		 }

		void clear()
		{
			mMap.clear();
			mLvMap.clear();
		}

		bool loadFromDbc(const String& fileName);
		bool reloadFromDbc(const String& fileName);
		bool saveToDbc(const String& filePath);
		bool loadFromCsv(const String& filePath);

	};
} // namespace Lynx

#endif // __LYNX_COMMON_LIB_TICKETFRIEND_TABLE_H__