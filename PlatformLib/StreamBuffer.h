#ifndef __LYNX_PLATFORM_LIB_STREAMBUFFER_H__
#define __LYNX_PLATFORM_LIB_STREAMBUFFER_H__

#include "Compat.h"
#include "DataStruct.h"
#include "Container.h"

namespace Lynx 
{
    struct StreamBuffer 
    {
        struct Node 
        {
            Node() : mOff(0), mLen(0) {}
            UInt32 mOff; // valid data position for read 
            UInt32 mLen; // data length
            //LYNX_DLIST_ENTRY(Node) mEntries; 
            struct Node *mNext;     /* next element */                                             \
            struct Node **mPrev;
        };

        UInt32 mNodeDataSize;
        LYNX_DLIST_HEAD(NodeQue, Node) mData;
        UInt32 mNodeCount;
        UInt32 mCacheSize;
        UInt32 mCacheCount;
        LYNX_DLIST_HEAD(NodeCacheQue, Node) mCache;
		
        StreamBuffer(UInt32 nodeDataSize = (1024 - sizeof(Node)), UInt32 cacheSize = 0) 
            : mNodeDataSize(nodeDataSize), mCacheSize(cacheSize), mCacheCount(0), mNodeCount(0)
        {
            LYNX_DLIST_INIT(&mData);
            LYNX_DLIST_INIT(&mCache);
        }

        ~StreamBuffer() 
        {
            Node* node;
            while (node = LYNX_DLIST_FIRST(&mData))
            {
                LYNX_DLIST_REMOVE(&mData, node, mEntries);
                XFREE(node);
            }
            mNodeCount = 0;

            while (node = LYNX_DLIST_FIRST(&mCache))
            {
                LYNX_DLIST_REMOVE(&mCache, node, mEntries);
                XFREE(node);
            }
            mCacheCount = 0;
        }

        /**
         * Allocate a memory block which size is mNodeDataSize. You can return it
         * through the same object function deallocNode or pushNode 
         */
        inline Node* allocNode()
        {
            Node* node = NULL;
            if (mCacheCount <= 0) 
            {
                node = (Node *)XALLOC(mNodeDataSize + sizeof(Node));
                ASSERT(node);
                return new (node) Node();
            }
				
            node = LYNX_DLIST_FIRST(&mCache);
            ASSERT(node); 
            LYNX_DLIST_REMOVE(&mCache, node, mEntries);
            mCacheCount--;
            return new (node) Node();
        }

        inline void deallocNode(Node* node)
        {
            ASSERT(node);
            if (mCacheCount >= mCacheSize)
            {
                XFREE(node);
                return;
            }
            LYNX_DLIST_INSERT_TAIL(&mCache, node, mEntries);
            mCacheCount++;
        }

        inline UInt32 getNodeCount()
        {
            mNodeCount = 0;
            Node* node;
            LYNX_DLIST_FOREACH(node, &mData, mEntries)
            {
                mNodeCount++;
            }
            return mNodeCount;
        }

        /**
         * The argument node must be allocated by the same object of allocNode or popNode
         */
        inline void pushNode(Node* node)
        {
            ASSERT(node);
            LYNX_DLIST_INSERT_TAIL(&mData, node, mEntries);
            mNodeCount++;
        }

        inline void insertNodeAtHead(Node* node)
        {
            ASSERT(node);
            LYNX_DLIST_INSERT_HEAD(&mData, node, mEntries);
            mNodeCount++;
        }

        inline Node* popNode()
        {
            Node* node = LYNX_DLIST_FIRST(&mData);
            if (node)
            {
                LYNX_DLIST_REMOVE(&mData, node, mEntries);
                mNodeCount--;
            }
            return node;
        }

        void print() /* string output */
        {
            Node* node;
            LYNX_DLIST_FOREACH(node, &mData, mEntries)
            {
                if (node->mLen + node->mOff < mNodeDataSize) 
                    *((char *)node + sizeof(Node) + node->mOff + node->mLen) = 0;
                std::cout << (char *)node + sizeof(Node) + node->mOff;
            }
            std::cout << std::endl;
        }

        UInt32 length() const
        {
            UInt32 length = 0;
            Node* node;
            LYNX_DLIST_FOREACH(node, &mData, mEntries)
            {
                length += node->mLen;
            }
            return length;
        }

