#if 0
#ifndef __LYNX_CPLUSPLUS_GENERATOR_H__
#define __LYNX_CPLUSPLUS_GENERATOR_H__

#include "PlatformLib.h"

namespace Lynx
{
	class CPlusPlusCoder
	{
	public:
		static void initial();

		static bool isBasicType(const String& type);
		static bool isContainerType(const String& type);

		static String getValueType(const TinyXmlNode* node);
		static bool hasContainerType(const TinyXmlNode* node);

		static bool printNamespaceBegin(char* buf, const String& namespaceName, const String& indent);
		static bool printNamespaceEnd(char* buf, const String& indent);

		static bool printClassBegin(char* buf, const String& className, const String& parentClassName, const String& indent);
		static bool printClassEnd(char* buf, const String& indent);
		static bool printClassMember(char* buf, const TinyXmlNode* node, const String& indent);
		static bool printClassSerializeSizeFunc(char* buf, const TinyXmlNode* node, const String& indent);
		static bool printClassSerializeFunc(char* buf, const TinyXmlNode* node, const String& indent, UInt32 mask);
		static bool printClassUnserializeFunc(char* buf, const TinyXmlNode* node, const String& indent, UInt32 mask);

	private:
		static bool printVectorForSerializeSize(char* buf, const String& memberName, 
			const String& iterName, const TinyXmlNode* node, const String& indent);
		static bool printVectorForSerialize(char* buf, const String& memberName, const String& iterName,
			const TinyXmlNode* node, const String& indent, UInt32 mask);
		static bool printVectorForUnserialize(char* buf, const String& memberName, const String& iterName,
			const TinyXmlNode* node, const String& indent, UInt32 mask);

		static bool printListForSerializeSize(char* buf, const String& memberName, 
			const String& iterName, const TinyXmlNode* node, const String& indent);
		static bool printListForSerialize(char* buf, const String& memberName, const String& iterName,
			const TinyXmlNode* node, const String& indent, UInt32 mask);
		static bool printListForUnserialize(char* buf, const String& memberName, const String& iterName,
			const TinyXmlNode* node, const String& indent, UInt32 mask);

		static bool printDequeForSerializeSize(char* buf, const String& memberName, 
			const String& iterName, const TinyXmlNode* node, const String& indent);
		static bool printDequeForSerialize(char* buf, const String& memberName, const String& iterName,
			const TinyXmlNode* node, const String& indent, UInt32 mask);
		static bool printDequeForUnserialize(char* buf, const String& memberName, const String& iterName,
			const TinyXmlNode* node, const String& indent, UInt32 mask);

		static bool printMapForSerializeSize(char* buf, const String& memberName, 
			const String& iterName, const TinyXmlNode* node, const String& indent);
		static bool printMapForSerialize(char* buf, const String& memberName, const String& iterName,
			const TinyXmlNode* node, const String& indent, UInt32 mask);
		static bool printMapForUnserialize(char* buf, const String& memberName, const String& iterName,
			const TinyXmlNode* node, const String& indent, UInt32 mask);

		static bool printSetForSerializeSize(char* buf, const String& memberName, 
			const String& iterName, const TinyXmlNode* node, const String& indent);
		static bool printSetForSerialize(char* buf, const String& memberName, const String& iterName,
			const TinyXmlNode* node, const String& indent, UInt32 mask);
		static bool printSetForUnserialize(char* buf, const String& memberName, const String& iterName,
			const TinyXmlNode* node, const String& indent, UInt32 mask);
	};
} // namespace Lynx

#endif // __LYNX_CPLUSPLUS_GENERATOR_H__
#endif 

