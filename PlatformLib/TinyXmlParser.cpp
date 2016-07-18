/*
www.sourceforge.net/projects/tinyxml
Original code (2.0 and earlier)copyright (c) 2000-2002 Lee Thomason (www.grinninglizard.com)

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
#include <stddef.h>

#include "TinyXml.h"
#include "MemoryManager.h"

//#define DEBUG_PARSER
#if defined(DEBUG_PARSER)
#	if defined(DEBUG) && defined(_MSC_VER)
#		include <windows.h>
#		define TinyXml_LOG OutputDebugString
#	else
#		define TinyXml_LOG printf
#	endif
#endif

// Note tha "PutString" hardcodes the same list. This
// is less flexible than it appears. Changing the entries
// or order will break putstring.	
TinyXmlBase::Entity TinyXmlBase::entity[ NUM_ENTITY ] = 
{
	{ "&amp;",  5, '&' },
	{ "&lt;",   4, '<' },
	{ "&gt;",   4, '>' },
	{ "&quot;", 6, '\"' },
	{ "&apos;", 6, '\'' }
};

// Bunch of unicode info at:
//		http://www.unicode.org/faq/utf_bom.html
// Including the basic of this table, which determines the #bytes in the
// sequence from the lead byte. 1 placed for invalid sequences --
// although the result will be junk, pass it through as much as possible.
// Beware of the non-characters in UTF-8:	
//				ef bb bf (Microsoft "lead bytes")
//				ef bf be
//				ef bf bf 

const unsigned char TinyXml_UTF_LEAD_0 = 0xefU;
const unsigned char TinyXml_UTF_LEAD_1 = 0xbbU;
const unsigned char TinyXml_UTF_LEAD_2 = 0xbfU;

const int TinyXmlBase::utf8ByteTable[256] = 
{
	//	0	1	2	3	4	5	6	7	8	9	a	b	c	d	e	f
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x00
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x10
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x20
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x30
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x40
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x50
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x60
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x70	end of ASCII range
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x80 0x80 to 0xc1 invalid
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x90 
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0xa0 
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0xb0 
		1,	1,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	// 0xc0 0xc2 to 0xdf 2 byte
		2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	// 0xd0
		3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	// 0xe0 0xe0 to 0xef 3 byte
		4,	4,	4,	4,	4,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1	// 0xf0 0xf0 to 0xf4 4 byte, 0xf5 and higher invalid
};


void TinyXmlBase::convertUTF32ToUTF8(unsigned long input, char* output, int* length)
{
	const unsigned long BYTE_MASK = 0xBF;
	const unsigned long BYTE_MARK = 0x80;
	const unsigned long FIRST_BYTE_MARK[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

	if (input < 0x80) 
		*length = 1;
	else if (input < 0x800)
		*length = 2;
	else if (input < 0x10000)
		*length = 3;
	else if (input < 0x200000)
		*length = 4;
	else
		{ *length = 0; return; }	// This code won't covert this correctly anyway.

	output += *length;

	// Scary scary fall throughs.
	switch (*length) 
	{
		case 4:
			--output; 
			*output = (char)((input | BYTE_MARK) & BYTE_MASK); 
			input >>= 6;
		case 3:
			--output; 
			*output = (char)((input | BYTE_MARK) & BYTE_MASK); 
			input >>= 6;
		case 2:
			--output; 
			*output = (char)((input | BYTE_MARK) & BYTE_MASK); 
			input >>= 6;
		case 1:
			--output; 
			*output = (char)(input | FIRST_BYTE_MARK[*length]);
	}
}


/*static*/ int TinyXmlBase::isAlpha(unsigned char anyByte, TinyXmlEncoding /*encoding*/)
{
	// This will only work for low-ascii, everything else is assumed to be a valid
	// letter. I'm not sure this is the best approach, but it is quite tricky trying
	// to figure out alhabetical vs. not across encoding. So take a very 
	// conservative approach.

//	if (encoding == TinyXml_ENCODING_UTF8)
//	{
		if (anyByte < 127)
			return isalpha(anyByte);
		else
			return 1;	// What else to do? The unicode set is huge...get the english ones right.
//	}
//	else
//	{
//		return isalpha(anyByte);
//	}
}


/*static*/ int TinyXmlBase::isAlphaNum(unsigned char anyByte, TinyXmlEncoding /*encoding*/)
{
	// This will only work for low-ascii, everything else is assumed to be a valid
	// letter. I'm not sure this is the best approach, but it is quite tricky trying
	// to figure out alhabetical vs. not across encoding. So take a very 
	// conservative approach.

//	if (encoding == TinyXml_ENCODING_UTF8)
//	{
		if (anyByte < 127)
			return isalnum(anyByte);
		else
			return 1;	// What else to do? The unicode set is huge...get the english ones right.
//	}
//	else
//	{
//		return isalnum(anyByte);
//	}
}


class _PlatformExport TinyXmlParsingData
{
	friend class TinyXmlDocument;
  public:
	void getStamp(const char* now, TinyXmlEncoding encoding);

	const TinyXmlCursor& getCursor()	{ return cursor; }

  private:
	// Only used by the document!
	TinyXmlParsingData(const char* start, int _tabsize, int row, int col)
	{
		ASSERT(start);
		stamp = start;
		tabsize = _tabsize;
		cursor.row = row;
		cursor.col = col;
	}

	TinyXmlCursor	cursor;
	const char*		stamp;
	int				tabsize;
};


