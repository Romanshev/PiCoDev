//PinaBtnHandlers.h
//#pragma once

//Global extern
extern HINSTANCE hModule;
extern HWND hwTb, hwSlider, hwEdit, hwMain; 
extern HWND hwCombo_BarIconsSize, hwCombo_BarStyle;

extern HIMAGELIST  hImgList_icons_24, hImgList_icons_32, hImgList_icons_48,
			hImgList_icons_64, hImgList_icons_72, hImgList_icons_128;
// Global Variables for user interface description (toolbar cmd and composite buttons and controls):
//void InfoPinaBtn(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern LPCTSTR szDescription;// = TEXT("Use it to describe btn action");;
//static const TCHAR* szDescription = 0;

// OnPaint() funcs and vars
// ImageList f-n-v - maybe send it to PaintHelper 
// PinaBar   f-n-v
// we try to save a simplest and transparent DemoApp.cpp structure ( Init, WndProc and msg handlers )
// outline it by 2-rows /////

////   Cmd button demo vars initialization we can make with btn creation
//extern bool bShowImgAsBlended;
//extern UINT fStyleOfImage_Normal_or_Blended;// for painting images in app windows by ImageList_DrawEx


// simple Tb button (or Cmd buttons) idCmdBtn_Img_Label_[Use]_Style

//extern int idCmdBtn_Save, idCmdBtn_SaveAs;	// demo of transforms of standart buttons into checkgroup couple (Menu Checkgroup)
//LRESULT	  OnCmdBtn_Save(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//LRESULT	  OnCmdBtn_SaveAs(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//Check group btn will be add from menu

//extern int idCmdBtn_Find_NewLabel;         // demo of deleting of the label and setting of new label ( Label - no label - new label )
//LRESULT   OnCmdBtn_NewLabel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

extern int idCmdBtn_Task_BlendImgOrNormal_Check;// demo of checkable single button
LRESULT   OnCmdBtn_Blend_Normal(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//extern int idCmdBtn_ColorFill_Checkgroup, // demo of checkgroup buttons
//			idCmdBtn_ColorLine_Checkgroup;
//LRESULT   OnCmdBtn_ColorFill(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//LRESULT   OnCmdBtn_ColorLine(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


//extern int idComboBtn_BarIconsSize; // you can choose std 16-24-32 or any other 48-64-72-128 sizes of icons
//LRESULT   OnComboBtn_BarIconsSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//
//extern int idComboBtn_BarStyle; // flat, list, transparent, top, bottom etc
//LRESULT   OnComboBtn_BarStyles(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



// separator variants:  Gaps and Seps
extern int idSep;			// traditional separator has BTNS_SEP style, wrap its group but leads into ugly too higher rows of Tb etc :((( 
extern int idGapAfterBlend;
// They have not handlers		