#ifndef __LYNX_PLATFORM_LIB_TINYXML_SERIALIZE_IMPLS_H__
#define __LYNX_PLATFORM_LIB_TINYXML_SERIALIZE_IMPLS_H__

#include <string>
#include <sstream>

#include "MathUtil.h"
#include "TinyXml.h"
#include "SerializeImpls.h"

#define DUMP_XML(objPointer)                                                     \
do {                                                                             \
    TinyXmlElement xmlObj(#objPointer);                                          \
    TinyXmlPrinter print;                                                        \
    Lynx::serialize(*objPointer, xmlObj);                                        \
    xmlObj.accept(&print);                                                       \
    LOG_PRINT("\n\n%s", print.CStr());                                           \
} while (0)  

namespace Lynx
{
    template <class Type> struct IsLynxTinyXmlData 
    { 
        static const bool s_Value = false; 
    };

    template <> struct IsLynxTinyXmlData<TinyXmlElement> 
    { 
        static const bool s_Value = true; 
    };

    static const char* TinyXmlDataTypeBool      = "Bool";
	static const char* TinyXmlDataTypeInt8      = "Int8";
	static const char* TinyXmlDataTypeUInt8     = "UInt8";
	static const char* TinyXmlDataTypeInt16     = "Int16";
	static const char* TinyXmlDataTypeUInt16    = "UInt16";
	static const char* TinyXmlDataTypeInt32     = "Int32";
	static const char* TinyXmlDataTypeUInt32    = "UInt32";
	static const char* TinyXmlDataTypeInt64     = "Int64";
	static const char* TinyXmlDataTypeUInt64    = "UInt64";
	static const char* TinyXmlDataTypeFloat     = "Float";
	static const char* TinyXmlDataTypeDouble    = "Double";
    static const char* TinyXmlDataTypeString    = "String";
    static const char* TinyXmlDataTypePair      = "Pair";
    static const char* TinyXmlDataTypeMap       = "Map";
	static const char* TinyXmlDataTypeSet       = "Set";
    static const char* TinyXmlDataTypeList      = "List";
	static const char* TinyXmlDataTypeDeque     = "Deque";
	static const char* TinyXmlDataTypeVector    = "Vector";
    static const char* TinyXmlDataTypeVector2   = "Vector2";
	static const char* TinyXmlDataTypeVector3   = "Vector3";
	static const char* TinyXmlDataTypeVector4   = "Vector4";

    static TinyXmlElement* allocTinyXmlElement(const char* value)
    {
        return XNEW(TinyXmlElement)(value);
    }

    static TinyXmlElement* allocTinyXmlElement(TinyXmlElement& parent, const char* value)
    {
        TinyXmlElement* elem = XNEW(TinyXmlElement)(value);
        parent.linkEndChild(elem);
        return elem;
    }

    template <class DataType>
    struct Object_Imples<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type >
    {
		template <class ObjType >
        static DataType& init(DataType& data, ObjType& obj, const char* type, const char* name)
        {
            if (!strcmp(name, "Anonymity"))
            {
                return data;
            }

            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", type);
            return *element;
        }

        template <class ObjType >
        static DataType* uninit(DataType& data, ObjType& obj, const char* type, const char* name)
        {
            if (!strcmp(name, "Anonymity"))
            {
                return &data;
            }
            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return NULL;
            TinyXmlElement* element = node->toElement();
            return element;
        }
    };

    template <class DataType >
    struct S11NBooleanImpls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeBool);
            element->setAttribute("value", obj ? "true" : "false");
			return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = (strncasecmp(p, "true", 4)) ? false : true;
            return true;
        }
    };

    template <class DataType >
    struct S11NInt8Impls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeInt8);
            char buf[128];
            lynxItoa<ObjType >(obj, buf, 128, 10);
            element->setAttribute("value", buf);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = lynxAtoi<ObjType >(p);
            return true;
        }
    };

	template <class DataType >
    struct S11NUInt8Impls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeUInt8);
            char buf[128];
            lynxItoa<ObjType >(obj, buf, 128, 10);
            element->setAttribute("value", buf);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = lynxAtoi<ObjType >(p);
            return true;
        }
    };

	template <class DataType >
    struct S11NInt16Impls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeInt16);
            char buf[128];
            lynxItoa<ObjType >(obj, buf, 128, 10);
            element->setAttribute("value", buf);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = lynxAtoi<ObjType >(p);
            return true;
        }
    };

	template <class DataType >
    struct S11NUInt16Impls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeUInt16);
            char buf[128];
            lynxItoa<ObjType >(obj, buf, 128, 10);
            element->setAttribute("value", buf);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = lynxAtoi<ObjType >(p);
            return true;
        }
    };

	template <class DataType >
    struct S11NInt32Impls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeInt32);
            char buf[128];
            lynxItoa<ObjType >(obj, buf, 128, 10);
            element->setAttribute("value", buf);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = lynxAtoi<ObjType >(p);
            return true;
        }
    };

	template <class DataType >
    struct S11NUInt32Impls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeUInt32);
            char buf[128];
            lynxItoa<ObjType >(obj, buf, 128, 10);
            element->setAttribute("value", buf);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = lynxAtoi<ObjType >(p);
            return true;
        }
    };

	template <class DataType >
    struct S11NInt64Impls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeInt64);
            char buf[128];
            lynxItoa<ObjType >(obj, buf, 128, 10);
            element->setAttribute("value", buf);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = lynxAtoi<ObjType >(p);
            return true;
        }
    };

	template <class DataType >
    struct S11NUInt64Impls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeUInt64);
            char buf[128];
            lynxItoa<ObjType >(obj, buf, 128, 10);
            element->setAttribute("value", buf);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = lynxAtoi<ObjType >(p);
            return true;
        }
    };

    template <class DataType >
    struct S11NFloatImpls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeFloat);
            std::stringstream tmpStr;  // TODO: performance optimized.
            tmpStr << obj;
            element->setAttribute("value", tmpStr.str().c_str());
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            std::stringstream tmpStr; // TODO: performance optimized.
            tmpStr << p;
            tmpStr >> obj;
            return true;
        }
    };

    //template <class DataType >
    //struct S11NDoubleImpls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    //{
    //    template <class ObjType >
    //    static UInt32 size(const ObjType& obj)
    //    {
    //        return 0;
    //    }

    //    template <class ObjType >
    //    static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
    //    {
    //        TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
    //        element->setAttribute("type", TinyXmlDataTypeDouble);
    //        std::stringstream tmpStr;
    //        tmpStr << obj;
    //        element->setAttribute("value", tmpStr.str().c_str());
    //        return 0;
    //    }

    //    template <class ObjType >
    //    static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
    //    {
    //        TinyXmlNode* node = data.getFirstChild(name);
    //        if (node == NULL) return false;
    //        TinyXmlElement* element = node->toElement();
    //        const char* p = element->getAttribute("value");
    //        if (p == "") return false;
    //        std::stringstream tmpStr; // TODO: performance optimized.
    //        tmpStr << p;
    //        tmpStr >> obj;
    //        return true;
    //    }
    //};

    template <class DataType >
    struct S11NStdBasicStringImpls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeString);
            element->setAttribute("value", obj.c_str()); // Don't support wstring.
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = p;
            return true;
        }
    };

    template <class DataType >
    struct S11NStdPairImpls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::first_type FirstType;
            typedef typename ObjType::second_type SecondType;

            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypePair);

            return S11NTraits<FirstType >::serialize(*element, (FirstType&)obj.first, "first", mask) 
                + S11NTraits<SecondType >::serialize(*element, (SecondType&)obj.second, "second", mask);
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::first_type FirstType;
            typedef typename ObjType::second_type SecondType;

            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();

            return S11NTraits<FirstType >::unserialize(*element, obj.first, "first", mask) &&
                S11NTraits<SecondType >::unserialize(*element, obj.second, "second", mask);
        }
    };

    template <class DataType >
    struct S11NStdMapImpls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeMap);
            element->setAttribute("size", obj.size());

            UInt32 i = 0;
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
            {
                char buf[128];
                snprintf(buf, sizeof(buf), "elem%d", i);
                S11NTraits<ElemType >::serialize(*element, (ElemType&)*it, buf, mask);
                i++;
            }
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            UInt32 size = lynxAtoi<UInt32>(element->getAttribute("size"));
            UInt32 i = 0;

            obj.clear();
            for (; size; --size)
            {
                ElemType elem;
                char buf[128];
                snprintf(buf, sizeof(buf), "elem%d", i);

                if (!S11NTraits<ElemType >::unserialize(*element, elem,  buf, mask))
                    return false;
                obj.insert(obj.end(), elem);
                i++;
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NStdVectorImpls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeVector);
            element->setAttribute("size", obj.size());

            UInt32 i = 0;
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
            {
                char buf[128];
                snprintf(buf, sizeof(buf), "elem%d", i);
                S11NTraits<ElemType >::serialize(*element, (ElemType&)*it, buf, mask);
                i++;
            }
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            UInt32 size = lynxAtoi<UInt32>(element->getAttribute("size"));
            UInt32 i = 0;

            obj.clear();
            for (; size; --size)
            {
                ElemType elem;
                char buf[128];
                snprintf(buf, sizeof(buf), "elem%d", i);

                if (!S11NTraits<ElemType >::unserialize(*element, elem,  buf, mask))
                    return false;
                obj.insert(obj.end(), elem);
                i++;
            }
            return true;
        }
    };

	template <class DataType >
    struct S11NStdListImpls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeList);
            element->setAttribute("size", obj.size());

            UInt32 i = 0;
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
            {
                char buf[128];
                snprintf(buf, sizeof(buf), "elem%d", i);
                S11NTraits<ElemType >::serialize(*element, (ElemType&)*it, buf, mask);
                i++;
            }
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            UInt32 size = lynxAtoi<UInt32>(element->getAttribute("size"));
            UInt32 i = 0;

            obj.clear();
            for (; size; --size)
            {
                ElemType elem;
                char buf[128];
                snprintf(buf, sizeof(buf), "elem%d", i);

                if (!S11NTraits<ElemType >::unserialize(*element, elem,  buf, mask))
                    return false;
                obj.insert(obj.end(), elem);
                i++;
            }
            return true;
        }
    };

	template <class DataType >
    struct S11NStdDequeImpls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeDeque);
            element->setAttribute("size", obj.size());

            UInt32 i = 0;
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
            {
                char buf[128];
                snprintf(buf, sizeof(buf), "elem%d", i);
                S11NTraits<ElemType >::serialize(*element, (ElemType&)*it, buf, mask);
                i++;
            }
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            UInt32 size = lynxAtoi<UInt32>(element->getAttribute("size"));
            UInt32 i = 0;

            obj.clear();
            for (; size; --size)
            {
                ElemType elem;
                char buf[128];
                snprintf(buf, sizeof(buf), "elem%d", i);

                if (!S11NTraits<ElemType >::unserialize(*element, elem,  buf, mask))
                    return false;
                obj.insert(obj.end(), elem);
                i++;
            }
            return true;
        }
    };

	template <class DataType >
    struct S11NStdSetImpls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeSet);
            element->setAttribute("size", obj.size());

            UInt32 i = 0;
            for (IteratorType it = obj.begin(); it != obj.end(); ++it)
            {
                char buf[128];
                snprintf(buf, sizeof(buf), "elem%d", i);
                S11NTraits<ElemType >::serialize(*element, (ElemType&)*it, buf, mask);
                i++;
            }
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::const_iterator IteratorType;

            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            UInt32 size = lynxAtoi<UInt32>(element->getAttribute("size"));
            UInt32 i = 0;

            obj.clear();
            for (; size; --size)
            {
                ElemType elem;
                char buf[128];
                snprintf(buf, sizeof(buf), "elem%d", i);

                if (!S11NTraits<ElemType >::unserialize(*element, elem,  buf, mask))
                    return false;
                obj.insert(obj.end(), elem);
                i++;
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NLynxBufferImpls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            return true;
        }
    };

    /*template <class DataType >
    struct S11NLynxBlobImpls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            return true;
        }
    };*/

    template <class DataType >
    struct S11NLynxFVector2Impls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeVector2);
            S11NTraits<float >::serialize(*element, obj.mX, "mX", mask);
            S11NTraits<float >::serialize(*element, obj.mY, "mY", mask);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            if (!S11NTraits<float>::unserialize(*element, obj.mX, "mX", mask))
                return false;
            if (!S11NTraits<float>::unserialize(*element, obj.mY, "mY", mask))
                return false;
            return true;
        }
    };

    template <class DataType >
    struct S11NLynxCharArrayImpls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeString);
            element->setAttribute("value", obj.c_str()); // Don't support wstring.
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = p;
            return true;
        }
    };

    template <class DataType >
    struct S11NLynxFVector3Impls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeVector3);
            S11NTraits<float >::serialize(*element, obj.mX, "mX", mask);
            S11NTraits<float >::serialize(*element, obj.mY, "mY", mask);
            S11NTraits<float >::serialize(*element, obj.mZ, "mZ", mask);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            if (!S11NTraits<float>::unserialize(*element, obj.mX, "mX", mask))
                return false;
            if (!S11NTraits<float>::unserialize(*element, obj.mY, "mY", mask))
                return false;
            if (!S11NTraits<float>::unserialize(*element, obj.mZ, "mZ", mask))
                return false;
            return true;
        }
    };

	template <class DataType >
    struct S11NLynxFVector4Impls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeVector4);
            S11NTraits<float >::serialize(*element, obj.mX, "mX", mask);
            S11NTraits<float >::serialize(*element, obj.mY, "mY", mask);
            S11NTraits<float >::serialize(*element, obj.mZ, "mZ", mask);
            S11NTraits<float >::serialize(*element, obj.mW, "mW", mask);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            if (!S11NTraits<float>::unserialize(*element, obj.mX, "mX", mask))
                return false;
            if (!S11NTraits<float>::unserialize(*element, obj.mY, "mY", mask))
                return false;
            if (!S11NTraits<float>::unserialize(*element, obj.mZ, "mZ", mask))
                return false;
			if (!S11NTraits<float>::unserialize(*element, obj.mW, "mW", mask))
				return false;
            return true;
        }
    };

    //template <class DataType >
    //struct S11NLynxColorImpls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    //{
    //    template <class ObjType >
    //    static UInt32 size(const ObjType& obj)
    //    {
    //        return 0;
    //    }

    //    template <class ObjType >
    //    static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
    //    {
    //        TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
    //        element->setAttribute("type", TinyXmlDataTypeColor);
    //        S11NTraits<float >::serialize(*element, obj.mR, "mR", mask);
    //        S11NTraits<float >::serialize(*element, obj.mG, "mG", mask);
    //        S11NTraits<float >::serialize(*element, obj.mB, "mB", mask);
    //        S11NTraits<float >::serialize(*element, obj.mA, "mA", mask);
    //        return 0;
    //    }

    //    template <class ObjType >
    //    static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
    //    {
    //        TinyXmlNode* node = data.getFirstChild(name);
    //        if (node == NULL) return false;
    //        TinyXmlElement* element = node->toElement();
    //        if (!S11NTraits<float>::unserialize(*element, obj.mR, "mR", mask))
    //            return false;
    //        if (!S11NTraits<float>::unserialize(*element, obj.mG, "mG", mask))
    //            return false;
    //        if (!S11NTraits<float>::unserialize(*element, obj.mB, "mB", mask))
    //            return false;
    //        if (!S11NTraits<float>::unserialize(*element, obj.mA, "mA", mask))
    //            return false;
    //        return true;
    //    }
    //};


    template <class DataType >
    struct S11NLynxListImpls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::iter_type IterType;

            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeList);
            element->setAttribute("size", obj.size());

            UInt32 i = 0;
            for (IterType* node = obj.begin(); node != obj.end(); node = obj.next(node))
            {
                char buf[128];
                snprintf(buf, sizeof(buf), "elem%d", i);
                S11NTraits<ElemType >::serialize(*element, (ElemType&)node->mValue, buf, mask);
                i++;
            }
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::iter_type IterType;

            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            UInt32 size = lynxAtoi<UInt32>(element->getAttribute("size"));
            UInt32 i = 0;

            obj.Clear();
            for (; size; --size)
            {
                ElemType elem;
                char buf[128];
#if defined(__APPLE__)
                snprintf(buf, 128, "elem%d", i);
#else
                snprintf(buf, "elem%d", i);
#endif

                if (!S11NTraits<ElemType >::unserialize(*element, elem,  buf, mask))
                    return false;
                obj.InsertTail(elem);
                i++;
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NLynxMapImpls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;
            typedef typename ObjType::iter_type IterType;

            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeMap);
            element->setAttribute("size", obj.size());

            UInt32 i = 0;
            for (IterType* node = obj.begin(); node != obj.end(); node = obj.next(node))
            {
                char buf[128];
                snprintf(buf, sizeof(buf), "elem%d", i);
                S11NTraits<ElemType >::serialize(*element, (const ElemType&)std::make_pair(node->mKey, node->mValue), buf, mask);
                i++;
            }
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::key_type KeyType;
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<KeyType, MappedType > ElemType;

            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            UInt32 size = lynxAtoi<UInt32>(element->getAttribute("size"));
            UInt32 i = 0;

            obj.Clear();
            for (; size; --size)
            {
                ElemType elem;
                char buf[128];
                snprintf(buf, sizeof(buf), "elem%d", i);

                if (!S11NTraits<ElemType >::unserialize(*element, elem,  buf, mask))
                    return false;
                obj.Insert(elem.first, elem.second);
                i++;
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NLynxStringMapImpls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<String, MappedType > ElemType;
            typedef typename ObjType::iter_type IterType;

            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeMap);
            element->setAttribute("size", obj.size());

            UInt32 i = 0;
            for (IterType* node = obj.begin(); node != obj.end(); node = obj.next(node))
            {
                char buf[128];
                snprintf(buf, sizeof(buf), "elem%d", i);
                S11NTraits<ElemType >::serialize(*element, (const ElemType&)std::make_pair(node->mKey, node->mValue), buf, mask);
                i++;
            }
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::mapped_type MappedType;
            typedef typename std::pair<String, MappedType > ElemType;

            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            UInt32 size = lynxAtoi<UInt32>(element->getAttribute("size"));
            UInt32 i = 0;

            obj.Clear();
            for (; size; --size)
            {
                ElemType elem;
                char buf[128];
                snprintf(buf, sizeof(buf), "elem%d", i);

                if (!S11NTraits<ElemType >::unserialize(*element, elem,  buf, mask))
                    return false;
                obj.Insert(elem.first, elem.second);
                i++;
            }
            return true;
        }
    };

    template <class DataType >
    struct S11NLynxSetImpls<DataType, typename EnableIf<IsLynxTinyXmlData<DataType >::s_Value>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::iter_type IterType;

            TinyXmlElement* element = Lynx::allocTinyXmlElement(data, name);
            element->setAttribute("type", TinyXmlDataTypeSet);
            element->setAttribute("size", obj.size());

            UInt32 i = 0;
            for (IterType* node = obj.begin(); node != obj.end(); node = obj.next(node))
            {
                char buf[128];
                snprintf(buf, sizeof(buf), "elem%d", i);
                S11NTraits<ElemType >::serialize(*element, (ElemType&)node->mValue, buf, mask);
                i++;
            }
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::value_type ElemType;
            typedef typename ObjType::iter_type IterType;

            TinyXmlNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            TinyXmlElement* element = node->toElement();
            UInt32 size = lynxAtoi<UInt32>(element->getAttribute("size"));
            UInt32 i = 0;

            obj.Clear();
            for (; size; --size)
            {
                ElemType elem;
                char buf[128];
                snprintf(buf, sizeof(buf), "elem%d", i);

                if (!S11NTraits<ElemType >::unserialize(*element, elem,  buf, mask))
                    return false;
                obj.Insert(elem);
                i++;
            }
            return true;
        }
    };

} // namespace Lynx      

#endif // __LYNX_PLATFORM_LIB_TINYXML_SERIALIZE_IMPLS_H__


