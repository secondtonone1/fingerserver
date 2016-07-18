#include "ConfigManager.h"

using namespace Lynx;

ConfigManager::ConfigManager() : mIsDirty(false), mFromXmlFlag(false)
{

}

ConfigManager::~ConfigManager()
{

}

bool
ConfigManager::initial(const String& filePath, bool fromXml)
{
    mFromXmlFlag = fromXml;
    if (!loadFromFile(filePath, fromXml))
    {
        LOG_WARN("Failed to load config file %s", filePath.c_str());
        return false;
    }
    return true;
}

void 
ConfigManager::release()
{
    saveToFile(mFromXmlFlag);
    mPropertyMap.clear();
}

bool 
ConfigManager::loadFromFile(const String& filePath, bool fromXml)
{
    mFilePath = filePath;
    mIsDirty = false;
    if (fromXml)
    {
        char* buffer;
        Int32 ret = FileUtil::loadFile(mFilePath.c_str(), &buffer);
        if (ret <= 0)
        {
            LOG_WARN("Failed to open config file %s", mFilePath.c_str());
            return false;
        }

        TinyXmlDocument doc;
        if (!doc.parse(buffer))
        {
            LOG_WARN("Failed to load xml file %s", mFilePath.c_str());
            return false;
        }

        FileUtil::freeBuffer(buffer);

        TinyXmlNode* node = doc.getFirstChild("config");
        if (node == 0)
        {
            LOG_WARN("Not found <config> element");
            return false;
        }

        TinyXmlElement* elem = node->toElement();
        TinyXmlNode* propertyNode = node->getFirstChild("property");
        if (propertyNode == 0)
        {
            LOG_WARN("Not found <property> element");
            return false;
        }
        elem = propertyNode->toElement();
        do
        {
            String name = elem->getAttribute("name");
            String value = elem->getAttribute("value");
            mPropertyMap.insert(name, value);
        } while ((elem = elem->nextSiblingElement("property")) != 0);
    }
    else
    {
        StreamBuffer streamBuffer;
        String packageFileName = lynxGetMainFileName(mFilePath);
        packageFileName += ".conf";
        if (!FileUtil::loadFile(packageFileName, streamBuffer))
        {
            LOG_WARN("Failed to load config file %s", mFilePath.c_str());
            return false;
        }

        if (!unserialize(mPropertyMap, streamBuffer, 8631))
        {
            LOG_WARN("Failed to unserialize file %s", packageFileName.c_str());
            return false;
        }
    }

    return true;
}

void 
ConfigManager::saveToFile(bool toXml)
{
    if (!mIsDirty || mPropertyMap.size() == 0)
    {
        return;
    }

    if (toXml)
    {
        TinyXmlElement root("config");
        for (StringMap<String>::Iter* iter = mPropertyMap.begin();
            iter != mPropertyMap.end(); iter = mPropertyMap.next(iter))
        {
            TinyXmlElement* elem = XNEW(TinyXmlElement)("property");
            root.linkEndChild(elem);
            elem->setAttribute("name", iter->mKey.c_str());
            elem->setAttribute("value", iter->mValue.c_str());
        }

        FILE* fp = fopen(mFilePath.c_str(), "wb");
        if (fp == NULL)
        {
            LOG_WARN("Failed to write client config %s", mFilePath.c_str());
            return;
        }

        StreamBuffer stream;
        TinyXmlPrinter printer;
        root.accept(&printer);
        stream.write("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n", 
            strlen("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"));
        stream.write(printer.c_str(), printer.size());

        rewind(fp);
        int nBytes = 0;

        while (1)
        {
            StreamBuffer::Node* node = stream.popNode();
            if (node == NULL) break;
            nBytes = fwrite((char*)node + sizeof(StreamBuffer::Node) + node->mOff,
                1, node->mLen, fp);
            if (nBytes != node->mLen)
            {
                LOG_WARN("Failed to fwrite file %s", mFilePath.c_str());
                return;
            }
            stream.deallocNode(node);
        }
        fclose(fp);    
    }
    else
    {
        StreamBuffer streamBuffer;
        String packageFileName = lynxGetMainFileName(mFilePath);
        packageFileName += ".conf";
        serialize(mPropertyMap, streamBuffer, 8631);
        if (!FileUtil::saveFile(packageFileName, streamBuffer))
        {
            LOG_WARN("Failed to save file %s", packageFileName.c_str());
        }
    }
}

