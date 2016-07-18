#include "Container.h"

#define LYNX_STATICCALLER__DELEGATE_TEMPLATE_PARAMS    LYNX_STATICCALLER__MAKE_PARAMS1(LYNX_STATICCALLER__DELEGATE_NUM_ARGS, class T)
// class T0, class T1, class T2, ...
#define LYNX_STATICCALLER__DELEGATE_TEMPLATE_ARGS      LYNX_STATICCALLER__MAKE_PARAMS1(LYNX_STATICCALLER__DELEGATE_NUM_ARGS, T)
// T0, T1, T2, ...
#define LYNX_STATICCALLER__DELEGATE_FUNCTION_PARAMS    LYNX_STATICCALLER__MAKE_PARAMS2(LYNX_STATICCALLER__DELEGATE_NUM_ARGS, T, a)
// T0 a0, T1 a1, T2 a2, ...
#define LYNX_STATICCALLER__DELEGATE_FUNCTION_ARGS      LYNX_STATICCALLER__MAKE_PARAMS1(LYNX_STATICCALLER__DELEGATE_NUM_ARGS, a)
// a0, a1, a2, ...


// Comma if nonzero number of arguments
#if LYNX_STATICCALLER__DELEGATE_NUM_ARGS == 0
#define LYNX_STATICCALLER__DELEGATE_COMMA   
#else
#define LYNX_STATICCALLER__DELEGATE_COMMA ,
#endif

namespace Lynx
{
    template<class R LYNX_STATICCALLER__DELEGATE_COMMA LYNX_STATICCALLER__DELEGATE_TEMPLATE_PARAMS>
    class StaticCaller<R (LYNX_STATICCALLER__DELEGATE_TEMPLATE_ARGS)>
    {
    public:

        typedef R (*CallbackFunc)(void* mCaller LYNX_STATICCALLER__DELEGATE_COMMA LYNX_STATICCALLER__DELEGATE_FUNCTION_PARAMS);
        StaticCaller(void* mCaller, CallbackFunc func){
            registerFunc(mCaller, func);
        }
        StaticCaller(){}

        struct CallbackData 
        {
            void* mCaller;
            CallbackFunc mFunc;
        };

        typedef Vector<CallbackData> CallbackVec;

        bool registerFunc(void* mCaller, CallbackFunc func)
        {
            if (isExist(mCaller, func))
            {
                return false;
            }

            CallbackData cData = {mCaller, func};

            mCallbackVec.push_back(cData);
            return true;
        }
        bool unregisterFunc(void* mCaller, CallbackFunc func)
        {
            for (typename CallbackVec::iterator i = mCallbackVec.begin(); i != mCallbackVec.end(); ++i)
            {
                if ((*i).mCaller == mCaller && (*i).mFunc == func)
                {
                    mCallbackVec.erase(i);
                    return true;
                }
            }
            return false;
        }
        inline bool isExist(void* mCaller, CallbackFunc func) const
        {
            for (typename CallbackVec::const_iterator i = mCallbackVec.begin(); i != mCallbackVec.end(); ++i)
            {
                if ((*i).mCaller == mCaller && (*i).mFunc == func)
                {
                    return true;
                }
            }
            return false;
        }
        void execute(LYNX_STATICCALLER__DELEGATE_FUNCTION_PARAMS)
        {
            for (typename CallbackVec::iterator i = mCallbackVec.begin(); i != mCallbackVec.end(); ++i)
            {
                (*i).mFunc((*i).mCaller LYNX_STATICCALLER__DELEGATE_COMMA LYNX_STATICCALLER__DELEGATE_FUNCTION_ARGS);
            }
        }
        void merge(const StaticCaller<R (LYNX_STATICCALLER__DELEGATE_TEMPLATE_ARGS)>& other)
        {
            for (typename CallbackVec::const_iterator i = other.mCallbackVec.begin(); i != other.mCallbackVec.end(); ++i)
            {
                registerFunc((*i).mCaller, (*i).mFunc);
            }
        }
        void cut(const StaticCaller<R (LYNX_STATICCALLER__DELEGATE_TEMPLATE_ARGS)>& other)
        {
            for (typename CallbackVec::const_iterator i = other.mCallbackVec.begin(); i != other.mCallbackVec.end(); ++i)
            {
                unregisterFunc((*i).mCaller, (*i).mFunc);
            }
        }
        R operator()(LYNX_STATICCALLER__DELEGATE_FUNCTION_PARAMS) const
        {
            for (typename CallbackVec::const_iterator i = mCallbackVec.begin(); i != mCallbackVec.end(); ++i)
            {
                (*i).mFunc((*i).mCaller LYNX_STATICCALLER__DELEGATE_COMMA LYNX_STATICCALLER__DELEGATE_FUNCTION_ARGS);
            }
            return R();
        }
        void operator +=(const StaticCaller<R (LYNX_STATICCALLER__DELEGATE_TEMPLATE_ARGS)>& other)
        {
            merge(other);
        }
        void operator -=(const StaticCaller<R (LYNX_STATICCALLER__DELEGATE_TEMPLATE_ARGS)>& other)
        {
            cut(other);
        }
        void operator = (const StaticCaller<R (LYNX_STATICCALLER__DELEGATE_TEMPLATE_ARGS)>& other)
        {
            clear();
            merge(other);
        }
        inline void clear()
        {
            mCallbackVec.clear();
        }
        bool empty()
        {
            return !mCallbackVec.size();
        }
    protected:
        CallbackVec mCallbackVec;
    };

}

#undef LYNX_STATICCALLER__DELEGATE_TEMPLATE_PARAMS
#undef LYNX_STATICCALLER__DELEGATE_TEMPLATE_ARGS
#undef LYNX_STATICCALLER__DELEGATE_FUNCTION_PARAMS
#undef LYNX_STATICCALLER__DELEGATE_FUNCTION_ARGS
#undef LYNX_STATICCALLER__DELEGATE_COMMA