void TinyXmlParsingData::getStamp(const char* now, TinyXmlEncoding encoding)
{
	ASSERT(now);

	// Do nothing if the tabsize is 0.
	if (tabsize < 1)
	{
		return;
	}

	// get the current row, column.
	int row = cursor.row;
	int col = cursor.col;
	const char* p = stamp;
	ASSERT(p);

	while (p < now)
	{
		// Treat p as unsigned, so we have a happy compiler.
		const unsigned char* pU = (const unsigned char*)p;

		// Code contributed by Fletcher Dunn: (modified by lee)
		switch (*pU) {
			case 0:
				// We *should* never get here, but in case we do, don't
				// advance past the terminating null character, ever
				return;

			case '\r':
				// bump down to the next line
				++row;
				col = 0;				
				// Eat the character
				++p;

				// check for \r\n sequence, and treat this as a single character
				if (*p == '\n') {
					++p;
				}
				break;

			case '\n':
				// bump down to the next line
				++row;
				col = 0;

				// Eat the character
				++p;

				// check for \n\r sequence, and treat this as a single
				// character.  (Yes, this bizarre thing does occur still
				// on some arcane platforms...)
				if (*p == '\r') {
					++p;
				}
				break;

			case '\t':
				// Eat the character
				++p;

				// Skip to next tab stop
				col = (col / tabsize + 1) * tabsize;
				break;

			case TinyXml_UTF_LEAD_0:
				if (encoding == TinyXml_ENCODING_UTF8)
				{
					if (*(p+1) && *(p+2))
					{
						// In these cases, don't advance the column. These are
						// 0-width spaces.
						if (*(pU+1)==TinyXml_UTF_LEAD_1 && *(pU+2)==TinyXml_UTF_LEAD_2)
							p += 3;	
						else if (*(pU+1)==0xbfU && *(pU+2)==0xbeU)
							p += 3;	
						else if (*(pU+1)==0xbfU && *(pU+2)==0xbfU)
							p += 3;	
						else
							{ p +=3; ++col; }	// A normal character.
					}
				}
				else
				{
					++p;
					++col;
				}
				break;

			default:
				if (encoding == TinyXml_ENCODING_UTF8)
				{
					// Eat the 1 to 4 byte utf8 character.
					int step = TinyXmlBase::utf8ByteTable[*((const unsigned char*)p)];
					if (step == 0)
						step = 1;		// error case from bad encoding, but handle gracefully.
					p += step;

					// Just advance one column, of course.
					++col;
				}
				else
				{
					++p;
					++col;
				}
				break;
		}
	}
	cursor.row = row;
	cursor.col = col;
	ASSERT(cursor.row >= -1);
	ASSERT(cursor.col >= -1);
	stamp = p;
	ASSERT(stamp);
}


const char* TinyXmlBase::skipWhiteSpace(const char* p, TinyXmlEncoding encoding)
{
	if (!p || !*p)
	{
		return 0;
	}
	if (encoding == TinyXml_ENCODING_UTF8)
	{
		while (*p)
		{
			const unsigned char* pU = (const unsigned char*)p;
			
			// Skip the stupid Microsoft UTF-8 Byte order marks
			if (*(pU+0)==TinyXml_UTF_LEAD_0
				 && *(pU+1)==TinyXml_UTF_LEAD_1 
				 && *(pU+2)==TinyXml_UTF_LEAD_2)
			{
				p += 3;
				continue;
			}
			else if(*(pU+0)==TinyXml_UTF_LEAD_0
				 && *(pU+1)==0xbfU
				 && *(pU+2)==0xbeU)
			{
				p += 3;
				continue;
			}
			else if(*(pU+0)==TinyXml_UTF_LEAD_0
				 && *(pU+1)==0xbfU
				 && *(pU+2)==0xbfU)
			{
				p += 3;
				continue;
			}

			if (isWhiteSpace(*p) || *p == '\n' || *p =='\r')		// Still using old rules for white space.
				++p;
			else
				break;
		}
	}
	else
	{
		while (*p && isWhiteSpace(*p) || *p == '\n' || *p =='\r')
			++p;
	}

	return p;
}

#ifdef TinyXml_USE_STL
/*static*/ bool TinyXmlBase::streamWhiteSpace(std::istream * in, TinyXml_STRING * tag)
{
	for(;;)
	{
		if (!in->good()) return false;

		int c = in->peek();
		// At this scope, we can't get to a document. So fail silently.
		if (!isWhiteSpace(c) || c <= 0)
			return true;

		*tag += (char) in->get();
	}
}

/*static*/ bool TinyXmlBase::streamTo(std::istream * in, int character, TinyXml_STRING * tag)
{
	//ASSERT(character > 0 && character < 128);	// else it won't work in utf-8
	while (in->good())
	{
		int c = in->peek();
		if (c == character)
			return true;
		if (c <= 0)		// Silent failure: can't get document at this scope
			return false;

		in->get();
		*tag += (char) c;
	}
	return false;
}
#endif

