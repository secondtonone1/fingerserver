#include <utility> // for std::pair
#include "Logger.h"
#include "MemoryManager.h"
//#define LYNX_JOIN(a, b)               a##b
//#define LYNX_MAKE_PARAMS1(n, t)       LYNX_JOIN(LYNX_MAKE_PARAMS1_, n) (t)
//#define LYNX_DELEGATE_TEMPLATE_PARAMS LYNX_MAKE_PARAMS1_LYNX_DELEGATE_NUM_ARGS(class T)
//实现参数动态绑定 王戊辰
#define LYNX_DELEGATE_TEMPLATE_PARAMS    LYNX_MAKE_PARAMS1(LYNX_DELEGATE_NUM_ARGS, class T)
// class T0, class T1, class T2, ...


#define LYNX_DELEGATE_TEMPLATE_ARGS      LYNX_MAKE_PARAMS1(LYNX_DELEGATE_NUM_ARGS, T)
//#define LYNX_DELEGATE_TEMPLATE_PARAMS LYNX_MAKE_PARAMS1_LYNX_DELEGATE_NUM_ARGS( T)
// T0, T1, T2, ...


#define LYNX_DELEGATE_FUNCTION_PARAMS    LYNX_MAKE_PARAMS2(LYNX_DELEGATE_NUM_ARGS, T, a)
//#define LYNX_DELEGATE_TEMPLATE_PARAMS LYNX_MAKE_PARAMS2_LYNX_DELEGATE_NUM_ARGS( T,a)
// T0 a0, T1 a1, T2 a2, ...


#define LYNX_DELEGATE_FUNCTION_ARGS      LYNX_MAKE_PARAMS1(LYNX_DELEGATE_NUM_ARGS, a)
//#define LYNX_DELEGATE_TEMPLATE_PARAMS LYNX_MAKE_PARAMS1_LYNX_DELEGATE_NUM_ARGS( a)
// a0, a1, a2, ...

// Comma if nonzero number of arguments
#if LYNX_DELEGATE_NUM_ARGS == 0
#define LYNX_DELEGATE_COMMA
#else
#define LYNX_DELEGATE_COMMA ,
#endif

namespace Lynx
{
    //-------------------------------------------------------------------------
    // class Delegate<R (T1, T2, ..., TN)>

    template <class R LYNX_DELEGATE_COMMA LYNX_DELEGATE_TEMPLATE_PARAMS>
    class Delegate<R (LYNX_DELEGATE_TEMPLATE_ARGS)>
    {
    // Declaractions
    private:
        class DelegateImplBase
        {
            // Fields
        public:
            DelegateImplBase* mPrevious; // singly-linked list

            // Constructor/Destructor
        protected:
            DelegateImplBase() : mPrevious(NULL) { }
            DelegateImplBase(const DelegateImplBase& other) : mPrevious(NULL) { }
        public:
            virtual ~DelegateImplBase() { }

            // Methods
        public:
            virtual DelegateImplBase* clone() const = 0;
            virtual R invoke(LYNX_DELEGATE_FUNCTION_PARAMS) const = 0;
        };

        template <class TFunctor>
        struct Invoker
        {
            static R invoke(const TFunctor& f LYNX_DELEGATE_COMMA LYNX_DELEGATE_FUNCTION_PARAMS)
            {
                return (const_cast<TFunctor&>(f))(LYNX_DELEGATE_FUNCTION_ARGS);
            }
        };

        template <class TPtr, class TFunctionPtr>
        struct Invoker<std::pair<TPtr, TFunctionPtr> >
        {
            static R invoke(const std::pair<TPtr, TFunctionPtr>& mf LYNX_DELEGATE_COMMA LYNX_DELEGATE_FUNCTION_PARAMS)
            {
                return ((*mf.first).*mf.second)(LYNX_DELEGATE_FUNCTION_ARGS);
            }
        };

        template <class TFunctor>
        class DelegateImpl : public DelegateImplBase
        {
            // Fields
        public:
            TFunctor mFunctor;

