#ifndef __LYNX_PLATFORM_LIB_STREAMBUFFER_SERIALIZE_IMPLS_H__
#define __LYNX_PLATFORM_LIB_STREAMBUFFER_SERIALIZE_IMPLS_H__

#include <vector>
#include <string>
#include <list>
#include <map>
#include <set>

#include "ByteOrder.h"
#include "Blob.h"
#include "MathUtil.h"
#include "StreamBuffer.h"
#include "Container.h"
#include "SerializeImpls.h"

#define LYNX_FLOAT_PRECISION  1000000
#define LYNX_DOUBLE_PRECISION 1000000000

#define SERVER_CLIENT_XOR_CODE 127 //服务器和客户端对于大于255个字符的包的异或码,这个异或码必须是小于255，并且大于0的数

namespace Lynx
{
    template <class Type> struct IsLynxStreamBufferData 
    { 
        static const bool sValue = false; 
    };

    template <> struct IsLynxStreamBufferData<Lynx::StreamBuffer> 
    { 
        static const bool sValue = true; 
    };

	template <class DataType >
    struct S11NPointerImpls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            return sizeof(void*); 
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
			data << obj;
            return sizeof(void*);
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            if (data.length() < sizeof(void*))
                return false;
            data >> obj;
            return true;
        }
    };

    template <class DataType >
    struct S11NBooleanImpls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            return sizeof(obj); 
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            data << obj;
            return sizeof(ObjType);
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            if (data.length() < sizeof(ObjType))
                return false;
            data >> obj;
            return true;
        }
    };

    template <class DataType >
    struct S11NInt8Impls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            return sizeof(obj); 
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ObjType i = obj;
            if (mask != 0)
            {
                i = obj ^ (ObjType)(mask + 0);
            }

#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            data << i;
#else
            data << EndianReverse<ObjType >::convert(i);
#endif
            return sizeof(ObjType);
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            if (data.length() < sizeof(ObjType))
                return false;

            data >> obj;
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            obj = EndianReverse<ObjType >::convert(obj);
#endif
            if (mask != 0)
            {
                obj ^= (ObjType)(mask + 0);
            }
            return true;
        }
    };

	template <class DataType >
    struct S11NUInt8Impls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            return sizeof(obj); 
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ObjType i = obj;
            if (mask != 0)
            {
                i = obj ^ (ObjType)(mask + 0);
            }

#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            data << i;
#else
            data << EndianReverse<ObjType >::convert(i);
#endif
            return sizeof(ObjType);
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            if (data.length() < sizeof(ObjType))
                return false;

            data >> obj;
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            obj = EndianReverse<ObjType >::convert(obj);
#endif
            if (mask != 0)
            {
                obj ^= (ObjType)(mask + 0);
            }
            return true;
        }
    };

	template <class DataType >
    struct S11NInt16Impls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            return sizeof(obj); 
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ObjType i = obj;
            if (mask != 0)
            {
                i = obj ^ (ObjType)(mask + 0);
            }

#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            data << i;
#else
            data << EndianReverse<ObjType >::convert(i);
#endif
            return sizeof(ObjType);
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            if (data.length() < sizeof(ObjType))
                return false;

            data >> obj;
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            obj = EndianReverse<ObjType >::convert(obj);
#endif
            if (mask != 0)
            {
                obj ^= (ObjType)(mask + 0);
            }
            return true;
        }
    };

	template <class DataType >
    struct S11NUInt16Impls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            return sizeof(obj); 
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ObjType i = obj;
            if (mask != 0)
            {
                i = obj ^ (ObjType)(mask + 0);
            }

#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            data << i;
#else
            data << EndianReverse<ObjType >::convert(i);
#endif
            return sizeof(ObjType);
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            if (data.length() < sizeof(ObjType))
                return false;

            data >> obj;
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            obj = EndianReverse<ObjType >::convert(obj);
#endif
            if (mask != 0)
            {
                obj ^= (ObjType)(mask + 0);
            }
            return true;
        }
    };

	template <class DataType >
    struct S11NInt32Impls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            return sizeof(obj); 
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ObjType i = obj;
            if (mask != 0)
            {
                i = obj ^ (ObjType)(mask + 0);
            }

#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            data << i;
#else
            data << EndianReverse<ObjType >::convert(i);
