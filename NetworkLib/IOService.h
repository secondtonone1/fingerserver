#ifndef __LYNX_CORE_LIB_IOSERVICE_H__
#define __LYNX_CORE_LIB_IOSERVICE_H__

#include "ErrorCode.h"
#include "StreamBuffer.h"
#include "TimeUtil.h"

#define IOSERVICE_EPOLL_DRIVER                    0x00000001   // EPOOL模型驱动
#define IOSERVICE_IOCP_DRIVER                     0x00000010   // IOCP模型驱动
#define IOSERVICE_SELECT_DRIVER                   0x00000100   // SELECT模型驱动

#define IOSERVICE_TIMEOUT_EVENT                   0x01000000  // 超时事件
#define IOSERVICE_READ_EVENT                      0x00000100  // 可读事件
#define IOSERVICE_WRITE_EVENT                     0x00001000  // 可写事件
#define IOSERVICE_SIGNAL_EVENT                    0x00000100  // 信号事件

namespace Lynx
{
    struct IOEvent;
    typedef void (*IOEventCallback)(IOEvent*, UInt32);

    //struct Overlapped 
    //{
    //    unsigned long* Internal;
    //    unsigned long* InternalHigh;
    //    union 
    //    {
    //        struct
    //        {
    //            unsigned long Offset;
    //            unsigned long OffsetHigh;
    //        };
    //
    //        void* Pointer;
    //    };
    //    void* hEvent;
    //};

    struct IOEvent
    {
        Int32                  mFd;             // 套接字文件描述符
        UInt32                 mErrno;          // IOEvent在事件数组中的位置
        UInt32                 mEventType;      // 事件类型
        bool                   mValid;          // 事件有效, 表示被添加到事件队列中
        IOEventCallback        mCallback;       // 事件对应的回调函数
        void*                  mOwner;          // 事件宿主指针
        StreamBuffer::Node*    mData;           // 事件数据指针, 用来处理异步收发
        Int32                  mHeapIndex;      // 最小堆索引, 用来记录Timeout使用
        Timeval                mTimeout;        // 超时时间
        Timeval                mDuration;       // 超时间隔
                                                 // 下面成员是IOCP模型专用
        //Int32                  mListenFd;       // 监听文件描述符
        //Overlapped             mOverlapped;     // 重迭结构对象
        //UInt32                 mDstIp;          // 远端IP地址, 通过inet_addr从字符串得到
        //UInt16                 mDstPort;        // 远端端口地址
        //UInt8                  mPostConnect;    // 标识是否为投递链接, 非0有效
        //UInt8                  mPostAccept;     // 标识是否为投递接受, 非0有效
        //UInt32                 mPostListen;     // 标识是否为投递监听, 非0有效
        //UInt32                 mDataSize;       // 用来记录接受BUFFER的空闲空间大小

        IOEvent() : mFd(0), mErrno(0), mEventType(0), mCallback(0), mOwner(0)
                  , mData(0), mHeapIndex(-1), mValid(false)
                  //, mListenFd(-1), mDstIp(0), mDstPort(0), mPostConnect(0)
                  //, mPostAccept(0), mPostListen(0), mDataSize(0)
        {
            //memset(&mOverlapped, 0, sizeof(mOverlapped));
        }
    };

    class IOService;
    struct IOEventOperator
    {
        Int32                  mType;
        const char*            mName;
        void*                  (*mInitial)(IOService* ioService);
        void                   (*mRelease)(void* op);
        Int32                  (*mAddEvent)(IOEvent* ev, void* op);
        Int32                  (*mDelEvent)(IOEvent* ev, void* op);
        Int32                  (*mDispatch)(IOService* ioService, void* op, Timeval* tv);
    };

    struct IOEventMinHeap
    {
        IOEventMinHeap() : mHeap(0), mSize(0), mAllocNumber(0) {}
        ~IOEventMinHeap() { if (mHeap) free(mHeap); }

        inline bool empty() { return (mSize == 0u); }
        inline UInt32 size() { return mSize; }
        inline IOEvent* top() { return mSize ? *mHeap : 0; }
  
