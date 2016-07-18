#ifndef __LYNX_GAME_SERVER_OPTION_PARSER_H__
#define __LYNX_GAME_SERVER_OPTION_PARSER_H__

#include <fcntl.h>
#if defined (_WIN32)
# include <io.h>
#elif defined (__linux__) || defined (__FreeBSD__)
# include <unistd.h>
# include <errno.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
#endif

#include "PlatformLib.h"

#define CMDARG_MAX_LEN  128
#define NEXT_CHAR(str)  (char)(*(str+1))

namespace Lynx
{
    class OptionParser : public Singleton<OptionParser>
    {
    public:
        OptionParser() : mOptArg(NULL), mOptInd(1), mOptErr(1), mOptIndex(NULL)
                       , mConfigFile("gameserver.conf"), mIsDaemonize(false) {}
        virtual ~OptionParser() {}

        const String& getConfigFile() const { return mConfigFile; }
        bool isDaemonize() const { return mIsDaemonize; }

        bool parser(Int32 argc, char* argv[])
        {
            Int32 c;
            if (argc == 1) return true;

            while ((c = getOption(argc, argv, "nc:")) != -1)
            {
                switch(c)
                {
                case 'n':
                    mIsDaemonize = false;
                    break;
                case 'c':
                    if (strlen(getOptArg()) > CMDARG_MAX_LEN)
                        return false;
                    mConfigFile = getOptArg();
                default:
                    printf("Unknown input argument [%c].\n", c);
                    return false;
                }
            }

            if (getOptInd() < argc)
            {
                printf("Unexpected argument %s\n", argv[getOptInd()]);
                return false;
            }
            return true;
        }

        Int32 getOption(Int32 argc, char* argv[], const char* opString)
        {
            mOptIndex = NULL;
            char *argStr = NULL, *optStr;    

            if (mOptIndex != NULL) 
            { 
                if (*(++mOptIndex))  
                    argStr = mOptIndex; 
            } 

            if (argStr == NULL) 
            { 
                if (mOptInd >= argc) 
                { 
                    mOptIndex = NULL;   
                    return EOF; 
                } 

                argStr = argv[mOptInd++]; 

                if ((*argStr != '/') && (*argStr != '-')) 
                { 
                    --mOptInd;       
                    mOptArg = mOptIndex = NULL; 
                    return EOF; 
                } 

                if ((strcmp(argStr, "-") == 0) || (strcmp(argStr, "--") == 0)) 
                { 
                    mOptArg = mOptIndex = NULL; 
                    return EOF; 
                } 

                argStr++; 
            } 

            if (*argStr == ':') 
            { 
                return (mOptErr ? (Int32)'?' : (Int32)':'); 
            } 

            if ((optStr = (char *)strchr(opString, *argStr)) == 0) 
            { 
                mOptArg = mOptIndex = NULL; 
                return (mOptErr ? (Int32)'?' : (Int32)*argStr); 
            } 

            if (NEXT_CHAR(optStr) == ':') 
            { 
                if (NEXT_CHAR(argStr) != '\0') 
                { 
                    mOptArg = &argStr[1]; 
                } 
                else 
                { 
                    if (mOptInd < argc) 
                    {
                        mOptArg = argv[mOptInd++]; 
                    } 
                    else 
                    { 
                        mOptArg = NULL; 
                        return (mOptErr ? (Int32)'?' : (Int32)*argStr); 
                    } 
                } 
                mOptIndex = NULL;
            } 
            else 
            { 
                mOptArg = NULL;
                mOptIndex = argStr; 
            } 
            return (Int32)*argStr; 
        }

        char* getOptArg() { return mOptArg; }
        Int32 getOptInd() { return mOptInd; }
        Int32 getOptErr() { return mOptErr; }

        Int32 daemonize(const char* name)
        {
#if defined (_WIN32)
            // Nothing to do
#elif defined (__linux__) || defined (__FreeBSD__)
            pid_t child;
            FILE* f;
            char pidFile[80];

            child = fork();
            if (!child)
            {
                child = fork();
                if (child) exit(0);
            }
            else
            {
                // The master process collects the zombie 
                // process of forking the child daemon and exits 
//#if !defined (__FreeBSD__)
//                waitpid(child, NULL, 0);
//                return 1;
//#else
                exit(0);
//#endif
            }

            // Get rid of controlling terminal.
            setsid();

            // Have to close the original standard I/O 
            // streams and create our own standard I/O streams,
            // otherwise the parment process cannot exit cleanly 
            umask(0);
            close(0);
            close(1);
            close(2);
            open("/dev/null", O_RDWR, 0);
            dup2(0, 1);
            dup2(0, 2);

            snprintf(pidFile, sizeof(pidFile), "/var/run/%s.pid", name);
            f = fopen(pidFile, "w");
            if (!f)
            {
                fprintf(stderr, "failed to open %s: %s", pidFile, strerror(errno));
            }
            else
            {
                fprintf(f, "%u\n", getpid());
                fclose(f);
            }
#endif
            return 0;
        }

    private:
        String                 mConfigFile;
        bool                   mIsDaemonize;

        char*                  mOptArg;
        Int32                  mOptInd;
        Int32                  mOptErr;
        char*                  mOptIndex;
    };
} // namespace KGame

#endif // __LYNX_GAME_SERVER_OPTION_PARSER_H__