// One of TinyXML's more performance demanding functions. Try to keep the memory overhead down. The
// "assign" optimization removes over 10% of the execution time.
//
const char* TinyXmlBase::readName(const char* p, TinyXml_STRING * name, TinyXmlEncoding encoding)
{
	// Oddly, not supported on some comilers,
	//name->clear();
	// So use this:
	*name = "";
	ASSERT(p);

	// Names start with letters or underscores.
	// Of course, in unicode, tinyxml has no idea what a letter *is*. The
	// algorithm is generous.
	//
	// After that, they can be letters, underscores, numbers,
	// hyphens, or colons. (Colons are valid ony for namespaces,
	// but tinyxml can't tell namespaces from names.)
	if (   p && *p 
		 && (isAlpha((unsigned char) *p, encoding) || *p == '_'))
	{
		const char* start = p;
		while(	p && *p
				&&	(	isAlphaNum((unsigned char) *p, encoding) 
						 || *p == '_'
						 || *p == '-'
						 || *p == '.'
						 || *p == ':'))
		{
			//(*name) += *p; // expensive
			++p;
		}
		if (p-start > 0) {
			name->assign(start, p-start);
		}
		return p;
	}
	return 0;
}

const char* TinyXmlBase::getEntity(const char* p, char* value, int* length, TinyXmlEncoding encoding)
{
	// Presume an entity, and pull it out.
    TinyXml_STRING ent;
	int i;
	*length = 0;

	if (*(p+1) && *(p+1) == '#' && *(p+2))
	{
		unsigned long ucs = 0;
		ptrdiff_t delta = 0;
		unsigned mult = 1;

		if (*(p+2) == 'x')
		{
			// Hexadecimal.
			if (!*(p+3)) return 0;

			const char* q = p+3;
			q = strchr(q, ';');

			if (!q || !*q) return 0;

			delta = q-p;
			--q;

			while (*q != 'x')
			{
				if (*q >= '0' && *q <= '9')
					ucs += mult * (*q - '0');
				else if (*q >= 'a' && *q <= 'f')
					ucs += mult * (*q - 'a' + 10);
				else if (*q >= 'A' && *q <= 'F')
					ucs += mult * (*q - 'A' + 10);
				else 
					return 0;
				mult *= 16;
				--q;
			}
		}
		else
		{
			// Decimal.
			if (!*(p+2)) return 0;

			const char* q = p+2;
			q = strchr(q, ';');

			if (!q || !*q) return 0;

			delta = q-p;
			--q;

			while (*q != '#')
			{
				if (*q >= '0' && *q <= '9')
					ucs += mult * (*q - '0');
				else 
					return 0;
				mult *= 10;
				--q;
			}
		}
		if (encoding == TinyXml_ENCODING_UTF8)
		{
			// convert the UCS to UTF-8
			convertUTF32ToUTF8(ucs, value, length);
		}
		else
		{
			*value = (char)ucs;
			*length = 1;
		}
		return p + delta + 1;
	}

	// Now try to match it.
	for(i=0; i<NUM_ENTITY; ++i)
	{
		if (strncmp(entity[i].str, p, entity[i].strLength) == 0)
		{
			ASSERT(strlen(entity[i].str) == entity[i].strLength);
			*value = entity[i].chr;
			*length = 1;
			return (p + entity[i].strLength);
		}
	}

	// So it wasn't an entity, its unrecognized, or something like that.
	*value = *p;	// Don't put back the last one, since we return it!
	//*length = 1;	// Leave unrecognized entities - this doesn't really work.
					// Just writes strange XML.
	return p+1;
}


bool TinyXmlBase::stringEqual(const char* p,
							 const char* tag,
							 bool ignoreCase,
							 TinyXmlEncoding encoding)
{
	ASSERT(p);
	ASSERT(tag);
	if (!p || !*p)
	{
		ASSERT(0);
		return false;
	}

	const char* q = p;

	if (ignoreCase)
	{
		while (*q && *tag && toLower(*q, encoding) == toLower(*tag, encoding))
		{
			++q;
			++tag;
		}

		if (*tag == 0)
			return true;
	}
	else
	{
		while (*q && *tag && *q == *tag)
		{
			++q;
			++tag;
		}

		if (*tag == 0)		// Have we found the end of the tag, and everything equal?
			return true;
	}
	return false;
}

const char* TinyXmlBase::readText(const char* p, 
									TinyXml_STRING * text, 
									bool trimWhiteSpace, 
									const char* endTag, 
									bool caseInsensitive,
									TinyXmlEncoding encoding)
{
    *text = "";
	if (   !trimWhiteSpace			// certain tags always keep whitespace
		 || !condenseWhiteSpace)	// if true, whitespace is always kept
	{
		// Keep all the white space.
		while (  p && *p
				&& !stringEqual(p, endTag, caseInsensitive, encoding)
			 )
		{
			int len;
			char cArr[4] = { 0, 0, 0, 0 };
			p = getChar(p, cArr, &len, encoding);
			text->append(cArr, len);
		}
	}
	else
	{
		bool whitespace = false;

		// remove leading white space:
		p = skipWhiteSpace(p, encoding);
		while (  p && *p
				&& !stringEqual(p, endTag, caseInsensitive, encoding))
		{
			if (*p == '\r' || *p == '\n')
			{
				whitespace = true;
				++p;
			}
			else if (isWhiteSpace(*p))
			{
				whitespace = true;
				++p;
			}
			else
			{
				// If we've found whitespace, add it before the
				// new character. Any whitespace just becomes a space.
				if (whitespace)
				{
					(*text) += ' ';
					whitespace = false;
				}
				int len;
				char cArr[4] = { 0, 0, 0, 0 };
				p = getChar(p, cArr, &len, encoding);
				if (len == 1)
					(*text) += cArr[0];	// more efficient
				else
					text->append(cArr, len);
			}
		}
	}
	if (p) 
		p += strlen(endTag);
	return p;
}

#ifdef TinyXml_USE_STL

