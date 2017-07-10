#include "stdafx.h"
#include "Resource.h"

#include "OnDropdownBtn_BarSettings.h"
#include "PinaBtnHandlers.h"
#include "PinaControla.h"
#include "IoHelper.h"
//
/////////////////////////
//////////////////////////////////
/////////////////////////////////////////
// fit for EX_DDA and PB_DROPDOWN (WM_COMMAND and TBN_DROPDOWN separately)
//CD for TB_GETITEMDROPDOWNRECT  
LRESULT OnDropdownBtn_BarSettings(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_COMMAND == uMsg)
	{
		szDescription = TEXT("szDescription:");
		InvalidateRect(hWnd, 0, true);
	}
	if (WM_NOTIFY == uMsg)
	{
		if (TBN_DROPDOWN == LPNMHDR(lParam)->code)
		{
			// Get the coordinates of the button.
			//RECT rc;
			//SendMessage(((LPNMTOOLBAR)lParam)->hdr.hwndFrom, 
			//	TB_GETRECT, 				
			//	(WPARAM)((LPNMTOOLBAR)lParam)->iItem, (LPARAM) &rc);
			// //Convert to screen coordinates.            
			//MapWindowPoints(((LPNMTOOLBAR)lParam)->hdr.hwndFrom, HWND_DESKTOP, (LPPOINT) &rc, 2);

			RECT rcBtn = GetBtnRect(hwTb, idDropdownBtn_BarSettings);// client coord of hwTb
			//RECT rcBtn = GetBtnRect(hwTb, 9, true);// client coord of hwTb - if we del Open btn we lost
			// right rect, so GetBtnRect (hwTb, GetBtnPos, true)

			// Convert to screen coordinates: 1.map to main and 2. to screen
			MapWindowPoints(hwTb, hwMain, (LPPOINT)&rcBtn, 2);
			MapWindowPoints(hwMain, HWND_DESKTOP, (LPPOINT)&rcBtn, 2);

			// Get the menu.
			HMENU hMenuLoaded = LoadMenu(hModule, MAKEINTRESOURCE(IDR_MENU1));

			// Get the submenu for the first menu item.
			HMENU hPopupMenu = GetSubMenu(hMenuLoaded, 0);

			// Set up the pop-up menu.
			// In case the toolbar is too close to the bottom of the screen, 
			// set rcExclude equal to the button rectangle and the menu will appear above 
			// the button, and not below it.

			TPMPARAMS tpm;
			tpm.cbSize = sizeof(TPMPARAMS);
			/*tpm.rcExclude = rc;*/
			tpm.rcExclude = rcBtn;
			// Show the menu and wait for input. 
			// If the user selects an item, its WM_COMMAND is sent, handle it in OnCommand(...)

			TrackPopupMenuEx(hPopupMenu,
				TPM_LEFTALIGN
				| TPM_LEFTBUTTON
				| TPM_VERTICAL
				//| TPM_NONOTIFY
				//| TPM_RETURNCMD, not works in this case
				//| TPM_NOANIMATION //not works, turn out animation in vis eff! SystemParametersInfo?
				//| TPM_RECURSE 
				,
				/*rc.left, rc.bottom, hwMain, &tpm); */          // command id we can place into OnCommand()
				rcBtn.left, rcBtn.bottom, hwMain, &tpm);
			DestroyMenu(hMenuLoaded);

			return TBDDRET_DEFAULT;
		}
	}

	return 0;
}
LRESULT OnDropdownBtn_BarSettings_UnSepArrow(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_NOTIFY == uMsg)
	{
		if (TBN_DROPDOWN == LPNMHDR(lParam)->code)
		{
			// Get the coordinates of the button.
			RECT rc;
			SendMessage(((LPNMTOOLBAR)lParam)->hdr.hwndFrom, TB_GETRECT,
				(WPARAM)((LPNMTOOLBAR)lParam)->iItem, (LPARAM)&rc);

			// Convert to screen coordinates.            
			MapWindowPoints(((LPNMTOOLBAR)lParam)->hdr.hwndFrom, HWND_DESKTOP, (LPPOINT)&rc, 2);

			// Get the menu.
			HMENU hMenuLoaded = LoadMenu(hModule, MAKEINTRESOURCE(IDR_MENU1));

			// Get the submenu for the first menu item.
			HMENU hPopupMenu = GetSubMenu(hMenuLoaded, 0);

			// Set up the pop-up menu.
			// In case the toolbar is too close to the bottom of the screen, 
			// set rcExclude equal to the button rectangle and the menu will appear above 
			// the button, and not below it.

			TPMPARAMS tpm;

			tpm.cbSize = sizeof(TPMPARAMS);
			tpm.rcExclude = rc;

			// Show the menu and wait for input. 
			// If the user selects an item, its WM_COMMAND is sent.

			TrackPopupMenuEx(hPopupMenu,
				TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,//| TPM_RETURNCMD, not works in this case
				rc.left, rc.bottom, hwMain, &tpm);           // command id we can place into OnCommand()
			DestroyMenu(hMenuLoaded);

			return TBDDRET_DEFAULT;
		}
		else if (NM_LDOWN == LPNMHDR(lParam)->code)
		{

			szDescription = TEXT("descriptio");
			InvalidateRect(hWnd, 0, true);
			return TBDDRET_TREATPRESSED;
		}
	}
	return 0;
}