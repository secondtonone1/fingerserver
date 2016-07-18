#ifndef __LYNX_UTIL_LIB_CONFIG_MANAGER_H__
#define __LYNX_UTIL_LIB_CONFIG_MANAGER_H__

#include "PlatformLib.h"

//<?xml version="1.0" encoding="utf-8"?>
//<config>
//    <property name="Assets" value="ResourceConfig.xml"/>
//    <property name="version" value="0.1"/> 
//    <property name="LoginIp" value="10.4.5.14"/>
//    <property name="LoginPort" value="9101"/>
//    <property name="Position" value="123.04,1122.02,234"/>
//    <property name="Color" value="200,129,30,0"/>
//</config>

namespace Lynx
{
    class _PlatformExport ConfigManager
    {
    public:
        ConfigManager();
        virtual ~ConfigManager();

        bool initial(const String& filePath, bool fromXml = true);
        void release();

        bool loadFromFile(const String& filePath, bool fromXml = true);
        void saveToFile(bool toXml = true);
        void saveBinToFile(const String& filePath);

        void setProperty(const String& name, const String& value);
        bool getProperty(const String& name, String& value);

        void setAsString(const String& name, const String& value);
        String getAsString(const String& name);
        bool getAsString(const String& name, String& str);

        void setAsInt32(const String& name, Int32 value);
        Int32 getAsInt32(const String& name);
        bool getAsInt32(const String& name, Int32& i);

        void setAsUInt32(const String& name, UInt32 value);
        UInt32 getAsUInt32(const String& name);
        bool getAsUInt32(const String& name, UInt32& i);

        void setAsInt16(const String& name, Int16 value);
        Int16 getAsInt16(const String& name);

        void setAsUInt16(const String& name, UInt16 value);
        UInt16 getAsUInt16(const String& name);

        void setAsInt8(const String& name, Int8 value);
        Int8 getAsInt8(const String& name);

        void setAsUInt8(const String& name, UInt8 value);
        UInt8 getAsUInt8(const String& name);

        void setAsFloat(const String& name, float value);
        float getAsFloat(const String& name);
        bool getAsFloat(const String& name, float& f);

        void setAsVector2(const String& name, const Vector2& value);
        Vector2 getAsVector2(const String& name);
        bool getAsVector2(const String& name, Vector2& value);

        void setAsVector3(const String& name, const Vector3& value);
        Vector3 getAsVector3(const String& name);
        bool getAsVector3(const String& name, Vector3& value);

		void setAsVector4(const String& name, const Vector4& value);
		Vector4 getAsVector4(const String& name);
		bool getAsVector4(const String& name, Vector4& value);

    private:
        String mFilePath;
        StringMap<String> mPropertyMap;
        bool mIsDirty;
        bool mFromXmlFlag;
    };
} // namespace

#endif // __LYNX_UTIL_LIB_CONFIG_MANAGER_H__

