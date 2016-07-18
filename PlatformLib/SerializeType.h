#ifndef __LYNX_PLATFORM_LIB_SERIALIZE_TYPE_H__
#define __LYNX_PLATFORM_LIB_SERIALIZE_TYPE_H__

#include "Compat.h"


namespace std // std type declared
{
    template <class FirstType, class SecondType > struct pair;
    template <class Type, class Traits, class AllocatorType > class basic_string;;
    template <class Type, class AllocatorType > class vector;
    template <class Type, class AllocatorType > class deque;
    template <class Type, class AllocatorType > class list;
    template <class KeyType, class CompType, class AllocatorType > class set;
    template <class KeyType, class CompType, class AllocatorType > class multiset;
    template <class KeyType, class MappedType, class CompType, class AllocatorType > class map;
    template <class KeyType, class MappedType, class CompType, class AllocatorType > class multimap;
} // namespace std;

namespace Lynx
{
    template <bool Boolean, class Type = void>
    struct EnableIf { typedef Type type; }; 
    template <class Type> struct EnableIf<false, Type > {};

    template <class Type> struct IsBoolean { static const bool sValue = false; };
    template <> struct IsBoolean<bool> { static const bool sValue = true; };

    //template <class Type> struct IsInteger { static const bool sValue = false; };
    //template <> struct IsInteger<wchar_t> { static const bool sValue = true; };

    //template <> struct IsInteger<Int8> { static const bool sValue = true; };
	template <class Type> struct IsInt8 { static const bool sValue = false; };
	template <> struct IsInt8<Int8> { static const bool sValue = true; };

	//template <> struct IsInteger<UInt8> { static const bool sValue = true; };
	template <class Type> struct IsUInt8 { static const bool sValue = false; };
	template <> struct IsUInt8<UInt8> { static const bool sValue = true; };

    //template <> struct IsInteger<Int16> { static const bool sValue = true; };
	template <class Type> struct IsInt16 { static const bool sValue = false; };
	template <> struct IsInt16<Int16> { static const bool sValue = true; };

	//template <> struct IsInteger<UInt16> { static const bool sValue = true; };
	template <class Type> struct IsUInt16 { static const bool sValue = false; };
	template <> struct IsUInt16<UInt16> { static const bool sValue = true; };

    //template <> struct IsInteger<Int32> { static const bool sValue = true; };
	template <class Type> struct IsInt32 { static const bool sValue = false; };
	template <> struct IsInt32<Int32> { static const bool sValue = true; };

	//template <> struct IsInteger<UInt32> { static const bool sValue = true; };
	template <class Type> struct IsUInt32 { static const bool sValue = false; };
	template <> struct IsUInt32<UInt32> { static const bool sValue = true; };

    //template <> struct IsInteger<Int64> { static const bool sValue = true; };
	template <class Type> struct IsInt64 { static const bool sValue = false; };
	template <> struct IsInt64<Int64> { static const bool sValue = true; };

	//template <> struct IsInteger<UInt64> { static const bool sValue = true; };
	template <class Type> struct IsUInt64 { static const bool sValue = false; };
	template <> struct IsUInt64<UInt64> { static const bool sValue = true; };

#if !defined __x86_64__
    //template <> struct IsInteger<long> { static const bool sValue = true; };
	template <> struct IsInt32<long> { static const bool sValue = true; };
    //template <> struct IsInteger<unsigned long> { static const bool sValue = true; };
	template <> struct IsUInt32<unsigned long> { static const bool sValue = true; };
#endif
//#if !defined(__APPLE__)
//    template <> struct IsInteger<UInt32> { static const bool sValue = true; };
//#endif
    //template <> struct IsInteger<UInt64> { static const bool sValue = true; };

	template <class Type> struct IsPointer { static const bool sValue = false; };
	template <> struct IsPointer<void*> { static const bool sValue = true; };

    template <class Type> struct IsFloat { static const bool sValue = false; };
    template <> struct IsFloat<float> { static const bool sValue = true; };

    template <class Type> struct IsDouble { static const bool sValue = false; };
    template <> struct IsDouble<double> { static const bool sValue = true; };

    template <class Type> struct IsStdBasicString { static const bool sValue = false; };
    template <class Type, class Traits, class AllocatorType>
    struct IsStdBasicString<std::basic_string<Type, Traits, AllocatorType > >
    { static const bool sValue = true; };

    template <class Type>
    struct IsStdVector { static const bool sValue = false; };
    template <class ValueType, class AllocatorType>
    struct IsStdVector<std::vector<ValueType, AllocatorType > > 
    { static const bool sValue = true; };

    template <class Type>
    struct IsStdDeque { static const bool sValue = false; }; 
    template <class ValueType, class AllocatorType>
    struct IsStdDeque<std::deque<ValueType, AllocatorType > > 
    { static const bool sValue = true; };

    template <class Type>
    struct IsStdList { static const bool sValue = false; };
    template <class ValueType, class AllocatorType>
    struct IsStdList<std::list<ValueType, AllocatorType > > 
    { static const bool sValue = true; };

