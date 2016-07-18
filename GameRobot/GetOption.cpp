#ifdef _WIN32
# include <stdio.h>
# include <string.h>
# define _next_char(str)  (char)(*(str+1)) 
# include "GetOption.h"

char *optarg = NULL; 
int optind = 1, opterr = 1;  
 
int 
getopt(int argc, char *argv[], char *opstring) 
{ 
    static char *index = NULL;
    char *argstr = NULL, *optstr;    
 
    if (index != NULL) 
    { 
        if (*(++index))  
            argstr = index; 
    } 
 
    if (argstr == NULL) 
    { 
        if (optind >= argc) 
        { 
            index = NULL;   
            return EOF; 
        } 

        argstr = argv[optind++]; 

        if ((*argstr != '/') && (*argstr != '-')) 
        { 
            --optind;       
            optarg = index = NULL; 
            return EOF; 
        } 

        if ((strcmp(argstr, "-") == 0) || (strcmp(argstr, "--") == 0)) 
        { 
            optarg = index = NULL; 
            return EOF; 
        } 

        argstr++; 
    } 

    if (*argstr == ':') 
    { 
        return (opterr ? (int)'?' : (int)':'); 
    } 

    if ((optstr = strchr(opstring, *argstr)) == 0) 
    { 
        optarg = index = NULL; 
        return (opterr ? (int)'?' : (int)*argstr); 
    } 
         
    if (_next_char(optstr) == ':') 
    { 
        if (_next_char(argstr) != '\0') 
        { 
            optarg = &argstr[1]; 
        } 
        else 
        { 
            if (optind < argc) 
            {
                optarg = argv[optind++]; 
            } 
            else 
            { 
                optarg = NULL; 
                return (opterr ? (int)'?' : (int)*argstr); 
            } 
        } 
        index = NULL;
    } 
    else 
    { 
        optarg = NULL;
        index = argstr; 
    } 
    return (int)*argstr; 
}
#endif 