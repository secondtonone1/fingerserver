#ifndef __LYNX_PLATFORM_LIB_MEMORY_MANAGER_H__
#define __LYNX_PLATFORM_LIB_MEMORY_MANAGER_H__

#include "Compat.h"
#include "Mutex.h"
#include "Singleton.h"
#include <map>
namespace Lynx
{

const UInt32 LYNX_MEM_HEAD_LEN    = sizeof(UInt32);
const UInt32 LYNX_MEM_TAIL_LEN    = sizeof(UInt32);
const UInt32 LYNX_MEM_ANNEX_LEN   = (LYNX_MEM_HEAD_LEN + LYNX_MEM_TAIL_LEN);
const UInt32 LYNX_MEM_CHECKCODE   = 0xFEFEFEFE;
const UInt32 LYNX_MEM_ALLOC_PARAM = 0x10;
const UInt32 LYNX_MEM_BASE        = 4;
const UInt32 LYNX_MEM_COUNT       = 32;
const UInt32 LYNX_MEM_MAX_FREE_NODE_SIZE = LYNX_MEM_BASE * LYNX_MEM_COUNT;


    class _PlatformExport MemoryManager : public Singleton<MemoryManager>
    {
    public:
        MemoryManager();
        virtual ~MemoryManager();

        virtual void* alloc(UInt32 size);
        virtual void dealloc(void* p);

		virtual void* allocDebug(char* file, int line, char* function, UInt32 size);
		virtual void deallocDebug(char* file, int line, char* function, void* p);

        inline UInt32 getMemTotle(){ return mMemTotle; }
        inline UInt32 getMemListSzie() { return mMemListSize; }
        void _debugPrint(UInt32 printSize);
    private:
        inline UInt32 roundUp(UInt32 size) 
        {
            return (((size) + (LYNX_MEM_BASE - 1))&~(LYNX_MEM_BASE - 1));
        }

        inline UInt32 getIndex(UInt32 size)
        {
            return (((size) + (LYNX_MEM_BASE - 1)) / LYNX_MEM_BASE - 1); 
        }

        char* allocBlock(UInt32 size, UInt32* n);
        void* refill(UInt32 size);

        union FreeNode 
        {
            union FreeNode* mNext;
            char mData[1];
        } *mFreeList[LYNX_MEM_COUNT];

        struct MemBlock
        {
            struct MemBlock* mNext;
            char* mData; 
        } *mBlockList;

        UInt32 mSize;
        UInt32 mValidSize;
        mutable SpinMutex mMutex;
        char* mFreeMemHead;
        char* mFreeMemTail;

		mutable SpinMutex mMutexDebugInfo;
		struct MemInfo 
		{
			UInt32 size;
			char* file;
			int line;
			char* function;
		};
		typedef std::map<void*, MemInfo> MemDebugInfo;
		MemDebugInfo mMemDebugInfo;

        UInt32 mMemListSize;
        
        UInt32 mMemTotle;

    };
} // namespace Lynx

