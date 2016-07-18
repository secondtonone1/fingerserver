#include "FileUtil.h"
#include "FileUtilImpl.h"

using namespace Lynx;

char* 
FileUtil::allocBuffer(UInt32 size)
{
#if defined(__ANDROID__) 
	return FileUtilAndroidImpl::allocBuffer(size);
#elif defined(__APPLE__)
    return FileUtilIOSImpl::allocBuffer(size);
#else 
	return FileUtilImpl::allocBuffer(size);
#endif
}

void 
FileUtil::freeBuffer(char* buffer)
{
#if defined(__ANDROID__) 
	FileUtilAndroidImpl::freeBuffer(buffer);
#elif defined(__APPLE__)
    FileUtilIOSImpl::freeBuffer(buffer);
#else 
	FileUtilImpl::freeBuffer(buffer);
#endif
}

void 
FileUtil::findFiles(const String& dir, const String& pattern, StringMap<String>& filePathMap, bool recursive)
{
#if defined(__ANDROID__) 
	FileUtilAndroidImpl::findFiles(dir, pattern, filePathMap, recursive);
#elif defined(__APPLE__)
    FileUtilIOSImpl::findFiles(dir, pattern, filePathMap, recursive);
#else 
	FileUtilImpl::findFiles(dir, pattern, filePathMap, recursive);
#endif
}

void 
FileUtil::findFiles(const String& dir, const String& pattern, List<String>& filePathList, bool recursive)
{
#if defined(__ANDROID__)
	FileUtilAndroidImpl::findFiles(dir, pattern, filePathList, recursive);
#elif defined(__APPLE__)
    FileUtilIOSImpl::findFiles(dir, pattern, filePathList, recursive);
#else
	FileUtilImpl::findFiles(dir, pattern, filePathList, recursive);
#endif
}

bool 
FileUtil::loadFile(const String& filePath, StreamBuffer& stream)
{
#if defined(__ANDROID__)
	return FileUtilAndroidImpl::loadFile(filePath, stream);
#elif defined(__APPLE__)
    return FileUtilIOSImpl::loadFile(filePath, stream);
#else
	return FileUtilImpl::loadFile(filePath, stream);
#endif
}

Int32 
FileUtil::loadFile(const String& filePath, char** buffer)
{
#if defined(__ANDROID__)
	return FileUtilAndroidImpl::loadFile(filePath, buffer);
#elif defined(__APPLE__)
    return FileUtilIOSImpl::loadFile(filePath, buffer);
#else
	return FileUtilImpl::loadFile(filePath, buffer);
#endif
}

Int32 
FileUtil::loadFile(const String& filePath, char* buffer, Int32 size)
{
#if defined(__ANDROID__)
	return FileUtilAndroidImpl::loadFile(filePath, buffer, size);
#elif defined(__APPLE__)
    return FileUtilIOSImpl::loadFile(filePath, buffer, size);
#else
	return FileUtilImpl::loadFile(filePath, buffer, size);
#endif
}

bool 
FileUtil::saveFile(const String& filePath, StreamBuffer& stream)
{
#if defined(__ANDROID__)
	return FileUtilAndroidImpl::saveFile(filePath, stream);
#elif defined(__APPLE__)
    return FileUtilIOSImpl::saveFile(filePath, stream);
#else
	return FileUtilImpl::saveFile(filePath, stream);
#endif
}

bool 
FileUtil::saveFile(const String& filePath, char* buffer, UInt32 bufLen)
{
#if defined(__ANDROID__)
	return FileUtilAndroidImpl::saveFile(filePath, buffer, bufLen);
#elif defined(__APPLE__)
    return FileUtilIOSImpl::saveFile(filePath, buffer, bufLen);
#else
	return FileUtilImpl::saveFile(filePath, buffer, bufLen);
#endif
}

Int32 
FileUtil::getFileSize(const String& filePath)
{
#if defined(__ANDROID__)
	return FileUtilAndroidImpl::getFileSize(filePath);
#elif defined(__APPLE__)
    return FileUtilIOSImpl::getFileSize(filePath);
#else
	return FileUtilImpl::getFileSize(filePath);
#endif
}

void* 
FileUtil::openFile(const String& filePath)
{
#if defined(__ANDROID__)
    return FileUtilAndroidImpl::openFile(filePath);
#elif defined(__APPLE__)
    return FileUtilIOSImpl::openFile(filePath);
#else
    return FileUtilImpl::openFile(filePath);
#endif
}

Int32 
FileUtil::seekFile(void* filePointer, Int32 offset, UInt8 whence)
{
#if defined(__ANDROID__)
    return FileUtilAndroidImpl::seekFile(filePointer, offset, whence);
#elif defined(__APPLE__)
    return FileUtilIOSImpl::seekFile(filePointer, offset, whence);
#else
    return FileUtilImpl::seekFile(filePointer, offset, whence);
#endif
}

Int32 
FileUtil::tellFile(void* filePointer)
{
#if defined(__ANDROID__)
    return FileUtilAndroidImpl::tellFile(filePointer);
#elif defined(__APPLE__)
    return FileUtilIOSImpl::tellFile(filePointer);
#else
    return FileUtilImpl::tellFile(filePointer);
#endif
}

void 
FileUtil::rewindFile(void* filePointer)
{
#if defined(__ANDROID__)
    return FileUtilAndroidImpl::rewindFile(filePointer);
#elif defined(__APPLE__)
    return FileUtilIOSImpl::rewindFile(filePointer);
#else
    return FileUtilImpl::rewindFile(filePointer);
#endif
}

Int32 
FileUtil::readFile(void* filePointer, char* buffer, UInt32 len)
{
#if defined(__ANDROID__)
    return FileUtilAndroidImpl::readFile(filePointer, buffer, len);
#elif defined(__APPLE__)
    return FileUtilIOSImpl::readFile(filePointer, buffer, len);
#else
    return FileUtilImpl::readFile(filePointer, buffer, len);
#endif
}

Int32 
FileUtil::readFile(void* filePointer, StreamBuffer& stream, UInt32 len)
{
#if defined(__ANDROID__)
    return FileUtilAndroidImpl::readFile(filePointer, stream, len);
#elif defined(__APPLE__)
    return FileUtilIOSImpl::readFile(filePointer, stream, len);
#else
    return FileUtilImpl::readFile(filePointer, stream, len);
#endif
}

void 
FileUtil::closeFile(void* filePointer)
{
#if defined(__ANDROID__)
    return FileUtilAndroidImpl::closeFile(filePointer);
#elif defined(__APPLE__)
    return FileUtilIOSImpl::closeFile(filePointer);
#else
    return FileUtilImpl::closeFile(filePointer);
#endif
}