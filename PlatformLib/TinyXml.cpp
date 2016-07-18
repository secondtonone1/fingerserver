/*
www.sourceforge.net/projects/tinyxml
Original code (2.0 and earlier)copyright (c) 2000-2006 Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a produce, an acknowledgment in the produce documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include <ctype.h>

#ifdef TinyXml_USE_STL
#include <sstream>
#include <iostream>
#endif

#include "TinyXml.h"
#include "MemoryManager.h"
#include "Utilex.h"
#include "StringConverter.h"


bool TinyXmlBase::condenseWhiteSpace = true;

// Microsoft compiler security
FILE* TinyXmlFOpen(const char* filename, const char* mode)
{
	#if defined(_MSC_VER) && (_MSC_VER >= 1400)
		FILE* fp = 0;
		errno_t err = fopen_s(&fp, filename, mode);
		if (!err && fp)
			return fp;
		return 0;
	#else
		return fopen(filename, mode);
	#endif
}

void TinyXmlBase::encodeString(const TinyXml_STRING& str, TinyXml_STRING* outString)
{
	int i=0;

	while(i<(int)str.length())
	{
		unsigned char c = (unsigned char) str[i];

		if (   c == '&' 
		     && i < ((int)str.length() - 2)
			 && str[i+1] == '#'
			 && str[i+2] == 'x')
		{
			// Hexadecimal character reference.
			// Pass through unchanged.
			// &#xA9;	-- copyright symbol, for example.
			//
			// The -1 is a bug fix from Rob Laveaux. It keeps
			// an overflow from happening if there is no ';'.
			// There are actually 2 ways to exit this loop -
			// while fails (error case) and break (semicolon found).
			// However, there is no mechanism (currently) for
			// this function to return an error.
			while (i<(int)str.length()-1)
			{
				outString->append(str.c_str() + i, 1);
				++i;
				if (str[i] == ';')
					break;
			}
		}
		else if (c == '&')
		{
			outString->append(entity[0].str, entity[0].strLength);
			++i;
		}
		else if (c == '<')
		{
			outString->append(entity[1].str, entity[1].strLength);
			++i;
		}
		else if (c == '>')
		{
			outString->append(entity[2].str, entity[2].strLength);
			++i;
		}
		else if (c == '\"')
		{
			outString->append(entity[3].str, entity[3].strLength);
			++i;
		}
		else if (c == '\'')
		{
			outString->append(entity[4].str, entity[4].strLength);
			++i;
		}
		else if (c < 32)
		{
			// Easy pass at non-alpha/numeric/symbol
			// Below 32 is symbolic.
			char buf[ 32 ];
			
			#if defined(TinyXml_SNPRINTF)		
				TinyXml_SNPRINTF(buf, sizeof(buf), "&#x%02X;", (unsigned) (c & 0xff));
			#else
				snprintf(buf, sizeof(buf), "&#x%02X;", (unsigned) (c & 0xff));
			#endif		

			//*ME:	warning C4267: convert 'size_t' to 'int'
			//*ME:	Int-Cast to make compiler happy ...
			outString->append(buf, (int)strlen(buf));
			++i;
		}
		else
		{
			//char realc = (char) c;
			//outString->append(&realc, 1);
			*outString += (char) c;	// somewhat more efficient function call.
			++i;
		}
	}
}


TinyXmlNode::TinyXmlNode(IterType _type) : TinyXmlBase()
{
	parent = 0;
	type = _type;
	firstChild = 0;
	lastChild = 0;
	prev = 0;
	next = 0;
}


TinyXmlNode::~TinyXmlNode()
{
	TinyXmlNode* node = firstChild;
	TinyXmlNode* temp = 0;

	while (node)
	{
		temp = node;
		node = node->next;
		XDELETE(temp); //delete temp;
	}	
}


void TinyXmlNode::copyTo(TinyXmlNode* target) const
{
	target->setValue (value.c_str());
	target->userData = userData; 
}


void TinyXmlNode::clear()
{
	TinyXmlNode* node = firstChild;
	TinyXmlNode* temp = 0;

	while (node)
	{
		temp = node;
		node = node->next;
		XDELETE(temp); //delete temp;
	}	

	firstChild = 0;
	lastChild = 0;
}


TinyXmlNode* TinyXmlNode::linkEndChild(TinyXmlNode* node)
{
	ASSERT(node->parent == 0 || node->parent == this);
	ASSERT(node->getDocument() == 0 || node->getDocument() == this->getDocument());

	if (node->Type() == TinyXmlNode::DOCUMENT)
	{
		XDELETE(node); //delete node;
		if (getDocument()) getDocument()->setError(TinyXml_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TinyXml_ENCODING_UNKNOWN);
		return 0;
	}

	node->parent = this;

	node->prev = lastChild;
	node->next = 0;

	if (lastChild)
		lastChild->next = node;
	else
		firstChild = node;			// it was an empty list.

	lastChild = node;
	return node;
}


TinyXmlNode* TinyXmlNode::insertEndChild(const TinyXmlNode& addThis)
{
	if (addThis.Type() == TinyXmlNode::DOCUMENT)
	{
		if (getDocument()) getDocument()->setError(TinyXml_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TinyXml_ENCODING_UNKNOWN);
		return 0;
	}
	TinyXmlNode* node = addThis.clone();
	if (!node)
		return 0;

	return linkEndChild(node);
}


TinyXmlNode* TinyXmlNode::insertBeforeChild(TinyXmlNode* beforeThis, const TinyXmlNode& addThis)
{	
	if (!beforeThis || beforeThis->parent != this) {
		return 0;
	}
	if (addThis.Type() == TinyXmlNode::DOCUMENT)
	{
		if (getDocument()) getDocument()->setError(TinyXml_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TinyXml_ENCODING_UNKNOWN);
		return 0;
	}

	TinyXmlNode* node = addThis.clone();
	if (!node)
		return 0;
	node->parent = this;

	node->next = beforeThis;
	node->prev = beforeThis->prev;
	if (beforeThis->prev)
	{
		beforeThis->prev->next = node;
	}
	else
	{
		ASSERT(firstChild == beforeThis);
		firstChild = node;
	}
	beforeThis->prev = node;
	return node;
}


TinyXmlNode* TinyXmlNode::insertAfterChild(TinyXmlNode* afterThis, const TinyXmlNode& addThis)
{
	if (!afterThis || afterThis->parent != this) {
		return 0;
	}
	if (addThis.Type() == TinyXmlNode::DOCUMENT)
	{
		if (getDocument()) getDocument()->setError(TinyXml_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TinyXml_ENCODING_UNKNOWN);
		return 0;
	}

	TinyXmlNode* node = addThis.clone();
	if (!node)
		return 0;
	node->parent = this;

	node->prev = afterThis;
	node->next = afterThis->next;
	if (afterThis->next)
	{
		afterThis->next->prev = node;
	}
	else
	{
		ASSERT(lastChild == afterThis);
		lastChild = node;
	}
	afterThis->next = node;
	return node;
}


TinyXmlNode* TinyXmlNode::replaceChild(TinyXmlNode* replaceThis, const TinyXmlNode& withThis)
{
	if (replaceThis->parent != this)
		return 0;

	TinyXmlNode* node = withThis.clone();
	if (!node)
		return 0;

	node->next = replaceThis->next;
	node->prev = replaceThis->prev;

	if (replaceThis->next)
		replaceThis->next->prev = node;
	else
		lastChild = node;

	if (replaceThis->prev)
		replaceThis->prev->next = node;
	else
		firstChild = node;

	XDELETE(replaceThis); //delete replaceThis;
	node->parent = this;
	return node;
}


bool TinyXmlNode::removeChild(TinyXmlNode* removeThis)
{
	if (removeThis->parent != this)
	{	
		ASSERT(0);
		return false;
	}

	if (removeThis->next)
		removeThis->next->prev = removeThis->prev;
	else
		lastChild = removeThis->prev;

	if (removeThis->prev)
		removeThis->prev->next = removeThis->next;
	else
		firstChild = removeThis->next;

	XDELETE(removeThis); //delete removeThis;
	return true;
}

const TinyXmlNode* TinyXmlNode::getFirstChild(const char * _value) const
{
	const TinyXmlNode* node;
	for (node = firstChild; node; node = node->next)
	{
		if (strcmp(node->getValue(), _value) == 0)
			return node;
	}
	return 0;
}


const TinyXmlNode* TinyXmlNode::getLastChild(const char * _value) const
{
	const TinyXmlNode* node;
	for (node = lastChild; node; node = node->prev)
	{
		if (strcmp(node->getValue(), _value) == 0)
			return node;
	}
	return 0;
}


const TinyXmlNode* TinyXmlNode::iterateChildren(const TinyXmlNode* previous) const
{
	if (!previous)
	{
		return getFirstChild();
	}
	else
	{
		ASSERT(previous->parent == this);
		return previous->nextSibling();
	}
}


const TinyXmlNode* TinyXmlNode::iterateChildren(const char * val, const TinyXmlNode* previous) const
{
	if (!previous)
	{
		return getFirstChild(val);
	}
	else
	{
		ASSERT(previous->parent == this);
		return previous->nextSibling(val);
	}
}


const TinyXmlNode* TinyXmlNode::nextSibling(const char * _value) const 
{
	const TinyXmlNode* node;
	for (node = next; node; node = node->next)
	{
		if (strcmp(node->getValue(), _value) == 0)
			return node;
	}
	return 0;
}


const TinyXmlNode* TinyXmlNode::previousSibling(const char * _value) const
{
	const TinyXmlNode* node;
	for (node = prev; node; node = node->prev)
	{
		if (strcmp(node->getValue(), _value) == 0)
			return node;
	}
	return 0;
}


void TinyXmlElement::removeAttribute(const char * name)
{
    #ifdef TinyXml_USE_STL
	TinyXml_STRING str(name);
	TinyXmlAttribute* node = attributeSet.find(str);
	#else
	TinyXmlAttribute* node = attributeSet.find(name);
	#endif
	if (node)
	{
		attributeSet.remove(node);
		XDELETE(node); //delete node;
	}
}

const TinyXmlElement* TinyXmlNode::firstChildElement() const
{
	const TinyXmlNode* node;

	for (node = getFirstChild();
			node;
			node = node->nextSibling())
	{
		if (node->toElement())
			return node->toElement();
	}
	return 0;
}


const TinyXmlElement* TinyXmlNode::firstChildElement(const char * _value) const
{
	const TinyXmlNode* node;

	for (node = getFirstChild(_value);
			node;
			node = node->nextSibling(_value))
	{
		if (node->toElement())
			return node->toElement();
	}
	return 0;
}


const TinyXmlElement* TinyXmlNode::nextSiblingElement() const
{
	const TinyXmlNode* node;

	for (node = nextSibling();
			node;
			node = node->nextSibling())
	{
		if (node->toElement())
			return node->toElement();
	}
	return 0;
}


const TinyXmlElement* TinyXmlNode::nextSiblingElement(const char * _value) const
{
	const TinyXmlNode* node;

	for (node = nextSibling(_value);
			node;
			node = node->nextSibling(_value))
	{
		if (node->toElement())
			return node->toElement();
	}
	return 0;
}


const TinyXmlDocument* TinyXmlNode::getDocument() const
{
	const TinyXmlNode* node;

	for(node = this; node; node = node->parent)
	{
		if (node->toDocument())
			return node->toDocument();
	}
	return 0;
}


TinyXmlElement::TinyXmlElement (const char * _value)
	: TinyXmlNode(TinyXmlNode::ELEMENT)
{
	firstChild = lastChild = 0;
	value = _value;
}


#ifdef TinyXml_USE_STL
TinyXmlElement::TinyXmlElement(const String& _value) 
	: TinyXmlNode(TinyXmlNode::ELEMENT)
{
	firstChild = lastChild = 0;
	value = _value;
}
#endif


TinyXmlElement::TinyXmlElement(const TinyXmlElement& copy)
	: TinyXmlNode(TinyXmlNode::ELEMENT)
{
	firstChild = lastChild = 0;
	copy.copyTo(this);	
}


void TinyXmlElement::operator=(const TinyXmlElement& base)
{
	clearThis();
	base.copyTo(this);
}


TinyXmlElement::~TinyXmlElement()
{
	clearThis();
}


void TinyXmlElement::clearThis()
{
	clear();
	while(attributeSet.first())
	{
		TinyXmlAttribute* node = attributeSet.first();
		attributeSet.remove(node);
		XDELETE(node); //delete node;
	}
}


const char* TinyXmlElement::getAttribute(const char* name) const
{
	const TinyXmlAttribute* node = attributeSet.find(name);
	if (node)
		return node->getValue();
	return ""; // Hack by Hailong.Xia
}


#ifdef TinyXml_USE_STL
const String* TinyXmlElement::getAttribute(const String& name) const
{
	const TinyXmlAttribute* node = attributeSet.find(name);
	if (node)
		return &node->ValueStr();
	return 0;
}
#endif


const char* TinyXmlElement::getAttribute(const char* name, int* i) const
{
	const char* s = getAttribute(name);
	if (i)
	{
		if (s) {
			*i = lynxAtoi<int>(s);
		}
		else {
			*i = 0;
		}
	}
	return s;
}


#ifdef TinyXml_USE_STL
const String* TinyXmlElement::getAttribute(const String& name, int* i) const
{
	const String* s = getAttribute(name);
	if (i)
	{
		if (s) {
			*i = lynxAtoi(s->c_str());
		}
		else {
			*i = 0;
		}
	}
	return s;
}
#endif


const char* TinyXmlElement::getAttribute(const char* name, double* d) const
{
	const char* s = getAttribute(name);
	if (d)
	{
		if (s) {
			*d = atof(s);
		}
		else {
			*d = 0;
		}
	}
	return s;
}

const char* TinyXmlElement::getAttribute(const char* name, float* f) const
{
    const char* s = getAttribute(name);
    if (f)
    {
        if (s) {
            *f = (float)atof(s);
        }
        else {
            *f = 0;
        }
    }
    return s;
}

const char* TinyXmlElement::getAttribute(const char* name, bool* b) const
{
    const char* s = getAttribute(name);
    if (b)
    {
        if (s) {
            *b = Lynx::StringConverter::parseBool(s);
        }
        else {
            *b = false;
        }
    }
    return s;
}

const char* TinyXmlElement::getAttribute(const char* name, unsigned int* ui) const
{
    const char* s = getAttribute(name);
    if (ui)
    {
        if (s) {
            *ui = Lynx::StringConverter::parseUnsignedInt(s);
        }
        else {
            *ui = false;
        }
    }
    return s;
}
#ifdef TinyXml_USE_STL
const String* TinyXmlElement::getAttribute(const String& name, double* d) const
{
	const String* s = getAttribute(name);
	if (d)
	{
		if (s) {
			*d = atof(s->c_str());
		}
		else {
			*d = 0;
		}
	}
	return s;
}
#endif


int TinyXmlElement::queryIntAttribute(const char* name, int* ival) const
{
	const TinyXmlAttribute* node = attributeSet.find(name);
	if (!node)
		return TinyXml_NO_ATTRIBUTE;
	return node->queryIntValue(ival);
}


#ifdef TinyXml_USE_STL
int TinyXmlElement::queryIntAttribute(const String& name, int* ival) const
{
	const TinyXmlAttribute* node = attributeSet.find(name);
	if (!node)
		return TinyXml_NO_ATTRIBUTE;
	return node->queryIntValue(ival);
}
#endif


int TinyXmlElement::queryDoubleAttribute(const char* name, double* dval) const
{
	const TinyXmlAttribute* node = attributeSet.find(name);
	if (!node)
		return TinyXml_NO_ATTRIBUTE;
	return node->queryDoubleValue(dval);
}


#ifdef TinyXml_USE_STL
int TinyXmlElement::queryDoubleAttribute(const String& name, double* dval) const
{
	const TinyXmlAttribute* node = attributeSet.find(name);
	if (!node)
		return TinyXml_NO_ATTRIBUTE;
	return node->queryDoubleValue(dval);
}
#endif


void TinyXmlElement::setAttribute(const char * name, int val)
{	
	char buf[64];
	#if defined(TinyXml_SNPRINTF)		
		TinyXml_SNPRINTF(buf, sizeof(buf), "%d", val);
	#else
		snprintf(buf, sizeof(buf), "%d", val);
	#endif
	setAttribute(name, buf);
}
void TinyXmlElement::setAttribute(const char * name, float val)
{
    char buf[64];
#if defined(TinyXml_SNPRINTF)		
    TinyXml_SNPRINTF(buf, sizeof(buf), "%f", val);
#else
    snprintf(buf, sizeof(buf), "%f", val);
#endif
    setAttribute(name, buf);
}
void TinyXmlElement::setAttribute(const char * name, bool val)
{
    char buf[64];
#if defined(TinyXml_SNPRINTF)		
    TinyXml_SNPRINTF(buf, sizeof(buf), "%s", Lynx::StringConverter::toString(val).c_str());
#else
    snprintf(buf, sizeof(buf), "%s", Lynx::StringConverter::toString(val).c_str());
#endif
    setAttribute(name, buf);
}

void TinyXmlElement::setAttribute(const char * name, unsigned int val)
{
    char buf[64];
#if defined(TinyXml_SNPRINTF)		
    TinyXml_SNPRINTF(buf, sizeof(buf), "%u", val);
#else
    snprintf(buf, sizeof(buf), "%u", val);
#endif
    setAttribute(name, buf);
}

#ifdef TinyXml_USE_STL
void TinyXmlElement::setAttribute(const String& name, int val)
{	
   std::ostringstream oss;
   oss << val;
   setAttribute(name, oss.str());
}
#endif


void TinyXmlElement::setDoubleAttribute(const char * name, double val)
{	
	char buf[256];
	#if defined(TinyXml_SNPRINTF)		
		TinyXml_SNPRINTF(buf, sizeof(buf), "%f", val);
	#else
		snprintf(buf, sizeof(buf), "%f", val);
	#endif
	setAttribute(name, buf);
}


void TinyXmlElement::setAttribute(const char * cname, const char * cvalue)
{
    #ifdef TinyXml_USE_STL
	TinyXml_STRING _name(cname);
	TinyXml_STRING _value(cvalue);
	#else
	const char* _name = cname;
	const char* _value = cvalue;
	#endif

	TinyXmlAttribute* node = attributeSet.find(_name);
	if (node)
	{
		node->setValue(_value);
		return;
	}

	TinyXmlAttribute* attrib = XNEW(TinyXmlAttribute)(cname, cvalue); //new TinyXmlAttribute(cname, cvalue);
	if (attrib)
	{
		attributeSet.add(attrib);
	}
	else
	{
		TinyXmlDocument* document = getDocument();
		if (document) document->setError(TinyXml_ERROR_OUT_OF_MEMORY, 0, 0, TinyXml_ENCODING_UNKNOWN);
	}
}


#ifdef TinyXml_USE_STL
void TinyXmlElement::setAttribute(const String& name, const String& _value)
{
	TinyXmlAttribute* node = attributeSet.find(name);
	if (node)
	{
		node->setValue(_value);
		return;
	}

	TinyXmlAttribute* attrib = XNEW(TinyXmlAttribute)(name, _value); //new TinyXmlAttribute(name, _value);
	if (attrib)
	{
		attributeSet.add(attrib);
	}
	else
	{
		TinyXmlDocument* document = getDocument();
		if (document) document->setError(TinyXml_ERROR_OUT_OF_MEMORY, 0, 0, TinyXml_ENCODING_UNKNOWN);
	}
}
#endif


void TinyXmlElement::print(FILE* cfile, int depth) const
{
	int i;
	ASSERT(cfile);
	for (i=0; i<depth; i++) {
		fprintf(cfile, "    ");
	}

	fprintf(cfile, "<%s", value.c_str());

	const TinyXmlAttribute* attrib;
	for (attrib = attributeSet.first(); attrib; attrib = attrib->getNext())
	{
		fprintf(cfile, " ");
		attrib->print(cfile, depth);
	}

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	TinyXmlNode* node;
	if (!firstChild)
	{
		fprintf(cfile, " />");
	}
	else if (firstChild == lastChild && firstChild->toText())
	{
		fprintf(cfile, ">");
		firstChild->print(cfile, depth + 1);
		fprintf(cfile, "</%s>", value.c_str());
	}
	else
	{
		fprintf(cfile, ">");

		for (node = firstChild; node; node=node->nextSibling())
		{
			if (!node->toText())
			{
				fprintf(cfile, "\n");
			}
			node->print(cfile, depth+1);
		}
		fprintf(cfile, "\n");
		for(i=0; i<depth; ++i) {
			fprintf(cfile, "    ");
		}
		fprintf(cfile, "</%s>", value.c_str());
	}
}


void TinyXmlElement::copyTo(TinyXmlElement* target) const
{
	// superclass:
	TinyXmlNode::copyTo(target);

	// element class: 
	// clone the attributes, then clone the children.
	const TinyXmlAttribute* attribute = 0;
	for(attribute = attributeSet.first();
	attribute;
	attribute = attribute->getNext())
	{
		target->setAttribute(attribute->getName(), attribute->getValue());
	}

	TinyXmlNode* node = 0;
	for (node = firstChild; node; node = node->nextSibling())
	{
		target->linkEndChild(node->clone());
	}
}

bool TinyXmlElement::accept(TinyXmlVisitor* visitor) const
{
	if (visitor->visitEnter(*this, attributeSet.first())) 
	{
		for (const TinyXmlNode* node = getFirstChild(); node; node = node->nextSibling())
		{
			if (!node->accept(visitor))
				break;
		}
	}
	return visitor->visitExit(*this);
}


TinyXmlNode* TinyXmlElement::clone() const
{
	TinyXmlElement* clone = XNEW(TinyXmlElement)(getValue()); //new TinyXmlElement(getValue());
	if (!clone)
		return 0;

	copyTo(clone);
	return clone;
}


const char* TinyXmlElement::getText() const
{
	const TinyXmlNode* child = this->getFirstChild();
	if (child) {
		const TinyXmlText* childText = child->toText();
		if (childText) {
			return childText->getValue();
		}
	}
	return 0;
}


TinyXmlDocument::TinyXmlDocument() : TinyXmlNode(TinyXmlNode::DOCUMENT)
{
	tabsize = 4;
	useMicrosoftBOM = false;
	clearError();
}

TinyXmlDocument::TinyXmlDocument(const char * documentName) : TinyXmlNode(TinyXmlNode::DOCUMENT)
{
	tabsize = 4;
	useMicrosoftBOM = false;
	value = documentName;
	clearError();
}


#ifdef TinyXml_USE_STL
TinyXmlDocument::TinyXmlDocument(const String& documentName) : TinyXmlNode(TinyXmlNode::DOCUMENT)
{
	tabsize = 4;
	useMicrosoftBOM = false;
    value = documentName;
	clearError();
}
#endif


TinyXmlDocument::TinyXmlDocument(const TinyXmlDocument& copy) : TinyXmlNode(TinyXmlNode::DOCUMENT)
{
	copy.copyTo(this);
}


void TinyXmlDocument::operator=(const TinyXmlDocument& copy)
{
	clear();
	copy.copyTo(this);
}


bool TinyXmlDocument::loadFile(TinyXmlEncoding encoding)
{
	// See STL_STRING_BUG below.
	//StringToBuffer buf(value);

	return loadFile(getValue(), encoding);
}


bool TinyXmlDocument::saveFile() const
{
	// See STL_STRING_BUG below.
//	StringToBuffer buf(value);
//
//	if (buf.buffer && saveFile(buf.buffer))
//		return true;
//
//	return false;
	return saveFile(getValue());
}

bool TinyXmlDocument::loadFile(const char* _filename, TinyXmlEncoding encoding)
{
	// There was a really terrifying little bug here. The code:
	//		value = filename
	// in the STL case, cause the assignment method of the String to
	// be called. What is strange, is that the String had the same
	// address as it's c_str() method, and so bad things happen. Looks
	// like a bug in the Microsoft STL implementation.
	// add an extra string to avoid the crash.
	TinyXml_STRING filename(_filename);
	value = filename;

	// reading in binary mode so that tinyxml can normalize the EOL
	FILE* file = TinyXmlFOpen(value.c_str (), "rb");	

	if (file)
	{
		bool result = loadFile(file, encoding);
		fclose(file);
		return result;
	}
	else
	{
		setError(TinyXml_ERROR_OPENING_FILE, 0, 0, TinyXml_ENCODING_UNKNOWN);
		return false;
	}
}

bool TinyXmlDocument::loadFile(FILE* file, TinyXmlEncoding encoding)
{
	if (!file) 
	{
		setError(TinyXml_ERROR_OPENING_FILE, 0, 0, TinyXml_ENCODING_UNKNOWN);
		return false;
	}

	// Delete the existing data:
	clear();
	location.clear();

	// get the file size, so we can pre-allocate the string. HUGE speed impact.
	long length = 0;
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET);

	// Strange case, but good to handle up front.
	if (length <= 0)
	{
		setError(TinyXml_ERROR_DOCUMENT_EMPTY, 0, 0, TinyXml_ENCODING_UNKNOWN);
		return false;
	}

	// If we have a file, assume it is all one big XML file, and read it in.
	// The document parser may decide the document ends sooner than the entire file, however.
	TinyXml_STRING data;
	data.reserve(length);

	// Subtle bug here. TinyXml did use fgets. But from the XML spec:
	// 2.11 end-of-Line Handling
	// <snip>
	// <quote>
	// ...the XML processor MUST behave as if it normalized all line breaks in external 
	// parsed entities (including the document entity) on input, before parsing, by translating 
	// both the two-character sequence #xD #xA and any #xD that is not followed by #xA to 
	// a single #xA character.
	// </quote>
	//
	// It is not clear fgets does that, and certainly isn't clear it works cross platform. 
	// Generally, you expect fgets to translate from the convention of the OS to the c/unix
	// convention, and not work generally.

	/*
	while(fgets(buf, sizeof(buf), file))
	{
		data += buf;
	}
	*/

	char* buf = new char[ length+1 ];
	buf[0] = 0;

	if (fread(buf, length, 1, file) != 1) {
		delete [] buf;
		setError(TinyXml_ERROR_OPENING_FILE, 0, 0, TinyXml_ENCODING_UNKNOWN);
		return false;
	}

	const char* lastPos = buf;
	const char* p = buf;

    buf[length] = 0;
	while(*p) {
		ASSERT(p < (buf+length));
		if (*p == 0xa) {
			// Newline character. No special rules for this. Append all the characters
			// since the last string, and include the newline.
			data.append(lastPos, (p-lastPos+1));	// append, include the newline
			++p;									// move past the newline
			lastPos = p;							// and point to the new buffer (may be 0)
			ASSERT(p <= (buf+length));
		}
		else if (*p == 0xd) {
			// Carriage return. Append what we have so far, then
			// handle moving forward in the buffer.
			if ((p-lastPos) > 0) {
				data.append(lastPos, p-lastPos);	// do not add the CR
			}
			data += (char)0xa;						// a proper newline

			if (*(p+1) == 0xa) {
				// Carriage return - new line sequence
				p += 2;
				lastPos = p;
				ASSERT(p <= (buf+length));
			}
			else {
				// it was followed by something else...that is presumably characters again.
				++p;
				lastPos = p;
				ASSERT(p <= (buf+length));
			}
		}
		else {
			++p;
		}
	}
	// Handle any left over characters.
	if (p-lastPos) {
		data.append(lastPos, p-lastPos);
	}		
	delete [] buf;
	buf = 0;

	parse(data.c_str(), 0, encoding);

	if ( getError())
        return false;
    else
		return true;
}