            // Constructor
        public:
            DelegateImpl(const TFunctor& f) : mFunctor(f) {}
            DelegateImpl(const DelegateImpl& other) : mFunctor(other.mFunctor) {}

            // Methods
        public:
            virtual DelegateImplBase* clone() const
            {
                return XNEW(DelegateImpl)(*this);
            }
            virtual R invoke(LYNX_DELEGATE_FUNCTION_PARAMS) const
            {
                return Invoker<TFunctor>::invoke(this->mFunctor LYNX_DELEGATE_COMMA LYNX_DELEGATE_FUNCTION_ARGS);
            }
        };

        // Fields
    private:
        DelegateImplBase* mLast;

        // Constructor/Destructor
    public:
        Delegate()
        {
            this->mLast = NULL;
        }

        template <class TFunctor>
        Delegate(const TFunctor& f)
        {
            this->mLast = NULL;
            *this = f;
        }

        template<class TPtr, class TFunctionPtr>
        Delegate(const TPtr& obj, const TFunctionPtr& mfp)
        {
            this->mLast = NULL;
            *this = std::make_pair(obj, mfp);
        }

        Delegate(const Delegate& d)
        {
            this->mLast = NULL;
            *this = d;
        }

        ~Delegate()
        {
            clear();
        }

        // Properties
    public:
        bool empty() const
        {
            return (this->mLast == NULL);
        }

        bool multicast() const
        {
            return (this->mLast != NULL && this->mLast->mPrevious != NULL);
        }

        // Static Methods
    private:
        static DelegateImplBase* cloneDelegateList(DelegateImplBase* list, /*out*/ DelegateImplBase** first)
        {
            DelegateImplBase* list2 = list;
            DelegateImplBase* newList = NULL;
            DelegateImplBase** pp = &newList;
            DelegateImplBase* temp = NULL;

            try
            {
                while (list2 != NULL)
                {
                    temp = list2->clone();
                    *pp = temp;
                    pp = &temp->mPrevious;
                    list2 = list2->mPrevious;
                }
            }
            catch (...)
            {
                freeDelegateList(newList);
                throw;
            }

            if (first != NULL)
            {
                *first = temp;
            }
            return newList;
        }

        static void freeDelegateList(DelegateImplBase* list)
        {
            DelegateImplBase* temp = NULL;
            while (list != NULL)
            {
                temp = list->mPrevious;
                XDELETE(list);
                list = temp;
            }
        }

        static void invokeDelegateList(DelegateImplBase* list LYNX_DELEGATE_COMMA LYNX_DELEGATE_FUNCTION_PARAMS)
        {
            if (list != NULL)
            {
                if (list->mPrevious != NULL)
                    invokeDelegateList(list->mPrevious LYNX_DELEGATE_COMMA LYNX_DELEGATE_FUNCTION_ARGS);
                list->invoke(LYNX_DELEGATE_FUNCTION_ARGS);
            }
        }

        // Methods
    public:
        template <class TFunctor>
        bool exist(const TFunctor& f) const
        {
            DelegateImpl<TFunctor>* last = dynamic_cast<DelegateImpl<TFunctor>*>(this->mLast);
            while (last)
            {
                if (last->mFunctor == f)
                {
                    return true;
                }
                else
                {
                    last = dynamic_cast<DelegateImpl<TFunctor>*>(last->mPrevious);
                }
            }
            return false;
        }

        template <class TFunctor>
        void add(const TFunctor& f)
        {
            if (!exist<TFunctor>(f))
            {
                DelegateImplBase* d = XNEW(DelegateImpl<TFunctor>)(f);
                d->mPrevious = this->mLast;
                this->mLast = d;
            }
        }

        template<class TPtr, class TFunctionPtr>
        void add(const TPtr& obj, const TFunctionPtr& mfp)
        {
            add(std::make_pair(obj, mfp));
            //if (!exist<TPtr, TFunctionPtr>(obj, mfp))
            //{
            //    DelegateImplBase* d = new DelegateImpl<std::pair<TPtr, TFunctionPtr> >(std::make_pair(obj, mfp));
            //    d->mPrevious = this->mLast;
            //    this->mLast = d;
            //}
        }