        void clear()
        {
            Node* node;
            while (node = LYNX_DLIST_FIRST(&mData))
            {
                LYNX_DLIST_REMOVE(&mData, node, mEntries);
                deallocNode(node);
            }
            mNodeCount = 0;
        }

        void drain(UInt32 nbytes)
        {
            Node* node = LYNX_DLIST_FIRST(&mData);
            while (node && nbytes > 0 && nbytes >= node->mLen)
            {
                nbytes -= node->mLen;
                Node* tmp = LYNX_DLIST_NEXT(node, mEntries);
                LYNX_DLIST_REMOVE(&mData, node, mEntries);
                deallocNode(node);
                mNodeCount--;
                node = tmp;
            }
 
            if (node && nbytes > 0)
            {
                node->mOff += nbytes;
                node->mLen -= nbytes;
            }
        }

        /**
         * The object's data will be erased for nbytes length
         */
        UInt32 read(void* buf, UInt32 nbytes)
        {
            Node* node = LYNX_DLIST_FIRST(&mData);
            if (node && nbytes <= node->mLen)
            {
                memcpy(buf, (char *)node + sizeof(Node) + node->mOff, nbytes);
                node->mOff += nbytes;
                node->mLen -= nbytes;
                if (node->mLen == 0)
                {
                    LYNX_DLIST_REMOVE(&mData, node, mEntries);
                    deallocNode(node);
                    mNodeCount--;
                }
                return nbytes;
            }

            UInt32 retlen = 0, drain = 0;
            while (node && retlen < nbytes)
            {
                if (node->mLen == 0)
                {
                    Node* tmp = LYNX_DLIST_NEXT(node, mEntries);
                    LYNX_DLIST_REMOVE(&mData, node, mEntries);
                    deallocNode(node);
                    mNodeCount--;
                    node = tmp;
                    continue;
                }
                drain = node->mLen < nbytes - retlen ? node->mLen : nbytes - retlen; 
                memcpy((char *)buf + retlen, (char *)node + sizeof(Node) + node->mOff, drain);
                node->mOff += drain;
                node->mLen -= drain;
                retlen += drain;
            }
            return retlen;
        }

		// copy data from stream 
		UInt32 copy(void* buf, UInt32 nbytes)
		{
			Node* node;
			UInt32 copyBytes = 0;
            LYNX_DLIST_FOREACH(node, &mData, mEntries)
            {
				if (node->mLen >= nbytes - copyBytes)
				{
					memcpy((char*)buf + copyBytes, (char*)node + sizeof(Node) + node->mOff, nbytes - copyBytes);
					copyBytes = nbytes;
				}
				else
				{
					memcpy((char*)buf + copyBytes, (char*)node + sizeof(Node) + node->mOff, node->mLen);
					copyBytes += node->mLen;
				}

				if (copyBytes == nbytes)
				{
				    return nbytes;
				}
			}

			if (copyBytes < nbytes)
			{
				return copyBytes;
			}
			else
			{
				return nbytes;
			}
		}


        /**
         * The object's data will be erased for nbytes length
         */
        UInt32 read(StreamBuffer& buf, UInt32 nbytes)
        {
            Node* node = LYNX_DLIST_FIRST(&mData);
            if (node && nbytes <= node->mLen)
            {
                buf.write((char *)node + sizeof(Node) + node->mOff, nbytes); 
                node->mOff += nbytes;
                node->mLen -= nbytes;
                if (node->mLen == 0)
                {
                    LYNX_DLIST_REMOVE(&mData, node, mEntries);
                    deallocNode(node);
                    mNodeCount--;
                }
                return nbytes; 
            }

            UInt32 retlen = 0, drain = 0; 
            while (node && retlen < nbytes)
            {
                if (node->mLen == 0)
                {
                    Node* tmp = LYNX_DLIST_NEXT(node, mEntries);
                    LYNX_DLIST_REMOVE(&mData, node, mEntries);
                    deallocNode(node);
                    mNodeCount--;
                    node = tmp;
                    continue;
                }
                drain = node->mLen < nbytes - retlen ? node->mLen : nbytes - retlen; 
                buf.write((char *)node + sizeof(Node) + node->mOff, drain);
                node->mOff += drain;
                node->mLen -= drain;
                retlen += drain;
            }
            return retlen; 
        }
 
