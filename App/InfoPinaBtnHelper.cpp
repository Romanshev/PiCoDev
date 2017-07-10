#include "stdafx.h"
#include "IoHelper.h"
#include "InfoPinaBtnHelper.h"
#include "PinaControla.h"

void InfoPinaBtn(HWND hwTb, UINT uMsg, WPARAM wParam, LPARAM lParam)

{
	if ((LPARAM)hwTb == lParam && WM_COMMAND == uMsg)
	{
		int idBtn = (int)LOWORD(wParam);

		const wchar_t* sTip = GetBtnTip(hwTb, idBtn) ? GetBtnTip(hwTb, idBtn) : TEXT("No tip");
		const wchar_t* sLabel = GetBtnLabel(hwTb, idBtn) ? GetBtnLabel(hwTb, idBtn) : TEXT("No label");

		int            iImg = GetBtnImg(hwTb, idBtn);
		TCHAR buf[10];
		TCHAR* snumImageList = _itow(iImg, buf, 10);
		const wchar_t* sImageListnum = ((iImg<0)) ? TEXT("No image") : snumImageList;

		Info(L"\n[%d] Desc:[%s] \n Label:[%s] Tip: [%s] Image: [%s] ImageInt: [%d]\n",
			++nInfo, szDescription, sLabel,  sTip,     sImageListnum, iImg);
	}
}