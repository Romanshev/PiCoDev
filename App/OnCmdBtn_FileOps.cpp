#include "stdafx.h"
#include "Resource.h"

#include "PinaControla.h"
#include "PinaBtnHandlers.h"// save general functions like InfoPinaBtn
#include "OnCmdBtn_FileOps.h"
#include "commdlg.h" // for Openfile structures

#include "IoHelper.h"
#include "InfoPinaBtnHelper.h"



LRESULT	OnCmdBtn_OpenFile(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_COMMAND == uMsg)
	{
		//  data for InfoPinaBtnProperties
		szDescription = TEXT("Cmd button Opens cmd.exe");
		InfoPinaBtn(hwTb, uMsg, wParam, lParam);
		InvalidateRect(hWnd, 0, true); // update output in rcInfoPinaBtnProperties

		// do something useful:
		//ShellExecuteW(hWnd, TEXT("open"),TEXT("c:\\windows\\system32\\cmd.exe"),	
		//NULL, NULL, SW_SHOWNORMAL);
		///////////////////////////////  "Right hdc painting in btn handler"  ///////////	
		/*HDC hdc = GetDC(hWnd);
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		RECT rect;
		rect.top=rect.left=200;
		rect.bottom = rect.right = 530;
		TextOutW(hdc, 200, 200,	L"Right hdc painting in btn handler", 42);
		ReleaseDC(hWnd, hdc);
		ValidateRect(hWnd, &rect);*/
		///////////////////////////////////////////////////////////////////////////////////
		TCHAR szPathname[_MAX_PATH];
		HANDLE hf;              // file handle

		OPENFILENAME ofn = { OPENFILENAME_SIZE_VERSION_400 };
		ofn.hwndOwner = hwMain;
		ofn.lpstrFilter = TEXT("*.*\0");
		lstrcpy(szPathname, TEXT("*.*"));
		ofn.lpstrFile = szPathname;
		ofn.nMaxFile = _countof(szPathname);
		ofn.lpstrTitle = TEXT("Select file");
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
		BOOL bOk = GetOpenFileName(&ofn);

	}
	return 0;
}


LRESULT OnCmdBtn_Save(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_COMMAND == uMsg)
	{

		szDescription = TEXT("Cmd idCmdBtn_Save could be turned into check and group: see menu Check single and Checkgroup");
		InfoPinaBtn(hwTb, uMsg, wParam, lParam);
		InvalidateRect(hWnd, 0, true);
	}
	return 0;
}

LRESULT OnCmdBtn_SaveAs(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_COMMAND == uMsg)
	{
		szDescription = TEXT("Cmd idCmdBtn_SaveAs is inserted from menu Checkgroup and coupled with idCmdBtn_Save");
		InfoPinaBtn(hwTb, uMsg, wParam, lParam);
		InvalidateRect(hWnd, 0, true);
	}
	return 0;
}