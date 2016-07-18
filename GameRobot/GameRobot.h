#ifndef __LYNX_GAME_ROBOT_H__
#define __LYNX_GAME_ROBOT_H__

#include "CommonLib.h"
#include "RobotMsgHandler.h"

namespace Lynx
{
    class GameRobot : public TcpConnection
    {
    public:
        GameRobot(IOService* ioService);
        virtual ~GameRobot();

        bool initial(const String& accountName, const String& password,
                     const String& ip, UInt16 port);
        void release();
        void update(const UInt64& accTime);

        template <typename MsgType >
        void sendMsg(const MsgType& msg)
        {
            if (mIsConnecting)
            {
                mMsgHandler.sendMsg(*this, msg);
            }
        }

        void registerMsgHandler();

    public:
        PlayerData& getPlayerData() { return mPlayerData; }
        void closeConnection();
        void sendChatMsg(const Guid& playerGuid, const String& contents);
        void sendGetOneHeroMsg();
        void sendGetTenHeroMsg();

        
    private:
        void onConnected(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);
        void onConnectFailed(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);
        void onConnectBroken(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);

    private:
        void onLoginSuccessResp(LoginSuccessResp& msg);
        void onLoginFailResp(LoginFailResp& msg);
        void onReconnectSuccessResp(ReconnectSuccessResp& msg);
        void onReconnectFailResp(ReconnectFailResp& msg);
        void onAccountWasLoginNotify(AccountWasLoginNotify& msg);
        //void onSyncPlayerChatNotify(SyncPlayerChatNotify& msg);
        void onSyncPlayerLevelNotify(SyncPlayerLevelNotify& msg);
        void onSyncPlayerExpNotify(SyncPlayerLevelExpNotify& msg);
        void onSyncPlayerEnergyNotify(SyncPlayerEnergyNotify& msg);
        void onSyncPlayerGoldNotify(SyncPlayerGoldNotify& msg);
        void onAddHeroNotify(AddHeroNotify& msg);
        void onDelHeroNotify(DelHeroNotify& msg);
        void onAddPetNotify(AddPetNotify& msg);
        void onDelPetNotify(DelPetNotify& msg);
      /*  void onGetOneHeroSuccessResp(GetOneHeroSuccessResp& msg);
        void onGetOneHeroFailResp(GetOneHeroFailResp& msg);
        void onGetTenHeroSuccessResp(GetTenHeroSuccessResp& msg);
        void onGetTenHeroFailResp(GetTenHeroFailResp& msg);*/
        void onSyncHeroExpNotify(SyncHeroLevelExpNotify& msg);
        void onSyncHeroAdvancedNotify(SyncHeroAdvancedNotify& msg);
        void onSyncHeroLevelNotify(SyncHeroLevelNotify& msg);
        void onAddItemNotify(AddItemNotify& msg);
        void onAddHeroDebrisNotify(AddHeroDebrisNotify& msg);
        void onUpdateItemNotify(UpdateItemNotify& msg);
        void onUpdateHeroDebrisNotify(UpdateHeroDebrisNotify& msg);
        void onDelItemNotify(DelItemNotify& msg);
        void onDelHeroDebrisNotify(DelHeroDebrisNotify& msg);
        void onAddEquipNotify(AddEquipNotify& msg);
        void onAddRuneNotify(AddRuneNotify& msg);
        void onUpdateEquipRefineLevelNotify(UpdateEquipRefineLevelNotify& msg);
        void onUpdateEquipEnchantLevelNotify(UpdateEquipEnchantLevelNotify& msg);
      /*  void onUpdateRuneRefineLevelNotify(UpdateRuneRefineLevelNotify& msg);*/
        void onDelEquipNotify(DelEquipNotify& msg);
        void onDelRuneNotify(DelRuneNotify& msg);

    private:
        void calculatePlayerData(PlayerData& playerData);
        UInt32 calculatePlayerBattleValue(const PlayerData& playerData);
        UInt32 calculateHeroHpUpper(const PlayerData& playerData, const HeroData& heroData); 
        UInt32 calculateHeroAngerUpper(const PlayerData& playerData, const HeroData& heroData);
        UInt32 calculateHeroAttack(const PlayerData& playerData, const HeroData& heroData);
        UInt32 calculateHeroPhysicsDefense(const PlayerData& playerData, const HeroData& heroData);
        UInt32 calculateHeroMagicDefense(const PlayerData& playerData, const HeroData& heroData);
        UInt32 calculateHeroCritical(const PlayerData& playerData, const HeroData& heroData);
        UInt32 calculateHeroHit(const PlayerData& playerData, const HeroData& heroData);
        UInt32 calculateHeroDodge(const PlayerData& playerData, const HeroData& heroData);
        UInt32 calculateHeroResilience(const PlayerData& playerData, const HeroData& heroData);
        UInt32 calculateHeroStamina(const PlayerData& playerData, const HeroData& heroData);
        UInt32 calculateHeroIntellect(const PlayerData& playerData, const HeroData& heroData);
        UInt32 calculateHeroStrength(const PlayerData& playerData, const HeroData& heroData);
        
    protected:
        IOService* mIOService;
        RobotMsgHandler mMsgHandler;
        MsgHdr mMsgHdr;
        String mAccountName;
        String mPassword;
        String mLoginIp;
        UInt16 mLoginPort;
        bool mIsConnecting;
        bool mStopFlag;

        PlayerData mPlayerData;
        UInt64 mLoginToken;
    };

} // namespace Lynx

#endif // __LYNX_GAME_ROBOT_H__