    template <class Type>
    struct IsStdSet { static const bool sValue = false; }; 
    template <class KeyType, class CompType, class AllocatorType>
    struct IsStdSet<std::set<KeyType, CompType, AllocatorType > > 
    { static const bool sValue = true; };

    template <class Type>
    struct IsStdMultiset { static const bool sValue = false; };
    template <class KeyType, class CompType, class AllocatorType>
    struct IsStdMultiset<std::multiset<KeyType, CompType, AllocatorType > > 
    { static const bool sValue = true; };

    //template <class Type, class Enable = void>
    //struct IsStdSetContainer { static const bool sValue = false; }; 
    //template <class Type> struct IsStdSetContainer<Type, 
    //    typename EnableIf<IsStdSet<Type >::sValue || IsStd_Set<Type >::sValue || 
    //    IsStdMultiset<Type >::sValue>::type>
    //{ static const bool sValue = true; };

    template <class Type, class Enable = void>
    struct IsStdSetContainer { static const bool sValue = false; }; 
    template <class Type> struct IsStdSetContainer<Type, 
        typename EnableIf<IsStdSet<Type >::sValue || 
        IsStdMultiset<Type >::sValue>::type>
    { static const bool sValue = true; };


    template <class Type> struct IsStdPair { static const bool sValue = false; };
    template <class FirstType, class SecondType> 
    struct IsStdPair<std::pair<FirstType, SecondType > > 
    { static const bool sValue = true; };

    template <class Type>
    struct IsStdMap { static const bool sValue = false; };
    template <class KeyType, class MappedType, class CompType, class AllocatorType>
    struct IsStdMap<std::map<KeyType, MappedType, CompType, AllocatorType > >
    { static const bool sValue = true; };

    template <class Type>
    struct IsStdMultimap { static const bool sValue = false; };
    template <class KeyType, class MappedType, class CompType, class AllocatorType>
    struct IsStdMultimap<std::multimap<KeyType, MappedType, CompType, AllocatorType > >
    { static const bool sValue = true; };

    //template <class Type, class Enable = void>
    //struct IsStdMapContainer { static const bool sValue = false; }; 
    //template <class Type> struct IsStdMapContainer<Type, 
    //    typename EnableIf<IsStdMap<Type >::sValue || IsStd_Map<Type >::sValue || 
    //    IsStdMultimap<Type >::sValue>::type>
    //{ static const bool sValue = true; };

    template <class Type, class Enable = void>
    struct IsStdMapContainer { static const bool sValue = false; }; 
    template <class Type> struct IsStdMapContainer<Type, 
        typename EnableIf<IsStdMap<Type >::sValue ||
        IsStdMultimap<Type >::sValue>::type>
    { static const bool sValue = true; };

    //template <class Type, class Enable = void>
    //struct IsStdSequenceContainer { static const bool sValue = false; }; 
    //template <class Type> struct IsStdSequenceContainer<Type, 
    //typename EnableIf<IsStdVector<Type >::sValue || IsStdList<Type >::sValue || 
    //IsStdDeque<Type >::sValue || IsStdBasicString<Type >::sValue>::type>
    //{ static const bool sValue = true; };

    // Due to improve string copy performance, we aren't conside string is a container.
    //template <class Type, class Enable = void>
    //struct IsStdSequenceContainer { static const bool sValue = false; }; 
    //template <class Type> struct IsStdSequenceContainer<Type, 
    //typename EnableIf<IsStdVector<Type >::sValue || IsStd_Vector<Type >::sValue || 
    //IsStdList<Type >::sValue || IsStd_List<Type >::sValue || IsStdDeque<Type >::sValue>::type>
    //{ static const bool sValue = true; };

    template <class Type, class Enable = void>
    struct IsStdSequenceContainer { static const bool sValue = false; }; 
    template <class Type> struct IsStdSequenceContainer<Type, 
    typename EnableIf<IsStdVector<Type >::sValue || IsStd_Vector<Type >::sValue || 
    IsStdList<Type >::sValue || IsStdDeque<Type >::sValue>::type>
    { static const bool sValue = true; };


    template <class Type, class Enable = void>
    struct IsStdAssociativeContainer { static const bool sValue = false; };
    template <class Type> struct IsStdAssociativeContainer<Type, 
    typename EnableIf<IsStdSetContainer<Type >::sValue || IsStdMapContainer<Type >::sValue>::type>
    { static const bool sValue = true; };

    template <class Type, class Enable = void>
    struct IsStdContainer { static const bool sValue = false; };
    template <class Type> struct IsStdContainer<Type, 
    typename EnableIf<IsStdSequenceContainer<Type >::sValue || 
    IsStdAssociativeContainer<Type >::sValue>::type>
    { static const bool sValue = true; };
} // namespace Lynx

#endif // __LYNX_PLATFORM_LIB_SERIALIZE_TYPE_H__


