#include "GameRobot.h"
#include "GetOption.h"
#include "StrategyManager.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace Lynx;

#define LYNX_CMDARG_MAX_LEN 256

const char* sHelpInfo=
"\n This is game robot program for test lynx project. \n\n"
"Usage: gamerobot [OPTION]\n"
"-h display help infomation\n"
"-d specify log level [0=>debug, 1=>info, 2=>warn, 3=>error], default 0\n"
"-o specify log output file [0=>stderr, 1=>file, 2=>csv] default 0\n"
"-s specify login server address [ip:port]\n"
"-u specify account info [prefix(1-n):password]\n"
"-t specify strategy type [chat_1000|battle_5000]\n"
"Report bugs to <hailong.xia@lynx.com.cn>\n";

static UInt32 sRobotCount = 0;
static UInt32 sLogLevel = 0;
static Map<String, UInt32> sStrategyMap;
static char sLoginIp[LYNX_CMDARG_MAX_LEN] = { 0 };
static char sAccountPrefix[LYNX_CMDARG_MAX_LEN] = { 0 };
static char sAccountSuffix[LYNX_CMDARG_MAX_LEN] = { 0 };
static UInt32 sLoginPort = 0;
static UInt32 sPrintType = 0;
static Map<UInt32, std::pair<std::string, std::string> > sAccountInfoMap;
const static UInt32 sMaxThreadCount = 256;
static UInt32 sThreadCount = 0;
static UInt32 sAccountIdArray[sMaxThreadCount];
//static UInt32 sThreadShutdownArray[sMaxThreadCount];

static Int32
parserOptions(Int32 argc, char* argv[])
{
    Int32 c;
    extern char* optarg;
    extern Int32 optind, opterr;

    opterr = 0;

    while ((c = getopt(argc, argv, "hd:o:s:u:t:")) != -1)
    {
        switch (c)
        {
        case 'h':
            printf(sHelpInfo);
            return 1;
        case 'd':
            sLogLevel = lynxAtoi<UInt32>(optarg);
            printf("--LogLevel: %u\n", sLogLevel);
            break;
        case 'o':
            sPrintType = lynxAtoi<UInt32>(optarg);
            printf("--PrintType: %u\n", sPrintType);
            break;
        case 's':
            {
                char loginAddress[LYNX_CMDARG_MAX_LEN] = { 0 };
                if (strlen(optarg) > LYNX_CMDARG_MAX_LEN) return -1;
                strcpy(loginAddress, optarg);
                char* pos = strstr(loginAddress, ":");
                if (pos == NULL) return -1;
                strncpy(sLoginIp, loginAddress, pos - loginAddress);
                sLoginPort = lynxAtoi<UInt32>(++pos);
                printf("--LoginIp: %s\n", sLoginIp);
                printf("--LoginPort: %u\n", sLoginPort);
            }
            break;
        case 'u':
            {
                char userInfo[LYNX_CMDARG_MAX_LEN] = { 0 };
                if (strlen(optarg) > LYNX_CMDARG_MAX_LEN) return -1;
                strcpy(userInfo, optarg);

                char username[LYNX_CMDARG_MAX_LEN] = { 0 };
                char password[LYNX_CMDARG_MAX_LEN] = { 0 };

                char* pos = strstr(userInfo, "(");
                if (pos == NULL) // Single account
                {
                    char* pos1 = strstr(userInfo, ")");
                    if (pos1 != NULL) return -1;

                    pos1 = strstr(userInfo, ":");
                    if (pos1 == NULL) return -1;
                    strncpy(username, userInfo, pos1 - userInfo);
                    strcpy(password, ++pos1);
                    sAccountInfoMap.insert(0, std::make_pair(username, password));
                    
                    printf("--account: %s\n", username);
                    printf("--password: %s\n", password);
                }
                else // Multi account
                {
                    strncpy(sAccountPrefix, userInfo, pos - userInfo);
                    char* pos1 = strstr(pos, "-");
                    UInt32 startNumber = lynxAtoi<UInt32>(++pos);
                    UInt32 endNumber = lynxAtoi<UInt32>(++pos1);

                    if (endNumber < startNumber) return -1;

                    char* pos2 = strstr(userInfo, ")");
                    if (pos2 == NULL) return -1;
                    char* pos3 = strstr(userInfo, ":");
                    if (pos3 == NULL) return -1;
                    strncpy(sAccountSuffix, pos2 + 1, pos3 - pos2 - 1);
                    strcpy(password, ++pos3);
                    
                    for (Int32 i = 0; i <= endNumber - startNumber; ++i)
                    {
                        snprintf(username, sizeof(username), "%s%d%s", sAccountPrefix, i + startNumber, sAccountSuffix);
                        sAccountInfoMap.insert(i, std::make_pair(username, password));
                    }
                    printf("--account:%s%d%s-%s%d%s\n", sAccountPrefix, startNumber, sAccountSuffix,
                            sAccountPrefix, endNumber, sAccountSuffix);
                }
            }
            break;
        case 't':
            {
                if (strlen(optarg) > LYNX_CMDARG_MAX_LEN) return -1;
                Vector<String> v;
                lynxStrSplit(optarg, "|", v, true);
                if (v.empty())
                {
                    printf("Invalid strategy option"); 
                    return -1;
                }

                for (Vector<String>::iterator it = v.begin();
                    it != v.end(); ++it)
                {
                    Vector<String> vv;
                    lynxStrSplit(*it, "_", vv, true);
                    if (vv.size() != 2)
                    {
                        printf("Invalid strategy option");
                        return -1;
                    }
                    sStrategyMap.insert(vv[0], lynxAtoi<UInt32>(vv[1].c_str()));
                }   
            }
            break;
        default:
            printf("Unknown input args [%c]\n", c);
            return -1;
        }
    }

    if (optind < argc)
    {
        printf("unexpected args %s\n", argv[optind]);
        return -1;
    }
    return 0;
}
            
