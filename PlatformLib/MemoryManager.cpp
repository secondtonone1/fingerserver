#include "MemoryManager.h"
#include "Logger.h"
#ifdef _WIN32
#include <windows.h>
#endif

using namespace Lynx;

MemoryManager::MemoryManager() : mFreeMemHead(0), mFreeMemTail(0), mSize(0), mMemTotle(0)
{
    SpinLock lock(mMutex);
    for (UInt32 i = 0; i < LYNX_MEM_COUNT; ++i)
    {
        mFreeList[i] = NULL;
    }
    mBlockList = NULL;
}

MemoryManager::~MemoryManager()
{
    SpinLock lock(mMutex);
    MemBlock* memoryBlock = NULL;
    while (mBlockList)
    {
        memoryBlock = mBlockList->mNext;
        ::free(mBlockList);
        mBlockList = memoryBlock;
    }
#ifdef _WIN32
#ifdef _DEBUG
	char str[1024];
	UInt32 MemSize = 0;
	snprintf(str, sizeof(str), "Memory Manager release======================================================================\n");
	OutputDebugString(str);
	for (MemDebugInfo::iterator i = mMemDebugInfo.begin();
		i != mMemDebugInfo.end(); ++i)
	{
        snprintf(str, sizeof(str), "%s(%d) : function: %s, size: %d\n"
			, i->second.file, i->second.line
			, i->second.function, i->second.size);
		MemSize += i->second.size;
		OutputDebugString(str);
	}
	snprintf(str, sizeof(str), "memory not release: %d, size: %d\n", mMemDebugInfo.size(), MemSize);
	OutputDebugString(str);
#endif
#endif
} 

void*
MemoryManager::allocDebug(char* file, int line, char* function, UInt32 size)
{
	MemInfo mi = {size, file, line, function};
	void* ptr = alloc(size);
	if (ptr)
	{
		SpinLock lock(mMutexDebugInfo);
        mMemTotle += size;
		mMemDebugInfo.insert(std::make_pair(ptr, mi));
	}
	return ptr;
}
void
MemoryManager::deallocDebug(char* file, int line, char* function, void* p)
{
	if (p)
	{
		SpinLock lock(mMutexDebugInfo);
		MemDebugInfo::iterator i = mMemDebugInfo.find(p);
		if (i != mMemDebugInfo.end())
		{
            mMemTotle -= i->second.size;
			mMemDebugInfo.erase(i);
		}
		else
		{
			char str[1024];
			snprintf(str, sizeof(str), "Not declared memory: %p, file: %s, line: %d, function: %s\n"
				, p, file, line, function);
#ifdef _WIN32
			OutputDebugString(str);
#endif
		}
	}
	dealloc(p);
}

void*
MemoryManager::alloc(UInt32 size)
{
    FreeNode **freeList, *node;
    if (size + LYNX_MEM_ANNEX_LEN > LYNX_MEM_MAX_FREE_NODE_SIZE)
    {
        node = (FreeNode*)malloc(size + LYNX_MEM_ANNEX_LEN);
    }
    else
    {
        SpinLock lock(mMutex);
        freeList = mFreeList + getIndex(size + LYNX_MEM_ANNEX_LEN);
        if (node = *freeList)
        {
            *freeList = (*freeList)->mNext;
        }
        else
        {
            node = (FreeNode*)refill(roundUp(size + LYNX_MEM_ANNEX_LEN));
        }
    }
    *((UInt32*)(node)) = size;
    *((UInt32*)(node->mData + LYNX_MEM_HEAD_LEN + size)) = LYNX_MEM_CHECKCODE;
    return node->mData + LYNX_MEM_HEAD_LEN;
}

