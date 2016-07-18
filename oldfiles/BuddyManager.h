#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_BUDDY_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_BUDDY_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class BuddyManager : public MainThread
    {
    public:
        BuddyManager();
        virtual ~BuddyManager();

        bool initial(Player* player);
        void release();

        const PlayerBuddyData& getPlayerBuddyData() const { return *mPlayerBuddyData; }
        ErrorId inviteBuddy(const Guid& buddyGuid);
        ErrorId addBuddy(const Guid& buddyGuid);
        ErrorId delBuddy(const Guid& buddyGuid);
        ErrorId getRecommendBuddyList(List<BuddyData>& recommendBuddyList);
        ErrorId syncInviteBuddyList(List<BuddyData>& addBuddyList, 
            List<BuddyData>& changeBuddyList, List<Guid>& delBuddyList);
        ErrorId syncBuddyList(List<BuddyData>& addBuddyList, 
            List<BuddyData>& changeBuddyList, List<Guid>& delBuddyList);
        ErrorId rejectBuddy(const Guid& buddyGuid);

    private:
        Player* mPlayer;
        PlayerBuddyData* mPlayerBuddyData;
        Map<Guid, BuddyData*> mBuddyDataMap;
        Map<Guid, BuddyData*> mInviteBuddyDataMap;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_BUDDY_MAMAGER_H__
