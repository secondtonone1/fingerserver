#ifndef __LYNX_CSHARP_GENERATOR_H__
#define __LYNX_CSHARP_GENERATOR_H__

#include "PlatformLib.h"

namespace Lynx
{
	class CSharpCoder
	{
	public:
        CSharpCoder();
		void Generate(char* buf, TinyXmlNode* node, const String& namespaceName, const String& className, const String& parentName, const String& csharpCodes, UInt32 mask);
        void GenerateTable(char* buf, TinyXmlNode* node, const String& namespaceName, const String& className, const String& parentName, const String& csharpCodes);


    private:
		bool isBasicType(const String& type);
		bool isContainerType(const String& type);

		String getValueType(const TinyXmlNode* node);
		bool hasContainerType(const TinyXmlNode* node);

        bool printNamespace(char* buf, TinyXmlNode* node, const String& namespaceName, const String& className, const String& parentName, const String& csharpCodes, UInt32 mask);
        bool printNamespaceTable(char* buf, TinyXmlNode* node, const String& namespaceName, const String& className, const String& parentName, const String& csharpCodes);
        bool printClass(char* buf, TinyXmlNode* node, const String& className, const String& parentName, const String& indent, const String& csharpCodes, UInt32 mask);
        bool printClassTable(char* buf, TinyXmlNode* node, const String& className, const String& parentName, const String& indent, const String& csharpCodes);

		bool printClassMember(char* buf, const TinyXmlNode* node, const String& indent);
		bool printClassCreateFunc(char* buf, const String& className, const String& indent);
		bool printClassSerializeSizeFunc(char* buf, const TinyXmlNode* node, const String& indent);
		bool printClassSerializeFunc(char* buf, const TinyXmlNode* node, const String& indent);
		bool printClassUnserializeFunc(char* buf, const TinyXmlNode* node, const String& indent, bool isOverride);
        bool printClassCloneFunc(char* buf, const TinyXmlNode* node, const String& className, const String& indent);
        bool printClassGetSingletonFunc(char* buf, const TinyXmlNode* node, const String& className, const String& indent);
        bool printClassGetFunc(char* buf, const TinyXmlNode* node, const String& className, const String& indent);
        bool printClassClearFunc(char* buf, const TinyXmlNode* node, const String& className, const String& indent);
        bool printClassLoadFromDbcFunc(char* buf, const TinyXmlNode* node, const String& className, const String& indent);

	
		bool printListForSerializeSize(char* buf, const String& memberName, 
			const String& iterName, const TinyXmlNode* node, const String& indent);
		bool printListForSerialize(char* buf, const String& memberName, const String& iterName,
			const TinyXmlNode* node, const String& indent);
		bool printListForUnserialize(char* buf, const String& memberName, const String& iterName,
			const TinyXmlNode* node, const String& indent);
        bool printListForClone(char* buf, const String& objectName, const String& memberName, const String& iterName,
            const TinyXmlNode* node, const String& indent);


		bool printMapForSerializeSize(char* buf, const String& memberName, 
			const String& iterName, const TinyXmlNode* node, const String& indent);
		bool printMapForSerialize(char* buf, const String& memberName, const String& iterName,
			const TinyXmlNode* node, const String& indent);
		bool printMapForUnserialize(char* buf, const String& memberName, const String& iterName,
			const TinyXmlNode* node, const String& indent);
        bool printMapForClone(char* buf, const String& objectName, const String& memberName, const String& iterName,
            const TinyXmlNode* node, const String& indent);

        Set<String> mBasicTypeSet;
        Set<String> mContainerTypeSet;

		struct MemberInitValueInfo
		{
			MemberInitValueInfo() : mTypeName(""), mMemberName(""), mInitValue("") {}
			String mTypeName;
			String mMemberName;
			String mInitValue;
		};
		List<MemberInitValueInfo> mMemberInitValueInfoList;

		void addMemberInitValueInfo(const String& typeName, const String& memberName, const String& initValue);
	};
} // namespace Lynx

#endif // __LYNX_CSHARP_GENERATOR_H__


