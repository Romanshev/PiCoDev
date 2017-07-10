#include "stdafx.h"
#include "Resource.h"

#include "PinaBtnHandlers.h"
#include "PinaControla.h"
#include "IoHelper.h"
#include "InfoPinaBtnHelper.h"

//#include "OpenFilesHandler.h"
//#include "commdlg.h" // for Openfile handler 




// Code of btn handlers ( for usability )


//
//LRESULT OnCmdBtn_NewLabel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	if (WM_COMMAND == uMsg)
//	{
//		if (GetBtnLabel(hwTb, idCmdBtn_Find_NewLabel))// Has it a label?
//		{
//			SetBtnLabel(hwTb, idCmdBtn_Find_NewLabel);// Set label to 0
//		}
//		else {
//			SetBtnLabel(hwTb, idCmdBtn_Find_NewLabel, L"New label");// else set new label
//			// how to work Pins, State and Styles
//			//bool Checked = GetBtnPins(hwTb, idCmdBtn_Find_NewLabel)&PB_CHECKED;
//			//bool Enabled = GetBtnPins(hwTb, idCmdBtn_Find_NewLabel)&PB_ENABLED;
//			//bool Hidden  = GetBtnPins(hwTb, idCmdBtn_Find_NewLabel)&PB_HIDDEN;
//		}
//		szDescription = TEXT("Cmd FindLabel got New Label and will lost it with next click");
//		InfoPinaBtn(hwTb, uMsg, wParam, lParam);
//		InvalidateRect(hWnd, 0, true);
//	}
//	return 0;
//}

//
//LRESULT OnComboBtn_BarIconsSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	if (WM_COMMAND == uMsg)
//	{
//
//		szDescription = TEXT("Composite button with Combo, select item:");
//
//		if (CBN_SELCHANGE == HIWORD(wParam))
//		{
//			int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
//			HIMAGELIST hImgListArr[6] =
//			{ hImgList_icons_24, hImgList_icons_32, hImgList_icons_48,
//			hImgList_icons_64, hImgList_icons_72, hImgList_icons_128 };
//
//			SetBarImgList(hwTb, hImgListArr[ItemIndex]);
//		}
//		InvalidateRect(hWnd, 0, true);
//	}
//	return 0;
//}
//LRESULT OnComboBtn_BarStyles(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	if (WM_COMMAND == uMsg)
//	{
//		szDescription = TEXT("Composite button with Combo, select item:");
//		InfoPinaBtn(hWnd, uMsg, wParam, lParam);
//
//		if (CBN_SELCHANGE == HIWORD(wParam))
//		{
//			int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
//			// or   SendMessageW(hwCombo_BarStyle, CB_GETCURSEL, 0, 0);
//			DWORD TbStyleArr[6] =
//			{
//				TbStyle_Wrap_Flat_List,
//				TbStyle_Wrap_Flat,
//				TbStyle_NoWrap_Flat,
//				TbStyle_NoWrap_Flat_List,
//				TbStyle_Wrap_NoFlat_List,
//				TbStyle_Wrap_Flat_List | CCS_BOTTOM
//			};
//
//			SetBarStyle(hwTb, TbStyleArr[ItemIndex]);
//		}
//		InvalidateRect(hWnd, 0, true);
//	}
//	return 0;
//}

//
//void InfoPinaBtn(HWND hwTb, UINT uMsg, WPARAM wParam, LPARAM lParam)
//
//{
//	if ((LPARAM)hwTb == lParam && WM_COMMAND == uMsg)
//	{
//		int idBtn = (int)LOWORD(wParam);
//
//		const wchar_t* sTip = GetBtnTip(hwTb, idBtn) ? GetBtnTip(hwTb, idBtn) : TEXT("No tip");
//		const wchar_t* sLabel = GetBtnLabel(hwTb, idBtn) ? GetBtnLabel(hwTb, idBtn) : TEXT("No label");
//
//		int            iImg = GetBtnImg(hwTb, idBtn);
//		TCHAR buf[10];
//		TCHAR* snumImageList = _itow(iImg, buf, 10);
//		const wchar_t* sImageListnum = ((iImg<0)) ? TEXT("No image") : snumImageList;
//
//		Info(L"\n[%d] Desc:[%s] \n Label:[%s] Tip: [%s] Image: [%s] ImageInt: [%d]",
//			++nInfo, szDescription, sLabel, sTip, sImageListnum, iImg);
//	}
//}