void
MemoryManager::dealloc(void* pointer)
{
    if (pointer == NULL) return;

    FreeNode* node = (FreeNode*)((char*)pointer - LYNX_MEM_HEAD_LEN);
    UInt32 size = *(UInt32*)(node);
#ifndef _DEBUG
    if (size > 1024 * 1024 * 100)
    {
        LOG_WARN("Logic error.");
        return;
    }
#endif
    if (size + LYNX_MEM_ANNEX_LEN > LYNX_MEM_MAX_FREE_NODE_SIZE)
    {
        ::free(node);
        return;
    }
#ifdef _DEBUG // 检查内存越界
    UInt32 checkcode = *((UInt32*)(node->mData + LYNX_MEM_HEAD_LEN + size));
    if (checkcode != LYNX_MEM_CHECKCODE)
    {
        UInt32* p = NULL;
        *p = 100;
    }
#endif
    SpinLock lock(mMutex);
    FreeNode** freeList = mFreeList + getIndex(size + LYNX_MEM_ANNEX_LEN);
    node->mNext = *freeList;
    *freeList = node;
}

char* 
MemoryManager::allocBlock(UInt32 size, UInt32* n)
{
    char* result;
    UInt32 totalBytes = size * (*n);
    UInt32 leftBytes = (UInt32)(mFreeMemTail - mFreeMemHead);
    
    if (leftBytes >= totalBytes)
    {
        result = mFreeMemHead;
        mFreeMemHead += totalBytes;
        return result;
    }
    else if (leftBytes >= size)
    {
        *n = (UInt32)(leftBytes / size);
        totalBytes = size * (*n);
        result = mFreeMemHead;
        mFreeMemHead += totalBytes;
        return result;
    }
    else
    {
        if (leftBytes > 0)
        {
            FreeNode** freeList = mFreeList + getIndex(leftBytes);
            ((FreeNode*)mFreeMemHead)->mNext = *freeList;
            *freeList = (FreeNode*)(mFreeMemHead);
        }

        //UInt32 allocBytes = 2 * totalBytes + roundUp(mSize / LYNX_MEM_COUNT);
        UInt32 allocBytes = totalBytes;
        MemBlock* block = (MemBlock*)malloc(sizeof(MemBlock) + allocBytes);
        block->mData = mFreeMemHead = ((char*)block + sizeof(MemBlock));
        block->mNext = mBlockList;
        mBlockList = block;
 
        //if (mFreeMemHead == NULL)
        //{
        //    FreeNode **freeList, *tmp;
        //    for (UInt32 i = size; i <= LYNX_MEM_MAX_FREE_NODE_SIZE; 
        //        i += LYNX_MEM_BASE)
        //    {
        //        freeList = mFreeList + getIndex(i);
        //        if ((tmp = *freeList) != NULL)
        //        {
        //            *freeList = tmp->mNext;
        //            mFreeMemHead = (char*)tmp;
        //            mFreeMemTail = mFreeMemHead + i;
        //            return allocBlock(size, n);
        //        }
        //    }
        //    mFreeMemTail = 0;
        //    LOG_ERROR("Failed to alloc memory for memory leak.");
        //    ASSERT(0); // system memory leak
        //}
        mSize += allocBytes;
        mFreeMemTail = mFreeMemHead + allocBytes;
        return allocBlock(size, n);
    }
}

void* 
MemoryManager::refill(UInt32 size)
{
    mMemListSize += size;
    UInt32 n = LYNX_MEM_ALLOC_PARAM;
    char* block = (char*)allocBlock(size, &n);
 
    if (n == 1) return block;
    
    FreeNode **freeList, **freeList1;
    FreeNode *result, *currNode, *nextNode;

    freeList = freeList1 = mFreeList + getIndex(size);
    result = (FreeNode*)block;
    *freeList = nextNode = (FreeNode*)(block + size);
    
    for (UInt32 i = 1; ; ++i)
    {
        currNode = nextNode;
        nextNode = (FreeNode*)((char*)nextNode + size);
        if (n - 1 == i)
        {
            currNode->mNext = NULL;
            break;
        }
        else
        {
            currNode->mNext = nextNode;
        }
    }
    return result;
}

void
MemoryManager::_debugPrint(UInt32 printSize)
{
    SpinLock lock(mMutexDebugInfo);
    for (MemDebugInfo::iterator i = mMemDebugInfo.begin(); i != mMemDebugInfo.end(); ++i)
    {
        MemInfo* mi = &(i->second);
        if (mi->size >= printSize)
        {
            LOG_DEBUG("allocate file: %s, func: %s, line: %u, size: %u", mi->file, mi->function, mi->line, mi->size);
        }
    }
}