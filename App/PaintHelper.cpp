#include "stdafx.h"
#include "PaintHelper.h"

/////////////////////////////////////////////////////////////////////////////////////////

void FrameRect(HDC hdc, RECT* pRc, COLORREF rgb)
{
	HBRUSH hbrush = CreateSolidBrush(rgb);
	HBRUSH hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);

	FrameRect(hdc, pRc, hbrush);

	SelectObject(hdc, hbrushOld);
	DeleteObject(hbrushOld);
	DeleteObject(hbrush);
}
/////////////////////////////////////////////////////////////////////////////////////////
void FrameRect(HDC hdc, int left, int top, int right, int bottom, COLORREF rgb)
{
	RECT rc = { left, top, right, bottom };

	FrameRect(hdc, &rc, rgb);
}

/////////////////////////////////////////////////////////////////////////////////////////
void FillRgn(HDC hdc, HRGN hRgn, COLORREF rgb)
{
	HBRUSH hbrush = CreateSolidBrush(rgb);

	FillRgn(hdc, hRgn, hbrush);

	DeleteObject(hbrush);
}
/////////////////////////////////////////////////////////////////////////////////////////
void FillRect(HDC hdc, const RECT* pRc, COLORREF rgb)
{
	HBRUSH hbrush = CreateSolidBrush(rgb);
	HBRUSH hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);

	FillRect(hdc, pRc, hbrush);

	SelectObject(hdc, hbrushOld);
	DeleteObject(hbrushOld);
	DeleteObject(hbrush);
}
/////////////////////////////////////////////////////////////////////////////////////////
void FillRect(HDC hdc, int left, int top, int right, int bottom, COLORREF rgb)
{
	RECT rc = { left, top, right, bottom };

	FillRect(hdc, &rc, rgb);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////
void FillRectSlow(HDC hdc, const RECT const* pRc, COLORREF rgb, int nPause)
{
	HPEN  hPen = CreatePen(PS_SOLID, 1, rgb);

	HPEN hPenOld = (HPEN)SelectObject(hdc, hPen);

	if (bSlowRedraw && nPause)
	{
		for (int y = pRc->top; y < pRc->bottom; ++y)
		{
			MoveToEx(hdc, pRc->left, y, 0);

			LineTo(hdc, pRc->right, y);

			if (0 == (y % nPause))
				Sleep(nPause);
		}
	}
	else
	{
		FillRect(hdc, pRc, rgb);
	}

	SelectObject(hdc, hPenOld);

	DeleteObject(hPen);
}