#endif
            return sizeof(ObjType);
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            if (data.length() < sizeof(ObjType))
                return false;

            data >> obj;
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            obj = EndianReverse<ObjType >::convert(obj);
#endif
            if (mask != 0)
            {
                obj ^= (ObjType)(mask + 0);
            }
            return true;
        }
    };

	template <class DataType >
    struct S11NUInt32Impls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            return sizeof(obj); 
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ObjType i = obj;
            if (mask != 0)
            {
                i = obj ^ (ObjType)(mask + 0);
            }

#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            data << i;
#else
            data << EndianReverse<ObjType >::convert(i);
#endif
            return sizeof(ObjType);
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            if (data.length() < sizeof(ObjType))
                return false;

            data >> obj;
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            obj = EndianReverse<ObjType >::convert(obj);
#endif
            if (mask != 0)
            {
                obj ^= (ObjType)(mask + 0);
            }
            return true;
        }
    };

	template <class DataType >
    struct S11NInt64Impls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            return sizeof(obj); 
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
			
            ObjType i = obj;
            if (mask != 0)
            {
                i = obj ^ (ObjType)(mask + 0);
            }

#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            data << i;
#else
            data << EndianReverse<ObjType >::convert(i);
#endif
            return sizeof(ObjType);
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            if (data.length() < sizeof(ObjType))
                return false;

            data >> obj;
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            obj = EndianReverse<ObjType >::convert(obj);
#endif
            if (mask != 0)
            {
                obj ^= (ObjType)(mask + 0);
            }
            return true;
        }
    };

	template <class DataType >
    struct S11NUInt64Impls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            return sizeof(obj); 
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
			
			ObjType i = obj;
            if (mask != 0)
            {
                i = obj ^ (ObjType)(mask + 0);
            }

#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            data << i;
#else
            data << EndianReverse<ObjType >::convert(i);
#endif
           
			return sizeof(ObjType);
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            if (data.length() < sizeof(ObjType))
                return false;

            data >> obj;
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            obj = EndianReverse<ObjType >::convert(obj);
#endif
            if (mask != 0)
            {
                obj ^= (ObjType)(mask + 0);
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NFloatImpls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            return sizeof(obj); 
#else
            return sizeof(obj) * 2;
#endif
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            data << obj;
            return sizeof(obj);
#else
            ASSERT(sizeof(ObjType) == 4);
            Int32 integer = (Int32)obj;
            Int32 decimal = (Int32)((obj - integer) * LYNX_FLOAT_PRECISION);
            data << EndianReverse<Int32>::convert(integer);
            data << EndianReverse<Int32>::convert(decimal);
            return sizeof(obj) * 2;
#endif
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            if (data.length() < sizeof(ObjType))
                return false;
            data >> obj;
#else
            ASSERT(sizeof(ObjType) == 4);
            if (data.length() < sizeof(ObjType) * 2)
                return false;
            Int32 integer, decimal;
            data >> integer;
            integer = EndianReverse<Int32>::convert(integer);
            data >> decimal;
            decimal = EndianReverse<Int32>::convert(decimal);
            obj = (ObjType)integer;
            obj += ((ObjType)(decimal)) / LYNX_FLOAT_PRECISION;
#endif
            return true;
        }
    };

    /*template <class DataType >
    struct S11NDoubleImpls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            return sizeof(obj); 
#else
            return sizeof(obj) * 2;
#endif
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            data << obj;
            return sizeof(obj);
#else
            Int64 integer = (Int64)obj;
            Int64 decimal = (Int64)((obj - integer) * LYNX_DOUBLE_PRECISION);
            data << EndianReverse<Int64>::convert(integer);
            data << EndianReverse<Int64>::convert(decimal);
            return sizeof(obj) * 2;
#endif
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            if (data.length() < sizeof(ObjType))
                return false;
            data >> obj;
#else
            ASSERT(sizeof(ObjType) == 8);
            if (data.length() < sizeof(ObjType) * 2)
                return false;
            Int64 integer, decimal;
            data >> integer;
            integer = EndianReverse<Int64>::convert(integer);
            data >> decimal;
            decimal = EndianReverse<Int64>::convert(decimal);
            obj = (ObjType)integer;
            obj += ((ObjType)(decimal)) / LYNX_DOUBLE_PRECISION;
#endif
            return true;
        }
    };*/

    template <class DataType >
    struct S11NStdBasicStringImpls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            UInt32 size = sizeof(UInt32);
            size += (UInt32)obj.size();
            return size;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
			
			typedef typename ObjType::value_type ElemType;
            UInt32 size = (UInt32)obj.size();

            if (mask != 0)
            {
                size ^= (UInt32)(mask + 0);
            }
