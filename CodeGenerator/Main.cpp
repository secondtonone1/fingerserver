#include "CommonLib.h"

#include "CSharpCoder.h"

using namespace Lynx;

#define ADD_CLASS(fileName, namespaceName, className, parentClassName) \
do \
{ \
    Map<String, List<String> >::Iter* iter = fileMap.find(#fileName); \
    if (iter == NULL) \
    { \
        List<String> classList; \
        iter = fileMap.insert(#fileName, classList); \
    } \
    memset(buf, 0, 409600); \
    className obj; \
    CSharpElement xmlObj(#className); \
    if (String("NULL") == #parentClassName) \
        gParentClassObjectSize = 0; \
    else \
        gParentClassObjectSize = sizeof(parentClassName); \
    gS11NObjectAddress = (char*)&obj; \
    serialize(obj, xmlObj); \
    Map<String, String>::Iter* jter = csharpCodeMap.find(#className);\
    coder.Generate(buf, &xmlObj, #namespaceName, #className, #parentClassName, jter->mValue, 0); \
    iter->mValue.insertTail(buf); \
} while (0) 

#define ADD_TABLE(fileName, namespaceName, className, parentClassName) \
do \
{ \
    Map<String, List<String> >::Iter* iter = fileMap.find(#fileName); \
    if (iter == NULL) \
    { \
        List<String> classList; \
        iter = fileMap.insert(#fileName, classList); \
    } \
    memset(buf, 0, 409600); \
    className obj; \
    CSharpElement xmlObj(#className); \
    if (String("NULL") == #parentClassName) \
        gParentClassObjectSize = 0; \
    else \
        gParentClassObjectSize = sizeof(parentClassName); \
    gS11NObjectAddress = (char*)&obj; \
    serialize(obj, xmlObj); \
    Map<String, String>::Iter* jter = csharpCodeMap.find(#className);\
    coder.GenerateTable(buf, &xmlObj, #namespaceName, #className, #parentClassName, jter->mValue); \
    iter->mValue.insertTail(buf); \
} while (0) 

#define ADD_MESSAGE(fileName, namespaceName, className, parentClassName) \
do \
{ \
    Map<String, List<String> >::Iter* iter = fileMap.find(#fileName); \
    if (iter == NULL) \
    { \
        List<String> classList; \
        iter = fileMap.insert(#fileName, classList); \
    } \
    memset(buf, 0, 409600); \
    className obj; \
    CSharpElement xmlObj(#className); \
    if (String("NULL") == #parentClassName) \
        gParentClassObjectSize = 0; \
    else \
        gParentClassObjectSize = sizeof(parentClassName); \
    gS11NObjectAddress = (char*)&obj; \
    serialize(obj, xmlObj); \
    Map<String, String>::Iter* jter = csharpCodeMap.find(#className);\
    coder.Generate(buf, &xmlObj, #namespaceName, #className, #parentClassName, jter->mValue, className::sMsgId); \
    iter->mValue.insertTail(buf); \
} while (0) 

int main(int argc, char** argv) 
{
    CSharpCoder coder; 
    char buf[409600]; 
    Map<String, List<String> > fileMap; 
    Map<String, String> csharpCodeMap; 
    StringMap<String> tableFileMap; 

    FileUtil::findFiles("../../CommonLib", "*Table.h", tableFileMap, false);
    tableFileMap.erase("Table.h");
    StreamBuffer stream;
    FileUtil::loadFile("../Binaries/codegenerator_tmp", stream); 
    unserialize(csharpCodeMap, stream); 
    //AccountData.h
    ADD_CLASS(AccountData.h, NULL, AccountData, NULL);

    //AchievementData.h
    ADD_CLASS(AchievementData.h, NULL, AchievementBaseData, NULL);

    //ArenaData.h
    ADD_CLASS(ArenaData.h, NULL, ArenaHeroShowData, NULL);
    ADD_CLASS(ArenaData.h, NULL, ArenaPlayerShowData, NULL);
    ADD_CLASS(ArenaData.h, NULL, ArenaBattleResultData, NULL);
    ADD_CLASS(ArenaData.h, NULL, ArenaHeroExtendData, NULL);

    //BattleData.h
    ADD_CLASS(BattleData.h, NULL, BattleSimulationData, NULL);
    ADD_CLASS(BattleData.h, NULL, BattleVerifyData, NULL);

    //BuddyData.h
    ADD_CLASS(BuddyData.h, NULL, BuddyData, NULL);

    //BufferData.h
    ADD_CLASS(BufferData.h, NULL, BufferData, NULL);

    //ChatData.h
    ADD_CLASS(ChatData.h, NULL, ChatData, NULL);

    //EmailData.h
    ADD_CLASS(EmailData.h, NULL, EmailItem, NULL);
    ADD_CLASS(EmailData.h, NULL, EmailData, NULL);

    //GMData.h
    ADD_CLASS(GMData.h, NULL, GMData, NULL);
    ADD_CLASS(GMData.h, NULL, GMServerInfo, NULL);

    //HeroData.h
    ADD_CLASS(HeroData.h, NULL, HeroBaseData, NULL);
    ADD_CLASS(HeroData.h, NULL, HeroExtendData, NULL);
    ADD_CLASS(HeroData.h, NULL, HeroSkillData, SkillData);
    ADD_CLASS(HeroData.h, NULL, HeroData, NULL);
    ADD_CLASS(HeroData.h, NULL, HeroGuidPair, NULL);

    //ItemData.h
    ADD_CLASS(ItemData.h, NULL, MsgItemBaseData, NULL);
    ADD_CLASS(ItemData.h, NULL, ItemData, NULL);
    ADD_CLASS(ItemData.h, NULL, EquipData, NULL);
    ADD_CLASS(ItemData.h, NULL, RuneData, NULL);
    ADD_CLASS(ItemData.h, NULL, HeroDebrisData, NULL);
    ADD_CLASS(ItemData.h, NULL, EquipDebrisData, NULL);
    ADD_CLASS(ItemData.h, NULL, PetDebrisData, NULL);
    ADD_CLASS(ItemData.h, NULL, StoreItemData, NULL);
    ADD_CLASS(ItemData.h, NULL, ExchangeItem, NULL);

    //MonsterData.h
    ADD_CLASS(MonsterData.h, NULL, MonsterBaseData, NULL);
    ADD_CLASS(MonsterData.h, NULL, MonsterExtendData, NULL);
    ADD_CLASS(MonsterData.h, NULL, MonsterSkillData, SkillData);
    ADD_CLASS(MonsterData.h, NULL, MonsterData, NULL);

    //PetData.h
    ADD_CLASS(PetData.h, NULL, PetBaseData, NULL);
    ADD_CLASS(PetData.h, NULL, PetTalentData, NULL);
    ADD_CLASS(PetData.h, NULL, PetSkillData, NULL);
    ADD_CLASS(PetData.h, NULL, PetData, NULL);

    //PlayerData.h
    ADD_CLASS(PlayerData.h, NULL, PlayerBaseData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerProduceData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerHeroData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerItemData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerEquipData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerHeroDebrisData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerEquipDebrisData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerPetDebrisData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerPetData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerRuneData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerHeroEquipData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerEmailData, NULL);
    ADD_CLASS(PlayerData.h, NULL, SceneGetAwardInfo, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerTollgateData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerStoreData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerRankData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerBuddyData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerSystemAwardData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerSummonData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerExchangeData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerActivityData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerAchievementData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerTowerData, NULL);
    ADD_CLASS(PlayerData.h, NULL, PlayerData, NULL);

    //RankData.h
    ADD_CLASS(RankData.h, NULL, RankBaseData, NULL);
    ADD_CLASS(RankData.h, NULL, LevelRankData, NULL);
    ADD_CLASS(RankData.h, NULL, BattleValueRankData, NULL);
    ADD_CLASS(RankData.h, NULL, TollgateRankData, NULL);

    //SecretStoreData.h
    ADD_CLASS(SecretStoreData.h, NULL, SecretStoreItemData, NULL);

    //SkillData.h
    ADD_CLASS(SkillData.h, NULL, SkillItem, NULL);
    ADD_CLASS(SkillData.h, NULL, SkillData, NULL);

    //TollgateData.h
    ADD_CLASS(TollgateData.h, NULL, TollgateDifficultyData, NULL);
    ADD_CLASS(TollgateData.h, NULL, TollgateData, NULL);

    //TowerData.h
    ADD_CLASS(TowerData.h, NULL, TowerData, NULL);
    ADD_CLASS(TowerData.h, NULL, TowerBufferData, NULL);
    ADD_CLASS(TowerData.h, NULL, TowerHeroShowData, NULL);
    ADD_CLASS(TowerData.h, NULL, TowerPlayerShowData, NULL);

    //AchievementTable.h
    ADD_TABLE(AchievementTable.h, NULL, AchievementTemplate, NULL);
    ADD_TABLE(AchievementTable.h, NULL, AchievementTable, NULL);

    //ActivityGoldAwardTable.h
    ADD_TABLE(ActivityGoldAwardTable.h, NULL, ActivityGoldAwardTemplate, NULL);
    ADD_TABLE(ActivityGoldAwardTable.h, NULL, ActivityGoldAwardTable, NULL);

    //ActivitySettingTable.h
    ADD_TABLE(ActivitySettingTable.h, NULL, ActivitySettingTemplate, NULL);
    ADD_TABLE(ActivitySettingTable.h, NULL, ActivitySettingTable, NULL);

    //AwardTable.h
    ADD_TABLE(AwardTable.h, NULL, AwardLine, NULL);
    ADD_TABLE(AwardTable.h, NULL, AwardTemplate, NULL);
    ADD_TABLE(AwardTable.h, NULL, AwardTable, NULL);

    //BufferTable.h
    ADD_TABLE(BufferTable.h, NULL, BufferTemplate, NULL);
    ADD_TABLE(BufferTable.h, NULL, BufferTable, NULL);

    //ConstItemIdTable.h
    ADD_TABLE(ConstItemIdTable.h, NULL, ConstItemIdTemplate, NULL);
    ADD_TABLE(ConstItemIdTable.h, NULL, ConstItemIdTable, NULL);

    //DropItemTable.h
    ADD_TABLE(DropItemTable.h, NULL, DropLine, NULL);
    ADD_TABLE(DropItemTable.h, NULL, DropItemTemplate, NULL);
    ADD_TABLE(DropItemTable.h, NULL, DropItemTable, NULL);

    //EffectTable.h
    ADD_TABLE(EffectTable.h, NULL, EffectTemplate, NULL);
    ADD_TABLE(EffectTable.h, NULL, EffectTable, NULL);

    //EquipDebrisTable.h
    ADD_TABLE(EquipDebrisTable.h, NULL, EquipDebrisTemplate, NULL);
    ADD_TABLE(EquipDebrisTable.h, NULL, EquipDebrisTable, NULL);

    //EquipEnchantTable.h
    ADD_TABLE(EquipEnchantTable.h, NULL, EquipEnchantTemplate, NULL);
    ADD_TABLE(EquipEnchantTable.h, NULL, EquipEnchantTable, NULL);

    //EquipRefineTable.h
    ADD_TABLE(EquipRefineTable.h, NULL, EquipRefineTemplate, NULL);
    ADD_TABLE(EquipRefineTable.h, NULL, EquipRefineTable, NULL);

    //EquipSetTable.h
    ADD_TABLE(EquipSetTable.h, NULL, EquipSetTemplate, NULL);
    ADD_TABLE(EquipSetTable.h, NULL, EquipSetTable, NULL);

    //EquipTable.h
    ADD_TABLE(EquipTable.h, NULL, EquipTemplate, NULL);
    ADD_TABLE(EquipTable.h, NULL, EquipTable, NULL);

    //ExchangeTable.h
    ADD_TABLE(ExchangeTable.h, NULL, ExchangeTemplate, NULL);
    ADD_TABLE(ExchangeTable.h, NULL, ExchangeTable, NULL);

    //FirstNameTable.h
    ADD_TABLE(FirstNameTable.h, NULL, FirstNameTemplate, NULL);
    ADD_TABLE(FirstNameTable.h, NULL, FirstNameTable, NULL);

    //HeroConditionTable.h
    ADD_TABLE(HeroConditionTable.h, NULL, HeroConditionTemplate, NULL);
    ADD_TABLE(HeroConditionTable.h, NULL, HeroConditionTable, NULL);

    //HeroDebrisTable.h
    ADD_TABLE(HeroDebrisTable.h, NULL, HeroDebrisTemplate, NULL);
    ADD_TABLE(HeroDebrisTable.h, NULL, HeroDebrisTable, NULL);

    //HeroGrowTable.h
    ADD_TABLE(HeroGrowTable.h, NULL, HeroGrowTemplate, NULL);
    ADD_TABLE(HeroGrowTable.h, NULL, HeroGrowTable, NULL);

    //HeroTable.h
    ADD_TABLE(HeroTable.h, NULL, HeroTemplateSkill, NULL);
    ADD_TABLE(HeroTable.h, NULL, HeroTemplateNexus, NULL);
    ADD_TABLE(HeroTable.h, NULL, HeroTemplateEvolution, NULL);
    ADD_TABLE(HeroTable.h, NULL, HeroTemplate, NULL);
    ADD_TABLE(HeroTable.h, NULL, HeroTable, NULL);

    //HonorAwardTable.h
    ADD_TABLE(HonorAwardTable.h, NULL, HonorAwardTemplate, NULL);
    ADD_TABLE(HonorAwardTable.h, NULL, HonorAwardTable, NULL);

    //ItemTable.h
    ADD_TABLE(ItemTable.h, NULL, ItemTemplate, NULL);
    ADD_TABLE(ItemTable.h, NULL, ItemTable, NULL);

    //LineupTable.h
    ADD_TABLE(LineupTable.h, NULL, LineupTemplate, NULL);
    ADD_TABLE(LineupTable.h, NULL, LineupTable, NULL);

    //MallTable.h
    ADD_TABLE(MallTable.h, NULL, MallTemplate, NULL);
    ADD_TABLE(MallTable.h, NULL, MallTable, NULL);

    //MonsterTable.h
    ADD_TABLE(MonsterTable.h, NULL, MonsterTemplate, NULL);
    ADD_TABLE(MonsterTable.h, NULL, MonsterTable, NULL);

    //NamePostfixTable.h
    ADD_TABLE(NamePostfixTable.h, NULL, NamePostfixTemplate, NULL);
    ADD_TABLE(NamePostfixTable.h, NULL, NamePostfixTable, NULL);

    //NamePrefixTable.h
    ADD_TABLE(NamePrefixTable.h, NULL, NamePrefixTemplate, NULL);
    ADD_TABLE(NamePrefixTable.h, NULL, NamePrefixTable, NULL);

    //NexusTable.h
    ADD_TABLE(NexusTable.h, NULL, NexusTemplate, NULL);
    ADD_TABLE(NexusTable.h, NULL, NexusTable, NULL);

    //PetAdvanceTable.h
    ADD_TABLE(PetAdvanceTable.h, NULL, PetAdvanceTemplate, NULL);
    ADD_TABLE(PetAdvanceTable.h, NULL, PetAdvanceTable, NULL);

    //PetDebrisTable.h
    ADD_TABLE(PetDebrisTable.h, NULL, PetDebrisTemplate, NULL);
    ADD_TABLE(PetDebrisTable.h, NULL, PetDebrisTable, NULL);

    //PetLevelGrowTable.h
    ADD_TABLE(PetLevelGrowTable.h, NULL, PetLevelGrowTemplate, NULL);
    ADD_TABLE(PetLevelGrowTable.h, NULL, PetLevelGrowTable, NULL);

    //PetStarGrowTable.h
    ADD_TABLE(PetStarGrowTable.h, NULL, PetStarGrowTemplate, NULL);
    ADD_TABLE(PetStarGrowTable.h, NULL, PetStarGrowTable, NULL);

    //PetTable.h
    ADD_TABLE(PetTable.h, NULL, PetTemplateTalent, NULL);
    ADD_TABLE(PetTable.h, NULL, PetTemplate, NULL);
    ADD_TABLE(PetTable.h, NULL, PetTable, NULL);

    //PlayerGrowTable.h
    ADD_TABLE(PlayerGrowTable.h, NULL, PlayerGrowTemplate, NULL);
    ADD_TABLE(PlayerGrowTable.h, NULL, PlayerGrowTable, NULL);

    //ProduceTable.h
    ADD_TABLE(ProduceTable.h, NULL, ProduceTemplate, NULL);
    ADD_TABLE(ProduceTable.h, NULL, ProduceTable, NULL);

    //RandomLineupTable.h
    ADD_TABLE(RandomLineupTable.h, NULL, RandomLineupTemplate, NULL);
    ADD_TABLE(RandomLineupTable.h, NULL, RandomLineupTable, NULL);

    //RefineMasterTable.h
    ADD_TABLE(RefineMasterTable.h, NULL, RefineMasterTemplate, NULL);
    ADD_TABLE(RefineMasterTable.h, NULL, RefineMasterTable, NULL);

    //RobotSetTable.h
    ADD_TABLE(RobotSetTable.h, NULL, RobotLine, NULL);
    ADD_TABLE(RobotSetTable.h, NULL, RobotSetTemplate, NULL);
    ADD_TABLE(RobotSetTable.h, NULL, RobotSetTable, NULL);

    //RuneRefineTable.h
    ADD_TABLE(RuneRefineTable.h, NULL, RuneRefineTemplate, NULL);
    ADD_TABLE(RuneRefineTable.h, NULL, RuneRefineTable, NULL);

    //RuneSetTable.h
    ADD_TABLE(RuneSetTable.h, NULL, RuneSetTemplate, NULL);
    ADD_TABLE(RuneSetTable.h, NULL, RuneSetTable, NULL);

    //RuneTable.h
    ADD_TABLE(RuneTable.h, NULL, RuneTemplate, NULL);
    ADD_TABLE(RuneTable.h, NULL, RuneTable, NULL);

    //SceneTable.h
    ADD_TABLE(SceneTable.h, NULL, SceneTemplate, NULL);
    ADD_TABLE(SceneTable.h, NULL, SceneTable, NULL);

    //SecondNameTable.h
    ADD_TABLE(SecondNameTable.h, NULL, SecondNameTemplate, NULL);
    ADD_TABLE(SecondNameTable.h, NULL, SecondNameTable, NULL);

    //SecretStoreTable.h
    ADD_TABLE(SecretStoreTable.h, NULL, SecretStoreTemplate, NULL);
    ADD_TABLE(SecretStoreTable.h, NULL, SecretStoreTable, NULL);

    //SkillTable.h
    ADD_TABLE(SkillTable.h, NULL, SkillTemplate, NULL);
    ADD_TABLE(SkillTable.h, NULL, SkillTable, NULL);

    //StoreTable.h
    ADD_TABLE(StoreTable.h, NULL, StoreTemplate, NULL);
    ADD_TABLE(StoreTable.h, NULL, StoreTable, NULL);

    //SystemAwardTable.h
    ADD_TABLE(SystemAwardTable.h, NULL, SystemAwardTemplate, NULL);
    ADD_TABLE(SystemAwardTable.h, NULL, SystemAwardTable, NULL);

    //TollgateTable.h
    ADD_TABLE(TollgateTable.h, NULL, DropPackage, NULL);
    ADD_TABLE(TollgateTable.h, NULL, DropItem, NULL);
    ADD_TABLE(TollgateTable.h, NULL, TollgateAwardInfo, NULL);
    ADD_TABLE(TollgateTable.h, NULL, TollgateTemplate, NULL);
    ADD_TABLE(TollgateTable.h, NULL, TollgateTable, NULL);

    //TowerBufferTable.h
    ADD_TABLE(TowerBufferTable.h, NULL, TowerBufferLine, NULL);
    ADD_TABLE(TowerBufferTable.h, NULL, TowerBufferTemplate, NULL);
    ADD_TABLE(TowerBufferTable.h, NULL, TowerBufferTable, NULL);

    //AchievementMsg.h
    ADD_MESSAGE(AchievementMsg.h, NULL, UpdateAchievementStatusNotify, NULL);
    ADD_MESSAGE(AchievementMsg.h, NULL, UpdateAchievementBaseDataNotify, NULL);
    ADD_MESSAGE(AchievementMsg.h, NULL, CompleteAchievementNotify, NULL);
    ADD_MESSAGE(AchievementMsg.h, NULL, GetAchievementAwardReq, NULL);
    ADD_MESSAGE(AchievementMsg.h, NULL, GetAchievementAwardResp, NULL);

    //ActivityMsg.h
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityChangeNotify, NULL);
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityRefreshSettingReq, NULL);
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityRefreshSettingResp, NULL);
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityRuneResultReq, NULL);
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityRuneResultResp, NULL);
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityGoldResultReq, NULL);
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityGoldResultResp, NULL);
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityPetResultReq, NULL);
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityPetResultResp, NULL);
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityEnchantResultReq, NULL);
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityEnchantResultResp, NULL);
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityRuneBattleReq, NULL);
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityRuneBattleResp, NULL);
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityGoldBattleReq, NULL);
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityGoldBattleResp, NULL);
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityPetBattleReq, NULL);
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityPetBattleResp, NULL);
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityEnchantBattleReq, NULL);
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityEnchantBattleResp, NULL);
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityRuneNumberReq, NULL);
    ADD_MESSAGE(ActivityMsg.h, NULL, ActivityRuneNumberResp, NULL);

    //ArenaMsg.h
    ADD_MESSAGE(ArenaMsg.h, NULL, OpenArenaReq, NULL);
    ADD_MESSAGE(ArenaMsg.h, NULL, OpenArenaResp, NULL);
    ADD_MESSAGE(ArenaMsg.h, NULL, OpenArenaRankReq, NULL);
    ADD_MESSAGE(ArenaMsg.h, NULL, OpenArenaRankResp, NULL);
    ADD_MESSAGE(ArenaMsg.h, NULL, StartArenaBattleReq, NULL);
    ADD_MESSAGE(ArenaMsg.h, NULL, StartArenaBattleResp, NULL);
    ADD_MESSAGE(ArenaMsg.h, NULL, RefreshArenaOppReq, NULL);
    ADD_MESSAGE(ArenaMsg.h, NULL, RefreshArenaOppResp, NULL);

    //BuddyMsg.h
    ADD_MESSAGE(BuddyMsg.h, NULL, InviteBuddyReq, NULL);
    ADD_MESSAGE(BuddyMsg.h, NULL, InviteBuddyResp, NULL);
    ADD_MESSAGE(BuddyMsg.h, NULL, SyncInviteBuddyListReq, NULL);
    ADD_MESSAGE(BuddyMsg.h, NULL, SyncInviteBuddyListResp, NULL);
    ADD_MESSAGE(BuddyMsg.h, NULL, AddBuddyReq, NULL);
    ADD_MESSAGE(BuddyMsg.h, NULL, AddBuddyResp, NULL);
    ADD_MESSAGE(BuddyMsg.h, NULL, DelBuddyReq, NULL);
    ADD_MESSAGE(BuddyMsg.h, NULL, DelBuddyResp, NULL);
    ADD_MESSAGE(BuddyMsg.h, NULL, RejectBuddyNotify, NULL);
    ADD_MESSAGE(BuddyMsg.h, NULL, SyncBuddyListReq, NULL);
    ADD_MESSAGE(BuddyMsg.h, NULL, SyncBuddyListResp, NULL);
    ADD_MESSAGE(BuddyMsg.h, NULL, GetRecommendBuddyListReq, NULL);
    ADD_MESSAGE(BuddyMsg.h, NULL, GetRecommendBuddyListResp, NULL);

    //ChatMsg.h
    ADD_MESSAGE(ChatMsg.h, NULL, SyncChatStatusNotify, NULL);
    ADD_MESSAGE(ChatMsg.h, NULL, SendChatMsgNotify, NULL);
    ADD_MESSAGE(ChatMsg.h, NULL, GetChatMsgReq, NULL);
    ADD_MESSAGE(ChatMsg.h, NULL, GetChatMsgResp, NULL);
    ADD_MESSAGE(ChatMsg.h, NULL, ForbiddenChatNotify, NULL);

    //CommonMsg.h
    ADD_MESSAGE(CommonMsg.h, NULL, MsgAckNotify, NULL);
    ADD_MESSAGE(CommonMsg.h, NULL, SyncTimeReq, NULL);
    ADD_MESSAGE(CommonMsg.h, NULL, SyncTimeResp, NULL);
    ADD_MESSAGE(CommonMsg.h, NULL, AccountWasLoginNotify, NULL);
    ADD_MESSAGE(CommonMsg.h, NULL, KeepalivePingReq, NULL);
    ADD_MESSAGE(CommonMsg.h, NULL, KeepalivePingResp, NULL);
    ADD_MESSAGE(CommonMsg.h, NULL, SystemNoticeNotify, NULL);

    //DebugMsg.h
    ADD_MESSAGE(DebugMsg.h, NULL, DebugRemoteLogNotify, NULL);

    //EmailMsg.h
    ADD_MESSAGE(EmailMsg.h, NULL, AddEmailNotify, NULL);
    ADD_MESSAGE(EmailMsg.h, NULL, DelEmailNotify, NULL);
    ADD_MESSAGE(EmailMsg.h, NULL, DelEmailReq, NULL);
    ADD_MESSAGE(EmailMsg.h, NULL, DelEmailResp, NULL);
    ADD_MESSAGE(EmailMsg.h, NULL, TakeEmailItemReq, NULL);
    ADD_MESSAGE(EmailMsg.h, NULL, TakeEmailItemResp, NULL);
    ADD_MESSAGE(EmailMsg.h, NULL, OpenEmailNotify, NULL);

    //EquipDebrisMsg.h
    ADD_MESSAGE(EquipDebrisMsg.h, NULL, UpdateEquipDebrisNotify, NULL);
    ADD_MESSAGE(EquipDebrisMsg.h, NULL, AddEquipDebrisNotify, NULL);
    ADD_MESSAGE(EquipDebrisMsg.h, NULL, DelEquipDebrisNotify, NULL);
    ADD_MESSAGE(EquipDebrisMsg.h, NULL, CombineEquipReq, NULL);
    ADD_MESSAGE(EquipDebrisMsg.h, NULL, CombineEquipResp, NULL);

    //EquipMsg.h
    ADD_MESSAGE(EquipMsg.h, NULL, AddEquipNotify, NULL);
    ADD_MESSAGE(EquipMsg.h, NULL, DelEquipNotify, NULL);
    ADD_MESSAGE(EquipMsg.h, NULL, UseEquipReq, NULL);
    ADD_MESSAGE(EquipMsg.h, NULL, UseEquipResp, NULL);
    ADD_MESSAGE(EquipMsg.h, NULL, UpdateEquipRefineLevelNotify, NULL);
    ADD_MESSAGE(EquipMsg.h, NULL, UpdateEquipEnchantLevelNotify, NULL);
    ADD_MESSAGE(EquipMsg.h, NULL, UpdateEquipPresentPosNotify, NULL);
    ADD_MESSAGE(EquipMsg.h, NULL, RefineEquipReq, NULL);
    ADD_MESSAGE(EquipMsg.h, NULL, RefineEquipResp, NULL);
    ADD_MESSAGE(EquipMsg.h, NULL, EnchantEquipReq, NULL);
    ADD_MESSAGE(EquipMsg.h, NULL, EnchantEquipResp, NULL);
    ADD_MESSAGE(EquipMsg.h, NULL, RebornEquipNotify, NULL);
    ADD_MESSAGE(EquipMsg.h, NULL, SellEquipReq, NULL);
    ADD_MESSAGE(EquipMsg.h, NULL, SellEquipResp, NULL);

    //ExchangeMsg.h
    ADD_MESSAGE(ExchangeMsg.h, NULL, ExchangeBuyItemReq, NULL);
    ADD_MESSAGE(ExchangeMsg.h, NULL, ExchangeBuyItemResp, NULL);
    ADD_MESSAGE(ExchangeMsg.h, NULL, ResetExchangeDateNotify, NULL);
    ADD_MESSAGE(ExchangeMsg.h, NULL, AddExchangeItemNotify, NULL);
    ADD_MESSAGE(ExchangeMsg.h, NULL, UpdateExchangeItemNotify, NULL);
    ADD_MESSAGE(ExchangeMsg.h, NULL, UpdateExchangeStatusNotify, NULL);
    ADD_MESSAGE(ExchangeMsg.h, NULL, GetArenaExchangeDataReq, NULL);
    ADD_MESSAGE(ExchangeMsg.h, NULL, GetArenaExchangeDataResp, NULL);
    ADD_MESSAGE(ExchangeMsg.h, NULL, ReceiveArenaHonorAwardReq, NULL);
    ADD_MESSAGE(ExchangeMsg.h, NULL, ReceiveArenaHonorAwardResp, NULL);

    //GMMsg.h
    ADD_MESSAGE(GMMsg.h, NULL, GMLoginReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMLoginSuccessResp, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMLoginFailResp, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMAccountWasLoginNotify, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMGetPlayerDataReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMGetPlayerDataResp, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMGetPlayerByOnlineReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMGetPlayerByOnlineResp, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMGetPlayerByPlayerGuidReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMGetPlayerByPlayerGuidResp, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMGetPlayerByPlayerNameReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMGetPlayerByPlayerNameResp, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMGetPlayerByAccountGuidReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMGetPlayerByAccountGuidResp, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMGetPlayerByAccountNameReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMGetPlayerByAccountNameResp, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMCommandResp, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMSetPlayerExpReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMSetPlayerLevelReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMSetPlayerEnergyReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMSetPlayerGoldReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMAddItemReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMDelItemReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMAddPetDebrisReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMDelPetDebrisReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMAddEquipReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMDelEquipReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMAddRuneReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMDelRuneReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMAddHeroDebrisReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMDelHeroDebrisReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMAddHeroReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMDelHeroReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMAddPetReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMDelPetReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMSetHeroExpReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMSetHeroAdvancedReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMSetHeroLevelReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMSetHeroStarLevelReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMSetHeroPresentPosReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMSetHeroEquipReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMSetHeroRuneReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMSetEquipRefineReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMSetRuneRefineReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMSetTollgateReq, NULL);
    ADD_MESSAGE(GMMsg.h, NULL, GMSendSystemNoticeReq, NULL);

    //HeroDebrisMsg.h
    ADD_MESSAGE(HeroDebrisMsg.h, NULL, UpdateHeroDebrisNotify, NULL);
    ADD_MESSAGE(HeroDebrisMsg.h, NULL, AddHeroDebrisNotify, NULL);
    ADD_MESSAGE(HeroDebrisMsg.h, NULL, DelHeroDebrisNotify, NULL);
    ADD_MESSAGE(HeroDebrisMsg.h, NULL, CombineHeroReq, NULL);
    ADD_MESSAGE(HeroDebrisMsg.h, NULL, CombineHeroResp, NULL);

    //HeroMsg.h
    ADD_MESSAGE(HeroMsg.h, NULL, AddHeroNotify, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, DelHeroNotify, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, SyncHeroLevelExpNotify, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, SyncHeroAdvancedNotify, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, SyncHeroLevelNotify, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, SyncHeroTemplateIdNotify, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, SyncHeroConsumeGoldNotify, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, AdvancedHeroReq, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, AdvancedHeroResp, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, AwakenHeroReq, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, AwakenHeroResp, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, RefineHeroReq, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, RefineHeroResp, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, RebornHeroNotify, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, SetHeroPresentPosReq, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, SetHeroPresentPosResp, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, SetHeroPresentPosNotify, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, ExchangeHeroPresentPosReq, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, ExchangeHeroPresentPosResp, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, ExchangeHeroPresentPosNotify, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, SellHeroReq, NULL);
    ADD_MESSAGE(HeroMsg.h, NULL, SellHeroResp, NULL);

    //ItemMsg.h
    ADD_MESSAGE(ItemMsg.h, NULL, AddItemNotify, NULL);
    ADD_MESSAGE(ItemMsg.h, NULL, DelItemNotify, NULL);
    ADD_MESSAGE(ItemMsg.h, NULL, UseItemReq, NULL);
    ADD_MESSAGE(ItemMsg.h, NULL, UseItemResp, NULL);
    ADD_MESSAGE(ItemMsg.h, NULL, UpdateItemNotify, NULL);
    ADD_MESSAGE(ItemMsg.h, NULL, SellItemReq, NULL);
    ADD_MESSAGE(ItemMsg.h, NULL, SellItemResp, NULL);

    //LoginMsg.h
    ADD_MESSAGE(LoginMsg.h, NULL, LoginReq, NULL);
    ADD_MESSAGE(LoginMsg.h, NULL, LoginSuccessResp, NULL);
    ADD_MESSAGE(LoginMsg.h, NULL, LoginFailResp, NULL);
    ADD_MESSAGE(LoginMsg.h, NULL, ReconnectReq, NULL);
    ADD_MESSAGE(LoginMsg.h, NULL, ReconnectSuccessResp, NULL);
    ADD_MESSAGE(LoginMsg.h, NULL, ReconnectFailResp, NULL);
    ADD_MESSAGE(LoginMsg.h, NULL, CreatePlayerNameNotify, NULL);
    ADD_MESSAGE(LoginMsg.h, NULL, CreatePlayerReq, NULL);
    ADD_MESSAGE(LoginMsg.h, NULL, CreatePlayerResp, NULL);

    //PetDebrisMsg.h
    ADD_MESSAGE(PetDebrisMsg.h, NULL, UpdatePetDebrisNotify, NULL);
    ADD_MESSAGE(PetDebrisMsg.h, NULL, AddPetDebrisNotify, NULL);
    ADD_MESSAGE(PetDebrisMsg.h, NULL, DelPetDebrisNotify, NULL);
    ADD_MESSAGE(PetDebrisMsg.h, NULL, CombinePetReq, NULL);
    ADD_MESSAGE(PetDebrisMsg.h, NULL, CombinePetResp, NULL);

    //PetMsg.h
    ADD_MESSAGE(PetMsg.h, NULL, AddPetNotify, NULL);
    ADD_MESSAGE(PetMsg.h, NULL, DelPetNotify, NULL);
    ADD_MESSAGE(PetMsg.h, NULL, SetPetPresentPosReq, NULL);
    ADD_MESSAGE(PetMsg.h, NULL, SetPetPresentPosResp, NULL);
    ADD_MESSAGE(PetMsg.h, NULL, SyncPetPresentPosNotify, NULL);
    ADD_MESSAGE(PetMsg.h, NULL, SyncPetLevelNotify, NULL);
    ADD_MESSAGE(PetMsg.h, NULL, SyncPetLevelExpNotify, NULL);
    ADD_MESSAGE(PetMsg.h, NULL, SyncPetStarLevelExpNotify, NULL);
    ADD_MESSAGE(PetMsg.h, NULL, SyncPetTalentNotify, NULL);
    ADD_MESSAGE(PetMsg.h, NULL, SyncPetTemplateIdNotify, NULL);
    ADD_MESSAGE(PetMsg.h, NULL, RefinePetReq, NULL);
    ADD_MESSAGE(PetMsg.h, NULL, RefinePetResp, NULL);
    ADD_MESSAGE(PetMsg.h, NULL, AwakenPetReq, NULL);
    ADD_MESSAGE(PetMsg.h, NULL, AwakenPetResp, NULL);
    ADD_MESSAGE(PetMsg.h, NULL, GetTalentReq, NULL);
    ADD_MESSAGE(PetMsg.h, NULL, GetTalentResp, NULL);
    ADD_MESSAGE(PetMsg.h, NULL, SaveTalentReq, NULL);
    ADD_MESSAGE(PetMsg.h, NULL, SaveTalentResp, NULL);
    ADD_MESSAGE(PetMsg.h, NULL, AdvancePetReq, NULL);
    ADD_MESSAGE(PetMsg.h, NULL, AdvancePetResp, NULL);

    //PlayerMsg.h
    ADD_MESSAGE(PlayerMsg.h, NULL, SyncPlayerLevelNotify, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, SyncPlayerLevelExpNotify, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, SyncPlayerEnergyNotify, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, SyncPlayerGoldNotify, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, SyncPlayerRmbNotify, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, SyncPlayerHeroExpPoolNotify, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, SyncPlayerSoulNotify, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, SyncPlayerHonorNotify, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, UpgreadGoldMineralReq, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, UpgreadGoldMineralResp, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, UpspeedGoldMineralReq, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, UpspeedGoldMineralResp, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, UpgreadDiamondMineralReq, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, UpgreadDiamondMineralResp, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, UpspeedDiamondMineralReq, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, UpspeedDiamondMineralResp, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, UpgreadExpMineralReq, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, UpgreadExpMineralResp, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, UpspeedExpMineralReq, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, UpspeedExpMineralResp, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, GetGoldMineralReq, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, GetGoldMineralResp, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, GetDiamondMineralReq, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, GetDiamondMineralResp, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, ViewPlayerInfoReq, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, ViewPlayerInfoResp, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, SummonHeroReq, NULL);
    ADD_MESSAGE(PlayerMsg.h, NULL, SummonHeroResp, NULL);

    //RankMsg.h
    ADD_MESSAGE(RankMsg.h, NULL, GetRankReq, NULL);
    ADD_MESSAGE(RankMsg.h, NULL, GetRankResp, NULL);
    ADD_MESSAGE(RankMsg.h, NULL, GetRankTopReq, NULL);
    ADD_MESSAGE(RankMsg.h, NULL, GetRankTopResp, NULL);
    ADD_MESSAGE(RankMsg.h, NULL, UpdateRankInfoNotify, NULL);

    //RuneMsg.h
    ADD_MESSAGE(RuneMsg.h, NULL, UpdateRunePresentPosNotify, NULL);
    ADD_MESSAGE(RuneMsg.h, NULL, AddRuneNotify, NULL);
    ADD_MESSAGE(RuneMsg.h, NULL, DelRuneNotify, NULL);
    ADD_MESSAGE(RuneMsg.h, NULL, UseRuneReq, NULL);
    ADD_MESSAGE(RuneMsg.h, NULL, UseRuneResp, NULL);
    ADD_MESSAGE(RuneMsg.h, NULL, SyncRuneRefineLevelNotify, NULL);
    ADD_MESSAGE(RuneMsg.h, NULL, RefineRuneReq, NULL);
    ADD_MESSAGE(RuneMsg.h, NULL, RefineRuneResp, NULL);
    ADD_MESSAGE(RuneMsg.h, NULL, SyncRuneRefineExpNotify, NULL);
    ADD_MESSAGE(RuneMsg.h, NULL, SellRuneReq, NULL);
    ADD_MESSAGE(RuneMsg.h, NULL, SellRuneResp, NULL);
    ADD_MESSAGE(RuneMsg.h, NULL, CombineRuneReq, NULL);
    ADD_MESSAGE(RuneMsg.h, NULL, CombineRuneResp, NULL);

    //SmeltMsg.h
    ADD_MESSAGE(SmeltMsg.h, NULL, SmeltReq, NULL);
    ADD_MESSAGE(SmeltMsg.h, NULL, SmeltResp, NULL);
    ADD_MESSAGE(SmeltMsg.h, NULL, RebornHeroReq, NULL);
    ADD_MESSAGE(SmeltMsg.h, NULL, RebornHeroResp, NULL);
    ADD_MESSAGE(SmeltMsg.h, NULL, RebornEquipReq, NULL);
    ADD_MESSAGE(SmeltMsg.h, NULL, RebornEquipResp, NULL);

    //StoreMsg.h
    ADD_MESSAGE(StoreMsg.h, NULL, StoreBuyItemReq, NULL);
    ADD_MESSAGE(StoreMsg.h, NULL, StoreBuyItemResp, NULL);
    ADD_MESSAGE(StoreMsg.h, NULL, ResetStoreDateNotify, NULL);
    ADD_MESSAGE(StoreMsg.h, NULL, AddStoreItemNotify, NULL);
    ADD_MESSAGE(StoreMsg.h, NULL, UpdateStoreItemNotify, NULL);
    ADD_MESSAGE(StoreMsg.h, NULL, ResetSecretStoreItemNotify, NULL);
    ADD_MESSAGE(StoreMsg.h, NULL, UpdatePlayerSecretStoreDataNotify, NULL);
    ADD_MESSAGE(StoreMsg.h, NULL, ResetSecretStoreItemReq, NULL);
    ADD_MESSAGE(StoreMsg.h, NULL, ResetSecretStoreItemResp, NULL);
    ADD_MESSAGE(StoreMsg.h, NULL, SecretStoreBuyItemReq, NULL);
    ADD_MESSAGE(StoreMsg.h, NULL, SecretStoreBuyItemResp, NULL);

    //SystemAwardMsg.h
    ADD_MESSAGE(SystemAwardMsg.h, NULL, SyncPlayerSystemAwardDataNotify, NULL);
    ADD_MESSAGE(SystemAwardMsg.h, NULL, UpdatePlayerSystemAwardStatusNotify, NULL);
    ADD_MESSAGE(SystemAwardMsg.h, NULL, GetAccumulateLoginAwardReq, NULL);
    ADD_MESSAGE(SystemAwardMsg.h, NULL, GetAccumulateLoginAwardResp, NULL);
    ADD_MESSAGE(SystemAwardMsg.h, NULL, GetContinousLoginAwardReq, NULL);
    ADD_MESSAGE(SystemAwardMsg.h, NULL, GetContinousLoginAwardResp, NULL);
    ADD_MESSAGE(SystemAwardMsg.h, NULL, GetLevelAwardReq, NULL);
    ADD_MESSAGE(SystemAwardMsg.h, NULL, GetLevelAwardResp, NULL);

    //TollgateMsg.h
    ADD_MESSAGE(TollgateMsg.h, NULL, StartTollgateBattleReq, NULL);
    ADD_MESSAGE(TollgateMsg.h, NULL, StartTollgateBattleResp, NULL);
    ADD_MESSAGE(TollgateMsg.h, NULL, StartFiveTollgateSweepReq, NULL);
    ADD_MESSAGE(TollgateMsg.h, NULL, StartFiveTollgateSweepResp, NULL);
    ADD_MESSAGE(TollgateMsg.h, NULL, TollgateBattleResultReq, NULL);
    ADD_MESSAGE(TollgateMsg.h, NULL, TollgateBattleResultResp, NULL);
    ADD_MESSAGE(TollgateMsg.h, NULL, AddTollgateNotify, NULL);
    ADD_MESSAGE(TollgateMsg.h, NULL, UpdateTollgateNotify, NULL);
    ADD_MESSAGE(TollgateMsg.h, NULL, AgainTollgateBattleReq, NULL);
    ADD_MESSAGE(TollgateMsg.h, NULL, AgainTollgateBattleResp, NULL);
    ADD_MESSAGE(TollgateMsg.h, NULL, GetScenePassAwardReq, NULL);
    ADD_MESSAGE(TollgateMsg.h, NULL, GetScenePassAwardResp, NULL);
    ADD_MESSAGE(TollgateMsg.h, NULL, StartStoreHouseBattleReq, NULL);
    ADD_MESSAGE(TollgateMsg.h, NULL, StartStoreHouseBattleResp, NULL);
    ADD_MESSAGE(TollgateMsg.h, NULL, StoreHouseBattleResultReq, NULL);
    ADD_MESSAGE(TollgateMsg.h, NULL, StoreHouseBattleResultResp, NULL);

    //TowerMsg.h
    ADD_MESSAGE(TowerMsg.h, NULL, SyncTowerDataToClientNotify, NULL);
    ADD_MESSAGE(TowerMsg.h, NULL, OpenTowerBufferReq, NULL);
    ADD_MESSAGE(TowerMsg.h, NULL, OpenTowerBufferResp, NULL);
    ADD_MESSAGE(TowerMsg.h, NULL, SelectBufferReq, NULL);
    ADD_MESSAGE(TowerMsg.h, NULL, SelectBufferResp, NULL);
    ADD_MESSAGE(TowerMsg.h, NULL, StartTowerBattleReq, NULL);
    ADD_MESSAGE(TowerMsg.h, NULL, StartTowerBattleResp, NULL);
    ADD_MESSAGE(TowerMsg.h, NULL, TowerBattleResultReq, NULL);
    ADD_MESSAGE(TowerMsg.h, NULL, TowerBattleResultResp, NULL);
    ADD_MESSAGE(TowerMsg.h, NULL, ResetTowerReq, NULL);
    ADD_MESSAGE(TowerMsg.h, NULL, ResetTowerResp, NULL);
    ADD_MESSAGE(TowerMsg.h, NULL, SweepTowerReq, NULL);
    ADD_MESSAGE(TowerMsg.h, NULL, SweepTowerResp, NULL);
    ADD_MESSAGE(TowerMsg.h, NULL, OpenTowerRankReq, NULL);
    ADD_MESSAGE(TowerMsg.h, NULL, OpenTowerRankResp, NULL);

    StreamBuffer streamBuffer;
    for (Map<String, List<String> >::Iter* iter = fileMap.begin(); 
        iter != NULL; iter = fileMap.next(iter))
    {
        streamBuffer.clear(); 
        char timestamp[512] = { 0 };
        TimeUtil::getTimestamp(timestamp, 512, TimeUtil::getTimeMilliSec());

        streamBuffer.write("//-----------------------------------------------------------------------\r\n",
            strlen("//-----------------------------------------------------------------------\r\n"));
        String fileHeadInfo = "//Lynx code generated by c++";
        fileHeadInfo += "\r\n";
        streamBuffer.write(fileHeadInfo.c_str(), fileHeadInfo.length());
        streamBuffer.write("//-----------------------------------------------------------------------\r\n\r\n",
            strlen("//-----------------------------------------------------------------------\r\n\r\n"));
        streamBuffer.write("using UnityEngine;\r\n", strlen("using UnityEngine;\r\n"));
        streamBuffer.write("using System.Collections;\r\n", strlen("using System.Collections;\r\n"));
        streamBuffer.write("using System.Collections.Generic;\r\n", strlen("using System.Collections.Generic;\r\n"));
        streamBuffer.write("using System;\r\n", strlen("using System;\r\n"));
        streamBuffer.write("using System.IO;\r\n", strlen("using System.IO;\r\n"));
        streamBuffer.write("using System.Runtime.InteropServices;\r\n\r\n", strlen("using System.Runtime.InteropServices;\r\n\r\n"));
        streamBuffer.write("//-----------------------------------------------------------------------\r\n",
            strlen("//-----------------------------------------------------------------------\r\n"));
        for (List<String>::Iter* jter = iter->mValue.begin(); 
            jter != NULL; jter = iter->mValue.next(jter)) 
        {
            streamBuffer.write(jter->mValue.c_str(), jter->mValue.length());
            streamBuffer.write("//-----------------------------------------------------------------------\r\n",
                strlen("//-----------------------------------------------------------------------\r\n"));
        }
        String outputFileName = lynxGetMainFileName(iter->mKey);
        String outputFilePath = "../../../Client/Assets/Script/Global/NetStruct/";
        outputFilePath += outputFileName + ".cs";
        FileUtil::saveFile(outputFilePath, streamBuffer);
    }

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s//-----------------------------------------------------------------------\r\n", buf);
    sprintf(buf, "%s//Lynx code generated by c++\r\n", buf);
    sprintf(buf, "%s//-----------------------------------------------------------------------\r\n\r\n", buf);
    sprintf(buf, "%susing UnityEngine;\r\n", buf);
    sprintf(buf, "%susing System.Collections;\r\n", buf);
    sprintf(buf, "%susing System.Collections.Generic;\r\n", buf);
    sprintf(buf, "%susing System;\r\n", buf);
    sprintf(buf, "%susing System.IO;\r\n", buf);
    sprintf(buf, "%susing System.Runtime.InteropServices;\r\n\r\n", buf);
    sprintf(buf, "%spublic class TableManager : MonoBehaviour\r\n", buf);
    sprintf(buf, "%s{\r\n", buf);
    sprintf(buf, "%s    public static bool load()\r\n", buf);
    sprintf(buf, "%s    {\r\n", buf);
    for (StringMap<String>::Iter* iter = tableFileMap.begin(); iter != NULL; iter = tableFileMap.next(iter))
    {
         sprintf(buf, "%s        if (!%s.GetSingleton().LoadFromDbc(\"%s\"))\r\n", buf, lynxGetMainFileName(iter->mKey).c_str(), lynxGetMainFileName(iter->mKey).c_str());
         sprintf(buf, "%s        {\r\n", buf);
         sprintf(buf, "%s            Debug.LogWarning(\"Failed to load %s table.\");\r\n", buf, lynxGetMainFileName(iter->mKey).c_str());
         sprintf(buf, "%s            return false;\r\n", buf);
         sprintf(buf, "%s        }\r\n", buf);
    }
    sprintf(buf, "%s         return true;\r\n", buf);
    sprintf(buf, "%s    }\r\n", buf);
    sprintf(buf, "%s}\r\n", buf);
    String tableFilePath = "../../../Client/Assets/Script/Global/NetStruct/TableManager.cs";
    FileUtil::saveFile(tableFilePath, buf, strlen(buf));

    return 0;
}
