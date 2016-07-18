#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_PET_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_PET_MAMAGER_H__

#include "MainThread.h"

namespace Lynx
{
    class Player;
    class PetManager : public MainThread
    {
    public:
        PetManager();
        virtual ~PetManager();

        bool initial(Player* player);
        void release();
        void update(const UInt64& accTime);
        void calculatePetData(const Guid& petId);

       // const PetData* createPet(const TemplateId& templateId, bool promp = true);
        bool destroyPet(Guid guid);
       // const PetData* getPetDataByGuid(const Guid& guid) const;
      //  const PetData* getPresentPet() const;

        // 吞噬 改成升级了！
        ErrorId awakenPet(const Guid& petId, const List<Guid>& petList, const List<Guid>& petDebrisList,
            UInt32& upLevel, TemplateId& newTemplateId);
        // 单次喂养
        ErrorId refinePet(const Guid& petId, UInt32& upLevelCount, const Guid& itemId);
        // 一键喂养
        ErrorId refinePet(const Guid& petId, UInt32& upLevelCount);
        // 普通领悟
     //   ErrorId getCommonTalent(const Guid& petId, PetTalentData& petTalentData);
        // 高级领悟
      //  ErrorId getAdvanceTalent(const Guid& petId, PetTalentData& petTalentData);
        // 保存天赋
        ErrorId saveTalent(const Guid& petId);
        // 取消天赋
        ErrorId cancelTalent(const Guid& petId);
        // 设置出场
        ErrorId setPetPresentPos(const Guid& petId, UInt8 pos);
        // 宠物进阶
        ErrorId advancePet(const Guid& petId);

    private:
        // 给宠物添加等级经验，触发升级
        ErrorId addLevelExp(const Guid& petId, UInt32& upLevelCount, UInt32 incLevelExp);
        // 给宠物添加星级经验， 触发升星
        ErrorId addStarLevelExp(const Guid& petId, TemplateId& templateId, UInt32 incStarLevelExp);


    public:
        void increasePetLevel(const Guid& petId, UInt16 offset);
        void reducePetLevel(const Guid& petId, UInt16 offset);
        void increasePetLevelExp(const Guid& petId, UInt32 offset);
        void reducePetLevelExp(const Guid& petId, UInt32 offset);
        void increasePetStarLevelExp(const Guid& petId, UInt32 offset);
        void reducePetStarLevelExp(const Guid& petId, UInt32 offset);

    private:
        Player* mPlayer;
     //   PlayerPetData* mPlayerPetData;
      //  Map<Guid, PetData*> mPlayerPetDataMap;
     //   PetData* mPresentPetData;
     //   Map<Guid, PetTalentData> mPetTalentDataMap;
    };
};

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_PET_MAMAGER_H__
