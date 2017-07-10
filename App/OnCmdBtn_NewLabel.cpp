#include "stdafx.h"
#include "PinaControla.h"
#include "OnCmdBtn_NewLabel.h"
#include "InfoPinaBtnHelper.h"


LRESULT OnCmdBtn_NewLabel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_COMMAND == uMsg)
	{
		if (GetBtnLabel(hwTb, idCmdBtn_Find_NewLabel))// Has it a label?
		{
			SetBtnLabel(hwTb, idCmdBtn_Find_NewLabel);// Set label to 0
		}
		else {
			SetBtnLabel(hwTb, idCmdBtn_Find_NewLabel, L"New label");// else set new label
																	// how to work Pins, State and Styles
																	//bool Checked = GetBtnPins(hwTb, idCmdBtn_Find_NewLabel)&PB_CHECKED;
																	//bool Enabled = GetBtnPins(hwTb, idCmdBtn_Find_NewLabel)&PB_ENABLED;
																	//bool Hidden  = GetBtnPins(hwTb, idCmdBtn_Find_NewLabel)&PB_HIDDEN;
		}
		szDescription = TEXT("Cmd FindLabel got New Label and will lost it with next click");
		InfoPinaBtn(hwTb, uMsg, wParam, lParam);
		InvalidateRect(hWnd, 0, true);
	}
	return 0;
}