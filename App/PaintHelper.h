#pragma once
/////////////////////////////////////////////////////////////////////////////////////////
// PaintHelper.h (and .cpp) 10 21 2015 
// contains functions for 
// 1. debugging - error output 
// 2. testing - string operations for info output to the TextBox of supporting InfoWindow  
// 3. data exchange
// 4. painting
/////////////////////////////////////////////////////////////////////////////////////////
//#include <math.h>
//#include <Psapi.h>
extern bool bSlowRedraw;
extern int  nSlowRedraw;

///////////////////////////////////////////////////////////////////////////////////////////

// 234556
 void FrameRect(HDC hdc, RECT* pRc, COLORREF rgb);

 void FrameRect(HDC hdc, int left, int top, int right, int bottom, COLORREF rgb);

 void FillRgn(HDC hdc, HRGN hRgn, COLORREF rgb);

 void FillRect(HDC hdc, const RECT* pRc, COLORREF rgb);

 void FillRect(HDC hdc, int left, int top, int right, int bottom, COLORREF rgb);

 void FillRectSlow(HDC hdc, const RECT const* pRc, COLORREF rgb, int nPause);
//{
//	HPEN  hPen = CreatePen(PS_SOLID, 1, rgb);
//
//	HPEN hPenOld = (HPEN)SelectObject(hdc, hPen);
//
//	if (bSlowRedraw && nPause)
//	{
//		for (int y = pRc->top; y < pRc->bottom; ++y)
//		{
//			MoveToEx(hdc, pRc->left, y, 0);
//
//			LineTo(hdc, pRc->right, y);
//
//			if (0 == (y % nPause))
//				Sleep(nPause);
//		}
//	}
//	else
//	{
//		FillRect(hdc, pRc, rgb);
//	}
//
//	SelectObject(hdc, hPenOld);
//
//	DeleteObject(hPen);
//}