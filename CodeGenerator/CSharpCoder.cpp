#include "CSharpCoder.h"

using namespace Lynx;


CSharpCoder::CSharpCoder()
{
    mBasicTypeSet.insert("bool");
    mBasicTypeSet.insert("SByte");
    mBasicTypeSet.insert("Byte");
    mBasicTypeSet.insert("Int16");
    mBasicTypeSet.insert("UInt16");
    mBasicTypeSet.insert("Int32");
    mBasicTypeSet.insert("UInt32");
    mBasicTypeSet.insert("Int64");
    mBasicTypeSet.insert("UInt64");
    mBasicTypeSet.insert("Single");
    mBasicTypeSet.insert("String");
    mBasicTypeSet.insert("Vector2");
    mBasicTypeSet.insert("Vector3");
    mBasicTypeSet.insert("Vector4");

    mContainerTypeSet.insert("Pair");
    mContainerTypeSet.insert("Dictionary");
    //sContainerTypeSet.insert("Set");
    mContainerTypeSet.insert("List");
    //sContainerTypeSet.insert("Deque");
    //sContainerTypeSet.insert("Vector");
}
void
CSharpCoder::Generate(char* buf, TinyXmlNode* node, const String& namespaceName, 
                      const String& className, const String& parentName, const String& csharpCodes,
                      UInt32 mask)
{
    printNamespace(buf, node, namespaceName, className, parentName, csharpCodes, mask);

}

void
CSharpCoder::GenerateTable(char* buf, TinyXmlNode* node, const String& namespaceName, 
                      const String& className, const String& parentName, const String& csharpCodes)
{
    printNamespaceTable(buf, node, namespaceName, className, parentName, csharpCodes);

}

bool 
CSharpCoder::isBasicType(const String& type)
{
	Set<String>::Iter* iter = mBasicTypeSet.find(type);
	if (iter)
	{
		return true;
	}
	return false;
}

bool 
CSharpCoder::isContainerType(const String& type)
{
	Set<String>::Iter* iter = mContainerTypeSet.find(type);
	if (iter)
	{
		return true;
	}
	return false;
}

