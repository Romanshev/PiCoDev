/////////////////////////////////////////////////////////////////////////////////////////////
// PinaControla.h: header file of PINACONTROLA.dll - developer tool for the composite toolbar building:
// v. 1.1 ( some website links updated, no code changes)

// 1. high performance desktop native development - pure Win32, MFC-free or any other libraries
// 2. any custom control placing - insert "composite" button with external window ( Wnd button, whereas ordinary one is Cmd )
// 3. wrapable styles support (flat and list) and so affordable user interface design
// 4. attractive color scheme design due to effective Custom Draw using ( 5 points gradient filling )
/////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
//  
// Copyright Roman Shevgalishyn 2013 All rights reserved.
//
// Online help on the developer site: http://www.pinacontrola.com/Howto
// Licences briefs:
//				1. Community Edition licence is free of charge for non-commercial use only.
//				1.1. Users of CE have to send valid feedback with their own experience mailto:support@pinacontrola.com 
//				2. Commercial licences are based on the developer seats  
//              3. Details on http://www.pinacontrola.com/Licensing
// Content of PinaControla.h:
//				1. Useful toolbar styles
//				2. Button styles, states and draw styles
//				3. Main functions ( message handlers, bar creation, button inserting (pinning) )
//				4. Helper functions ( button actions: delete, set labels, images etc )
//				5. Color setting functions ( template for all button of toolbar SetBtnTem_ or individual SetBtn_ )
//				6. Useful color constants ( label, border (frame) and 5 points background ) - define your own better!
//
//////////////////////////////////////////////////////////////////////////////////////////////

#ifdef PINACONTROLA
#else
#define PINACONTROLA extern "C" __declspec(dllimport)
#endif 

// 1. ////////////////////////////////////////////////////////////////////////////////////////////
//  Default  toolbar style:
const DWORD TbStyle_Wrap_Flat_List  = 0	| WS_CHILD | WS_VISIBLE| WS_CLIPCHILDREN | WS_CLIPSIBLINGS | TBSTYLE_FLAT
										| TBSTYLE_LIST| TBSTYLE_TOOLTIPS| TBSTYLE_WRAPABLE| CCS_NODIVIDER | CCS_TOP;
// Other useful styles:
const DWORD TbStyle_Wrap_Flat		= 0	| WS_CHILD	| WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS 						
										| TBSTYLE_FLAT 	| TBSTYLE_TOOLTIPS	| TBSTYLE_WRAPABLE |CCS_NODIVIDER |  CCS_TOP;

const DWORD TbStyle_NoWrap_Flat		= 0	| WS_CHILD	| WS_VISIBLE| WS_CLIPCHILDREN | WS_CLIPSIBLINGS	 						
										| TBSTYLE_FLAT 	| TBSTYLE_TOOLTIPS|CCS_NODIVIDER |  CCS_TOP;

const DWORD TbStyle_NoWrap_Flat_List = 0 | WS_CHILD	| WS_VISIBLE| WS_CLIPCHILDREN | WS_CLIPSIBLINGS								
										| TBSTYLE_FLAT 	| TBSTYLE_LIST 	| TBSTYLE_TOOLTIPS|CCS_NODIVIDER |  CCS_TOP ;

const DWORD TbStyle_Wrap_NoFlat_List = 0 | WS_CHILD	| WS_VISIBLE| WS_CLIPCHILDREN | WS_CLIPSIBLINGS								
										| TBSTYLE_WRAPABLE 	| TBSTYLE_LIST 	| TBSTYLE_TOOLTIPS| CCS_TOP ;
// use CCS_BOTTOM if you need 

// Some comments about styles: 
// TBSTYLE_FLAT mandatory for hot-tracking while TBSTYLE_LIST is optional, so on CCS_TOP or CCS_BOTTOM;
// next styles are not recommended for use: 
//1. CSS_VERT, CCS_LEFT, CCS_RIGHT too wide toolbar body 
//2. CCS_ADJUSTABLE|| TBSTYLE_ALTDRAG we not use CreateToolbarEx()
//3. TBSTYLE_TRANSPARENT now is not supported as style, transparency of bar and buttons is default in color settings

