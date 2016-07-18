#include "SummonManager.h"
#include "Player.h"
#include "GameServer.h"

using namespace Lynx;

SummonManager::SummonManager() : mPlayer(NULL), mPlayerSummonData(NULL)
{

}


SummonManager::~SummonManager()
{

}

bool 
SummonManager::initial(Player* player)
{
    if (player == NULL)
    {
        LOG_WARN("Player pointer is NULL.");
        return false;
    }

    mPlayer = player;
	//mPlayerData.mSummonData
    //mPlayerSummonData = &mPlayer->mPlayerData.mSummonData;

    return true;
}

void 
SummonManager::release()
{
    mPlayer = NULL;
    mPlayerSummonData = NULL;
}

ErrorId 
SummonManager::summonHeroByItem(List<Guid>& heroGuidList)
{
    Guid heroGuid = randPkg(ITEM_SUMMON_PKG);
    if (!heroGuid)
    {
        return LynxErrno::HeroNotExist;
    }
    heroGuidList.insertTail(heroGuid);
    return LynxErrno::None;
}

ErrorId 
SummonManager::summonHeroByDiamond(List<Guid>& heroGuidList)
{
    if (mPlayerSummonData->mDiamondSummonCount == 0)
    {
        Guid heroGuid = randPkg(DIAMOND_SUMMON_PKG_0);
        if (!heroGuid)
        {
            return LynxErrno::HeroNotExist;
        }
        heroGuidList.insertTail(heroGuid);
        mPlayerSummonData->mDiamondSummonCount++;
        return LynxErrno::None;
    }
    else if (mPlayerSummonData->mDiamondSummonCount == 1)
    {
        Guid heroGuid = randPkg(DIAMOND_SUMMON_PKG_1);
        if (!heroGuid)
        {
            return LynxErrno::HeroNotExist;
        }
        heroGuidList.insertTail(heroGuid);
        mPlayerSummonData->mDiamondSummonCount++;
        return LynxErrno::None;
    }
    else if (mPlayerSummonData->mDiamondSummonCount == 2)
    {
        Guid heroGuid = randPkg(DIAMOND_SUMMON_PKG_2);
        if (!heroGuid)
        {
            return LynxErrno::HeroNotExist;
        }
        heroGuidList.insertTail(heroGuid);
        mPlayerSummonData->mDiamondSummonCount++;
        return LynxErrno::None;
    }
    else if (mPlayerSummonData->mDiamondSummonCount == 3)
    {
        Guid heroGuid = randPkg(DIAMOND_SUMMON_PKG_3);
        if (!heroGuid)
        {
            return LynxErrno::HeroNotExist;
        }
        heroGuidList.insertTail(heroGuid);
        mPlayerSummonData->mDiamondSummonCount++;
        return LynxErrno::None;
    }
    else if (mPlayerSummonData->mDiamondSummonCount == 4)
    {
        Guid heroGuid = randPkg(DIAMOND_SUMMON_PKG_4);
        if (!heroGuid)
        {
            return LynxErrno::HeroNotExist;
        }
        heroGuidList.insertTail(heroGuid);
        mPlayerSummonData->mDiamondSummonCount++;
        return LynxErrno::None;
    }
    else
    {
        if ((mPlayerSummonData->mDiamondSummonCount + mPlayerSummonData->mFreeDiamondSummonCount) % 10 == 0) // 必抽5星卡
        {
            Guid heroGuid = randPkg(SUPPER_HERO_PKG);
            if (!heroGuid)
            {
                return LynxErrno::HeroNotExist;
            }
            heroGuidList.insertTail(heroGuid);
            mPlayerSummonData->mDiamondSummonCount++;
            return LynxErrno::None;
        }
        else
        {
            Guid heroGuid = randPkg(DIAMOND_SUMMON_PKG);
            if (!heroGuid)
            {
                return LynxErrno::HeroNotExist;
            }
            heroGuidList.insertTail(heroGuid);
            mPlayerSummonData->mDiamondSummonCount++;
            return LynxErrno::None;
        }
    }
}

ErrorId 
SummonManager::summonHeroByFreeDiamond(List<Guid>& heroGuidList)
{
    if (mPlayerSummonData->mFreeDiamondSummonCount == 0)
    {
        Guid heroGuid = randPkg(FREE_DIAMOND_SUMMON_PKG_0);
        if (!heroGuid)
        {
            return LynxErrno::HeroNotExist;
        }
        heroGuidList.insertTail(heroGuid);
        mPlayerSummonData->mFreeDiamondSummonCount++;
        return LynxErrno::None;
    }
    else if (mPlayerSummonData->mFreeDiamondSummonCount == 1)
    {
        Guid heroGuid = randPkg(FREE_DIAMOND_SUMMON_PKG_1);
        if (!heroGuid)
        {
            return LynxErrno::HeroNotExist;
        }
        heroGuidList.insertTail(heroGuid);
        mPlayerSummonData->mFreeDiamondSummonCount++;
        return LynxErrno::None;
    }
    else if (mPlayerSummonData->mFreeDiamondSummonCount == 2)
    {
        Guid heroGuid = randPkg(FREE_DIAMOND_SUMMON_PKG_2);
        if (!heroGuid)
        {
            return LynxErrno::HeroNotExist;
        }
        heroGuidList.insertTail(heroGuid);
        mPlayerSummonData->mFreeDiamondSummonCount++;
        return LynxErrno::None;
    }
    else if (mPlayerSummonData->mFreeDiamondSummonCount == 3)
    {
        Guid heroGuid = randPkg(FREE_DIAMOND_SUMMON_PKG_3);
        if (!heroGuid)
        {
            return LynxErrno::HeroNotExist;
        }
        heroGuidList.insertTail(heroGuid);
        mPlayerSummonData->mFreeDiamondSummonCount++;
        return LynxErrno::None;
    }
    else if (mPlayerSummonData->mFreeDiamondSummonCount == 4)
    {
        Guid heroGuid = randPkg(FREE_DIAMOND_SUMMON_PKG_4);
        if (!heroGuid)
        {
            return LynxErrno::HeroNotExist;
        }
        heroGuidList.insertTail(heroGuid);
        mPlayerSummonData->mFreeDiamondSummonCount++;
        return LynxErrno::None;
    }
    else
    {
        if ((mPlayerSummonData->mDiamondSummonCount + mPlayerSummonData->mFreeDiamondSummonCount) % 10 == 0) // 必抽5星卡
        {
            Guid heroGuid = randPkg(SUPPER_HERO_PKG);
            if (!heroGuid)
            {
                return LynxErrno::HeroNotExist;
            }
            heroGuidList.insertTail(heroGuid);
            mPlayerSummonData->mFreeDiamondSummonCount++;
            return LynxErrno::None;
        }
        else
        {
            Guid heroGuid = randPkg(FREE_DIAMOND_SUMMON_PKG);
            if (!heroGuid)
            {
                return LynxErrno::HeroNotExist;
            }
            heroGuidList.insertTail(heroGuid);
            mPlayerSummonData->mFreeDiamondSummonCount++;
            return LynxErrno::None;
        }
    }
    return LynxErrno::None;
}

