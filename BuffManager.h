#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_BUFF_MAMAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_BUFF_MAMAGER_H__


#include "MainThread.h"


namespace Lynx
{
	class Player;

	struct BuffType
	{
		UInt32 buffBigType;
		UInt32 buffSubType;
	};

	struct Buffer
	{
		UInt32 buffId;
		UInt32 bufferCD;
		BuffTemplate bufferData;
		bool timeOver;

		


// 			self.buffId = data.id 
// 			self.buffCD = data.cd
// 
// 			self.buffData = nil
// 			self.timeover = false
// 
// 			self.server = server
// 			self.logicObj = nil
	};


	class BuffManager : public MainThread ,public Singleton<BuffManager>
	{
	public:
		BuffManager();
		virtual ~BuffManager();

		bool initial(Player* player);
		void release();

		std::string convertBuffDataToJson();


		BuffType gettypes(BuffTemplate bufferData);

		void update(Buffer buffer);

		void updateLogic(Buffer buffer);
	
		void reset(Buffer buffer);

		String getBuffParam(Buffer buffer,UInt32 id);
	

		void onBuffStartAffect( Buffer buffer );

		void onBuffInterrupt(  );

		void onBuffEffectChange(  );

		void onBuffEnd(  );

		bool  checkBuffExistById(UInt32 buffID);



	private:
		void buffMapInitial();

		Player* m_pPlayer;

		List<BufferData> *m_pListBuffers;

		Map<UInt64, BufferData*> m_mapIDToBuffData;

	};
};


#endif// __LYNX_GAME_SERVER_GAME_MANAGER_BUFF_MAMAGER_H__