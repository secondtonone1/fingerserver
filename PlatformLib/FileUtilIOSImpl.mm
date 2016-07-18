#include "FileUtilIOSImpl.h"
#include "Utilex.h"
#include "TimeUtil.h"

using namespace Lynx;

char* 
FileUtilIOSImpl::allocBuffer(UInt32 size)
{
    return (char *)malloc(size);
}

void 
FileUtilIOSImpl::freeBuffer(char* buffer)
{
    free(buffer);
}

void
FileUtilIOSImpl::findFiles(const String& dir,
                           const String& pattern,
                           StringMap<String>& filePathMap,
                           bool recursive)
{
    NSBundle* mainBundle = [NSBundle mainBundle];
    NSString* bundlePath = [mainBundle bundlePath];
    NSString* searchPath = [bundlePath stringByAppendingPathComponent:[NSString stringWithUTF8String:dir.c_str()]];
    
    NSError* error;
    NSArray* filesArray = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:searchPath error:&error];
    if (filesArray != nil)
    {
        for (NSString* fileName in filesArray)
        {
            const char* name = [[fileName lastPathComponent] UTF8String];
            String pathWithName = dir + "/";
            pathWithName += name;
            if(lynxStringMatch(name, pattern.c_str(), false))
            {
                filePathMap.insert(name, pathWithName.c_str());
            }
        }
    }
}

void
FileUtilIOSImpl::findFiles(const String& dir,
                               const String& pattern,
                               List<String>& filePathList, 
                               bool recursive)
{
    NSBundle* mainBundle = [NSBundle mainBundle];
    NSString* bundlePath = [mainBundle bundlePath];
    NSString* searchPath = [bundlePath stringByAppendingPathComponent:[NSString stringWithUTF8String:dir.c_str()]];

    NSArray* filesArray = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:searchPath error:nil];
    if (filesArray != nil)
    {
        for (NSString* filePath in filesArray)
        {
            const char* fileName = [[filePath lastPathComponent] UTF8String];
            if(lynxStringMatch(fileName, pattern.c_str(), false))
            {
                filePathList.insertTail([filePath UTF8String]);
            }
        }
    }
}

bool
FileUtilIOSImpl::loadFile(const String& filePath, StreamBuffer& stream)
{
    NSBundle* mainBundle = [NSBundle mainBundle];
    NSString* bundlePath = [mainBundle bundlePath];
    NSString* fullPath = [bundlePath stringByAppendingPathComponent:[NSString stringWithUTF8String:filePath.c_str()]];
    
    NSData* fileData = [[NSFileManager defaultManager] contentsAtPath:fullPath];
    if (fileData == nil)
    {
        LOG_WARN("Failed to get NSData from path %s.", [fullPath UTF8String]);
        return false;
    }
    
    NSUInteger readData = 0;
    NSUInteger leftData = [fileData length];
    
    while (1)
    {
        NSRange loadRange;
        loadRange.location = readData;
        loadRange.length = leftData;
        
        if (loadRange.length > stream.mNodeDataSize)
        {
            loadRange.length = stream.mNodeDataSize;
        }
        
        StreamBuffer::Node* node = stream.allocNode();
        
        @try
        {
            [fileData getBytes:(char*)node + sizeof(StreamBuffer::Node) range:loadRange];
        }
        @catch (NSException *exception)
        {
            LOG_WARN("Failed to get bytes from NSData for %s.", [exception description]);
            
            stream.deallocNode(node);
            stream.clear();
            
            return false;
        }
        
        readData += loadRange.length;
        leftData =  [fileData length] - readData;
        
        node->mLen = (UInt32)loadRange.length;
        
        stream.pushNode(node);
        
        if (leftData == 0)
        {
            break;
        }
    }
    
    return true;
}

Int32 
FileUtilIOSImpl::loadFile(const String& filePath, char** buffer)
{
    NSBundle* mainBundle = [NSBundle mainBundle];
    NSString* bundlePath = [mainBundle bundlePath];
    NSString* fullPath = [bundlePath stringByAppendingPathComponent:[NSString stringWithUTF8String:filePath.c_str()]];
    
    NSData* fileData = [[NSFileManager defaultManager] contentsAtPath:fullPath];
    if (fileData == nil)
    {
        LOG_WARN("Failed to get NSData from path %s.", [fullPath UTF8String]);
        return -1;
    }
    
    UInt32 fileLen = (UInt32)[fileData length];
    *buffer = allocBuffer(fileLen + 1);
    
    [fileData getBytes:(*buffer)];
    
    (*buffer)[fileLen] = 0;

    return fileLen;
}

Int32 
FileUtilIOSImpl::loadFile(const String& filePath, char* buffer, Int32 size)
{
    NSBundle* mainBundle = [NSBundle mainBundle];
    NSString* bundlePath = [mainBundle bundlePath];
    NSString* fullPath = [bundlePath stringByAppendingPathComponent:[NSString stringWithUTF8String:filePath.c_str()]];
    
    NSData* fileData = [[NSFileManager defaultManager] contentsAtPath:fullPath];
    if (fileData == nil)
    {
        LOG_WARN("Failed to get NSData from path %s.", [fullPath UTF8String]);
        return -1;
    }
    
    UInt32 fileLen = (UInt32)[fileData length];
    if (fileLen != (size - 1))
    {
        LOG_WARN("file length is not matched the size from argument, file length is %d, size is %d", fileLen, size);
        return -1;
    }
    
    [fileData getBytes:buffer];
    
    buffer[fileLen] = 0;
    
    return fileLen;
}

bool
FileUtilIOSImpl::saveFile(const String& filePath, StreamBuffer& stream)
{
    return false;
}

bool
FileUtilIOSImpl::saveFile(const String& filePath, char* buffer, UInt32 bufLen)
{
    return false;
}

Int32 
FileUtilIOSImpl::getFileSize(const String& filePath)
{
    NSBundle* mainBundle = [NSBundle mainBundle];
    NSString* bundlePath = [mainBundle bundlePath];
    NSString* fullPath = [bundlePath stringByAppendingPathComponent:[NSString stringWithUTF8String:filePath.c_str()]];
    
    NSData* fileData = [[NSFileManager defaultManager] contentsAtPath:fullPath];
    if (fileData == nil)
    {
        LOG_WARN("Failed to get NSData from path %s.", [fullPath UTF8String]);
        return 0;
    }
    
    UInt32 fileLen = (UInt32)[fileData length];
    
    return fileLen;
}

///////////////////////////  EMPTY FUNCTIONS  /////////////////////////////////

void*
FileUtilIOSImpl::openFile(const String& filePath)
{
    return NULL;
}

Int32
FileUtilIOSImpl::seekFile(void* filePointer, Int32 offset, UInt8 whence/*0=SEEK_SET,1=SEEK_CUR,2=SEEK_END*/)
{
    return -1;
}

Int32
FileUtilIOSImpl::tellFile(void* filePointer)
{
    return -1;
}

void
FileUtilIOSImpl::rewindFile(void* filePointer)
{

}

Int32
FileUtilIOSImpl::readFile(void* filePointer, char* buffer, UInt32 len)
{
    return -1;
}

Int32
FileUtilIOSImpl::readFile(void* filePointer, StreamBuffer& stream, UInt32 len)
{
    return -1;
}

void
FileUtilIOSImpl::closeFile(void* filePointer)
{

}