        template <typename _T_>
        _T_ read() 
        {
            _T_ obj;
            UInt32 len = read((char *)&obj, sizeof(_T_));
            ASSERT(len == sizeof(_T_));
            return obj; 
        }

        void write(const Int8* src, UInt32 nbytes)
        {
            ASSERT(src);
            Node* node = LYNX_DLIST_LAST(&mData, NodeQue);
            if (node == NULL || node->mOff + node->mLen + 8 >= mNodeDataSize)
            {
                pushNode(allocNode());
                node = LYNX_DLIST_LAST(&mData, NodeQue);
            }

            if (node->mOff + node->mLen + nbytes <= mNodeDataSize)
            {
                memcpy((char *)node + sizeof(Node) + node->mOff + node->mLen, src, nbytes);
                node->mLen += nbytes;
                return;
            }

            UInt32 blank = mNodeDataSize - node->mOff - node->mLen;
            memcpy((char *)node + sizeof(Node) + node->mOff + node->mLen, src, blank);
            node->mLen += blank;
            write(src + blank, nbytes - blank);
        }

        template <typename _T_> 
        void write(_T_ value) 
        { 
            write((Int8 *)&value, sizeof(value)); 
        }

        /**
         * The source object's data will be not erased
         */
        StreamBuffer& operator = (StreamBuffer& other)
        {
            Node* node;
            while (node = LYNX_DLIST_FIRST(&mData))
            {
                LYNX_DLIST_REMOVE(&mData, node, mEntries);
                deallocNode(node);
                mNodeCount--;
            }
            this->operator += (other);
            return *this;
        }

        /**
         * The source object's data will be not erased
         */
        StreamBuffer& operator += (const StreamBuffer& other)
        {
            Node* node;
            LYNX_DLIST_FOREACH(node, &other.mData, mEntries)
            {
                write((const Int8 *)((char *)node + sizeof(Node) + node->mOff), node->mLen);
            }
            return *this;
        }

        /**
         * The source object's data will be erased
         */
        StreamBuffer& operator >> (StreamBuffer& value)
        {
            value += *this;
            Node* node;
            while (node = LYNX_DLIST_FIRST(&mData))
            {
                LYNX_DLIST_REMOVE(&mData, node, mEntries);
                deallocNode(node);
                mNodeCount--;
            }
            return *this;
        }

        StreamBuffer& operator << (bool value) 
        { 
            write<Int8>((Int8)value);  
            return *this; 
        }

        StreamBuffer& operator >> (bool& value) 
        { 
            value = read<Int8>() == 0 ? false : true; 
            return *this; 
        }

        StreamBuffer& operator << (Int8 value) 
        { 
            write<Int8>((Int8)value); 
            return *this; 
        }

        StreamBuffer& operator >> (Int8& value) 
        { 
            value = read<Int8>(); 
            return *this; 
        }

        StreamBuffer& operator << (UInt8 value) 
        { 
            write<UInt8>((UInt8)value); 
            return *this; 
        }
 
        StreamBuffer& operator >> (UInt8& value) 
        {
          value = read<UInt8>(); 
          return *this; 
        }

        StreamBuffer& operator << (Int16 value) 
        { 
            write<Int16>((Int16)value); 
            return *this; 
        }

        StreamBuffer& operator >> (Int16& value) 
        { 
            value = read<Int16>(); 
            return *this; 
        }

        StreamBuffer& operator << (UInt16 value) 
        { 
          write<UInt16>((UInt16)value); 
          return *this; 
        }

        StreamBuffer& operator >> (UInt16& value) 
        { 
           value = read<UInt16>(); 
           return *this; 
        }

#if !defined __x86_64__
        StreamBuffer& operator << (long value) 
        { 
            write<Int32>((long)value); 
            return *this; 
        }

        StreamBuffer& operator >> (long& value)
        { 
            value = read<long>(); 
            return *this; 
        }

        StreamBuffer& operator << (unsigned long value) 
        { 
            write<Int32>((unsigned long)value); 
            return *this; 
        }