#if _LYNX_USE_MEM_MANAGER
    #if defined(_WIN32)
        #if defined(_DEBUG)
	        #define XALLOC(size) Lynx::MemoryManager::getSingleton().allocDebug(__FILE__, __LINE__, __FUNCTION__, (size))
	        #define XFREE(ptr) Lynx::MemoryManager::getSingleton().deallocDebug(__FILE__, __LINE__, __FUNCTION__, (ptr))

	        template <typename ObjType >
	        void DestroyFuncDebug(char* file, int line, char* function, ObjType* obj)
	        {
		        obj->~ObjType(); 
		        Lynx::MemoryManager::getSingleton().deallocDebug(file, line, function, obj);
	        }
	        #define XNEW(ObjType) new (XALLOC(sizeof(ObjType))) (ObjType)
	        #define XDELETE(Obj) do { if (Obj) DestroyFuncDebug(__FILE__, __LINE__, __FUNCTION__, Obj); } while (0)
	        #define XSAFE_DELETE(Obj) do { XDELETE(Obj); Obj = NULL; } while(0)
        #else
			#define XALLOC malloc
            #define XFREE free
            #define XNEW new
            #define XDELETE(Obj) do { if (Obj) delete(Obj); } while (0)
            #define XSAFE_DELETE(Obj) do { XDELETE(Obj); Obj = NULL; } while (0)
        #endif
    #else
		#define XALLOC(size) Lynx::MemoryManager::getSingleton().alloc((size))
		#define XFREE(ptr) Lynx::MemoryManager::getSingleton().dealloc((ptr))
		template <typename ObjType >
		void DestroyFunc(ObjType* obj)
		{
			obj->~ObjType(); 
			Lynx::MemoryManager::getSingleton().dealloc(obj);
		}

		#define XNEW(ObjType) new (XALLOC(sizeof(ObjType))) (ObjType)
		#define XDELETE(Obj) do { if (Obj) DestroyFunc(Obj); } while (0)
		#define XSAFE_DELETE(Obj) do { XDELETE(Obj); Obj = NULL; } while(0)
	#endif
#else
	#define XALLOC malloc
	#define XFREE free
	#define XNEW new
	#define XDELETE(Obj) do { if (Obj) delete(Obj); } while (0)
	#define XSAFE_DELETE(Obj) do { XDELETE(Obj); Obj = NULL; } while (0)
#endif 

namespace Lynx
{
    template <UInt32 NodeSize, UInt32 size>
    class MemPool
    {
    public:
        MemPool() : mLength(0) {}
        virtual ~MemPool() 
        {
            for (UInt32 i = 0; i < mLength; ++i)
            {
                if (mNodes[i] != NULL)
                {
                    XFREE(mNodes[i]);
                }
            }
        }

        inline void* alloc()
        {
            if (mLength == 0)
            {
                return XALLOC(NodeSize);
            }
            else
            {
                return mNodes[--mLength];
            }
        }

        inline void dealloc(void* p)
        {
            if (mLength == size)
            {
                XFREE(p);
            }
            else
            {
                mNodes[mLength++] = p;
            }
        }

    protected:
        UInt32 mLength;
        void* mNodes[size];
    };
} // namespace Lynx

namespace Lynx
{
    template <typename Type, UInt32 size = 20>
    class ObjPool : public MemPool<sizeof(Type), size>
    {
    public:
        ObjPool() : MemPool<sizeof(Type), size>() {}
        virtual ~ObjPool() {} 

        Type* pop()
        {
            Type* obj = (Type*)MemPool<sizeof(Type), size>::alloc();
            obj = new (obj) Type();
            return obj;
        }

        template <typename ArgType1>
        Type* pop(ArgType1& arg1)
        {
            Type* obj = (Type*)MemPool<sizeof(Type), size>::alloc();
            obj = new (obj) Type(arg1);
            return obj;
        }

        template <typename ArgType1, typename ArgType2>
        Type* pop(ArgType1& arg1, ArgType2& arg2) 
        {
            Type* obj = (Type*)MemPool<sizeof(Type), size>::alloc();
            obj = new (obj) Type(arg1, arg2);
            return obj;
        }

        template <typename ArgType1, typename ArgType2, typename ArgType3>
        Type* pop(ArgType1& arg1, ArgType2& arg2, ArgType3& arg3) 
        {
            Type* obj = (Type*)MemPool<sizeof(Type), size>::alloc();
            obj = new (obj) Type(arg1, arg2, arg3);
            return obj;
        }

        template <typename ArgType1, typename ArgType2, 
            typename ArgType3, typename ArgType4>
            Type* pop(ArgType1& arg1, ArgType2& arg2, ArgType3& arg3, ArgType4& arg4) 
        {
            Type* obj = (Type*)MemPool<sizeof(Type), size>::alloc();
            obj = new (obj) Type(arg1, arg2, arg3, arg4);
            return obj;
        }