void 
ConfigManager::saveBinToFile(const String& filePath)
{
    StreamBuffer streamBuffer;
    String packageFileName = lynxGetMainFileName(filePath);
    packageFileName += ".conf.bin";
    serialize(mPropertyMap, streamBuffer, 8631);
    if (!FileUtil::saveFile(packageFileName, streamBuffer))
    {
        LOG_WARN("Failed to save file %s", packageFileName.c_str());
    }
}

void 
ConfigManager::setProperty(const String& name, const String& value)
{
    StringMap<String>::Iter* iter = mPropertyMap.find(name);
    if (iter != mPropertyMap.end())
    {
        iter->mValue = value;
    }
    else
    {
        mPropertyMap.insert(name, value);
    }
    mIsDirty = true;

    saveToFile(mFromXmlFlag);
}

bool 
ConfigManager::getProperty(const String& name, String& value)
{
    StringMap<String>::Iter* iter = mPropertyMap.find(name);
    if (iter == mPropertyMap.end())
    {
        return false;
    }
    value = iter->mValue;
    return true;
}

void 
ConfigManager::setAsString(const String& name, const String& value)
{
    setProperty(name, value);
}

String 
ConfigManager::getAsString(const String& name)
{
    String value = "";
    getProperty(name, value);
    return value;
}
bool
ConfigManager::getAsString(const String& name, String& str)
{
    String value = "";
    bool rt = getProperty(name, value);
    if (rt)
    {
        str = value;
    }
    return rt;
}

void 
ConfigManager::setAsInt32(const String& name, Int32 value)
{
    char tmpBuf[128];
    snprintf(tmpBuf, sizeof(tmpBuf), "%d", value);
    setProperty(name, tmpBuf);
}

Int32 
ConfigManager::getAsInt32(const String& name)
{
    String value = "";
    getProperty(name, value);
    return lynxAtoi<Int32>(value.c_str());
}
bool
ConfigManager::getAsInt32(const String& name, Int32& i)
{
    String value = "";
    bool rt = getProperty(name, value);
    if (rt)
    {
        i = lynxAtoi<Int32>(value.c_str());
    }
    return rt;
}
void 
ConfigManager::setAsUInt32(const String& name, UInt32 value)
{
    char tmpBuf[128];
    snprintf(tmpBuf, sizeof(tmpBuf), "%d", value);
    setProperty(name, tmpBuf);
}

UInt32 
ConfigManager::getAsUInt32(const String& name)
{
    String value = "";
    getProperty(name, value);
    return lynxAtoi<UInt32>(value.c_str());
}
bool
ConfigManager::getAsUInt32(const String& name, UInt32& i)
{
    String value = "";
    bool rt = getProperty(name, value);
    if (rt)
    {
        i = lynxAtoi<UInt32>(value.c_str());
    }
    return rt;
}
void 
ConfigManager::setAsInt16(const String& name, Int16 value)
{
    char tmpBuf[128];
    snprintf(tmpBuf, sizeof(tmpBuf), "%d", value);
    setProperty(name, tmpBuf);
}

Int16 
ConfigManager::getAsInt16(const String& name)
{
    String value = "";
    getProperty(name, value);
    return lynxAtoi<Int16>(value.c_str());
}

void 
ConfigManager::setAsUInt16(const String& name, UInt16 value)
{
    char tmpBuf[128];
    snprintf(tmpBuf, sizeof(tmpBuf), "%d", value);
    setProperty(name, tmpBuf);
}

UInt16 
ConfigManager::getAsUInt16(const String& name)
{
    String value = "";
    getProperty(name, value);
    return lynxAtoi<UInt16>(value.c_str());
}

void 
ConfigManager::setAsInt8(const String& name, Int8 value)
{
    char tmpBuf[128];
    snprintf(tmpBuf, sizeof(tmpBuf), "%d", value);
    setProperty(name, tmpBuf);
}

Int8 
ConfigManager::getAsInt8(const String& name)
{
    String value = "";
    getProperty(name, value);
    return lynxAtoi<Int8>(value.c_str());
}

void 
ConfigManager::setAsUInt8(const String& name, UInt8 value)
{
    char tmpBuf[128];
    snprintf(tmpBuf, sizeof(tmpBuf), "%d", value);
    setProperty(name, tmpBuf);
}

UInt8 
ConfigManager::getAsUInt8(const String& name)
{
    String value = "";
    getProperty(name, value);
    return lynxAtoi<UInt8>(value.c_str());
}

void 
ConfigManager::setAsFloat(const String& name, float value)
{
    char tmpBuf[128];
    snprintf(tmpBuf, sizeof(tmpBuf), "%f", value);
    setProperty(name, tmpBuf);
}

