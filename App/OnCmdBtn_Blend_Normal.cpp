// OnCmdBtn_Blend_Normal.cpp

#include "stdafx.h"
#include "Resource.h"

#include "PinaControla.h"
#include "PinaBtnHandlers.h"// save general functions like InfoPinaBtn
#include "OnCmdBtn_Blend_Normal.h"
//#include "commdlg.h" // for Openfile structures

#include "IoHelper.h"
#include "InfoPinaBtnHelper.h"

LRESULT OnCmdBtn_Blend_Normal(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_COMMAND == uMsg)
	{
		szDescription = TEXT("Cmd double-states (checkable) button forces images to blend50");
		InfoPinaBtn(hwTb, uMsg, wParam, lParam);

		if (GetBtnState(hwTb, idCmdBtn_Task_BlendImgOrNormal_Check) & TBSTATE_CHECKED)// Is it checked?
		{
			SetBtnLabel(hwTb, idCmdBtn_Task_BlendImgOrNormal_Check, L"Normal");// Set label to Uncheck me
			bShowImgAsBlended = true;
			fStyleOfImage_Normal_or_Blended = ILD_BLEND50;
		}
		else
		{
			SetBtnLabel(hwTb, idCmdBtn_Task_BlendImgOrNormal_Check, L"Blend");// else set new label
			bShowImgAsBlended = false;
			fStyleOfImage_Normal_or_Blended = ILD_NORMAL;
		}

		InvalidateRect(hWnd, 0, true);
	}
	return 0;
}