#ifndef __LYNX_GAME_ROBOT_GET_OPTION_H__
#define __LYNX_GAME_ROBOT_GET_OPTION_H__

#ifndef _WIN32
# include <unistd.h>
#else

#ifdef __cplusplus
extern "C" {
#endif

int getopt(int argc, char *argv[], char *opstring);

#ifdef __cplusplus
}
#endif
#endif


#endif // __LYNX_GAME_ROBOT_GET_OPTION_H__