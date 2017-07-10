//OnComboBtn_Paste.h
//#pragma once

//Global extern
extern HINSTANCE hModule;
extern HWND hwMain, hwTb;
extern HWND hwCombo_BarIconsSize, hwCombo_BarStyle;

extern int idComboBtn_Paste;	// anybody need to embed Combo
LRESULT   OnComboBtn_Paste(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);