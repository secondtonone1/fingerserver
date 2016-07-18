#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_PRODUCT_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_PRODUCT_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class ProduceManager : public MainThread
    {
    public:
        ProduceManager();
        virtual ~ProduceManager();

        bool initial(Player* player);
        void release();

        bool upgreadGoldMineral();
        bool upspeedGoldMineral();
        bool upgreadDiamondMineral();
        bool upspeedDiamondMineral();
        bool upgreadExpMineral();
        bool upspeedExpMineral();

        UInt32 getGoldMineral();
        UInt32 getDiamondMineral();
    private:
        Player* mPlayer;
        PlayerProduceData* mPlayerProduceData;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_PRODUCT_MAMAGER_H__
