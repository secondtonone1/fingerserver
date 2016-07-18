#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_RUNE_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_RUNE_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class RuneManager : public MainThread
    {
    public: 
        RuneManager();
        virtual ~RuneManager();

        bool initial(Player* player);
        void release();
        void calculateRuneData(UInt8 runePresentPos);

    public:
        // 检查符文栏是否满格
        bool isRuneFull();
        bool isRuneFull(List<TemplateId>& runeList);
        bool isRuneFull(List<AwardLine>& awardList);
        // 获得符文栏空格子数
        UInt8 getRuneFieldSpaceSlot();
        // 锁定符文
        void lockRune(const Guid& runeId);
        // 解锁符文
        void unLockRune(const Guid& runeId);

        // 摧毁符文
        void destroyRune(Guid runeId);
        // 清空符文
        void destroyAllRune();

        // 使用符文
        ErrorId useRune(const Guid& runeId, UInt8 targetPos);
        // 创建符文
        const RuneData* createRune(TemplateId runeTemplateId, UInt8 source, bool promp = true);

        void setRuneRefineLevel(const Guid& runeId, UInt32 refineLevel);

        // 添加符文强化经验, 触发升级
        ErrorId addRuneRefineExp(const Guid& runeId, UInt32 incExp, UInt32& refineLevel);

        // 强化符文
        ErrorId refineRune(const Guid& runeId, const List<Guid>& itemList, UInt32& refineLevel);

        const RuneData* getRuneDataByGuid(const Guid& runeId);

        void setRunePresentPos(const Guid& runeId, UInt8 pos);
        ErrorId sellRune(List<Guid>& runeList, UInt32& getGold);
    private:
        Player* mPlayer;
        List<RuneData>* mRuneList;
        Map<Guid, RuneData*> mRuneMap;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_RUNE_MAMAGER_H__


