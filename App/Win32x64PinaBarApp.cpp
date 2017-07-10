// Win32x64 PinaBar.cpp : Defines the entry point for the application.
// This demo uses v. 6.0 or 6.10 of comctl32.dll, visual styles are enabled but null theme is set
// It shows only the main features of UX design with PinaControla:
// toolbar creating, standart and composite ( with a pinned control ) buttons inserting,
// examples of color settings for typical button states etc.



#include "stdafx.h"
#include "Win32x64PinaBarApp.h"

#include "PinaControla.h"
//////////////////////////  Button Handlers    //////////////////////////////////////
#include "OnCmdBtn_FileOps.h" // some cmd btns groups by actions
#include "OnCmdBtn_Blend_Normal.h"
#include "OnCmdBtn_ColorFill_Line.h"
#include "OnDropdownBtn_BarSettings.h"
#include "OnEditBtn_Search.h"
#include "OnComboBtn_Paste.h"
#include "OnSliderBtn_MoveIcons.h"
#include "OnComboBtn_BarStyles.h"
#include "OnComboBtn_BarIconsSize.h"
#include "OnCmdBtn_NewLabel.h"

#include "PinaBtnHandlers.h"
#include "InfoPinaBtnHelper.h"
#include "IoHelper.h"
#include "PaintHelper.h"
#include "ImageListHelper.h"


static const TCHAR* const tsWinClassName = TEXT("Win32x64  PinaBar UX ClassName");// the main window class name
static const TCHAR* const tsTitle        = TEXT("Win32x64  PinaBar UX app");// the main window title

UINT_PTR HideBtn_TimerId = 1;
UINT HideDelayTime = 500; // Delay time for it
UINT_PTR ShowBtn_TimerId = 2;
UINT ShowDelayTime = 1000; // Delay time for it

//----------------------------------
//bool bSlowRedraw = true;
//int nSlowRedraw =5;
//----------------------------------

// Global Variables:
HINSTANCE  hModule;	// current instance

HWND hwMain, hwTb, hwSlider, hwComboBox, hwEdit;  // all our windows handles

HWND hwCombo_BarIconsSize, hwCombo_BarStyle;// add combo for styles and icon sizes

HBITMAP bmpTips, // Bitmaps for main window client rect drawing
		bmp_Usual_open_save_find_blend, 
		bmp_Usual_color_fill_line,
		bmp_Usual_dropdown,
		bmp_SliderAndCombo;

/////////////////////////////////////////////////////////////////////////////////////////////
////////////  Forward declarations of functions included in this code module  ///////////////
BOOL InitApplication();
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About  (HWND, UINT, WPARAM, LPARAM);
void CALLBACK    HideBtnsTimerProc(HWND hWnd, UINT msg, UINT_PTR TimerId , DWORD curSysTime);
void CALLBACK    ShowBtnsTimerProc(HWND hWnd, UINT msg, UINT_PTR TimerId , DWORD curSysTime);
/////////////////////////////////////////////////////////////////////////////////////////////

