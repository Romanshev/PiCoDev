
#include "stdafx.h"
#include "Resource.h"

#include "OnCmdBtn_ColorFill_Line.h"
#include "PinaBtnHandlers.h"
#include "PinaControla.h"

#include "IoHelper.h"
#include "InfoPinaBtnHelper.h"

LRESULT OnCmdBtn_ColorFill(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	if (WM_COMMAND == uMsg)
	{
		szDescription = TEXT("Cmd ColorFills groupcheck with ColorLine");
		InfoPinaBtn(hwTb, uMsg, wParam, lParam);

		// 1 v
		HBRUSH hbr_newBkgd = CreateSolidBrush(RGB(180, 120, 150));
		SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)hbr_newBkgd);

		// 2 v
		//DeleteObject((HBRUSH)SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(RGB(180, 105, 150))));

		//HBRUSH* poldClassBkgd = (HBRUSH*)GetClassLongPtrW(hWnd, GCLP_HBRBACKGROUND);
		//const HBRUSH* phbr_newBkgd;
		//phbr_newBkgd = &hbr_newBkgd;
		//SetClassLongPtrW(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR) phbr_newBkgd);


	}
	InvalidateRect(hWnd, 0, true);
	return 0;
}
LRESULT OnCmdBtn_ColorLine(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_COMMAND == uMsg)
	{
		szDescription = TEXT("Cmd ColorLine is groupcheck with ColorFill");
		InfoPinaBtn(hwTb, uMsg, wParam, lParam);

	}

	InvalidateRect(hWnd, 0, true);
	return 0;
}
