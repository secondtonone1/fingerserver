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


#ifndef TINYXML_INCLUDED__H
#define TINYXML_INCLUDED__H

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4530)
#pragma warning(disable : 4786)
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Help out windows:
#if defined(_DEBUG) && !defined(DEBUG)
#define DEBUG
#endif

#ifdef TinyXml_USE_STL
	#include <string>
 	#include <iostream>
	#include <sstream>
	#define TinyXml_STRING		String
#else
	#include "TinyStr.h"
	#define TinyXml_STRING		TinyXmlString
#endif

// Deprecated library function hell. Compilers want to use the
// new safe versions. This probably doesn't fully address the problem,
// but it gets closer. There are too many compilers for me to fully
// test. If you get compilation troubles, undefine TinyXml_SAFE
#define TinyXml_SAFE

#ifdef TinyXml_SAFE
	#if defined(_MSC_VER) && (_MSC_VER >= 1400)
		// Microsoft visual studio, version 2005 and higher.
		#define TinyXml_SNPRINTF _snprintf_s
		#define TinyXml_SNSCANF  _snscanf_s
		#define TinyXml_SSCANF   sscanf_s
	#elif defined(_MSC_VER) && (_MSC_VER >= 1200)
		// Microsoft visual studio, version 6 and higher.
		//#pragma message("Using _sn* functions.")
		#define TinyXml_SNPRINTF _snprintf
		#define TinyXml_SNSCANF  _snscanf
		#define TinyXml_SSCANF   sscanf
	#elif defined(__GNUC__) && (__GNUC__ >= 3)
		// GCC version 3 and higher.s
		//#warning("Using sn* functions.")
		#define TinyXml_SNPRINTF snprintf
		#define TinyXml_SNSCANF  snscanf
		#define TinyXml_SSCANF   sscanf
	#else
		#define TinyXml_SSCANF   sscanf
	#endif
#endif	

class TinyXmlDocument;
class TinyXmlElement;
class TinyXmlComment;
class TinyXmlUnknown;
class TinyXmlAttribute;
class TinyXmlText;
class TinyXmlDeclaration;
class TinyXmlParsingData;

const int TinyXml_MAJOR_VERSION = 2;
const int TinyXml_MINOR_VERSION = 5;
const int TinyXml_PATCH_VERSION = 3;

/*	Internal structure for tracking location of items 
	in the XML file.
*/
struct TinyXmlCursor
{
	TinyXmlCursor()		{ clear(); }
	void clear()		{ row = col = -1; }

	int row;	// 0 based.
	int col;	// 0 based.
};


/**
	If you call the accept() method, it requires being passed a TinyXmlVisitor
	class to handle callbacks. For nodes that contain other nodes (Document, element)
	you will get called with a visitEnter/visitExit pair. Nodes that are always leaves
	are simple called with visit().

	If you return 'true' from a visit method, recursive parsing will continue. If you return
	false, <b>no children of this node or its sibilings</b> will be Visited.

	All flavors of visit methods have a default implementation that returns 'true' (continue 
	visiting). You need to only override methods that are interesting to you.

	Generally accept() is called on the TinyXmlDocument, although all nodes suppert Visiting.

	You should never change the document from a callback.

	@sa TinyXmlNode::accept()
*/
class _PlatformExport TinyXmlVisitor
{
public:
	virtual ~TinyXmlVisitor() {}

	/// visit a document.
	virtual bool visitEnter(const TinyXmlDocument& /*doc*/)			{ return true; }
	/// visit a document.
	virtual bool visitExit(const TinyXmlDocument& /*doc*/)			{ return true; }

	/// visit an element.
	virtual bool visitEnter(const TinyXmlElement& /*element*/, const TinyXmlAttribute* /*getFirstAttribute*/)	{ return true; }
	/// visit an element.
	virtual bool visitExit(const TinyXmlElement& /*element*/)		{ return true; }

	/// visit a declaration
	virtual bool visit(const TinyXmlDeclaration& /*declaration*/)	{ return true; }
	/// visit a text node
	virtual bool visit(const TinyXmlText& /*text*/)					{ return true; }
	/// visit a comment node
	virtual bool visit(const TinyXmlComment& /*comment*/)			{ return true; }
	/// visit an unknow node
	virtual bool visit(const TinyXmlUnknown& /*unknown*/)			{ return true; }
};

// Only used by attribute::Query functions
enum 
{ 
	TinyXml_SUCCESS,
	TinyXml_NO_ATTRIBUTE,
	TinyXml_WRONG_TYPE
};


// Used by the parsing routines.
enum TinyXmlEncoding
{
	TinyXml_ENCODING_UNKNOWN,
	TinyXml_ENCODING_UTF8,
	TinyXml_ENCODING_LEGACY
};

const TinyXmlEncoding TinyXml_DEFAULT_ENCODING = TinyXml_ENCODING_UNKNOWN;

/** TinyXmlBase is a base class for every class in TinyXml.
	It does little except to establish that TinyXml classes
	can be printed and provide some utility functions.

	In XML, the document and elements can contain
	other elements and other types of nodes.

	@verbatim
	A Document can contain:	element	(container or leaf)
							Comment (leaf)
							unknown (leaf)
							Declaration(leaf)

	An element can contain:	element (container or leaf)
							Text	(leaf)
							Attributes (not on tree)
							Comment (leaf)
							unknown (leaf)

	A Decleration contains: Attributes (not on tree)
	@endverbatim
*/
class _PlatformExport TinyXmlBase
{
	friend class TinyXmlNode;
	friend class TinyXmlElement;
	friend class TinyXmlDocument;

public:
	TinyXmlBase()	:	userData(0)		{}
	virtual ~TinyXmlBase()			{}

	/**	All TinyXml classes can print themselves to a filestream
		or the string class (TinyXmlString in non-STL mode, String
		in STL mode.) Either or both cfile and str can be null.
		
		This is a formatted print, and will insert 
		tabs and newlines.
		
		(For an unformatted stream, use the << operator.)
	*/
	virtual void print(FILE* cfile, int depth) const = 0;

	/**	The world does not agree on whether white space should be kept or
		not. In order to make everyone happy, these global, static functions
		are provided to set whether or not TinyXml will condense all white space
		into a single space or not. The default is to condense. Note changing this
		value is not thread safe.
	*/
	static void setCondenseWhiteSpace(bool condense)		{ condenseWhiteSpace = condense; }

	/// Return the current white space setting.
	static bool isWhiteSpaceCondensed()						{ return condenseWhiteSpace; }

	/** Return the position, in the original source file, of this node or attribute.
		The row and column are 1-based. (That is the first row and first column is
		1,1). If the returns values are 0 or less, then the parser does not have
		a row and column value.

		Generally, the row and column value will be set when the TinyXmlDocument::Load(),
		TinyXmlDocument::loadFile(), or any TinyXmlNode::parse() is called. It will NOT be set
		when the DOM was created from operator>>.

		The values reflect the initial load. Once the DOM is modified programmatically
		(by adding or changing nodes and attributes) the new values will NOT update to
		reflect changes in the document.

		There is a minor performance cost to computing the row and column. Computation
		can be disabled if TinyXmlDocument::setTabSize() is called with 0 as the value.

		@sa TinyXmlDocument::setTabSize()
	*/
	int row() const			{ return location.row + 1; }
	int column() const		{ return location.col + 1; }	///< See row()

	void  setUserData(void* user)			{ userData = user; }	///< Set a pointer to arbitrary user data.
	void* getUserData()						{ return userData; }	///< get a pointer to arbitrary user data.
	const void* getUserData() const 		{ return userData; }	///< get a pointer to arbitrary user data.

	// Table that returs, for a given lead byte, the total number of bytes
	// in the UTF-8 sequence.
	static const int utf8ByteTable[256];

	virtual const char* parse(const char* p, 
							 TinyXmlParsingData* data, 
							 TinyXmlEncoding encoding /*= TinyXml_ENCODING_UNKNOWN */) = 0;

	/** Expands entities in a string. Note this should not contian the tag's '<', '>', etc, 
		or they will be transformed into entities!
	*/
	static void encodeString(const TinyXml_STRING& str, TinyXml_STRING* out);

