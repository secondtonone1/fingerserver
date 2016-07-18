#ifndef __LYNX_GAME_SERVER_REDISMANAGER_H__
#define __LYNX_GAME_SERVER_REDISMANAGER_H__

#include "MainThread.h"
#include "Player.h"
#include "Guild.h"
#ifdef __linux__
#include "hiredis.h"
#endif

namespace Lynx
{
	class RedisManager : public MainThread, public Singleton < RedisManager >
	{
	public:

	        RedisManager();
		virtual ~RedisManager();

		bool initial();
		void release();
               
                void set(std::string key, std::string value);

                std::string get(std::string key);

        private:

                #ifdef __linux__
                redisContext* m_pConnect;
                redisReply* m_pReply;
               #endif
		

	};
} // namespace Lynx


#endif // __LYNX_GAME_SERVER_REDISMANAGER_H__