ErrorId 
SummonManager::summonHeroByBatch(List<Guid>& heroGuidList)
{
    UInt32 supperHeroPos = mPlayerSummonData->mDiamondSummonCount + mPlayerSummonData->mFreeDiamondSummonCount;
    supperHeroPos = supperHeroPos % 10;

    Guid supperHeroGuid = randPkg(SUPPER_HERO_PKG);
    if (!supperHeroGuid)
    {
        return LynxErrno::HeroNotExist;
    }

    Guid heroGuid[8];
    for (UInt32 i = 0; i < 8; ++i)
    {
        heroGuid[i] = randPkg(BATCH_SUMMON_PKG_0);
        if (!heroGuid[i])
        {
            return LynxErrno::HeroNotExist;
        }
    }

    Guid randHeroGuid = randPkg(BATCH_SUMMON_PKG_1);
    if (!randHeroGuid)
    {
        return LynxErrno::HeroNotExist;
    }

    UInt32 randHeroPos = 0;
    while (1)
    {
        randHeroPos = RANDOM().getUInt32Random(0, 9);
        if (randHeroPos != supperHeroPos)
        {
            break;
        }
    }

    UInt32 heroGuidIndex = 0;
    if (supperHeroPos < randHeroPos)
    {
        for (UInt32 i = 0; i < supperHeroPos; ++i)
        {
            heroGuidList.insertTail(heroGuid[heroGuidIndex]);
            heroGuidIndex++;
        }

        heroGuidList.insertTail(supperHeroGuid);

        for (UInt32 i = supperHeroPos + 1; i < randHeroPos; ++i)
        {
            heroGuidList.insertTail(heroGuid[heroGuidIndex]);
            heroGuidIndex++;
        }

        heroGuidList.insertTail(randHeroGuid);

        for (UInt32 i = randHeroPos + 1; i < 10; ++i)
        {
            heroGuidList.insertTail(heroGuid[heroGuidIndex]);
            heroGuidIndex++;
        }
    }
    else
    {
        for (UInt32 i = 0; i < randHeroPos; ++i)
        {
            heroGuidList.insertTail(heroGuid[heroGuidIndex]);
            heroGuidIndex++;
        }

        heroGuidList.insertTail(randHeroGuid);

        for (UInt32 i = randHeroPos + 1; i < supperHeroPos; ++i)
        {
            heroGuidList.insertTail(heroGuid[heroGuidIndex]);
            heroGuidIndex++;
        }

        heroGuidList.insertTail(supperHeroGuid);

        for (UInt32 i = supperHeroPos + 1; i < 10; ++i)
        {
            heroGuidList.insertTail(heroGuid[heroGuidIndex]);
            heroGuidIndex++;
        }
    }
    
    return LynxErrno::None;
}

Guid 
SummonManager::randPkg(const TemplateId& pkgId)
{
    DropItemTemplate* dropItemTemplate = DROP_ITEM_TABLE().get(pkgId);
    if (!dropItemTemplate)
    {
        LOG_WARN("Failed to get DropItemTable by %u", pkgId);
        return 0;
    }

    UInt32 total = 0;
    for (List<DropLine>::Iter* iter = dropItemTemplate->mDropItemList.begin();
        iter != NULL; iter = dropItemTemplate->mDropItemList.next(iter))
    {
        total += iter->mValue.mDropOdds;
    }

    if (total == 0)
    {
        return 0;
    }

    TemplateId heroTemplateId = 0;

    // 随机一个数字
    UInt32 randValue = RANDOM().getUInt32Random(1, total);
    UInt32 currValue = 0;
    for (List<DropLine>::Iter* iter = dropItemTemplate->mDropItemList.begin();
        iter != NULL; iter = dropItemTemplate->mDropItemList.next(iter))
    {
        currValue += iter->mValue.mDropOdds;
        if (randValue > currValue)
        {
            continue;
        }
        // 没超过， 是这个数
        heroTemplateId = iter->mValue.mItemId;
    }


    if (heroTemplateId != 0)
    {
        const HeroData* heroData = mPlayer->getHeroManager().createHero(heroTemplateId);
        if (heroData)
        {
            return heroData->mBaseData.mHeroId;
        }
    }

    return 0;
}