bool TinyXmlDocument::saveFile(const char * filename) const
{
	// The old c stuff lives on...
	FILE* fp = TinyXmlFOpen(filename, "w");
	if (fp)
	{
		bool result = saveFile(fp);
		fclose(fp);
		return result;
	}
	return false;
}


bool TinyXmlDocument::saveFile(FILE* fp) const
{
	if (useMicrosoftBOM) 
	{
		const unsigned char TinyXml_UTF_LEAD_0 = 0xefU;
		const unsigned char TinyXml_UTF_LEAD_1 = 0xbbU;
		const unsigned char TinyXml_UTF_LEAD_2 = 0xbfU;

		fputc(TinyXml_UTF_LEAD_0, fp);
		fputc(TinyXml_UTF_LEAD_1, fp);
		fputc(TinyXml_UTF_LEAD_2, fp);
	}
	print(fp, 0);
	return (ferror(fp) == 0);
}


void TinyXmlDocument::copyTo(TinyXmlDocument* target) const
{
	TinyXmlNode::copyTo(target);

	target->error = error;
	target->errorId = errorId;
	target->errorDesc = errorDesc;
	target->tabsize = tabsize;
	target->errorLocation = errorLocation;
	target->useMicrosoftBOM = useMicrosoftBOM;

	TinyXmlNode* node = 0;
	for (node = firstChild; node; node = node->nextSibling())
	{
		target->linkEndChild(node->clone());
	}	
}