	enum
	{
		TinyXml_NO_ERROR = 0,
		TinyXml_ERROR,
		TinyXml_ERROR_OPENING_FILE,
		TinyXml_ERROR_OUT_OF_MEMORY,
		TinyXml_ERROR_PARSING_ELEMENT,
		TinyXml_ERROR_FAILED_TO_READ_ELEMENT_NAME,
		TinyXml_ERROR_READING_ELEMENT_VALUE,
		TinyXml_ERROR_READING_ATTRIBUTES,
		TinyXml_ERROR_PARSING_EMPTY,
		TinyXml_ERROR_READING_END_TAG,
		TinyXml_ERROR_PARSING_UNKNOWN,
		TinyXml_ERROR_PARSING_COMMENT,
		TinyXml_ERROR_PARSING_DECLARATION,
		TinyXml_ERROR_DOCUMENT_EMPTY,
		TinyXml_ERROR_EMBEDDED_NULL,
		TinyXml_ERROR_PARSING_CDATA,
		TinyXml_ERROR_DOCUMENT_TOP_ONLY,

		TinyXml_ERROR_STRING_COUNT
	};

protected:

	static const char* skipWhiteSpace(const char*, TinyXmlEncoding encoding);
	inline static bool isWhiteSpace(char c)		
	{ 
		return (isspace((unsigned char) c) || c == '\n' || c == '\r'); 
	}
	inline static bool isWhiteSpace(int c)
	{
		if (c < 256)
			return isWhiteSpace((char) c);
		return false;	// Again, only truly correct for English/Latin...but usually works.
	}

	#ifdef TinyXml_USE_STL
	static bool	streamWhiteSpace(std::istream * in, TinyXml_STRING * tag);
	static bool streamTo(std::istream * in, int character, TinyXml_STRING * tag);
	#endif

	/*	Reads an XML name into the string provided. Returns
		a pointer just past the last character of the name,
		or 0 if the function has an error.
	*/
	static const char* readName(const char* p, TinyXml_STRING* name, TinyXmlEncoding encoding);

	/*	Reads text. Returns a pointer past the given end tag.
		Wickedly complex options, but it keeps the (sensitive) code in one place.
	*/
	static const char* readText(const char* in,				// where to start
								TinyXml_STRING* text,			// the string read
								bool ignoreWhiteSpace,		// whether to keep the white space
								const char* endTag,			// what ends this text
								bool ignoreCase,			// whether to ignore case in the end tag
								TinyXmlEncoding encoding);	// the current encoding

	// If an entity has been found, transform it into a character.
	static const char* getEntity(const char* in, char* value, int* length, TinyXmlEncoding encoding);

	// get a character, while interpreting entities.
	// The length can be from 0 to 4 bytes.
	inline static const char* getChar(const char* p, char* _value, int* length, TinyXmlEncoding encoding)
	{
		ASSERT(p);
		if (encoding == TinyXml_ENCODING_UTF8)
		{
			*length = utf8ByteTable[ *((const unsigned char*)p) ];
			ASSERT(*length >= 0 && *length < 5);
		}
		else
		{
			*length = 1;
		}

		if (*length == 1)
		{
			if (*p == '&')
				return getEntity(p, _value, length, encoding);
			*_value = *p;
			return p+1;
		}
		else if (*length)
		{
			//strncpy(_value, p, *length);	// lots of compilers don't like this function (unsafe),
												// and the null terminator isn't needed
			for(int i=0; p[i] && i<*length; ++i) {
				_value[i] = p[i];
			}
			return p + (*length);
		}
		else
		{
			// Not valid text.
			return 0;
		}
	}

	// Return true if the next characters in the stream are any of the endTag sequences.
	// Ignore case only works for english, and should only be relied on when comparing
	// to English words: stringEqual(p, "version", true) is fine.
	static bool stringEqual(const char* p,
								const char* endTag,
								bool ignoreCase,
								TinyXmlEncoding encoding);

	static const char* errorString[ TinyXml_ERROR_STRING_COUNT ];

	TinyXmlCursor location;

    /// Field containing a generic user pointer
	void*			userData;
	
	// None of these methods are reliable for any language except English.
	// Good for approximation, not great for accuracy.
	static int isAlpha(unsigned char anyByte, TinyXmlEncoding encoding);
	static int isAlphaNum(unsigned char anyByte, TinyXmlEncoding encoding);
	inline static int toLower(int v, TinyXmlEncoding encoding)
	{
		if (encoding == TinyXml_ENCODING_UTF8)
		{
			if (v < 128) return tolower(v);
			return v;
		}
		else
		{
			return tolower(v);
		}
	}
	static void convertUTF32ToUTF8(unsigned long input, char* output, int* length);

private:
	TinyXmlBase(const TinyXmlBase&);				// not implemented.
	void operator=(const TinyXmlBase& base);	// not allowed.

	struct Entity
	{
		const char*     str;
		unsigned int	strLength;
		char		    chr;
	};
	enum
	{
		NUM_ENTITY = 5,
		MAX_ENTITY_LENGTH = 6

	};
	static Entity entity[ NUM_ENTITY ];
	static bool condenseWhiteSpace;
};


/** The parent class for everything in the Document Object Model.
	(Except for attributes).
	Nodes have siblings, a parent, and children. A node can be
	in a document, or stand on its own. The type of a TinyXmlNode
	can be queried, and it can be cast to its more defined type.
*/
class _PlatformExport TinyXmlNode : public TinyXmlBase
{
	friend class TinyXmlDocument;
	friend class TinyXmlElement;

public:
	#ifdef TinyXml_USE_STL	

	    /** An input stream operator, for every class. Tolerant of newlines and
		    formatting, but doesn't expect them.
	    */
	    friend std::istream& operator >> (std::istream& in, TinyXmlNode& base);

	    /** An output stream operator, for every class. Note that this outputs
		    without any newlines or formatting, as opposed to print(), which
		    includes tabs and new lines.

		    The operator<< and operator>> are not completely symmetric. Writing
		    a node to a stream is very well defined. You'll get a nice stream
		    of output, without any extra whitespace or newlines.
		    
		    But reading is not as well defined. (As it always is.) If you create
		    a TinyXmlElement (for example) and read that from an input stream,
		    the text needs to define an element or junk will result. This is
		    true of all input streams, but it's worth keeping in mind.

		    A TinyXmlDocument will read nodes until it reads a root element, and
			all the children of that root element.
	    */	
	    friend std::ostream& operator<< (std::ostream& out, const TinyXmlNode& base);

		/// Appends the XML node or attribute to a String.
		friend String& operator<< (String& out, const TinyXmlNode& base);

	#endif

	/** The types of XML nodes supported by TinyXml. (All the
			unsupported types are picked up by UNKNOWN.)
	*/
	enum IterType
	{
		DOCUMENT,
		ELEMENT,
		COMMENT,
		UNKNOWN,
		TEXT,
		DECLARATION,
		TYPECOUNT
	};

	virtual ~TinyXmlNode();

	/** The meaning of 'value' changes for the specific type of
		TinyXmlNode.
		@verbatim
		Document:	filename of the xml file
		element:	name of the element
		Comment:	the comment text
		unknown:	the tag contents
		Text:		the text string
		@endverbatim

		The subclasses will wrap this function.
	*/
	const char *getValue() const { return value.c_str (); }

    #ifdef TinyXml_USE_STL
	/** Return getValue() as a String. If you only use STL,
	    this is more efficient than calling getValue().
		Only available in STL mode.
	*/
	const String& ValueStr() const { return value; }
	#endif

	const TinyXml_STRING& valueTStr() const { return value; }

	/** Changes the value of the node. Defined as:
		@verbatim
		Document:	filename of the xml file
		element:	name of the element
		Comment:	the comment text
		unknown:	the tag contents
		Text:		the text string
		@endverbatim
	*/
	void setValue(const char * value) { value = value;}

    #ifdef TinyXml_USE_STL
	/// STL String form.
	void setValue(const String& _value)	{ value = _value; }
	#endif

	/// Delete all the children of this node. Does not affect 'this'.
	void clear();