        StreamBuffer& operator >> (unsigned long& value)
        { 
            value = read<unsigned long>(); 
            return *this; 
        }
#endif

        StreamBuffer& operator << (Int32 value) 
        { 
            write<Int32>((Int32)value); 
            return *this; 
        }

        StreamBuffer& operator >> (Int32& value)
        { 
            value = read<Int32>(); 
            return *this; 
        }

#if !defined(__APPLE__)
        StreamBuffer& operator << (UInt32 value)
        { 
            write<UInt32>((UInt32)value); 
            return *this; 
        }

        StreamBuffer& operator >> (UInt32& value)
        { 
            value = read<UInt32>(); 
            return *this; 
        }
#endif

        StreamBuffer& operator << (Int64 value)
        { 
            write<Int64>((Int64)value); 
            return *this; 
        }

        StreamBuffer& operator >> (Int64& value) 
        { 
            value = read<Int64>(); 
            return *this; 
        }

        StreamBuffer& operator << (UInt64 value) 
        { 
            write<UInt64>((UInt64)value); 
            return *this; 
        }

        StreamBuffer& operator >> (UInt64& value) 
        { 
            value = read<UInt64>(); 
            return *this; 
        }

        StreamBuffer& operator << (float value) 
        { 
            write<float>((float)value); 
            return *this; 
        }

        StreamBuffer& operator >> (float& value) 
        { 
            value = read<float>(); 
            return *this; 
        }

        StreamBuffer& operator << (double value) 
        { 
            write<double>((double)value); 
            return *this; 
        }

        StreamBuffer& operator >> (double& value) 
        { 
            value = read<double>(); 
            return *this; 
        }

        StreamBuffer& operator >> (void*& value)
        {
            read(&value, sizeof(void*));
            return *this;
        }

        StreamBuffer& operator << (void* value)
        {
            write((const Int8*)&value, sizeof(void*));
            return *this;
        }

        StreamBuffer& operator << (const wchar_t& value)
        {
            write<wchar_t>((wchar_t)value); 
            return *this;
        }

        StreamBuffer& operator >> (wchar_t& value)
        {
            value = read<wchar_t>();
            return *this;
        }

        StreamBuffer& operator << (const NameString& value)
        {
            write((Int8 const *)value.c_str(), (UInt32)value.length());
            write((Int8)0);
            return *this;
        }

        StreamBuffer& operator >> (NameString& value)
        {
            while (LYNX_DLIST_FIRST(&mData))
            {
                Int8 c = read<Int8>();
                if (c == 0) break;
                value += c;
            }
            return *this;
        }

        StreamBuffer& operator << (const String& value)
        {
            write((Int8 const *)value.c_str(), (UInt32)value.length());
            write((Int8)0);
            return *this;
        }

        StreamBuffer& operator << (const char* value)
        {
            write((Int8 const *)value, value ? (UInt32)strlen(value) : 0);
            write((Int8)0);
            return *this;
        }

        StreamBuffer& operator >> (String& value)
        {
            //value.clear();
            while (LYNX_DLIST_FIRST(&mData))
            {
                Int8 c = read<Int8>();
                if (c == 0) break;
                value += c;
            }
            return *this;
        }

        StreamBuffer& operator << (const wchar_t* value)
        {
            write((Int8 const *)value, value ? (UInt32)wcslen(value)*2 : 0);
            write((UInt16)0);
            return *this;
        }

        StreamBuffer& operator << (const BasicStringW& value)
        {
            write((Int8 const *)value.c_str(), (UInt32)value.length()*2);
            write((UInt16)0);
            return *this;
        }
 
        StreamBuffer& operator >> (BasicStringW& value)
        {
            //value.clear();
            while (LYNX_DLIST_FIRST(&mData))
            {
                UInt16 c = read<UInt16>();
                if (c == 0) break;
                value += c;
            }
            return *this;
        }
    }; 

    template <class Type> struct IsLynxStreamBuffer 
    { 
        static const bool sValue = false; 
    };

    template <> struct IsLynxStreamBuffer<StreamBuffer> 
    { 
        static const bool sValue = true; 
    };

} // namespace Lynx

#endif // __LYNX_PLATFORM_LIB_STREAMBUFFER_H__


