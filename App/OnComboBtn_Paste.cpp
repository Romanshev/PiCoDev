#include "stdafx.h"
#include "Resource.h"

#include "PinaControla.h"
#include "PinaBtnHandlers.h"// save general functions like InfoPinaBtn
#include "OnComboBtn_Paste.h"


#include "IoHelper.h"
#include "InfoPinaBtnHelper.h"



LRESULT	OnComboBtn_Paste(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_COMMAND == uMsg)
	{
		// If the user makes a selection from the list:
		//   Send CB_GETCURSEL message to get the index of the selected list item.
		//   Send CB_GETLBTEXT message to get the item.
		//   Display the item in a messagebox.

		szDescription = TEXT("Composite button with embedded Combo ");
		InfoPinaBtn(hwTb, uMsg, wParam, lParam);
		if (CBN_SELCHANGE == HIWORD(wParam))
		{

			int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
				(WPARAM)0, (LPARAM)0);
			TCHAR  ListItem[256];
			(TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT,
				(WPARAM)ItemIndex, (LPARAM)ListItem);
			MessageBox(hWnd, (LPCWSTR)ListItem, TEXT("Item Selected"), MB_OK);
		}


		//InvalidateRect(hWnd, 0, true);
	}
	return 0;
}