	/// One step up the DOM.
	TinyXmlNode* getParent()							{ return parent; }
	const TinyXmlNode* getParent() const				{ return parent; }

	const TinyXmlNode* getFirstChild()	const		{ return firstChild; }	///< The first child of this node. Will be null if there are no children.
	TinyXmlNode* getFirstChild()						{ return firstChild; }
	const TinyXmlNode* getFirstChild(const char * value) const;			///< The first child of this node with the matching 'value'. Will be null if none found.
	/// The first child of this node with the matching 'value'. Will be null if none found.
	TinyXmlNode* getFirstChild(const char * _value) {
		// Call through to the const version - safe since nothing is changed. Exiting syntax: cast this to a const (always safe)
		// call the method, cast the return back to non-const.
		return const_cast< TinyXmlNode* > ((const_cast< const TinyXmlNode* >(this))->getFirstChild(_value));
	}
	const TinyXmlNode* getLastChild() const	{ return lastChild; }		/// The last child of this node. Will be null if there are no children.
	TinyXmlNode* getLastChild()	{ return lastChild; }
	
	const TinyXmlNode* getLastChild(const char * value) const;			/// The last child of this node matching 'value'. Will be null if there are no children.
	TinyXmlNode* getLastChild(const char * _value) {
		return const_cast< TinyXmlNode* > ((const_cast< const TinyXmlNode* >(this))->getLastChild(_value));
	}

    #ifdef TinyXml_USE_STL
	const TinyXmlNode* getFirstChild(const String& _value) const	{	return firstChild (_value.c_str ());	}	///< STL String form.
	TinyXmlNode* getFirstChild(const String& _value)				{	return firstChild (_value.c_str ());	}	///< STL String form.
	const TinyXmlNode* getLastChild(const String& _value) const	{	return lastChild (_value.c_str ());	}	///< STL String form.
	TinyXmlNode* getLastChild(const String& _value)				{	return lastChild (_value.c_str ());	}	///< STL String form.
	#endif

	/** An alternate way to walk the children of a node.
		One way to iterate over nodes is:
		@verbatim
			for(child = parent->getFirstChild(); child; child = child->nextSibling())
		@endverbatim

		iterateChildren does the same thing with the syntax:
		@verbatim
			child = 0;
			while(child = parent->iterateChildren(child))
		@endverbatim

		iterateChildren takes the previous child as input and finds
		the next one. If the previous child is null, it returns the
		first. iterateChildren will return null when done.
	*/
	const TinyXmlNode* iterateChildren(const TinyXmlNode* previous) const;
	TinyXmlNode* iterateChildren(const TinyXmlNode* previous) {
		return const_cast< TinyXmlNode* >((const_cast< const TinyXmlNode* >(this))->iterateChildren(previous));
	}

	/// This flavor of iterateChildren searches for children with a particular 'value'
	const TinyXmlNode* iterateChildren(const char * value, const TinyXmlNode* previous) const;
	TinyXmlNode* iterateChildren(const char * _value, const TinyXmlNode* previous) {
		return const_cast< TinyXmlNode* >((const_cast< const TinyXmlNode* >(this))->iterateChildren(_value, previous));
	}

    #ifdef TinyXml_USE_STL
	const TinyXmlNode* iterateChildren(const String& _value, const TinyXmlNode* previous) const	{	return iterateChildren (_value.c_str (), previous);	}	///< STL String form.
	TinyXmlNode* iterateChildren(const String& _value, const TinyXmlNode* previous) {	return iterateChildren (_value.c_str (), previous);	}	///< STL String form.
	#endif

	/** add a new node related to this. Adds a child past the lastChild.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	TinyXmlNode* insertEndChild(const TinyXmlNode& addThis);


	/** add a new node related to this. Adds a child past the lastChild.

		NOTE: the node to be added is passed by pointer, and will be
		henceforth owned (and deleted) by tinyXml. This method is efficient
		and avoids an extra copy, but should be used with care as it
		uses a different memory model than the other insert functions.

		@sa insertEndChild
	*/
	TinyXmlNode* linkEndChild(TinyXmlNode* addThis);

	/** add a new node related to this. Adds a child before the specified child.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	TinyXmlNode* insertBeforeChild(TinyXmlNode* beforeThis, const TinyXmlNode& addThis);

	/** add a new node related to this. Adds a child after the specified child.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	TinyXmlNode* insertAfterChild(TinyXmlNode* afterThis, const TinyXmlNode& addThis);

	/** Replace a child of this node.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	TinyXmlNode* replaceChild(TinyXmlNode* replaceThis, const TinyXmlNode& withThis);

	/// Delete a child of this node.
	bool removeChild(TinyXmlNode* removeThis);

	/// Navigate to a sibling node.
	const TinyXmlNode* previousSibling() const			{ return prev; }
	TinyXmlNode* previousSibling()						{ return prev; }

	/// Navigate to a sibling node.
	const TinyXmlNode* previousSibling(const char *) const;
	TinyXmlNode* previousSibling(const char *_prev) {
		return const_cast< TinyXmlNode* >((const_cast< const TinyXmlNode* >(this))->previousSibling(_prev));
	}

    #ifdef TinyXml_USE_STL
	const TinyXmlNode* previousSibling(const String& _value) const	{	return previousSibling (_value.c_str ());	}	///< STL String form.
	TinyXmlNode* previousSibling(const String& _value) 			{	return previousSibling (_value.c_str ());	}	///< STL String form.
	const TinyXmlNode* nextSibling(const String& _value) const		{	return nextSibling (_value.c_str ());	}	///< STL String form.
	TinyXmlNode* nextSibling(const String& _value) 					{	return nextSibling (_value.c_str ());	}	///< STL String form.
	#endif

	/// Navigate to a sibling node.
	const TinyXmlNode* nextSibling() const				{ return next; }
	TinyXmlNode* nextSibling()							{ return next; }

	/// Navigate to a sibling node with the given 'value'.
	const TinyXmlNode* nextSibling(const char *) const;
	TinyXmlNode* nextSibling(const char* _next) {
		return const_cast< TinyXmlNode* >((const_cast< const TinyXmlNode* >(this))->nextSibling(_next));
	}

	/** Convenience function to get through elements.
		Calls nextSibling and toElement. Will skip all non-element
		nodes. Returns 0 if there is not another element.
	*/
	const TinyXmlElement* nextSiblingElement() const;
	TinyXmlElement* nextSiblingElement() {
		return const_cast< TinyXmlElement* >((const_cast< const TinyXmlNode* >(this))->nextSiblingElement());
	}

	/** Convenience function to get through elements.
		Calls nextSibling and toElement. Will skip all non-element
		nodes. Returns 0 if there is not another element.
	*/
	const TinyXmlElement* nextSiblingElement(const char *) const;
	TinyXmlElement* nextSiblingElement(const char *_next) {
		return const_cast< TinyXmlElement* >((const_cast< const TinyXmlNode* >(this))->nextSiblingElement(_next));
	}

    #ifdef TinyXml_USE_STL
	const TinyXmlElement* nextSiblingElement(const String& _value) const	{	return nextSiblingElement (_value.c_str ());	}	///< STL String form.
	TinyXmlElement* nextSiblingElement(const String& _value)				{	return nextSiblingElement (_value.c_str ());	}	///< STL String form.
	#endif

	/// Convenience function to get through elements.
	const TinyXmlElement* firstChildElement()	const;
	TinyXmlElement* firstChildElement() {
		return const_cast< TinyXmlElement* >((const_cast< const TinyXmlNode* >(this))->firstChildElement());
	}

	/// Convenience function to get through elements.
	const TinyXmlElement* firstChildElement(const char * _value) const;
	TinyXmlElement* firstChildElement(const char * _value) {
		return const_cast< TinyXmlElement* >((const_cast< const TinyXmlNode* >(this))->firstChildElement(_value));
	}

    #ifdef TinyXml_USE_STL
	const TinyXmlElement* firstChildElement(const String& _value) const	{	return firstChildElement (_value.c_str ());	}	///< STL String form.
	TinyXmlElement* firstChildElement(const String& _value)				{	return firstChildElement (_value.c_str ());	}	///< STL String form.
	#endif

