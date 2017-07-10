#include "stdafx.h"
#include "IoHelper.h"

static HWND hwInfo;
int nInfo = 0;

static const unsigned TabDistance = 16; // Specify the tab stops, in dialog template units.

//----------------------------------
//bool bSlowRedraw = false;
//int nSlowRedraw =10;
//----------------------------------
bool bInfoWindow = false;
bool bNoInfo = false;

/////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK InfoWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DebugInfo(const wchar_t* fmt, ...)
{
	wchar_t sbuf[1024];

	*sbuf = 0;

	va_list marker;

	va_start(marker, fmt);     //Initialize variable arguments

	vswprintf_s(sbuf, 1024, fmt, marker);

	va_end(marker);

	OutputDebugStringW(sbuf);
}
//-------------------------------------------------------------
void DebugInfo(const char* fmt, ...)
{
	char sbuf[1024];

	*sbuf = 0;

	va_list marker;

	va_start(marker, fmt);     //Initialize variable arguments

	vsprintf_s(sbuf, 1024, fmt, marker);

	va_end(marker);

	OutputDebugStringA(sbuf);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// You Must delete pointer to buffer after use it
TCHAR* StringFormat(TCHAR* format, ...)
{
	va_list args;
	size_t len;
	TCHAR* buffer;

	va_start(args, format);

	len = _vsctprintf(format, args) + 1;	// _vscprintf doesn't count terminating '\0'

	buffer = new TCHAR[len];

	_vstprintf_s(buffer, len, format, args);

	//_putts (buffer );

	return buffer;// Must be deleted  as: delete buffer
}
/////////////////////////////////////////////////////////////////////////////////////////////
// Add text strings to textbox control
void AddText(HWND hwTextBox, TCHAR* fmt, ...)
{
	TCHAR sbuf[1024];
	TCHAR sbufA[1024];
	TCHAR* pwc = sbufA;
	int n;

	va_list marker;

	va_start(marker, fmt);     //Initialize variable arguments

	_vstprintf(sbuf, fmt, marker);

	va_end(marker);

	for (n = 0; sbuf[n]; ++n)
	{
		if (sbuf[n] == '\n')
			*pwc++ = '\r';

		*pwc++ = sbuf[n];
	}
	*pwc = sbuf[n];

	SendMessage(hwTextBox, EM_REPLACESEL, FALSE, (LPARAM)sbufA);
}

/////////////////////////////////////////////////////////////////////////////////////////
void Info(TCHAR* fmt, ...)
{
	if (bNoInfo) return;

	if (!bInfoWindow)
	{
		TCHAR sbuf[1024];

		*sbuf = 0;

		va_list marker;

		va_start(marker, fmt);     //Initialize variable arguments

		_vstprintf(sbuf, fmt, marker);

		va_end(marker);

		OutputDebugString(sbuf);
		//OutputDebugString(L"\n");
	}
	else
	{
		TCHAR sbuf[1024];
		TCHAR sbufA[1024];
		TCHAR* pwc = sbufA;
		int n;

		va_list marker;

		va_start(marker, fmt);     //Initialize variable arguments

		_vstprintf(sbuf, fmt, marker);

		va_end(marker);

		for (n = 0; sbuf[n]; ++n)
		{

#ifndef _CONSOLE
			if (sbuf[n] == '\n')
				*pwc++ = '\r';
#endif
			*pwc++ = sbuf[n];
		}
		*pwc = sbuf[n];

#ifdef _CONSOLE
		_tprintf(_T("%s"), sbufA);
#else
		SendMessage(hwInfo, EM_REPLACESEL, FALSE, (LPARAM)sbufA);
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
bool Check(bool test, TCHAR* fmt, ...)
{
	if (test)
		return true;

	TCHAR sbuf[1024];
	TCHAR sbufA[1024];
	TCHAR* pwc = sbufA;
	int n;

	va_list marker;

	va_start(marker, fmt);     //Initialize variable arguments

	_vstprintf(sbuf, fmt, marker);

	va_end(marker);

	for (n = 0; sbuf[n]; ++n)
	{
#ifndef _CONSOLE
		if (sbuf[n] == '\n')
			*pwc++ = '\r';
#endif
		*pwc++ = sbuf[n];
	}
	*pwc = sbuf[n];

#ifdef _CONSOLE
	_tprintf(_T("%s"), sbufA);
#else
	SendMessage(hwInfo, EM_REPLACESEL, FALSE, (LPARAM)sbufA);
#endif

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
void ClearInfo()
{
	SendMessage(hwInfo, WM_SETTEXT, 0, (LPARAM)TEXT(""));
}

/////////////////////////////////////////////////////////////////////////////////////////
long getLongEdit(HWND hwnd)
{
	TCHAR str[256];

	SendMessage(hwnd, (UINT)WM_GETTEXT, 255, (LPARAM)str);

	return _ttol(str);
}

//////////////////// Set integer to Combo or Edit  ////////////////////////////////////
void setLongEdit(HWND hwnd, long value)
{
	TCHAR str[256];

	_ltot(value, str, 10);

	SendMessage(hwnd, (UINT)WM_SETTEXT, 0, (LPARAM)str);
}

////////////////// Get float from Combo or Edit /////////////////////////////////////////
double getDoubleEdit(HWND hwnd)
{
	TCHAR str[256];

	SendMessage(hwnd, (UINT)WM_GETTEXT, 255, (LPARAM)str);

	return _tstof(str);
}

//////////////////// Set Double to Combo or Edit  ////////////////////////////////////

void setDoubleEdit(HWND hwnd, int IDC, double value, int digits)
{
	char str[256];

	_gcvt(value, digits, str);

#ifdef _UNICODE

	wchar_t wstr[256];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstr, 256);
	SendMessage(hwnd, (UINT)WM_SETTEXT, 0, (LPARAM)wstr);

#else

	SendMessage(hwnd, (UINT)WM_SETTEXT, 0, (LPARAM)str);

#endif
}

/////////////////////////////////////////////////////////////////////////////////////////
__int64 get64Edit(HWND hwnd)
{
	TCHAR str[256];

	SendMessage(hwnd, (UINT)WM_GETTEXT, 255, (LPARAM)str);

	return _tstoi64(str);
}

//////////////////// Set integer to Combo or Edit  ////////////////////////////////////
void set64Edit(HWND hwnd, __int64 value)
{
	TCHAR str[256];

	_i64tot(value, str, 10);

	SendMessage(hwnd, (UINT)WM_SETTEXT, 0, (LPARAM)str);
}

/////////////////////////////////////////////////////////////////////////////////////////
HWND CreateInfoWindow(int left, int top, int width, int height)
{
	HWND hw;
	WNDCLASSEX  wci;
	RECT rc;
	//-------------------------------------------------------------------------------------
	wci.cbSize = sizeof(WNDCLASSEX);
	wci.style = CS_HREDRAW | CS_VREDRAW;
	wci.lpfnWndProc = (WNDPROC)InfoWinProc;
	wci.cbClsExtra = 0;
	wci.cbWndExtra = 0;
	wci.hInstance = 0;
	wci.hIcon = NULL;
	wci.hCursor = LoadCursor(NULL, IDC_ARROW);
	wci.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wci.lpszMenuName = NULL;
	wci.lpszClassName = _T("PICO_Info");
	wci.hIconSm = NULL;

	if (!RegisterClassEx(&wci) && (ERROR_CLASS_ALREADY_EXISTS != GetLastError()))
	{
		return NULL;
	}

	//-------------------------------------------------------------------------------------
	hw = CreateWindowEx(0,
		_T("PICO_Info"),
		_T("Info"),
		WS_TILEDWINDOW | WS_CLIPCHILDREN | WS_SIZEBOX | WS_VISIBLE,
		left, top, width, height,
		NULL,
		NULL,
		GetModuleHandle(0),
		NULL);

	if (!hw)
		return NULL;
	//--------------------------------------------------------------------------
	GetClientRect(hw, &rc);

	hwInfo = CreateWindowEx(0,
		_T("EDIT"),
		NULL,
		WS_CHILD | WS_VSCROLL | WS_VISIBLE
		| ES_MULTILINE | ES_WANTRETURN,
		0, 0, rc.right, rc.bottom,
		hw,
		NULL,
		GetModuleHandle(0),
		NULL);

	if (!hwInfo)
		return NULL;


	SendMessage(hwInfo, EM_SETLIMITTEXT, (WPARAM)0, 0);

	SendMessage(hwInfo, WM_SETFONT, (WPARAM)GetStockObject(ANSI_VAR_FONT), TRUE);
	SendMessage(hwInfo, EM_SETTABSTOPS, (WPARAM)1, (LPARAM)&TabDistance);


	return hw;
}

//////////////////////////////////////////////////////////////////////////////////////////
static void OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	MoveWindow(hwInfo, 0, 0, cx, cy, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK InfoWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		HANDLE_MSG(hWnd, WM_SIZE, OnSize);
		//---------------------------------------------		
	default:
		return (DefWindowProc(hWnd, uMsg, wParam, lParam));
	}

	return (0);
}

//////////////////////////////////////////////////////////////////////////////////////////
TCHAR* StrAdd(const TCHAR* sFileName, const TCHAR* suffix, bool filter)
{
	static TCHAR sBuff[_MAX_PATH];
	TCHAR* pch;

	_tcscpy(sBuff, sFileName);

	if (filter)
	{
		for (pch = sBuff; pch = _tcspbrk(sBuff, _T(":/\\<>")); *pch = '_');
	}

	return _tcscat(sBuff, suffix);
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
#ifdef _CONSOLE //======================================================================

long GetLong(TCHAR* sPrompt)
{
	TCHAR sBuff[256];

	_tprintf(_T("%s"), sPrompt);
	return _tstol(_getts(sBuff));
}
///////////////////////////////////////////////////////////////////////////////////////
double GetDouble(TCHAR* sPrompt)
{
	TCHAR sBuff[256];

	_tprintf(_T("%s"), sPrompt);
	return _tstof(_getts(sBuff));
}

///////////////////////////////////////////////////////////////////////////////////////
TCHAR* GetString(TCHAR* sPrompt)
{
	TCHAR sBuff[256];

	_tprintf(_T("%s"), sPrompt);
	return _getts(sBuff);
}

///////////////////////////////////////////////////////////////////////////////////////
#endif //_CONSOLE =====================================================================


///////////////////////////////////////////////////////////////////////////////////////
BOOL GetFileNameFromHandle(HANDLE hFileMap, TCHAR* sFileName)
{
	BOOL bSuccess = FALSE;

	void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);

	if (pMem)
	{
		GetMappedFileName(GetCurrentProcess(), pMem, sFileName, MAX_PATH);

		bSuccess = TRUE;
		UnmapViewOfFile(pMem);
	}

	CloseHandle(hFileMap);

	Info(_T("\n File name is %s"), sFileName);

	return(bSuccess);
}