#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            data << size;
#else
            data << EndianReverse<UInt32>::convert(size);

#endif
            if (mask != 0)
            {
                //长度对255取余数，如果余数为零，那么将长度（用于异或字符）设置为1
				Int32 strLenth = obj.size();
				
				if(strLenth > 255)
				{
					strLenth = SERVER_CLIENT_XOR_CODE;
				}
				else if(strLenth == 255)
				{
					strLenth = 1;
				}

				for (Int32 i = 0; i < (Int32)obj.size(); i++)
                {
                    data << (ElemType)(obj[i] ^ (ElemType)(strLenth));
                }
                
				return sizeof(UInt32) + (UInt32)obj.size();

            }
            else
            {
                data.write((Int8 const *)obj.c_str(), (UInt32)obj.size() * sizeof(ElemType));

                return sizeof(UInt32) + (UInt32)obj.size();
            }
			
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            if (data.length() < sizeof(UInt32))
                return false;

            UInt32 size;
            data >> size;
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            size = EndianReverse<UInt32>::convert(size);
#endif
            if (mask != 0)
            {
                size ^= (UInt32)(mask + 0);
            }

            if (data.length() < size) 
                return false;

            if (mask != 0)
            {
				//长度对255取余数，如果余数为零，那么将长度（用于异或字符）设置为1
				Int32 strLenth = size;
				
				if(strLenth > 255)
				{
					strLenth = SERVER_CLIENT_XOR_CODE;
				}
				else if(strLenth == 255)
				{
					strLenth = 1;
				}

				ElemType c;
                for (UInt32 i = 0; i < size; i++)
                {
                    data >> c; 
                    obj.push_back(c ^ (ElemType)(strLenth));
                }
            }
            else
            {
                ElemType tmpbuf[2048];
                UInt32 readlen;
                obj.clear();
                while (size > 0) 
                {
                    readlen = size < 2046 ? size : 2046; 
                    data.read(tmpbuf, readlen * sizeof(ElemType)); 
                    tmpbuf[readlen] = 0;
                    obj += tmpbuf;
                    size -= readlen;
                }
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NStdPairImpls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            typedef typename ObjType::first_type FirstType;
            typedef typename ObjType::second_type SecondType;

            return S11NTraits<FirstType >::size(data, obj.first) + 
                S11NTraits<SecondType >::size(data, obj.second);
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::first_type FirstType;
            typedef typename ObjType::second_type SecondType;

            return S11NTraits<FirstType >::serialize(data, (FirstType&)obj.first, name, mask) 
                + S11NTraits<SecondType >::serialize(data, (SecondType&)obj.second, name, mask);
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::first_type FirstType;
            typedef typename ObjType::second_type SecondType;

            return S11NTraits<FirstType >::unserialize(data, obj.first, name, mask) &&
                S11NTraits<SecondType >::unserialize(data, obj.second, name, mask);
        }
    };

    template <class DataType >
    struct S11NStdMapImpls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            UInt32 size = sizeof(UInt32);
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
                size += S11NTraits<ElemType >::size(data, *it);
            return size;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            UInt32 size = obj.size();
            if (mask != 0)
            {
                size ^= (UInt32)(mask + 0);
            }

#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            data << size;
#else
            data << EndianReverse<UInt32>::convert(size);