void TinyXmlDocument::streamIn(std::istream * in, TinyXml_STRING * tag)
{
	// The basic issue with a document is that we don't know what we're
	// streaming. read something presumed to be a tag (and hope), then
	// identify it, and call the appropriate stream method on the tag.
	//
	// This "pre-streaming" will never read the closing ">" so the
	// sub-tag can orient itself.

	if (!streamTo(in, '<', tag)) 
	{
		setError(TinyXml_ERROR_PARSING_EMPTY, 0, 0, TinyXml_ENCODING_UNKNOWN);
		return;
	}

	while (in->good())
	{
		int tagIndex = (int) tag->length();
		while (in->good() && in->peek() != '>')
		{
			int c = in->get();
			if (c <= 0)
			{
				setError(TinyXml_ERROR_EMBEDDED_NULL, 0, 0, TinyXml_ENCODING_UNKNOWN);
				break;
			}
			(*tag) += (char) c;
		}

		if (in->good())
		{
			// We now have something we presume to be a node of 
			// some sort. identify it, and call the node to
			// continue streaming.
			TinyXmlNode* node = identify(tag->c_str() + tagIndex, TinyXml_DEFAULT_ENCODING);

			if (node)
			{
				node->streamIn(in, tag);
				bool isElement = node->toElement() != 0;
				XDELETE(node); //delete node;
				node = 0;

				// If this is the root element, we're done. Parsing will be
				// done by the >> operator.
				if (isElement)
				{
					return;
				}
			}
			else
			{
				setError(TinyXml_ERROR, 0, 0, TinyXml_ENCODING_UNKNOWN);
				return;
			}
		}
	}
	// We should have returned sooner.
	setError(TinyXml_ERROR, 0, 0, TinyXml_ENCODING_UNKNOWN);
}

#endif

const char* TinyXmlDocument::parse(const char* p, TinyXmlParsingData* prevData, TinyXmlEncoding encoding)
{
	clearError();

	// parse away, at the document level. Since a document
	// contains nothing but other tags, most of what happens
	// here is skipping white space.
	if (!p || !*p)
	{
		setError(TinyXml_ERROR_DOCUMENT_EMPTY, 0, 0, TinyXml_ENCODING_UNKNOWN);
		return 0;
	}

	// Note that, for a document, this needs to come
	// before the while space skip, so that parsing
	// starts from the pointer we are given.
	location.clear();
	if (prevData)
	{
		location.row = prevData->cursor.row;
		location.col = prevData->cursor.col;
	}
	else
	{
		location.row = 0;
		location.col = 0;
	}
	TinyXmlParsingData data(p, tabSize(), location.row, location.col);
	location = data.getCursor();

	if (encoding == TinyXml_ENCODING_UNKNOWN)
	{
		// check for the Microsoft UTF-8 lead bytes.
		const unsigned char* pU = (const unsigned char*)p;
		if (*(pU+0) && *(pU+0) == TinyXml_UTF_LEAD_0
			 && *(pU+1) && *(pU+1) == TinyXml_UTF_LEAD_1
			 && *(pU+2) && *(pU+2) == TinyXml_UTF_LEAD_2)
		{
			encoding = TinyXml_ENCODING_UTF8;
			useMicrosoftBOM = true;
		}
	}

    p = skipWhiteSpace(p, encoding);
	if (!p)
	{
		setError(TinyXml_ERROR_DOCUMENT_EMPTY, 0, 0, TinyXml_ENCODING_UNKNOWN);
		return 0;
	}

	while (p && *p)
	{
		TinyXmlNode* node = identify(p, encoding);
		if (node)
		{
			p = node->parse(p, &data, encoding);
			linkEndChild(node);
		}
		else
		{
			break;
		}

		// Did we get encoding info?
		if (   encoding == TinyXml_ENCODING_UNKNOWN
			 && node->toDeclaration())
		{
			TinyXmlDeclaration* dec = node->toDeclaration();
			const char* enc = dec->getEncoding();
			ASSERT(enc);

			if (*enc == 0)
				encoding = TinyXml_ENCODING_UTF8;
			else if (stringEqual(enc, "UTF-8", true, TinyXml_ENCODING_UNKNOWN))
				encoding = TinyXml_ENCODING_UTF8;
			else if (stringEqual(enc, "UTF8", true, TinyXml_ENCODING_UNKNOWN))
				encoding = TinyXml_ENCODING_UTF8;	// incorrect, but be nice
			else 
				encoding = TinyXml_ENCODING_LEGACY;
		}

		p = skipWhiteSpace(p, encoding);
	}

	// Was this empty?
	if (!firstChild) {
		setError(TinyXml_ERROR_DOCUMENT_EMPTY, 0, 0, encoding);
		return 0;
	}

	// All is well.
	return p;
}

void TinyXmlDocument::setError(int err, const char* pError, TinyXmlParsingData* data, TinyXmlEncoding encoding)
{	
	// The first error in a chain is more accurate - don't set again!
	if (error)
		return;

	ASSERT(err > 0 && err < TinyXml_ERROR_STRING_COUNT);
	error   = true;
	errorId = err;
	errorDesc = errorString[ errorId ];

	errorLocation.clear();
	if (pError && data)
	{
		data->getStamp(pError, encoding);
		errorLocation = data->getCursor();
	}
}