int APIENTRY _tWinMain (_In_ HINSTANCE hInstance,
						_In_opt_ HINSTANCE hPrevInstance,
						_In_ LPTSTR    lpCmdLine,
						_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
	MSG msg;
	hModule = hInstance;

	//////////////////////////////////////
	HBRUSH hbrBgrd = CreateSolidBrush( RGB(100,120,230) );
	
	// Perform instance initialization:
	if (!InitApplication()) { return 0; } 	
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
			TranslateMessage(&msg);
			DispatchMessage(&msg);	
	}
	return (int) msg.wParam;
}
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL InitApplication()
{
	//HBRUSH hbrBkgrnd = CreateSolidBrush(RGB(100, 120, 230));

	WNDCLASSEX  wc;
	// Fill in window class structure with parameters that describe the main window
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style         = 0;//CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hModule;
	wc.hIcon         = LoadIcon(hModule , MAKEINTRESOURCE(IDI_WIN32X64PINABAR)); //hModule IDI_ICON
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(100, 120, 230));
	//hbrBkgrnd; //(HBRUSH)(COLOR_MENU + 1);

	wc.lpszMenuName  = MAKEINTRESOURCE(IDC_WIN32X64PINABAR);
	wc.lpszClassName = tsWinClassName;
	wc.hIconSm       = LoadIcon(hModule , MAKEINTRESOURCE(IDI_SMALL)); //hModule
	

	// Register the window class and return success/failure code.
	if( !RegisterClassEx(&wc) && (ERROR_CLASS_ALREADY_EXISTS != GetLastError()) )
	{ return NULL; }
	
	//////////////////////////////////////////////////////////////////////////////
	// Create the application window.
	//
	// Because the CreateWindow function takes its size in pixels, we
	// obtain the system DPI and use it to scale the window size.
	int dpiX = 0;
	int dpiY = 0;
	HDC hdc = GetDC(NULL);
	if (hdc)
	{
		dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
		dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
		ReleaseDC(NULL, hdc);
	}

	hwMain = CreateWindowEx( WS_EX_COMPOSITED, //double-buffering for non-flickering0, 
							tsWinClassName, tsTitle,
							WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
							//AppLeft, 
							static_cast<UINT>(ceil( AppLeft* dpiX / 96.f)),
							//AppTop,
							static_cast<UINT>(ceil(AppTop * dpiY / 96.f)),
							//AppWidth, 
							static_cast<UINT>(ceil(AppWidth * dpiX / 96.f)),
							//AppHeight,
							static_cast<UINT>(ceil(AppHeight * dpiX / 96.f)),
							NULL, NULL, hModule, NULL);


	if (!hwMain) return (FALSE);		
	//SetWindowTheme(hwMain, L" ", L" ");
	//----------------------------------------------------------------------------
	//CreateImgList_Bmps_32_old();// We create imagelist from bmps for owr toolbar buttons

	//	To initialize PinaBar: 
	//  Create empty bar, set the null theme for bar, define the colors of the bar and the color templates for the buttons
	//  Insert usual buttons and set individual colors if you want to show some differences from the templates
	//  Create the controls you want to embed in the toolbar and insert the composite buttons with them to the bar
	//  At last you can set the width of gaps and buttons if not like autosized ones.
	//  Show the app window with PinaBar!
	//  Hope, you will find the button and control message handlers are easy to use.

	//INITCOMMONCONTROLSEX icex;
	//icex.dwSize = sizeof(INITCOMMONCONTROLSEX);// Ensure that the common control DLL is loaded. 
	//icex.dwICC = ICC_WIN95_CLASSES; // W7 not need?
	//InitCommonControlsEx(&icex);

	//  load icons from files
	 hImgList_icons_files_48 = CreateImgList_Icons_Files(hModule, 48, 48, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	 hImgList_icons_files_64 = CreateImgList_Icons_Files(hModule, 64, 64, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	// Scale it by slider ticks?
	 hImgList_icons_128 = CreateImgList_Icons(hModule, IDI_ICON1, IDI_ICON13, 128, 128, LR_CREATEDIBSECTION);
	 hImgList_icons_72  = CreateImgList_Icons(hModule, IDI_ICON1, IDI_ICON13, 72, 72, LR_CREATEDIBSECTION);
	 hImgList_icons_64  = CreateImgList_Icons(hModule, IDI_ICON1, IDI_ICON13, 64, 64, LR_CREATEDIBSECTION);
	 hImgList_icons_48  = CreateImgList_Icons(hModule, IDI_ICON1, IDI_ICON13, 48, 48, LR_CREATEDIBSECTION);
	 hImgList_icons_32  = CreateImgList_Icons(hModule, IDI_ICON1, IDI_ICON13, 32, 32, LR_CREATEDIBSECTION);
	 hImgList_icons_24  = CreateImgList_Icons(hModule, IDI_ICON1, IDI_ICON13, 24, 24, LR_CREATEDIBSECTION);
	
	//hImgList_icons_48_old = CreateImgList_Icons_old(hModule, IDI_ICON1, IDI_ICON13, 48, 48, LR_CREATEDIBSECTION);

	 hImgList_bmps_24 = CreateImgList_Bmps(hModule, IDB_BITMAP7,  IDB_BITMAP17, 24, 24, LR_CREATEDIBSECTION, RGB(0, 0, 0));	
	 hImgList_bmps_32 = CreateImgList_Bmps(hModule, IDB_BITMAP32, IDB_BITMAP42, 32, 32, LR_CREATEDIBSECTION, RGB(0, 0, 0));
	 hImgList_bmps_48 = CreateImgList_Bmps(hModule, IDB_BITMAP32, IDB_BITMAP42, 48, 48, LR_CREATEDIBSECTION, RGB(0, 0, 0));
		// not scale if LoadImage with 0, 0 sizes so Img_Y = Img_X = cxDesired
		//may be ILC_ORIGINALSIZE ??
		

	hwTb = CreatePinaBar(hwMain, hImgList_bmps_32);//this is TbStyle_Wrap_Flat_List by default (try TbStyle_Wrap_Flat too)
	//hwTb = CreatePinaBar(hwMain, hImgList_bmps_32, TbStyle_Wrap_Flat);// Usually it used when all buttons are labeled
	SetWindowTheme(hwTb, L" ", L" ");// function from Uxtheme.h included at stdafx.h
	//---------------
		
	
	//-------------- Background, border and label colors
	//SetBarBk        ( hwTb, &Bk_Dark_Violet);	
	SetBarBk(hwTb, &Bk_Black);
	//SetBarBorder    ( hwTb, &Border_Violet );
	SetBarLabelColor( hwTb, &Label_Ruby); //  Use it to set another than system default ( ??? but only one for all states)

	//-------------- Or by one string -----------------------------------
	//SetBarColors ( hwTb,  &Bk_Dark_Violet, &Border_Violet, &Label_Ruby);
	//-------------------------------------------------------------------

	//---------------- Color Templates for Bk, Border and Labels of all buttons beside individually painting ones -------

	//SetBtnTem_DefaultBk			( hwTb, &Bk_Dark_Violet);
	SetBtnTem_DefaultBk(hwTb, &Bk_Black);
	
	SetBtnTem_DefaultBorder		( hwTb, &Border_BlackRed);
	SetBtnTem_DefaultLabelColor	( hwTb, &Label_White );//== work at Item Prepaint stage  
													  //  ^^ use it with two or more states colors
	SetBtnTem_HoveredBk			( hwTb, &Bk_Yellow); 
	SetBtnTem_HoveredBorder		( hwTb, &Border_Green);
	SetBtnTem_HoveredLabelColor(hwTb, &Label_Blue);
	
	SetBtnTem_PressedBk			( hwTb, &Bk_Rain);
	SetBtnTem_PressedBorder		( hwTb, &Border_Pink);
	SetBtnTem_PressedLabelColor	( hwTb, &Label_Pink);// Label_White
	//
	SetBtnTem_CheckedBk			( hwTb, &Bk_Blue);	
	SetBtnTem_CheckedBorder		( hwTb, &Border_Blue);
	SetBtnTem_CheckedLabelColor	( hwTb, &Label_SolidYellow);

	SetBtnTem_CheckedHoveredBk			( hwTb, &Bk_Black);   
	SetBtnTem_CheckedHoveredBorder		( hwTb, &Border_White);
	SetBtnTem_CheckedHoveredLabelColor	( hwTb, &Label_White);

	SetBtnTem_DisabledBk ( hwTb, &Bk_GrayStd);	// for gap initial color
	SetBtnTem_DisabledBorder(hwTb, &Border_Std);
	SetBtnTem_DisabledLabelColor(hwTb, &Label_Cian);

	//------ standart button without label uses tempated colors
	idCmdBtn_Open = PinCmdBtn( hwTb, OnCmdBtn_OpenFile, 0, L"Open", L"", -1,//Standart button
																PB_AUTOSIZE| PB_ENABLED
																|PB_UseCdColors| PB_NoBk| PB_NoEdges
																);	

	//| PB_NoEdges| PB_CHECK // Must be coupled to draw borders !!

	//-- Std button will be turned into check btn ( menu Check single) or grouped (menu Checkgroup) --
	idCmdBtn_Save = PinCmdBtn ( hwTb, OnCmdBtn_Save, 1, L"", L"Save UX secrets", -1, 
																PB_AUTOSIZE|PB_ENABLED|PB_NoEdges);

	//-----  Std btn and Label appears demo ---------------------------------------------------------
	idCmdBtn_Find_NewLabel = PinCmdBtn( hwTb, OnCmdBtn_NewLabel, 2, L"", L"Set and del new label", -1);
	SetBtn_HoveredLabelColor	( hwTb, idCmdBtn_Find_NewLabel, &Label_Light_Blue);// + - Label
	
	//----- Checkable btn use templated colors defined above ----------------------------------------

	idCmdBtn_Task_BlendImgOrNormal_Check = PinCmdBtn( hwTb, OnCmdBtn_Blend_Normal, 4, 
								L"Blend", L"Check btn to blend imgs", -1,
								PB_AUTOSIZE| PB_ENABLED	| PB_CHECK 
								| PB_NoEdges | PB_UseCdColors);
	
	//---- Checkgroup buttons pair of ColorFill and ColorLine demo ----------------------------------

	idCmdBtn_ColorFill_Checkgroup = PinCmdBtn ( hwTb, OnCmdBtn_ColorFill, 7, L"", L"Individual colors",
													 -1, PB_NoEdges|PB_AUTOSIZE|PB_ENABLED|PB_CHECKGROUP);
	SetBtn_DefaultLabelColor ( hwTb, idCmdBtn_ColorFill_Checkgroup, &Label_Lemon);// See it when set bar style FLAT and labels (menu 1.1)
	SetBtn_DefaultBk( hwTb, idCmdBtn_ColorFill_Checkgroup, &Bk_Blue_Violet);
	SetBtn_HoveredBk( hwTb, idCmdBtn_ColorFill_Checkgroup, &Bk_Blue);

	// ----- Second button in group -----------------------------------------------------------------
	idCmdBtn_ColorLine_Checkgroup = PinCmdBtn ( hwTb, OnCmdBtn_ColorLine, 8, L"", L"Individual colors",
													 -1, PB_NoEdges|PB_AUTOSIZE|PB_ENABLED|PB_CHECKGROUP);
	SetBtn_DefaultLabelColor ( hwTb, idCmdBtn_ColorLine_Checkgroup, &Label_Lemon);// See it when set bar style FLAT and labels (menu 1.1)
	SetBtn_DefaultBk( hwTb, idCmdBtn_ColorLine_Checkgroup, &Bk_Blue_Violet);
	SetBtn_HoveredBk( hwTb, idCmdBtn_ColorLine_Checkgroup, &Bk_Blue);

	//--- Composite button with Edit ----------------------------------------------------------------
	hwEdit = CreateWindowExW(0, L"Edit", NULL,
							WS_CHILD | WS_BORDER | WS_VISIBLE | ES_LEFT 
							| ES_AUTOHSCROLL | ES_AUTOVSCROLL //
							//| ES_MULTILINE
							//| ES_WANTRETURN
							| ES_NUMBER
							,
							0, 0, 0, 0, 
							hwTb,     // or hwMain,
							(HMENU) 1, hModule, 0 );	
	SendMessage( hwEdit, EM_SETCUEBANNER, 0, (LPARAM)(TEXT("CueBanner...")) );// works if v.6 and not ES_MULTILINE 	
	idEditBtn_Print = PinWinBtn( hwTb, OnEditBtn_Search, hwEdit, 102, 24, -1, 5, L"", 4);	//Edit

	//----- Composite button with Slider ( individual colors of default state bk ) -----------------

	hwSlider = CreateWindowExW(0, TRACKBAR_CLASS, 0,             
									WS_CHILD | WS_VISIBLE |TBS_TOOLTIPS,  
									0, 0, 0, 0,                       
									hwMain, 0, hModule, 0 ); 

	SendMessage(hwSlider, TBM_SETRANGE,   (WPARAM) TRUE, (LPARAM) MAKELONG(0, 100));
	SendMessage(hwSlider, TBM_SETPAGESIZE,            0, (LPARAM) 40); 
	SendMessageW(hwSlider, TBM_SETPOS,     (WPARAM) TRUE, (LPARAM) 40);


	idSliderBtn_Zoom = PinWinBtn( hwTb, OnSliderBtn_MoveIcons, hwSlider, 80, 18, -1, 9, L"", -1); // Default fButton pins

	//SetBtn_DefaultBorder(hwTb, idSliderBtn_Zoom, &Border_Green);
	SetBtn_DefaultBk(hwTb, idSliderBtn_Zoom, &Bk_Emerald);
	
	//---- Composite button with Combo --------------------------------------- ----------------------

	hwComboBox = CreateWindowExW(0, WC_COMBOBOXW, 
											TEXT("ComboName"), 
											CBS_DROPDOWN | CBS_HASSTRINGS |CBS_AUTOHSCROLL 
											| WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
											0, 0, 0, 0, 
											hwMain, 0, hModule, 0);
	//SetWindowTheme(hwComboBox, L" ", L" "); // you can set null theme for combo too
//==========================================================================================
	//TCHAR Buttons[10][12] =   
	//{
	// TEXT("Open"),  TEXT("Save"),	TEXT("Find"),		TEXT("Zoom"),		TEXT("Task"),       
	// TEXT("Print"), TEXT("Props"),	TEXT("ColorFill"),	TEXT("ColorLine"),	TEXT("Paste") 
	//};
	//   
	//TCHAR Kind[16]; 
	//int  k = 0; 
	//memset( &Kind, 0, sizeof(Kind) );   // fill it with 0    
	//for (k = 0; k <= 9; k += 1) // hint s<=9  (10-1) 
	//{         
	//	wcscpy_s( Kind, sizeof(Kind)/sizeof(TCHAR),  (TCHAR*)Buttons[k] );
	//	// Add strings to combobox
	//	SendMessage( hwComboBox, (UINT) CB_ADDSTRING, (WPARAM) 0, (LPARAM)Kind ); 
	//}  
	//======================================================================================
	static const TCHAR* ComboBoxItems[] = { _T(" Item 1"), _T("Item 2"), _T("Item 3"),
									_T("Item 4"), _T("Item 5"), _T("Item    6") };
	int nItems = _countof(ComboBoxItems);
	for (int a = 0; a < nItems; a++)
	{
		SendMessage(hwComboBox, CB_ADDSTRING, 0, (LPARAM)ComboBoxItems[a]);

	}

	// Send the CB_SETCURSEL message to display an initial item in the selection field  	
	SendMessage(hwComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	
	idComboBtn_Paste = PinWinBtn( hwTb, OnComboBtn_Paste, hwComboBox, 120, 24, -1, 7, L"Combo", -1);
	
	//---- Dropdown button Bar Setting  ---------------------------------------------

	// Create poup menu ( we did it in resourse editor ) and show it in OnDropdownBtn_BarSettings
	idDropdownBtn_BarSettings = PinCmdBtn (hwTb, OnDropdownBtn_BarSettings, 6, 
											L"Bar Setting", L"Create popup menu to use with me", -1, 
											PB_DROPDOWN | PB_AUTOSIZE | PB_ENABLED 
											|PB_UseCdColors| PB_NoEdges | PB_NoBk);
	//SetBtn_HoveredBorder(hwTb, idDropdownBtn_BarSettings, &Border_Ruby);

	//---- Composite button with Combo for Icon Size Choice----------------------

	hwCombo_BarIconsSize = CreateWindowExW(0, WC_COMBOBOXW,
											TEXT("BarIconsSizes"),
											CBS_DROPDOWN | CBS_HASSTRINGS | CBS_AUTOHSCROLL
											| WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
											0, 0, 0, 0,
											hwMain, 0, hModule, 0);
	//SetWindowTheme(hwComboBox, L" ", L" "); // you can set null theme for combo too

	TCHAR IconSizes[6][4] =
	{
		TEXT("24"), TEXT("32"), TEXT("48"), 
		TEXT("64"),	TEXT("72"), TEXT("128")
	};

	TCHAR Size[16];
	int  is = 0;
	memset(&Size, 0, sizeof(Size));
	for (is = 0; is <= 5; is += 1) // hint s<=5  (6-1) 
	{
		wcscpy_s(Size, sizeof(Size) / sizeof(TCHAR), (TCHAR*)IconSizes[is]);

		// Add strings to combobox
		SendMessage(hwCombo_BarIconsSize, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)Size);
	}
	// Send the CB_SETCURSEL message to display an initial item in the selection field  	
	SendMessage(hwCombo_BarIconsSize, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

	idComboBtn_BarIconsSize = PinWinBtn(hwTb, OnComboBtn_BarIconsSize,
										hwCombo_BarIconsSize, 
										60, 24, -1, 3, L"Icons Size", -1, 
										PB_HIDDEN | PB_AUTOSIZE | PB_ENABLED 
										| PB_UseCdColors | PB_NoEdges);

	//---- Composite button with Combo Bar Style Choice  ----------------

	hwCombo_BarStyle = CreateWindowExW(0, WC_COMBOBOXW,
										TEXT("BarStyle"),
										CBS_DROPDOWN | CBS_HASSTRINGS | CBS_AUTOHSCROLL
										| WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
										0, 0, 0, 0,
										hwMain, 0, hModule, 0);
	//SetWindowTheme(hwComboBox, L" ", L" "); // you can set null theme for combo too

	TCHAR BarStyles[6][20] =   // 6 - number of strings ( 20 - max lengh of strings )
	{  
		 TEXT("FlatListWrap"), 
		 TEXT("FlatWrap"),
		 TEXT("FlatNoWrap"), 
		 TEXT("FlatListNoWrap"),
		 TEXT("ListWrap"),
		 TEXT("FlatListWrapBottom")		
	};

	TCHAR Style[20];
	int  s = 0;
	memset(&Style, 0, sizeof(Style));
	for (s = 0; s <= 5; s += 1) // hint s<=5  (6-1)  
	{
		wcscpy_s(Style, sizeof(Style) / sizeof(TCHAR), (TCHAR*)BarStyles[s]);

		// Add strings to combobox
		SendMessage(hwCombo_BarStyle, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)Style);
	}
	// Send the CB_SETCURSEL message to display an initial item in the selection field  	
	SendMessage(hwCombo_BarStyle, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

	idComboBtn_BarStyle = PinWinBtn(hwTb, OnComboBtn_BarStyles, hwCombo_BarStyle, 
									180, 24, -1, 2, L"Bar Style", -1,
									PB_HIDDEN |	 PB_AUTOSIZE | PB_ENABLED 
									| PB_UseCdColors | PB_NoEdges);
		
	//================ Toolbar NuiBar init end ======================================

	bmpTips=(HBITMAP)LoadImageW( hModule, MAKEINTRESOURCE(IDB_BITMAP1),IMAGE_BITMAP,0,0,LR_DEFAULTSIZE);
	
	bmp_Usual_open_save_find_blend=(HBITMAP)LoadImageW( hModule, MAKEINTRESOURCE(IDB_BITMAP6),IMAGE_BITMAP,0,0,LR_DEFAULTSIZE);
	
	bmp_Usual_color_fill_line=(HBITMAP)LoadImageW( hModule, MAKEINTRESOURCE(IDB_BITMAP4),IMAGE_BITMAP,0,0,LR_DEFAULTSIZE);
	
	bmp_Usual_dropdown=(HBITMAP)LoadImageW( hModule, MAKEINTRESOURCE(IDB_BITMAP5),IMAGE_BITMAP,0,0,LR_DEFAULTSIZE);
	
	bmp_SliderAndCombo=(HBITMAP)LoadImageW( hModule, MAKEINTRESOURCE(IDB_BITMAP3),IMAGE_BITMAP,0,0,LR_DEFAULTSIZE);
	////////////////////
	//////////////////// init gnewdc compatible
	if (gnewdc == NULL)
	{
		HDC lhdc = GetDC(hwMain);
		gnewdc = CreateCompatibleDC(lhdc);
		ReleaseDC(hwMain, lhdc);
	};
	//////////////////////////
	///////////////////////////
	//-----------------------------------------------------------------------------
	ShowWindow  (hwMain, SW_SHOW);
	UpdateWindow(hwMain);
	//----------------------------------------------------------------------------
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK OnCommand(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{ 
	//------------------------------------------------------------------
	if(0== lParam)
	{	
		//Parse the menu selections:
		switch( LOWORD(wParam) )
		{			
			case ID_BAR_FLAT_LABELS:
			{
				// add labels to buttons:
				SetBtnLabel(hwTb, idCmdBtn_Open, L"Open");
				SetBtnLabel(hwTb, idCmdBtn_Save, L"Save");
				SetBtnLabel(hwTb, idCmdBtn_Find_NewLabel, L"Find");
				SetBtnLabel(hwTb, idEditBtn_Print, L"Print Edit");
				SetBtnLabel(hwTb, idCmdBtn_ColorFill_Checkgroup, L" Color Fill");
				SetBtnLabel(hwTb, idCmdBtn_ColorLine_Checkgroup, L"Color Line");
				SetBtnLabel(hwTb, idSliderBtn_Zoom, L"Zoom Imgs");
				SetBtnLabel(hwTb, idComboBtn_Paste, L"Button Imgs");
				SetBtnLabel(hwTb, idDropdownBtn_BarSettings, L"Color Line");								
				SetBarStyle(hwTb, TbStyle_Wrap_Flat);
				//InvalidateRect(hwTb, 0, TRUE);
			}
			break;
			//------------------------------------------------------
			case ID_BAR_FLAT_NOLABELS:
			{
				// delete labels of buttons:
				SetBtnLabel(hwTb, idCmdBtn_Open, L"");
				SetBtnLabel(hwTb, idCmdBtn_Save, L"");
				SetBtnLabel(hwTb, idCmdBtn_Find_NewLabel, L"");
				SetBtnLabel(hwTb, idCmdBtn_Task_BlendImgOrNormal_Check, L"");
				SetBtnLabel(hwTb, idEditBtn_Print, L"");
				SetBtnLabel(hwTb, idCmdBtn_ColorFill_Checkgroup, L"");
				SetBtnLabel(hwTb, idCmdBtn_ColorLine_Checkgroup, L"");
				SetBtnLabel(hwTb, idSliderBtn_Zoom, L"");
				SetBtnLabel(hwTb, idComboBtn_Paste, L"");	
				SetBtnLabel(hwTb, idDropdownBtn_BarSettings, L"");						
				SetBarStyle(hwTb, TbStyle_Wrap_Flat);
				//InvalidateRect(hwTb, 0, TRUE);
			}
			break;
			//------------------------------------------------------
			case ID_BAR_FLATLIST:
			{
				SetBarStyle(hwTb, TbStyle_Wrap_Flat_List);	
			}
			break;
			//------------------------------------------------------
			case ID_BAR_BOTTOM:
			{
				SetBarStyle(hwTb, TbStyle_Wrap_Flat_List|CCS_BOTTOM);	
			}
			break;
						//------------------------------------------------------
			case ID_BAR_TRANSPARENT:
			{
				SetBarStyle(hwTb);	
				SetBarColors(hwTb);
				SetBtnTem_DefaultBk(hwTb);
				SetBtnTem_HoveredBk(hwTb);
				SetBtnTem_DefaultLabelColor(hwTb);
				SetBtn_DefaultBk(hwTb, idCmdBtn_ColorFill_Checkgroup);
				SetBtn_DefaultBk(hwTb, idCmdBtn_ColorLine_Checkgroup);
				SetBtn_DefaultBk(hwTb, idSliderBtn_Zoom);				
				
				InvalidateRect(hwTb, 0, TRUE);
			}
			break;
			//------------------------------------------------------
			
			case ID_STD_MOVE_OPEN_TO_THE_END:
			{	
				int iPosFrom = GetBtnPos(hwTb, idCmdBtn_Open);
				MoveBtnPosFromTo(hwTb,	iPosFrom, 9);	
					
			}
			break;
			//------------------------------------------------------
			case ID_STD_HIDE_OPEN:
			{
				//SetBtnPins(hwTb, idCmdBtn_Open, 
				//						PB_AUTOSIZE|PB_ENABLED|PB_HIDDEN	// Right
				//						|PB_NoEdges|PB_NoEtched|PB_UseCdColors
				//						);

				PB idCmd_Open_Pins = GetBtnPins(hwTb, idCmdBtn_Open);
				SetBtnPins(hwTb, idCmdBtn_Open,
					idCmd_Open_Pins |= PB_HIDDEN);
															
				//SendMessage(hwTb, TB_HIDEBUTTON, WPARAM(idCmdBtn_Open), MAKELPARAM(TRUE, 0)); // Wrong way!	
			}
			break;
			//------------------------------------------------------
			case ID_STD_SHOW_OPEN:
			{
				/*SetBtnPins(hwTb, idCmdBtn_Open,
							PB_AUTOSIZE|PB_ENABLED
							|PB_NoEdges|PB_NoEtched|PB_UseCdColors
							);*/
				PB idCmd_Open_Pins = GetBtnPins(hwTb, idCmdBtn_Open);
				SetBtnPins(hwTb, idCmdBtn_Open,
					idCmd_Open_Pins &=(~PB_HIDDEN));												
				//SendMessage(hwTb, TB_HIDEBUTTON, WPARAM(idCmdBtn_Open), MAKELPARAM(FALSE, 0)); // Wrong way!	
			}
			break;
			//------------------------------------------------------
			case ID_STD_DEL_OPEN:
			{	
				DelBtn(hwTb, idCmdBtn_Open);	// Right								
			}
			break;

			//------------------------------------------------------
			case ID_GAPS_PIN_AFTER_BLEND:
			{
				idGapAfterBlend = PinGap(hwTb, 4);
				// comment next strings to seen fit colors and ubcomment to see contrast ones
				SetBtn_DisabledBk( hwTb, idGapAfterBlend, &Bk_Yellow );// need update
				InvalidateRect(hwTb, 0, TRUE);
			}
			break;
			//------------------------------------------------------
			case ID_GAPS_SET_WIDTH:
			{
				SetBtnWidth(hwTb, idGapAfterBlend, 20);								
			}
			break;			
			//------------------------------------------------------
			case ID_GAPS_DEL_GAP:
			{
				DelBtn(hwTb, idGapAfterBlend);
			}
			break;	
			//------------------------------------------------------
			case ID_SEPS_PIN_AFTER_PRINTEDIT:
			{			
				idSep = PinSep(hwTb, 5, 6);
			}
			break;
			//------------------------------------------------------
			case ID_SEPS_DEL:
			{			
				DelBtn(hwTb, idSep);			
			}
			break;
			//------------------------------------------------------
			case ID_CHECK_SET_SAVE:
			{			
				SetBtnPins(hwTb, idCmdBtn_Save, 
							PB_CHECK|PB_ENABLED|PB_AUTOSIZE
							|PB_UseCdColors|PB_NoEdges);			
			}
			break;
			//------------------------------------------------------
			case ID_CHECK_GROUP_SAVE_SAVEAS:// attn! they must be neighbours
			{	
				idCmdBtn_SaveAs = PinCmdBtn ( hwTb, OnCmdBtn_SaveAs, 10, L"", L"SaveAs", 1, 
																PB_NoEdges|PB_AUTOSIZE|PB_ENABLED);																		
				SetBtnPins(hwTb, idCmdBtn_Save,	PB_CHECKGROUP|PB_ENABLED|PB_AUTOSIZE|PB_UseCdColors|PB_NoEdges);	
				SetBtnPins(hwTb, idCmdBtn_SaveAs, PB_CHECKGROUP|PB_ENABLED|PB_AUTOSIZE|PB_UseCdColors|PB_NoEdges);		
			}
			break;
			//------------------------------------------------------		
			case ID_DROPDOWN_STYLE_WHOLE:
			{			 
				SetBtnPins(hwTb, idDropdownBtn_BarSettings, PB_WHOLEDROPDOWN| PB_AUTOSIZE|PB_ENABLED|PB_UseCdColors);
				// Good bye, Default button!				
			}
			break;
						//------------------------------------------------------		
			case ID_DROPDOWN_STYLE_SEP:
			{			 
				SetBtnPins(hwTb, idDropdownBtn_BarSettings, PB_DROPDOWN| PB_AUTOSIZE|PB_ENABLED|PB_UseCdColors);
				// Hi, Default button!				
			}
			break;
			//------------------------------------------------------		
			case ID_DROPDOWNMENU_FIRST:
			{	
				szDescription=TEXT("Change icon size in ComboBtn");
				InvalidateRect(hwMain, 0, TRUE);
				// show btn combo icon size
				SetBtnPins(hwTb, idComboBtn_BarIconsSize, 
						GetBtnPins(hwTb, idComboBtn_BarIconsSize)&(~PB_HIDDEN));
			}
			break;
			//------------------------------------------------------
			case ID_DROPDOWNMENU_SECOND:
			{				
				szDescription=TEXT(";;;;;;;;;;;:");
				InvalidateRect(hwMain, 0, TRUE);	
				SetBtnPins(hwTb, idComboBtn_BarStyle,
					GetBtnPins(hwTb, idComboBtn_BarStyle)&(~PB_HIDDEN));
			}
			break;
			//------------------------------------------------------	
			
			case ID_COMPOSITE_MOVE_SLIDER_END:
			{	
				int iSliderPosFrom = GetBtnPos(hwTb,idSliderBtn_Zoom);
				MoveBtnPosFromTo(hwTb,	iSliderPosFrom, 9);		
			}
			break;
			//------------------------------------------------------
			case ID_COMPOSITE_HIDE_SLIDER:
			{	
				SetBtnPins(hwTb,idSliderBtn_Zoom, 
										PB_ENABLED|PB_HIDDEN
										|PB_NoEdges|PB_NoOffset|PB_NoEtched|PB_UseCdColors);
											
				//SendMessageW(hwTb, TB_HIDEBUTTON, WPARAM(idSliderBtn_Zoom), MAKELPARAM(TRUE, 0)); // Wrong way!								
			}
			break;
			//------------------------------------------------------
			case ID_COMPOSITE_SHOW_SLIDER:
			{	
				SetBtnPins(hwTb,idSliderBtn_Zoom,
							PB_ENABLED|PB_NoEdges|PB_NoOffset|PB_NoEtched|PB_UseCdColors);									
			}
			break;
			//------------------------------------------------------
			case ID_COMPOSITE_DEL_PRINTEDIT:
			{	
				DelBtn(hwTb, idEditBtn_Print);		
			}
			break;
						//------------------------------------------------------
			case ID_COMPOSITE_CHANGE_SLIDER_BK:
			{	
				SetBtn_DefaultBk(hwTb,idSliderBtn_Zoom, &Bk_Blue_Black);
				InvalidateRect(hwTb, 0, TRUE);					
			}
			break;
			//------------------------------------------------------
			//------------------------------------------------------
			case ID_COMPOSITE_ADD_ITEM_COMBO:
			{				
				SendMessage(hwComboBox, CB_ADDSTRING, 0, (LPARAM)_T("New Item"));
			}
			break;
			case ID_HELP_ONLINE:
			{	
				//ShellExecuteW(hwMain, TEXT("open"), TEXT("http://pinacontrola.com/howto"), NULL, NULL, SW_SHOWNORMAL);	
				//ShellExecuteW(hwMain, TEXT("open"), TEXT("psr.exe"), NULL, NULL, SW_SHOWNORMAL);
				
				
			}

			break;

			case ID_HELP_SENDFEEDBACK:
			{	
				ShellExecuteW(hwMain, TEXT("open"), TEXT("mailto:support@pinacontrola.com"), NULL, NULL, SW_SHOWNORMAL);
				
				/*ShellExecuteW(hwMain, TEXT("open"), TEXT("onenote:///C:\\Users\\RS\\Documents
				\\Записные%20книжки%20OneNote\\UserInter\\Интерфейсы.one#GUI%20tools&section-id=
				{E6D1C668-67B6-448E-900F-BC996BFAE55A}&page-id={4BF285CA-321D-4D76-81E5-8D274BF86009}
				&end"), NULL, NULL, SW_SHOWNORMAL);*/
			
			}
			break;
			case ID_MAINWINDOW_DOUBLE:
			{
				SetWindowLongPtr(hwMain, GWL_EXSTYLE, WS_EX_COMPOSITED);
			}
			break;
			case ID_MAINWINDOW_USUAL:
			{
				SetWindowLongPtr(hwMain, GWL_EXSTYLE, 0);

			}
			break;
			//---------------------Icon size -----------------------------
			case ID_ICONSIZE_24:
				SetBarImgList(hwTb, hImgList_icons_24);
				InvalidateRect(hwMain, 0, TRUE);				
				break;
			case ID_ICONSIZE_32:				
				SetBarImgList(hwTb, hImgList_icons_32);// icons
				InvalidateRect(hwMain, 0, TRUE);
				break;
			case ID_ICONSIZE_48:
				SetBarImgList(hwTb, hImgList_icons_48);// icons				
				InvalidateRect(hwMain, 0, TRUE);
				break;
			case ID_ICONSIZE_64:				
				SetBarImgList(hwTb, hImgList_icons_64);
				InvalidateRect(hwMain, 0, TRUE);
				break;
			case ID_ICONSIZE_72:
				SetBarImgList(hwTb, hImgList_icons_72);
				InvalidateRect(hwMain, 0, TRUE);
				break;
			case ID_ICONSIZE_128:	
				SetBarImgList(hwTb, hImgList_icons_128);
				InvalidateRect(hwMain, 0, TRUE);
				break;
			case ID_ICONSIZE_48_LOAD_FROM_FILE:
				SetBarImgList(hwTb, hImgList_icons_files_48);
				InvalidateRect(hwMain, 0, TRUE);
				break;
			case ID_ICONSIZE_64_LOAD_FROM_FILE:	
				SetBarImgList(hwTb, hImgList_icons_files_64);
				InvalidateRect(hwMain, 0, TRUE);
				break;
			//-------------------- Bmp sizes  ---------------------------
			case ID_BMPSIZE_24:
				SetBarImgList(hwTb, hImgList_bmps_24);
				InvalidateRect(hwMain, 0, TRUE);
				break;
			case ID_BMPSIZE_32:				
				SetBarImgList(hwTb, hImgList_bmps_32);
				InvalidateRect(hwMain, 0, TRUE);
				break;
			case ID_BMPSIZE_48:				
				SetBarImgList(hwTb, hImgList_bmps_48);
				InvalidateRect(hwMain, 0, TRUE);
				break;
				//------------------------------------------------------
			case ID_TIMER_HIDE:						
				SetTimer(hwMain, HideBtn_TimerId, HideDelayTime, HideBtnsTimerProc);				
				break;
			case ID_TIMER_SHOW:						
				SetTimer(hwMain, ShowBtn_TimerId, ShowDelayTime, ShowBtnsTimerProc);				
				break;
			//------------------------------------------------------
			case IDM_ABOUT:
				DialogBox(hModule, MAKEINTRESOURCE(IDD_DEMO_APP), hWnd, About);
				break;
			case IDM_EXIT:
			{
				DestroyWindow (hWnd);
			}
			break;
			
			//------------------------------------------------------
			default:
				break;
		}
	}
	//------------------------------------------------------------------
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK OnRButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT retCode = 0;
	//------------------------------------------------------------------
	//------------------------------------------------------------------
	return retCode;
}

/////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK OnLButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT retCode = 0;
	//------------------------------------------------------------------

	//--- will we set toolbar to the new position at this point?/////////////
	/////////////////////////////////////////////////////////////////////////
	
	

	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

	//if (PtInRect(&rcTestMiniBar, pt))
	//{

	//	//SetWindowLongPtrW(hwTb, GWL_STYLE, (LONG_PTR)( ~CCS_TOP)); ???
	//	SetBarStyle(hwTb, GetBarStyle(hwTb) & ~CCS_TOP);
	//	SetWindowPos(hwTb, 0, rcTestMiniBar.top, rcTestMiniBar.left, 600, 200, SWP_DRAWFRAME | SWP_SHOWWINDOW);
	//	//MoveWindow(hwTb, rcTestMiniBar.top, rcTestMiniBar.left, 600, 200, false);
	//	//SetBarStyle(hwTb, 0 | WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | TBSTYLE_FLAT| TBSTYLE_LIST | TBSTYLE_TOOLTIPS | TBSTYLE_WRAPABLE | CCS_NOPARENTALIGN );
	//	//MoveWindow(hwTb, rcTestMiniBar.top, rcTestMiniBar.left, 600, 200, false);
	//	
	//	InvalidateRect(hwTb, 0, TRUE);
	//	UpdateWindow(hwTb);
	//}
	//--- WS_POPUP //////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	//--

	//------------------------------------------------------------------
	return retCode;
}

/////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK OnMouseMove(HWND hWnd, int x, int y, UINT keyFlags)
{
	LRESULT retCode = 0;
	//------------------------------------------------------------------

	//------------------------------------------------------------------
	return retCode;
}
////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint (hWnd, &ps);

	//---------------------------------------------------------------------------------	

	//////////////////////////////  Fit for Painting = Client RECT - Toolbar Sizes  ////////////////////////////////
	SetBkMode(hdc, TRANSPARENT);

	GetClientRect(hWnd, &rcClient);		
	GetWindowRect(hwTb, &rcTb);
	int heightTb= rcTb.top-rcTb.bottom;
	rcUnderBar=rcClient;
	rcUnderBar.top=rcClient.top - heightTb;	
	
	DrawTextW(hdc, TEXT("MsForUser"), -1, &rcUnderBar, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	//OffsetRect(&...);
	
	//Define_rcUnderBar();
	TextOutW(hdc, 300, 300, szDescription, _tcslen(szDescription));
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
	/*{GetClientRect(hWnd, &gclientrect);
	ResizeBitmap();
	if (gnewdc == NULL) gnewdc = CreateCompatibleDC(hdc);

	BitBlt(hdc, 0, 0, gclientrect.right, gclientrect.bottom, gnewdc, 0, 0, SRCCOPY);
	}*/
	///////////////////////////////////  Prepare data for MsgForUser Painting  //////////////////////////////////
	GetTextExtentPoint32W(hdc, szMsgForUser, _tcslen(szMsgForUser), &sszMsgForUser);// for DPI - hdc
	Define_rcMsgForUser_Centered();	
	Define_rcMsgForUser_CenteredPad();

	////////////////////////////////////   MsgForUser Rect Painting    ///////////////////////////////////////////
	//
	//HBRUSH hbrush    = CreateSolidBrush(Border_Violet);  //Border of client under bar
	//HBRUSH hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);
	//FillRect( hdc, &rcMsgForUser_CenteredPad, hbrush);	
	//SelectObject(hdc, hbrushOld);
	//DeleteObject(hbrushOld);
	//DeleteObject(hbrush);	

	//RECT rcColor={200,200,100,200};
	////const RECT*prcColor=&rcColor;

	FillRect(hdc, &rcMsgForUser_CenteredPad, RGB(220, 100,100));

	//TextOutW (hdc, rcMsgForUser_Centered.left,	rcMsgForUser_Centered.top,	
	//	szMsgForUser, _tcslen(szMsgForUser));
	/////////////////////////////////// Next painting is bound to the size of current icons ///////////////////////

	hImgList_current = GetBarImgList(hwTb);// update after changes
	//int curIconSizex, curIconSizexy;  // define icon size to bind all paintings to them
	ImageList_GetIconSize(hImgList_current, &curIconSizex, &curIconSizexy);

	///////////////////////////////// Slider Stepping Images Painting ///////////////////////////////////////////// 
	
	topmargin=leftmargin=width=curIconSizex;// =cx*coeff	

	Define_rcImgSliding();
	MovePicturesFromImageListByTrackbarDown(hdc, hImgList_current, rcImgSliding.left, rcImgSliding.top,  curIconSizexy);

	///////////////////////////////// Standart button properties info output in rcInfoPinaBtnProperties ////////////

	topmargin=leftmargin=3*curIconSizex;
	width=curIconSizex+120;// _tcslen(szLabelmax)	
			
	////////////////////////////////////////////////////////////////////////////////////
	
	// Draw  Tips.bmp 
	HDC hMemDC = CreateCompatibleDC(hdc);// or (0);
	//SelectObject(hMemDC, bmpTips);
	//BitBlt(hdc, rcUnderBar.left + 3*curIconSizex, rcUnderBar.top + 2*curIconSizexy, 623, 388, hMemDC, 0, 0, SRCCOPY);
	//   Usual btns pics
	SelectObject(hMemDC, bmp_Usual_open_save_find_blend);
	BitBlt(hdc, rcUnderBar.left + 2*curIconSizex, rcUnderBar.top + 490, 216, 41, hMemDC, 0, 0, SRCCOPY);
		
	SelectObject(hMemDC, bmp_Usual_color_fill_line);
	BitBlt(hdc, rcUnderBar.left + 3*curIconSizex + 280, rcUnderBar.top + 490, 85, 43, hMemDC, 0, 0, SRCCOPY);
		
	SelectObject(hMemDC, bmp_Usual_dropdown);
	BitBlt(hdc, rcUnderBar.left + 3*curIconSizex + 380, rcUnderBar.top + 490, 148, 45, hMemDC, 0, 0, SRCCOPY);
	//  Composite btns pics	
	SelectObject(hMemDC, bmp_SliderAndCombo);
	BitBlt(hdc, rcUnderBar.left + 3*curIconSizex, rcUnderBar.top + 580, 303, 47, hMemDC, 0, 0, SRCCOPY);
	DeleteDC(hMemDC);	
	
	TextOutW(hdc, rcUnderBar.left + 3*curIconSizex, rcUnderBar.top + 460,
		L"Usual buttons: standart, checkable, dropdown and sep", 52);
	
	TextOutW(hdc, rcUnderBar.left + 3*curIconSizex, rcUnderBar.top + 550,
		L"Composite buttons: with embedded slider, combo and edit", 55);
	

	
	////////////////////////////////////////////////////////////////////////
	EndPaint (hWnd, &ps);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK OnNotify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//--------------------------------------------------------------------------
	return 0; // must return zero to default processing the WM_NOTIFY
}
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{  	
	//----- Must be first in msg handler ------------------------------------------	

	if( IsThisMsgForPinaBar( hWnd, uMsg, wParam, lParam)) //
	{
		return MsgHandlerPinaBar( hWnd, uMsg, wParam, lParam); // 
	}

	//------------------------------------------------------------------------
	if(WM_DESTROY == uMsg)
	{
		return OnDestroy(hWnd, uMsg, wParam, lParam);
	}
	//------------------------------------------------------------------------
	else if(WM_LBUTTONDOWN == uMsg)
	{
		return OnLButtonDown(hWnd, uMsg, wParam, lParam);
	}
	//------------------------------------------------------------------------
	else if(WM_MOUSEMOVE == uMsg)
	{
		return OnMouseMove(hWnd, uMsg, wParam, lParam);
	}
	//------------------------------------------------------------------------
	else if(WM_COMMAND == uMsg)
	{
		return OnCommand(hWnd, uMsg, wParam, lParam);
	}
	//------------------------------------------------------------------------
	else if(WM_NOTIFY == uMsg)
	{
		return OnNotify(hWnd, uMsg, wParam, lParam);
	}
		//------------------------------------------------------------------------
	else if(WM_PAINT == uMsg)
	{
		return OnPaint(hWnd, uMsg, wParam, lParam);
	}

	//------------------------------------------------------------------------
	else if(WM_WINDOWPOSCHANGED == uMsg)
	{
		return 0;
	}
	//------------------------------------------------------------------------
	else 
	{
		return (DefWindowProc(hWnd, uMsg, wParam, lParam));
	}

	//------------------------------------------------------------------------
	return (0);
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void CALLBACK    HideBtnsTimerProc(HWND hWnd, UINT msg, UINT_PTR TimerId , DWORD curTime)
{
     MessageBeep (MB_ICONWARNING) ;// MB_OK  MB_ICONINFORMATION
	 int btnCount = SendMessageW(hwTb, TB_BUTTONCOUNT, 0, 0);
	 static int iHideTimerCalls;
	 int curBtn;
	 curBtn = btnCount- ++iHideTimerCalls;
	 PB PinsCurrentBtn =  GetBtnPins(hwTb, curBtn, true);
	 SetBtnPins(hwTb, curBtn, PinsCurrentBtn |= PB_HIDDEN, true);
	 if (curBtn == 0) 
	 {
	 KillTimer(hWnd, HideBtn_TimerId);
	  iHideTimerCalls = 0;
	  };

}
void CALLBACK    ShowBtnsTimerProc(HWND hWnd, UINT msg, UINT_PTR TimerId , DWORD curTime)
{
     MessageBeep (MB_ICONWARNING) ;// MB_OK  MB_ICONINFORMATION
	 int btnCount = SendMessageW(hwTb, TB_BUTTONCOUNT, 0, 0);
	 static int iShowTimerCalls;
	 int curBtn;
	 curBtn = iShowTimerCalls++;
	 PB PinsCurrentBtn =  GetBtnPins(hwTb, curBtn, true);
	 SetBtnPins(hwTb, curBtn, PinsCurrentBtn &=(~PB_HIDDEN), true);
	 if (btnCount - 1 == curBtn) 
	 {KillTimer(hWnd, ShowBtn_TimerId); iShowTimerCalls = 0; };
}


void Define_rcUnderBar(){
	rcUnderBar.left = rcClient.left;
	rcUnderBar.top = rcClient.top + rcTb.bottom - rcTb.top;
	rcUnderBar.right = rcClient.right;
	rcUnderBar.bottom = rcClient.bottom;
}
void Define_rcMsgForUser_Centered(){
	rcMsgForUser_Centered.left = (rcUnderBar.right - sszMsgForUser.cx) / 2;
	rcMsgForUser_Centered.top = (rcUnderBar.bottom - rcUnderBar.top) / 2 + rcTb.bottom - rcTb.top;
	rcMsgForUser_Centered.right = rcMsgForUser_Centered.left + sszMsgForUser.cx;
	rcMsgForUser_Centered.bottom = rcMsgForUser_Centered.top + sszMsgForUser.cy;
}

void Define_rcMsgForUser_CenteredPad(){
	rcMsgForUser_CenteredPad.left = rcMsgForUser_Centered.left - pad.cx;
	rcMsgForUser_CenteredPad.top = rcMsgForUser_Centered.top - pad.cy;
	rcMsgForUser_CenteredPad.right = rcMsgForUser_Centered.right + pad.cx;
	rcMsgForUser_CenteredPad.bottom = rcMsgForUser_Centered.bottom + pad.cy;
}

///////////////////////////////// Slider Stepping Images Painting ///////////////////
void Define_rcImgSliding()
{
	rcImgSliding.top = rcUnderBar.top + topmargin;
	rcImgSliding.left = rcUnderBar.left + leftmargin;
	rcImgSliding.right = rcImgSliding.left + width;
	rcImgSliding.bottom = rcUnderBar.bottom;
}

void MovePicturesFromImageListByTrackbarDown(HDC hdc, HIMAGELIST hImgList, int x, int y, int stepy)
{
	for (int i = 0; i< ImageList_GetImageCount(hImgList); i++)
	{

		ImageList_DrawEx(hImgList, i, hdc,
			x, y + i*stepy, 0, 0,
			CLR_NONE, CLR_NONE, ILD_NORMAL);
		y += imgSpaceSlider;	
	}

}

//TODO:
void MovePicturesFromImageListByTrackbarWrap(HDC hdc, HIMAGELIST hImgList, int x, int y, int step_x, int rightBoundary)
{
	for (int i = 0; i< ImageList_GetImageCount(hImgList); i++)
	{
		ImageList_DrawEx(hImgList, i, hdc,
			(x + i*step_x)%rightBoundary, 
			y,
			0, 0,
			CLR_NONE, CLR_NONE, ILD_NORMAL);
		x += imgSpaceSlider;	
	}
}


	//HIMAGELIST hImgList_current = GetBarImgList(hwTb);
	//int cx, cy;  // define icon size to bind next output to them
	//ImageList_GetIconSize(hImgList_current, &cx, &cy);


	//if (iImg_idBtn + 1) // show pic, to verify image or not (-1)??  TODO //////
		/*ImageList_DrawEx(hImgList_current, iImg_idBtn,
						hdcDst, xLeft, yBottom, 0, 0, 
						CLR_DEFAULT, CLR_DEFAULT, fstyle_imgDraw);*/

//}

// hidden and gap-sep btn, current ImgList, btn count
//void InfoPinaBar()
//{
//	HIMAGELIST hImgList_current = GetBarImgList(hwTb);
//	GetBtnCount(hwTb);
//	// цикл поиска скрытых и разделителей, 
//  GetHiddenBtnCount(hwTb);
//  GetGapCount(hwTb);
// TextOut
// DrawText
//}