#endif
            UInt32 len = 0;
            len += sizeof(UInt32);
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
                len += S11NTraits<ElemType >::serialize(data, (ElemType&)*it, name, mask);
            return len;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            if (data.length() < sizeof(UInt32))
                return false;
            UInt32 size;
            data >> size;
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            size = EndianReverse<UInt32>::convert(size);
#endif
            if (mask != 0)
            {
                size ^= (UInt32)(mask + 0);
            }

            obj.clear();
            for (; size; --size)
            {
                ElemType elem;
                if (!S11NTraits<ElemType >::unserialize(data, elem, name, mask))
                    return false;
                obj.insert(obj.end(), elem);
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NStdVectorImpls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            UInt32 size = sizeof(UInt32);
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
                size += S11NTraits<ElemType >::size(data, *it);
            return size;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            UInt32 size = (UInt32)obj.size();
            if (mask != 0)
            {
                size ^= (UInt32)(mask + 0);
            }

#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            data << size;
#else
            data << EndianReverse<UInt32>::convert(size);
#endif
            UInt32 len = 0;
            len += sizeof(UInt32);
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
                len += S11NTraits<ElemType >::serialize(data, (ElemType&)*it, name, mask);
            return len;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            if (data.length() < sizeof(UInt32))
                return false;
            UInt32 size;
            data >> size;
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            size = EndianReverse<UInt32>::convert(size);
#endif
            if (mask != 0)
            {
                size ^= (UInt32)(mask + 0);
            }

            obj.clear();
            for (; size; --size)
            {
                ElemType elem;
                if (!S11NTraits<ElemType >::unserialize(data, elem, name, mask))
                    return false;
                obj.insert(obj.end(), elem);
            }
            return true;
        }
    };

	template <class DataType >
    struct S11NStdListImpls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            UInt32 size = sizeof(UInt32);
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
                size += S11NTraits<ElemType >::size(data, *it);
            return size;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            UInt32 size = (UInt32)obj.size();
            if (mask != 0)
            {
                size ^= (UInt32)(mask + 0);
            }

#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            data << size;
#else
            data << EndianReverse<UInt32>::convert(size);
#endif
            UInt32 len = 0;
            len += sizeof(UInt32);
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
                len += S11NTraits<ElemType >::serialize(data, (ElemType&)*it, name, mask);
            return len;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            if (data.length() < sizeof(UInt32))
                return false;
            UInt32 size;
            data >> size;
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            size = EndianReverse<UInt32>::convert(size);
#endif
            if (mask != 0)
            {
                size ^= (UInt32)(mask + 0);
            }

            obj.clear();
            for (; size; --size)
            {
                ElemType elem;
                if (!S11NTraits<ElemType >::unserialize(data, elem, name, mask))
                    return false;
                obj.insert(obj.end(), elem);
            }
            return true;
        }
    };

	template <class DataType >
    struct S11NStdDequeImpls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            UInt32 size = sizeof(UInt32);
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
                size += S11NTraits<ElemType >::size(data, *it);
            return size;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            UInt32 size = (UInt32)obj.size();
            if (mask != 0)
            {
                size ^= (UInt32)(mask + 0);
            }

#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            data << size;
#else
            data << EndianReverse<UInt32>::convert(size);
#endif
            UInt32 len = 0;
            len += sizeof(UInt32);
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
                len += S11NTraits<ElemType >::serialize(data, (ElemType&)*it, name, mask);
            return len;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            if (data.length() < sizeof(UInt32))
                return false;
            UInt32 size;
            data >> size;
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            size = EndianReverse<UInt32>::convert(size);
#endif
            if (mask != 0)
            {
                size ^= (UInt32)(mask + 0);
            }

            obj.clear();
            for (; size; --size)
            {
                ElemType elem;
                if (!S11NTraits<ElemType >::unserialize(data, elem, name, mask))
                    return false;
                obj.insert(obj.end(), elem);
            }
            return true;
        }
    };

	template <class DataType >
    struct S11NStdSetImpls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            UInt32 size = sizeof(UInt32);
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
                size += S11NTraits<ElemType >::size(data, *it);
            return size;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            UInt32 size = (UInt32)obj.size();
            if (mask != 0)
            {
                size ^= (UInt32)(mask + 0);
            }

#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            data << size;
#else
            data << EndianReverse<UInt32>::convert(size);