TinyXmlNode* TinyXmlDocument::clone() const
{
	TinyXmlDocument* clone = XNEW(TinyXmlDocument)(); //new TinyXmlDocument();
	if (!clone)
		return 0;

	copyTo(clone);
	return clone;
}


void TinyXmlDocument::print(FILE* cfile, int depth) const
{
	ASSERT(cfile);
	for (const TinyXmlNode* node = getFirstChild(); node; node = node->nextSibling())
	{
		node->print(cfile, depth);
		fprintf(cfile, "\n");
	}
}


bool TinyXmlDocument::accept(TinyXmlVisitor* visitor) const
{
	if (visitor->visitEnter(*this))
	{
		for (const TinyXmlNode* node = getFirstChild(); node; node = node->nextSibling())
		{
			if (!node->accept(visitor))
				break;
		}
	}
	return visitor->visitExit(*this);
}


const TinyXmlAttribute* TinyXmlAttribute::getNext() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if (next->value.empty() && next->name.empty())
		return 0;
	return next;
}

/*
TinyXmlAttribute* TinyXmlAttribute::getNext()
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if (next->value.empty() && next->name.empty())
		return 0;
	return next;
}
*/

const TinyXmlAttribute* TinyXmlAttribute::previous() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if (prev->value.empty() && prev->name.empty())
		return 0;
	return prev;
}

