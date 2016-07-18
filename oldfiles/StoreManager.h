#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_STORE_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_STORE_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class StoreManager : public MainThread
    {
    public:
        StoreManager();
        virtual ~StoreManager();

        bool initial(Player* player);
        void release();
        void update(const UInt64& accTime);

        ErrorId buyStoreItem(TemplateId& templateId, UInt8& type, UInt32& count);
        ErrorId buySecretStoreItem(TemplateId& templateId);
        ErrorId resetSecretStoreItemByUseItem();
    private:
        void resetDaily();
        void resetSecretStore();
    private:
        Player* mPlayer;
        PlayerStoreData* mPlayerStoreData;
        Map<TemplateId, StoreItemData*> mStoreItemMap;
        UInt32 mCurTimeSec;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_STORE_MAMAGER_H__