//| WS_OVERLAPPED 
//| WS_BORDER	
//| CCS_NOMOVEX 
//| CCS_NOMOVEY 
//| CCS_NOPARENTALIGN 
//| CCS_NORESIZE     
						
// 2. /////////////////////////////////////////////////////////////////////////
typedef unsigned PB; // PB - initial settings of button styles, states and custom draw properties
//--------   See Toolbar Custom Draw return flags in CommCtrl.h ----------------------------------
static const PB PB_Null	   =   0;
static const PB PB_BlendIcon =  (1);			// Use BLENDICON50 at button image ( work for enabled state only )
static const PB PB_NoEdges =  (PB_BlendIcon<<1);// Don't draw button edges
static const PB PB_NoOffset =  (PB_NoEdges <<1);// Don't offset button if pressed
static const PB PB_NoMark   =  (PB_NoOffset<<1);// Don't draw default highlight of image/text for TBSTATE_MARKED
static const PB PB_NoEtched =  (PB_NoMark  <<1);// Don't draw etched effect for disabled items
static const PB PB_NoBk     =  (PB_NoEtched<<1);// Don't draw button bk using clrBtnFace on the non-FLAT bar
static const PB PB_UseCdColors =  (PB_NoBk <<1);// Use Custom Draw Colors to RenderText regardless of Visual Style
////-----  See Toolbar Control and Button Styles in MSDN -----------------------------------------
 static const PB PB_AUTOSIZE = (PB_UseCdColors << 1);// button's width will be calculated based on 
																// the width of  the text plus the image of the button
static const PB PB_BUTTON =  (PB_AUTOSIZE <<1);	// create simple button, should be used to signify
																//  that no other flags of style are set ( 0 of style)
static const PB PB_CHECK =   (PB_BUTTON   <<1);	// dual-state push button
static const PB PB_CHECKGROUP = (PB_CHECK <<1);	// button stays pressed until another button in the group is pressed 															// 
static const PB PB_DROPDOWN =(PB_CHECKGROUP <<1);// drop-down style btn, send a TBN_DROPDOWN notification
static const PB PB_GROUP =   (PB_DROPDOWN <<1);	// When combined with BTNS_CHECK, creates a button that stays
												// pressed until another button in the group is pressed
static const PB PB_NOPREFIX =   (PB_GROUP <<1);	// button text will not have an accelerator prefix associated with it
static const PB PB_SEP =     (PB_NOPREFIX <<1);	// separator, providing a small gap between button groups 
static const PB PB_WHOLEDROPDOWN = (PB_SEP<<1);  // button will have a drop-down arrow, without split arrow section

////-----  See Toolbar Button States in MSDN -----------------------------------------------------------------------
static const PB PB_CHECKED =(PB_WHOLEDROPDOWN <<1);	// button has the BTNS_CHECK style and is being clicked
static const PB PB_ENABLED =(PB_CHECKED <<1);		// button accepts user input, if it doesn't - is grayed
static const PB PB_HIDDEN  =(PB_ENABLED <<1);		// button is not visible and cannot receive user input
static const PB PB_INDETERMINATE =(PB_HIDDEN <<1);	// button is grayed
static const PB PB_MARKED  =(PB_INDETERMINATE <<1);	// button is marked
static const PB PB_PRESSED =(PB_MARKED  <<1);		// button is being clicked
static const PB PB_WRAP    =(PB_PRESSED <<1);		// button is followed by a line break

//--------- Diagnostic settings for PinaBar info functions ---------------------------------------------------
//static const PB PB_GAP    =(PB_WRAP <<1); // disabled empty button is used to divide visually some groups of btns

//////////////////////////////////////////////////////////////////////////////////////////////
// 3. //////  Main functions //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
// CmdBtn - usual toolbar button: simple (standart), checkable or dropdown
// WinBtn - composite button which include some window (control)

// Notification handlers of PINACONTROLA buttons e.g. PinaBar Item Msg Handler