/*
TinyXmlAttribute* TinyXmlAttribute::previous()
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if (prev->value.empty() && prev->name.empty())
		return 0;
	return prev;
}
*/

void TinyXmlAttribute::print(FILE* cfile, int /*depth*/, TinyXml_STRING* str) const
{
	TinyXml_STRING n, v;

	encodeString(name, &n);
	encodeString(value, &v);

	if (value.find ('\"') == TinyXml_STRING::npos) {
		if (cfile) {
		fprintf (cfile, "%s=\"%s\"", n.c_str(), v.c_str());
		}
		if (str) {
			(*str) += n; (*str) += "=\""; (*str) += v; (*str) += "\"";
		}
	}
	else {
		if (cfile) {
		fprintf (cfile, "%s='%s'", n.c_str(), v.c_str());
		}
		if (str) {
			(*str) += n; (*str) += "='"; (*str) += v; (*str) += "'";
		}
	}
}


int TinyXmlAttribute::queryIntValue(int* ival) const
{
	if (TinyXml_SSCANF(value.c_str(), "%d", ival) == 1)
		return TinyXml_SUCCESS;
	return TinyXml_WRONG_TYPE;
}

int TinyXmlAttribute::queryDoubleValue(double* dval) const
{
	if (TinyXml_SSCANF(value.c_str(), "%lf", dval) == 1)
		return TinyXml_SUCCESS;
	return TinyXml_WRONG_TYPE;
}

