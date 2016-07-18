#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_CHARACTOR_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_CHARACTOR_MAMAGER_H__


#include "MainThread.h"


namespace Lynx
{
	class Player;


	class CharactorManager : public MainThread,public Singleton<CharactorManager>
	{
	public:
		CharactorManager();
		virtual ~CharactorManager();

		bool initial(Player* player);
		void release();

		void buyCharactor(UInt64 charactor);
		std::string convertCharactorDataToJson();

		void equipFashion(UInt32 modelId, UInt64 fashionId);

		void changeCharactor(UInt64 charactor);

	private:

		Player* m_pPlayer;
		List<CharactorData> * m_pCharactorList;
		Map<UInt64, CharactorData*> m_mapCharactor;

	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_CHARACTOR_MAMAGER_H__