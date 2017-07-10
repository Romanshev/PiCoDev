#pragma once
//Global extern
extern HINSTANCE hModule;
extern HWND hwTb, hwSlider, hwEdit, hwMain;

// Global Variables for user interface description (toolbar cmd and composite buttons and controls):
void InfoPinaBtn(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern LPCTSTR szDescription;// = TEXT("Use it to describe btn action");;
							 //static const TCHAR* szDescription = 0;