TinyXmlNode* TinyXmlNode::identify(const char* p, TinyXmlEncoding encoding)
{
	TinyXmlNode* returnNode = 0;

	p = skipWhiteSpace(p, encoding);
	if(!p || !*p || *p != '<')
	{
		return 0;
	}

	TinyXmlDocument* doc = getDocument();
	p = skipWhiteSpace(p, encoding);

	if (!p || !*p)
	{
		return 0;
	}

	// What is this thing? 
	// - Elements start with a letter or underscore, but xml is reserved.
	// - Comments: <!--
	// - Decleration: <?xml
	// - Everthing else is unknown to tinyxml.
	//

	const char* xmlHeader = { "<?xml" };
	const char* commentHeader = { "<!--" };
	const char* dtdHeader = { "<!" };
	const char* cdataHeader = { "<![getCData[" };

	if (stringEqual(p, xmlHeader, true, encoding))
	{
		#ifdef DEBUG_PARSER
			TinyXml_LOG("XML parsing Declaration\n");
		#endif
		returnNode = XNEW(TinyXmlDeclaration)(); //new TinyXmlDeclaration();
	}
	else if (stringEqual(p, commentHeader, false, encoding))
	{
		#ifdef DEBUG_PARSER
			TinyXml_LOG("XML parsing Comment\n");
		#endif
		returnNode = XNEW(TinyXmlComment)(); //new TinyXmlComment();
	}
	else if (stringEqual(p, cdataHeader, false, encoding))
	{
		#ifdef DEBUG_PARSER
			TinyXml_LOG("XML parsing getCData\n");
		#endif
		TinyXmlText* text = XNEW(TinyXmlText)(""); //new TinyXmlText("");
		text->setCData(true);
		returnNode = text;
	}
	else if (stringEqual(p, dtdHeader, false, encoding))
	{
		#ifdef DEBUG_PARSER
			TinyXml_LOG("XML parsing unknown(1)\n");
		#endif
		returnNode = XNEW(TinyXmlUnknown)(); //new TinyXmlUnknown();
	}
	else if (   isAlpha(*(p+1), encoding)
			  || *(p+1) == '_')
	{
		#ifdef DEBUG_PARSER
			TinyXml_LOG("XML parsing element\n");
		#endif
		returnNode = XNEW(TinyXmlElement)(""); //new TinyXmlElement("");
	}
	else
	{
		#ifdef DEBUG_PARSER
			TinyXml_LOG("XML parsing unknown(2)\n");
		#endif
		returnNode = XNEW(TinyXmlUnknown)(); //new TinyXmlUnknown();
	}

	if (returnNode)
	{
		// Set the parent, so it can report errors
		returnNode->parent = this;
	}
	else
	{
		if (doc)
			doc->setError(TinyXml_ERROR_OUT_OF_MEMORY, 0, 0, TinyXml_ENCODING_UNKNOWN);
	}
	return returnNode;
}

#ifdef TinyXml_USE_STL

void TinyXmlElement::streamIn (std::istream * in, TinyXml_STRING * tag)
{
	// We're called with some amount of pre-parsing. That is, some of "this"
	// element is in "tag". Go ahead and stream to the closing ">"
	while(in->good())
	{
		int c = in->get();
		if (c <= 0)
		{
			TinyXmlDocument* document = getDocument();
			if (document)
				document->setError(TinyXml_ERROR_EMBEDDED_NULL, 0, 0, TinyXml_ENCODING_UNKNOWN);
			return;
		}
		(*tag) += (char) c ;
		
		if (c == '>')
			break;
	}

	if (tag->length() < 3) return;

	// Okay...if we are a "/>" tag, then we're done. We've read a complete tag.
	// If not, identify and stream.

	if (   tag->at(tag->length() - 1) == '>' 
		 && tag->at(tag->length() - 2) == '/')
	{
		// All good!
		return;
	}
	else if (tag->at(tag->length() - 1) == '>')
	{
		// There is more. Could be:
		//		text
		//		cdata text (which looks like another node)
		//		closing tag
		//		another node.
		for (;;)
		{
			streamWhiteSpace(in, tag);

			// Do we have text?
			if (in->good() && in->peek() != '<') 
			{
				// Yep, text.
				TinyXmlText text("");
				text.streamIn(in, tag);

				// What follows text is a closing tag or another node.
				// Go around again and figure it out.
				continue;
			}

			// We now have either a closing tag...or another node.
			// We should be at a "<", regardless.
			if (!in->good()) return;
			ASSERT(in->peek() == '<');
			int tagIndex = (int) tag->length();

			bool closingTag = false;
			bool firstCharFound = false;

			for(;;)
			{
				if (!in->good())
					return;

				int c = in->peek();
				if (c <= 0)
				{
					TinyXmlDocument* document = getDocument();
					if (document)
						document->setError(TinyXml_ERROR_EMBEDDED_NULL, 0, 0, TinyXml_ENCODING_UNKNOWN);
					return;
				}
				
				if (c == '>')
					break;

				*tag += (char) c;
				in->get();

				// Early out if we find the getCData id.
				if (c == '[' && tag->size() >= 9)
				{
					size_t len = tag->size();
					const char* start = tag->c_str() + len - 9;
					if (strcmp(start, "<![getCData[") == 0) {
						ASSERT(!closingTag);
						break;
					}
				}

				if (!firstCharFound && c != '<' && !isWhiteSpace(c))
				{
					firstCharFound = true;
					if (c == '/')
						closingTag = true;
				}
			}
			// If it was a closing tag, then read in the closing '>' to clean up the input stream.
			// If it was not, the streaming will be done by the tag.
			if (closingTag)
			{
				if (!in->good())
					return;

				int c = in->get();
				if (c <= 0)
				{
					TinyXmlDocument* document = getDocument();
					if (document)
						document->setError(TinyXml_ERROR_EMBEDDED_NULL, 0, 0, TinyXml_ENCODING_UNKNOWN);
					return;
				}
				ASSERT(c == '>');
				*tag += (char) c;

				// We are done, once we've found our closing tag.
				return;
			}
			else
			{
				// If not a closing tag, id it, and stream.
				const char* tagloc = tag->c_str() + tagIndex;
				TinyXmlNode* node = identify(tagloc, TinyXml_DEFAULT_ENCODING);
				if (!node)
					return;
				node->streamIn(in, tag);
				XDELETE(node); //delete node;
				node = 0;

				// No return: go around from the beginning: text, closing tag, or node.
			}
		}
	}
}
#endif

