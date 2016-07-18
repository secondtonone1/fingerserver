#ifdef __ANDROID__
#include "FileUtilAndroidImpl.h"
#include "Utilex.h"
#include "TimeUtil.h"

using namespace Lynx;

AAssetManager* FileUtilAndroidImpl::mAssetMgr = NULL;

char* 
FileUtilAndroidImpl::allocBuffer(UInt32 size)
{
    return (char *)malloc(size);
}

void 
FileUtilAndroidImpl::freeBuffer(char* buffer)
{
    free(buffer);
}

void
FileUtilAndroidImpl::setAssetManager(AAssetManager* assetMgr)
{
    mAssetMgr = assetMgr;
}

AAssetManager*
FileUtilAndroidImpl::getAssetManager()
{
    return mAssetMgr;
}

void
FileUtilAndroidImpl::findFiles(const String& dir,
                               const String& pattern,
                               StringMap<String>& filePathMap, 
                               bool recursive)
{
    AAssetDir* assetDir = AAssetManager_openDir(mAssetMgr, dir.c_str());
    const char* fileName = NULL;
    while((fileName = AAssetDir_getNextFileName(assetDir)) != NULL)
    {
        if(lynxStringMatch(fileName, pattern.c_str(), false))
        {
            filePathMap.insert(fileName, dir + "/" + fileName);
        }
    }
    AAssetDir_close(assetDir);
}

void
FileUtilAndroidImpl::findFiles(const String& dir,
                               const String& pattern,
                               List<String>& filePathList, 
                               bool recursive)
{
    AAssetDir* assetDir = AAssetManager_openDir(mAssetMgr, dir.c_str());
    const char* fileName = NULL;
    while((fileName = AAssetDir_getNextFileName(assetDir)) != NULL)
    {
        if(lynxStringMatch(fileName, pattern.c_str(), false))
        {
            filePathList.insertTail(dir + "/" + fileName);
        }
    }
    AAssetDir_close(assetDir);
}

bool
FileUtilAndroidImpl::loadFile(const String& filePath, StreamBuffer& stream)
{
    AAsset* asset = AAssetManager_open(mAssetMgr, filePath.c_str(), AASSET_MODE_UNKNOWN);
    if (!asset)
    {
        return false;
    }
    int nBytes = 0;
    while (1)
    {
        StreamBuffer::Node* node = stream.allocNode();
        //AAsset_seek(asset, nBytes, SEEK_CUR);
        nBytes = AAsset_read(asset, (char*)node + sizeof(StreamBuffer::Node), stream.mNodeDataSize);
        node->mLen += nBytes;
        if (nBytes == 0)
        {
            stream.deallocNode(node);
            break;
        }
        stream.pushNode(node);
    }
    AAsset_close(asset);
    return true;
}

Int32 
FileUtilAndroidImpl::loadFile(const String& filePath, char** buffer)
{
    AAsset* asset = AAssetManager_open(mAssetMgr, filePath.c_str(), AASSET_MODE_UNKNOWN);
    if (asset == NULL)
    {
        return -1;
    }

    UInt32 fileLen = AAsset_getLength(asset);
    *buffer = allocBuffer(fileLen + 1);
    memcpy((void*)*buffer, AAsset_getBuffer(asset), fileLen);

    (*buffer)[fileLen] = 0;
    AAsset_close(asset);
    return fileLen;
}

Int32 
FileUtilAndroidImpl::loadFile(const String& filePath, char* buffer, Int32 size)
{
    AAsset* asset = AAssetManager_open(mAssetMgr, filePath.c_str(), AASSET_MODE_UNKNOWN);
    if (asset == NULL)
    {
        return -1;
    }

    UInt32 fileLen = AAsset_getLength(asset);
    if (fileLen != (size -1))
    {
    	AAsset_close(asset);
	return -1;
    }

    memcpy((void*)buffer, AAsset_getBuffer(asset), fileLen);

    buffer[fileLen] = 0;
    AAsset_close(asset);
    return fileLen;
}

bool
FileUtilAndroidImpl::saveFile(const String& filePath, StreamBuffer& stream)
{
    // Android APK不支持文件写入.
    return false;
}

bool
FileUtilAndroidImpl::saveFile(const String& filePath, char* buffer, UInt32 bufLen)
{
    return false;
}

Int32 
FileUtilAndroidImpl::getFileSize(const String& filePath)
{
    AAsset* asset = AAssetManager_open(mAssetMgr, filePath.c_str(), AASSET_MODE_UNKNOWN);
    if (asset == NULL)
    {
        return 0;
    }

    UInt32 len = AAsset_getLength(asset);
    AAsset_close(asset);
    return len;
}

void* 
FileUtilAndroidImpl::openFile(const String& filePath)
{
    AAsset* asset = AAssetManager_open(mAssetMgr, filePath.c_str(), AASSET_MODE_UNKNOWN);
    return asset;
}

Int32 
FileUtilAndroidImpl::seekFile(void* filePointer, Int32 offset, UInt8 whence)
{
    if (AAsset_seek((AAsset*)filePointer, offset, whence) == -1)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

Int32 
FileUtilAndroidImpl::tellFile(void* filePointer)
{

}

void 
FileUtilAndroidImpl::rewindFile(void* filePointer)
{
    AAsset_seek((AAsset*)filePointer, 0, SEEK_SET);
}

Int32 
FileUtilAndroidImpl::readFile(void* filePointer, char* buffer, UInt32 len)
{
    return AAsset_read((AAsset*)filePointer, buffer, len);
}

Int32 
FileUtilAndroidImpl::readFile(void* filePointer, StreamBuffer& stream, UInt32 len)
{
    UInt32 remainReadLen = len;
    Int32 nBytes = 0;
    while (remainReadLen)
    {
        UInt32 readLen = remainReadLen < stream.mNodeDataSize ? remainReadLen : stream.mNodeDataSize;
        StreamBuffer::Node* node = stream.allocNode();
        nBytes = AAsset_read((AAsset*)filePointer, (char*)node + sizeof(StreamBuffer::Node), readLen);
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
FileUtilAndroidImpl::closeFile(void* filePointer)
{
    AAsset_close((AAsset*)filePointer);
}

#endif
