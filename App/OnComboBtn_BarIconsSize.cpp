#include "stdafx.h"
#include "PinaControla.h"
#include "OnComboBtn_BarIconsSize.h"
#include "InfoPinaBtnHelper.h"
#include "ImageListHelper.h" // for hImgListArr members

LRESULT OnComboBtn_BarIconsSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_COMMAND == uMsg)
	{

		szDescription = TEXT("Composite button with Combo, select item:");
		InfoPinaBtn(hWnd, uMsg, wParam, lParam);

		if (CBN_SELCHANGE == HIWORD(wParam))
		{
			int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
			HIMAGELIST hImgListArr[6] =
			{ hImgList_icons_24, hImgList_icons_32, hImgList_icons_48,
			hImgList_icons_64, hImgList_icons_72, hImgList_icons_128 };

			SetBarImgList(hwTb, hImgListArr[ItemIndex]);
		}
		InvalidateRect(hWnd, 0, true);
	}
	return 0;
}