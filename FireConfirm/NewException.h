#ifndef __LYNX_GAME_SERVER_GAME_MANAGER_NEW_EXCEPTION_MANAGER_H__
#define __LYNX_GAME_SERVER_GAME_MANAGER_NEW_EXCEPTION_MANAGER_H__

#ifdef _WIN32

#endif

#ifdef __linux__

#include "fcntl.h"
#include "wait.h"
#include "setjmp.h"



#endif



#ifdef _WIN32
#define tryAll __try
#define catchAll __except(EXCEPTION_EXECUTE_HANDLER)

#endif

#ifdef __linux__

#define tryAll if(sigsetjmp(jmpbuf,1) == 0)
#define catchAll else

#endif

#ifdef __linux__

void sig_cl( int );

#endif

#ifdef __linux__

static sigjmp_buf jmpbuf; 

#endif


namespace Lynx
{

	
	class NewException: public Singleton<NewException>
	{
		
	
	public:
		NewException();			
		virtual ~NewException();
		UInt32 init();

		
	};





}

#endif // __LYNX_GAME_SERVER_GAME_MANAGER_NEW_EXCEPTION_MANAGER_H__
