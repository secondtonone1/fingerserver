#include "FileUtilImpl.h"

using namespace Lynx;

#ifdef _WIN32
#	include "FileUtilWin32Impl.h"
#elif defined(__linux__) || defined(__FreeBSD__)
#	include "FileUtilLinuxImpl.h"
#endif

static const bool s_Dirs = false;
static const bool s_IsIgnoreHidden = true;

static bool 
isReservedDir(const char* dir)
{
    return (dir[0] == '.' && (dir[1] == 0 || (dir[1] == '.' && dir[2] == 0)));
}

static bool 
isAbsolutePath(const char* path)
{
#if defined(_WIN32)
    if (isalpha(unsigned char(path[0])) && path[1] == ':')
    {
        return true;
    }
#endif
    return path[0] == '/' || path[0] == '\\';
}

static String 
concatenatePath(const String& base, const String& name)
{
    if (base.empty() || isAbsolutePath(name.data()))
    {
        return name;
    }
    else
    {
        String ret = base;
        ret += '/';
        ret += name;
        return ret;
    }
}

char* 
FileUtilImpl::allocBuffer(UInt32 size)
{
    return (char *)malloc(size);
}

void 
FileUtilImpl::freeBuffer(char* buffer)
{
    free(buffer);
}

void 
FileUtilImpl::findFiles(const String& dir, const String& pattern, StringMap<String>& filePathMap, bool recursive)
{
    long lHandle, res;
    struct _finddata_t tagData;

    int pos1 = pattern.rfind('/');
    int pos2 = pattern.rfind('\\');
    if (pos1 == pattern.npos || ((pos2 != pattern.npos) && (pos1 < pos2)))
    {
        pos1 = pos2;
    }

    String directory;
    if (pos1 != pattern.npos)
    {
        directory = pattern.substr(0, pos1 + 1);
    }

    String fullPattern = concatenatePath(dir, pattern);
    lHandle = _findfirst(fullPattern.c_str(), &tagData);
    res = 0;

    while (lHandle != -1 && res != -1)
    {
        if ((s_Dirs == ((tagData.attrib & _A_SUBDIR) != 0)) &&
            (!s_IsIgnoreHidden || (tagData.attrib & _A_HIDDEN) == 0) &&
            (!s_Dirs || !isReservedDir(tagData.name)))
        {
            String tmpPath = dir;
            tmpPath += '/';
            tmpPath += directory;
            tmpPath += tagData.name;
            filePathMap.insert(tagData.name, tmpPath);
        }
        res = _findnext(lHandle, &tagData);
    }

    if (lHandle != -1)
    {
        _findclose(lHandle);
    }

    if (recursive)
    {
        String baseDir = dir;
        if (!directory.empty())
        {
            baseDir = concatenatePath(dir, directory);
            baseDir.erase(baseDir.length() - 1);
        }
        baseDir += "/*";

        String mask("/");
        if (pos1 != pattern.npos)
        {
            mask += pattern.substr(pos1 + 1);
        }
        else
        {
            mask += pattern;
        }

        lHandle = _findfirst(baseDir.c_str(), &tagData);
        res = 0;
        while (lHandle != -1 &&  res != -1)
        {
            if ((tagData.attrib & _A_SUBDIR) &&
                (!s_IsIgnoreHidden || (tagData.attrib & _A_HIDDEN) == 0) &&
                !isReservedDir(tagData.name))
            {
                baseDir = directory;
                baseDir += tagData.name;
                baseDir += mask;
                findFiles(dir, baseDir, filePathMap);
            }
            res = _findnext(lHandle, &tagData);
        }
        if (lHandle != -1)
        {
            _findclose(lHandle);
        }
    }
}

void 
FileUtilImpl::findFiles(const String& dir, const String& pattern, List<String>& filePathList, bool recursive)
{
    long lHandle, res;
    struct _finddata_t tagData;

    int pos1 = pattern.rfind('/');
    int pos2 = pattern.rfind('\\');
    if (pos1 == pattern.npos || ((pos2 != pattern.npos) && (pos1 < pos2)))
    {
        pos1 = pos2;
    }

    String directory;
    if (pos1 != pattern.npos)
    {
        directory = pattern.substr(0, pos1 + 1);
    }

    String fullPattern = concatenatePath(dir, pattern);
    lHandle = _findfirst(fullPattern.c_str(), &tagData);
    res = 0;

    while (lHandle != -1 && res != -1)
    {
        if ((s_Dirs == ((tagData.attrib & _A_SUBDIR) != 0)) &&
            (!s_IsIgnoreHidden || (tagData.attrib & _A_HIDDEN) == 0) &&
            (!s_Dirs || !isReservedDir(tagData.name)))
        {
            String tmpPath = dir;
            tmpPath += '/';
            tmpPath += directory;
            tmpPath += tagData.name;
            filePathList.insertTail(tmpPath);
        }
        res = _findnext(lHandle, &tagData);
    }

    if (lHandle != -1)
    {
        _findclose(lHandle);
    }

    if (recursive)
    {
        String baseDir = dir;
        if (!directory.empty())
        {
            baseDir = concatenatePath(dir, directory);
            baseDir.erase(baseDir.length() - 1);
        }
        baseDir += "/*";

        String mask("/");
        if (pos1 != pattern.npos)
        {
            mask += pattern.substr(pos1 + 1);
        }
        else
        {
            mask += pattern;
        }

        lHandle = _findfirst(baseDir.c_str(), &tagData);
        res = 0;
        while (lHandle != -1 &&  res != -1)
        {
            if ((tagData.attrib & _A_SUBDIR) &&
                (!s_IsIgnoreHidden || (tagData.attrib & _A_HIDDEN) == 0) &&
                !isReservedDir(tagData.name))
            {
                baseDir = directory;
                baseDir += tagData.name;
                baseDir += mask;
                findFiles(dir, baseDir, filePathList);
            }
            res = _findnext(lHandle, &tagData);
        }
        if (lHandle != -1)
        {
            _findclose(lHandle);
        }
    }
}