	/** Query the type (as an enumerated value, above) of this node.
		The possible types are: DOCUMENT, ELEMENT, COMMENT,
								UNKNOWN, TEXT, and DECLARATION.
	*/
	int Type() const	{ return type; }

	/** Return a pointer to the Document this node lives in.
		Returns null if not in a document.
	*/
	const TinyXmlDocument* getDocument() const;
	TinyXmlDocument* getDocument() {
		return const_cast< TinyXmlDocument* >((const_cast< const TinyXmlNode* >(this))->getDocument());
	}

	/// Returns true if this node has no children.
	bool NoChildren() const						{ return !firstChild; }

	virtual const TinyXmlDocument*    toDocument()    const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const TinyXmlElement*     toElement()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const TinyXmlComment*     toComment()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const TinyXmlUnknown*     toUnknown()     const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const TinyXmlText*        toText()        const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const TinyXmlDeclaration* toDeclaration() const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.

	virtual TinyXmlDocument*          toDocument()    { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual TinyXmlElement*           toElement()	  { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual TinyXmlComment*           toComment()     { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual TinyXmlUnknown*           toUnknown()	  { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual TinyXmlText*	          toText()        { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual TinyXmlDeclaration*       toDeclaration() { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.

	/** create an exact duplicate of this node and return it. The memory must be deleted
		by the caller. 
	*/
	virtual TinyXmlNode* clone() const = 0;

	/** accept a hierchical visit the nodes in the TinyXML DOM. Every node in the 
		XML tree will be conditionally visited and the host will be called back
		via the TinyXmlVisitor interface.

		This is essentially a SAX interface for TinyXML. (Note however it doesn't re-parse
		the XML for the callbacks, so the performance of TinyXML is unchanged by using this
		interface versus any other.)

		The interface has been based on ideas from:

		- http://www.saxproject.org/
		- http://c2.com/cgi/wiki?HierarchicalVisitorPattern 

		Which are both good references for "visiting".

		An example of using accept():
		@verbatim
		TinyXmlPrinter printer;
		tinyxmlDoc.accept(&printer);
		const char* xmlcstr = printer.c_str();
		@endverbatim
	*/
	virtual bool accept(TinyXmlVisitor* visitor) const = 0;

protected:
	TinyXmlNode(IterType _type);

	// Copy to the allocated object. Shared functionality between clone, Copy constructor,
	// and the assignment operator.
	void copyTo(TinyXmlNode* target) const;

	#ifdef TinyXml_USE_STL
	    // The real work of the input operator.
	virtual void streamIn(std::istream* in, TinyXml_STRING* tag) = 0;
	#endif

	// Figure out what is at *p, and parse it. Returns null if it is not an xml node.
	TinyXmlNode* identify(const char* start, TinyXmlEncoding encoding);

	TinyXmlNode*		parent;
	IterType		    type;

	TinyXmlNode*		firstChild;
	TinyXmlNode*		lastChild;

	TinyXml_STRING	    value;

	TinyXmlNode*		prev;
	TinyXmlNode*		next;

private:
	TinyXmlNode(const TinyXmlNode&);				// not implemented.
	void operator=(const TinyXmlNode& base);	// not allowed.
};


/** An attribute is a name-value pair. Elements have an arbitrary
	number of attributes, each with a unique name.

	@note The attributes are not TinyXmlNodes, since they are not
		  part of the tinyXML document object model. There are other
		  suggested ways to look at this problem.
*/
class _PlatformExport TinyXmlAttribute : public TinyXmlBase
{
	friend class TinyXmlAttributeSet;

public:
	/// Construct an empty attribute.
	TinyXmlAttribute() : TinyXmlBase()
	{
		document = 0;
		prev = next = 0;
	}

	#ifdef TinyXml_USE_STL
	/// String constructor.
	TinyXmlAttribute(const String& _name, const String& _value)
	{
		name = _name;
		value = _value;
		document = 0;
		prev = next = 0;
	}
	#endif

	/// Construct an attribute with a name and value.
	TinyXmlAttribute(const char * _name, const char * _value)
	{
		name = _name;
		value = _value;
		document = 0;
		prev = next = 0;
	}

	const char*		getName()  const		{ return name.c_str(); }		///< Return the name of this attribute.
	const char*		getValue() const		{ return value.c_str(); }		///< Return the value of this attribute.
	#ifdef TinyXml_USE_STL
	const String& ValueStr() const	{ return value; }				///< Return the value of this attribute.
	#endif
	int				intValue() const;									///< Return the value of this attribute, converted to an integer.
	double			doubleValue() const;								///< Return the value of this attribute, converted to a double.

	// get the tinyxml string representation
	const TinyXml_STRING& nameTStr() const { return name; }

	/** queryIntValue examines the value string. It is an alternative to the
		intValue() method with richer error checking.
		If the value is an integer, it is stored in 'value' and 
		the call returns TinyXml_SUCCESS. If it is not
		an integer, it returns TinyXml_WRONG_TYPE.

		A specialized but useful call. Note that for success it returns 0,
		which is the opposite of almost all other TinyXml calls.
	*/
	int queryIntValue(int* _value) const;
	/// queryDoubleValue examines the value string. See queryIntValue().
	int queryDoubleValue(double* _value) const;

	void setName(const char* _name)	{ name = _name; }				///< Set the name of this attribute.
	void setValue(const char* _value)	{ value = _value; }				///< Set the value.

	void setIntValue(int _value);										///< Set the value from an integer.
	void setDoubleValue(double _value);								///< Set the value from a double.

    #ifdef TinyXml_USE_STL
	/// STL String form.
	void setName(const String& _name)	{ name = _name; }	
	/// STL String form.	
	void setValue(const String& _value)	{ value = _value; }
	#endif

	/// get the next sibling attribute in the DOM. Returns null at end.
	const TinyXmlAttribute* getNext() const;
	TinyXmlAttribute* getNext() {
		return const_cast< TinyXmlAttribute* >((const_cast< const TinyXmlAttribute* >(this))->getNext()); 
	}

	/// get the previous sibling attribute in the DOM. Returns null at beginning.
	const TinyXmlAttribute* previous() const;
	TinyXmlAttribute* previous() {
		return const_cast< TinyXmlAttribute* >((const_cast< const TinyXmlAttribute* >(this))->previous()); 
	}

	bool operator==(const TinyXmlAttribute& rhs) const { return rhs.name == name; }
	bool operator<(const TinyXmlAttribute& rhs)	 const { return name < rhs.name; }
	bool operator>(const TinyXmlAttribute& rhs)  const { return name > rhs.name; }

	/*	attribute parsing starts: first letter of the name
						 returns: the next char after the value end quote
	*/
	virtual const char* parse(const char* p, TinyXmlParsingData* data, TinyXmlEncoding encoding);

	// Prints this attribute to a FILE stream.
	virtual void print(FILE* cfile, int depth) const {
		print(cfile, depth, 0);
	}
	void print(FILE* cfile, int depth, TinyXml_STRING* str) const;

	// [internal use]
	// Set the document pointer so the attribute can report errors.
	void setDocument(TinyXmlDocument* doc)	{ document = doc; }

private:
	TinyXmlAttribute(const TinyXmlAttribute&);				// not implemented.
	void operator=(const TinyXmlAttribute& base);	// not allowed.

	TinyXmlDocument*	document;	// A pointer back to a document, for error reporting.
	TinyXml_STRING      name;
	TinyXml_STRING      value;
	TinyXmlAttribute*	prev;
	TinyXmlAttribute*	next;
};


/*	A class used to manage a group of attributes.
	It is only used internally, both by the ELEMENT and the DECLARATION.
	
	The set can be changed transparent to the element and Declaration
	classes that use it, but NOT transparent to the attribute
	which has to implement a getNext() and previous() method. Which makes
	it a bit problematic and prevents the use of STL.

	This version is implemented with circular lists because:
		- I like circular lists
		- it demonstrates some independence from the (typical) doubly linked list.
*/
class _PlatformExport TinyXmlAttributeSet
{
public:
	TinyXmlAttributeSet();
	~TinyXmlAttributeSet();

	void add(TinyXmlAttribute* attribute);
	void remove(TinyXmlAttribute* attribute);

	const TinyXmlAttribute* first()	const	{ return (sentinel.next == &sentinel) ? 0 : sentinel.next; }
	TinyXmlAttribute* first()					{ return (sentinel.next == &sentinel) ? 0 : sentinel.next; }
	const TinyXmlAttribute* last() const		{ return (sentinel.prev == &sentinel) ? 0 : sentinel.prev; }
	TinyXmlAttribute* last()					{ return (sentinel.prev == &sentinel) ? 0 : sentinel.prev; }

	const TinyXmlAttribute*	find(const char* _name) const;
	TinyXmlAttribute*	find(const char* _name) {
		return const_cast< TinyXmlAttribute* >((const_cast< const TinyXmlAttributeSet* >(this))->find(_name));
	}
	#ifdef TinyXml_USE_STL
	const TinyXmlAttribute*	find(const String& _name) const;
	TinyXmlAttribute*	find(const String& _name) {
		return const_cast< TinyXmlAttribute* >((const_cast< const TinyXmlAttributeSet* >(this))->find(_name));
	}

	#endif

private:
	//*ME:	Because of hidden/disabled copy-construktor in TinyXmlAttribute (sentinel-element),
	//*ME:	this class must be also use a hidden/disabled copy-constructor !!!
	TinyXmlAttributeSet(const TinyXmlAttributeSet&);	// not allowed
	void operator=(const TinyXmlAttributeSet&);	// not allowed (as TinyXmlAttribute)

	TinyXmlAttribute sentinel;
};


/** The element is a container class. It has a value, the element name,
	and can contain other elements, text, comments, and unknowns.
	Elements also contain an arbitrary number of attributes.
*/
class _PlatformExport TinyXmlElement : public TinyXmlNode
{
public:
	/// Construct an element.
	TinyXmlElement (const char * in_value);

	#ifdef TinyXml_USE_STL
	/// String constructor.
	TinyXmlElement(const String& _value);
	#endif

	TinyXmlElement(const TinyXmlElement&);

	void operator=(const TinyXmlElement& base);

	virtual ~TinyXmlElement();

	/** Given an attribute name, getAttribute() returns the value
		for the attribute of that name, or null if none exists.
	*/
	const char* getAttribute(const char* name) const;

	/** Given an attribute name, getAttribute() returns the value
		for the attribute of that name, or null if none exists.
		If the attribute exists and can be converted to an integer,
		the integer value will be put in the return 'i', if 'i'
		is non-null.
	*/
	const char* getAttribute(const char* name, int* i) const;

	/** Given an attribute name, getAttribute() returns the value
		for the attribute of that name, or null if none exists.
		If the attribute exists and can be converted to an double,
		the double value will be put in the return 'd', if 'd'
		is non-null.
	*/
	const char* getAttribute(const char* name, double* d) const;
    
    //edit by liu jiang [7/9/2014]
    const char* getAttribute(const char* name, bool* b) const;
    const char* getAttribute(const char* name, float* f) const;
    const char* getAttribute(const char* name, unsigned int* ui) const;

	/** queryIntAttribute examines the attribute - it is an alternative to the
		getAttribute() method with richer error checking.
		If the attribute is an integer, it is stored in 'value' and 
		the call returns TinyXml_SUCCESS. If it is not
		an integer, it returns TinyXml_WRONG_TYPE. If the attribute
		does not exist, then TinyXml_NO_ATTRIBUTE is returned.
	*/	
	int queryIntAttribute(const char* name, int* _value) const;
	/// queryDoubleAttribute examines the attribute - see queryIntAttribute().
	int queryDoubleAttribute(const char* name, double* _value) const;
	/// queryFloatAttribute examines the attribute - see queryIntAttribute().
	int queryFloatAttribute(const char* name, float* _value) const {
		double d;
		int result = queryDoubleAttribute(name, &d);
		if (result == TinyXml_SUCCESS) {
			*_value = (float)d;
		}
		return result;
	}

    #ifdef TinyXml_USE_STL
	/** Template form of the attribute query which will try to read the
		attribute into the specified type. Very easy, very powerful, but
		be careful to make sure to call this with the correct type.
		
		NOTE: This method doesn't work correctly for 'string' types.

		@return TinyXml_SUCCESS, TinyXml_WRONG_TYPE, or TinyXml_NO_ATTRIBUTE
	*/
	template< typename T > int queryValueAttribute(const String& name, T* outValue) const
	{
		const TinyXmlAttribute* node = attributeSet.find(name);
		if (!node)
			return TinyXml_NO_ATTRIBUTE;

		std::stringstream sstream(node->ValueStr());
		sstream >> *outValue;
		if (!sstream.fail())
			return TinyXml_SUCCESS;
		return TinyXml_WRONG_TYPE;
	}
	/*
	 This is - in theory - a bug fix for "QueryValueAtribute returns truncated String"
	 but template specialization is hard to get working cross-compiler. Leaving the bug for now.
	 
	// The above will fail for String because the space character is used as a seperator.
	// Specialize for strings. Bug [ 1695429 ] QueryValueAtribute returns truncated String
	template<> int queryValueAttribute(const String& name, String* outValue) const
	{
		const TinyXmlAttribute* node = attributeSet.find(name);
		if (!node)
			return TinyXml_NO_ATTRIBUTE;
		*outValue = node->ValueStr();
		return TinyXml_SUCCESS;
	}
	*/
	#endif

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void setAttribute(const char* name, const char * _value);

    #ifdef TinyXml_USE_STL
	const String* getAttribute(const String& name) const;
	const String* getAttribute(const String& name, int* i) const;
	const String* getAttribute(const String& name, double* d) const;
	int queryIntAttribute(const String& name, int* _value) const;
	int queryDoubleAttribute(const String& name, double* _value) const;

	/// STL String form.
	void setAttribute(const String& name, const String& _value);
	///< STL String form.
	void setAttribute(const String& name, int _value);
	#endif

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void setAttribute(const char * name, int value);

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void setDoubleAttribute(const char * name, double value);

    //edit by liu jiang [7/9/2014]
    void setAttribute(const char * name, float value);
    void setAttribute(const char * name, bool value);
    void setAttribute(const char * name, unsigned int value);

	/** Deletes an attribute with the given name.
	*/
	void removeAttribute(const char * name);
    #ifdef TinyXml_USE_STL
	void removeAttribute(const String& name)	{	removeAttribute (name.c_str ());	}	///< STL String form.
	#endif

	const TinyXmlAttribute* getFirstAttribute() const	{ return attributeSet.first(); }		///< Access the first attribute in this element.
	TinyXmlAttribute* getFirstAttribute() 				{ return attributeSet.first(); }
	const TinyXmlAttribute* getLastAttribute()	const 	{ return attributeSet.last(); }		///< Access the last attribute in this element.
	TinyXmlAttribute* getLastAttribute()					{ return attributeSet.last(); }

	/** Convenience function for easy access to the text inside an element. Although easy
		and concise, getText() is limited compared to getting the TinyXmlText child
		and accessing it directly.
	
		If the first child of 'this' is a TinyXmlText, the getText()
		returns the character string of the Text node, else null is returned.

		This is a convenient method for getting the text of simple contained text:
		@verbatim
		<foo>This is text</foo>
		const char* str = fooElement->getText();
		@endverbatim

		'str' will be a pointer to "This is text". 
		
		Note that this function can be misleading. If the element foo was created from
		this XML:
		@verbatim
		<foo><b>This is text</b></foo> 
		@endverbatim

		then the value of str would be null. The first child node isn't a text node, it is
		another element. From this XML:
		@verbatim
		<foo>This is <b>text</b></foo> 
		@endverbatim
		getText() will return "This is ".

		WARNING: getText() accesses a child node - don't become confused with the 
				 similarly named TinyXmlHandle::Text() and TinyXmlNode::toText() which are 
				 safe type casts on the referenced node.
	*/
	const char* getText() const;

	/// Creates a new element and returns it - the returned element is a copy.
	virtual TinyXmlNode* clone() const;
	// print the element to a FILE stream.
	virtual void print(FILE* cfile, int depth) const;

	/*	Attribtue parsing starts: next char past '<'
						 returns: next char past '>'
	*/
	virtual const char* parse(const char* p, TinyXmlParsingData* data, TinyXmlEncoding encoding);

	virtual const TinyXmlElement*     toElement()     const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual TinyXmlElement*           toElement()	          { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

	/** Walk the XML tree visiting this node and all of its children. 
	*/
	virtual bool accept(TinyXmlVisitor* visitor) const;

protected:

	void copyTo(TinyXmlElement* target) const;
	void clearThis();	// like clear, but initializes 'this' object as well

	// Used to be public [internal use]
	#ifdef TinyXml_USE_STL
	virtual void streamIn(std::istream * in, TinyXml_STRING * tag);
	#endif
	/*	[internal use]
		Reads the "value" of the element -- another element, or text.
		This should terminate with the current end tag.
	*/
	const char* readValue(const char* in, TinyXmlParsingData* prevData, TinyXmlEncoding encoding);

private:

	TinyXmlAttributeSet attributeSet;
};


/**	An XML comment.
*/
class _PlatformExport TinyXmlComment : public TinyXmlNode
{
public:
	/// Constructs an empty comment.
	TinyXmlComment() : TinyXmlNode(TinyXmlNode::COMMENT) {}
	/// Construct a comment from text.
	TinyXmlComment(const char* _value) : TinyXmlNode(TinyXmlNode::COMMENT) {
		setValue(_value);
	}
	TinyXmlComment(const TinyXmlComment&);
	void operator=(const TinyXmlComment& base);

	virtual ~TinyXmlComment()	{}

	/// Returns a copy of this Comment.
	virtual TinyXmlNode* clone() const;
	// write this Comment to a FILE stream.
	virtual void print(FILE* cfile, int depth) const;

	/*	Attribtue parsing starts: at the ! of the !--
						 returns: next char past '>'
	*/
	virtual const char* parse(const char* p, TinyXmlParsingData* data, TinyXmlEncoding encoding);

	virtual const TinyXmlComment*  toComment() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual TinyXmlComment*  toComment() { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

	/** Walk the XML tree visiting this node and all of its children. 
	*/
	virtual bool accept(TinyXmlVisitor* visitor) const;

protected:
	void copyTo(TinyXmlComment* target) const;

	// used to be public
	#ifdef TinyXml_USE_STL
	virtual void streamIn(std::istream * in, TinyXml_STRING * tag);
	#endif
//	virtual void streamOut(TinyXml_OSTREAM * out) const;

private:

};


/** XML text. A text node can have 2 ways to output the next. "normal" output 
	and getCData. It will default to the mode it was parsed from the XML file and
	you generally want to leave it alone, but you can change the output mode with 
	setCData() and query it with getCData().
*/
class _PlatformExport TinyXmlText : public TinyXmlNode
{
	friend class TinyXmlElement;
public:
	/** Constructor for text element. By default, it is treated as 
		normal, encoded text. If you want it be output as a getCData text
		element, set the parameter _cdata to 'true'
	*/
	TinyXmlText (const char * initValue) : TinyXmlNode (TinyXmlNode::TEXT)
	{
		setValue(initValue);
		cdata = false;
	}
	virtual ~TinyXmlText() {}

	#ifdef TinyXml_USE_STL
	/// Constructor.
	TinyXmlText(const String& initValue) : TinyXmlNode (TinyXmlNode::TEXT)
	{
		setValue(initValue);
		cdata = false;
	}
	#endif

	TinyXmlText(const TinyXmlText& copy) : TinyXmlNode(TinyXmlNode::TEXT)	{ copy.copyTo(this); }
	void operator=(const TinyXmlText& base)							 	{ base.copyTo(this); }

	// write this text object to a FILE stream.
	virtual void print(FILE* cfile, int depth) const;

	/// Queries whether this represents text using a getCData section.
	bool getCData() const				{ return cdata; }
	/// Turns on or off a getCData representation of text.
	void setCData(bool _cdata)	{ cdata = _cdata; }

	virtual const char* parse(const char* p, TinyXmlParsingData* data, TinyXmlEncoding encoding);

	virtual const TinyXmlText* toText() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual TinyXmlText*       toText()       { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

	/** Walk the XML tree visiting this node and all of its children. 
	*/
	virtual bool accept(TinyXmlVisitor* content) const;

protected :
	///  [internal use] Creates a new element and returns it.
	virtual TinyXmlNode* clone() const;
	void copyTo(TinyXmlText* target) const;

	bool blank() const;	// returns true if all white space and new lines
	// [internal use]
	#ifdef TinyXml_USE_STL
	virtual void streamIn(std::istream * in, TinyXml_STRING * tag);
	#endif

private:
	bool cdata;			// true if this should be input and output as a getCData style text element
};


/** In correct XML the declaration is the first entry in the file.
	@verbatim
		<?xml version="1.0" standalone="yes"?>
	@endverbatim

	TinyXml will happily read or write files without a declaration,
	however. There are 3 possible attributes to the declaration:
	version, encoding, and standalone.

	Note: In this version of the code, the attributes are
	handled as special cases, not generic attributes, simply
	because there can only be at most 3 and they are always the same.
*/
class _PlatformExport TinyXmlDeclaration : public TinyXmlNode
{
public:
	/// Construct an empty declaration.
	TinyXmlDeclaration()   : TinyXmlNode(TinyXmlNode::DECLARATION) {}

#ifdef TinyXml_USE_STL
	/// Constructor.
	TinyXmlDeclaration(const String& _version,
						const String& _encoding,
						const String& _standalone);
#endif

	/// Construct.
	TinyXmlDeclaration(const char* _version,
					   const char* _encoding,
					   const char* _standalone);

	TinyXmlDeclaration(const TinyXmlDeclaration& copy);
	void operator=(const TinyXmlDeclaration& copy);

	virtual ~TinyXmlDeclaration()	{}

	/// version. Will return an empty string if none was found.
	const char *getVersion() const			{ return version.c_str (); }
	/// encoding. Will return an empty string if none was found.
	const char *getEncoding() const		{ return encoding.c_str (); }
	/// Is this a standalone document?
	const char *getStandalone() const		{ return standalone.c_str (); }

	/// Creates a copy of this Declaration and returns it.
	virtual TinyXmlNode* clone() const;
	// print this declaration to a FILE stream.
	virtual void print(FILE* cfile, int depth, TinyXml_STRING* str) const;
	virtual void print(FILE* cfile, int depth) const {
		print(cfile, depth, 0);
	}

	virtual const char* parse(const char* p, TinyXmlParsingData* data, TinyXmlEncoding encoding);

	virtual const TinyXmlDeclaration* toDeclaration() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual TinyXmlDeclaration*       toDeclaration()       { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

	/** Walk the XML tree visiting this node and all of its children. 
	*/
	virtual bool accept(TinyXmlVisitor* visitor) const;

protected:
	void copyTo(TinyXmlDeclaration* target) const;
	// used to be public
	#ifdef TinyXml_USE_STL
	virtual void streamIn(std::istream * in, TinyXml_STRING * tag);
	#endif

private:

	TinyXml_STRING version;
	TinyXml_STRING encoding;
	TinyXml_STRING standalone;
};


/** Any tag that tinyXml doesn't recognize is saved as an
	unknown. It is a tag of text, but should not be modified.
	It will be written back to the XML, unchanged, when the file
	is saved.

	DTD tags get thrown into TinyXmlUnknowns.
*/
class _PlatformExport TinyXmlUnknown : public TinyXmlNode
{
public:
	TinyXmlUnknown() : TinyXmlNode(TinyXmlNode::UNKNOWN)	{}
	virtual ~TinyXmlUnknown() {}

	TinyXmlUnknown(const TinyXmlUnknown& copy) : TinyXmlNode(TinyXmlNode::UNKNOWN)		{ copy.copyTo(this); }
	void operator=(const TinyXmlUnknown& copy)										{ copy.copyTo(this); }

	/// Creates a copy of this unknown and returns it.
	virtual TinyXmlNode* clone() const;
	// print this unknown to a FILE stream.
	virtual void print(FILE* cfile, int depth) const;

	virtual const char* parse(const char* p, TinyXmlParsingData* data, TinyXmlEncoding encoding);

	virtual const TinyXmlUnknown*     toUnknown()     const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual TinyXmlUnknown*           toUnknown()	    { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

	/** Walk the XML tree visiting this node and all of its children. 
	*/
	virtual bool accept(TinyXmlVisitor* content) const;

protected:
	void copyTo(TinyXmlUnknown* target) const;

	#ifdef TinyXml_USE_STL
	virtual void streamIn(std::istream * in, TinyXml_STRING * tag);
	#endif

private:

};


/** Always the top level node. A document binds together all the
	XML pieces. It can be saved, loaded, and printed to the screen.
	The 'value' of a document node is the xml file name.
*/
class _PlatformExport TinyXmlDocument : public TinyXmlNode
{
public:
	/// create an empty document, that has no name.
	TinyXmlDocument();
	/// create a document with a name. The name of the document is also the filename of the xml.
	TinyXmlDocument(const char * documentName);

	#ifdef TinyXml_USE_STL
	/// Constructor.
	TinyXmlDocument(const String& documentName);
	#endif

	TinyXmlDocument(const TinyXmlDocument& copy);
	void operator=(const TinyXmlDocument& copy);

	virtual ~TinyXmlDocument() {}

	/** Load a file using the current document value.
		Returns true if successful. Will delete any existing
		document data before loading.
	*/
	bool loadFile(TinyXmlEncoding encoding = TinyXml_DEFAULT_ENCODING);
	/// save a file using the current document value. Returns true if successful.
	bool saveFile() const;
	/// Load a file using the given filename. Returns true if successful.
	bool loadFile(const char * filename, TinyXmlEncoding encoding = TinyXml_DEFAULT_ENCODING);
	/// save a file using the given filename. Returns true if successful.
	bool saveFile(const char * filename) const;
	/** Load a file using the given FILE*. Returns true if successful. Note that this method
		doesn't stream - the entire object pointed at by the FILE*
		will be interpreted as an XML file. TinyXML doesn't stream in XML from the current
		file location. Streaming may be added in the future.
	*/
	bool loadFile(FILE*, TinyXmlEncoding encoding = TinyXml_DEFAULT_ENCODING);
	/// save a file using the given FILE*. Returns true if successful.
	bool saveFile(FILE*) const;

	#ifdef TinyXml_USE_STL
	bool loadFile(const String& filename, TinyXmlEncoding encoding = TinyXml_DEFAULT_ENCODING)			///< STL String version.
	{
//		StringToBuffer f(filename);
//		return (f.buffer && loadFile(f.buffer, encoding));
		return loadFile(filename.c_str(), encoding);
	}
	bool saveFile(const String& filename) const		///< STL String version.
	{
//		StringToBuffer f(filename);
//		return (f.buffer && saveFile(f.buffer));
		return saveFile(filename.c_str());
	}
	#endif

	/** parse the given null terminated block of xml data. Passing in an encoding to this
		method (either TinyXml_ENCODING_LEGACY or TinyXml_ENCODING_UTF8 will force TinyXml
		to use that encoding, regardless of what TinyXml might otherwise try to detect.
	*/
	virtual const char* parse(const char* p, TinyXmlParsingData* data = 0, TinyXmlEncoding encoding = TinyXml_DEFAULT_ENCODING);

	/** get the root element -- the only top level element -- of the document.
		In well formed XML, there should only be one. TinyXml is tolerant of
		multiple elements at the document level.
	*/
	const TinyXmlElement* rootElement() const		{ return firstChildElement(); }
	TinyXmlElement* rootElement()					{ return firstChildElement(); }

	/** If an error occurs, error will be set to true. Also,
		- The geterrorId() will contain the integer identifier of the error (not generally useful)
		- The getErrorDesc() method will return the name of the error. (very useful)
		- The errorRow() and errorCol() will return the location of the error (if known)
	*/	
	bool getError() const						{ return error; }

	/// Contains a textual (english) description of the error if one occurs.
	const char * getErrorDesc() const	{ return errorDesc.c_str (); }

	/** Generally, you probably want the error string (getErrorDesc()). But if you
		prefer the errorId, this function will fetch it.
	*/
	int geterrorId()	const				{ return errorId; }

	/** Returns the location (if known) of the error. The first column is column 1, 
		and the first row is row 1. A value of 0 means the row and column wasn't applicable
		(memory errors, for example, have no row/column) or the parser lost the error. (An
		error in the error reporting, in that case.)

		@sa setTabSize, row, column
	*/
	int errorRow() const	{ return errorLocation.row+1; }
	int errorCol() const	{ return errorLocation.col+1; }	///< The column where the error occured. See errorRow()

	/** setTabSize() allows the error reporting functions (errorRow() and errorCol())
		to report the correct values for row and column. It does not change the output
		or input in any way.
		
		By calling this method, with a tab size
		greater than 0, the row and column of each node and attribute is stored
		when the file is loaded. Very useful for tracking the DOM back in to
		the source file.

		The tab size is required for calculating the location of nodes. If not
		set, the default of 4 is used. The tabsize is set per document. Setting
		the tabsize to 0 disables row/column tracking.

		Note that row and column tracking is not supported when using operator>>.

		The tab size needs to be enabled before the parse or load. Correct usage:
		@verbatim
		TinyXmlDocument doc;
		doc.setTabSize(8);
		doc.Load("myfile.xml");
		@endverbatim

		@sa row, column
	*/
	void setTabSize(int _tabsize)		{ tabsize = _tabsize; }

	int tabSize() const	{ return tabsize; }

	/** If you have handled the error, it can be reset with this call. The error
		state is automatically cleared if you parse a new XML block.
	*/
	void clearError()						{	error = false; 
												errorId = 0; 
												errorDesc = ""; 
												errorLocation.row = errorLocation.col = 0; 
												//errorLocation.last = 0; 
											}

	/** write the document to standard out using formatted printing ("pretty print"). */
	void print() const						{ print(stdout, 0); }

	/* write the document to a string using formatted printing ("pretty print"). This
		will allocate a character array (new char[]) and return it as a pointer. The
		calling code pust call delete[] on the return char* to avoid a memory leak.
	*/
	//char* PrintToMemory() const; 

	/// print this Document to a FILE stream.
	virtual void print(FILE* cfile, int depth = 0) const;
	// [internal use]
	void setError(int err, const char* errorLocation, TinyXmlParsingData* prevData, TinyXmlEncoding encoding);

	virtual const TinyXmlDocument*    toDocument()    const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual TinyXmlDocument*          toDocument()          { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

	/** Walk the XML tree visiting this node and all of its children. 
	*/
	virtual bool accept(TinyXmlVisitor* content) const;

protected :
	// [internal use]
	virtual TinyXmlNode* clone() const;
	#ifdef TinyXml_USE_STL
	virtual void streamIn(std::istream * in, TinyXml_STRING * tag);
	#endif

private:
	void copyTo(TinyXmlDocument* target) const;

	bool error;
	int  errorId;
	TinyXml_STRING errorDesc;
	int tabsize;
	TinyXmlCursor errorLocation;
	bool useMicrosoftBOM;		// the UTF-8 BOM were found when read. Note this, and try to write.
};


/**
	A TinyXmlHandle is a class that wraps a node pointer with null checks; this is
	an incredibly useful thing. Note that TinyXmlHandle is not part of the TinyXml
	DOM structure. It is a separate utility class.

	Take an example:
	@verbatim
	<Document>
		<element attributeA = "valueA">
			<child attributeB = "value1" />
			<child attributeB = "value2" />
		</element>
	<Document>
	@endverbatim

	Assuming you want the value of "attributeB" in the 2nd "child" element, it's very 
	easy to write a *lot* of code that looks like:

	@verbatim
	TinyXmlElement* root = document.firstChildElement("Document");
	if (root)
	{
		TinyXmlElement* element = root->firstChildElement("element");
		if (element)
		{
			TinyXmlElement* child = element->firstChildElement("child");
			if (child)
			{
				TinyXmlElement* child2 = child->nextSiblingElement("child");
				if (child2)
				{
					// Finally do something useful.
	@endverbatim

	And that doesn't even cover "else" cases. TinyXmlHandle addresses the verbosity
	of such code. A TinyXmlHandle checks for null	pointers so it is perfectly safe 
	and correct to use:

	@verbatim
	TinyXmlHandle docHandle(&document);
	TinyXmlElement* child2 = docHandle.getFirstChild("Document").getFirstChild("element").child("child", 1).toElement();
	if (child2)
	{
		// do something useful
	@endverbatim

	Which is MUCH more concise and useful.

	It is also safe to copy handles - internally they are nothing more than node pointers.
	@verbatim
	TinyXmlHandle handleCopy = handle;
	@endverbatim

	What they should not be used for is iteration:

	@verbatim
	int i=0; 
	while (true)
	{
		TinyXmlElement* child = docHandle.getFirstChild("Document").getFirstChild("element").child("child", i).toElement();
		if (!child)
			break;
		// do something
		++i;
	}
	@endverbatim

	It seems reasonable, but it is in fact two embedded while loops. The child method is 
	a linear walk to find the element, so this code would iterate much more than it needs 
	to. Instead, prefer:

	@verbatim
	TinyXmlElement* child = docHandle.getFirstChild("Document").getFirstChild("element").getFirstChild("child").toElement();

	for(child; child; child=child->nextSiblingElement())
	{
		// do something
	}
	@endverbatim
*/
class _PlatformExport TinyXmlHandle
{
public:
	/// create a handle from any node (at any depth of the tree.) This can be a null pointer.
	TinyXmlHandle(TinyXmlNode* _node)					{ this->node = _node; }
	/// Copy constructor
	TinyXmlHandle(const TinyXmlHandle& ref)			{ this->node = ref.node; }
	TinyXmlHandle operator=(const TinyXmlHandle& ref) { this->node = ref.node; return *this; }

	/// Return a handle to the first child node.
	TinyXmlHandle getFirstChild() const;
	/// Return a handle to the first child node with the given name.
	TinyXmlHandle getFirstChild(const char * value) const;
	/// Return a handle to the first child element.
	TinyXmlHandle firstChildElement() const;
	/// Return a handle to the first child element with the given name.
	TinyXmlHandle firstChildElement(const char * value) const;

	/** Return a handle to the "index" child with the given name. 
		The first child is 0, the second 1, etc.
	*/
	TinyXmlHandle child(const char* value, int index) const;
	/** Return a handle to the "index" child. 
		The first child is 0, the second 1, etc.
	*/
	TinyXmlHandle child(int index) const;
	/** Return a handle to the "index" child element with the given name. 
		The first child element is 0, the second 1, etc. Note that only TinyXmlElements
		are indexed: other types are not counted.
	*/
	TinyXmlHandle childElement(const char* value, int index) const;
	/** Return a handle to the "index" child element. 
		The first child element is 0, the second 1, etc. Note that only TinyXmlElements
		are indexed: other types are not counted.
	*/
	TinyXmlHandle childElement(int index) const;

	#ifdef TinyXml_USE_STL
	TinyXmlHandle getFirstChild(const String& _value) const				{ return getFirstChild(_value.c_str()); }
	TinyXmlHandle firstChildElement(const String& _value) const		{ return firstChildElement(_value.c_str()); }

	TinyXmlHandle child(const String& _value, int index) const			{ return child(_value.c_str(), index); }
	TinyXmlHandle childElement(const String& _value, int index) const	{ return childElement(_value.c_str(), index); }
	#endif

	/** Return the handle as a TinyXmlNode. This may return null.
	*/
	TinyXmlNode* toNode() const			{ return node; } 
	/** Return the handle as a TinyXmlElement. This may return null.
	*/
	TinyXmlElement* toElement() const		{ return ((node && node->toElement()) ? node->toElement() : 0); }
	/**	Return the handle as a TinyXmlText. This may return null.
	*/
	TinyXmlText* toText() const			{ return ((node && node->toText()) ? node->toText() : 0); }
	/** Return the handle as a TinyXmlUnknown. This may return null.
	*/
	TinyXmlUnknown* toUnknown() const		{ return ((node && node->toUnknown()) ? node->toUnknown() : 0); }

	/** @deprecated use toNode. 
		Return the handle as a TinyXmlNode. This may return null.
	*/
	TinyXmlNode* getNode() const			{ return toNode(); } 
	/** @deprecated use toElement. 
		Return the handle as a TinyXmlElement. This may return null.
	*/
	TinyXmlElement* element() const	{ return toElement(); }
	/**	@deprecated use toText()
		Return the handle as a TinyXmlText. This may return null.
	*/
	TinyXmlText* Text() const			{ return toText(); }
	/** @deprecated use toUnknown()
		Return the handle as a TinyXmlUnknown. This may return null.
	*/
	TinyXmlUnknown* unknown() const	{ return toUnknown(); }

private:
	TinyXmlNode* node;
};


/** print to memory functionality. The TinyXmlPrinter is useful when you need to:

	-# print to memory (especially in non-STL mode)
	-# Control formatting (line endings, etc.)

	When constructed, the TinyXmlPrinter is in its default "pretty printing" mode.
	Before calling accept() you can call methods to control the printing
	of the XML document. After TinyXmlNode::accept() is called, the printed document can
	be accessed via the c_str(), Str(), and size() methods.

	TinyXmlPrinter uses the Visitor API.
	@verbatim
	TinyXmlPrinter printer;
	printer.setIndent("\t");

	doc.accept(&printer);
	fprintf(stdout, "%s", printer.c_str());
	@endverbatim
*/
class _PlatformExport TinyXmlPrinter : public TinyXmlVisitor
{
public:
	TinyXmlPrinter() : depth(0), simpleTextPrint(false),
					 buffer(), indent("    "), lineBreak("\n") {}

	virtual bool visitEnter(const TinyXmlDocument& doc);
	virtual bool visitExit(const TinyXmlDocument& doc);

	virtual bool visitEnter(const TinyXmlElement& element, const TinyXmlAttribute* getFirstAttribute);
	virtual bool visitExit(const TinyXmlElement& element);

	virtual bool visit(const TinyXmlDeclaration& declaration);
	virtual bool visit(const TinyXmlText& text);
	virtual bool visit(const TinyXmlComment& comment);
	virtual bool visit(const TinyXmlUnknown& unknown);

	/** Set the indent characters for printing. By default 4 spaces
		but tab (\t) is also useful, or null/empty string for no indentation.
	*/
	void setIndent(const char* _indent)			{ indent = _indent ? _indent : "" ; }
	/// Query the indention string.
	const char* getIndent()							{ return indent.c_str(); }
	/** Set the line breaking string. By default set to newline (\n). 
		Some operating systems prefer other characters, or can be
		set to the null/empty string for no indenation.
	*/
	void setLineBreak(const char* _lineBreak)		{ lineBreak = _lineBreak ? _lineBreak : ""; }
	/// Query the current line breaking string.
	const char* getLineBreak()							{ return lineBreak.c_str(); }

	/** Switch over to "stream printing" which is the most dense formatting without 
		linebreaks. Common when the XML is needed for network transmission.
	*/
	void setStreamPrinting()						{ indent = "";
													  lineBreak = "";
													}	
	/// Return the result.
	const char* c_str()								{ return buffer.c_str(); }
	/// Return the length of the result string.
	size_t size()									{ return buffer.size(); }

	#ifdef TinyXml_USE_STL
	/// Return the result.
	const String& Str()						{ return buffer; }
	#endif

private:
	void doIndent()	{
		for(int i=0; i<depth; ++i)
			buffer += indent;
	}
	void doLineBreak() {
		buffer += lineBreak;
	}

	int depth;
	bool simpleTextPrint;
	TinyXml_STRING buffer;
	TinyXml_STRING indent;
	TinyXml_STRING lineBreak;
};


#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif

