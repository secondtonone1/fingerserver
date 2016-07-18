#ifndef __LYNX_PLATFORM_LIB_BYTE_ORDER_H__
#define __LYNX_PLATFORM_LIB_BYTE_ORDER_H__

#include "Compat.h"

namespace Lynx
{
    template <typename _T_> 
    class EndianReverse
    {
    public:
        inline static _T_ convert(_T_ val) { return val; }
    };

    template <>
    inline Int16 EndianReverse<Int16>::convert(Int16 input)
    {
        return (Int16) ((input << 8) & 0xff00) | ((input >> 8) & 0x00ff);
    }

    template <>
    inline UInt16 EndianReverse<UInt16>::convert(UInt16 input)
    {
        return (UInt16) ((input << 8) & 0xff00) | ((input >> 8) & 0x00ff);
    }

    template <>
    inline Int32 EndianReverse<Int32>::convert(Int32 input)
    {
        return (Int32) ((input << 24) & 0xff000000)| 
                       ((input << 8)  & 0x00ff0000)| 
                       ((input >> 8)  & 0x0000ff00)| 
                       ((input >> 24) & 0x000000ff);
    }

    template <>
    inline UInt32 EndianReverse<UInt32>::convert(UInt32 input)
    {
        return (UInt32) ((input << 24) & 0xff000000)| 
                        ((input << 8)  & 0x00ff0000)| 
                        ((input >> 8)  & 0x0000ff00)| 
                        ((input >> 24) & 0x000000ff);
    }

    template <>
    inline Int64 EndianReverse<Int64>::convert(Int64 input)
    {
        return (Int64) ((input << 56) & 0xff00000000000000ull)|
                       ((input << 40) & 0x00ff000000000000ull)|
                       ((input << 24) & 0x0000ff0000000000ull)|
                       ((input << 8)  & 0x000000ff00000000ull)|
                       ((input >> 8)  & 0x00000000ff000000ull)|
                       ((input >> 24) & 0x0000000000ff0000ull)|
                       ((input >> 40) & 0x000000000000ff00ull)|
                       ((input >> 56) & 0x00000000000000ffull);
    }

    template <>
    inline UInt64 EndianReverse<UInt64>::convert(UInt64 input)
    {
        return (UInt64) ((input << 56) & 0xff00000000000000ull)|
                        ((input << 40) & 0x00ff000000000000ull)|
                        ((input << 24) & 0x0000ff0000000000ull)|
                        ((input << 8)  & 0x000000ff00000000ull)|
                        ((input >> 8)  & 0x00000000ff000000ull)|
                        ((input >> 24) & 0x0000000000ff0000ull)|
                        ((input >> 40) & 0x000000000000ff00ull)|
                        ((input >> 56) & 0x00000000000000ffull);
    }

} // namesapce Lynx
                           
#endif // LYNX_BYTE_ORDER_H__


