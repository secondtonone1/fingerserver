#include "ResourceGroupManager.h"

#include "Logger.h"
#include "TinyXml.h"
#include "StreamBufferSerializeImpls.h"

using namespace Lynx;

ResourceGroupManager* gResourceGroupManager = NULL;

ResourceGroupManager::ResourceGroupManager() : mIsLoadFromXml(false)
{

}

ResourceGroupManager::~ResourceGroupManager()
{

}

bool
ResourceGroupManager::initial(const String& fileName, bool isLoadFromXml)
{
	gResourceGroupManager = this;

	mIsLoadFromXml = isLoadFromXml;
	if (mIsLoadFromXml)
	{
		return loadFromXml(fileName);
	}
	else
	{
		return loadFromBinary(fileName);
	}
}

void
ResourceGroupManager::release()
{
	mResourceGroupMap.clear();
}

bool
ResourceGroupManager::loadFromXml(const String& fileName)
{
	FILE* fp = fopen(fileName.c_str(), "rb");
	if (fp == NULL)
	{
		LOG_WARN("Failed to open resource config file %s", fileName.c_str());
		return false;
	}

	TinyXmlDocument doc;
    if (!doc.loadFile(fp))
    {
        LOG_WARN("Failed to load xml resourceconfig");
        return false;
    }

    fclose(fp);

	TinyXmlNode* node = doc.getFirstChild("config");
    if (node == 0)
    {
        LOG_WARN("Not found <config> element");
        return false;
    }

    TinyXmlElement* elem = node->toElement();
	TinyXmlNode* resourcegroupNode = node->getFirstChild("resourcegroup");
    if (resourcegroupNode == 0)
    {
        LOG_WARN("Not found <resourcegroup> element");
        return false;
    }
    elem = resourcegroupNode->toElement();
    do 
    {
        String groupName = elem->getAttribute("name");
        TinyXmlNode* subNode = elem->getFirstChild("resource");
        if (subNode == 0)
        {
            continue;
        }
        TinyXmlElement* subElem = subNode->toElement();
        ResourceGroup resourceGroup; 

        do 
        {
            String szPath = subElem->getAttribute("path");
            String szType = subElem->getAttribute("type");
            resourceGroup.insert(szPath, szType);
        } while ((subElem = subElem->nextSiblingElement("resource"))!= 0);

        mResourceGroupMap.insert(groupName, resourceGroup);

        
    } while ((elem = elem->nextSiblingElement("resourcegroup")) != 0);  
    return true;
}

bool
ResourceGroupManager::saveToBinary(const String& fileName)
{
	StreamBuffer streamBuffer;

	UInt32 size = serialize(mResourceGroupMap, streamBuffer, 13865);
	if (size <= 0)
	{
		LOG_WARN("Failed to serialize resource group map.");
		return false;
	}

	if (!FileUtil::saveFile(fileName, streamBuffer))
	{
		LOG_WARN("Failed to save file %s", fileName.c_str());
		return false;
	}
	return true;
}

bool
ResourceGroupManager::loadFromBinary(const String& fileName)
{
	StreamBuffer streamBuffer;

	if (!FileUtil::loadFile(fileName, streamBuffer))
	{
		LOG_WARN("Failed to load file %s", fileName.c_str());
		return false;
	}

	if (!unserialize(mResourceGroupMap, streamBuffer, 13865))
	{
		LOG_WARN("Failed to unserialize resource group.");
		return false;
	}
	return true;
}

String
ResourceGroupManager::getFilePath(const String& fileName, const String& resourceGroupName)
{
	String tmpFileName = fileName;
	lynxToLowerCase(tmpFileName);

	if (resourceGroupName.empty())
	{
		for (ResourceGroupMap::Iter* iter = mResourceGroupMap.begin();
			iter != mResourceGroupMap.end(); iter = mResourceGroupMap.next(iter))
		{
			ResourceGroup& resourceGroup = iter->mValue;
			for (ResourceGroup::Iter* jter = resourceGroup.begin();
				jter != resourceGroup.end(); jter = resourceGroup.next(jter))
			{
				if (!strncasecmp("File", jter->mValue.c_str(), 4))
				{
					StringMap<String> filePathMap;
					FileUtil::findFiles(jter->mKey, tmpFileName, filePathMap);
					if (!filePathMap.empty())
					{
						return filePathMap.begin()->mValue;
					}
				}
			}
		}
	}
	else
	{
		ResourceGroupMap::Iter* iter = mResourceGroupMap.find(resourceGroupName);
        if (iter != mResourceGroupMap.end())
        {
            ResourceGroup resourceGroup = iter->mValue;
            for (ResourceGroup::Iter* jter = resourceGroup.begin(); 
                jter != resourceGroup.end(); jter = resourceGroup.next(jter))
            {
                if (!strncasecmp("File", jter->mValue.c_str(), 4))
                {
                    StringMap<String> filePathMap;
                    FileUtil::findFiles(jter->mKey, tmpFileName, filePathMap);
                    if (!filePathMap.empty())
                    {
                        return filePathMap.begin()->mValue;
                    }
                }
            }
        }
	}
	return "";
}