bool 
FileUtilImpl::loadFile(const String& filePath, StreamBuffer& stream)
{
    FILE* fp = fopen(filePath.c_str(), "rb");
    if (fp == NULL)
    {
        return false;
    }
    Int32 nBytes = 0;
    while (1)
    {
        StreamBuffer::Node* node = stream.allocNode();
        nBytes = fread((char*)node + sizeof(StreamBuffer::Node), 
            1, stream.mNodeDataSize, fp);
        if (nBytes <= 0)
        {
            stream.deallocNode(node);
            break;
        }
        node->mLen += nBytes;
        stream.pushNode(node);
    }
    fclose(fp);
    return true;
}

Int32 
FileUtilImpl::loadFile(const String& filePath, char** buffer)
{
    FILE* fp = fopen(filePath.c_str(), "rb");
    if (fp == NULL)
    {
        return -1;
    }

    fseek(fp, 0L, SEEK_END);
    UInt32 fileLen = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
	if (fileLen == 0) return 0;
    *buffer = allocBuffer(fileLen + 1);
    if (fread((char*)*buffer, fileLen, 1, fp) != 1)
    {
        freeBuffer(*buffer);
        fclose(fp);
        return -1;
    }
    (*buffer)[fileLen] = 0;
    fclose(fp);
    return fileLen;
}

Int32 
FileUtilImpl::loadFile(const String& filePath, char* buffer, Int32 size)
{
    FILE* fp = fopen(filePath.c_str(), "rb");
    if (fp == NULL)
    {
        return -1;
    }

    fseek(fp, 0L, SEEK_END);
    UInt32 fileLen = ftell(fp);
    if (fileLen != (size - 1))
    {
        fclose(fp);
        return -1;
    }

    fseek(fp, 0L, SEEK_SET);
    if (fread(buffer, fileLen, 1, fp) != 1)
    {
        fclose(fp);
        return -1;
    }

    buffer[fileLen] = 0;
    fclose(fp);
    return fileLen;
}

bool 
FileUtilImpl::saveFile(const String& filePath, StreamBuffer& stream)
{
    FILE* fpw = fopen(filePath.c_str(),"w+");
	if(fpw != NULL)
	{
		fclose(fpw);
	}
	
	FILE* fp = fopen(filePath.c_str(), "wb");
    if (fp == NULL)
    {
        return false;
    }

    rewind(fp);

    Int32 nBytes = 0;
    while (1)
    {
        StreamBuffer::Node* node = stream.popNode();
        if (node == NULL)
        {
            break;
        }
        nBytes = fwrite((char*)node + sizeof(StreamBuffer::Node) + node->mOff,
            1, node->mLen, fp);
        if (nBytes != node->mLen)
        {
            stream.deallocNode(node);
            fclose(fp);
            return false;
        }
        stream.deallocNode(node);
    }
    fclose(fp);
    return true;
}

bool 
FileUtilImpl::saveFile(const String& filePath, char* buffer, UInt32 bufLen)
{
    FILE* fp = fopen(filePath.c_str(), "wb");
    if (fp == NULL)
    {
        return false;
    }
    
    rewind(fp);
    
    Int32 nBytes = fwrite(buffer, 1, bufLen, fp);
    if (nBytes != bufLen)
    {
        fclose(fp);
        return false;
    }
    fclose(fp);
    return true;
}

Int32 
FileUtilImpl::getFileSize(const String& filePath)
{
    FILE* fp = fopen(filePath.c_str(), "r");
    if (fp == NULL)
    {
        return 0;
    }

    fseek(fp, 0L, SEEK_END);
    UInt32 len = ftell(fp);
    fclose(fp);
    return len;
}

void* 
FileUtilImpl::openFile(const String& filePath)
{
    FILE* p = fopen(filePath.c_str(), "rb");
    return (void*)p;
}

Int32 
FileUtilImpl::seekFile(void* filePointer, Int32 offset, UInt8 whence)
{
    return fseek((FILE*)filePointer, offset, whence);
}

Int32 
FileUtilImpl::tellFile(void* filePointer)
{
    return ftell((FILE*)filePointer);
}

void 
FileUtilImpl::rewindFile(void* filePointer)
{
    rewind((FILE*)filePointer);
}

Int32 
FileUtilImpl::readFile(void* filePointer, char* buffer, UInt32 len)
{
   return fread(buffer, 1, len, (FILE*)filePointer);
}

Int32 
FileUtilImpl::readFile(void* filePointer, StreamBuffer& stream, UInt32 len)
{
    UInt32 remainReadLen = len;
    Int32 nBytes = 0;
    while (remainReadLen)
    {
        UInt32 readLen = remainReadLen < stream.mNodeDataSize ? remainReadLen : stream.mNodeDataSize;
        StreamBuffer::Node* node = stream.allocNode();
        nBytes = fread((char*)node + sizeof(StreamBuffer::Node), 
            1, readLen, (FILE*)filePointer);
        if (nBytes <= 0)
        {
            stream.deallocNode(node);
            break;
        }
        node->mLen += nBytes;
        remainReadLen -= nBytes;
        stream.pushNode(node);
    }
    return len - remainReadLen;
}

void 
FileUtilImpl::closeFile(void* filePointer)
{
    fclose((FILE*)filePointer);
}

