#pragma once
extern bool bInfoWindow;
extern bool bNoInfo;
extern int nInfo;

void SetInfoWindowMode();
void SetInfoPinaBtnMode(HWND hwTb, bool bInfoPinaBtn);
void SetBufferedPaintMode();
///////////////////////////////////////////////////////////////////////////////////////
void AddText(HWND hwTextBox, TCHAR* fmt, ...);

////////////////////////////////////////////////////////////////////////////////////////
//  in Debug mode to Output Window of VS
void DebugInfo(const wchar_t* fmt, ...);
void DebugInfo(const char* fmt, ...);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// You Must delete pointer to buffer after use it
TCHAR* StringFormat(TCHAR* format, ...);

///////////////////////////////////////////////////////////////////////////////////////
void Info(TCHAR* fmt, ...);
//-------------------------------------------------------------------------------------
// Must be inserted before directive #include <auxilary.h>
//#define VF_TEST // to view info, or 
//#undef VF_TEST // to hide info
//-------------------------------------------------------------------------------------
#ifdef PICO_TEST
#define TestInfo(...) Info(__VA_ARGS__)
#else
#define TestInfo(...)
#endif

///////////////////////////////////////////////////////////////////////////////////////
bool Check(bool test, TCHAR* fmt, ...);
//-------------------------------------------------------------------------------------
// Must be inserted before directive #include <useful.h>
//#define PC_CHECK // to view info, or 
//#undef PC_CHECK // to hide info
//-------------------------------------------------------------------------------------
#ifdef PICO_CHECK
#define TestCheck(...) Check(__VA_ARGS__)
#else
#define TestCheck(...)
#endif

///////////////////////////////////////////////////////////////////////////////////////
void ClearInfo();

long getLongEdit(HWND hwnd);
void setLongEdit(HWND hwnd, long value);

double getDoubleEdit(HWND hwnd);
void setDoubleEdit(HWND hwnd, double value, int digits);

__int64 get64Edit(HWND hwnd);
void set64Edit(HWND hwnd, __int64 value);

/////////////////////////////////////////////////////////////////////////////////////////
HWND CreateInfoWindow(int left, int top, int width, int height);

/////////////////////////////////////////////////////////////////////////////////////////
#ifdef _CONSOLE //======================================================================

long GetLong(TCHAR* sPrompt);
double GetDouble(TCHAR* sPrompt);
TCHAR* GetString(TCHAR* sPrompt);

#endif //_CONSOLE =====================================================================

/////////////////////////////////////////////////////////////////////////////////////////
inline void ErrorInfo(TCHAR* tc)
{
	LPVOID lpMsgBuf;

	if (!FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL))
	{
		Info(_T("\n %s ->  Error in FormatMessage()"), tc);
	}
	else
	{
		Info(_T("\n %s-> GetLastError: %s"), tc, lpMsgBuf);
	}

	LocalFree(lpMsgBuf);
}
//-------------------------------------------------------------------------------------
// Must be inserted before directive #include useful.h
//#define PICO_TEST_ERR // to view info, or 
//#undef PICO_TEST_ERR // to hide info
//-------------------------------------------------------------------------------------
#ifdef PICO_TEST_ERR
#define TestErr(...) ErrorInfo(__VA_ARGS__)
#else
#define TestErr(...)
#endif

//--------------------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
TCHAR* StrAdd(const TCHAR* sFileName, const TCHAR* suffix, bool filter = false);
/////////////////////////////////////////////////////////////////////////////////////////
BOOL GetFileNameFromHandle(HANDLE hFileMap, TCHAR* sFileName);