#endif
            UInt32 len = 0;
            len += sizeof(UInt32);
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
                len += S11NTraits<ElemType >::serialize(data, (ElemType&)*it, name, mask);
            return len;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            if (data.length() < sizeof(UInt32))
                return false;
            UInt32 size;
            data >> size;
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            size = EndianReverse<UInt32>::convert(size);
#endif
            if (mask != 0)
            {
                size ^= (UInt32)(mask + 0);
            }

            obj.clear();
            for (; size; --size)
            {
                ElemType elem;
                if (!S11NTraits<ElemType >::unserialize(data, elem, name, mask))
                    return false;
                obj.insert(obj.end(), elem);
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NLynxBufferImpls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            return sizeof(UInt32) + obj.length();
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            UInt32 len = obj.length();
            UInt32 len1 = len;
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            len1 = EndianReverse<UInt32>::convert(len);
#endif
            if (mask != 0)
            {
                len1 ^= (UInt32)(mask + 0);
            }

            data << (UInt32) len1;
            data += obj;
            return sizeof(UInt32) + len;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            if (data.length() < sizeof(UInt32))
                return false;
            UInt32 len;
            data >> len;

            if (mask != 0)
            {
                len ^= (UInt32)(mask + 0);
            }

#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            len = EndianReverse<UInt32>::convert(len);
#endif
            if (data.length() < len)
                return false;
            data.read(obj, len);
            return true;
        }
    };

    template <class DataType >
    struct S11NLynxBlobImpls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            return sizeof(obj.mLen) + obj.mLen;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            UInt32 len = obj.mLen;
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            len = EndianReverse<UInt32>::convert(obj.mLen);
#endif
            if (mask != 0)
            {
                len ^= (UInt32)(mask + 0);
            }

            data << (UInt32) len;
            data.write(obj.mData, obj.mLen);
            return sizeof(UInt32) + obj.mLen;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            if (data.length() < sizeof(obj.mLen))
                return false;

            data >> obj.mLen;
            if (mask != 0)
            {
                obj.mLen ^= (UInt32)(mask + 0);
            }
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
             obj.mLen = EndianReverse<UInt32>::convert(obj.mLen);
#endif
             
             if (data.length() < obj.mLen)
                 return false;
             data.read(obj.mData, obj.mLen);
            return true;
        }
    };

	template <class DataType >
    struct S11NLynxCharArrayImpls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            return sizeof(UInt32) + obj.length();
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            UInt32 len = obj.length();
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            len = EndianReverse<UInt32>::convert(len);
#endif
            if (mask != 0)
            {
                len ^= (UInt32)(mask + 0);
            }

            data << (UInt32) len;

            if (mask != 0)
            {
                for (Int32 i = 0; i < (Int32)obj.length(); i++)
                {
                    data << (char)(obj[i] ^ (char)(obj.length() + 0));
                }
                return sizeof (UInt32) + obj.length();
            }
            else
            {
                data.write(obj.c_str(), obj.length());
                return sizeof(UInt32) + obj.length();
            }
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            if (data.length() < sizeof(obj.length()))
                return false;

            UInt32 len;
            data >> len;
            if (mask != 0)
            {
                len ^= (UInt32)(mask + 0);
            }
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
             len = EndianReverse<UInt32>::convert(len);