float 
ConfigManager::getAsFloat(const String& name)
{
    String value = "";
    getProperty(name, value);
    return atof(value.c_str());
}

bool
ConfigManager::getAsFloat(const String& name, float& f)
{
    String value = "";
    bool rt = getProperty(name, value);
    if (rt)
    {
        f = atof(value.c_str());
    }
    return rt;
}

void 
ConfigManager::setAsVector2(const String& name, const Vector2& value)
{
    char tmpBuf[128];
    snprintf(tmpBuf, sizeof(tmpBuf), "%f,%f", value.mX, value.mY);
    setProperty(name, tmpBuf);
}

Vector2 
ConfigManager::getAsVector2(const String& name)
{
    String value = "";
    Vector2 vector2(0.0f, 0.0f);
    getProperty(name, value);
    if (!value.empty())
    {
        Vector<String> strVector;
        lynxStrSplit(value, ",", strVector, true);
        if (strVector.size() == 2)
        {
            vector2.mX = atof(strVector[0].c_str());
            vector2.mY = atof(strVector[1].c_str());
        }
    }
    return vector2;
}
bool
ConfigManager::getAsVector2(const String& name, Vector2& value)
{
    String str = "";
    Vector2 vector2(0.0f, 0.0f);
    getProperty(name, str);
    if (!str.empty())
    {
        Vector<String> strVector;
        lynxStrSplit(str, ",", strVector, true);
        if (strVector.size() == 2)
        {
            vector2.mX = atof(strVector[0].c_str());
            vector2.mY = atof(strVector[1].c_str());
        }
        value = vector2;
        return true;
    }
    return false;
}
void 
ConfigManager::setAsVector3(const String& name, const Vector3& value)
{
    char tmpBuf[128];
    snprintf(tmpBuf, sizeof(tmpBuf), "%f,%f,%f", value.mX, value.mY, value.mZ);
    setProperty(name, tmpBuf);
}

Vector3 
ConfigManager::getAsVector3(const String& name)
{
    String value = "";
    Vector3 vector3(0.0f, 0.0f, 0.0f);
    getProperty(name, value);
    if (!value.empty())
    {
        Vector<String> strVector;
        lynxStrSplit(value, ",", strVector, true);
        if (strVector.size() == 3)
        {
            vector3.mX = atof(strVector[0].c_str());
            vector3.mY = atof(strVector[1].c_str());
            vector3.mZ = atof(strVector[2].c_str());
        }
    }
    return vector3;
}
bool
ConfigManager::getAsVector3(const String& name, Vector3& value)
{
    String str = "";
    Vector3 vector3(0.0f, 0.0f, 0.0f);
    getProperty(name, str);
    if (!str.empty())
    {
        Vector<String> strVector;
        lynxStrSplit(str, ",", strVector, true);
        if (strVector.size() == 3)
        {
            vector3.mX = atof(strVector[0].c_str());
            vector3.mY = atof(strVector[1].c_str());
            vector3.mZ = atof(strVector[2].c_str());
        }
        value = vector3;
        return true;
    }
    return false;
}

void 
ConfigManager::setAsVector4(const String& name, const Vector4& value)
{
	char tmpBuf[128];
    snprintf(tmpBuf, sizeof(tmpBuf), "%f,%f,%f,%f", value.mX, value.mY, value.mZ, value.mW);
    setProperty(name, tmpBuf);
}

Vector4 
ConfigManager::getAsVector4(const String& name)
{
	String value = "";
    Vector4 vector4(0.0f, 0.0f, 0.0f, 0.0f);
    getProperty(name, value);
    if (!value.empty())
    {
        Vector<String> strVector;
        lynxStrSplit(value, ",", strVector, true);
        if (strVector.size() == 4)
        {
            vector4.mX = atof(strVector[0].c_str());
            vector4.mY = atof(strVector[1].c_str());
            vector4.mZ = atof(strVector[2].c_str());
			vector4.mW = atof(strVector[3].c_str());
        }
    }
    return vector4;
}

bool 
ConfigManager::getAsVector4(const String& name, Vector4& value)
{
	String str = "";
    Vector4 vector4(0.0f, 0.0f, 0.0f, 0.0f);
    getProperty(name, str);
    if (!str.empty())
    {
        Vector<String> strVector;
        lynxStrSplit(str, ",", strVector, true);
        if (strVector.size() == 4)
        {
            vector4.mX = atof(strVector[0].c_str());
            vector4.mY = atof(strVector[1].c_str());
            vector4.mZ = atof(strVector[2].c_str());
			vector4.mW = atof(strVector[3].c_str());
        }
        value = vector4;
        return true;
    }
    return false;
}

