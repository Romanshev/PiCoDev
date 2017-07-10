
// we describe here ui tb elements which we use in our application as extern 
// All these elements and additional under development are in BtnHandlers.h
// All implementations of functions are in BtnHandlers.cpp
#pragma once
#include "resource.h"
//#define _CRT_SECURE_NO_WARNINGS // _itoa InfoPinaBtn _itoa_s etc
const int AppLeft   = 150; // position of main window in px
const int AppTop    = 50;
const int AppWidth  = 600;
const int AppHeight = 600;

bool bSlowRedraw;
int  nSlowRedraw;

//// Global Variables for user interface description (toolbar cmd and composite buttons and controls):
//void InfoPinaBtn(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
 LPCTSTR szDescription = TEXT("Use it to describe btn action");;
////static const TCHAR* szDescription = 0;
//
//// OnPaint() funcs and vars
//// ImageList f-n-v - maybe send it to PaintHelper 
//// PinaBar   f-n-v
//// we try to save a simplest and transparent Win32x64PinaBar.cpp structure ( Init, WndProc and msg handlers )
//// outline it by 2-rows /////
//
////   Cmd button demo vars initialization we can make with btn creation
 bool bShowImgAsBlended;
 UINT fStyleOfImage_Normal_or_Blended;// for painting images in app windows by ImageList_DrawEx
//	
//	
//// simple Tb button (or Cmd buttons) idCmdBtn_Img_Label_[Use]_Style
 int idCmdBtn_Open;			        // demo of standart button, the templated colors and actions like Move, Hide, Show, Delete (menu Std)
//LRESULT	  OnCmdBtn_OpenFile(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//
int idCmdBtn_Save, idCmdBtn_SaveAs;	// demo of transforms of standart buttons into checkgroup couple (Menu Checkgroup)
//LRESULT	  OnCmdBtn_Save(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//LRESULT	  OnCmdBtn_SaveAs(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
////Check group btn will be add from menu
//
 int idCmdBtn_Find_NewLabel;         // demo of deleting of the label and setting of new label ( Label - no label - new label )
//LRESULT   OnCmdBtn_NewLabel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//
 int idCmdBtn_Task_BlendImgOrNormal_Check;// demo of checkable single button
//LRESULT   OnCmdBtn_Blend_Normal(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//
 int idCmdBtn_ColorFill_Checkgroup, // demo of checkgroup buttons
    idCmdBtn_ColorLine_Checkgroup;   
//LRESULT   OnCmdBtn_ColorFill(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//LRESULT   OnCmdBtn_ColorLine(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//
////  composite Tb button (or Wnd button) ids:                      
 int idEditBtn_Print;    // embed edit better than msdn sample
//LRESULT   OnEditBtn_Search(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//
 int idSliderBtn_Zoom;   // embed trackbar 
//LRESULT	  OnSliderBtn_MoveIcons(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
////  Slider demo vars
int padScale_Slider; // Increase and decrease Centered rect by pad scaling
int imgSpaceSlider;	// Increase distance between Images of buttons on bottom part of app window
RECT rcImgSliding;
//
 int idComboBtn_Paste;	// anybody need to embed Combo
//LRESULT   OnComboBtn_Paste(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//
 int idComboBtn_BarIconsSize; // you can choose std 16-24-32 or any other 48-64-72-128 sizes of icons
// LRESULT   OnComboBtn_BarIconsSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//
 int idComboBtn_BarStyle; // flat, list, transparent, top, bottom etc
// LRESULT   OnComboBtn_BarStyles(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//
 int idDropdownBtn_BarSettings;  // styles, icon sizes, show/hide btns etc...
// //   DropDown button has popup menu on arrow and cmd on default button
// LRESULT   OnDropdownBtn_BarSettings(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//
//// separator variants:  Gaps and Seps
 int idSep;			// traditional separator has BTNS_SEP style, wrap its group but leads into ugly too higher rows of Tb etc :((( 
 int idGap;			// the gap has not BTNS_SEP and its busters in appearence :-) and not wraps its own group too :-(
 int idGapAfterBlend;
// // They have not handlers		

	
// for OnPaint()
RECT rcClient, rcTb, rcUnderBar;
//int TbHeight= rcTb.bottom - rcTb.top;

