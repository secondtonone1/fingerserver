#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_RHYME_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_RHYME_MAMAGER_H__


#include "MainThread.h"

namespace Lynx
{
	class Player;
	
	class RhymeManager : public MainThread,  public Singleton<RhymeManager>
	{
	public:
		RhymeManager();
		virtual ~RhymeManager();

		bool initial(Player* player);
		void release();

		//韵文强化
		void onEnhanceOnce();

		void onEnhanceTen();

		//穴道激活
		void activeRhymeAcupoint(UInt64 acupointIndex,UInt32 step ,UInt32 level);

		//韵功装备
		void activeSkillSet(UInt32 rhymeSkillIndex);

		//韵文升阶
		void rhymeStep();
	

		UInt32 getRhymeSkillEquip(void);
		
		std::string convertRhymeDataToJson();

		void addRhymeSoul(UInt32 count);

		void addRhymeGrass(UInt32 grassType, UInt32 count);

		UInt32 getRhymeGrass(UInt32 grassType);

		void delRhymeSoul(UInt32 count);

		UInt32 getRhymeSoul();
		
	private:

		bool checkGrassTypeCount(UInt32 type, UInt32 count);
			
		bool delRhymeGrass(UInt32 grassType, UInt32 count);

		
		Player * m_pPlayer;

		PlayerRhymeData * m_pRhymeData;

	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_RHYME_MAMAGER_H__
