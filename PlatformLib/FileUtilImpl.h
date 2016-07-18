#ifndef __LYNX_PLATFORM_LIB_FILE_UTIL_IMPL_H__
#define __LYNX_PLATFORM_LIB_FILE_UTIL_IMPL_H__

#include "Compat.h"
#include "StreamBuffer.h"

namespace Lynx
{
    class FileUtilImpl
    {
    public:
        static char* allocBuffer(UInt32 size);
        static void  freeBuffer(char* buffer);

        static void findFiles(const String& dir, const String& pattern,
            StringMap<String>& filePathMap, bool recursive = true);
        static void findFiles(const String& dir, const String& pattern,
            List<String>& filePathList, bool recursive = true);

        static bool loadFile(const String& filePath, StreamBuffer& stream);
        static Int32 loadFile(const String& filePath, char** buffer);
        static Int32 loadFile(const String& filePath, char* buffer, Int32 size);

        static bool saveFile(const String& filePath, StreamBuffer& stream);
        static bool saveFile(const String& filePath, char* buffer, UInt32 bufLen);
        
        static Int32 getFileSize(const String& filePath);

        /* openFile return file pointer, NULL is failed. */
        static void* openFile(const String& filePath);
        /* seekFile return 0 success */
        static Int32 seekFile(void* filePointer, Int32 offset, UInt8 whence/*0=SEEK_SET,1=SEEK_CUR,2=SEEK_END*/);
        /* tellFile return current file stream pos. */
        static Int32 tellFile(void* filePointer);
        /* rewindFile == seekFile(filePointer, 0, SEEK_SET) */
        static void rewindFile(void* filePointer);
        /* readFile return nBytes for read */
        static Int32 readFile(void* filePointer, char* buffer, UInt32 len);
        /* readFile return stream for read */
        static Int32 readFile(void* filePointer, StreamBuffer& stream, UInt32 len);
        /* closeFile */
        static void closeFile(void* filePointer);
    };
} // namespace Lynx 

#endif // __LYNX_PLATFORM_LIB_FILE_UTIL_IMPL_H__


