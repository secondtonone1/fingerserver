#ifndef __LYNX_AVATAR_EDITOR_XL_INC_H__
#define __LYNX_AVATAR_EDITOR_XL_INC_H__

#pragma warning (disable:4996)
#pragma warning (disable:4005)

#ifndef ENOMEM
#define ENOMEM 1
#endif

#ifndef EINVAL
#define EINVAL 22
#endif

#ifndef ERANGE
#define ERANGE 3
#endif

#ifndef STRUNCATE
#define STRUNCATE 80
#endif

#include <windows.h>

#include <atlbase.h>
#include <atlapp.h>

#include <algorithm>
#include <atlmisc.h>
#include <atlcom.h>
#include <atlhost.h>
#include <atlwin.h>
#include <atlctl.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#pragma warning(disable: 4067)

#import "..\..\TableExport\Bin\MSO.DLL" rename("RGB", "MSRGB")
#import "..\..\TableExport\Bin\VBE6EXT.OLB" \
	raw_interfaces_only, rename("Reference", "ignorethis"), rename("VBE", "GREATWSVBE")
#import "..\..\TableExport\Bin\excel.exe" \
    exclude("IFont", "IPicture") rename("RGB", "ignorethis"), rename("DialogBox", "ignorethis"), rename("VBE", "GREATWSVBE")

#endif //__LYNX_AVATAR_EDITOR_XL_INC_H__