        template <typename ArgType1, typename ArgType2, typename ArgType3, 
            typename ArgType4, typename ArgType5>
            Type* pop(ArgType1& arg1, ArgType2& arg2, ArgType3& arg3, ArgType4& arg4, ArgType5& arg5) 
        {
            Type* obj = (Type*)MemPool<sizeof(Type), size>::alloc();
            obj = new (obj) Type(arg1, arg2, arg3, arg4, arg5);
            return obj;
        }

        template <typename ArgType1, typename ArgType2, typename ArgType3, 
            typename ArgType4, typename ArgType5, typename ArgType6>
            Type* pop(ArgType1& arg1, ArgType2& arg2, ArgType3& arg3, 
            ArgType4& arg4, ArgType5& arg5, ArgType6& arg6) 
        {
            Type* obj = (Type*)MemPool<sizeof(Type), size>::alloc();
            obj = new (obj) Type(arg1, arg2, arg3, arg4, arg5, arg6);
            return obj;
        }

        void push(void* obj)
        {
            if (obj == NULL) return;
            ((Type*)obj)->~Type();
            MemPool<sizeof(Type), size>::dealloc((Type*)obj);
        }
    };
} // namespace Lynx

namespace Lynx
{
    template <typename T>
    class StlAllocator
    {
    public:
        typedef T value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        template <typename U>
        struct rebind
        {
            typedef StlAllocator<U> other;
        };

        inline explicit StlAllocator() {}
        inline ~StlAllocator() {}
        inline StlAllocator(StlAllocator const&) {}
        template<typename U > 
        inline StlAllocator(StlAllocator<U > const&) {}
        inline pointer address(reference r) { return &r; }
        inline const_pointer address(const_reference r) { return &r; }

        inline pointer allocate(size_type count, typename std::allocator<void>::const_pointer = 0)
        {
            return (pointer)XALLOC(count * sizeof(T));
        }

        inline void deallocate(pointer p, size_type size)
        {
            XFREE(p);
        }

        inline size_type max_size() const
        {
            return UINT_MAX / sizeof(T);
        }

        inline void construct(pointer p, const T& t) 
        { 
            new (static_cast<void*>(p)) T (t); 
        }

        inline void destroy(pointer p) 
        { 
            p->~T(); 
        }

        inline bool operator == (StlAllocator const&) const 
        { 
            return true; 
        }

        inline bool operator != (StlAllocator const& a) const 
        { 
            return !operator == (a); 
        }
    };

    /// determine equality, can memory from another allocator
	/// be released by this allocator, (ISO C++)
	template<typename T, typename T2>
	inline bool operator == (StlAllocator<T> const&, StlAllocator<T2> const&)
	{
		// same alloc policy (P), memory can be freed
		return true;
	}

	/// determine equality, can memory from another allocator
	/// be released by this allocator, (ISO C++)
	template<typename T, typename OtherAllocator>
	inline bool operator == (StlAllocator<T> const&, OtherAllocator const&)
	{
		return false;
	}

	/// determine equality, can memory from another allocator
	/// be released by this allocator, (ISO C++)
	template<typename T, typename T2>
	inline bool operator != (StlAllocator<T> const&, StlAllocator<T2> const&)
	{
		// same alloc policy (P), memory can be freed
		return false;
	}

	/// determine equality, can memory from another allocator
	/// be released by this allocator, (ISO C++)
	template<typename T, typename OtherAllocator>
	inline bool operator != (StlAllocator<T> const&, OtherAllocator const&)
	{
		return true;
	}

#ifdef _LYNX_USE_MEM_MANAGER
    typedef std::basic_string<char, std::char_traits<char>, StlAllocator<char> > _StringBase;
    //typedef std::basic_string<char, std::char_traits<char> > _StringBase;
    typedef std::basic_stringstream<char, std::char_traits<char>, StlAllocator<char> > _StringStreamBase;
    //typedef std::basic_stringstream<char, std::char_traits<char> > _StringStreamBase;

    typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, StlAllocator<wchar_t> > _StringBaseW;
    typedef std::basic_stringstream<wchar_t,std::char_traits<wchar_t>, StlAllocator<wchar_t> > _StringStreamBaseW;

    #define StdStringT(T) std::basic_string<T, std::char_traits<T>, std::allocator<T> >	
    #define CustomMemoryStringT(T) std::basic_string<T, std::char_traits<T>, StlAllocator<T> >	

    template<typename T>
    bool operator <(const CustomMemoryStringT(T)& l,const StdStringT(T)& o)
    {
        return l.compare(0,l.length(),o.c_str(),o.length())<0;
    }

    template<typename T>
    bool operator <(const StdStringT(T)& l,const CustomMemoryStringT(T)& o)
    {
        return l.compare(0,l.length(),o.c_str(),o.length())<0;
    }

    template<typename T>
    bool operator <=(const CustomMemoryStringT(T)& l,const StdStringT(T)& o)
    {
        return l.compare(0,l.length(),o.c_str(),o.length())<=0;
    }

    template<typename T>
    bool operator <=(const StdStringT(T)& l,const CustomMemoryStringT(T)& o)
    {
        return l.compare(0,l.length(),o.c_str(),o.length())<=0;
    }

    template<typename T>
    bool operator >(const CustomMemoryStringT(T)& l,const StdStringT(T)& o)
    {
        return l.compare(0,l.length(),o.c_str(),o.length())>0;
    }

    template<typename T>
    bool operator >(const StdStringT(T)& l,const CustomMemoryStringT(T)& o)
    {
        return l.compare(0,l.length(),o.c_str(),o.length())>0;
    }

    template<typename T>
    bool operator >=(const CustomMemoryStringT(T)& l,const StdStringT(T)& o)
    {
        return l.compare(0,l.length(),o.c_str(),o.length())>=0;
    }

    template<typename T>
    bool operator >=(const StdStringT(T)& l,const CustomMemoryStringT(T)& o)
    {
        return l.compare(0,l.length(),o.c_str(),o.length())>=0;
    }

    template<typename T>
    bool operator ==(const CustomMemoryStringT(T)& l,const StdStringT(T)& o)
    {
        return l.compare(0,l.length(),o.c_str(),o.length())==0;
    }
    template<typename T>
    bool operator ==(const StdStringT(T)& l,const CustomMemoryStringT(T)& o)
    {
        return l.compare(0,l.length(),o.c_str(),o.length())==0;
    }

    template<typename T>
    bool operator !=(const CustomMemoryStringT(T)& l,const StdStringT(T)& o)
    {
        return l.compare(0,l.length(),o.c_str(),o.length())!=0;
    }

    template<typename T>
    bool operator !=(const StdStringT(T)& l,const CustomMemoryStringT(T)& o)
    {
        return l.compare(0,l.length(),o.c_str(),o.length())!=0;
    }

    template<typename T>
    CustomMemoryStringT(T) operator +=(const CustomMemoryStringT(T)& l,const StdStringT(T)& o)
    {
        return CustomMemoryStringT(T)(l)+=o.c_str();
    }
    template<typename T>
    CustomMemoryStringT(T) operator +=(const StdStringT(T)& l,const CustomMemoryStringT(T)& o)
    {
        return CustomMemoryStringT(T)(l.c_str())+=o.c_str();
    }

    template<typename T>
    CustomMemoryStringT(T) operator +(const CustomMemoryStringT(T)& l,const StdStringT(T)& o)
    {
        return CustomMemoryStringT(T)(l)+=o.c_str();
    }

    template<typename T>
    CustomMemoryStringT(T) operator +(const StdStringT(T)& l,const CustomMemoryStringT(T)& o)
    {
        return CustomMemoryStringT(T)(l.c_str())+=o.c_str();
    }