        Int32 elemGreater(IOEvent* a, IOEvent* b)
        {
            return TIMERCMP(&a->mTimeout, &b->mTimeout, >);
        }

        inline Int32 push(IOEvent* ev)
        {
            if (reserve(mSize + 1))
            {
                return -1;
            }
            shiftUp(mSize++, ev);
            return 0;
        }

        inline IOEvent* pop()
        {
            if (mSize)
            {
                IOEvent* ioEvent = *mHeap;
                shiftDown(0u, mHeap[--mSize]);
                ioEvent->mHeapIndex = -1;
                return ioEvent;
            }
            return 0;
        }

        inline Int32 erase(IOEvent* ev)
        {
            if (-1 != ev->mHeapIndex)
            {
                IOEvent* last = mHeap[--mSize];
                UInt32 parent = (ev->mHeapIndex - 1) / 2;
                // we replace e with the last element in the heap.  We might need to
                // shift it upward if it is less than its parent, or downward if it is
                // greater than one or both its children. Since the children are known
                // to be less than the parent, it can't need to shift both up and down.
                if (ev->mHeapIndex > 0 && elemGreater(mHeap[parent], last))
                {
                    shiftUp(ev->mHeapIndex, last);
                }
                else
                {
                    shiftDown(ev->mHeapIndex, last);
                }
                ev->mHeapIndex = -1;
                return 0;
            }
            return -1;
        }

        inline Int32 reserve(UInt32 n)
        {
            if (mAllocNumber < n)
            {
                UInt32 number = mAllocNumber ? mAllocNumber * 2 : 8;
                if (number < n)
                {
                    number = n;
                }
                IOEvent** ev = 0;
                if (!(ev = (IOEvent**)realloc(mHeap, number * sizeof(*ev))))
                {
                    return -1;
                }
                mHeap = ev;
                mAllocNumber = number;
            }
            return 0;
        }

        inline void shiftUp(UInt32 holeIndex, IOEvent* ev)
        {
            UInt32 parent = (holeIndex - 1) / 2;
            while (holeIndex && elemGreater(mHeap[parent], ev))
            {
                (mHeap[holeIndex] = mHeap[parent])->mHeapIndex = holeIndex;
                holeIndex = parent;
                parent = (holeIndex - 1) / 2;
            }
            (mHeap[holeIndex] = ev)->mHeapIndex = holeIndex;
        }

        inline void shiftDown(UInt32 holeIndex, IOEvent* ev)
        {
            unsigned minChild = 2 * (holeIndex + 1);
            while (minChild <= mSize)
            {
                minChild -= minChild == mSize || elemGreater(mHeap[minChild], mHeap[minChild - 1]);
                if (!(elemGreater(ev, mHeap[minChild])))
                {
                    break;
                }
                (mHeap[holeIndex] = mHeap[minChild])->mHeapIndex = holeIndex;
                holeIndex = minChild;
                minChild = 2 * (holeIndex + 1);
            }
            (mHeap[holeIndex] = ev)->mHeapIndex = holeIndex;
        }

    public:
        IOEvent**              mHeap;
        UInt32                 mSize;
        UInt32                 mAllocNumber;
    };

    class IOService
    {
    public:
        IOService(Int32 type = IOSERVICE_SELECT_DRIVER);
        virtual ~IOService();

        inline Int32 getType() { return mIOEventOperator->mType; }

        Int32 addEvent(IOEvent* ev);
        void  delEvent(IOEvent* ev);

        Int32 poll(UInt32 times = 1);
        Int32 run();

        void pause();
        void resume();
        void stop();
        
    private:
        bool                   mShutdown;           // 标识服务是否关闭
        bool                   mPaused;             // 标识服务是否暂停
        IOEventOperator*       mIOEventOperator;    // 事件操作基类指针, 该指针根据条件编译
        void*                  mOperation;          // 事件操作基础数据指针, 应用层只负责保留,//select模型数据，epoll模型数据等
        IOEventMinHeap         mTimeoutHeap;        // 定时器计算使用的最小堆
        Timeval                mTimeFlag;           // 定时器计算使用的时间标记, 用来纠错时间
    };
} // namespace Lynx

#endif // __LYNX_CORE_LIB_IOSERVICE_H__