const char* TinyXmlElement::parse(const char* p, TinyXmlParsingData* data, TinyXmlEncoding encoding)
{
	p = skipWhiteSpace(p, encoding);
	TinyXmlDocument* document = getDocument();

	if (!p || !*p)
	{
		if (document) document->setError(TinyXml_ERROR_PARSING_ELEMENT, 0, 0, encoding);
		return 0;
	}

	if (data)
	{
		data->getStamp(p, encoding);
		location = data->getCursor();
	}

	if (*p != '<')
	{
		if (document) document->setError(TinyXml_ERROR_PARSING_ELEMENT, p, data, encoding);
		return 0;
	}

	p = skipWhiteSpace(p+1, encoding);

	// read the name.
	const char* pErr = p;

    p = readName(p, &value, encoding);
	if (!p || !*p)
	{
		if (document)	document->setError(TinyXml_ERROR_FAILED_TO_READ_ELEMENT_NAME, pErr, data, encoding);
		return 0;
	}

    TinyXml_STRING endTag ("</");
	endTag += value;
	endTag += ">";

	// check for and read attributes. Also look for an empty
	// tag or an end tag.
	while (p && *p)
	{
		pErr = p;
		p = skipWhiteSpace(p, encoding);
		if (!p || !*p)
		{
			if (document) document->setError(TinyXml_ERROR_READING_ATTRIBUTES, pErr, data, encoding);
			return 0;
		}
		if (*p == '/')
		{
			++p;
			// empty tag.
			if (*p  != '>')
			{
				if (document) document->setError(TinyXml_ERROR_PARSING_EMPTY, p, data, encoding);		
				return 0;
			}
			return (p+1);
		}
		else if (*p == '>')
		{
			// Done with attributes (if there were any.)
			// read the value -- which can include other
			// elements -- read the end tag, and return.
			++p;
			p = readValue(p, data, encoding);		// Note this is an element method, and will set the error if one happens.
			if (!p || !*p) {
				// We were looking for the end tag, but found nothing.
				// Fix for [ 1663758 ] Failure to report error on bad XML
				if (document) document->setError(TinyXml_ERROR_READING_END_TAG, p, data, encoding);
				return 0;
			}

			// We should find the end tag now
			if (stringEqual(p, endTag.c_str(), false, encoding))
			{
				p += endTag.length();
				return p;
			}
			else
			{
				if (document) document->setError(TinyXml_ERROR_READING_END_TAG, p, data, encoding);
				return 0;
			}
		}
		else
		{
			// Try to read an attribute:
			TinyXmlAttribute* attrib = XNEW(TinyXmlAttribute)(); //new TinyXmlAttribute();
			if (!attrib)
			{
				if (document) document->setError(TinyXml_ERROR_OUT_OF_MEMORY, pErr, data, encoding);
				return 0;
			}

			attrib->setDocument(document);
			pErr = p;
			p = attrib->parse(p, data, encoding);

			if (!p || !*p)
			{
				if (document) document->setError(TinyXml_ERROR_PARSING_ELEMENT, pErr, data, encoding);
				XDELETE(attrib); //delete attrib;
				return 0;
			}

			// Handle the strange case of double attributes:
			#ifdef TinyXml_USE_STL
			TinyXmlAttribute* node = attributeSet.find(attrib->nameTStr());
			#else
			TinyXmlAttribute* node = attributeSet.find(attrib->getName());
			#endif
			if (node)
			{
				node->setValue(attrib->getValue());
				XDELETE(attrib); //delete attrib;
				return 0;
			}

			attributeSet.add(attrib);
		}
	}
	return p;
}


const char* TinyXmlElement::readValue(const char* p, TinyXmlParsingData* data, TinyXmlEncoding encoding)
{
	TinyXmlDocument* document = getDocument();

	// read in text and elements in any order.
	const char* pWithWhiteSpace = p;
	p = skipWhiteSpace(p, encoding);

	while (p && *p)
	{
		if (*p != '<')
		{
			// Take what we have, make a text element.
			TinyXmlText* textNode = XNEW(TinyXmlText)(""); //new TinyXmlText("");

			if (!textNode)
			{
				if (document) document->setError(TinyXml_ERROR_OUT_OF_MEMORY, 0, 0, encoding);
				    return 0;
			}

			if (TinyXmlBase::isWhiteSpaceCondensed())
			{
				p = textNode->parse(p, data, encoding);
			}
			else
			{
				// Special case: we want to keep the white space
				// so that leading spaces aren't removed.
				p = textNode->parse(pWithWhiteSpace, data, encoding);
			}

			if (!textNode->blank())
				linkEndChild(textNode);
			else
				XDELETE(textNode); //delete textNode;
		} 
		else 
		{
			// We hit a '<'
			// Have we hit a new element or an end tag? This could also be
			// a TinyXmlText in the "getCData" style.
			if (stringEqual(p, "</", false, encoding))
			{
				return p;
			}
			else
			{
				TinyXmlNode* node = identify(p, encoding);
				if (node)
				{
					p = node->parse(p, data, encoding);
					linkEndChild(node);
				}				
				else
				{
					return 0;
				}
			}
		}
		pWithWhiteSpace = p;
		p = skipWhiteSpace(p, encoding);
	}

	if (!p)
	{
		if (document) document->setError(TinyXml_ERROR_READING_ELEMENT_VALUE, 0, 0, encoding);
	}	
	return p;
}