String 
CSharpCoder::getValueType(const TinyXmlNode* node)
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
/*		    if (typeName == "Vector")
			{
				sprintf(buf, "%sList<", buf);
				i++;
			}
			else */if (typeName == "List")
			{
				sprintf(buf, "%sList<", buf);
				i++;
			}
			//else if (typeName == "Deque")
			//{
			//	sprintf(buf, "%sList<", buf);
			//	i++;
			//}
			//else if (typeName == "Set")
			//{
			//	sprintf(buf, "%sList<", buf);
			//	i++;
			//}
			else if (typeName == "Dictionary")
			{
				String keyType = node->getFirstChild()->getFirstChild("first")->toElement()->getAttribute("type");
				String valueType = getValueType(node->getFirstChild()->getFirstChild("second"));

				sprintf(buf, "%sDictionary<%s, %s >", buf, keyType.c_str(), valueType.c_str());
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
CSharpCoder::hasContainerType(const TinyXmlNode* node)
{
	const TinyXmlElement* elem = (TinyXmlElement*)node->getFirstChild();
	String memberType;

	while (elem)
	{
		memberType = elem->getAttribute("type");

		if (isContainerType(memberType))
		{
			return true;
		}
        elem = elem->nextSiblingElement();
	}

	return false;
}

bool 
CSharpCoder::printNamespace(char* buf, TinyXmlNode* node, const String& namespaceName, const String& className, const String& parentName, const String& csharpCodes, UInt32 mask)
{
    String indent, classIndent;
    if (namespaceName != "NULL")
    {
        sprintf(buf, "%s%snamespace %s\r\n", buf, indent.c_str(), namespaceName.c_str());
        sprintf(buf, "%s%s{\r\n", buf, indent.c_str());
        classIndent = indent + "    ";
    }

    
    printClass(buf, node, className, parentName, classIndent, csharpCodes, mask);

    if (namespaceName != "NULL")
    {
        sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
    }
    
    return true;
}

bool 
CSharpCoder::printNamespaceTable(char* buf, TinyXmlNode* node, const String& namespaceName, const String& className, const String& parentName, const String& csharpCodes)
{
    String indent, classIndent;
    if (namespaceName != "NULL")
    {
        sprintf(buf, "%s%snamespace %s\r\n", buf, indent.c_str(), namespaceName.c_str());
        sprintf(buf, "%s%s{\r\n", buf, indent.c_str());
        classIndent = indent + "    ";
    }

    
    printClassTable(buf, node, className, parentName, classIndent, csharpCodes);

    if (namespaceName != "NULL")
    {
        sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
    }
    
    return true;
}


bool 
CSharpCoder::printClass(char* buf, TinyXmlNode* node, const String& className, const String& parentName, const String& indent, const String& csharpCodes, UInt32 mask)
{
    if (parentName != "NULL")
    {
        sprintf(buf, "%s%spublic class %s : %s \r\n", buf, indent.c_str(), className.c_str(), parentName.c_str());
    }
    else
    {
        sprintf(buf, "%s%spublic class %s : NetData\r\n", buf, indent.c_str(), className.c_str());
    }
    sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

    String memberIndent = indent + "    ";
    printClassMember(buf, node, memberIndent);
	printClassCreateFunc(buf, className, "    ");
    printClassSerializeSizeFunc(buf, node, memberIndent);
    printClassSerializeFunc(buf, node, memberIndent);
    printClassUnserializeFunc(buf, node, memberIndent, true);

    if (!(className == "HeroExtendData" ||
        className == "PlayerProduceData"))
    {
        printClassCloneFunc(buf, node, className, memberIndent);
    }
    
	if (mask != 0)
	{
		sprintf(buf, "%s%spublic const UInt16 mMessageID = %u;\r\n", buf, memberIndent.c_str(), mask);
		sprintf(buf, "%s%spublic override UInt16 getMessageID(){ return mMessageID; }\r\n", buf, memberIndent.c_str());
	}
    sprintf(buf, "%s%s", buf, csharpCodes.c_str());
    sprintf(buf, "%s%s};\r\n", buf, indent.c_str());
    return true;
}

bool 
CSharpCoder::printClassTable(char* buf, TinyXmlNode* node, const String& className, const String& parentName, const String& indent, const String& csharpCodes)
{
    if (parentName != "NULL")
    {
        sprintf(buf, "%s%spublic class %s : %s \r\n", buf, indent.c_str(), className.c_str(), parentName.c_str());
    }
    else
    {
        sprintf(buf, "%s%spublic class %s \r\n", buf, indent.c_str(), className.c_str());
    }
    sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

    String memberIndent = indent + "    ";
    printClassMember(buf, node, memberIndent);
    printClassUnserializeFunc(buf, node, memberIndent, false);
    
    sprintf(buf, "%s%s", buf, csharpCodes.c_str());

    int pos = className.find("Table");
    if (pos != -1)
    {
        if (className.size() - pos == 5)
        {
            printClassGetSingletonFunc(buf, node, className, indent);
            printClassGetFunc(buf, node, className, indent);
            printClassClearFunc(buf, node, className, indent);
            printClassLoadFromDbcFunc(buf, node, className, indent);
        }
    }

    sprintf(buf, "%s%s};\r\n", buf, indent.c_str());
    return true;
}

void 
CSharpCoder::addMemberInitValueInfo(const String& typeName, const String& memberName, 
									const String& initValue)
{
	MemberInitValueInfo initValueInfo;
	initValueInfo.mTypeName = typeName;
	initValueInfo.mMemberName = memberName;
	initValueInfo.mInitValue = initValue;
	mMemberInitValueInfoList.insertTail(initValueInfo);
}

bool 
CSharpCoder::printClassMember(char* buf, const TinyXmlNode* node, const String& indent)
{
	mMemberInitValueInfoList.clear();

    if (!node->getFirstChild())
    {
        return true;
    }
	//const TinyXmlElement* elem = node->getFirstChild()->toElement();
	String memberType, memberName, memberValue, memberShip;
	
	node = node->getFirstChild();
	while (node)
	{
		String memberType2 = ((TinyXmlElement*)node)->getAttribute("type");
		memberType = getValueType(node);
		memberName = node->toElement()->getValue();
		memberShip = node->toElement()->getAttribute("membership");

		if (memberShip == "myself")
		{

			if (isBasicType(memberType))
			{
            	sprintf(buf, "%s%spublic %s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
            	memberValue = node->toElement()->getAttribute("value");
            	addMemberInitValueInfo(memberType, memberName, memberValue);
				//if (memberType == "Bool")
				//{
				//	sprintf(buf, "%s%spublic bool %s;\r\n", buf, indent.c_str(), /*memberType.c_str(),*/ memberName.c_str());
				//	memberValue = node->toElement()->getAttribute("value");
				//	addMemberInitValueInfo("Bool", memberName, memberValue);
				//}
				//else if (memberType == "String")
				//{
				//	sprintf(buf, "%s%spublic %s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
				//	memberValue = node->toElement()->getAttribute("value");
				//	addMemberInitValueInfo("String", memberName, memberValue);
				//}
				//else if (memberType == "Float")
				//{
				//	sprintf(buf, "%s%spublic Single %s;\r\n", buf, indent.c_str(), /*memberType.c_str(),*/ memberName.c_str());
				//	memberValue = node->toElement()->getAttribute("value");
				//	addMemberInitValueInfo("Float", memberName, memberValue);
				//}
				//else if (memberType == "Int8")
				//{
				//	sprintf(buf, "%s%spublic SByte %s;\r\n", buf, indent.c_str(), /*memberType.c_str(),*/ memberName.c_str());
				//	memberValue = node->toElement()->getAttribute("value");
				//	addMemberInitValueInfo("Int8", memberName, memberValue);
				//}
				//else if (memberType == "UInt8")
				//{
				//	sprintf(buf, "%s%spublic Byte %s;\r\n", buf, indent.c_str(), /*memberType.c_str(),*/ memberName.c_str());
				//	memberValue = node->toElement()->getAttribute("value");
				//	addMemberInitValueInfo("UInt8", memberName, memberValue);
				//}
				//else if (memberType == "Int16")
				//{
				//	sprintf(buf, "%s%spublic %s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
				//	memberValue = node->toElement()->getAttribute("value");
				//	addMemberInitValueInfo("Int16", memberName, memberValue);
				//}
				//else if (memberType == "UInt16")
				//{
				//	sprintf(buf, "%s%spublic %s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
				//	memberValue = node->toElement()->getAttribute("value");
				//	addMemberInitValueInfo("UInt16", memberName, memberValue);
				//}
				//else if (memberType == "Int32")
				//{
				//	sprintf(buf, "%s%spublic %s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
				//	memberValue = node->toElement()->getAttribute("value");
				//	addMemberInitValueInfo("Int32", memberName, memberValue);
				//}
				//else if (memberType == "UInt32")
				//{
				//	sprintf(buf, "%s%spublic %s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
				//	memberValue = node->toElement()->getAttribute("value");
				//	addMemberInitValueInfo("UInt32", memberName, memberValue);
				//}
				//else if (memberType == "Int64")
				//{
				//	sprintf(buf, "%s%spublic %s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
				//	memberValue = node->toElement()->getAttribute("value");
				//	addMemberInitValueInfo("Int64", memberName, memberValue);
				//}
				//else if (memberType == "UInt64")
				//{
				//	sprintf(buf, "%s%spublic %s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
				//	memberValue = node->toElement()->getAttribute("value");
				//	addMemberInitValueInfo("UInt64", memberName, memberValue);
				//}
				//else if (memberType == "Vector2")
				//{
				//	sprintf(buf, "%s%spublic %s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
				//}
				//else if (memberType == "Vector3")
				//{
				//	sprintf(buf, "%s%spublic %s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
				//}
				//else if (memberType == "Vector4")
				//{
				//	sprintf(buf, "%s%spublic %s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
				//}
				//else if (memberType == "Double")
				//{
				//	LOG_WARN("Invalid Type [Double].");
				//	return false;
				//}
			}
			else if (isContainerType(memberType2))
			{
				if (memberType2 == "Pair")
				{
					LOG_WARN("Invalid type [Pair]");
					return false;
				}
				//else if (memberType == "Vector")
				//{
				//	String valueType = elem->getFirstChild()->toElement()->getAttribute("type");
				//	sprintf(buf, "%s%sList<%s > %s;\r\n", buf, indent.c_str(), valueType.c_str(), memberName.c_str());
				//}
				else if (memberType2 == "List")
				{
					String valueType = getValueType(node->getFirstChild());//node->getFirstChild()->toElement()->getAttribute("type");
					sprintf(buf, "%s%spublic List<%s > %s;\r\n", buf, indent.c_str(), valueType.c_str(), memberName.c_str()); 
				}
				//else if (memberType == "Deque")
				//{
				//	String valueType = elem->getFirstChild()->toElement()->getAttribute("type");
				//	sprintf(buf, "%s%sList<%s > %s;\r\n", buf, indent.c_str(), valueType.c_str(), memberName.c_str()); 
				//}
				//else if(memberType == "Set")
				//{
				//	String valueType = elem->getFirstChild()->toElement()->getAttribute("type");
				//	sprintf(buf, "%s%sList<%s > %s;\r\n", buf, indent.c_str(), valueType.c_str(), memberName.c_str()); 
				//}
				else if (memberType2 == "Dictionary")
				{
					String keyType = node->getFirstChild()->getFirstChild("first")->toElement()->getAttribute("type");
					String valueType = getValueType(node->getFirstChild()->getFirstChild("second"));//node->getFirstChild()->getFirstChild("second")->toElement()->getAttribute("type");
					sprintf(buf, "%s%spublic Dictionary<%s, %s > %s;\r\n", buf, indent.c_str(), keyType.c_str(), valueType.c_str(), memberName.c_str());
				}
			}
			else // Object
			{
				sprintf(buf, "%s%spublic %s %s;\r\n", buf, indent.c_str(), memberType.c_str(), memberName.c_str());
			}
		}
        node = node->nextSiblingElement();
	}

	sprintf(buf, "%s\r\n", buf);
	
	return true;
}

bool 
CSharpCoder::printClassCreateFunc(char* buf, const String& className, const String& indent)
{
	sprintf(buf, "%s%spublic %s()\r\n", buf, indent.c_str(), className.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());
	String childIndent = indent + "    ";
	
	for (List<MemberInitValueInfo>::Iter* iter = mMemberInitValueInfoList.begin();
		iter != NULL; iter = mMemberInitValueInfoList.next(iter))
	{
		String type = iter->mValue.mTypeName;
		
		if (type == "bool")
		{
			sprintf(buf, "%s%s%s = %s;\r\n", buf, childIndent.c_str(), iter->mValue.mMemberName.c_str(), iter->mValue.mInitValue.c_str());
		}
		else if (type == "String")
		{
			if (!iter->mValue.mInitValue.empty())
			{
			    sprintf(buf, "%s%s%s = \"%s\";\r\n", buf, childIndent.c_str(), iter->mValue.mMemberName.c_str(), iter->mValue.mInitValue.c_str());
			}
		}
		else if (type == "Single")
		{
			sprintf(buf, "%s%s%s = %sf;\r\n", buf, childIndent.c_str(), iter->mValue.mMemberName.c_str(), iter->mValue.mInitValue.c_str());
		}
		else
		{
			if (iter->mValue.mInitValue != "0")
			{
			    sprintf(buf, "%s%s%s = %s;\r\n", buf, childIndent.c_str(), iter->mValue.mMemberName.c_str(), iter->mValue.mInitValue.c_str());
			}
		}
	}
	sprintf(buf, "%s%s}\r\n\r\n", buf, indent.c_str());
	return true;
}

bool 
CSharpCoder::printClassSerializeSizeFunc(char* buf, const TinyXmlNode* node, const String& indent)
{
	sprintf(buf, "%s%spublic override Int32 serializeSize()\r\n", buf, indent.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

	String childIndent = indent + "    ";

	sprintf(buf, "%s%sInt32 returnSize = 0;\r\n", buf, childIndent.c_str());
	if (hasContainerType(node))
	{
		sprintf(buf, "%s%sInt32 containerSize = 0;\r\n", buf, childIndent.c_str());
	}

	const TinyXmlElement* elem = (TinyXmlElement*)node->getFirstChild();
	String memberName, memberType;

	while (elem)
	{
		memberType = elem->getAttribute("type");
		memberName = elem->getValue();

		if (isBasicType(memberType))
		{
			sprintf(buf, "%s%sreturnSize += LynxSerialize.serializeSize(%s);\r\n", buf, childIndent.c_str(), memberName.c_str());
		}
		else if (isContainerType(memberType))
		{
			if (memberType == "Pair")
			{
				LOG_WARN("Invalid Type [Pair].");
				return false;
			}
			//else if (memberType == "Vector")
			//{
			//	if (!printVectorForSerializeSize(buf, memberName, "i", elem->getFirstChild(), childIndent))
			//	{
			//		return false;
			//	}
			//}
			else if (memberType == "List")
			{
				if (!printListForSerializeSize(buf, memberName, "i", elem->getFirstChild(), childIndent))
				{
					return false;
				}
			}
			//else if (memberType == "Deque")
			//{
			//	if (!printDequeForSerializeSize(buf, memberName, "i", elem->getFirstChild(), childIndent))
			//	{
			//		return false;
			//	}
			//}
			else if (memberType == "Dictionary")
			{
				if (!printMapForSerializeSize(buf, memberName, "i", elem->getFirstChild(), childIndent))
				{
					return false;
				}
			}
			//else if (memberType == "Set")
			//{
			//	if (!printSetForSerializeSize(buf, memberName, "i", elem->getFirstChild(), childIndent))
			//	{
			//		return false;
			//	}
			//}
		}
		else // Object
		{
			sprintf(buf, "%s%sreturnSize += %s.serializeSize();\r\n", buf, childIndent.c_str(), memberName.c_str());
		}
        elem = elem->nextSiblingElement();
	}

	sprintf(buf, "%s%sreturn returnSize;\r\n", buf, childIndent.c_str());
	sprintf(buf, "%s%s}\r\n\r\n", buf, indent.c_str());
	return true;
}

bool
CSharpCoder::printClassSerializeFunc(char* buf, const TinyXmlNode* node, const String& indent)
{
	sprintf(buf, "%s%spublic override Int32 serialize(BinaryWriter stream, UInt32 mask)\r\n", buf, indent.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

	String childIndent = indent + "    ";

	sprintf(buf, "%s%sInt32 returnSize = 0;\r\n", buf, childIndent.c_str());
	if (hasContainerType(node))
	{
		sprintf(buf, "%s%sInt32 containerSize = 0;\r\n", buf, childIndent.c_str());
	}

	const TinyXmlElement* elem = (TinyXmlElement*)node->getFirstChild();
	String memberName, memberType;

	while (elem)
	{
		memberType = elem->getAttribute("type");
		memberName = elem->getValue();

		if (isBasicType(memberType))
		{
			sprintf(buf, "%s%sreturnSize += LynxSerialize.serialize(%s, stream, mask);\r\n", buf, childIndent.c_str(), memberName.c_str());
		}
		else if (isContainerType(memberType))
		{
			if (memberType == "Pair")
			{
				LOG_WARN("Invalid Type [Pair].");
				return false;
			}
			//else if (memberType == "Vector")
			//{
			//	if (!printVectorForSerialize(buf, memberName, "i", elem->getFirstChild(), childIndent, mask))
			//	{
			//		return false;
			//	}
			//}
			else if (memberType == "List")
			{
				if (!printListForSerialize(buf, memberName, "i", elem->getFirstChild(), childIndent))
				{
					return false;
				}
			}
			//else if (memberType == "Deque")
			//{
			//	if (!printDequeForSerialize(buf, memberName, "i", elem->getFirstChild(), childIndent, mask))
			//	{
			//		return false;
			//	}
			//}
			else if (memberType == "Dictionary")
			{
				if (!printMapForSerialize(buf, memberName, "i", elem->getFirstChild(), childIndent))
				{
					return false;
				}
			}
			//else if (memberType == "Set")
			//{
			//	if (!printSetForSerialize(buf, memberName, "i", elem->getFirstChild(), childIndent, mask))
			//	{
			//		return false;
			//	}
			//}
		}
		else // Object
		{
			sprintf(buf, "%s%sreturnSize += %s.serialize(stream, mask);\r\n", buf, childIndent.c_str(), memberName.c_str());
		}
        elem = elem->nextSiblingElement();
	}

	sprintf(buf, "%s%sreturn returnSize;\r\n", buf, childIndent.c_str());
	sprintf(buf, "%s%s}\r\n\r\n", buf, indent.c_str());
	return true;
}

bool 
CSharpCoder::printClassUnserializeFunc(char* buf, const TinyXmlNode* node, const String& indent, bool isOverride)
{
    if (isOverride)
    {
        sprintf(buf, "%s%spublic override Int32 unserialize(BinaryReader stream, UInt32 mask)\r\n", buf, indent.c_str());
    }
    else
    {
        sprintf(buf, "%s%spublic Int32 unserialize(BinaryReader stream, UInt32 mask)\r\n", buf, indent.c_str());
    }
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

	String childIndent = indent + "    ";

	const TinyXmlElement* elem = (TinyXmlElement*)node->getFirstChild();
	String memberName, memberType;

	while (elem)
	{
		memberType = elem->getAttribute("type");
		memberName = elem->getValue();

		if (isBasicType(memberType))
		{
			sprintf(buf, "%s%sif (LynxSerialize.unserialize(ref %s, stream, mask) == 0) return 0;\r\n", buf, childIndent.c_str(), memberName.c_str());
		}
		else if (isContainerType(memberType))
		{
			if (memberType == "Pair")
			{
				LOG_WARN("Invalid Type [Pair].");
				return false;
			}
			//else if (memberType == "Vector")
			//{
			//	if (!printVectorForUnserialize(buf, memberName, "i", elem->getFirstChild(), childIndent, mask))
			//	{
			//		return false;
			//	}
			//}
			else if (memberType == "List")
			{
				if (!printListForUnserialize(buf, memberName, "i", elem->getFirstChild(), childIndent))
				{
					return false;
				}
			}
			//else if (memberType == "Deque")
			//{
			//	if (!printDequeForUnserialize(buf, memberName, "i", elem->getFirstChild(), childIndent, mask))
			//	{
			//		return false;
			//	}
			//}
			else if (memberType == "Dictionary")
			{
				if (!printMapForUnserialize(buf, memberName, "i", elem->getFirstChild(), childIndent))
				{
					return false;
				}
			}
			//else if (memberType == "Set")
			//{
			//	if (!printSetForUnserialize(buf, memberName, "i", elem->getFirstChild(), childIndent, mask))
			//	{
			//		return false;
			//	}
			//}
		}
		else // Object
		{
            sprintf(buf, "%s%s%s = new %s();\r\n", buf, childIndent.c_str(), memberName.c_str(), memberType.c_str());
			sprintf(buf, "%s%sif (%s.unserialize(stream, mask) == 0) return 0;\r\n", buf, childIndent.c_str(), memberName.c_str());
		}
        elem = elem->nextSiblingElement();
	}

	sprintf(buf, "%s%sreturn 1;\r\n", buf, childIndent.c_str());
	sprintf(buf, "%s%s}\r\n\r\n", buf, indent.c_str());
	return true;
}

bool
CSharpCoder::printClassCloneFunc(char* buf, const TinyXmlNode* node, const String& className, const String& indent)
{
    sprintf(buf, "%s%spublic override NetData clone()\r\n", buf, indent.c_str());
    sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

    String childIndent = indent + "    ";

    String newClassName = "new" + className;

    sprintf(buf, "%s%s%s %s = new %s();\r\n", buf, childIndent.c_str(), className.c_str(), newClassName.c_str(), className.c_str());

    const TinyXmlElement* elem = (TinyXmlElement*)node->getFirstChild();
    String memberName, memberType;

    while (elem)
    {
        memberType = elem->getAttribute("type");
        memberName = elem->getValue();

        if (isBasicType(memberType))
        {
            sprintf(buf, "%s%s%s.%s = %s;\r\n", buf, childIndent.c_str(), newClassName.c_str(), memberName.c_str(), memberName.c_str());
        }
        else if (isContainerType(memberType))
        {
            String valueTypeDecl = getValueType(elem);
            String newObjectName = newClassName + "." + memberName;
            sprintf(buf, "%s%s%s.%s = new %s();\r\n", buf, childIndent.c_str(), newClassName.c_str(), memberName.c_str(), valueTypeDecl.c_str());
            if (memberType == "List")
            {
                if (!printListForClone(buf, newObjectName, memberName, "i", elem->getFirstChild(), childIndent))
                {
                    return false;
                }
            }
            else if (memberType == "Dictionary")
            {
                if (!printMapForClone(buf, newObjectName, memberName, "i", elem->getFirstChild(), childIndent))
                {
                    return false;
                }
            }
        }
        else // Object
        {
            sprintf(buf, "%s%s%s.%s = %s.clone() as %s;\r\n", buf, childIndent.c_str(), newClassName.c_str(), memberName.c_str(), memberName.c_str(), memberType.c_str());
        }
        elem = elem->nextSiblingElement();
    }

    sprintf(buf, "%s%sreturn %s;\r\n", buf, childIndent.c_str(), newClassName.c_str());
    sprintf(buf, "%s%s}\r\n\r\n", buf, indent.c_str());
    return true;
}

bool
CSharpCoder::printListForClone(char* buf, const String& objectName, const String& memberName, const String& iterName, const TinyXmlNode* node, const String& indent)
{
    String valueTypeDecl = getValueType(node);

    sprintf(buf, "%s%sforeach (%s %s in %s)\r\n", buf, indent.c_str(), valueTypeDecl.c_str(), iterName.c_str(), memberName.c_str());
    sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

    String childIndent = indent + "    ";

    String valueType = node->toElement()->getAttribute("type");

    if (isBasicType(valueType))
    {
        sprintf(buf, "%s%s%s.Add(%s);\r\n", buf, indent.c_str(), objectName.c_str(), iterName.c_str());
    }
    else if (isContainerType(valueType))
    {
        String childMemberName = iterName;
        String childIterName = iterName + "i";

        String newObjectName = "obj_" + memberName;

        sprintf(buf, "%s%s%s %s = new %s();\r\n", buf, childIndent.c_str(), valueTypeDecl.c_str(), newObjectName.c_str(), valueTypeDecl.c_str());
        sprintf(buf, "%s%s%s.Add(%s);\r\n", buf, childIndent.c_str(), objectName.c_str(), newObjectName.c_str());

        if (valueType == "List")
        {
            if (!printListForClone(buf, newObjectName, childMemberName, childIterName, 
                node->getFirstChild(), childIndent))
            {
                return false;
            }
        }
        else if (valueType == "Dictionary")
        {
            if (!printMapForClone(buf, newObjectName, childMemberName, childIterName, 
                node->getFirstChild(), childIndent))
            {
                return false;
            }
        }
    }
    else // object
    {
        sprintf(buf, "%s%s%s.Add(%s.clone() as %s);\r\n", buf, childIndent.c_str(), objectName.c_str(), iterName.c_str(), valueType.c_str());
    }
    sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
    return true;
}

bool 
CSharpCoder::printListForSerializeSize(char* buf, const String& memberName, 
			                             const String& iterName, const TinyXmlNode* node, const String& indent)
{
	String valueType = getValueType(node);

	sprintf(buf, "%s%scontainerSize = %s.Count;\r\n", buf, indent.c_str(), memberName.c_str());
	sprintf(buf, "%s%sreturnSize += LynxSerialize.serializeSize(containerSize);\r\n", buf, indent.c_str());

	//sprintf(buf, "%s%sfor (List<%s >::Iter* %s = %s.begin(); %s != NULL; %s = %s.next(%s))\r\n", 
	//	buf, indent.c_str(), valueType.c_str(), iterName.c_str(), memberName.c_str(),
	//	iterName.c_str(), iterName.c_str(), memberName.c_str(), iterName.c_str());
    sprintf(buf, "%s%sforeach (%s %s in %s)\r\n", buf, indent.c_str(), valueType.c_str(), iterName.c_str(), memberName.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

	String childIndent = indent + "    ";

	valueType = node->toElement()->getAttribute("type");

	if (isBasicType(valueType))
	{
		sprintf(buf, "%s%sreturnSize += LynxSerialize.serializeSize(%s);\r\n", buf, childIndent.c_str(), iterName.c_str());
	}
	else if (isContainerType(valueType))
	{
		String childMemberName = iterName;
		String childIterName = iterName + "i";

/*		if (valueType == "Vector")
		{
			if (!printVectorForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else*/ if (valueType == "List")
		{
			if (!printListForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		//else if (valueType == "Deque")
		//{
		//	if (!printDequeForSerializeSize(buf, childMemberName, childIterName, 
		//		node->getFirstChild(), childIndent))
		//	{
		//		return false;
		//	}
		//}
		else if (valueType == "Dictionary")
		{
			if (!printMapForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		//else if (valueType == "Set")
		//{
		//	if (!printSetForSerializeSize(buf, childMemberName, childIterName, 
		//		node->getFirstChild(), childIndent))
		//	{
		//		return false;
		//	}
		//}
	}
	else // Object
	{
		sprintf(buf, "%s%sreturnSize += %s.serializeSize();\r\n", buf, childIndent.c_str(), iterName.c_str());
	}

	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}

bool 
CSharpCoder::printListForSerialize(char* buf, const String& memberName, const String& iterName,
									 const TinyXmlNode* node, const String& indent)
{
	String valueType = getValueType(node);

	sprintf(buf, "%s%scontainerSize = %s.Count;\r\n", buf, indent.c_str(), memberName.c_str());
	sprintf(buf, "%s%sreturnSize += LynxSerialize.serialize(containerSize, stream, mask);\r\n", buf, indent.c_str());

	//sprintf(buf, "%s%sfor (List<%s >::Iter* %s = %s.begin(); %s != NULL; %s = %s.next(%s))\r\n", 
	//	buf, indent.c_str(), valueType.c_str(), iterName.c_str(), memberName.c_str(),
	//	iterName.c_str(), iterName.c_str(), memberName.c_str(), iterName.c_str());
    sprintf(buf, "%s%sforeach (%s %s in %s)\r\n", buf, indent.c_str(), valueType.c_str(), iterName.c_str(), memberName.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

	String childIndent = indent + "    ";

	valueType = node->toElement()->getAttribute("type");

	if (isBasicType(valueType))
	{
		sprintf(buf, "%s%sreturnSize += LynxSerialize.serialize(%s, stream, mask);\r\n", buf, childIndent.c_str(), iterName.c_str());
	}
	else if (isContainerType(valueType))
	{
		String childMemberName = iterName;
		String childIterName = iterName + "i";

/*		if (valueType == "Vector")
		{
			if (!printVectorForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent, mask))
			{
				return false;
			}
		}
		else*/ if (valueType == "List")
		{
			if (!printListForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		//else if (valueType == "Deque")
		//{
		//	if (!printDequeForSerialize(buf, childMemberName, childIterName, 
		//		node->getFirstChild(), childIndent, mask))
		//	{
		//		return false;
		//	}
		//}
		else if (valueType == "Dictionary")
		{
			if (!printMapForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		//else if (valueType == "Set")
		//{
		//	if (!printSetForSerialize(buf, childMemberName, childIterName, 
		//		node->getFirstChild(), childIndent, mask))
		//	{
		//		return false;
		//	}
		//}
	}
	else // Object
	{
		sprintf(buf, "%s%sreturnSize += %s.serialize(stream, mask);\r\n", buf, childIndent.c_str(), iterName.c_str());
	}

	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}

bool 
CSharpCoder::printListForUnserialize(char* buf, const String& memberName, const String& iterName,
									   const TinyXmlNode* node, const String& indent)
{
	String valueTypeDecl = getValueType(node);

	sprintf(buf, "%s%s//List<%s >\r\n", buf, indent.c_str(), valueTypeDecl.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());
	String tmpIndent = indent + "    ";
	sprintf(buf, "%s%sUInt32 %sSize = 0;\r\n", buf, tmpIndent.c_str(), iterName.c_str());

	sprintf(buf, "%s%sif (LynxSerialize.unserialize(ref %sSize, stream, mask) == 0) return 0;\r\n", buf, tmpIndent.c_str(), iterName.c_str());
    
    sprintf(buf, "%s%s%s = new List<%s >();\r\n", buf, tmpIndent.c_str(), memberName.c_str(), valueTypeDecl.c_str());
	sprintf(buf, "%s%sfor (UInt32 %s = 0; %s < %sSize; ++%s)\r\n", buf, tmpIndent.c_str(), 
		iterName.c_str(), iterName.c_str(), iterName.c_str(), iterName.c_str()); 
	sprintf(buf, "%s%s{\r\n", buf, tmpIndent.c_str());

	String childIndent = tmpIndent + "    ";

	String valueType = node->toElement()->getAttribute("type");

	if (isBasicType(valueType))
	{
        if (valueType == "String")
        {
            sprintf(buf, "%s%s%s val = \"\";\r\n", buf, childIndent.c_str(), valueType.c_str());
        }
        else
        {
            sprintf(buf, "%s%s%s val = 0;\r\n", buf, childIndent.c_str(), valueType.c_str());
        }
		sprintf(buf, "%s%sif (LynxSerialize.unserialize(ref val, stream, mask) == 0) return 0;\r\n", buf, childIndent.c_str());
        sprintf(buf, "%s%s%s.Add(val);\r\n", buf, childIndent.c_str(), memberName.c_str());
	}
	else if (isContainerType(valueType))
	{
        String childMemberName = "i" + memberName;
        String childIterName = iterName + "i";

        sprintf(buf, "%s%s%s %s;\r\n", buf, childIndent.c_str(), valueTypeDecl.c_str(), childMemberName.c_str());        

        if (valueType == "List")
		{
			if (!printListForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}

		else if (valueType == "Dictionary")
		{
			if (!printMapForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild(), childIndent))
			{
				return false;
			}
		}

        sprintf(buf, "%s%s%s.Add(%s);\r\n", buf, childIndent.c_str(), memberName.c_str(), childMemberName.c_str());
	}
	else // Object
	{
        sprintf(buf, "%s%s%s %sValue = new %s();\r\n", buf, childIndent.c_str(), valueType.c_str(), iterName.c_str(), valueType.c_str());
		sprintf(buf, "%s%sif (%sValue.unserialize(stream, mask) == 0) return 0;\r\n", buf, childIndent.c_str(), iterName.c_str());
        sprintf(buf, "%s%s%s.Add(%sValue);\r\n", buf, childIndent.c_str(), memberName.c_str(), iterName.c_str());
	}

	sprintf(buf, "%s%s}\r\n", buf, tmpIndent.c_str());
	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}


bool 
CSharpCoder::printMapForSerializeSize(char* buf, const String& memberName, 
			                            const String& iterName, const TinyXmlNode* node, const String& indent)
{
	String keyType = node->getFirstChild("first")->toElement()->getAttribute("type");
	String valueType = getValueType(node->getFirstChild("second"));

	sprintf(buf, "%s%scontainerSize = %s.Count;\r\n", buf, indent.c_str(), memberName.c_str());
	sprintf(buf, "%s%sreturnSize += LynxSerialize.serializeSize(containerSize);\r\n", buf, indent.c_str());

	//sprintf(buf, "%s%sfor (Map<%s %s >::Iter* %s = %s.begin(); %s != NULL; %s = %s.next(%s))\r\n", 
	//	buf, indent.c_str(), keyType.c_str(), valueType.c_str(), iterName.c_str(), memberName.c_str(),
	//	iterName.c_str(), iterName.c_str(), memberName.c_str(), iterName.c_str());
    sprintf(buf, "%s%sforeach (KeyValuePair<%s, %s> %s in %s)\r\n", buf, indent.c_str(), keyType.c_str(), valueType.c_str(), iterName.c_str(), memberName.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

	String childIndent = indent + "    ";

	valueType = node->getFirstChild("second")->toElement()->getAttribute("type");

	if (isBasicType(keyType))
	{
		sprintf(buf, "%s%sreturnSize += LynxSerialize.serializeSize(%s.Key);\r\n", buf, childIndent.c_str(), iterName.c_str());
	}
	else
	{
		return false;
	}

	if (isBasicType(valueType))
	{
		sprintf(buf, "%s%sreturnSize += LynxSerialize.serializeSize(%s.Value);\r\n", buf, childIndent.c_str(), iterName.c_str());
	}
	else if (isContainerType(valueType))
	{
		String childMemberName = iterName + ".Value";
		String childIterName = iterName + "i";

/*		if (valueType == "Vector")
		{
			if (!printVectorForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		else*/ if (valueType == "List")
		{
			if (!printListForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		//else if (valueType == "Deque")
		//{
		//	if (!printDequeForSerializeSize(buf, childMemberName, childIterName, 
		//		node->getFirstChild("second")->getFirstChild(), childIndent))
		//	{
		//		return false;
		//	}
		//}
		else if (valueType == "Dictionary")
		{
			if (!printMapForSerializeSize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent))
			{
				return false;
			}
		}
		//else if (valueType == "Set")
		//{
		//	if (!printSetForSerializeSize(buf, childMemberName, childIterName, 
		//		node->getFirstChild("second")->getFirstChild(), childIndent))
		//	{
		//		return false;
		//	}
		//}
	}
	else // Object
	{
		sprintf(buf, "%s%sreturnSize += %s.Value.serializeSize();\r\n", buf, childIndent.c_str(), iterName.c_str());
	}
	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}

bool 
CSharpCoder::printMapForSerialize(char* buf, const String& memberName, const String& iterName,
									const TinyXmlNode* node, const String& indent)
{
	String keyType = node->getFirstChild("first")->toElement()->getAttribute("type");
	String valueType = getValueType(node->getFirstChild("second"));

	sprintf(buf, "%s%scontainerSize = %s.Count;\r\n", buf, indent.c_str(), memberName.c_str());
	sprintf(buf, "%s%sreturnSize += LynxSerialize.serialize(containerSize, stream, mask);\r\n", buf, indent.c_str());

	//sprintf(buf, "%s%sfor (Map<%s %s >::Iter* %s = %s.begin(); %s != NULL; %s = %s.next(%s))\r\n", 
	//	buf, indent.c_str(), keyType.c_str(), valueType.c_str(), iterName.c_str(), memberName.c_str(),
	//	iterName.c_str(), iterName.c_str(), memberName.c_str(), iterName.c_str());
    sprintf(buf, "%s%sforeach (KeyValuePair<%s, %s> %s in %s)\r\n", buf, indent.c_str(), keyType.c_str(), valueType.c_str(), iterName.c_str(), memberName.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

	String childIndent = indent + "    ";

	valueType = node->getFirstChild("second")->toElement()->getAttribute("type");

	if (isBasicType(keyType))
	{
		sprintf(buf, "%s%sreturnSize += LynxSerialize.serialize(%s.Key, stream, mask);\r\n", buf, childIndent.c_str(), iterName.c_str());
	}
	else
	{
		return false;
	}

	if (isBasicType(valueType))
	{
		sprintf(buf, "%s%sreturnSize += LynxSerialize.serialize(%s.Value, stream, mask);\r\n", buf, childIndent.c_str(), iterName.c_str());
	}
	else if (isContainerType(valueType))
	{
		String childMemberName = iterName + ".Value";
		String childIterName = iterName + "i";

     if (valueType == "List")
		{
			if (!printListForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent))
			{
				return false;
			}
		}

		else if (valueType == "Dictionary")
		{
			if (!printMapForSerialize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent))
			{
				return false;
			}
		}

	}
	else // Object
	{
		sprintf(buf, "%s%sreturnSize += %s.Value.serialize(stream, mask);\r\n", buf, childIndent.c_str(), iterName.c_str());
	}
	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}

bool 
CSharpCoder::printMapForUnserialize(char* buf, const String& memberName, const String& iterName,
									  const TinyXmlNode* node, const String& indent)
{
	String keyType = node->getFirstChild("first")->toElement()->getAttribute("type");
	String valueTypeDecl = getValueType(node->getFirstChild("second"));

	sprintf(buf, "%s%s//Map<%s %s >\r\n", buf, indent.c_str(), keyType.c_str(), valueTypeDecl.c_str());
	sprintf(buf, "%s%s{\r\n", buf, indent.c_str());
	String tmpIndent = indent + "    ";
	sprintf(buf, "%s%sUInt32 %sSize = 0;\r\n", buf, tmpIndent.c_str(), iterName.c_str());

	sprintf(buf, "%s%sif (LynxSerialize.unserialize(ref %sSize, stream, mask) == 0) return 0;\r\n", buf, tmpIndent.c_str(), iterName.c_str());

    sprintf(buf, "%s%s%s = new Dictionary<%s, %s >();\r\n", buf, tmpIndent.c_str(), memberName.c_str(), keyType.c_str(), valueTypeDecl.c_str());
	sprintf(buf, "%s%sfor (UInt32 %s = 0; %s < %sSize; ++%s)\r\n", buf, tmpIndent.c_str(), 
		iterName.c_str(), iterName.c_str(), iterName.c_str(), iterName.c_str()); 
	sprintf(buf, "%s%s{\r\n", buf, tmpIndent.c_str());

	String childIndent = tmpIndent + "    ";

	String valueType = node->getFirstChild("second")->toElement()->getAttribute("type");

	if (isBasicType(keyType))
	{
        if (keyType == "String")
        {
            sprintf(buf, "%s%s%s %sKey = \"\";\r\n", buf, childIndent.c_str(), keyType.c_str(), iterName.c_str());
        }
        else
        {
            sprintf(buf, "%s%s%s %sKey = 0;\r\n", buf, childIndent.c_str(), keyType.c_str(), iterName.c_str());
        }
		sprintf(buf, "%s%sif (LynxSerialize.unserialize(ref %sKey, stream, mask) == 0) return 0;\r\n", buf, childIndent.c_str(), iterName.c_str());
	}
	else
	{
		return false;
	}

	if (isBasicType(valueType))
	{
        if (valueType == "String")
        {
            sprintf(buf, "%s%s%s %sValue = \"\";\r\n", buf, childIndent.c_str(), valueType.c_str(), iterName.c_str());
        }
        else
        {
            sprintf(buf, "%s%s%s %sValue = 0;\r\n", buf, childIndent.c_str(), valueType.c_str(), iterName.c_str());
        }
		sprintf(buf, "%s%sif (LynxSerialize.unserialize(ref %sValue, stream, mask) == 0) return 0;\r\n", buf, childIndent.c_str(), iterName.c_str());
        sprintf(buf, "%s%s%s.Add(%sKey, %sValue);\r\n", buf, childIndent.c_str(), memberName.c_str(), iterName.c_str(), iterName.c_str());
	}
	else if (isContainerType(valueType))
	{
        String childMemberName = "i" + memberName;
        String childIterName = iterName + "i";

        sprintf(buf, "%s%s%s %s;\r\n", buf, childIndent.c_str(), valueTypeDecl.c_str(), childMemberName.c_str());
        

        if (valueType == "List")
		{
			if (!printListForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent))
			{
				return false;
			}
		}

		else if (valueType == "Dictionary")
		{
			if (!printMapForUnserialize(buf, childMemberName, childIterName, 
				node->getFirstChild("second")->getFirstChild(), childIndent))
			{
				return false;
			}
		}
        sprintf(buf, "%s%s%s.Add(%sKey, %s);\r\n", buf, childIndent.c_str(), memberName.c_str(), iterName.c_str(), childMemberName.c_str());
	}
	else // Object
	{
        sprintf(buf, "%s%s%s %sValue = new %s();\r\n", buf, childIndent.c_str(), valueType.c_str(), iterName.c_str(), valueType.c_str());
		sprintf(buf, "%s%sif (%sValue.unserialize(stream, mask) == 0) return 0;\r\n", buf, childIndent.c_str(), iterName.c_str());
        sprintf(buf, "%s%s%s.Add(%sKey, %sValue);\r\n", buf, childIndent.c_str(), memberName.c_str(), iterName.c_str(), iterName.c_str());
	}

	sprintf(buf, "%s%s}\r\n", buf, tmpIndent.c_str());
	sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
	return true;
}


bool 
CSharpCoder::printMapForClone(char* buf, const String& objectName, const String& memberName, const String& iterName, const TinyXmlNode* node, const String& indent)
{
    String keyType = node->getFirstChild("first")->toElement()->getAttribute("type");
    String valueTypeDecl = getValueType(node->getFirstChild("second"));

    //sprintf(buf, "%s%s%s.%s = new Dictionary<%s, %s >();\r\n", buf, indent.c_str(), objectName.c_str(), memberName.c_str(), keyType.c_str(), valueType.c_str());
    sprintf(buf, "%s%sforeach (KeyValuePair<%s, %s> %s in %s)\r\n", buf, indent.c_str(), keyType.c_str(), valueTypeDecl.c_str(), iterName.c_str(), memberName.c_str());
    sprintf(buf, "%s%s{\r\n", buf, indent.c_str());

    String childIndent = indent + "    ";

    String valueType = node->getFirstChild("second")->toElement()->getAttribute("type");;

    if (!isBasicType(keyType))
    {
        return false;
    }


    if (isBasicType(valueType))
    {
        String keyIterName = iterName + ".Key";
        String valueIterName = iterName + ".Value";
        sprintf(buf, "%s%s%s.Add(%s, %s);\r\n", buf, indent.c_str(), memberName.c_str(), keyIterName.c_str(), valueIterName.c_str());
    }
    else if (isContainerType(valueType))
    {
        String childMemberName = iterName + ".Value";
        String childIterName = iterName + "i";

        String newObjectName = "obj_" + memberName;
        String keyIterName = iterName + ".Key";

        sprintf(buf, "%s%s%s %s = new %s();\r\n", buf, childIndent.c_str(), valueTypeDecl.c_str(), newObjectName.c_str(), valueTypeDecl.c_str());
        sprintf(buf, "%s%s%s.Add(%s, %s);\r\n", buf, childIndent.c_str(), objectName.c_str(), keyIterName.c_str(), newObjectName.c_str());

        if (valueType == "List")
        {
            if (!printListForClone(buf, newObjectName, childMemberName, childIterName, 
                node->getFirstChild("second")->getFirstChild(), childIndent))
            {
                return false;
            }
        }
        else if (valueType == "Dictionary")
        {
            if (!printMapForClone(buf, newObjectName, childMemberName, childIterName, 
                node->getFirstChild("second")->getFirstChild(), childIndent))
            {
                return false;
            }
        }
    }
    else // object
    {
        String keyIterName = iterName + ".Key";
        String valueIterName = iterName + ".Value";
        sprintf(buf, "%s%s%s.Add(%s, %s.clone() as %s);\r\n", buf, childIndent.c_str(), objectName.c_str(), keyIterName.c_str(), valueIterName.c_str(), valueType.c_str());
    }

    sprintf(buf, "%s%s}\r\n", buf, indent.c_str());
    return true;
}

bool
CSharpCoder::printClassGetSingletonFunc(char* buf, const TinyXmlNode* node, const String& className, const String& indent)
{
    String childIndent = indent + "    ";
    sprintf(buf, "%s%sprivate static %s sInstance = null;\r\n\r\n", buf, childIndent.c_str(), className.c_str());
    sprintf(buf, "%s%spublic static %s GetSingleton()\r\n", buf, childIndent.c_str(), className.c_str());
    sprintf(buf, "%s%s{\r\n", buf, childIndent.c_str());
    sprintf(buf, "%s%s    if (sInstance == null)\r\n", buf, childIndent.c_str());
    sprintf(buf, "%s%s    {\r\n", buf, childIndent.c_str());
    sprintf(buf, "%s%s         sInstance = new %s();\r\n", buf, childIndent.c_str(), className.c_str());
    sprintf(buf, "%s%s    }\r\n", buf, childIndent.c_str());
    sprintf(buf, "%s%s    return sInstance;\r\n", buf, childIndent.c_str());
    sprintf(buf, "%s%s}\r\n\r\n", buf, childIndent.c_str());
    return true;
}

bool 
CSharpCoder::printClassGetFunc(char* buf, const TinyXmlNode* node, const String& className, const String& indent)
{
    int pos = className.find("Table");
    String str = className.substr(0, pos);
    str += "Template";
    
    String childIndent = indent + "    ";
    sprintf(buf, "%s%spublic %s Get(UInt32 id)\r\n", buf, childIndent.c_str(), str.c_str());
    sprintf(buf, "%s%s{\r\n", buf, childIndent.c_str());
    sprintf(buf, "%s%s    %s template = null;\r\n", buf, childIndent.c_str(), str.c_str());
    sprintf(buf, "%s%s    if (!mMap.TryGetValue(id, out template))\r\n", buf, childIndent.c_str());
    sprintf(buf, "%s%s    {\r\n", buf, childIndent.c_str());
    sprintf(buf, "%s%s         return null;\r\n", buf, childIndent.c_str());
    sprintf(buf, "%s%s    }\r\n", buf, childIndent.c_str());
    sprintf(buf, "%s%s    return template;\r\n", buf, childIndent.c_str());
    sprintf(buf, "%s%s}\r\n\r\n", buf, childIndent.c_str());
    return true;
}

bool 
CSharpCoder::printClassClearFunc(char* buf, const TinyXmlNode* node, const String& className, const String& indent)
{
    String childIndent = indent + "    ";
    sprintf(buf, "%s%spublic void Clear(UInt32 id)\r\n", buf, childIndent.c_str());
    sprintf(buf, "%s%s{\r\n", buf, childIndent.c_str());
    sprintf(buf, "%s%s    mMap.Clear();\r\n", buf, childIndent.c_str());
    sprintf(buf, "%s%s}\r\n\r\n", buf, childIndent.c_str());
    return true;
}

bool 
CSharpCoder::printClassLoadFromDbcFunc(char* buf, const TinyXmlNode* node, const String& className, const String& indent)
{
    String childIndent = indent + "    ";
    sprintf(buf, "%s%spublic bool LoadFromDbc(String fileName)\r\n", buf, childIndent.c_str());
    sprintf(buf, "%s%s{\r\n", buf, childIndent.c_str());
    sprintf(buf, "%s%s    return unserialize(DBCFileManager.GetDBCFileObject(fileName), 0) == 1;\r\n", buf, childIndent.c_str());
    sprintf(buf, "%s%s}\r\n\r\n", buf, childIndent.c_str());
    return true;
}
