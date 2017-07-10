//Edit_SearchHandler.h
//#pragma once

//Global extern
extern HINSTANCE hModule;
extern HWND hwTb, hwEdit, hwMain; 


//  composite Tb button (or Wnd button) ids:                      
extern int idEditBtn_Print;    // embed edit better than msdn sample
LRESULT   OnEditBtn_Search(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);