void TinyXmlAttribute::setIntValue(int _value)
{
	char buf [64];
	#if defined(TinyXml_SNPRINTF)		
		TinyXml_SNPRINTF(buf, sizeof(buf), "%d", _value);
	#else
		snprintf (buf, sizeof(buf), "%d", _value);
	#endif
	setValue (buf);
}

void TinyXmlAttribute::setDoubleValue(double _value)
{
	char buf [256];
	#if defined(TinyXml_SNPRINTF)		
		TinyXml_SNPRINTF(buf, sizeof(buf), "%lf", _value);
	#else
		snprintf (buf, sizeof(buf), "%lf", _value);
	#endif
	setValue (buf);
}

int TinyXmlAttribute::intValue() const
{
	return lynxAtoi<int>(value.c_str ());
}

double  TinyXmlAttribute::doubleValue() const
{
	return atof (value.c_str ());
}


TinyXmlComment::TinyXmlComment(const TinyXmlComment& copy) : TinyXmlNode(TinyXmlNode::COMMENT)
{
	copy.copyTo(this);
}


void TinyXmlComment::operator=(const TinyXmlComment& base)
{
	clear();
	base.copyTo(this);
}


void TinyXmlComment::print(FILE* cfile, int depth) const
{
	ASSERT(cfile);
	for (int i=0; i<depth; i++)
	{
		fprintf(cfile,  "    ");
	}
	fprintf(cfile, "<!--%s-->", value.c_str());
}


