#ifndef __LYNX_PLATFORM_LIB_BLOB_H__
#define __LYNX_PLATFORM_LIB_BLOB_H__

#include "Compat.h"

namespace Lynx  
{
    struct _PlatformExport Blob 
    { 
        Blob() : mLen(0), mData(0) {}
        Blob(const Blob& blob) 
        {
            mLen = blob.mLen;
            mData = blob.mData;
        }

        inline Blob& operator = (const Blob& blob)
        {
            mLen = blob.mLen;
            mData = blob.mData;
            return *this;
        }

        UInt32 mLen;
        Int8*  mData;
    };

    template <class Type> struct IsLynxBlob 
    { 
        static const bool sValue = false; 
    };

    template <> struct IsLynxBlob<Blob> 
    { 
        static const bool sValue = true; 
    };
} // namesapce Lynx

#endif // ifndef __LYNX_PLATFORM_LIB_BLOB_H__


