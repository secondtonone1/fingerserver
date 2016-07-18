#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_SKILL_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_SKILL_MAMAGER_H__


#include "MainThread.h"


namespace Lynx
{
	class Player;
	

	class SkillManager : public MainThread,  public Singleton<SkillManager>
	{
	public:
		SkillManager();
		virtual ~SkillManager();

		bool initial(Player* player);
		void release();

		std::string convertSkillDataToJson();

		void changeCharactorSkill(UInt64 charactorId);
		
		void skillLevelUp(UInt64 skillId);

		void skillPositonSet(Map<UInt64,UInt32> skillEquipMap);

		List<SkillData> * getSkillList(void);

		Vector<SkillData *> &getSkillVec(void);

		List<SkillData> activeSkill(void);
		
	private:
		void skillMapInitial();
		
		Player* m_pPlayer;
		
		List<SkillData> * m_pListSkillDatas;

		Map<UInt64, SkillData*> m_mapIDToSkillData;

		Vector<SkillData*> m_pVecSkillDatas;

	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_SKILL_MAMAGER_H__
