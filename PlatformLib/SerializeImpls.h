#ifndef __LYNX_PLATFORM_LIB_SERIALIZE_IMPLS_H__
#define __LYNX_PLATFORM_LIB_SERIALIZE_IMPLS_H__

#include "SerializeType.h"

namespace Lynx
{
    template <class DataType, class Enable = void>
    struct Object_Imples
    {
        template <class ObjType >
        static DataType& init(DataType& data, ObjType& obj, const char* type, const char* name)
        {
            return data;
        }

        template <class ObjType >
        static DataType* uninit(DataType& data, ObjType& obj, const char* type, const char* name)
        {
            return &data;
        }
    };

	template <class DataType, class Enable = void>
    struct S11NPointerImpls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NBooleanImpls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NInt8Impls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

	template <class DataType, class Enable = void>
    struct S11NUInt8Impls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

	template <class DataType, class Enable = void>
    struct S11NInt16Impls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

	template <class DataType, class Enable = void>
    struct S11NUInt16Impls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

	template <class DataType, class Enable = void>
    struct S11NInt32Impls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

	template <class DataType, class Enable = void>
    struct S11NUInt32Impls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

	template <class DataType, class Enable = void>
    struct S11NInt64Impls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

	template <class DataType, class Enable = void>
    struct S11NUInt64Impls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NFloatImpls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NDoubleImpls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NStdBasicStringImpls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NStdPairImpls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NStdMapImpls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

	template <class DataType, class Enable = void>
    struct S11NStdVectorImpls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

	template <class DataType, class Enable = void>
    struct S11NStdListImpls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

	template <class DataType, class Enable = void>
    struct S11NStdDequeImpls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

	template <class DataType, class Enable = void>
    struct S11NStdSetImpls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };


    template <class DataType, class Enable = void>
    struct S11NLynxBufferImpls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NLynxBlobImpls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

	template <class DataType, class Enable = void>
    struct S11NLynxCharArrayImpls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NLynxFVector2Impls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NLynxFVector3Impls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

	template <class DataType, class Enable = void>
    struct S11NLynxFVector4Impls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

    /*
	template <class DataType, class Enable = void>
    struct S11NLynxColorImpls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };
	*/


    template <class DataType, class Enable = void>
    struct S11NLynxListImpls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NLynxMapImpls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NLynxStringMapImpls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };

    template <class DataType, class Enable = void>
    struct S11NLynxSetImpls
    {
        template <class ObjType >
        static UInt32 size(const ObjType& obj, DataType& data)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static UInt32 serialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return 0;
        }

        template <class ObjType >
        static bool unserialize(ObjType& obj, DataType& data, const char* name, UInt32 mask)
        {
            ASSERT(0);
            return true;
        }
    };
} // namespace Lynx

#include "SerializeTraits.h"

#endif // __LYNX_PLATFORM_LIB_SERIALIZE_IMPLS_H__