#ifdef _WIN32
DWORD WINAPI threadFunc(LPVOID args)
#else
void* threadFunc(void* args)
#endif
{
    Int32 i = *(Int32*)args;
    
    char tmpBuf[256];
    Map<UInt32, std::pair<std::string, std::string> >::Iter* iter = sAccountInfoMap.find(i);
    snprintf(tmpBuf, sizeof(tmpBuf), "%s.log", iter->mValue.first.c_str());
    if (sPrintType == 1)
    {
        LOG_INITIAL(Logger::OUTPUT_FILE, true, false);
        LOG_SET_OUTPUT_FILE(tmpBuf);
    }
    else if (sPrintType == 2)
    {
        LOG_INITIAL(Logger::OUTPUT_STDERR, false, false);
        LOG_SET_OUTPUT_FILE(NULL);
    }
    else if (sPrintType == 0)
    {
        LOG_INITIAL(Logger::OUTPUT_STDERR, false, false);
        LOG_SET_OUTPUT_FILE(NULL);
    }

    LOG_SET_SILENCE_LEVEL(sLogLevel);

    IOService ioService(IOSERVICE_SELECT_DRIVER);
    GameRobot* robot = XNEW(GameRobot)(&ioService);
    if (!robot->initial(iter->mValue.first.c_str(), iter->mValue.second.c_str(), sLoginIp, sLoginPort))
    {
        LOG_WARN("Failed to initial game robot.");
        return -1;
    }

    StrategyManager strategyManager(robot);

    for (Map<String, UInt32>::Iter* iter = sStrategyMap.begin();
        iter != NULL; iter = sStrategyMap.next(iter))
    {
        strategyManager.addStrategy(iter->mKey, iter->mValue);
    }

    while (1)
    {
        ioService.poll();
        strategyManager.update(TimeUtil::getTimeMilliSec());
        TimeUtil::sleep(30);
    }

    robot->release();
    
    LOG_RELEASE();

    return 0;
}

int main(int argc, char** argv)
{
    printf("%s start ... \n", argv[0]);

    Int32 ret = parserOptions(argc, argv);
    if (ret)
    {
        if (ret == -1) printf(sHelpInfo);
        return -1;
    }
    if (sLoginPort <= 0)
    {
        printf("Invalid login port %d\n", sLoginPort);
        return -1;
    }

    if (strlen(sLoginIp) == 0)
    {
        printf("Invalid login ip \n");
        return -1;
    }

    LOG_INITIAL(Logger::OUTPUT_STDERR, false, false);
    LOG_SET_SILENCE_LEVEL(0);
    
    LOG_INFO("GameRobot started...");

    if (!ResourceGroupManager::getSingleton().initial("ResourceConfig.xml", true))
    {
        LOG_ERROR("Failed to initial resouce group manager.");
        return -1;
    }

    if (!TableManager::getSingleton().load())
    {
        LOG_ERROR("Failed to load template table.");
        return -1;
    }

    Random::getSingleton().setSeed(0);

    sThreadCount = sAccountInfoMap.size();
    if (sThreadCount > sMaxThreadCount)
    {
        LOG_ERROR("ThreadCount %u > MaxThreadCount %u", sThreadCount, sMaxThreadCount);
        return -1;
    }
	
#ifndef _WIN32
    pthread_t tid;
#endif
    
    for (Int32 i = 0; i < sThreadCount; ++i)
    {
        TimeUtil::sleep(100);
        sAccountIdArray[i] = i;
#ifdef _WIN32
        if (!CreateThread(NULL, 0, threadFunc, &sAccountIdArray[i], 0, 0))
#else
        if (pthread_create(&tid, NULL, threadFunc, &sAccountIdArray[i]))
#endif
        {
            LOG_WARN("Failed to create thread.");
            return -1;
        }
    }

    while (1)
    {
        bool isRunning = false;
        for (Int32 i = 0; i < sThreadCount; i++)
        {
            if (sAccountIdArray[i] != -1)
            {
                isRunning = true;
                break;
            }
        }

        if (!isRunning)
        {
            break;
        }
        TimeUtil::sleep(500);
    }

    LOG_RELEASE();
    return 0;
}