#ifdef TinyXml_USE_STL
void TinyXmlUnknown::streamIn(std::istream * in, TinyXml_STRING * tag)
{
	while (in->good())
	{
		int c = in->get();	
		if (c <= 0)
		{
			TinyXmlDocument* document = getDocument();
			if (document)
				document->setError(TinyXml_ERROR_EMBEDDED_NULL, 0, 0, TinyXml_ENCODING_UNKNOWN);
			return;
		}
		(*tag) += (char) c;

		if (c == '>')
		{
			// All is well.
			return;		
		}
	}
}
#endif


const char* TinyXmlUnknown::parse(const char* p, TinyXmlParsingData* data, TinyXmlEncoding encoding)
{
	TinyXmlDocument* document = getDocument();
	p = skipWhiteSpace(p, encoding);

	if (data)
	{
		data->getStamp(p, encoding);
		location = data->getCursor();
	}
	if (!p || !*p || *p != '<')
	{
		if (document) document->setError(TinyXml_ERROR_PARSING_UNKNOWN, p, data, encoding);
		return 0;
	}
	++p;
    value = "";

	while (p && *p && *p != '>')
	{
		value += *p;
		++p;
	}

	if (!p)
	{
		if (document)	document->setError(TinyXml_ERROR_PARSING_UNKNOWN, 0, 0, encoding);
	}
	if (*p == '>')
		return p+1;
	return p;
}

#ifdef TinyXml_USE_STL
void TinyXmlComment::streamIn(std::istream * in, TinyXml_STRING * tag)
{
	while (in->good())
	{
		int c = in->get();	
		if (c <= 0)
		{
			TinyXmlDocument* document = getDocument();
			if (document)
				document->setError(TinyXml_ERROR_EMBEDDED_NULL, 0, 0, TinyXml_ENCODING_UNKNOWN);
			return;
		}

		(*tag) += (char) c;

		if (c == '>' 
			 && tag->at(tag->length() - 2) == '-'
			 && tag->at(tag->length() - 3) == '-')
		{
			// All is well.
			return;		
		}
	}
}
#endif


const char* TinyXmlComment::parse(const char* p, TinyXmlParsingData* data, TinyXmlEncoding encoding)
{
	TinyXmlDocument* document = getDocument();
	value = "";

	p = skipWhiteSpace(p, encoding);

	if (data)
	{
		data->getStamp(p, encoding);
		location = data->getCursor();
	}
	const char* startTag = "<!--";
	const char* endTag   = "-->";

	if (!stringEqual(p, startTag, false, encoding))
	{
		document->setError(TinyXml_ERROR_PARSING_COMMENT, p, data, encoding);
		return 0;
	}
	p += strlen(startTag);

	// [ 1475201 ] TinyXML parses entities in comments
	// Oops - readText doesn't work, because we don't want to parse the entities.
	// p = readText(p, &value, false, endTag, false, encoding);
	//
	// from the XML spec:
	/*
	 [Definition: Comments may appear anywhere in a document outside other markup; in addition, 
	              they may appear within the document type declaration at places allowed by the grammar. 
				  They are not part of the document's character data; an XML processor MAY, but need not, 
				  make it possible for an application to retrieve the text of comments. For compatibility, 
				  the string "--" (double-hyphen) MUST NOT occur within comments.] Parameter entity 
				  references MUST NOT be recognized within comments.

				  An example of a comment:

				  <!-- declarations for <head> & <body> -->
	*/

    value = "";
	// Keep all the white space.
	while (p && *p && !stringEqual(p, endTag, false, encoding))
	{
		value.append(p, 1);
		++p;
	}
	if (p) 
		p += strlen(endTag);

	return p;
}


const char* TinyXmlAttribute::parse(const char* p, TinyXmlParsingData* data, TinyXmlEncoding encoding)
{
	p = skipWhiteSpace(p, encoding);
	if (!p || !*p) return 0;

//	int tabsize = 4;
//	if (document)
//		tabsize = document->tabSize();

	if (data)
	{
		data->getStamp(p, encoding);
		location = data->getCursor();
	}
	// read the name, the '=' and the value.
	const char* pErr = p;
	p = readName(p, &name, encoding);
	if (!p || !*p)
	{
		if (document) document->setError(TinyXml_ERROR_READING_ATTRIBUTES, pErr, data, encoding);
		return 0;
	}
	p = skipWhiteSpace(p, encoding);
	if (!p || !*p || *p != '=')
	{
		if (document) document->setError(TinyXml_ERROR_READING_ATTRIBUTES, p, data, encoding);
		return 0;
	}

	++p;	// skip '='
	p = skipWhiteSpace(p, encoding);
	if (!p || !*p)
	{
		if (document) document->setError(TinyXml_ERROR_READING_ATTRIBUTES, p, data, encoding);
		return 0;
	}
	
	const char* end;
	const char SINGLE_QUOTE = '\'';
	const char DOUBLE_QUOTE = '\"';

	if (*p == SINGLE_QUOTE)
	{
		++p;
		end = "\'";		// single quote in string
		p = readText(p, &value, false, end, false, encoding);
	}
	else if (*p == DOUBLE_QUOTE)
	{
		++p;
		end = "\"";		// double quote in string
		p = readText(p, &value, false, end, false, encoding);
	}
	else
	{
		// All attribute values should be in single or double quotes.
		// But this is such a common error that the parser will try
		// its best, even without them.
		value = "";
		while (   p && *p											// existence
				&& !isWhiteSpace(*p) && *p != '\n' && *p != '\r'	// whitespace
				&& *p != '/' && *p != '>')							// tag end
		{
			if (*p == SINGLE_QUOTE || *p == DOUBLE_QUOTE) {
				// [ 1451649 ] attribute values with trailing quotes not handled correctly
				// We did not have an opening quote but seem to have a 
				// closing one. Give up and throw an error.
				if (document) document->setError(TinyXml_ERROR_READING_ATTRIBUTES, p, data, encoding);
				return 0;
			}
			value += *p;
			++p;
		}
	}
	return p;
}