Int32
ResourceGroupManager::loadFileToStream(const String& fileName, StreamBuffer& streamBuffer,
									   const String& resourceGroupName)
{
	String tmpFileName = fileName;
    lynxToLowerCase(tmpFileName);

    if (resourceGroupName.empty())
    {
        for (ResourceGroupMap::Iter* iter = mResourceGroupMap.begin(); 
            iter != mResourceGroupMap.end(); iter = mResourceGroupMap.next(iter))
        {
            ResourceGroup resourceGroup = iter->mValue;
            for (ResourceGroup::Iter* jter = resourceGroup.begin(); 
                jter != resourceGroup.end(); jter = resourceGroup.next(jter))
            { 
                if (!strncasecmp("File", jter->mValue.c_str(), 4))
                { 
                    StringMap<String> filePathMap;
                    FileUtil::findFiles(jter->mKey, tmpFileName, filePathMap);
                    if (!filePathMap.empty())
                    {
                        if (!FileUtil::loadFile(filePathMap.begin()->mValue, streamBuffer))
                        {
                            continue;
                        }
                        return streamBuffer.length();
                    }
                }
            }
        }
    }
    else
    {
        ResourceGroupMap::Iter* iter = mResourceGroupMap.find(resourceGroupName);
        if (iter != mResourceGroupMap.end())
        {
            ResourceGroup resourceGroup = iter->mValue;
            for (ResourceGroup::Iter* jter = resourceGroup.begin(); 
                jter != resourceGroup.end(); jter = resourceGroup.next(jter))
            { 
                if (!strncasecmp("File", jter->mValue.c_str(), 4))
                { 
                    StringMap<String> filePathMap;
                    FileUtil::findFiles(jter->mKey, tmpFileName, filePathMap);
                    if (!filePathMap.empty())
                    {
                        if (!FileUtil::loadFile(filePathMap.begin()->mValue, streamBuffer))
                        {
                            LOG_WARN("Failed to load file %s", filePathMap.begin()->mValue.c_str());
                            return -1;
                        }
                        return streamBuffer.length();
                    }
                }
            }
        }
    }
    return -1;
}

Int32
ResourceGroupManager::loadFileToBuffer(const String& fileName, char** buffer,
									   const String& resourceGroupName)
{
	String tmpFileName = fileName;
    lynxToLowerCase(tmpFileName);

    if (resourceGroupName.empty())
    {
        for (ResourceGroupMap::Iter* iter = mResourceGroupMap.begin(); 
            iter != mResourceGroupMap.end(); iter = mResourceGroupMap.next(iter))
        {
            ResourceGroup resourceGroup = iter->mValue;
            for (ResourceGroup::Iter* jter = resourceGroup.begin(); 
                jter != resourceGroup.end(); jter = resourceGroup.next(jter))
            { 
                if (!strncasecmp("File", jter->mValue.c_str(), 4))
                { 
                    StringMap<String> filePathMap;
                    FileUtil::findFiles(jter->mKey, tmpFileName, filePathMap);
                    if (!filePathMap.empty())
                    {
                        return FileUtil::loadFile(filePathMap.begin()->mValue, buffer);
                    }
                }
            }
        }
    }
    else
    {
        ResourceGroupMap::Iter* iter = mResourceGroupMap.find(resourceGroupName);
        if (iter != mResourceGroupMap.end())
        {
            ResourceGroup resourceGroup = iter->mValue;
            for (ResourceGroup::Iter* jter = resourceGroup.begin(); 
                jter != resourceGroup.end(); jter = resourceGroup.next(jter))
            { 
                if (!strncasecmp("File", jter->mValue.c_str(), 4))
                { 
                    StringMap<String> filePathMap;
                    FileUtil::findFiles(jter->mKey, tmpFileName, filePathMap);
                    if (!filePathMap.empty())
                    {
                        return FileUtil::loadFile(filePathMap.begin()->mValue, buffer);
                    }
                }
            }
        }
    }
    return -1;
}

void
ResourceGroupManager::freeBuffer(char* buffer)
{
	FileUtil::freeBuffer(buffer);
}

bool
ResourceGroupManager::getResourceGroup(const String& resourceGroupName, ResourceGroup& resourceGroup)
{
	ResourceGroupMap::Iter* iter = mResourceGroupMap.find(resourceGroupName);
	if (iter != mResourceGroupMap.end())
	{
		resourceGroup = iter->mValue;
		return true;
	}
	return false;
}

void 
ResourceGroupManager::findFiles(const String& resourceGroupName, const String& pattern, 
                                List<String>& filePaths)
{
    String tmpPattern = pattern;
    lynxToLowerCase(tmpPattern);
   
    ResourceGroupMap::Iter* iter = mResourceGroupMap.find(resourceGroupName);
    if (iter != mResourceGroupMap.end())
    {
        ResourceGroup resourceGroup = iter->mValue;
        for (ResourceGroup::Iter* jter = resourceGroup.begin(); 
            jter != resourceGroup.end(); jter = resourceGroup.next(jter))
        {
            if (!strncasecmp("File", jter->mValue.c_str(), 4))
            {
                FileUtil::findFiles(jter->mKey, tmpPattern, filePaths);
            }
        }
    }
}

void 
ResourceGroupManager::findFiles(const String& resourceGroupName, const String& pattern, 
                                StringMap<String>& filePaths)
{
    String tmpPattern = pattern;
    lynxToLowerCase(tmpPattern);

    ResourceGroupMap::Iter* iter = mResourceGroupMap.find(resourceGroupName);
    if (iter != mResourceGroupMap.end())
    {
        ResourceGroup resourceGroup = iter->mValue;
        for (ResourceGroup::Iter* jter = resourceGroup.begin(); 
            jter != resourceGroup.end(); jter = resourceGroup.next(jter))
        {
            if (!strncasecmp("File", jter->mValue.c_str(), 4))
            {
                FileUtil::findFiles(jter->mKey, tmpPattern, filePaths);
			}
        }
    }
}

bool 
ResourceGroupManager::fileIsExist(const String& resourceGroupName, const String& pattern)
{
    List<String> filePaths;
    findFiles(resourceGroupName, pattern, filePaths);
    if (filePaths.empty())
    {
        return false;
    }
    return true;
}
