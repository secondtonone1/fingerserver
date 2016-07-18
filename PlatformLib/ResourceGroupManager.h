#ifndef __LYNX_PLATFROM_LIB_RESOURCE_GROUP_MANAGER_H__
#define __LYNX_PLATFROM_LIB_RESOURCE_GROUP_MANAGER_H__

#include "Compat.h"
#include "Utilex.h"
#include "Singleton.h"
#include "Container.h"
#include "StreamBuffer.h"
#include "FileUtil.h"

namespace Lynx
{
	typedef StringMap<String> ResourceGroup;
	typedef StringMap<ResourceGroup> ResourceGroupMap;

	class _PlatformExport ResourceGroupManager : public Singleton<ResourceGroupManager>
	{
	public:
		ResourceGroupManager();
		virtual ~ResourceGroupManager();

		bool initial(const String& fileName, bool isLoadFromXml = false);
		void release();

		bool loadFromXml(const String& fileName);
		bool saveToBinary(const String& fileName);
		bool loadFromBinary(const String& fileName);

		String getFilePath(const String& fileName, const String& resourceGroupName = "");

		Int32 loadFileToStream(const String& fileName, StreamBuffer& streamBuffer, const String& resourceGroupName = "");
		Int32 loadFileToBuffer(const String& fileName, char** buffer, const String& resourceGroupName = "");
		void freeBuffer(char* buffer);

		void findFiles(const String& resourceGroupName, const String& pattern, List<String>& filePaths);
		void findFiles(const String& resourceGroupName, const String& pattern, StringMap<String>& filePaths);

		bool fileIsExist(const String& resourceGroupName, const String& pattern);

		bool getResourceGroup(const String& resourceGroupName, ResourceGroup& resourceGroup);

	private:
		ResourceGroupMap mResourceGroupMap;
		StringMap<String> mFilePathMap;
		bool mIsLoadFromXml;
	};
} // namespace Lynx

extern _PlatformExport Lynx::ResourceGroupManager* gResourceGroupManager;
#define RESOURCE_GROUP_MANAGER() (*gResourceGroupManager)

#endif // __LYNX_PLATFROM_LIB_RESOURCE_GROUP_MANAGER_H__