#ifdef TinyXml_USE_STL
void TinyXmlText::streamIn(std::istream * in, TinyXml_STRING * tag)
{
	while (in->good())
	{
		int c = in->peek();	
		if (!cdata && (c == '<')) 
		{
			return;
		}
		if (c <= 0)
		{
			TinyXmlDocument* document = getDocument();
			if (document)
				document->setError(TinyXml_ERROR_EMBEDDED_NULL, 0, 0, TinyXml_ENCODING_UNKNOWN);
			return;
		}

		(*tag) += (char) c;
		in->get();	// "commits" the peek made above

		if (cdata && c == '>' && tag->size() >= 3) {
			size_t len = tag->size();
			if ((*tag)[len-2] == ']' && (*tag)[len-3] == ']') {
				// terminator of cdata.
				return;
			}
		}    
	}
}
#endif

const char* TinyXmlText::parse(const char* p, TinyXmlParsingData* data, TinyXmlEncoding encoding)
{
	value = "";
	TinyXmlDocument* document = getDocument();

	if (data)
	{
		data->getStamp(p, encoding);
		location = data->getCursor();
	}

	const char* const startTag = "<![getCData[";
	const char* const endTag   = "]]>";

	if (cdata || stringEqual(p, startTag, false, encoding))
	{
		cdata = true;

		if (!stringEqual(p, startTag, false, encoding))
		{
			document->setError(TinyXml_ERROR_PARSING_CDATA, p, data, encoding);
			return 0;
		}
		p += strlen(startTag);

		// Keep all the white space, ignore the encoding, etc.
		while (  p && *p
				&& !stringEqual(p, endTag, false, encoding)
			 )
		{
			value += *p;
			++p;
		}

		TinyXml_STRING dummy; 
		p = readText(p, &dummy, false, endTag, false, encoding);
		return p;
	}
	else
	{
		bool ignoreWhite = true;

		const char* end = "<";
		p = readText(p, &value, ignoreWhite, end, false, encoding);
		if (p)
			return p-1;	// don't truncate the '<'
		return 0;
	}
}

#ifdef TinyXml_USE_STL
void TinyXmlDeclaration::streamIn(std::istream * in, TinyXml_STRING * tag)
{
	while (in->good())
	{
		int c = in->get();
		if (c <= 0)
		{
			TinyXmlDocument* document = getDocument();
			if (document)
				document->setError(TinyXml_ERROR_EMBEDDED_NULL, 0, 0, TinyXml_ENCODING_UNKNOWN);
			return;
		}
		(*tag) += (char) c;

		if (c == '>')
		{
			// All is well.
			return;
		}
	}
}
#endif

const char* TinyXmlDeclaration::parse(const char* p, TinyXmlParsingData* data, TinyXmlEncoding _encoding)
{
	p = skipWhiteSpace(p, _encoding);
	// find the beginning, find the end, and look for
	// the stuff in-between.
	TinyXmlDocument* document = getDocument();
	if (!p || !*p || !stringEqual(p, "<?xml", true, _encoding))
	{
		if (document) document->setError(TinyXml_ERROR_PARSING_DECLARATION, 0, 0, _encoding);
		return 0;
	}
	if (data)
	{
		data->getStamp(p, _encoding);
		location = data->getCursor();
	}
	p += 5;

	version = "";
	encoding = "";
	standalone = "";

	while (p && *p)
	{
		if (*p == '>')
		{
			++p;
			return p;
		}

		p = skipWhiteSpace(p, _encoding);
		if (stringEqual(p, "version", true, _encoding))
		{
			TinyXmlAttribute attrib;
			p = attrib.parse(p, data, _encoding);		
			version = attrib.getValue();
		}
		else if (stringEqual(p, "encoding", true, _encoding))
		{
			TinyXmlAttribute attrib;
			p = attrib.parse(p, data, _encoding);		
			encoding = attrib.getValue();
		}
		else if (stringEqual(p, "standalone", true, _encoding))
		{
			TinyXmlAttribute attrib;
			p = attrib.parse(p, data, _encoding);		
			standalone = attrib.getValue();
		}
		else
		{
			// read over whatever it is.
			while(p && *p && *p != '>' && !isWhiteSpace(*p))
				++p;
		}
	}
	return 0;
}

bool TinyXmlText::blank() const
{
	for (unsigned i=0; i<value.length(); i++)
		if (!isWhiteSpace(value[i]))
			return false;
	return true;
}

