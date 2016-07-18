#ifndef __LYNX_PLATFORM_LIB_CSHARP_SERIALIZE_IMPLS_H__
#define __LYNX_PLATFORM_LIB_CSHARP_SERIALIZE_IMPLS_H__

#include <string>
#include <sstream>

#include "MathUtil.h"
#include "TinyXml.h"
#include "SerializeImpls.h"

namespace Lynx
{
	extern UInt32 gParentClassObjectSize;
	extern char* gS11NObjectAddress;

	class CSharpElement : public TinyXmlElement
	{
	public:
		CSharpElement(const char* value) : TinyXmlElement(value) {}
	};

	class CSharpNode : public TinyXmlNode
	{

	};

	typedef TinyXmlPrinter CSharpPrinter;
	//typedef TinyXmlElement CSharpElement;
	//typedef TinyXmlNode CSharpNode;

    template <class Type> struct IsLynxCSharpData 
    { 
        static const bool sValue = false; 
    };

    template <> struct IsLynxCSharpData<CSharpElement> 
    { 
        static const bool sValue = true; 
    };

    static const char* CSharpDataTypeBool      = "bool";
	static const char* CSharpDataTypeInt8      = "SByte";
	static const char* CSharpDataTypeUInt8     = "Byte";
	static const char* CSharpDataTypeInt16     = "Int16";
	static const char* CSharpDataTypeUInt16    = "UInt16";
	static const char* CSharpDataTypeInt32     = "Int32";
	static const char* CSharpDataTypeUInt32    = "UInt32";
	static const char* CSharpDataTypeInt64     = "Int64";
	static const char* CSharpDataTypeUInt64    = "UInt64";
	static const char* CSharpDataTypeFloat     = "Single";
	static const char* CSharpDataTypeDouble    = "Double";
    static const char* CSharpDataTypeString    = "String";
    static const char* CSharpDataTypePair      = "Pair";
    static const char* CSharpDataTypeMap       = "Dictionary";
	static const char* CSharpDataTypeSet       = "List";
    static const char* CSharpDataTypeList      = "List";
	static const char* CSharpDataTypeDeque     = "List";
	static const char* CSharpDataTypeVector    = "List";
    static const char* CSharpDataTypeVector2   = "Vector2";
	static const char* CSharpDataTypeVector3   = "Vector3";
	static const char* CSharpDataTypeVector4   = "Vector4";

    static CSharpElement* allocCSharpElement(const char* value)
    {
        return XNEW(CSharpElement)(value);
    }

    static CSharpElement* allocCSharpElement(CSharpElement& parent, const char* value)
    {
        CSharpElement* elem = XNEW(CSharpElement)(value);
        parent.linkEndChild(elem);
        return elem;
    }

    template <class DataType >
    struct Object_Imples<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type >
    {
		template <class ObjType >
        static DataType& init(DataType& data, ObjType& obj, const char* type, const char* name)
        {
            if (!strcmp(name, "Anonymity"))
            {
                return data;
            }

            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", type);
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}
            return *element;
        }