RECT rcBtnInfo;
RECT rcTbInfo;
 
//int curIconSizex, curIconSizexy; // if you change ImgList you must renew icon size in handlers to not overload OnPaint()
int topmargin, leftmargin, width;// =cx*coeff

RECT rcMsgForUser_Centered;
RECT rcMsgForUser_CenteredPad; // use it for message for user

LPCTSTR szMsgForUser = TEXT("Use slider to move images");// init
SIZE sszMsgForUser;		// size of msg for users
SIZE pad = {10, 5};	// padding of this centered rect ( to paint bk )

void Define_rcUnderBar();
void Define_rcMsgForUser_Centered();
void Define_rcMsgForUser_CenteredPad();

///////////////////////////////// Slider Stepping Images Painting  for diff sizes ///////////////////
void Define_rcImgSliding();

void MovePicturesFromImageListByTrackbarDown(HDC hdc, HIMAGELIST hImgList, int x, int y, int stepy);
	// а можем и с переводом строки
	// int z=50;
	//ImageList_DrawEx( hImgList_current, i, hdc, 
	//                (z%rcUnderBar.right),y+(z/rcUnderBar.right)*stepy, 0,0, 
	//                CLR_NONE,CLR_NONE, ILD_NORMAL);
	// z += step+imgSpaceSlider;


//void UpdateDataForOnPaint()
//{
//	hImgList_current = GetBarImgList(hwTb);	 
//	 // define icon size to bind all paintings to them
//	ImageList_GetIconSize(hImgList_current, &curIconSizex, &curIconSizexy);//
//
//}

// ImageLists from bmp and icons, resoursec and files (see ImageLists.cpp)
HIMAGELIST hImgList_current;
int curIconSizex, curIconSizexy;

HIMAGELIST hImgList_bmps_24, hImgList_bmps_32, hImgList_bmps_48; // from bmp res

HIMAGELIST hImgList_icons_24, hImgList_icons_32, hImgList_icons_48, // from icon res
hImgList_icons_64, hImgList_icons_72, hImgList_icons_128;

HIMAGELIST hImgList_icons_files_48, hImgList_icons_files_64;       //  from icon files

HIMAGELIST CreateImgList_Icons_Files(HINSTANCE hinst, int cxDesired, int cyDesired, UINT fuLoad);
HIMAGELIST CreateImgList_Icons(HINSTANCE hinst, WORD IDI_ICON_START, WORD IDI_ICON_FIN,
	int cxDesired, int cyDesired, UINT fuLoad);
HIMAGELIST CreateImgList_Bmps(HINSTANCE hinst, WORD IDB_BITMAP_, WORD IDB_BITMAP_FIN,
	int cxDesired, int cyDesired, UINT fuLoad, COLORREF crMask);

HIMAGELIST CreateImgList_Pngs(HINSTANCE hinst, WORD IDB_BITMAP_, WORD IDB_BITMAP_FIN,
	int cxDesired, int cyDesired, UINT fuLoad, COLORREF crMask);

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
RECT gclientrect;
HDC gnewdc;
HBITMAP gnewbitmap;
int* gpnewbitmap;

void ResizeBitmap()
{
	if ((gnewdc != NULL) && (gclientrect.right != 0) && (gclientrect.bottom != 0))
	{
		BITMAPINFO lbitmapinfo;
		lbitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lbitmapinfo.bmiHeader.biWidth = gclientrect.right;
		lbitmapinfo.bmiHeader.biHeight = -gclientrect.bottom;
		lbitmapinfo.bmiHeader.biPlanes = 1;
		lbitmapinfo.bmiHeader.biBitCount = 32;
		lbitmapinfo.bmiHeader.biCompression = BI_RGB;
		lbitmapinfo.bmiHeader.biSizeImage = gclientrect.right * gclientrect.bottom * 4;
		lbitmapinfo.bmiHeader.biClrUsed = 0;
		lbitmapinfo.bmiHeader.biClrImportant = 0;
		if (gnewbitmap != NULL) DeleteObject(gnewbitmap);
		gnewbitmap = CreateDIBSection(gnewdc, &lbitmapinfo, DIB_RGB_COLORS, (void**)&gpnewbitmap, NULL, NULL);
		SelectObject(gnewdc, gnewbitmap);
	};
};
