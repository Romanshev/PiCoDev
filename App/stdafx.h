// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>


// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h> // for vsprintf_s in useful.cpp
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>// ceil dpi
#include <Psapi.h> // for GetFileNameFromHandle in useful.cpp
//#include <objbase.h> // wish initializes the COM library
				   // Specific for DemoApp v.6.0 v.6.10
#include "Uxtheme.h"  // we simply set null theme for PinaBar					
				// Libs
#pragma comment(lib,"UxTheme")//
#pragma comment(lib,"Msimg32") 
#pragma comment(lib,"ComCtl32") 
 

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <shellapi.h> // wish to call IE?