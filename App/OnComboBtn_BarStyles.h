#pragma once

//Global extern
extern HINSTANCE hModule;
extern HWND hwTb, hwSlider, hwEdit, hwMain;
extern HWND hwCombo_BarIconsSize, hwCombo_BarStyle;
extern LPCTSTR szDescription;

LRESULT   OnComboBtn_BarStyles(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
