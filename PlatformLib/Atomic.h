#ifndef __LYNX_PLATFORM_LIB_ATOMIC_H__
#define __LYNX_PLATFORM_LIB_ATOMIC_H__

#include "Compat.h"

#if defined(__linux__) || defined(__FreeBSD__)

namespace Lynx
{
    static inline void					
    atomicAddLone(volatile unsigned long *p, unsigned long v)
    {							
        __asm __volatile("lock ; addl %1,%0"	
                         : "=m" (*p)			
                         : "ir" (v), "m" (*p));		
    }

    static inline void
    atomicSubtractLong(volatile unsigned long *p, unsigned long v)
    {							
        __asm __volatile(" lock ; subl %1,%0"	
                         : "=m" (*p)			
                         : "ir" (v), "m" (*p));		
    }

    static inline bool
    atomicCompareExchangePointer(UInt32* destination, UInt32 comparand, UInt32 exchange)
    {
        return __sync_bool_compare_and_swap(destination, comparand, exchange);
    }

} // namespace Lynx

#define InterlockedIncrement(inc) Lynx::atomicAddLone(inc, 1)
#define InterlockedDecrement(dec) Lynx::atomicSubtractLong(dec, 1)
#define InterLockedCompareExchangePointer(destination, comparand, exchange) \
    Lynx::atomicCompareExchangePointer(destination, comparand, exchange)

#endif

#endif // __LYNX_PLATFORM_LIB_ATOMIC_H__