void TinyXmlComment::copyTo(TinyXmlComment* target) const
{
	TinyXmlNode::copyTo(target);
}


bool TinyXmlComment::accept(TinyXmlVisitor* visitor) const
{
	return visitor->visit(*this);
}


TinyXmlNode* TinyXmlComment::clone() const
{
	TinyXmlComment* clone = XNEW(TinyXmlComment)(); //new TinyXmlComment();

	if (!clone)
		return 0;

	copyTo(clone);
	return clone;
}


void TinyXmlText::print(FILE* cfile, int depth) const
{
	ASSERT(cfile);
	if (cdata)
	{
		int i;
		fprintf(cfile, "\n");
		for (i=0; i<depth; i++) {
			fprintf(cfile, "    ");
		}
		fprintf(cfile, "<![getCData[%s]]>\n", value.c_str());	// unformatted output
	}
	else
	{
		TinyXml_STRING buffer;
		encodeString(value, &buffer);
		fprintf(cfile, "%s", buffer.c_str());
	}
}


void TinyXmlText::copyTo(TinyXmlText* target) const
{
	TinyXmlNode::copyTo(target);
	target->cdata = cdata;
}


bool TinyXmlText::accept(TinyXmlVisitor* visitor) const
{
	return visitor->visit(*this);
}


TinyXmlNode* TinyXmlText::clone() const
{	
	TinyXmlText* clone = 0;
	clone = XNEW(TinyXmlText)(""); //new TinyXmlText("");

	if (!clone)
		return 0;

	copyTo(clone);
	return clone;
}