        template <class ObjType >
        static DataType* uninit(DataType& data, ObjType& obj, const char* type, const char* name)
        {
            if (!strcmp(name, "Anonymity"))
            {
                return &data;
            }
            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return NULL;
            CSharpElement* element = node->toElement();
            return element;
        }
    };

    template <class DataType >
    struct S11NBooleanImpls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeBool);
            element->setAttribute("value", obj ? "true" : "false");
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}
			return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = (strncasecmp(p, "true", 4)) ? false : true;
            return true;
        }
    };

    template <class DataType >
    struct S11NInt8Impls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeInt8);
            char buf[128];
            lynxItoa<ObjType >(obj, buf, 128, 10);
            element->setAttribute("value", buf);
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = lynxAtoi<ObjType >(p);
            return true;
        }
    };

	template <class DataType >
    struct S11NUInt8Impls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeUInt8);
            char buf[128];
            lynxItoa<ObjType >(obj, buf, 128, 10);
            element->setAttribute("value", buf);
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = lynxAtoi<ObjType >(p);
            return true;
        }
    };

	template <class DataType >
    struct S11NInt16Impls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeInt16);
            char buf[128];
            lynxItoa<ObjType >(obj, buf, 128, 10);
            element->setAttribute("value", buf);
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = lynxAtoi<ObjType >(p);
            return true;
        }
    };

	template <class DataType >
    struct S11NUInt16Impls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeUInt16);
            char buf[128];
            lynxItoa<ObjType >(obj, buf, 128, 10);
            element->setAttribute("value", buf);
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = lynxAtoi<ObjType >(p);
            return true;
        }
    };

	template <class DataType >
    struct S11NInt32Impls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeInt32);
            char buf[128];
            lynxItoa<ObjType >(obj, buf, 128, 10);
            element->setAttribute("value", buf);
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = lynxAtoi<ObjType >(p);
            return true;
        }
    };

	template <class DataType >
    struct S11NUInt32Impls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeUInt32);
            char buf[128];
            lynxItoa<ObjType >(obj, buf, 128, 10);
            element->setAttribute("value", buf);
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = lynxAtoi<ObjType >(p);
            return true;
        }
    };

	template <class DataType >
    struct S11NInt64Impls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeInt64);
            char buf[128];
            lynxItoa<ObjType >(obj, buf, 128, 10);
            element->setAttribute("value", buf);
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = lynxAtoi<ObjType >(p);
            return true;
        }
    };

	template <class DataType >
    struct S11NUInt64Impls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeUInt64);
            char buf[128];
            lynxItoa<ObjType >(obj, buf, 128, 10);
            element->setAttribute("value", buf);
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = lynxAtoi<ObjType >(p);
            return true;
        }
    };

    template <class DataType >
    struct S11NFloatImpls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeFloat);
            std::stringstream tmpStr;  // TODO: performance optimized.
            tmpStr << obj;
            element->setAttribute("value", tmpStr.str().c_str());
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            std::stringstream tmpStr; // TODO: performance optimized.
            tmpStr << p;
            tmpStr >> obj;
            return true;
        }
    };

    //template <class DataType >
    //struct S11NDoubleImpls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
    //{
    //    template <class ObjType >
    //    static UInt32 size(const ObjType& obj)
    //    {
    //        return 0;
    //    }

    //    template <class ObjType >
    //    static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
    //    {
    //        CSharpElement* element = Lynx::allocCSharpElement(data, name);
    //        element->setAttribute("type", CSharpDataTypeDouble);
    //        std::stringstream tmpStr;
    //        tmpStr << obj;
    //        element->setAttribute("value", tmpStr.str().c_str());
    //        return 0;
    //    }

    //    template <class ObjType >
    //    static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
    //    {
    //        CSharpNode* node = data.getFirstChild(name);
    //        if (node == NULL) return false;
    //        CSharpElement* element = node->toElement();
    //        const char* p = element->getAttribute("value");
    //        if (p == "") return false;
    //        std::stringstream tmpStr; // TODO: performance optimized.
    //        tmpStr << p;
    //        tmpStr >> obj;
    //        return true;
    //    }
    //};

    template <class DataType >
    struct S11NStdBasicStringImpls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeString);
            element->setAttribute("value", obj.c_str()); // Don't support wstring.
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = p;
            return true;
        }
    };

    template <class DataType >
    struct S11NStdPairImpls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
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

            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypePair);


            return S11NTraits<FirstType >::serialize(*element, (FirstType&)obj.first, "first", mask) 
                + S11NTraits<SecondType >::serialize(*element, (SecondType&)obj.second, "second", mask);
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            typedef typename ObjType::first_type FirstType;
            typedef typename ObjType::second_type SecondType;

            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();

            return S11NTraits<FirstType >::unserialize(*element, obj.first, "first", mask) &&
                S11NTraits<SecondType >::unserialize(*element, obj.second, "second", mask);
        }
    };

    template <class DataType >
    struct S11NStdMapImpls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
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

            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeMap);
            element->setAttribute("size", obj.size());
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}

			////////////////
			KeyType key;
			MappedType value;
			obj[key] = value;
			//obj.insert(std::make_pair(key, value));
			//////////////////

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

            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
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
    struct S11NStdVectorImpls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
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

            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeVector);
            element->setAttribute("size", obj.size());
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}

			///////////////////
			ElemType e;
			obj.push_back(e);
			//////////////////

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

            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
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
    struct S11NStdListImpls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
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

            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeList);
            element->setAttribute("size", obj.size());
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}

			/////////////////
			ElemType e;
			obj.push_back(e);
			/////////////////

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

            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
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
    struct S11NStdDequeImpls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
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

            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeDeque);
            element->setAttribute("size", obj.size());
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}

			//////////////////////
			ElemType e;
			obj.push_back(e);
			//////////////////////

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

            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
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
    struct S11NStdSetImpls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
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

            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeSet);
            element->setAttribute("size", obj.size());
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}

			///////////////////
			ElemType e;
			obj.insert(e);
			///////////////////

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

            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
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
    struct S11NLynxBufferImpls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
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
    struct S11NLynxBlobImpls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
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
    struct S11NLynxFVector2Impls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeVector2);
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}
            S11NTraits<float >::serialize(*element, obj.mX, "mX", mask);
            S11NTraits<float >::serialize(*element, obj.mY, "mY", mask);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
            if (!S11NTraits<float>::unserialize(*element, obj.mX, "mX", mask))
                return false;
            if (!S11NTraits<float>::unserialize(*element, obj.mY, "mY", mask))
                return false;
            return true;
        }
    };

    template <class DataType >
    struct S11NLynxCharArrayImpls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeString);
            element->setAttribute("value", obj.c_str()); // Don't support wstring.

            if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
            const char* p = element->getAttribute("value");
            if (p == "") return false;
            obj = p;
            return true;
        }
    };

    template <class DataType >
    struct S11NLynxFVector3Impls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeVector3);
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}

            S11NTraits<float >::serialize(*element, obj.mX, "mX", mask);
            S11NTraits<float >::serialize(*element, obj.mY, "mY", mask);
            S11NTraits<float >::serialize(*element, obj.mZ, "mZ", mask);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
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
    struct S11NLynxFVector4Impls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj)
        {
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeVector4);
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}

            S11NTraits<float >::serialize(*element, obj.mX, "mX", mask);
            S11NTraits<float >::serialize(*element, obj.mY, "mY", mask);
            S11NTraits<float >::serialize(*element, obj.mZ, "mZ", mask);
			S11NTraits<float >::serialize(*element, obj.mW, "mW", mask);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
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
    //struct S11NLynxColorImpls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
    //{
    //    template <class ObjType >
    //    static UInt32 size(const ObjType& obj)
    //    {
    //        return 0;
    //    }

    //    template <class ObjType >
    //    static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
    //    {
    //        CSharpElement* element = Lynx::allocCSharpElement(data, name);
    //        element->setAttribute("type", CSharpDataTypeColor);
    //        S11NTraits<float >::serialize(*element, obj.mR, "mR", mask);
    //        S11NTraits<float >::serialize(*element, obj.mG, "mG", mask);
    //        S11NTraits<float >::serialize(*element, obj.mB, "mB", mask);
    //        S11NTraits<float >::serialize(*element, obj.mA, "mA", mask);
    //        return 0;
    //    }

    //    template <class ObjType >
    //    static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
    //    {
    //        CSharpNode* node = data.getFirstChild(name);
    //        if (node == NULL) return false;
    //        CSharpElement* element = node->toElement();
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
    struct S11NLynxListImpls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
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

            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeList);
            element->setAttribute("size", obj.size());
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}

			////////////////////////
			ElemType e;
			obj.insertHead(e);
			///////////////////////

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

            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
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
    struct S11NLynxMapImpls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
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

            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeMap);
            element->setAttribute("size", obj.size());
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}

			//////////////////////////
			KeyType key;
			MappedType value;
			obj.insert(key, value);
			//////////////////////////

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

            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
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
    struct S11NLynxStringMapImpls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
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

            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeMap);
            element->setAttribute("size", obj.size());
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}

			//////////////////////////////
			String key;
			MappedType value;
			obj.insert(key, value);
			//////////////////////////////

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

            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
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
    struct S11NLynxSetImpls<DataType, typename EnableIf<IsLynxCSharpData<DataType >::sValue>::type>
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

            CSharpElement* element = Lynx::allocCSharpElement(data, name);
            element->setAttribute("type", CSharpDataTypeSet);
            element->setAttribute("size", obj.size());
			if (gParentClassObjectSize == 0)
			{
				element->setAttribute("membership", "myself");
			}
			else
			{
				char* address = (char*)&obj;
				UInt32 addressOffset = address - gS11NObjectAddress;
				if (addressOffset >= gParentClassObjectSize)
				{
					element->setAttribute("membership", "myself");
				}
				else
				{
					element->setAttribute("membership", "parent");
				}
			}

			///////////////////////
			ElemType e;
			obj.insert(e);
			//////////////////////

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

            CSharpNode* node = data.getFirstChild(name);
            if (node == NULL) return false;
            CSharpElement* element = node->toElement();
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

#endif // __LYNX_PLATFORM_LIB_CSHARP_SERIALIZE_IMPLS_H__


