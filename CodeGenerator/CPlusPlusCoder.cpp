#if 0
#include "CPlusPlusCoder.h"

using namespace Lynx;

static Set<String> sBasicTypeSet;
static Set<String> sContainerTypeSet;

void
CPlusPlusCoder::initial()
{
	sBasicTypeSet.insert("Bool");
	sBasicTypeSet.insert("Int8");
	sBasicTypeSet.insert("UInt8");
	sBasicTypeSet.insert("Int16");
	sBasicTypeSet.insert("UInt16");
	sBasicTypeSet.insert("Int32");
	sBasicTypeSet.insert("UInt32");
	sBasicTypeSet.insert("Int64");
	sBasicTypeSet.insert("UInt64");
	sBasicTypeSet.insert("Float");
	sBasicTypeSet.insert("String");
	sBasicTypeSet.insert("Vector2");
	sBasicTypeSet.insert("Vector3");
	sBasicTypeSet.insert("Vector4");

	sContainerTypeSet.insert("Pair");
	sContainerTypeSet.insert("Map");
	sContainerTypeSet.insert("Set");
	sContainerTypeSet.insert("List");
	sContainerTypeSet.insert("Deque");
	sContainerTypeSet.insert("Vector");
}

bool 
CPlusPlusCoder::isBasicType(const String& type)
{
	Set<String>::Iter* iter = sBasicTypeSet.find(type);
	if (iter)
	{
		return true;
	}
	return false;
}

bool 
CPlusPlusCoder::isContainerType(const String& type)
{
	Set<String>::Iter* iter = sContainerTypeSet.find(type);
	if (iter)
	{
		return true;
	}
	return false;
}

String 
CPlusPlusCoder::getValueType(const TinyXmlNode* node)
{
	char buf[1024] = { 0 };
	int i = 0;
	bool isMapFlag = false;
	do 
	{
		String typeName = node->toElement()->getAttribute("type");

		if (isBasicType(typeName))
		{
			if (i == 0)
			{
			    return typeName;
			}
			else
			{
				sprintf(buf, "%s%s", buf, typeName.c_str());
				break;
			}
		}
		else if (isContainerType(typeName))
		{
		    if (typeName == "Vector")
			{
				sprintf(buf, "%sVector<", buf);
				i++;
			}
			else if (typeName == "List")
			{
				sprintf(buf, "%sList<", buf);
				i++;
			}
			else if (typeName == "Deque")
			{
				sprintf(buf, "%sDeque<", buf);
				i++;
			}
			else if (typeName == "Set")
			{
				sprintf(buf, "%sSet<", buf);
				i++;
			}
			else if (typeName == "Map")
			{
				String keyType = node->getFirstChild()->getFirstChild("first")->toElement()->getAttribute("type");
				String valueType = getValueType(node->getFirstChild()->getFirstChild("second"));

				sprintf(buf, "%sMap<%s, %s >", buf, keyType.c_str(), valueType.c_str());
				break;
			}
		}
		else // OBJECT
		{
			if (i == 0)
			{
			    return typeName;
			}
			else
			{
				sprintf(buf, "%s%s", buf, typeName.c_str());
				break;
			}
		}
	} while (node = node->getFirstChild());

	for (int j = 0; j < i; ++j)
	{
		sprintf(buf, "%s >", buf);
	}
	return buf;
}

bool 
CPlusPlusCoder::hasContainerType(const TinyXmlNode* node)
{
	const TinyXmlElement* elem = node->getFirstChild()->toElement();
	String memberType;

	do
	{
		memberType = elem->getAttribute("type");

		if (isContainerType(memberType))
		{
			return true;
		}
	} while (elem = elem->nextSiblingElement());

	return false;
}

bool 
CPlusPlusCoder::printNamespaceBegin(char* buf, const String& namespaceName, const String& indent)
{
	if (namespaceName.empty())
	{
		sprintf(buf, "%s%s{\r\n", buf, indent.c_str());
	}
	else
	{
		sprintf(buf, "%s%snamespace %s\r\n", buf, indent.c_str(), namespaceName.c_str());
		sprintf(buf, "%s%s{\r\n", buf, indent.c_str());
	}
	return true;
}

bool 
CPlusPlusCoder::printNamespaceEnd(char* buf, const String& indent)
{
	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}

bool 
CPlusPlusCoder::printClassBegin(char* buf, const String& className, const String& parentClassName, const String& indent)
{
	if (!parentClassName.empty())
	{
		sprintf(buf, "%s%sstruct %s : public %s \r\n", buf, indent.c_str(), className.c_str(), parentClassName.c_str());
	}
	else
	{
		sprintf(buf, "%s%sstruct %s\r\n", buf, indent.c_str(), className.c_str());
	}
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());
	return true;
}

bool 
CPlusPlusCoder::printClassEnd(char* buf, const String& indent)
{
	sprintf(buf, "%s%s};\r\n", buf, indent.c_str());
	return true;
}

bool 
CPlusPlusCoder::printClassMember(char* buf, const TinyXmlNode* node, const String& indent)
{
	const TinyXmlElement* elem = node->getFirstChild()->toElement();
	String memberType, memberName;
	
	node = node->getFirstChild();
	do
	{
		//memberType = elem->getAttribute("type");
		memberType = getValueType(node);
		memberName = node->toElement()->getValue();

		if (isBasicType(memberType))
		{
			if (memberType == "Bool")
			{
				sprintf(buf, "%s%s%s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
			}
			else if (memberType == "String")
			{
				sprintf(buf, "%s%s%s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
			}
			else if (memberType == "Float")
			{
				sprintf(buf, "%s%s%s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
			}
			else if (memberType == "Int8")
			{
				sprintf(buf, "%s%s%s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
			}
			else if (memberType == "UInt8")
			{
				sprintf(buf, "%s%s%s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
			}
			else if (memberType == "Int16")
			{
				sprintf(buf, "%s%s%s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
			}
			else if (memberType == "UInt16")
			{
				sprintf(buf, "%s%s%s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
			}
			else if (memberType == "Int32")
			{
				sprintf(buf, "%s%s%s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
			}
			else if (memberType == "UInt32")
			{
				sprintf(buf, "%s%s%s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
			}
			else if (memberType == "Int64")
			{
				sprintf(buf, "%s%s%s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
			}
			else if (memberType == "UInt64")
			{
				sprintf(buf, "%s%s%s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
			}
			else if (memberType == "Vector2")
			{
				sprintf(buf, "%s%s%s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
			}
			else if (memberType == "Vector3")
			{
				sprintf(buf, "%s%s%s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
			}
			else if (memberType == "Vector4")
			{
				sprintf(buf, "%s%s%s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
			}
			else if (memberType == "Double")
			{
				LOG_WARN("Invalid Type [Double].");
				return false;
			}
		}
		else if (isContainerType(memberType))
		{
			if (memberType == "Pair")
			{
				LOG_WARN("Invalid type [Pair]");
				return false;
			}
			else if (memberType == "Vector")
			{
				String valueType = elem->getFirstChild()->toElement()->getAttribute("type");
				sprintf(buf, "%s%sVector<%s > %s;\r\n", buf, indent.c_str(), valueType.c_str(), memberName.c_str());
			}
			else if (memberType == "List")
			{
				String valueType = elem->getFirstChild()->toElement()->getAttribute("type");
				sprintf(buf, "%s%sList<%s > %s;\r\n", buf, indent.c_str(), valueType.c_str(), memberName.c_str()); 
			}
			else if (memberType == "Deque")
			{
				String valueType = elem->getFirstChild()->toElement()->getAttribute("type");
				sprintf(buf, "%s%sDeque<%s > %s;\r\n", buf, indent.c_str(), valueType.c_str(), memberName.c_str()); 
			}
			else if(memberType == "Set")
			{
				String valueType = elem->getFirstChild()->toElement()->getAttribute("type");
				sprintf(buf, "%s%sSet<%s > %s;\r\n", buf, indent.c_str(), valueType.c_str(), memberName.c_str()); 
			}
			else if (memberType == "Map")
			{
				String keyType = elem->getFirstChild()->getFirstChild("first")->toElement()->getAttribute("type");
				String valueType = elem->getFirstChild()->getFirstChild("second")->toElement()->getAttribute("type");
				sprintf(buf, "%s%sMap<%s, %s > %s;\r\n", buf, indent.c_str(), keyType.c_str(), valueType.c_str(), memberName.c_str());
			}
		}
		else // Object
		{
			sprintf(buf, "%s%s%s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
		}
	} while (node = node->nextSiblingElement());

	sprintf(buf, "%s\r\n", buf);
	
	return true;
}

bool 
CPlusPlusCoder::printClassSerializeSizeFunc(char* buf, const TinyXmlNode* node, const String& indent)
{
	sprintf(buf, "%s%sUInt32 serializeSize()\r\n", buf, indent.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

	String childIndent = indent + "    ";

	sprintf(buf, "%s%sUInt32 returnSize = 0;\r\n", buf, childIndent.c_str());
	if (hasContainerType(node))
	{
		sprintf(buf, "%s%sUInt32 containerSize = 0;\r\n", buf, childIndent.c_str());
	}

	const TinyXmlElement* elem = node->getFirstChild()->toElement();
	String memberName, memberType;

	do
	{
		memberType = elem->getAttribute("type");
		memberName = elem->getValue();

		if (isBasicType(memberType))
		{
			sprintf(buf, "%s%sreturnSize += serializeSize(%s);\r\n", buf, childIndent.c_str(), memberName.c_str());
		}
		else if (isContainerType(memberType))
		{
			if (memberType == "Pair")
			{
				LOG_WARN("Invalid Type [Pair].");
				return false;
			}
			else if (memberType == "Vector")
			{
				if (!printVectorForSerializeSize(buf, memberName, "i", elem->getFirstChild(), childIndent))
				{
					return false;
				}
			}
			else if (memberType == "List")
			{
				if (!printListForSerializeSize(buf, memberName, "i", elem->getFirstChild(), childIndent))
				{
					return false;
				}
			}
			else if (memberType == "Deque")
			{
				if (!printDequeForSerializeSize(buf, memberName, "i", elem->getFirstChild(), childIndent))
				{
					return false;
				}
			}
			else if (memberType == "Map")
			{
				if (!printMapForSerializeSize(buf, memberName, "i", elem->getFirstChild(), childIndent))
				{
					return false;
				}
			}
			else if (memberType == "Set")
			{
				if (!printSetForSerializeSize(buf, memberName, "i", elem->getFirstChild(), childIndent))
				{
					return false;
				}
			}
		}
		else // Object
		{
			sprintf(buf, "%s%sreturnSize += %s.serializeSize();\r\n", buf, childIndent.c_str(), memberName.c_str());
		}
	} while (elem = elem->nextSiblingElement());

	sprintf(buf, "%s%sreturn returnSize;\r\n", buf, childIndent.c_str());
	sprintf(buf, "%s%s}\r\n\r\n", buf, indent.c_str());
	return true;
}

bool
CPlusPlusCoder::printClassSerializeFunc(char* buf, const TinyXmlNode* node, const String& indent, UInt32 mask)
{
	sprintf(buf, "%s%sUInt32 serialize(StreamBuffer& stream, UInt32 mask)\r\n", buf, indent.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

	String childIndent = indent + "    ";

	sprintf(buf, "%s%sUInt32 returnSize = 0;\r\n", buf, childIndent.c_str());
	if (hasContainerType(node))
	{
		sprintf(buf, "%s%sUInt32 containerSize = 0;\r\n", buf, childIndent.c_str());
	}

	const TinyXmlElement* elem = node->getFirstChild()->toElement();
	String memberName, memberType;

	do
	{
		memberType = elem->getAttribute("type");
		memberName = elem->getValue();

		if (isBasicType(memberType))
		{
			sprintf(buf, "%s%sreturnSize += serialize(%s, stream, %u);\r\n", buf, childIndent.c_str(), memberName.c_str(), mask);
		}
		else if (isContainerType(memberType))
		{
			if (memberType == "Pair")
			{
				LOG_WARN("Invalid Type [Pair].");
				return false;
			}
			else if (memberType == "Vector")
			{
				if (!printVectorForSerialize(buf, memberName, "i", elem->getFirstChild(), childIndent, mask))
				{
					return false;
				}
			}
			else if (memberType == "List")
			{
				if (!printListForSerialize(buf, memberName, "i", elem->getFirstChild(), childIndent, mask))
				{
					return false;
				}
			}
			else if (memberType == "Deque")
			{
				if (!printDequeForSerialize(buf, memberName, "i", elem->getFirstChild(), childIndent, mask))
				{
					return false;
				}
			}
			else if (memberType == "Map")
			{
				if (!printMapForSerialize(buf, memberName, "i", elem->getFirstChild(), childIndent, mask))
				{
					return false;
				}
			}
			else if (memberType == "Set")
			{
				if (!printSetForSerialize(buf, memberName, "i", elem->getFirstChild(), childIndent, mask))
				{
					return false;
				}
			}
		}
		else // Object
		{
			sprintf(buf, "%s%sreturnSize += %s.serialize(stream, %u);\r\n", buf, childIndent.c_str(), memberName.c_str(), mask);
		}
	} while (elem = elem->nextSiblingElement());

	sprintf(buf, "%s%sreturn returnSize;\r\n", buf, childIndent.c_str());
	sprintf(buf, "%s%s}\r\n\r\n", buf, indent.c_str());
	return true;
}

bool 
CPlusPlusCoder::printClassUnserializeFunc(char* buf, const TinyXmlNode* node, const String& indent, UInt32 mask)
{
	sprintf(buf, "%s%sUInt32 unserialize(StreamBuffer& stream, UInt32 mask)\r\n", buf, indent.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

	String childIndent = indent + "    ";

	const TinyXmlElement* elem = node->getFirstChild()->toElement();
	String memberName, memberType;

	do
	{
		memberType = elem->getAttribute("type");
		memberName = elem->getValue();

		if (isBasicType(memberType))
		{
			sprintf(buf, "%s%sif (!unserialize(%s, stream, %u)) return false;\r\n", buf, childIndent.c_str(), memberName.c_str(), mask);
		}
		else if (isContainerType(memberType))
		{
			if (memberType == "Pair")
			{
				LOG_WARN("Invalid Type [Pair].");
				return false;
			}
			else if (memberType == "Vector")
			{
				if (!printVectorForUnserialize(buf, memberName, "i", elem->getFirstChild(), childIndent, mask))
				{
					return false;
				}
			}
			else if (memberType == "List")
			{
				if (!printListForUnserialize(buf, memberName, "i", elem->getFirstChild(), childIndent, mask))
				{
					return false;
				}
			}
			else if (memberType == "Deque")
			{
				if (!printDequeForUnserialize(buf, memberName, "i", elem->getFirstChild(), childIndent, mask))
				{
					return false;
				}
			}
			else if (memberType == "Map")
			{
				if (!printMapForUnserialize(buf, memberName, "i", elem->getFirstChild(), childIndent, mask))
				{
					return false;
				}
			}
			else if (memberType == "Set")
			{
				if (!printSetForUnserialize(buf, memberName, "i", elem->getFirstChild(), childIndent, mask))
				{
					return false;
				}
			}
		}
		else // Object
		{
			sprintf(buf, "%s%sif (!%s.unserialize(stream, %u)) return false;\r\n", buf, childIndent.c_str(), memberName.c_str(), mask);
		}
	} while (elem = elem->nextSiblingElement());

	sprintf(buf, "%s%sreturn 0;\r\n", buf, childIndent.c_str());
	sprintf(buf, "%s%s}\r\n\r\n", buf, indent.c_str());
	return true;
}

bool 
CPlusPlusCoder::printVectorForSerializeSize(char* buf, const String& memberName, 
			                               const String& iterName, const TinyXmlNode* node, const String& indent)
{
	String valueType = getValueType(node);

	sprintf(buf, "%s%scontainerSize = %s.size();\r\n", buf, indent.c_str(), memberName.c_str());
	sprintf(buf, "%s%sreturnSize += serializeSize(containerSize);\r\n", buf, indent.c_str());

	sprintf(buf, "%s%sfor (Vector<%s >::Iter* %s = %s.begin(); %s != NULL; %s = %s.next(%s))\r\n", 
		buf, indent.c_str(), valueType.c_str(), iterName.c_str(), memberName.c_str(),
		iterName.c_str(), iterName.c_str(), memberName.c_str(), iterName.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

	String childIndent = indent + "    ";

	valueType = node->toElement()->getAttribute("type");

	if (isBasicType(valueType))
	{
		sprintf(buf, "%s%sreturnSize += serializeSize(%s->mValue);\r\n", buf, childIndent.c_str(), iterName.c_str());
	}
	else if (isContainerType(valueType))
	{
		String childMemberName = iterName + "->mValue";
		String childIterName = iterName + "i";

		if (valueType == "Vector")
		{
			if (!printVectorForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else if (valueType == "List")
		{
			if (!printListForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else if (valueType == "Deque")
		{
			if (!printDequeForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else if (valueType == "Map")
		{
			if (!printMapForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else if (valueType == "Set")
		{
			if (!printSetForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
	}
	else // Object
	{
		sprintf(buf, "%s%sreturnSize += %s->mValue.serializeSize();\r\n", buf, childIndent.c_str(), iterName.c_str());
	}
	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}

bool 
CPlusPlusCoder::printVectorForSerialize(char* buf, const String& memberName, const String& iterName,
									   const TinyXmlNode* node, const String& indent, UInt32 mask)
{
	String valueType = getValueType(node);

	sprintf(buf, "%s%scontainerSize = %s.size();\r\n", buf, indent.c_str(), memberName.c_str());
	sprintf(buf, "%s%sreturnSize += serialize(containerSize, stream, %u);\r\n", buf, indent.c_str(), mask);

	sprintf(buf, "%s%sfor (Vector<%s >::Iter* %s = %s.begin(); %s != NULL; %s = %s.next(%s))\r\n", 
		buf, indent.c_str(), valueType.c_str(), iterName.c_str(), memberName.c_str(),
		iterName.c_str(), iterName.c_str(), memberName.c_str(), iterName.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

	String childIndent = indent + "    ";

	valueType = node->toElement()->getAttribute("type");

	if (isBasicType(valueType))
	{
		sprintf(buf, "%s%sreturnSize += serialize(%s->mValue, stream, %u);\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}
	else if (isContainerType(valueType))
	{
		String childMemberName = iterName + "->mValue";
		String childIterName = iterName + "i";

		if (valueType == "Vector")
		{
			if (!printVectorForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "List")
		{
			if (!printListForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Deque")
		{
			if (!printDequeForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Map")
		{
			if (!printMapForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Set")
		{
			if (!printSetForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
	}
	else // Object
	{
		sprintf(buf, "%s%sreturnSize += %s->mValue.serialize(stream, %u);\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}
	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}

bool 
CPlusPlusCoder::printVectorForUnserialize(char* buf, const String& memberName, const String& iterName,
										 const TinyXmlNode* node, const String& indent, UInt32 mask)
{
    String valueType = getValueType(node);

	sprintf(buf, "%s%s//Vector<%s >\r\n", buf, indent.c_str(), valueType.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());
	String tmpIndent = indent + "    ";
	sprintf(buf, "%s%sUInt32 %sSize = 0;\r\n", buf, tmpIndent.c_str(), iterName.c_str());

	sprintf(buf, "%s%sif (!unserialize(%sSize, stream, %u)) return false;\r\n", buf, tmpIndent.c_str(), iterName.c_str(), mask);

	sprintf(buf, "%s%sfor (UInt32 %s = 0; %s < %sSize; ++%s)\r\n", buf, tmpIndent.c_str(), 
		iterName.c_str(), iterName.c_str(), iterName.c_str(), iterName.c_str()); 
	sprintf(buf, "%s%s{\r\n", buf, tmpIndent.c_str());

	String childIndent = tmpIndent + "    ";

	valueType = node->toElement()->getAttribute("type");

	if (isBasicType(valueType))
	{
		sprintf(buf, "%s%sif (!unserialize(%s->mValue, stream, %u)) return false;\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}
	else if (isContainerType(valueType))
	{
		String childMemberName = iterName + "->mValue";
		String childIterName = iterName + "i";

		if (valueType == "Vector")
		{
			if (!printVectorForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "List")
		{
			if (!printListForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Deque")
		{
			if (!printDequeForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Map")
		{
			if (!printMapForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Set")
		{
			if (!printSetForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
	}
	else // Object
	{
		sprintf(buf, "%s%sif (!%s->mValue.serialize(stream, %u)) return false;\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}
	sprintf(buf, "%s%s}\r\n", buf, tmpIndent.c_str());
	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}

bool 
CPlusPlusCoder::printListForSerializeSize(char* buf, const String& memberName, 
			                             const String& iterName, const TinyXmlNode* node, const String& indent)
{
	String valueType = getValueType(node);

	sprintf(buf, "%s%scontainerSize = %s.size();\r\n", buf, indent.c_str(), memberName.c_str());
	sprintf(buf, "%s%sreturnSize += serializeSize(containerSize);\r\n", buf, indent.c_str());

	sprintf(buf, "%s%sfor (List<%s >::Iter* %s = %s.begin(); %s != NULL; %s = %s.next(%s))\r\n", 
		buf, indent.c_str(), valueType.c_str(), iterName.c_str(), memberName.c_str(),
		iterName.c_str(), iterName.c_str(), memberName.c_str(), iterName.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

	String childIndent = indent + "    ";

	valueType = node->toElement()->getAttribute("type");

	if (isBasicType(valueType))
	{
		sprintf(buf, "%s%sreturnSize += serializeSize(%s->mValue);\r\n", buf, childIndent.c_str(), iterName.c_str());
	}
	else if (isContainerType(valueType))
	{
		String childMemberName = iterName + "->mValue";
		String childIterName = iterName + "i";

		if (valueType == "Vector")
		{
			if (!printVectorForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else if (valueType == "List")
		{
			if (!printListForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else if (valueType == "Deque")
		{
			if (!printDequeForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else if (valueType == "Map")
		{
			if (!printMapForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else if (valueType == "Set")
		{
			if (!printSetForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
	}
	else // Object
	{
		sprintf(buf, "%s%sreturnSize += %s->mValue.serializeSize();\r\n", buf, childIndent.c_str(), iterName.c_str());
	}

	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}

bool 
CPlusPlusCoder::printListForSerialize(char* buf, const String& memberName, const String& iterName,
									 const TinyXmlNode* node, const String& indent, UInt32 mask)
{
	String valueType = getValueType(node);

	sprintf(buf, "%s%scontainerSize = %s.size();\r\n", buf, indent.c_str(), memberName.c_str());
	sprintf(buf, "%s%sreturnSize += serialize(containerSize, stream, %u);\r\n", buf, indent.c_str(), mask);

	sprintf(buf, "%s%sfor (List<%s >::Iter* %s = %s.begin(); %s != NULL; %s = %s.next(%s))\r\n", 
		buf, indent.c_str(), valueType.c_str(), iterName.c_str(), memberName.c_str(),
		iterName.c_str(), iterName.c_str(), memberName.c_str(), iterName.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

	String childIndent = indent + "    ";

	valueType = node->toElement()->getAttribute("type");

	if (isBasicType(valueType))
	{
		sprintf(buf, "%s%sreturnSize += serialize(%s->mValue, stream, %u);\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}
	else if (isContainerType(valueType))
	{
		String childMemberName = iterName + "->mValue";
		String childIterName = iterName + "i";

		if (valueType == "Vector")
		{
			if (!printVectorForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "List")
		{
			if (!printListForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Deque")
		{
			if (!printDequeForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Map")
		{
			if (!printMapForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Set")
		{
			if (!printSetForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
	}
	else // Object
	{
		sprintf(buf, "%s%sreturnSize += %s->mValue.serialize(stream, %u);\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}

	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}

bool 
CPlusPlusCoder::printListForUnserialize(char* buf, const String& memberName, const String& iterName,
									   const TinyXmlNode* node, const String& indent, UInt32 mask)
{
	String valueType = getValueType(node);

	sprintf(buf, "%s%s//List<%s >\r\n", buf, indent.c_str(), valueType.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());
	String tmpIndent = indent + "    ";
	sprintf(buf, "%s%sUInt32 %sSize = 0;\r\n", buf, tmpIndent.c_str(), iterName.c_str());

	sprintf(buf, "%s%sif (!unserialize(%sSize, stream, %u)) return false;\r\n", buf, tmpIndent.c_str(), iterName.c_str(), mask);

	sprintf(buf, "%s%sfor (UInt32 %s = 0; %s < %sSize; ++%s)\r\n", buf, tmpIndent.c_str(), 
		iterName.c_str(), iterName.c_str(), iterName.c_str(), iterName.c_str()); 
	sprintf(buf, "%s%s{\r\n", buf, tmpIndent.c_str());

	String childIndent = tmpIndent + "    ";

	valueType = node->toElement()->getAttribute("type");

	if (isBasicType(valueType))
	{
		sprintf(buf, "%s%sif (!unserialize(%s->mValue, stream, %u)) return false;\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}
	else if (isContainerType(valueType))
	{
		String childMemberName = iterName + "->mValue";
		String childIterName = iterName + "i";

		if (valueType == "Vector")
		{
			if (!printVectorForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "List")
		{
			if (!printListForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Deque")
		{
			if (!printDequeForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Map")
		{
			if (!printMapForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Set")
		{
			if (!printSetForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
	}
	else // Object
	{
		sprintf(buf, "%s%sif (!%s->mValue.serialize(stream, %u)) return false;\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}

	sprintf(buf, "%s%s}\r\n", buf, tmpIndent.c_str());
	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}

bool 
CPlusPlusCoder::printDequeForSerializeSize(char* buf, const String& memberName, 
			                             const String& iterName, const TinyXmlNode* node, const String& indent)
{
	String valueType = getValueType(node);

	sprintf(buf, "%s%scontainerSize = %s.size();\r\n", buf, indent.c_str(), memberName.c_str());
	sprintf(buf, "%s%sreturnSize += serializeSize(containerSize);\r\n", buf, indent.c_str());

	sprintf(buf, "%s%sfor (Deque<%s >::Iter* %s = %s.begin(); %s != NULL; %s = %s.next(%s))\r\n", 
		buf, indent.c_str(), valueType.c_str(), iterName.c_str(), memberName.c_str(),
		iterName.c_str(), iterName.c_str(), memberName.c_str(), iterName.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

	String childIndent = indent + "    ";

	valueType = node->toElement()->getAttribute("type");

	if (isBasicType(valueType))
	{
		sprintf(buf, "%s%sreturnSize += serializeSize(%s->mValue);\r\n", buf, childIndent.c_str(), iterName.c_str());
	}
	else if (isContainerType(valueType))
	{
		String childMemberName = iterName + "->mValue";
		String childIterName = iterName + "i";

		if (valueType == "Vector")
		{
			if (!printVectorForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else if (valueType == "List")
		{
			if (!printListForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else if (valueType == "Deque")
		{
			if (!printDequeForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else if (valueType == "Map")
		{
			if (!printMapForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else if (valueType == "Set")
		{
			if (!printSetForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
	}
	else // Object
	{
		sprintf(buf, "%s%sreturnSize += %s->mValue.serializeSize();\r\n", buf, childIndent.c_str(), iterName.c_str());
	}
	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}

bool 
CPlusPlusCoder::printDequeForSerialize(char* buf, const String& memberName, const String& iterName,
									  const TinyXmlNode* node, const String& indent, UInt32 mask)
{
	String valueType = getValueType(node);

	sprintf(buf, "%s%scontainerSize = %s.size();\r\n", buf, indent.c_str(), memberName.c_str());
	sprintf(buf, "%s%sreturnSize += serialize(containerSize, stream, %u);\r\n", buf, indent.c_str(), mask);

	sprintf(buf, "%s%sfor (Deque<%s >::Iter* %s = %s.begin(); %s != NULL; %s = %s.next(%s))\r\n", 
		buf, indent.c_str(), valueType.c_str(), iterName.c_str(), memberName.c_str(),
		iterName.c_str(), iterName.c_str(), memberName.c_str(), iterName.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

	String childIndent = indent + "    ";

	valueType = node->toElement()->getAttribute("type");

	if (isBasicType(valueType))
	{
		sprintf(buf, "%s%sreturnSize += serialize(%s->mValue, stream, %u);\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}
	else if (isContainerType(valueType))
	{
		String childMemberName = iterName + "->mValue";
		String childIterName = iterName + "i";

		if (valueType == "Vector")
		{
			if (!printVectorForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "List")
		{
			if (!printListForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Deque")
		{
			if (!printDequeForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Map")
		{
			if (!printMapForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Set")
		{
			if (!printSetForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
	}
	else // Object
	{
		sprintf(buf, "%s%sreturnSize += %s->mValue.serialize(stream, %u);\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}
	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}

bool 
CPlusPlusCoder::printDequeForUnserialize(char* buf, const String& memberName, const String& iterName,
										const TinyXmlNode* node, const String& indent, UInt32 mask)
{
	String valueType = getValueType(node);

	sprintf(buf, "%s%s//Deque<%s >\r\n", buf, indent.c_str(), valueType.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());
	String tmpIndent = indent + "    ";
	sprintf(buf, "%s%sUInt32 %sSize = 0;\r\n", buf, tmpIndent.c_str(), iterName.c_str());

	sprintf(buf, "%s%sif (!unserialize(%sSize, stream, %u)) return false;\r\n", buf, tmpIndent.c_str(), iterName.c_str(), mask);

	sprintf(buf, "%s%sfor (UInt32 %s = 0; %s < %sSize; ++%s)\r\n", buf, tmpIndent.c_str(), 
		iterName.c_str(), iterName.c_str(), iterName.c_str(), iterName.c_str()); 
	sprintf(buf, "%s%s{\r\n", buf, tmpIndent.c_str());

	String childIndent = tmpIndent + "    ";

	valueType = node->toElement()->getAttribute("type");

	if (isBasicType(valueType))
	{
		sprintf(buf, "%s%sif (!unserialize(%s->mValue, stream, %u)) return false;\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}
	else if (isContainerType(valueType))
	{
		String childMemberName = iterName + "->mValue";
		String childIterName = iterName + "i";

		if (valueType == "Vector")
		{
			if (!printVectorForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "List")
		{
			if (!printListForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Deque")
		{
			if (!printDequeForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Map")
		{
			if (!printMapForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Set")
		{
			if (!printSetForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
	}
	else // Object
	{
		sprintf(buf, "%s%sif (!%s->mValue.serialize(stream, %u)) return false;\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}
	sprintf(buf, "%s%s}\r\n", buf, tmpIndent.c_str());
	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}

bool 
CPlusPlusCoder::printMapForSerializeSize(char* buf, const String& memberName, 
			                            const String& iterName, const TinyXmlNode* node, const String& indent)
{
	String keyType = node->getFirstChild("first")->toElement()->getAttribute("type");
	String valueType = getValueType(node->getFirstChild("second"));

	sprintf(buf, "%s%scontainerSize = %s.size();\r\n", buf, indent.c_str(), memberName.c_str());
	sprintf(buf, "%s%sreturnSize += serializeSize(containerSize);\r\n", buf, indent.c_str());

	sprintf(buf, "%s%sfor (Map<%s %s >::Iter* %s = %s.begin(); %s != NULL; %s = %s.next(%s))\r\n", 
		buf, indent.c_str(), keyType.c_str(), valueType.c_str(), iterName.c_str(), memberName.c_str(),
		iterName.c_str(), iterName.c_str(), memberName.c_str(), iterName.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

	String childIndent = indent + "    ";

	valueType = node->getFirstChild("second")->toElement()->getAttribute("type");

	if (isBasicType(keyType))
	{
		sprintf(buf, "%s%sreturnSize += serializeSize(%s->mKey);\r\n", buf, childIndent.c_str(), iterName.c_str());
	}
	else
	{
		return false;
	}

	if (isBasicType(valueType))
	{
		sprintf(buf, "%s%sreturnSize += serializeSize(%s->mValue);\r\n", buf, childIndent.c_str(), iterName.c_str());
	}
	else if (isContainerType(valueType))
	{
		String childMemberName = iterName + "->mValue";
		String childIterName = iterName + "i";

		if (valueType == "Vector")
		{
			if (!printVectorForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else if (valueType == "List")
		{
			if (!printListForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else if (valueType == "Deque")
		{
			if (!printDequeForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else if (valueType == "Map")
		{
			if (!printMapForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else if (valueType == "Set")
		{
			if (!printSetForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent))
			{
				return false;
			}
		}
	}
	else // Object
	{
		sprintf(buf, "%s%sreturnSize += %s->mValue.serializeSize();\r\n", buf, childIndent.c_str(), iterName.c_str());
	}
	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}

bool 
CPlusPlusCoder::printMapForSerialize(char* buf, const String& memberName, const String& iterName,
									const TinyXmlNode* node, const String& indent, UInt32 mask)
{
	String keyType = node->getFirstChild("first")->toElement()->getAttribute("type");
	String valueType = getValueType(node->getFirstChild("second"));

	sprintf(buf, "%s%scontainerSize = %s.size();\r\n", buf, indent.c_str(), memberName.c_str());
	sprintf(buf, "%s%sreturnSize += serialize(containerSize, stream, %u);\r\n", buf, indent.c_str(), mask);

	sprintf(buf, "%s%sfor (Map<%s %s >::Iter* %s = %s.begin(); %s != NULL; %s = %s.next(%s))\r\n", 
		buf, indent.c_str(), keyType.c_str(), valueType.c_str(), iterName.c_str(), memberName.c_str(),
		iterName.c_str(), iterName.c_str(), memberName.c_str(), iterName.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

	String childIndent = indent + "    ";

	valueType = node->getFirstChild("second")->toElement()->getAttribute("type");

	if (isBasicType(keyType))
	{
		sprintf(buf, "%s%sreturnSize += serialize(%s->mKey, stream, %u);\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}
	else
	{
		return false;
	}

	if (isBasicType(valueType))
	{
		sprintf(buf, "%s%sreturnSize += serialize(%s->mValue, stream, %u);\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}
	else if (isContainerType(valueType))
	{
		String childMemberName = iterName + "->mValue";
		String childIterName = iterName + "i";

		if (valueType == "Vector")
		{
			if (!printVectorForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "List")
		{
			if (!printListForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Deque")
		{
			if (!printDequeForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Map")
		{
			if (!printMapForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Set")
		{
			if (!printSetForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
	}
	else // Object
	{
		sprintf(buf, "%s%sreturnSize += %s->mValue.serialize(stream, %u);\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}
	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}

bool 
CPlusPlusCoder::printMapForUnserialize(char* buf, const String& memberName, const String& iterName,
									  const TinyXmlNode* node, const String& indent, UInt32 mask)
{
	String keyType = node->getFirstChild("first")->toElement()->getAttribute("type");
	String valueType = getValueType(node->getFirstChild("second"));

	sprintf(buf, "%s%s//Map<%s %s >\r\n", buf, indent.c_str(), keyType.c_str(), valueType.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());
	String tmpIndent = indent + "    ";
	sprintf(buf, "%s%sUInt32 %sSize = 0;\r\n", buf, tmpIndent.c_str(), iterName.c_str());

	sprintf(buf, "%s%sif (!unserialize(%sSize, stream, %u)) return false;\r\n", buf, tmpIndent.c_str(), iterName.c_str(), mask);

	sprintf(buf, "%s%sfor (UInt32 %s = 0; %s < %sSize; ++%s)\r\n", buf, tmpIndent.c_str(), 
		iterName.c_str(), iterName.c_str(), iterName.c_str(), iterName.c_str()); 
	sprintf(buf, "%s%s{\r\n", buf, tmpIndent.c_str());

	String childIndent = tmpIndent + "    ";

	valueType = node->getFirstChild("second")->toElement()->getAttribute("type");

	if (isBasicType(keyType))
	{
		sprintf(buf, "%s%sif (!serialize(%s->mKey, stream, %u)) return false;\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}
	else
	{
		return false;
	}

	if (isBasicType(valueType))
	{
		sprintf(buf, "%s%sif (!unserialize(%s->mValue, stream, %u)) return false;\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}
	else if (isContainerType(valueType))
	{
		String childMemberName = iterName + "->mValue";
		String childIterName = iterName + "i";

		if (valueType == "Vector")
		{
			if (!printVectorForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "List")
		{
			if (!printListForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Deque")
		{
			if (!printDequeForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Map")
		{
			if (!printMapForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Set")
		{
			if (!printSetForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
	}
	else // Object
	{
		sprintf(buf, "%s%sif (!%s->mValue.serialize(stream, %u)) return false;\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}
	sprintf(buf, "%s%s}\r\n", buf, tmpIndent.c_str());
	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}

bool 
CPlusPlusCoder::printSetForSerializeSize(char* buf, const String& memberName, 
			                            const String& iterName, const TinyXmlNode* node, const String& indent)
{
	String valueType = getValueType(node);

	sprintf(buf, "%s%scontainerSize = %s.size();\r\n", buf, indent.c_str(), memberName.c_str());
	sprintf(buf, "%s%sreturnSize += serializeSize(containerSize);\r\n", buf, indent.c_str());

	sprintf(buf, "%s%sfor (Set<%s >::Iter* %s = %s.begin(); %s != NULL; %s = %s.next(%s))\r\n", 
		buf, indent.c_str(), valueType.c_str(), iterName.c_str(), memberName.c_str(),
		iterName.c_str(), iterName.c_str(), memberName.c_str(), iterName.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

	String childIndent = indent + "    ";

	valueType = node->toElement()->getAttribute("type");

	if (isBasicType(valueType))
	{
		sprintf(buf, "%s%sreturnSize += serializeSize(%s->mValue);\r\n", buf, childIndent.c_str(), iterName.c_str());
	}
	else if (isContainerType(valueType))
	{
		String childMemberName = iterName + "->mValue";
		String childIterName = iterName + "i";

		if (valueType == "Vector")
		{
			if (!printVectorForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else if (valueType == "List")
		{
			if (!printListForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else if (valueType == "Deque")
		{
			if (!printDequeForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else if (valueType == "Map")
		{
			if (!printMapForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else if (valueType == "Set")
		{
			if (!printSetForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
	}
	else // Object
	{
		sprintf(buf, "%s%sreturnSize += %s->mValue.serializeSize();\r\n", buf, childIndent.c_str(), iterName.c_str());
	}
	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}

bool 
CPlusPlusCoder::printSetForSerialize(char* buf, const String& memberName, const String& iterName,
									const TinyXmlNode* node, const String& indent, UInt32 mask)
{
	String valueType = getValueType(node);

	sprintf(buf, "%s%scontainerSize = %s.size();\r\n", buf, indent.c_str(), memberName.c_str());
	sprintf(buf, "%s%sreturnSize += serialize(containerSize, stream, %u);\r\n", buf, indent.c_str(), mask);

	sprintf(buf, "%s%sfor (Set<%s >::Iter* %s = %s.begin(); %s != NULL; %s = %s.next(%s))\r\n", 
		buf, indent.c_str(), valueType.c_str(), iterName.c_str(), memberName.c_str(),
		iterName.c_str(), iterName.c_str(), memberName.c_str(), iterName.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

	String childIndent = indent + "    ";

	valueType = node->toElement()->getAttribute("type");

	if (isBasicType(valueType))
	{
		sprintf(buf, "%s%sreturnSize += serialize(%s->mValue, stream, %u);\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}
	else if (isContainerType(valueType))
	{
		String childMemberName = iterName + "->mValue";
		String childIterName = iterName + "i";

		if (valueType == "Vector")
		{
			if (!printVectorForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "List")
		{
			if (!printListForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Deque")
		{
			if (!printDequeForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Map")
		{
			if (!printMapForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Set")
		{
			if (!printSetForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
	}
	else // Object
	{
		sprintf(buf, "%s%sreturnSize += %s->mValue.serialize(stream, %u);\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}
	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}

bool 
CPlusPlusCoder::printSetForUnserialize(char* buf, const String& memberName, const String& iterName,
									  const TinyXmlNode* node, const String& indent, UInt32 mask)
{
	String valueType = getValueType(node);

	sprintf(buf, "%s%s//Set<%s >\r\n", buf, indent.c_str(), valueType.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());
	String tmpIndent = indent + "    ";
	sprintf(buf, "%s%sUInt32 %sSize = 0;\r\n", buf, tmpIndent.c_str(), iterName.c_str());

	sprintf(buf, "%s%sif (!unserialize(%sSize, stream, %u)) return false;\r\n", buf, tmpIndent.c_str(), iterName.c_str(), mask);

	sprintf(buf, "%s%sfor (UInt32 %s = 0; %s < %sSize; ++%s)\r\n", buf, tmpIndent.c_str(), 
		iterName.c_str(), iterName.c_str(), iterName.c_str(), iterName.c_str()); 
	sprintf(buf, "%s%s{\r\n", buf, tmpIndent.c_str());

	String childIndent = tmpIndent + "    ";

	valueType = node->toElement()->getAttribute("type");

	if (isBasicType(valueType))
	{
		sprintf(buf, "%s%sif (!serialize(%s->mValue, stream, %u)) return false;\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}
	else if (isContainerType(valueType))
	{
		String childMemberName = iterName + "->mValue";
		String childIterName = iterName + "i";

		if (valueType == "Vector")
		{
			if (!printVectorForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "List")
		{
			if (!printListForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Deque")
		{
			if (!printDequeForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Map")
		{
			if (!printMapForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else if (valueType == "Set")
		{
			if (!printSetForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
	}
	else // Object
	{
		sprintf(buf, "%s%sif (!%s->mValue.serialize(stream, %u)) return false;\r\n", buf, childIndent.c_str(), iterName.c_str(), mask);
	}
	sprintf(buf, "%s%s}\r\n", buf, tmpIndent.c_str());
	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}

#endif