TinyXmlDeclaration::TinyXmlDeclaration(const char * _version,
									const char * _encoding,
									const char * _standalone)
	: TinyXmlNode(TinyXmlNode::DECLARATION)
{
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}


#ifdef TinyXml_USE_STL
TinyXmlDeclaration::TinyXmlDeclaration(const String& _version,
									const String& _encoding,
									const String& _standalone)
	: TinyXmlNode(TinyXmlNode::DECLARATION)
{
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}
#endif


TinyXmlDeclaration::TinyXmlDeclaration(const TinyXmlDeclaration& copy)
	: TinyXmlNode(TinyXmlNode::DECLARATION)
{
	copy.copyTo(this);	
}


void TinyXmlDeclaration::operator=(const TinyXmlDeclaration& copy)
{
	clear();
	copy.copyTo(this);
}


void TinyXmlDeclaration::print(FILE* cfile, int /*depth*/, TinyXml_STRING* str) const
{
	if (cfile) fprintf(cfile, "<?xml ");
	if (str)	 (*str) += "<?xml ";

	if (!version.empty()) {
		if (cfile) fprintf (cfile, "version=\"%s\" ", version.c_str ());
		if (str) { (*str) += "version=\""; (*str) += version; (*str) += "\" "; }
	}
	if (!encoding.empty()) {
		if (cfile) fprintf (cfile, "encoding=\"%s\" ", encoding.c_str ());
		if (str) { (*str) += "encoding=\""; (*str) += encoding; (*str) += "\" "; }
	}
	if (!standalone.empty()) {
		if (cfile) fprintf (cfile, "standalone=\"%s\" ", standalone.c_str ());
		if (str) { (*str) += "standalone=\""; (*str) += standalone; (*str) += "\" "; }
	}
	if (cfile) fprintf(cfile, "?>");
	if (str)	 (*str) += "?>";
}


void TinyXmlDeclaration::copyTo(TinyXmlDeclaration* target) const
{
	TinyXmlNode::copyTo(target);

	target->version = version;
	target->encoding = encoding;
	target->standalone = standalone;
}


bool TinyXmlDeclaration::accept(TinyXmlVisitor* visitor) const
{
	return visitor->visit(*this);
}


TinyXmlNode* TinyXmlDeclaration::clone() const
{	
	TinyXmlDeclaration* clone = XNEW(TinyXmlDeclaration)(); //new TinyXmlDeclaration();

	if (!clone)
		return 0;

	copyTo(clone);
	return clone;
}


void TinyXmlUnknown::print(FILE* cfile, int depth) const
{
	for (int i=0; i<depth; i++)
		fprintf(cfile, "    ");
	fprintf(cfile, "<%s>", value.c_str());
}


void TinyXmlUnknown::copyTo(TinyXmlUnknown* target) const
{
	TinyXmlNode::copyTo(target);
}


bool TinyXmlUnknown::accept(TinyXmlVisitor* visitor) const
{
	return visitor->visit(*this);
}


TinyXmlNode* TinyXmlUnknown::clone() const
{
	TinyXmlUnknown* clone = XNEW(TinyXmlUnknown)(); //new TinyXmlUnknown();

	if (!clone)
		return 0;

	copyTo(clone);
	return clone;
}


TinyXmlAttributeSet::TinyXmlAttributeSet()
{
	sentinel.next = &sentinel;
	sentinel.prev = &sentinel;
}


TinyXmlAttributeSet::~TinyXmlAttributeSet()
{
	ASSERT(sentinel.next == &sentinel);
	ASSERT(sentinel.prev == &sentinel);
}


void TinyXmlAttributeSet::add(TinyXmlAttribute* addMe)
{
    #ifdef TinyXml_USE_STL
	ASSERT(!find(TinyXml_STRING(addMe->getName())));	// Shouldn't be multiply adding to the set.
	#else
	ASSERT(!find(addMe->getName()));	// Shouldn't be multiply adding to the set.
	#endif

	addMe->next = &sentinel;
	addMe->prev = sentinel.prev;

	sentinel.prev->next = addMe;
	sentinel.prev      = addMe;
}

void TinyXmlAttributeSet::remove(TinyXmlAttribute* removeMe)
{
	TinyXmlAttribute* node;

	for(node = sentinel.next; node != &sentinel; node = node->next)
	{
		if (node == removeMe)
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
			node->next = 0;
			node->prev = 0;
			return;
		}
	}
	ASSERT(0);		// we tried to remove a non-linked attribute.
}


#ifdef TinyXml_USE_STL
const TinyXmlAttribute* TinyXmlAttributeSet::find(const String& name) const
{
	for(const TinyXmlAttribute* node = sentinel.next; node != &sentinel; node = node->next)
	{
		if (node->name == name)
			return node;
	}
	return 0;
}

/*
TinyXmlAttribute*	TinyXmlAttributeSet::find(const String& name)
{
	for(TinyXmlAttribute* node = sentinel.next; node != &sentinel; node = node->next)
	{
		if (node->name == name)
			return node;
	}
	return 0;
}
*/
#endif


const TinyXmlAttribute* TinyXmlAttributeSet::find(const char* name) const
{
	for(const TinyXmlAttribute* node = sentinel.next; node != &sentinel; node = node->next)
	{
		if (strcmp(node->name.c_str(), name) == 0)
			return node;
	}
	return 0;
}

/*
TinyXmlAttribute*	TinyXmlAttributeSet::find(const char* name)
{
	for(TinyXmlAttribute* node = sentinel.next; node != &sentinel; node = node->next)
	{
		if (strcmp(node->name.c_str(), name) == 0)
			return node;
	}
	return 0;
}
*/

#ifdef TinyXml_USE_STL	
std::istream& operator>> (std::istream & in, TinyXmlNode & base)
{
	TinyXml_STRING tag;
	tag.reserve(8 * 1000);
	base.streamIn(&in, &tag);

	base.parse(tag.c_str(), 0, TinyXml_DEFAULT_ENCODING);
	return in;
}
#endif


