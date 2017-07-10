#include "stdafx.h"
#include "Resource.h"

#include "PinaBtnHandlers.h"
#include "PinaControla.h"
#include "OnSliderBtn_MoveIcons.h"
#include "IoHelper.h"
#include "InfoPinaBtnHelper.h"




LRESULT	OnSliderBtn_MoveIcons(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if ((WM_HSCROLL == uMsg) || (WM_VSCROLL == uMsg))//(WM_COMMAND==uMsg)||
	{
		imgSpaceSlider = SendMessageW(hwSlider, TBM_GETPOS, 0, 0);
		InvalidateRect(hWnd, &rcImgSliding, true);
	}
	if ((WM_COMMAND == uMsg)){// really we need update only img in cmd button description

		szDescription = TEXT("Composite button with embedded Slider ");
		InfoPinaBtn(hwTb, uMsg, wParam, lParam);

		imgSpaceSlider = SendMessage(hwSlider, TBM_GETPOS, 0, 0);
		InvalidateRect(hWnd, &rcImgSliding, true);
		SetFocus(hwSlider);
	}
	return 0;
}