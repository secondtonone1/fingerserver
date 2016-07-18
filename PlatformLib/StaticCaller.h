#ifndef __LYNX_PLATFORM_LIB_STATIC_CALLER_H__
#define __LYNX_PLATFORM_LIB_STATIC_CALLER_H__

#define LYNX_STATICCALLER__JOIN(a, b)               a##b

#define LYNX_STATICCALLER__MAKE_PARAMS1_0(t)
#define LYNX_STATICCALLER__MAKE_PARAMS1_1(t)        t##1
#define LYNX_STATICCALLER__MAKE_PARAMS1_2(t)        t##1, t##2
#define LYNX_STATICCALLER__MAKE_PARAMS1_3(t)        t##1, t##2, t##3
#define LYNX_STATICCALLER__MAKE_PARAMS1_4(t)        t##1, t##2, t##3, t##4
#define LYNX_STATICCALLER__MAKE_PARAMS1_5(t)        t##1, t##2, t##3, t##4, t##5
#define LYNX_STATICCALLER__MAKE_PARAMS1_6(t)        t##1, t##2, t##3, t##4, t##5, t##6

#define LYNX_STATICCALLER__MAKE_PARAMS2_0(t1, t2)
#define LYNX_STATICCALLER__MAKE_PARAMS2_1(t1, t2)   t1##1 t2##1
#define LYNX_STATICCALLER__MAKE_PARAMS2_2(t1, t2)   t1##1 t2##1, t1##2 t2##2
#define LYNX_STATICCALLER__MAKE_PARAMS2_3(t1, t2)   t1##1 t2##1, t1##2 t2##2, t1##3 t2##3
#define LYNX_STATICCALLER__MAKE_PARAMS2_4(t1, t2)   t1##1 t2##1, t1##2 t2##2, t1##3 t2##3, t1##4 t2##4
#define LYNX_STATICCALLER__MAKE_PARAMS2_5(t1, t2)   t1##1 t2##1, t1##2 t2##2, t1##3 t2##3, t1##4 t2##4, t1##5 t2##5
#define LYNX_STATICCALLER__MAKE_PARAMS2_6(t1, t2)   t1##1 t2##1, t1##2 t2##2, t1##3 t2##3, t1##4 t2##4, t1##5 t2##5, t1##6 t2##6

#define LYNX_STATICCALLER__MAKE_PARAMS1(n, t)       LYNX_STATICCALLER__JOIN(LYNX_STATICCALLER__MAKE_PARAMS1_, n) (t)
#define LYNX_STATICCALLER__MAKE_PARAMS2(n, t1, t2)  LYNX_STATICCALLER__JOIN(LYNX_STATICCALLER__MAKE_PARAMS2_, n) (t1, t2)
namespace Lynx
{
    template <class TSignature>
    class StaticCaller; // no body
}
#pragma warning(push)
#pragma warning(disable: 4715)

#define LYNX_STATICCALLER__DELEGATE_NUM_ARGS	0 // Delegate<R ()>
#include "StaticCallerTemplate.h"
#undef LYNX_STATICCALLER__DELEGATE_NUM_ARGS

#define LYNX_STATICCALLER__DELEGATE_NUM_ARGS	1 // Delegate<R (T1)>
#include "StaticCallerTemplate.h"
#undef LYNX_STATICCALLER__DELEGATE_NUM_ARGS

#define LYNX_STATICCALLER__DELEGATE_NUM_ARGS	2 // Delegate<R (T1, T2)>
#include "StaticCallerTemplate.h"
#undef LYNX_STATICCALLER__DELEGATE_NUM_ARGS

#define LYNX_STATICCALLER__DELEGATE_NUM_ARGS	3 // Delegate<R (T1, T2, T3)>
#include "StaticCallerTemplate.h"
#undef LYNX_STATICCALLER__DELEGATE_NUM_ARGS

#define LYNX_STATICCALLER__DELEGATE_NUM_ARGS	4 // Delegate<R (T1, T2, T3, T4)>
#include "StaticCallerTemplate.h"
#undef LYNX_STATICCALLER__DELEGATE_NUM_ARGS

#define LYNX_STATICCALLER__DELEGATE_NUM_ARGS	5 // Delegate<R (T1, T2, T3, T4, T5)>
#include "StaticCallerTemplate.h"
#undef LYNX_STATICCALLER__DELEGATE_NUM_ARGS

#define LYNX_STATICCALLER__DELEGATE_NUM_ARGS	6 // Delegate<R (T1, T2, T3, T4, T5, T6)>
#include "StaticCallerTemplate.h"
#undef LYNX_STATICCALLER__DELEGATE_NUM_ARGS

#pragma warning(pop)

#endif //__LYNX_PLATFORM_LIB_STATIC_CALLER_H__