#endif
             
             if (data.length() < len)
                 return false;

             if (mask != 0)
            {
                char c;
                for (UInt32 i = 0; i < len; i++)
                {
                    data >> c; 
                    obj += (c ^ (char)(len + 0));
                }
            }
            else
            {
                char tmpbuf[2048];
                UInt32 readlen;
                while (len > 0) 
                {
                    readlen = len < 2046 ? len : 2046; 
                    data.read(tmpbuf, readlen * sizeof(char)); 
                    tmpbuf[readlen] = 0;
                    obj += tmpbuf;
                    len -= readlen;
                }
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NLynxFVector2Impls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            return S11NTraits<float >::size(data, obj.mX) + S11NTraits<float >::size(data, obj.mY);
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            return S11NTraits<float >::serialize(data, obj.mX, name, mask) +
                S11NTraits<float >::serialize(data, obj.mY, name, mask);
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            if (!S11NTraits<float >::unserialize(data, obj.mX, name, mask))
                    return false;
            if (!S11NTraits<float >::unserialize(data, obj.mY, name, mask))
                    return false;
            return true;
        }
    };

    template <class DataType >
    struct S11NLynxFVector3Impls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            return S11NTraits<float >::size(data, obj.mX) + S11NTraits<float >::size(data, obj.mY) + S11NTraits<float >::size(data, obj.mZ);
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            return S11NTraits<float >::serialize(data, obj.mX, name, mask) +
                S11NTraits<float >::serialize(data, obj.mY, name, mask) +
                S11NTraits<float >::serialize(data, obj.mZ, name, mask);
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            if (!S11NTraits<float >::unserialize(data, obj.mX, name, mask))
                    return false;
            if (!S11NTraits<float >::unserialize(data, obj.mY, name, mask))
                    return false;
            if (!S11NTraits<float >::unserialize(data, obj.mZ, name, mask))
                    return false;
            return true;
        }
    };

	template <class DataType >
    struct S11NLynxFVector4Impls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            return S11NTraits<float >::size(data, obj.mX) + S11NTraits<float >::size(data, obj.mY) 
				+ S11NTraits<float >::size(data, obj.mZ) + S11NTraits<float >::size(data, obj.mW);
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            return S11NTraits<float >::serialize(data, obj.mX, name, mask) +
                S11NTraits<float >::serialize(data, obj.mY, name, mask) +
                S11NTraits<float >::serialize(data, obj.mZ, name, mask) +
				S11NTraits<float >::serialize(data, obj.mW, name, mask);
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            if (!S11NTraits<float >::unserialize(data, obj.mX, name, mask))
                    return false;
            if (!S11NTraits<float >::unserialize(data, obj.mY, name, mask))
                    return false;
            if (!S11NTraits<float >::unserialize(data, obj.mZ, name, mask))
                    return false;
			if (!S11NTraits<float >::unserialize(data, obj.mW, name, mask))
			        return false;
            return true;
        }
    };

    /*template <class DataType >
    struct S11NLynxColorImpls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            return S11NTraits<float >::size(data, obj.mR) + S11NTraits<float >::size(data, obj.mG) 
                + S11NTraits<float >::size(data, obj.mB) + S11NTraits<float >::size(data, obj.mA);
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            return S11NTraits<float >::serialize(data, obj.mR, name, mask) +
                S11NTraits<float >::serialize(data, obj.mG, name, mask) +
                S11NTraits<float >::serialize(data, obj.mB, name, mask) +
                S11NTraits<float >::serialize(data, obj.mA, name, mask);
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            if (!S11NTraits<float >::unserialize(data, obj.mR, name, mask))
                return false;
            if (!S11NTraits<float >::unserialize(data, obj.mG, name, mask))
                return false;
            if (!S11NTraits<float >::unserialize(data, obj.mB, name, mask))
                return false;
            if (!S11NTraits<float >::unserialize(data, obj.mA, name, mask))
                return false;
            return true;
        }
    };*/


    template <class DataType >
    struct S11NLynxListImpls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::iter_type IterType;

            UInt32 size = sizeof(UInt32);
            for (IterType* node = obj.begin(); node != obj.end(); node = obj.next(node))
                size += S11NTraits<ElemType >::size(data, node->mValue);
            return size;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::iter_type IterType;

            UInt32 size = obj.size();
            if (mask != 0)
            {
                size ^= (UInt32)(mask + 0);
            }
#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            data << size;
#else
            data << EndianReverse<UInt32>::convert(size);
