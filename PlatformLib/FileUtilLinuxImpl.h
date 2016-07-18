#ifndef __LYNX_PLATFORM_LIB_FILE_UTIL_LINUX_IMPL_H__
#define __LYNX_PLATFORM_LIB_FILE_UTIL_LINUX_IMPL_H__

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define _A_NORMAL 0x00  /* Normalfile-Noread/writerestrictions */
#define _A_RDONLY 0x01  /* read only file */
#define _A_HIDDEN 0x02  /* Hidden file */
#define _A_SYSTEM 0x04  /* System file */
#define _A_ARCH   0x20  /* Archive file */
#define _A_SUBDIR 0x10  /* Subdirectory */

/* Our simplified data entry structure */
struct _finddata_t
{
    char *name;
    int attrib;
    unsigned long size;
};

static int 
_findclose(long id);

static int 
_findnext(long id, struct _finddata_t *data);

static bool 
fnmatch (String& pattern, String name, int dummy)
{
    if (pattern == "*")
    {
        return true;
    }
    if (pattern.substr(0,2) == "*.")
    {
        std::transform(pattern.begin(), pattern.end(), pattern.begin(), ::tolower);
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        String extToFind = pattern.substr(2, pattern.length() - 2);
        if ((name.length() > extToFind.length()) &&
            (extToFind == name.substr(name.length() - extToFind.length(), extToFind.length())))
        {
            return true; // match
        }
        else
        {
            return false; // don't match
        }
    }
    // 处理全名字情况, 暂时不处理xxx.*情况 
    if (pattern == name)
    {
        return true;
    }
    else
    {
        return false;
    }
}

struct _find_search_t
{
    char *pattern;
    char *curfn;
    char *directory;
    int dirlen;
    DIR *dirfd;
};
        
static long 
_findfirst(const char *pattern, struct _finddata_t *data)
{
    _find_search_t *fs = new _find_search_t;
    fs->curfn = NULL;
    fs->pattern = NULL;

    // Separate the mask from directory name
    const char *mask = strrchr (pattern, '/');
    if (mask)
    {
        fs->dirlen = mask - pattern;
        mask++;
        fs->directory = (char *)malloc (fs->dirlen + 1);
        memcpy (fs->directory, pattern, fs->dirlen);
        fs->directory [fs->dirlen] = 0;
    }
    else
    {
        mask = pattern;
        fs->directory = strdup (".");
        fs->dirlen = 1;
    }

    fs->dirfd = opendir (fs->directory);
    if (!fs->dirfd)
    {
        _findclose ((long)fs);
        return -1;
    }

    /* Hack for "*.*" -> "*' from DOS/Windows */
    if (strcmp (mask, "*.*") == 0)
        mask += 2;
    fs->pattern = strdup (mask);

    /* get the first entry */
    if (_findnext ((long)fs, data) < 0)
    {
        _findclose ((long)fs);
        return -1;
    }
    return (long)fs;
}

int 
_findnext(long id, struct _finddata_t *data)
{
    _find_search_t *fs = (_find_search_t *)id;

    /* Loop until we run out of entries or find the next one */
    dirent *entry;
    for (;;)
    {
        if (!(entry = readdir (fs->dirfd)))
            return -1;

        /* See if the filename matches our pattern */
        String pattern = fs->pattern;
        String d_name = entry->d_name;
        if (fnmatch (pattern, d_name, 0))
            break;
    }

    if (fs->curfn)
        free (fs->curfn);
    data->name = fs->curfn = strdup (entry->d_name);

    size_t namelen = strlen (entry->d_name);
    UInt32 xfnLen = fs->dirlen + 1 + namelen + 1;
    char *xfn = new char [xfnLen];
    snprintf (xfn, xfnLen, "%s/%s", fs->directory, entry->d_name);

    /* stat the file to get if it's a subdir and to find its length */
    struct stat stat_buf;
    if (stat (xfn, &stat_buf))
    {
        // Hmm strange, imitate a zero-length file then
        data->attrib = _A_NORMAL;
        data->size = 0;
    }
    else
    {
        if (S_ISDIR(stat_buf.st_mode))
            data->attrib = _A_SUBDIR;
        else
            /* Default type to a normal file */
            data->attrib = _A_NORMAL;

        data->size = stat_buf.st_size;
    }

    delete [] xfn;

    /* Files starting with a dot are hidden files in Unix */
    if (data->name [0] == '.')
        data->attrib |= _A_HIDDEN;

    return 0;
}

int 
_findclose(long id)
{
    int ret;
    _find_search_t *fs = (_find_search_t *)id;
    
    ret = fs->dirfd ? closedir (fs->dirfd) : 0;
    free (fs->pattern);
    free (fs->directory);
    if (fs->curfn)
        free (fs->curfn);
    delete fs;
    return ret;
}

#endif // __LYNX_PLATFORM_LIB_FILE_UTIL_LINUX_IMPL_H__
