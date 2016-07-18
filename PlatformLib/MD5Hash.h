#ifndef __LYNX_UTIL_LIB_MD5_HASH__H__
#define __LYNX_UTIL_LIB_MD5_HASH__H__

#include "MemoryManager.h"

namespace Lynx
{
    class _PlatformExport MD5Hash
    {
    protected:
        unsigned long mState[4];
        unsigned long mCount[2];
        unsigned char mBuf[64];
        unsigned int  mBPos;
        bool          mCommitted;
        unsigned char mMd5[16];

        void initial();
        void update();
        void commit();
        bool isCommitted() const;

    public:
        MD5Hash();
        MD5Hash(const char* sum);

        void put(const char* buf, unsigned int size);

        operator const unsigned char*();
        String toString();
        String toTempString();

        bool operator == (const MD5Hash& hash);

        static String toString(const unsigned char* md5);
    };

} // namespace Lynx

#endif // __LYNX_UTIL_LIB_MD5_HASH__H__