#endif

            UInt32 len = 0;
            len += sizeof(UInt32);

            for (IterType* node = obj.begin(); node != obj.end(); node = obj.next(node))
                len += S11NTraits<ElemType >::serialize(data, node->mValue, name, mask);
            return len;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::iter_type IterType;

            if (data.length() < sizeof(UInt32))
                return false;
            UInt32 size;
            data >> size;
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            size = EndianReverse<UInt32>::convert(size);
#endif
            if (mask != 0)
            {
                size ^= (UInt32)(mask + 0);
            }

            obj.clear();
            for (; size; --size)
            {
                ElemType elem;
                if (!S11NTraits<ElemType >::unserialize(data, elem, name, mask))
                    return false;
                obj.insertTail(elem);
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NLynxMapImpls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;
            typedef typename ObjType::iter_type IterType;

            UInt32 size = sizeof(UInt32);
            for (IterType* node = obj.begin(); node != obj.end(); node = obj.next(node))
                size += S11NTraits<ElemType >::size(data, std::make_pair(node->mKey, node->mValue));
            return size;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;
            typedef typename ObjType::iter_type IterType;

            UInt32 size = obj.size();
            if (mask != 0)
            {
                size ^= (UInt32)(mask + 0);
            }
#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            data << size;
#else
            data << EndianReverse<UInt32>::convert(size);
#endif

            UInt32 len = 0;
            len += sizeof(UInt32);

            for (IterType* node = obj.begin(); node != obj.end(); node = obj.next(node))
                len += S11NTraits<ElemType >::serialize(data, (const ElemType&)std::make_pair(node->mKey, node->mValue), name, mask);
            return len;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;
            typedef typename ObjType::iter_type IterType;

            if (data.length() < sizeof(UInt32))
                return false;
            UInt32 size;
            data >> size;
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            size = EndianReverse<UInt32>::convert(size);
#endif
            if (mask != 0)
            {
                size ^= (UInt32)(mask + 0);
            }

            obj.clear();
            for (; size; --size)
            {
                ElemType elem;
                if (!S11NTraits<ElemType >::unserialize(data, elem, name, mask))
                    return false;
                obj.insert(elem.first, elem.second);
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NLynxStringMapImpls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<String, MappedType > ElemType;
            typedef typename ObjType::iter_type IterType;

            UInt32 size = sizeof(UInt32);
            for (IterType* node = obj.begin(); node != obj.end(); node = obj.next(node))
                size += S11NTraits<ElemType >::size(data, std::make_pair(node->mKey, node->mValue));
            return size;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<String, MappedType > ElemType;
            typedef typename ObjType::iter_type IterType;

            UInt32 size = obj.size();
            if (mask != 0)
            {
                size ^= (UInt32)(mask + 0);
            }
#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            data << size;
#else
            data << EndianReverse<UInt32>::convert(size);
#endif

            UInt32 len = 0;
            len += sizeof(UInt32);

            for (IterType* node = obj.begin(); node != obj.end(); node = obj.next(node))
                len += S11NTraits<ElemType >::serialize(data, (const ElemType&)std::make_pair(node->mKey, node->mValue), name, mask);
            return len;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<String, MappedType > ElemType;
            typedef typename ObjType::iter_type IterType;

            if (data.length() < sizeof(UInt32))
                return false;
            UInt32 size;
            data >> size;
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            size = EndianReverse<UInt32>::convert(size);
#endif
            if (mask != 0)
            {
                size ^= (UInt32)(mask + 0);
            }

            obj.clear();
            for (; size; --size)
            {
                ElemType elem;
                if (!S11NTraits<ElemType >::unserialize(data, elem, name, mask))
                    return false;
                obj.insert(elem.first, elem.second);
            }
            return true;
        }
    };


    template <class DataType >
    struct S11NLynxSetImpls<DataType, typename EnableIf<IsLynxStreamBufferData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::iter_type IterType;

            UInt32 size = sizeof(UInt32);
            for (IterType* node = obj.begin(); node != obj.end(); node = obj.next(node))
                size += S11NTraits<ElemType >::size(data, node->mValue);
            return size;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::iter_type IterType;

            UInt32 size = obj.size();
            if (mask != 0)
            {
                size ^= (UInt32)(mask + 0);
            }
#if defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            data << size;
#else
            data << EndianReverse<UInt32>::convert(size);
#endif

            UInt32 len = 0;
            len += sizeof(UInt32);

            for (IterType* node = obj.begin(); node != obj.end(); node = obj.next(node))
                len += S11NTraits<ElemType >::serialize(data, node->mValue, name, mask);
            return len;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::iter_type IterType;

            if (data.length() < sizeof(UInt32))
                return false;
            UInt32 size;
            data >> size;
#if !defined (_LYNX_BYTE_ORDER_NOT_CONVERT)
            size = EndianReverse<UInt32>::convert(size);
#endif
            if (mask != 0)
            {
                size ^= (UInt32)(mask + 0);
            }

            obj.clear();
            for (; size; --size)
            {
                ElemType elem;
                if (!S11NTraits<ElemType >::unserialize(data, elem, name, mask))
                    return false;
                obj.insert(elem);
            }
            return true;
        }
    };
} // namespace Lynx

#endif // __LYNX_PLATFORM_LIB_STREAMBUFFER_SERIALIZE_IMPLS_H__