    template<typename T>
    CustomMemoryStringT(T) operator +(const T* l,const CustomMemoryStringT(T)& o)
    {
        return CustomMemoryStringT(T)(l)+=o;
    }

    #undef StdStringT
    #undef CustomMemoryStringT
#else
    typedef std::basic_string<char, std::char_traits<char> > _StringBase;
    typedef std::basic_stringstream<char, std::char_traits<char> > _StringStreamBase;

    typedef std::basic_string<wchar_t, std::char_traits<wchar_t> > _StringBaseW;
    typedef std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > _StringStreamBaseW;
#endif

    typedef _StringBase BasicString;
    typedef _StringBase String;
    typedef _StringStreamBase StringStream;
    typedef StringStream stringstream;

    typedef _StringBaseW BasicStringW;
    typedef _StringBaseW StringW;
    typedef _StringStreamBaseW StringStreamW;
    typedef StringStreamW stringstreamw;
} // namespace Lynx

namespace Lynx
{
#define Pair std::pair

#if _LYNX_USE_MEM_MANAGER
    template <typename T> 
    struct Vector : public std::vector<T, StlAllocator<T> >
	{ 
        inline Vector() : std::vector<T, StlAllocator<T> >() {}
        inline Vector(size_t count) : std::vector<T, StlAllocator<T> >(count) {}
        inline Vector(size_t count, const T& value) : std::vector<T, StlAllocator<T> >(count, value) {}
	}; 
#else
    template <typename T> 
    struct Vector : public std::vector<T>
	{ 
        inline Vector() : std::vector<T>() {}
        inline Vector(size_t count) : std::vector<T>(count) {}
        inline Vector(size_t count, const T& value) : std::vector<T>(count, value) {}
	}; 
#endif

//#ifdef _LYNX_USE_MEM_MANAGER
//    template <typename T >
//    class List : public std::list<T, StlAllocator<T > >
//    {
//    };
//#else
//    template <typename T >
//    class std_list : public std::list<T >
//    {
//    };
//#endif
//
//#ifdef _LYNX_USE_MEM_MANAGER
//    template <typename _K, typename _V, typename P = std::less<_K> >
//    class Map : public std::map<K, V, P, kgame_allocator<std::pair<const K, V> > >
//    {   
//    };
//#else
//    template <typename _K, typename _V, typename _P = std::less<K> >
//    class Map : public std::map<K, V, P>
//    {   
//    };
//#endif
//
//#ifdef _LYNX_USE_MEM_MANAGER
//    template <typename _K, typename _P = std::less<K> >
//    class Set : public std::set<K, P, kgame_allocator<K> >
//    {
//    };
//#else
//    template <typename K, typename P = std::less<K> >
//    class Set : public std::set<K, P >
//    {
//    };
//#endif
} // namespace Lynx

template <class Type >
struct IsStd_Vector { static const bool sValue = false; };
template <class ValueType >
struct IsStd_Vector<Lynx::Vector<ValueType > > { static const bool sValue = true; };

//template <class Type >
//struct IsStd_List { static const bool sValue = false; };
//template <class ValueType >
//struct IsStd_List<Lynx::List<ValueType > > { static const bool sValue = true; };
//
//template <class Type>
//struct IsStd_Map { static const bool sValue = false; };
//template <class KeyType, class MappedType, class CompType >
//struct IsStd_Map<Lynx::Map<KeyType, MappedType, CompType> >
//{ static const bool sValue = true; };
//
//template <class Type>
//struct IsStd_Set { static const bool sValue = false; }; 
//template <class KeyType, class CompType>
//struct IsStd_Set<Lynx::Set<KeyType, CompType> > 
//{ static const bool sValue = true; };

#endif // __LYNX_PLATFORM_LIB_MEMORY_MANAGER_H__



