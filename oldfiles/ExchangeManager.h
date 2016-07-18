#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_EXCHANGE_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_EXCHANGE_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class ExchangeManager : public MainThread
    {
    public:
        ExchangeManager();
        virtual ~ExchangeManager();

        bool initial(Player* player);
        void release();
        void update(const UInt64& accTime);

        ErrorId exchangeItem(UInt32 exchangeId, UInt32 count = 1);
        ErrorId receiveHonorAward(bool& flag);

    private:
        void resetDaily();
    private:
        Player* mPlayer;
        PlayerExchangeData* mPlayerExchangeData;
        Map<TemplateId, ExchangeItem*> mExchangeItemMap;
        UInt32 mCurTimeSec;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_EXCHANGE_MAMAGER_H__
