#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_ACTIVITY_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_ACTIVITY_MAMAGER_H__

#include "MainThread.h"
namespace Lynx
{
    class Player;
    class ActivityManager : public MainThread
    {
    public:
        ActivityManager();
        virtual ~ActivityManager();


        class Alarm
        {
        public:
            typedef Delegate<void ()> AlarmCallback;
            struct AlarmInfo 
            {
                UInt32 alarm;
                AlarmCallback callback;
            };
            List<AlarmInfo> mAlarmList;
            List<AlarmInfo> mAlarmEndList;
            UInt32 mPreTime; //上一帧时间
            void initial()
            {
                mPreTime = TimeUtil::getLocalTimeSec();
            }
            void start(UInt32 time, const AlarmCallback& cb)
            {
                AlarmInfo ai = {time, cb};
                mAlarmList.insertTail(ai);
            }
            void update()
            {
                UInt32 currTime = TimeUtil::getLocalTimeSec();
                UInt32 deltaTime = currTime - mPreTime;
                mPreTime = currTime;

                for (List<AlarmInfo>::Iter* i = mAlarmList.begin(); i != mAlarmList.end(); )
                {
                    if (i->mValue.alarm <= deltaTime)
                    {
                        mAlarmEndList.insertTail(i->mValue);
                        i = mAlarmList.erase(i);
                    }
                    else
                    {
                        i->mValue.alarm -= deltaTime;
                        i = mAlarmList.next(i);
                    }
                }

                for (List<AlarmInfo>::Iter* i = mAlarmEndList.begin(); i != mAlarmEndList.end(); i = mAlarmEndList.next(i))
                {
                    i->mValue.callback();
                }
                mAlarmEndList.clear();
            }

            void release()
            {
                mAlarmEndList.clear();
                mAlarmList.clear();
            }
        };

        bool initial(Player* player);
        void release();
        void update(UInt32 accTime);

        void startRuneBattle(TemplateId tollgateId);
        void runeBattleResult(const ActivityRuneResultReq& msg);
        void startGoldBattle(TemplateId tollgateId);
        void goldBattleResult(const ActivityGoldResultReq& msg);
        void startPetBattle(TemplateId tollgateId);
        void petBattleResult(const ActivityPetResultReq& msg);
        void startEnchantBattle(TemplateId tollgateId);
        void enchantBattleResult(const ActivityEnchantResultReq& msg);
        // 查询今天哪个符文能打
        void runeNumber(ActivityRuneNumberReq& msg);
        void refreshSetting();

        UInt32 getActivitySettingResetTime(UInt32 id);
        UInt8 getTodayActivityStatus();
        UInt8 getActivityCount(UInt32 activityType);

        void syncActivityToClient();

        void addStorehouseCount();

        inline void setDirty(){ mDirtyFlag = true; }
    private:

        ErrorId genActivityBattleData(TemplateId tollgateId);
        ErrorId checkActivityTollgateId(TemplateId tollgateId, UInt32 activityType);
        ErrorId checkActivityResult(TemplateId tollgateId, const BattleVerifyData& battleVerifyData, UInt32 activityType);
        void genActivityOppBattleData(PlayerData& oppBattleData);
        
        void startRuneTimer();
        void startGoldTimer();
        void startEnchantTimer();
        void startPetTimer();
        void startStoreHouseTimer();

        void onRuneAlarm();
        void onGoldAlarm();
        void onEnchantAlarm();
        void onPetAlarm();
        void onStoreHouseAlarm();

        void saveActivityData();

        Player* mPlayer;
        UInt32 mPreUpdateTime;
        //UInt32 mRuneActivityTimer;
        //UInt32 mGoldActivityTimer;
        //UInt32 mEnchantActivityTimer;
        //UInt32 mPetActivityTimer;

        Alarm* mAlarm;

        bool mRuneTimer;
        bool mGoldTimer;
        bool mEnchantTimer;
        bool mPetTimer;
        bool mStoreHouseTimer;

        // 正在进行活动战斗的关卡ID
        TemplateId mTollgateIdInBattle;
        List<MonsterData> mActivityMonsterList;
       // PetData mActivityPet;
        // 一天金币活动中累计伤害值
        UInt32 mGoldActivityMyDps;

        bool mDirtyFlag;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_ACTIVITY_MAMAGER_H__