#ifdef TinyXml_USE_STL	
std::ostream& operator<< (std::ostream & out, const TinyXmlNode & base)
{
	TinyXmlPrinter printer;
	printer.setStreamPrinting();
	base.accept(&printer);
	out << printer.Str();

	return out;
}


String& operator<< (String& out, const TinyXmlNode& base)
{
	TinyXmlPrinter printer;
	printer.setStreamPrinting();
	base.accept(&printer);
	out.append(printer.Str());

	return out;
}
#endif


TinyXmlHandle TinyXmlHandle::getFirstChild() const
{
	if (node)
	{
		TinyXmlNode* child = node->getFirstChild();
		if (child)
			return TinyXmlHandle(child);
	}
	return TinyXmlHandle(0);
}


TinyXmlHandle TinyXmlHandle::getFirstChild(const char * value) const
{
	if (node)
	{
		TinyXmlNode* child = node->getFirstChild(value);
		if (child)
			return TinyXmlHandle(child);
	}
	return TinyXmlHandle(0);
}


TinyXmlHandle TinyXmlHandle::firstChildElement() const
{
	if (node)
	{
		TinyXmlElement* child = node->firstChildElement();
		if (child)
			return TinyXmlHandle(child);
	}
	return TinyXmlHandle(0);
}


TinyXmlHandle TinyXmlHandle::firstChildElement(const char * value) const
{
	if (node)
	{
		TinyXmlElement* child = node->firstChildElement(value);
		if (child)
			return TinyXmlHandle(child);
	}
	return TinyXmlHandle(0);
}


TinyXmlHandle TinyXmlHandle::child(int count) const
{
	if (node)
	{
		int i;
		TinyXmlNode* child = node->getFirstChild();
		for (i=0;
				child && i<count;
				child = child->nextSibling(), ++i)
		{
			// nothing
		}
		if (child)
			return TinyXmlHandle(child);
	}
	return TinyXmlHandle(0);
}


TinyXmlHandle TinyXmlHandle::child(const char* value, int count) const
{
	if (node)
	{
		int i;
		TinyXmlNode* child = node->getFirstChild(value);
		for (i=0;
				child && i<count;
				child = child->nextSibling(value), ++i)
		{
			// nothing
		}
		if (child)
			return TinyXmlHandle(child);
	}
	return TinyXmlHandle(0);
}


TinyXmlHandle TinyXmlHandle::childElement(int count) const
{
	if (node)
	{
		int i;
		TinyXmlElement* child = node->firstChildElement();
		for (i=0;
				child && i<count;
				child = child->nextSiblingElement(), ++i)
		{
			// nothing
		}
		if (child)
			return TinyXmlHandle(child);
	}
	return TinyXmlHandle(0);
}


TinyXmlHandle TinyXmlHandle::childElement(const char* value, int count) const
{
	if (node)
	{
		int i;
		TinyXmlElement* child = node->firstChildElement(value);
		for (i=0;
				child && i<count;
				child = child->nextSiblingElement(value), ++i)
		{
			// nothing
		}
		if (child)
			return TinyXmlHandle(child);
	}
	return TinyXmlHandle(0);
}


bool TinyXmlPrinter::visitEnter(const TinyXmlDocument&)
{
	return true;
}

bool TinyXmlPrinter::visitExit(const TinyXmlDocument&)
{
	return true;
}

bool TinyXmlPrinter::visitEnter(const TinyXmlElement& element, const TinyXmlAttribute* firstAttribute)
{
	doIndent();
	buffer += "<";
	buffer += element.getValue();

	for(const TinyXmlAttribute* attrib = firstAttribute; attrib; attrib = attrib->getNext())
	{
		buffer += " ";
		attrib->print(0, 0, &buffer);
	}

	if (!element.getFirstChild()) 
	{
		buffer += " />";
		doLineBreak();
	}
	else 
	{
		buffer += ">";
		if (   element.getFirstChild()->toText()
			  && element.getLastChild() == element.getFirstChild()
			  && element.getFirstChild()->toText()->getCData() == false)
		{
			simpleTextPrint = true;
			// no doLineBreak()!
		}
		else
		{
			doLineBreak();
		}
	}
	++depth;	
	return true;
}


bool TinyXmlPrinter::visitExit(const TinyXmlElement& element)
{
	--depth;
	if (!element.getFirstChild()) 
	{
		// nothing.
	}
	else 
	{
		if (simpleTextPrint)
		{
			simpleTextPrint = false;
		}
		else
		{
			doIndent();
		}
		buffer += "</";
		buffer += element.getValue();
		buffer += ">";
		doLineBreak();
	}
	return true;
}


bool TinyXmlPrinter::visit(const TinyXmlText& text)
{
	if (text.getCData())
	{
		doIndent();
		buffer += "<![getCData[";
		buffer += text.getValue();
		buffer += "]]>";
		doLineBreak();
	}
	else if (simpleTextPrint)
	{
		TinyXml_STRING str;
		TinyXmlBase::encodeString(text.valueTStr(), &str);
		buffer += str;
	}
	else
	{
		doIndent();
		TinyXml_STRING str;
		TinyXmlBase::encodeString(text.valueTStr(), &str);
		buffer += str;
		doLineBreak();
	}
	return true;
}


bool TinyXmlPrinter::visit(const TinyXmlDeclaration& declaration)
{
	doIndent();
	declaration.print(0, 0, &buffer);
	doLineBreak();
	return true;
}


bool TinyXmlPrinter::visit(const TinyXmlComment& comment)
{
	doIndent();
	buffer += "<!--";
	buffer += comment.getValue();
	buffer += "-->";
	doLineBreak();
	return true;
}


bool TinyXmlPrinter::visit(const TinyXmlUnknown& unknown)
{
	doIndent();
	buffer += "<";
	buffer += unknown.getValue();
	buffer += ">";
	doLineBreak();
	return true;
}

