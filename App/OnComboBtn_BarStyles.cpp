#include "stdafx.h"
#include "PinaControla.h"
#include "OnComboBtn_BarStyles.h"
#include "InfoPinaBtnHelper.h"



LRESULT OnComboBtn_BarStyles(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_COMMAND == uMsg)
	{
		szDescription = TEXT("Composite button with Combo, select item:");
		InfoPinaBtn(hWnd, uMsg, wParam, lParam);

		if (CBN_SELCHANGE == HIWORD(wParam))
		{
			int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
			// or   SendMessageW(hwCombo_BarStyle, CB_GETCURSEL, 0, 0);
			DWORD TbStyleArr[6] =
			{
				TbStyle_Wrap_Flat_List,
				TbStyle_Wrap_Flat,
				TbStyle_NoWrap_Flat,
				TbStyle_NoWrap_Flat_List,
				TbStyle_Wrap_NoFlat_List,
				TbStyle_Wrap_Flat_List | CCS_BOTTOM
			};

			SetBarStyle(hwTb, TbStyleArr[ItemIndex]);
		}
		InvalidateRect(hWnd, 0, true);
	}
	return 0;
}