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

#include "TinyXml.h"

// The goal of the seperate error file is to make the first
// step towards localization. tinyxml (currently) only supports
// english error messages, but the could now be translated.
//
// It also cleans up the code a bit.
//

const char* TinyXmlBase::errorString[ TinyXml_ERROR_STRING_COUNT ] =
{
	"No error",
	"error",
	"Failed to open file",
	"Memory allocation failed.",
	"error parsing element.",
	"Failed to read element name",
	"error reading element value.",
	"error reading Attributes.",
	"error: empty tag.",
	"error reading end tag.",
	"error parsing unknown.",
	"error parsing Comment.",
	"error parsing Declaration.",
	"error document empty.",
	"error null (0) or unexpected EOF found in input stream.",
	"error parsing getCData.",
	"error when TinyXmlDocument added to document, because TinyXmlDocument can only be at the root.",
};
