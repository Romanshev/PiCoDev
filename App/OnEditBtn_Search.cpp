#include "stdafx.h"
#include "Resource.h"

#include "PinaControla.h"
#include "PinaBtnHandlers.h"// save general functions like InfoPinaBtn
#include "OnEditBtn_Search.h"

#include "IoHelper.h"
#include "InfoPinaBtnHelper.h"

LRESULT	OnEditBtn_Search(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR szEditInput[255];

	if (WM_COMMAND == uMsg)
	{
		szDescription = TEXT("Composite button with embedded Edit ");
		InfoPinaBtn(hwTb, uMsg, wParam, lParam);

		if ((LPARAM)hwTb == lParam) SetFocus(hwEdit);
		//((WORD*)szEditInput)[0] = 5;// we can cut input to 5 chars at output

		int len = SendMessage(hwEdit, EM_GETLINE, 0, (LPARAM)szEditInput);
		szEditInput[len] = 0;

		if (EN_CHANGE == HIWORD(wParam))
		{
			if (10 == len) MessageBoxW(hWnd, (LPCWSTR)szEditInput, TEXT("Info"), MB_OK);
		}
		//else if (EN_KILLFOCUS == HIWORD(wParam))
		//{				
		//	MessageBoxW(hWnd, (LPCWSTR)szEditInput, TEXT("Info"), MB_OK);
		//}

		InvalidateRect(hWnd, 0, true);
	}
	return 0;
}