typedef LRESULT (*PinaBtnHandler)( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Place  the next PinaBar message handler at the start block of WndProc and it will retranslate fit messages 
// to the embedded control by calling a fit button handler and CustomDraw service for painting 
PINACONTROLA inline LRESULT MsgHandlerPinaBar	( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
PINACONTROLA inline BOOL	IsThisMsgForPinaBar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Create new toolbar, return the handle
PINACONTROLA HWND CreatePinaBar	(	HWND hwParent,			// parent window handle
									HIMAGELIST hImgList,	// ImageList handle
									DWORD TbStyle = TbStyle_Wrap_Flat_List, // bar style
									HMODULE hModule = GetModuleHandleW(0));	// current instance
													
// Insert (or pin) command button in created PinaBar (HWND hwTb) at iPos position, 
// return command identifier of the new button
PINACONTROLA int PinCmdBtn( 
	HWND hwTb,				// toolbar (PinaBar) handle - where we insert, maybe two or more bars
	PinaBtnHandler pHandler,// pointer to notification handler					
	int iImg =-1,			// index of image in ImageList, by default no image
	const wchar_t* sLabel =0,// label of button
	const wchar_t* sTip  =0, // infotip
	int iPos =-1,			 // null based position index of button on the bar, by default at the end of all buttons
	PB fButton = (PB_AUTOSIZE | PB_ENABLED |PB_NoEdges|PB_UseCdColors) );// button pins (initial setting)


// Insert (or pin) composite button with your control (HWND hwIns) in created PinaBar (HWND hwTb) at iPos position, 
// return command identifier of the new button
// NOTE You must create hwIns as child of the PinaBar window ( hwTb ) or PinaBar's parent ( hwMain )
PINACONTROLA int PinWinBtn( 
	HWND hwTb,				// toolbar (PinaBar) handle - where we insert, maybe two or more bars
	PinaBtnHandler pHandler,// pointer to notification handler
	HWND hwIns,				// which control we try to embed
	int width, int height,	// size of control
	int offset =-1,			// offset from the left border of button, by default we place it after image or label  
	int iImg =-1,			// index of image in ImageList
	const wchar_t* sLabel =0,// label of button
	int iPos  =-1,			// null based position index of button on the bar, by default at the end of all buttons
	PB fButton = (PB_NoOffset | PB_ENABLED | PB_NoEdges | PB_UseCdColors));// button pins (initial setting)

//  Separators are the buttons too				   
PINACONTROLA int PinGap( HWND hwTb, int iPos=-1);// alternate to the standart separator which has side effect (see below),
// Gap is based on standart disabled button without images and may need individual color setting for good-looking
// design. You can set its width using SetBtnWidth(hwTb, idGapName). Unlike separator gap does not wrap its button group .

PINACONTROLA int PinSep( HWND hwTb, int iPos=-1, int width=4);// standart separator is depricated: it causes
													// side effect - too high toolbar on the second (wrapped) shelf

/////////////////////////  Actions on buttons //////////////////////////////////////////////////////
	//--------------  Delete any button ( Cmd or Wnd and assosiated data (and embedded window of course) )
PINACONTROLA void DelBtn(HWND hwTb, int id, bool byIndex = false);	
	//--------------  Return zero-based index of the button position iPos assosiated with button id	
PINACONTROLA  int GetBtnPos( HWND hwTb, int id);
	//--------------- Then move it to the new position, easy-to-use at runtime --------------------
PINACONTROLA void MoveBtnPosFromTo(HWND hwTb, int iPosFrom, int iPosTo);
	//------ If you need to know rect of tb button by id (name or position) in client coordinates of hwTb
	// For using it in popup menu handler converse them to screen through the main app window: see BtnHanlers.cpp
PINACONTROLA RECT GetBtnRect(HWND hwTb, int id, bool byIndex = false);

	//--------------  Label, tip, image and width of button ( ordinary or composite )  -----------
PINACONTROLA void SetBtnLabel( HWND hwTb, int id, const wchar_t* sLabel =0, bool byIndex = false);
PINACONTROLA void SetBtnTip  ( HWND hwTb, int id, const wchar_t* sTip   =0, bool byIndex = false);	
PINACONTROLA void SetBtnImg  ( HWND hwTb, int id, int iImg  =-1,			bool byIndex = false);	
PINACONTROLA void SetBtnWidth( HWND hwTb, int id, int width =-1,			bool byIndex = false);
//=================================================================================================
PINACONTROLA const wchar_t* GetBtnLabel( HWND hwTb, int id,                 bool byIndex = false);
PINACONTROLA const wchar_t* GetBtnTip  ( HWND hwTb, int id,                 bool byIndex = false);	
PINACONTROLA int            GetBtnImg  ( HWND hwTb, int id,   				bool byIndex = false);	
PINACONTROLA int            GetBtnWidth( HWND hwTb, int id,      			bool byIndex = false);

	//---------------- States and Styles of Btn  -------------------------------------------------
PINACONTROLA BYTE           GetBtnState( HWND hwTb, int id,   				bool byIndex = false);	
PINACONTROLA BYTE           GetBtnStyle( HWND hwTb, int id,      			bool byIndex = false);

	//--------------  Pins: style, state and draw style of button ( usual or composite ) ----------
PINACONTROLA void			SetBtnPins( HWND hwTb, int id, PB fButton,		bool byIndex = false);
PINACONTROLA PB				GetBtnPins( HWND hwTb, int id,					bool byIndex = false);

	//--------------  Style of PINACONTROLA  Bar  -------------------------------------------------
PINACONTROLA void  SetBarStyle(HWND hwTb, DWORD dwStyle = TbStyle_Wrap_Flat_List ); 
PINACONTROLA DWORD GetBarStyle(HWND hwTb) ;

	//--------------  ImageLists of PINACONTROLA  Bar  -------------------------------------------------
PINACONTROLA void SetBarImgList(HWND hwTb, HIMAGELIST hImgList);
PINACONTROLA HIMAGELIST GetBarImgList(HWND hwTb);

	//---------------  PinaBar Buttons Info  ----------------------------------------------------------
PINACONTROLA int GetBarBtnCount(HWND hwTb);
PINACONTROLA int GetBarHiddenBtnCount(HWND hwTb);
PINACONTROLA int GetBarGapCount(HWND hwTb);

//===================  Colors of PINACONTROLA  Bar  ===============================================

PINACONTROLA void SetBarBk        ( HWND hwTb,	const COLORREF (*pBarBk)[5]   =0); 
PINACONTROLA void SetBarBorder    ( HWND hwTb,	const COLORREF  *pBarBorder   =0);
PINACONTROLA void SetBarLabelColor( HWND hwTb,	const COLORREF  *pBarLabelClr =0);

PINACONTROLA void SetBarColors	( HWND hwTb,	const COLORREF (*pBarBk)[5] =0, 
												const COLORREF *pBarBorder  =0, 
												const COLORREF *pBarLabelClr=0  );
//PINACONTROLA void SetBarBrMonoDither( HWND hwTb, const COLORREF  *pbrBarMonoDither );// TODO later

	//---------------  Colors of Templates for All Buttons of This Bar ----------- Background
PINACONTROLA void SetBtnTem_HoveredBk ( HWND hwTb, const COLORREF (*pBtnTem_HoveredBk )[5]=0);
PINACONTROLA void SetBtnTem_DefaultBk ( HWND hwTb, const COLORREF (*pBtnTem_DefaultBk )[5]=0);
PINACONTROLA void SetBtnTem_DisabledBk( HWND hwTb, const COLORREF (*pBtnTem_DisabledBk)[5]=0);
PINACONTROLA void SetBtnTem_MarkedBk  ( HWND hwTb, const COLORREF (*pBtnTem_MarkedBk  )[5]=0);
PINACONTROLA void SetBtnTem_CheckedBk ( HWND hwTb, const COLORREF (*pBtnTem_CheckedBk )[5]=0);
PINACONTROLA void SetBtnTem_PressedBk ( HWND hwTb, const COLORREF (*pBtnTem_PressedBk )[5]=0);
PINACONTROLA void SetBtnTem_CheckedHoveredBk ( HWND hwTb, const COLORREF (*pBtnTem_CheckedHoveredBk)[5]=0);

	//---------------------------------------------------------------------------   Border 
PINACONTROLA void SetBtnTem_HoveredBorder  ( HWND hwTb, const COLORREF *pBtnTem_HoveredBorder =0 );
PINACONTROLA void SetBtnTem_DefaultBorder  ( HWND hwTb, const COLORREF *pBtnTem_DefaultBorder =0 );
PINACONTROLA void SetBtnTem_DisabledBorder ( HWND hwTb, const COLORREF *pBtnTem_DisabledBorder=0 );
PINACONTROLA void SetBtnTem_MarkedBorder   ( HWND hwTb, const COLORREF *pBtnTem_MarkedBorder  =0 );
PINACONTROLA void SetBtnTem_CheckedBorder  ( HWND hwTb, const COLORREF *pBtnTem_CheckedBorder =0 );
PINACONTROLA void SetBtnTem_PressedBorder  ( HWND hwTb, const COLORREF *prBtnTem_PressedBorder=0 );
PINACONTROLA void SetBtnTem_CheckedHoveredBorder ( HWND hwTb, const COLORREF *pBtnTem_CheckedHoveredBorder=0);

	//---------------------------------------------------------------------------   Labels
PINACONTROLA void SetBtnTem_HoveredLabelColor  ( HWND hwTb, const COLORREF *pBtnTem_HoveredLabelClr =0 );
PINACONTROLA void SetBtnTem_DefaultLabelColor  ( HWND hwTb, const COLORREF *pBtnTem_DefaultLabelClr =0);
PINACONTROLA void SetBtnTem_DisabledLabelColor ( HWND hwTb, const COLORREF *pBtnTem_DisabledLabelClr=0);
PINACONTROLA void SetBtnTem_MarkedLabelColor   ( HWND hwTb, const COLORREF *pBtnTem_MarkedLabelClr  =0);
PINACONTROLA void SetBtnTem_CheckedLabelColor  ( HWND hwTb, const COLORREF *pBtnTem_CheckedLabelClr =0);
PINACONTROLA void SetBtnTem_PressedLabelColor  ( HWND hwTb, const COLORREF *prBtnTem_PressedLabelClr=0);
PINACONTROLA void SetBtnTem_CheckedHoveredLabelColor ( HWND hwTb, const COLORREF *pBtnTem_CheckedHoveredLabelClr=0);


//===========  Colors of individual Buttons ( by id or byIndex ) surpass any templates above =======================

	//-----------------------------------------------------------------------------------------------------  Background
PINACONTROLA void SetBtn_HoveredBk  ( HWND hwTb, int id, const COLORREF (*pBtn_HoveredBk )[5]=0, bool byIndex = false);
PINACONTROLA void SetBtn_DefaultBk  ( HWND hwTb, int id, const COLORREF (*pBtn_DefaultBk )[5]=0, bool byIndex = false);
PINACONTROLA void SetBtn_DisabledBk ( HWND hwTb, int id, const COLORREF (*pBtn_DisabledBk )[5]=0, bool byIndex = false);
PINACONTROLA void SetBtn_MarkedBk   ( HWND hwTb, int id, const COLORREF (*pBtn_MarkedBk  )[5]=0, bool byIndex = false);
PINACONTROLA void SetBtn_CheckedBk  ( HWND hwTb, int id, const COLORREF (*pBtn_CheckedBk )[5]=0, bool byIndex = false);
PINACONTROLA void SetBtn_PressedBk  ( HWND hwTb, int id, const COLORREF (*pBtn_PressedBk )[5]=0, bool byIndex = false);
PINACONTROLA void SetBtn_CheckedHoveredBk( HWND hwTb, int id, const COLORREF (*pBtn_CheckedHoveredBk )[5], bool byIndex = false);

	//---------------------------------------------------------------------------------------------------------- Border
PINACONTROLA void SetBtn_HoveredBorder ( HWND hwTb, int id, const COLORREF *pBtn_HoveredBorder=0, bool byIndex = false);
PINACONTROLA void SetBtn_DefaultBorder ( HWND hwTb, int id, const COLORREF *pBtn_DefaultBorder=0, bool byIndex = false);
PINACONTROLA void SetBtn_DisabledBorder( HWND hwTb, int id, const COLORREF *pBtn_DisabledBorder=0, bool byIndex = false);
PINACONTROLA void SetBtn_MarkedBorder  ( HWND hwTb, int id, const COLORREF *pBtn_MarkedBorder =0, bool byIndex = false);
PINACONTROLA void SetBtn_CheckedBorder ( HWND hwTb, int id, const COLORREF *pBtn_CheckedBorder=0, bool byIndex = false);
PINACONTROLA void SetBtn_PressedBorder ( HWND hwTb, int id, const COLORREF *pBtn_PressedBorder=0, bool byIndex = false);
PINACONTROLA void SetBtn_CheckedHoveredBorder ( HWND hwTb, int id, 
															const COLORREF *pBtn_CheckedHoveredBorder=0, bool byIndex = false);

	//----------------------------------------------------------------------------------------------------------- Labels
PINACONTROLA void SetBtn_DefaultLabelColor  ( HWND hwTb, int id, const COLORREF *pBtn_DefaultLabelClr=0, bool byIndex = false );
PINACONTROLA void SetBtn_DisabledLabelColor ( HWND hwTb, int id, const COLORREF *pBtn_DisabledLabelClr=0, bool byIndex = false );
PINACONTROLA void SetBtn_HoveredLabelColor  ( HWND hwTb, int id, const COLORREF *pBtn_HoveredLabelClr=0, bool byIndex = false );
PINACONTROLA void SetBtn_MarkedLabelColor   ( HWND hwTb, int id, const COLORREF *pBtn_MarkedLabelClr=0,  bool byIndex = false );
PINACONTROLA void SetBtn_CheckedLabelColor  ( HWND hwTb, int id, const COLORREF *pBtn_CheckedLabelClr=0, bool byIndex = false );
PINACONTROLA void SetBtn_PressedLabelColor  ( HWND hwTb, int id, const COLORREF *pBtn_PressedLabelClr=0, bool byIndex = false );
PINACONTROLA void SetBtn_CheckedHoveredLabelColor(HWND hwTb, int id, 
																 const COLORREF *pBtn_CheckedHoveredLabelClr=0,	
																										bool byIndex = false );

//=========== end of the Colors of Bar and Items ===============================================================

//////////////////////////////////////////////////////////////////////////////////////////////
//								Custom colors for the Bar and Buttons    
//////////////////////////////////////////////////////////////////////////////////////////////
const COLORREF Bk_Dark_Violet[5] =	{
										RGB( 130,  22, 133 ),
										RGB( 130,  22, 133 ),
										RGB( 113,  18, 115 ),
										RGB( 113,  18, 115 ),
										RGB( 115,  21, 127 )};
const COLORREF Bk_Light_Violet[5] =	{
										RGB( 120, 18, 125 ),
										RGB( 120, 18, 125 ),
										RGB( 143, 28, 155 ),
										RGB( 143, 28, 155 ),
										RGB( 133, 21, 130 )};	
const COLORREF Bk_Blue_Violet[5] =	{
										RGB( 130,  22, 233 ),// Blue
										RGB( 130,  22, 233 ),// Blue
										RGB( 113,  18, 115 ),
										RGB( 113,  18, 115 ),
										RGB( 115,  21, 127 )};
const COLORREF Bk_Violet_Blue[5] =	{
										RGB( 130,  22, 183 ),
										RGB( 130,  22, 183 ),
										RGB( 113,  18, 165 ),
										RGB( 113,  18, 165 ),
										RGB( 115,  21, 227 )};// CR Blue

const COLORREF Border_Violet    =       RGB( 130,  22, 133);
const COLORREF Border_Violet_Blue =     RGB( 130,  22, 233);
const COLORREF Label_Violet_Blue  =     RGB( 130,  22, 233);



const COLORREF Border_Ruby		= RGB(255,100,100); // crimson, blood red, reddish
const COLORREF Border_Std		= RGB(255,183,183);
const COLORREF Border_Cian		= RGB(  0,235,235);
const COLORREF Border_Lemon		= RGB(200,255,  0);
const COLORREF Border_Green		= RGB(  0,255,  0); 
const COLORREF Border_BlackRed	= RGB(150, 15, 10);

const COLORREF Border_Blue			= RGB( 50, 55,255); 
const COLORREF Border_Pink			= RGB(250,200,190);
const COLORREF Border_White			= RGB(255,235,225);
const COLORREF Border_Light_Blue	= RGB( 55,155,255);
const COLORREF Border_LightYellow	= RGB(250,255,155);
const COLORREF Border_SolidYellow	= RGB(255,200, 55);


const COLORREF Label_White			= RGB(255,235,225);
const COLORREF Label_Black			= RGB(0,0,0);
const COLORREF Label_Blue			= RGB( 50, 55,255); 
const COLORREF Label_Pink			= RGB(250,200,190); 
const COLORREF Label_Cian			= RGB(  0,235,235);
const COLORREF Label_Ruby			= RGB(255,100,100); 
const COLORREF Label_Lemon			= RGB(200,255,  0);
const COLORREF Label_Light_Blue		= RGB( 55,155,255);
const COLORREF Label_SolidYellow	= RGB(255,200, 55);

const COLORREF Bk_Rain[5]	=	{ 	RGB(165,165,187),
									RGB(155,155,187),
									RGB(144,144,155),
									RGB(144,144,155),
									RGB(138,138,146)	}; //(LT TR BR BL CR)
const COLORREF Bk_GrayStd[5] =	{	
									RGB(243,243,243),
									RGB(243,243,243),
									RGB(193,193,193),
									RGB(193,193,193),
									RGB(200,200,200)	};
const COLORREF Bk_Blue[5]	=	{			
									RGB( 120, 120, 253),
									RGB( 130, 130, 253),
									RGB(  10,  10, 125),
									RGB(  10,  10, 135),
									RGB(  60,  60, 225)	};
const COLORREF Bk_Black[5] =	{		
									RGB(  80,  80,  80),
									RGB(  80,  80,  80),
									RGB( 120, 120, 120),
									RGB( 120, 120, 120),
									RGB(  90,  90,  90)	};

const COLORREF Bk_Emerald[5] =	{
									RGB(   0,  150, 143),
									RGB(   0,  150, 143),
									RGB(   0, 160,   0),
									RGB(   0, 170,   0),
									RGB( 120, 220, 120)	};
const COLORREF Bk_Yellow[5] =	{
									RGB( 243, 255, 255),
									RGB( 243, 255, 255),
									RGB( 193, 203,   0),
									RGB( 193, 203,   0),
									RGB( 220, 220, 220)	};
const COLORREF Bk_Std[5] =		{
									RGB(   0, 255, 255),
									RGB(   0, 255, 255),
									RGB( 193, 203,   0),
									RGB( 193, 203,   0),
									RGB( 220, 220, 220)	};
const COLORREF Bk_Black_Red[5] =	{
										RGB(  80,  80,  80),
										RGB(  80,  80,  80),
										RGB( 150,  15,  10),
										RGB( 150,  15,  10),
										RGB( 130,  45,  20)	};
const COLORREF Bk_Grey_Red[5] =		{                        
										RGB(  80,  80,  80),
										RGB(  80,  80,  80),
										RGB( 180,  40, 30),
										RGB( 180,  40, 30),
										RGB( 160,  90,  40)	};
const COLORREF Bk_Black_White[5] =	{
										RGB(  80,  80,  80),
										RGB(  80,  80,  80),
										RGB( 150, 150, 150),
										RGB( 150, 150, 150),
										RGB( 110, 110, 110)	};
const COLORREF Bk_Black_Yellow[5] =	{
										RGB(  80,  80,  80),
										RGB(  80,  80,  80),
										RGB( 230, 220, 110),
										RGB( 230, 220, 110),
										RGB( 170, 130, 80 )	};
const COLORREF Bk_Yellow_Black[5] =	{
										RGB( 230, 220, 110),
										RGB( 230, 220, 110),
										RGB( 170, 130,  80),
										RGB(  80,  80,  80),
										RGB(  80,  80,  80)	};	
const COLORREF Bk_Blue_Black[5] =	{
										RGB( 110, 110, 240 ),
										RGB( 110, 110, 240 ),
										RGB( 100, 100, 140 ),
										RGB(  80,  80,  50 ),
										RGB(  80,  80,  50 )};
