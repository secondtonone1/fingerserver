// #include "FireConfirmManager.h"
 #include "../LogicSystem.h"
// #include "StringConverter.h"
// #include "../PlatformLib/SerializeType.h"
// // #include "../CommonLib/PlayerData.h"
// #include "../PersistSystem.h"
// #include "../PlatformLib/Utilex.h"
// #include "../CommonLib/CopyFinishTime.h"
// #include "Gift.h"
#include "NewException.h"

using namespace Lynx;

NewException::NewException()
{
}

NewException:: ~NewException()
{

}
UInt32 NewException::init()
{
#ifdef __linux__
	signal(SIGFPE,sig_cl);
#endif
	return 1;
}
#ifdef __linux__
void sig_cl(int sig)
{
	LOG_WARN("sig %u",sig);
	siglongjmp(jmpbuf, 1);
}
#endif