        template <class TFunctor>
        bool remove(const TFunctor& f)
        {
            DelegateImplBase* d = this->mLast;
            DelegateImplBase** pp = &this->mLast;
            DelegateImpl<TFunctor>* impl = NULL;

            while (d != NULL)
            {
                impl = dynamic_cast<DelegateImpl<TFunctor>*>(d);
                if (impl != NULL && impl->mFunctor == f)
                {
                    *pp = d->mPrevious;
                    XDELETE(impl);
                    return true;
                }
                pp = &d->mPrevious;
                d = d->mPrevious;
            }
            return false;
        }

        template<class TPtr, class TFunctionPtr>
        bool remove(const TPtr& obj, const TFunctionPtr& mfp)
        {
            return remove(std::make_pair(obj, mfp));
        }

        void clear()
        {
            freeDelegateList(this->mLast);
            this->mLast = NULL;
        }

    private:
        template <class TFunctor>
        bool equals(const TFunctor& f) const
        {
            if (this->mLast == NULL || this->mLast->mPrevious != NULL)
            {
                return false;
            }

            DelegateImpl<TFunctor>* impl = dynamic_cast<DelegateImpl<TFunctor>*>(this->mLast);
            if (impl == NULL)
            {
                return false;
            }
            return (impl->mFunctor == f);
        }

        // Operators
    public:
        operator bool() const
        {
            return !empty();
        }

        bool operator!() const
        {
            return empty();
        }

        template <class TFunctor>
        Delegate& operator=(const TFunctor& f)
        {
            DelegateImplBase* d = XNEW(DelegateImpl<TFunctor>)(f);
            freeDelegateList(this->mLast);
            this->mLast = d;
            return *this;
        }

        Delegate& operator=(const Delegate& d)
        {
            if (this != &d)
            {
                DelegateImplBase* list = cloneDelegateList(d.mLast, NULL);
                freeDelegateList(this->mLast);
                this->mLast = list;
            }
            return *this;
        }

        template <class TFunctor>
        Delegate& operator+=(const TFunctor& f)
        {
            add(f);
            return *this;
        }

        template <class TFunctor>
        Delegate& operator-=(const TFunctor& f)
        {
            remove(f);
            return *this;
        }

        template <class TFunctor>
        bool operator==(const TFunctor& f)
        {
            return this->exist(f);
        }

        //template <class TFunctor>
        //friend bool operator==(const Delegate& d, const TFunctor& f)
        //{
        //    return d.exist(f);
        //}

        //template <class TFunctor>
        //friend bool operator==(const TFunctor& f, const Delegate& d)
        //{
        //   return (d == f);
        //}

        //template <class TFunctor>
        //friend bool operator!=(const Delegate& d, const TFunctor& f)
        //{
        //    return !(d == f);
        //}

        //template <class TFunctor>
        //friend bool operator!=(const TFunctor& f, const Delegate& d)
        //{
        //    return (d != f);
        //}

        R operator()(LYNX_DELEGATE_FUNCTION_PARAMS) const
        {
            if (this->mLast == NULL)
                return R();

            if (this->mLast->mPrevious != NULL)
                invokeDelegateList(this->mLast->mPrevious LYNX_DELEGATE_COMMA LYNX_DELEGATE_FUNCTION_ARGS);
            return this->mLast->invoke(LYNX_DELEGATE_FUNCTION_ARGS);
        }
    };
} // namesapce Lynx

#undef LYNX_DELEGATE_TEMPLATE_PARAMS
#undef LYNX_DELEGATE_TEMPLATE_ARGS
#undef LYNX_DELEGATE_FUNCTION_PARAMS
#undef LYNX_DELEGATE_FUNCTION_ARGS
#undef LYNX_DELEGATE_COMMA

