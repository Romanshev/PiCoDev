// PinaControla.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#define PINACONTROLA extern "C" __declspec(dllexport) 
#include "PinaControla.h"

///////////////////////////////////////////////

const int Aux_GradientRectVertexNumber = 5;
void GradientRect(HDC hdc, RECT* pRc, const COLORREF VertexColor[Aux_GradientRectVertexNumber]);
void FrameRect(HDC hdc, RECT* pRc, COLORREF rgb);

bool bSlowRedraw;
int  nSlowRedraw;
//================= new items from nuibar.h
static const wchar_t* const wcsUIBar = L"PinaBarObj";

static BYTE getState(PB fff)// get state of btn in terms of TBSTATE_  to trans PB to TOOLBARINFO
{
	return (0
			| ((fff & PB_CHECKED) ? TBSTATE_CHECKED   : 0)
			| ((fff & PB_ENABLED) ? TBSTATE_ENABLED   : 0)
			| ((fff & PB_HIDDEN)  ? TBSTATE_HIDDEN    : 0)
			| ((fff & PB_INDETERMINATE) ? TBSTATE_INDETERMINATE : 0)
			| ((fff & PB_MARKED)  ? TBSTATE_MARKED    : 0)
			| ((fff & PB_PRESSED) ? TBSTATE_PRESSED   : 0)
			| ((fff & PB_WRAP)    ? TBSTATE_WRAP      : 0)			
			);
}

static LRESULT getDrawStyle(PB fff) 
{
	return (0
			| ((fff & PB_BlendIcon) ? TBCDRF_BLENDICON      : 0)// Use ILD_BLEND50 on the icon image
			| ((fff & PB_NoEdges)   ? TBCDRF_NOEDGES        : 0)// Don't draw button edges
			| ((fff & PB_NoOffset)  ? TBCDRF_NOOFFSET       : 0)// Don't offset button if pressed
			| ((fff & PB_NoMark)    ? TBCDRF_NOMARK         : 0)// Don't draw default hlt of image/text for TBSTATE_MARKED or dithered !!
			| ((fff & PB_NoEtched)  ? TBCDRF_NOETCHEDEFFECT : 0)// Don't draw etched effect for disabled items
			| ((fff & PB_NoBk)      ? TBCDRF_NOBACKGROUND   : 0)// Don't draw button bk
			| ((fff & PB_UseCdColors) ? TBCDRF_USECDCOLORS  : 0)// Use Custom Draw Colors to render labels regardless of VisualSt
			);
}
// add TBCDRF_NOBACKGROUND if we are going to paint bk ourselves, 
// not use it if define clrBtnFace, clrBtnHighlight, clrMark and clrHighlightHotTrack ( use with TBCDRF_HILITEHOTTRACK )
// later we must add TBCDRF_HILITEHOTTRACK  and this staff ^^^.
static BYTE getStyle(PB fff)
{
	return (0
			| ((fff & PB_AUTOSIZE)		? BTNS_AUTOSIZE : 0)
			| ((fff & PB_BUTTON)		? BTNS_BUTTON	: 0)//This flag is defined as 0, and should be used to 
			| ((fff & PB_CHECK)			? BTNS_CHECK	: 0)//^^^^^^^^^^^^signify that no other flags are set.
			| ((fff & PB_CHECKGROUP)	? BTNS_CHECKGROUP : 0)
			| ((fff & PB_DROPDOWN)		? BTNS_DROPDOWN : 0)
			| ((fff & PB_GROUP)			? BTNS_GROUP	: 0)
			| ((fff & PB_NOPREFIX)		? BTNS_NOPREFIX : 0)
			| ((fff & PB_SEP)			? BTNS_SEP		: 0)			
			| ((fff & PB_WHOLEDROPDOWN) ? BTNS_WHOLEDROPDOWN : 0)
			);		       
}

//================= 

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
// Core type for all UIBar objects
class IUIBar
{	
public:
	virtual ~IUIBar(){;}	
};

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
class CUIBar;
//////////////////////////////////////////////////////////////////////////////////////////////
// Notification handler
//typedef LRESULT (*PUIBarItemMsgHandler)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//--------------------------------------------------------------------------------------------
// Toolbar button data
class IUIBarData : virtual public IUIBar
{
protected:
	wchar_t* wcsText;
	wchar_t* wcsTip;

public:
	CUIBar* pUIBar; // pointer to owner UIBar
	PB fButton;
	PinaBtnHandler pHandler;		// Items - composite WndBtn or usual CmdBtn

										// ----------------- Bk and Borders ------------------
	const COLORREF (*pBtn_HoveredBk)[5];    
	const COLORREF*  pBtn_HoveredBorder;	
	
	const COLORREF (*pBtn_PressedBk)[5];	
	const COLORREF*  pBtn_PressedBorder;	
	
	const COLORREF (*pBtn_DefaultBk)[5];	
	const COLORREF*  pBtn_DefaultBorder;	

	const COLORREF (*pBtn_DisabledBk)[5];	
	const COLORREF*  pBtn_DisabledBorder;	
	
	const COLORREF (*pBtn_CheckedBk)[5];	
	const COLORREF*  pBtn_CheckedBorder;	

	const COLORREF (*pBtn_CheckedHoveredBk)[5];	
	const COLORREF*  pBtn_CheckedHoveredBorder;	

	const COLORREF (*pBtn_MarkedBk)[5];	
	const COLORREF*  pBtn_MarkedBorder;	
										// -------------  Label colors  --------------------------
	const COLORREF* pBtn_DefaultLabelClr;	    
	const COLORREF* pBtn_DisabledLabelClr;   

	const COLORREF* pBtn_CheckedLabelClr;		
	const COLORREF* pBtn_CheckedHoveredLabelClr;

	const COLORREF* pBtn_HoveredLabelClr;   
	const COLORREF* pBtn_MarkedLabelClr;    

	const COLORREF* pBtn_PressedLabelClr;  	


	//---------------------------------------------------------------
	IUIBarData()
	{ 
		pUIBar  =0;
		wcsText =0;
		wcsTip  =0;
		fButton = PB_Null;
		pHandler =0;

		pBtn_HoveredBk		=0;
		pBtn_HoveredBorder	=0;		

		pBtn_DefaultBk		=0;
		pBtn_DefaultBorder	=0;

		pBtn_DisabledBk		=0;
		pBtn_DisabledBorder	=0;

		pBtn_CheckedBk		=0;  
		pBtn_CheckedBorder	=0;

		pBtn_CheckedHoveredBk		=0;  
		pBtn_CheckedHoveredBorder	=0; 

		pBtn_MarkedBk		=0;  
		pBtn_MarkedBorder	=0; 

		pBtn_PressedBk		=0;
		pBtn_PressedBorder	=0;
								// ------ Label clrs ----------------------------
		pBtn_CheckedLabelClr	=0;		 
		pBtn_CheckedHoveredLabelClr =0;

		pBtn_DefaultLabelClr	=0; 
		pBtn_DisabledLabelClr	=0; 

		pBtn_HoveredLabelClr	=0;
		pBtn_MarkedLabelClr		=0;		
		
		pBtn_PressedLabelClr	=0; 
		
	}

	virtual ~IUIBarData()
	{
		if(wcsText) { delete[] wcsText;}
		if(wcsTip ) { delete[] wcsTip; }
	}
	//---------------------------------------------------------------
	void setText(const wchar_t* wcsText)
	{		
		if(this->wcsText)
		{ 
			delete[] this->wcsText;
					 this->wcsText =0;
		}

		if(wcsText && wcslen(wcsText))
		{
				   this->wcsText = new wchar_t[wcslen(wcsText)+1];
				   
			wcscpy(this->wcsText, wcsText);
		}
	}
	//---------------------------------------------------------------
	const wchar_t* getText() const { return wcsText; }

	//---------------------------------------------------------------
	void setTip(const wchar_t* wcsTip)
	{
		if(this->wcsTip)
		{ 
			delete[] this->wcsTip;
					 this->wcsTip =0;
		}

		if(wcsTip && wcslen(wcsTip))
		{
				   this->wcsTip = new wchar_t[wcslen(wcsTip)+1];
			wcscpy(this->wcsTip, wcsTip);
		}
	}
	//---------------------------------------------------------------
	const wchar_t* getTip() const { return wcsTip; }
	//---------------------------------------------------------------
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Button data of the placeholder for the external window used with insertWnd
class CUIBarDataWnd : virtual public IUIBarData
{
public:
	HWND hWnd;
	int width, height, offset; // desired window dimensions and offset
	//------------------------------------------------------------------
	CUIBarDataWnd(PinaBtnHandler pHandler, HWND hWnd, int width, int height, int offset, PB fButton)
	{
		this->hWnd = hWnd;
		this->width = width;
		this->height = height;
		this->offset = offset;
		this->fButton = fButton;
		this->pHandler = pHandler;

		if(0== SetPropW(hWnd, wcsUIBar, (HANDLE)(dynamic_cast<IUIBar*>(this)))) { throw(-1); } 
		// SetProp for insWnd
	}
	//------------------------------------------------------------------
	~CUIBarDataWnd() { if(hWnd){ DestroyWindow(hWnd); } }
};

//////////////////////////////////////////////////////////////////////////////////////////////
// usual button data: handler - tip - flags
class CUIBarDataCmd : virtual public IUIBarData
{
public:

	CUIBarDataCmd(PinaBtnHandler pHandler, const wchar_t* wcsTip, PB fButton)
	{
		this->pHandler = pHandler;
		setTip(wcsTip);
		this->fButton = fButton;
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
class CUIBar : virtual public IUIBar
{
	static const int iCmdStart = 10000; //To provide ability assign iCommand for button in range(0-9999)
	int iCmd; // for auto command id - toolbar requre a unique idCommand for the buttons, 
			  // for example, for the ToolTip functionality

public:
	HMODULE hModule;	// calling instance --
	HWND	hwTb;		// Toolbar Window handle

	 
	const COLORREF (*pBarBk)[5];  // Background of the toolbar pBarBk 
	const COLORREF* pBarBorder;   // Border of the normal state pBarBorder
	const COLORREF* pBarLabelClr; // Label color of normal (default) state 

	const COLORREF* pBtnTem_HoveredLabelClr;    
	const COLORREF* pBtnTem_DefaultLabelClr; 
	const COLORREF* pBtnTem_DisabledLabelClr;

	const COLORREF* pBtnTem_CheckedLabelClr;
	const COLORREF* pBtnTem_CheckedHoveredLabelClr;
	const COLORREF* pBtnTem_MarkedLabelClr;

	const COLORREF* pBtnTem_PressedLabelClr;	

	// --------------- Color Templates of the Button States---------------

	// --------------- Background Templates -------------------------------
		
	const COLORREF (*pBtnTem_HoveredBk )[5];  
	const COLORREF (*pBtnTem_DefaultBk )[5];  
	const COLORREF (*pBtnTem_DisabledBk )[5];  
	const COLORREF (*pBtnTem_CheckedBk )[5];  
	const COLORREF (*pBtnTem_CheckedHoveredBk )[5]; 
	const COLORREF (*pBtnTem_MarkedBk  )[5];  
	const COLORREF (*pBtnTem_PressedBk )[5];  


	// --------------- Border Templates -----------------------------------
	const COLORREF* pBtnTem_HoveredBorder;
	const COLORREF* pBtnTem_DefaultBorder;
	const COLORREF* pBtnTem_DisabledBorder;
	const COLORREF* pBtnTem_MarkedBorder;	
	const COLORREF* pBtnTem_CheckedBorder;
	const COLORREF* pBtnTem_CheckedHoveredBorder;
	const COLORREF* pBtnTem_PressedBorder;
	
	//------------------------------------------------------------------
	//CUIBar(HWND hwParent, HIMAGELIST hImgList, 
	//       DWORD TbStyle = TbStyle_Default, HMODULE hModule = GetModuleHandleW(0))
	
	CUIBar(HWND hwParent, HIMAGELIST hImgList, DWORD TbStyle, HMODULE hModule)
	{
		INITCOMMONCONTROLSEX icex;
		icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icex.dwICC  = ICC_BAR_CLASSES;
		InitCommonControlsEx(&icex);

		this->hModule = hModule;	

		pBarBk  =0;
		pBarBorder =0;
		pBarLabelClr =0;

		pBtnTem_HoveredLabelClr =0;
		pBtnTem_DefaultLabelClr  =0;
		pBtnTem_DisabledLabelClr  =0;
		pBtnTem_MarkedLabelClr =0;
		pBtnTem_CheckedLabelClr=0;
		pBtnTem_CheckedHoveredLabelClr=0;
		pBtnTem_PressedLabelClr=0;	
		
		pBtnTem_HoveredBk =0;
		pBtnTem_DefaultBk =0;	
		pBtnTem_DisabledBk=0;	
		pBtnTem_MarkedBk  =0;
		pBtnTem_CheckedBk =0;
		pBtnTem_CheckedHoveredBk=0;
		pBtnTem_PressedBk =0;

		pBtnTem_HoveredBorder =0;
		pBtnTem_DefaultBorder =0;
		pBtnTem_DisabledBorder=0;
		pBtnTem_MarkedBorder  =0;	
		pBtnTem_CheckedBorder =0;
		pBtnTem_CheckedHoveredBorder=0;
		pBtnTem_PressedBorder =0;

		//------------------- Создаем тулбар     ----------------------------------------
		hwTb = CreateWindowExW( WS_EX_COMPOSITED, //0, //Composited - double-buffering for non-flickering
								TOOLBARCLASSNAMEW,
								NULL,
								TbStyle,
								0, 0, 0, 0,
								hwParent, 
								0,      // no menu
								hModule,// hInstance
								NULL);
		//-------------------------------------------------------------------------------
		if(0== (hwTb && SetPropW(hwTb, wcsUIBar, (HANDLE)dynamic_cast<IUIBar*>(this)))) { throw(-1); }
		//-------------------------------------------------------------------------------
		// Send the TB_BUTTONSTRUCTSIZE message, which is required for backward compatibility if CreateWinEx is used
		SendMessageW( hwTb, TB_BUTTONSTRUCTSIZE, (WPARAM) sizeof(TBBUTTON), 0); 
		
		SendMessageW( hwTb, TB_SETEXTENDEDSTYLE, 0, (LPARAM)(DWORD) (TBSTYLE_EX_DRAWDDARROWS|TBSTYLE_EX_DOUBLEBUFFER));	
		SendMessageW( hwTb, TB_SETIMAGELIST, 0, (LPARAM)hImgList);

		layout();

		iCmd = iCmdStart;
	}
	//----------------------------------------------------------------------------------------
	~CUIBar()
	{
		if(hwTb)
		{
			for(int cnt = SendMessageW( hwTb, TB_BUTTONCOUNT, 0, 0); --cnt >=0; )
			{
				TBBUTTON tb ={0};
				if(SendMessageW( hwTb, TB_GETBUTTON, (WPARAM)cnt, (LPARAM)(TBBUTTON*) &tb))
				{
					delete ((IUIBarData*)tb.dwData);
				}
				else { break; }
			}
			//---------------------------------------------------------------
			DestroyWindow(hwTb);
		}
	}
	

protected:
	//----------------------------------------------------------------------------------------
	// Insert the buttons to the toolbar at iButton pos
	// iImg =(-1) - for no image, we set iBitmap to I_IMAGENONE to indicate that the button does not have an image	
	// iPos = (-1) - to insert at the end
	// Return command identifier of the new buton
	int _insertButton(IUIBarData* pData, int iImg, const wchar_t* sText, int iPos )//=-1 =0  =-1
	{
		pData->pUIBar =this;   //
		pData->setText(sText); //
		
		int cnt = SendMessageW( hwTb, TB_BUTTONCOUNT, 0, 0);

		if((iPos <0) || (iPos >cnt)) { iPos = cnt; } // get to current  counter

		//--------------------------------------------------------------
		TBBUTTON tbb ={0};
		tbb.iBitmap = ((iImg <0) ? I_IMAGENONE : iImg); 
		tbb.idCommand = ++iCmd; 
		tbb.fsState   = getState(pData->fButton); 
		tbb.fsStyle   = getStyle(pData->fButton); 
		tbb.iString   = (  INT_PTR) pData->getText();
		tbb.dwData    = (DWORD_PTR) pData;            // important! App save data here

		SendMessageW(hwTb, TB_INSERTBUTTON, (WPARAM) iPos, (LPARAM) (LPTBBUTTON) &tbb);
		//--- Zero-based index of a button iPos. The message inserts the new button to the left of this button   ----------
		layout();

		return iCmd;
	}

public:
	//------------------------------------------------------------------
	// Insert command button at iPos pos
	// Return command identifier of the new button
	int insertCmd( PinaBtnHandler pHandler,
					int iImg , const wchar_t* sText, const wchar_t* wcsTip, int iPos ,// =-1 =0 =0 =-1					
					PB fButton  )//= (PB_AUTOSIZE | PB_BUTTON | PB_ENABLED )
	{
		int idCmd = _insertButton(new CUIBarDataCmd(pHandler, wcsTip, fButton),
									iImg, sText, iPos);
		layout();
		return idCmd;
	} // if iImg=-1 must be only text, but if fstyle = BTNS_SEP from v. 5.8 cx_width = iBitmap in px,
	// when iBitmap > last num in imagelist, cx = pic placeholder
	



	//------------------------------------------------------------------
	// Insert external window (for example - EDIT control) at iPos pos
	// width, height, offset - desired window dimensions and offset relative left border
	// if offset  = -1, then hwIns will placed after last element, (image or text)
	// Return command identifier of the new button
	// NOTE You must create hwIns as child of the UIBar window or UIBar's parent 

	int insertWnd( PinaBtnHandler pHandler,
					HWND hwIns, int width, int height, 
					int offset,	int iImg, const wchar_t* sText, int iPos,// =-1, =-1, =0,=-1
					PB fButton  )//= (PB_ENABLED|PB_NoEdges|PB_NoOffset|PB_NoEtched) ?
					
	{
		int idCmd = _insertButton( new CUIBarDataWnd(pHandler, hwIns, width, height, offset, fButton),
								iImg, sText, iPos);
		SetParent(hwIns, hwTb);
		layout();
		return idCmd;
	}

	//------------------------- The way to another imgList using =====================
	void setBarImgList(HIMAGELIST hImgList) 
	{		
		if (hImgList)
		{			
			SendMessageW(hwTb, TB_SETIMAGELIST, 0, (LPARAM)(hImgList));		
		}
		layout();		
	}
	HIMAGELIST getBarImgList() { return (HIMAGELIST) SendMessageW(hwTb, TB_GETIMAGELIST, 0, 0); }
	// if some imglists such index we must point in wparam ... TB_GETIMAGELIST, 1, 0);

	//------------------------- The way to another wrapable style lay down via ~wrapable one =====================
	void setBarStyle( DWORD dwStyle )//--= TbStyle_Default ) 
	{ 		
		if(dwStyle & TBSTYLE_WRAPABLE)
		{			
		SendMessageW( hwTb, TB_SETSTYLE, 0, (LPARAM) ( dwStyle & (~TBSTYLE_WRAPABLE) ) );
		SendMessageW( hwTb, TB_SETSTYLE, 0, (LPARAM)   dwStyle );		
		}		
		else SendMessageW( hwTb, TB_SETSTYLE, 0, (LPARAM) dwStyle ); 		
		layout(); 		
	}

	DWORD getBarStyle() { return SendMessageW( hwTb, TB_GETSTYLE, 0, 0); }

	//------------------------------------------------------------------
	void deleteButton(int id, bool byIndex  = false)  
	{
		if(!byIndex) { id = SendMessageW(hwTb, TB_COMMANDTOINDEX, (WPARAM)id, 0); } // byName to byInd

		if(IUIBarData* pData = getItemData(id, true)) { delete pData; }
		//-----------------------------------------------------------
		SendMessageW( hwTb, TB_DELETEBUTTON, (WPARAM) (int) id, 0);
		layout();
	}

	//------------------------------------------------------------------
	IUIBarData* getItemData(int id, bool byIndex = false)    
	{
		TBBUTTONINFOW tbi ={0};
		tbi.cbSize = sizeof(TBBUTTONINFOW);
		tbi.dwMask = TBIF_LPARAM | (byIndex ? TBIF_BYINDEX : 0); //	byIndex
		
		SendMessageW(hwTb, TB_GETBUTTONINFOW, (WPARAM) id, (LPARAM) (TBBUTTONINFOW*) &tbi);

		return reinterpret_cast<IUIBarData*>(tbi.lParam);
	}

	//------------------------------------------------------------------
	BYTE  getBtnStyle(int id, bool byIndex = false)// for  user
	{
		TBBUTTONINFOW tbi = { 0 }; // sys fills it from tbb
		tbi.cbSize = sizeof(TBBUTTONINFOW);
		tbi.dwMask = TBIF_STYLE | (byIndex ? TBIF_BYINDEX : 0);	   //	byIndex
		SendMessageW(hwTb, TB_GETBUTTONINFOW, (WPARAM)id, (LPARAM)(TBBUTTONINFOW*)&tbi);
		return tbi.fsStyle;
	}/////////////////////

	BYTE  getBtnPins(const IUIBarData* pData)// for tool dev
	{
		return pData->fButton;
	}/////////////////////

	// Return zero-based index of the button assosiated with given data ( for tool dev)
	int getBtnPos(const IUIBarData* pData) 
	{
		TBBUTTON tb ={0};
		int cnt = SendMessageW ( hwTb, TB_BUTTONCOUNT, 0, 0);
		for( ; --cnt >=0; )
		{
			if( SendMessageW( hwTb, TB_GETBUTTON, (WPARAM)cnt, (LPARAM)(TBBUTTON*) &tb) )
			{
				if(pData == reinterpret_cast<IUIBarData*>(tb.dwData)) { break; }
			}
		}
		return cnt;
	}
	//------------------------------------------------------------------
	int getBarBtnCount()
	{
		return SendMessageW ( hwTb, TB_BUTTONCOUNT, 0, 0);
	}
	int getBarHiddenBtnCount()
	{
		TBBUTTON tb ={0};
		int hiddenBtnCount=0;
		int cnt = SendMessageW ( hwTb, TB_BUTTONCOUNT, 0, 0);
		for( ; --cnt >=0; )
		{
			if( SendMessageW( hwTb, TB_GETBUTTON, (WPARAM)cnt, (LPARAM)(TBBUTTON*) &tb) )
			{
				if(tb.fsState & TBSTATE_HIDDEN) { ++hiddenBtnCount; }
			}
		}
		return hiddenBtnCount;	
	}

	//---------------------------------------------------------------------------------
	//int getBarGapCount()////////////////////////////// mine
	//{
	//	TBBUTTON tb ={0};
	//	int gapCount=0;
	//	int cnt = SendMessageW ( hwTb, TB_BUTTONCOUNT, 0, 0);
	//	for( ; --cnt >=0; )
	//	{
	//		if( SendMessageW( hwTb, TB_GETBUTTON, (WPARAM)cnt, (LPARAM)(TBBUTTON*) &tb) )
	//		{
	//			IUIBarData* pData=reinterpret_cast<IUIBarData*>(tb.dwData);
	//			if( pData->fButton & PB_GAP)  ++gapCount; 
	//		}
	//	} 
	//	return gapCount;
	//}
	//---------------------------------------------------------------------------------
	int getBarDisableCount()
	{
		TBBUTTON tb ={0};
		int disableCount=0;
		int cnt = SendMessageW ( hwTb, TB_BUTTONCOUNT, 0, 0);
		for( ; --cnt >=0; )
		{
			if( SendMessageW( hwTb, TB_GETBUTTON, (WPARAM)cnt, (LPARAM)(TBBUTTON*) &tb) )
			{
				if(tb.fsState & (~TBSTATE_ENABLED)) { ++disableCount; }
			}
		} 
		return disableCount;
	}
	//------------------------------------------------------------------
	// Return zero-based index of the button assosiated with command idCmd ( id )
	int getCmdPos(int idCmd)
	{
		return SendMessageW( hwTb, TB_COMMANDTOINDEX, (WPARAM)idCmd, 0);
	}
	//------------------------------------------------------------------
	// Return pRc of button with idCmd
	// not use for TB_HIDDEN

	RECT getBtnRc(int id, bool byIndex=false) // might use TB_GETITEMRECT and TB_ITEMDROPDOWNRECT for BTNS_DD
	{
		RECT ButtonRc;  // Not TB_HIDDEN
		if(!byIndex) 
		{
			SendMessageW(hwTb, TB_GETRECT, (WPARAM)id, (LPARAM) &ButtonRc);// iCmd 
		}else{ // on index
		
			if(getBtnStyle(id, true)==BTNS_DROPDOWN)
			{
				SendMessageW(hwTb, TB_GETITEMDROPDOWNRECT, (WPARAM)id, (LPARAM) &ButtonRc);// client coord
			}
			else
			{
				SendMessageW(hwTb, TB_GETITEMRECT, (WPARAM)id, (LPARAM) &ButtonRc); // client coord
			}
		
		}
		return ButtonRc;
	}

	// Move button from the position iPosFrom to the iPosTo /////////////////// test it !!!!!
	void moveBtnPosFromTo( int iPosFrom, int iPosTo)
	{
		SendMessageW( hwTb, TB_MOVEBUTTON, (WPARAM)iPosFrom, (LPARAM)iPosTo);
		layout();
	}
	//////////////////////////////////////////////////////////////////////////// test it !!!!!

	// To move btn with id to another place we must getCmdPos position and then change it with
	// TB_MOVEBUTTON as we need

	//------------------------------------------------------------------
	void setBtnPins(int id, PB fButton, bool byIndex = false)
	{
		if(IUIBarData* pData = getItemData(id, byIndex))
		{
			pData->fButton = fButton;

			TBBUTTONINFOW tbi ={0};
			tbi.cbSize = sizeof(TBBUTTONINFOW);
			tbi.dwMask = TBIF_STYLE | TBIF_STATE | (byIndex ? TBIF_BYINDEX : 0);   //	byIndex
			tbi.fsStyle = getStyle(fButton);
			tbi.fsState = getState(fButton);
			
			SendMessageW ( hwTb, TB_SETBUTTONINFOW, (WPARAM) id, (LPARAM) (TBBUTTONINFOW*) &tbi);

			layout();
		}
	}
	
	//----------------------------------------------------------------  May be Tooltip too?
	void setButtonText(int id, const wchar_t* sText =0, bool byIndex= false) // =0,  = false
	{
		if(IUIBarData* pData = getItemData(id, byIndex))
		{
			pData->setText(sText);

			TBBUTTONINFOW tbi ={0};
			tbi.cbSize = sizeof(TBBUTTONINFOW);
			tbi.dwMask = TBIF_TEXT | (byIndex ? TBIF_BYINDEX : 0);	 //	byIndex
			tbi.pszText = const_cast<wchar_t*>(pData->getText());

			SendMessageW ( hwTb, TB_SETBUTTONINFOW, (WPARAM) id, (LPARAM) (TBBUTTONINFOW*) &tbi);

			layout();
		}
	}
	//------------------------------------------------------------------
	void setButtonImg(int id, int iImg =-1, bool byIndex = false )       
	{
		TBBUTTONINFOW tbi ={0};
		tbi.cbSize = sizeof(TBBUTTONINFOW);
		tbi.dwMask = TBIF_IMAGE | (byIndex ? TBIF_BYINDEX : 0);	   //	byIndex
		tbi.iImage = ((iImg <0) ? I_IMAGENONE : iImg);

		SendMessageW(hwTb, TB_SETBUTTONINFOW, (WPARAM) id, (LPARAM) (TBBUTTONINFOW*) &tbi);

		layout();
	}
	//------------------------------------------------------------------
	// If width == (-1) - BTNS_AUTOSIZE will applyed
	// width =0 - reset to default width and zeroize BTNS_AUTOSIZE
	void setButtonWidth(int id, int width = -1, bool byIndex = false)            //=-1, = false
	{
		TBBUTTONINFOW tbi ={0};
		tbi.cbSize = sizeof(TBBUTTONINFOW);
		tbi.dwMask = TBIF_STYLE | TBIF_SIZE | (byIndex ? TBIF_BYINDEX : 0);		//	byIndex

		SendMessageW( hwTb, TB_GETBUTTONINFOW, (WPARAM) id, (LPARAM) (TBBUTTONINFOW*) &tbi);

		if(width <0)
		{
			tbi.fsStyle |= BTNS_AUTOSIZE; // Assignment by bitwise OR
			tbi.cx =0;
		}
		else
		{
			tbi.cx = width;
			tbi.fsStyle &= (~BTNS_AUTOSIZE); // Assignment by bitwise AND ( Logical bitwise NOT )
		}

		SendMessageW( hwTb, TB_SETBUTTONINFOW, (WPARAM) id, (LPARAM) (TBBUTTONINFOW*) &tbi);

		layout();
	}

	//------------------------------------------------------------------
	void layout()
	{
		TBBUTTONINFOW tbi ={0};
		tbi.cbSize = sizeof(TBBUTTONINFOW);

		DWORD pad = SendMessageW(hwTb, TB_GETPADDING, 0, 0); // get the padding for a toolbar button			
		int maxH = -1; // Calculate max height and width of the buttons
		//int maxW = -1;
		for(int cnt = SendMessageW(hwTb, TB_BUTTONCOUNT, 0, 0); --cnt>=0; )
		{
			tbi.dwMask = TBIF_BYINDEX | TBIF_LPARAM | TBIF_SIZE | TBIF_STYLE;// | TBIF_IMAGE;
			if(0<= SendMessageW(hwTb, TB_GETBUTTONINFOW, (WPARAM) cnt, (LPARAM) (TBBUTTONINFOW*) &tbi))
			{
				if(CUIBarDataWnd* pW = dynamic_cast<CUIBarDataWnd*>((IUIBarData*)tbi.lParam))//только с окнами				
				{					                          
					tbi.fsStyle = getStyle(pW->fButton) | BTNS_AUTOSIZE;									
					SendMessageW(hwTb, TB_SETBUTTONINFOW, (WPARAM) cnt, (LPARAM) (TBBUTTONINFOW*) &tbi);					
					RECT rc;
					SendMessageW(hwTb, TB_GETITEMRECT, (WPARAM) cnt, (LPARAM) &rc);					
					maxH = ( ( pW->height + 2*LOWORD(pad) ) > (rc.bottom - rc.top) ) ? 
							 ( pW->height + 2*LOWORD(pad) ) : (rc.bottom - rc.top);	
				}   // высота определяется по кнопкам с окнами, и если у них нет подписей то ... 
				// а ширина как определяется, если она таки установлена через setBtnWidth ?
				// надо ли добавить  if( fstyle & (~BTNS_AUTOSIZE)) {int maxW = tbi.cx} ? 
				else{// for ordinary btns					
					tbi.cx = 0;					
					SendMessageW(hwTb, TB_SETBUTTONINFOW, (WPARAM)cnt, (LPARAM)(TBBUTTONINFOW*)&tbi);
					RECT rc;
					SendMessageW(hwTb, TB_GETITEMRECT, (WPARAM)cnt, (LPARAM)&rc);
					}		
				
			}
			else { break; }
		}
		 //send to Tb maxH TB_SETBUTTONSIZE should generally be called after adding buttons
													// width      height
		SendMessageW( hwTb, TB_SETBUTTONSIZE, 0, (LPARAM) MAKELONG (-1, maxH)); // so autosize btn
		//SendMessageW( hwTb, TB_AUTOSIZE, 0, 0); //--- ? verify
		//---------------------------------------------------------------
		int cnt = SendMessageW(hwTb, TB_BUTTONCOUNT, 0, 0);
		for(int id = -1; ++id <cnt; ) // Must be enumerated  from left/top to right/bottom button order
		{
			tbi.dwMask = TBIF_BYINDEX | TBIF_LPARAM;
			if(0<= SendMessageW(hwTb, TB_GETBUTTONINFOW, (WPARAM) id, (LPARAM) (TBBUTTONINFOW*) &tbi))
			{
				if(CUIBarDataWnd* pW = dynamic_cast<CUIBarDataWnd*>((IUIBarData*)tbi.lParam))
				{
					tbi.dwMask = TBIF_BYINDEX | TBIF_SIZE  | TBIF_STYLE ; 
					// TBIF_IMAGE -- if we'll handle  I_IMAGENONE to get width no img with only Label 
					// --| TBIF_STATE -> get Indeterminate State ?? verify this may be Disabled
					tbi.cx = 0; // reset width; 	
					//if (iImg==-1)
					//tbi.iImage = (iImg <0) ? I_IMAGENONE : iImg);

					tbi.fsStyle = getStyle(pW->fButton) | BTNS_AUTOSIZE;					
					SendMessageW(hwTb, TB_SETBUTTONINFOW, (WPARAM) id, (LPARAM) (TBBUTTONINFOW*) &tbi);
					RECT rc;
					SendMessageW(hwTb, TB_GETITEMRECT, (WPARAM) id, (LPARAM) &rc);
					// we take rc coords so can define new:						
					if(getBarStyle() & TBSTYLE_LIST) // that fit for LIST
					{
					   if( pW->offset <0) { tbi.cx = (rc.right-rc.left) + pW->width + LOWORD(pad); }
					   else { tbi.cx = max(pW->width + pW->offset + LOWORD(pad), (rc.right-rc.left)); }

					}else{                           // or fit for FLAT 
					   if( pW->offset <0) { tbi.cx = max((pW->width + 2*LOWORD(pad)), (rc.right-rc.left));}
					   else { tbi.cx =  max((pW->width + pW->offset + LOWORD(pad)), (rc.right-rc.left));}
					   }					
					tbi.fsStyle = getStyle(pW->fButton) & (~BTNS_AUTOSIZE); // no more btns_autosize				
					SendMessageW(hwTb, TB_SETBUTTONINFOW, (WPARAM) id, (LPARAM) (TBBUTTONINFOW*) &tbi);
					SendMessageW(hwTb, TB_AUTOSIZE,	0, 0);
					RECT rcWnd;
					SendMessageW(hwTb, TB_GETITEMRECT, (WPARAM) id, (LPARAM) &rcWnd);//new sizes adopted to control pin-up
								 // initialization as result of ternar operation высота для LIST да для FLAT
					int height = (pW->height < rcWnd.bottom-rcWnd.top) ? pW->height : (rcWnd.bottom - rcWnd.top);					
					int ypos = 0; // vertical pos
					int xpos = 0; // horiz    pos
					if ( TBSTYLE_LIST & getBarStyle() )
					{ 
						ypos = (rcWnd.bottom + rcWnd.top - height)/2;
						//xpos = (pW->offset <0) ? ( rcWnd.right - pW->width - LOWORD(pad)) : ( rcWnd.left  + pW->offset );
						xpos = (pW->offset <0) ? (rcWnd.right - pW->width - LOWORD(pad)) : (rcWnd.left + pW->offset);

					}else{
						ypos = rcWnd.top  + LOWORD(pad);
						xpos = rcWnd.left + LOWORD(pad);
						}	
					if(TBSTATE_HIDDEN & getState(pW->fButton))
					{
						ShowWindow(pW->hWnd, SW_HIDE);				
					}else{				
					SetWindowPos(	pW->hWnd,	
									0, 	
									xpos,		ypos,												  
									pW->width,	height, 								  	
								SWP_DRAWFRAME| SWP_SHOWWINDOW											
								 );					
							}
					// has button Wnd text or not?
					// if(*tbi.pszText==0){int Y = rc.top + LOWORD(pad);// top position
					// }else{int Y = rc.bottom - height - LOWORD(pad); }// bottom pos
					// SendMessageW(hwTb, TB_AUTOSIZE, 0, 0); 
				}
			}
			else { break; }
		}		
		SendMessageW(hwTb, TB_AUTOSIZE, 0, 0);  

	};
	
	//---------- Статические фунции-члены класса разделяются всеми инстанциями
	//---------- Соответствено не имеют указателя this и доступа к членам класса
	//---------- Поэтому должны содержать указатель или ссылку на инстанцию класса

	//---------В параметре может быть инфа, определяемая приложением    Надо ли её экспортировать?
	//static __declspec(dllexport) BOOL CALLBACK UIBarLayoutEnumProc(HWND hWnd, LPARAM lParam)
	 
	static	 BOOL CALLBACK UIBarLayoutEnumProc(HWND hWnd, LPARAM lParam)
	{
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hWnd, wcsUIBar)))    
		{
			if(CUIBar* pBar = dynamic_cast<CUIBar*>(pIB))
			{
				pBar->layout();
			}
		}
		return true;//To continue enumeration, the callback function must return TRUE;
					//to stop enumeration, it must return FALSE. 
	}

	//--Эта функция определяет, что пользовательский ввод вызвал сообщения от Тулбара или потомков
	//--и заполняет данные каждого источника сообщений в соответствующем объекте
	
	static 	CUIBar* isUIBar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		CUIBar* pBar =0;
		//-------------------------------------- отрабатываем мышь и клаву ----------------------
		if((WM_COMMAND == uMsg) || (WM_HSCROLL == uMsg) || (WM_VSCROLL == uMsg))// may be LVM_HSCROLL
		{
			if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW((HWND)lParam, wcsUIBar))) 
				//GetPropW((HWND)lParam, wcsUIBar)
			{
				if(IUIBarData* pData = dynamic_cast<IUIBarData*>(pIB)) // from ToolBar's child window
				{
					pBar = pData->pUIBar;   
					//  По указателю на данные окна в кнопке обращаемся к указателю на тулбар 
					//  то есть pBar представляет данные и методы отрисовки --
				}
				else { pBar = dynamic_cast<CUIBar*>(pIB); } // may be from ToolBar и его обычных кнопок
			}
		}
		else if(WM_NOTIFY == uMsg)
		{
			if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(LPNMHDR(lParam)->hwndFrom, wcsUIBar))) 
				// GetProp
			{
				if(IUIBarData* pData = dynamic_cast<IUIBarData*>(pIB)) // from ToolBar's child window
				{
					pBar = pData->pUIBar; // Можем обратится к Тулбару с уведомлениями встроенного окна --
				}
				else { pBar = dynamic_cast<CUIBar*>(pIB); } // may be from ToolBar и его обычных кнопок
			}
		}
		//------------- Layout all PicoBar obj --------------------------------------------
		if(WM_WINDOWPOSCHANGED == uMsg)
		{
			EnumChildWindows(hWnd, UIBarLayoutEnumProc, 0);
		}
		//-------------------------------------------------------------------------------
		return pBar;
	}

	//------------------------------------------------------------------
	// Parent's Win32 message handler - MUST be inserted to parent's WndProc!!!
	// We handle here user input and call corresponding handler for buttons
	// Also we perform custom drawing of buttons

	//static //__declspec(dllexport)
	static LRESULT ParentMsgHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if((WM_COMMAND == uMsg) || (WM_HSCROLL == uMsg) || (WM_VSCROLL == uMsg))
		{
			if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW((HWND)lParam, wcsUIBar)))
			{
				if(CUIBar* pBar = dynamic_cast<CUIBar*>(pIB))// from ToolBar
				{
					if(IUIBarData* pData = pBar->getItemData(wParam, false))
					{
						if(pData->pHandler) { pData->pHandler(hWnd, uMsg, wParam, lParam); }
					}
				}
				else if(IUIBarData* pData = dynamic_cast<IUIBarData*>(pIB)) // from ToolBar's child window
				{
					if(pData->pHandler) { pData->pHandler(hWnd, uMsg, wParam, lParam); }
				}
			}
		}//------------------------------------------------------------------------------
		else if(WM_NOTIFY == uMsg)
		{
			if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(LPNMHDR(lParam)->hwndFrom, wcsUIBar)))
			{
				if(IUIBarData* pData = dynamic_cast<IUIBarData*>(pIB)) // from ToolBar's child window
				{
					if(pData->pHandler) { pData->pHandler(hWnd, uMsg, wParam, lParam); }// call their handlers
				}
				else if(CUIBar* pBar = dynamic_cast<CUIBar*>(pIB))     // from ToolBar
				{
					if((NM_CLICK == LPNMHDR(lParam)->code) || (NM_RCLICK == LPNMHDR(lParam)->code))
					 {
						if(IUIBarData* pData = reinterpret_cast<IUIBarData*>(LPNMMOUSE(lParam)->dwItemData))
						{
							if(pData->pHandler) { pData->pHandler(hWnd, uMsg, wParam, lParam); }
						}
						return true;
					}//----------------------------------------------------------------------
					else if(TBN_DROPDOWN == LPNMHDR(lParam)->code)   // call drop-down button handler ( menu)
					{
						if(IUIBarData* pData = pBar->getItemData(LPNMTOOLBAR(lParam)->iItem, false))
						{
							if(pData->pHandler) { pData->pHandler(hWnd, uMsg, wParam, lParam); }
						}
						// we can define some flags for CD of DD_btns with this msg
						return TBDDRET_DEFAULT; // if handler not returns any fit ones?
						// Return codes for TBN_DROPDOWN
			//#define TBDDRET_DEFAULT         0   // The drop-down was handled
			//#define TBDDRET_NODEFAULT       1   // The drop-down was not handled
			//#define TBDDRET_TREATPRESSED    2   // The drop-down was handled, but treat 
						                              // the button like a regular button
			// we can define some flags for CD of DD_btns with these codes
						//return true; // ??? handler for CDID_SELECTED or TBSTATE_PRESSED ???
					}//----------------------------------------------------------------------
					else if(TBN_GETINFOTIP == LPNMHDR(lParam)->code)  //--Draw Infotips -----
					{
						NMTBGETINFOTIP* pit = LPNMTBGETINFOTIP(lParam);

						if(IUIBarData* pData = reinterpret_cast<IUIBarData*>(pit->lParam))
						{
							if(pData->getTip())
							{
								swprintf_s(pit->pszText, pit->cchTextMax, L"%s", pData->getTip());
							}
						}
						return true;
					}//-------------  end of Draw Infotips  ------------------------------

//====================================== Now Custom Drawing   ==============================================

					else if(NM_CUSTOMDRAW == LPNMHDR(lParam)->code)
					{
						NMTBCUSTOMDRAW *pcd = (NMTBCUSTOMDRAW*)lParam;
						pcd->hbrMonoDither = 0;// is to draw checked Bk- todo pBar->pbrMonoDither!!!!!
						pcd->clrBtnFace = CLR_NONE;	
						pcd->clrBtnHighlight = CLR_NONE;					
						pcd->clrHighlightHotTrack = CLR_NONE;
						pcd->nHLStringBkMode = TRANSPARENT; //OPAQUE;
						pcd->nStringBkMode   = TRANSPARENT; //OPAQUE;
						pcd->hbrLines = 0;	//CreateSolidBrush(RGB(200, 50, 20));
											//SelectObject(pcd->nmcd.hdc, pcd->hbrLines);						
						pcd->hpenLines = 0;	//CreatePen(PS_SOLID, 1, RGB(0, 255, 0)); 
											//SelectObject(pcd->nmcd.hdc, pcd->hpenLines);												

	//==== Control Prepaint Draw Stage    ======================================================
						if(pcd->nmcd.dwDrawStage == CDDS_PREPAINT)  
						{	if(pBar->pBarBk) 
							{	GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *(pBar->pBarBk)); }															
							if(pBar->pBarBorder)
							{   FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *(pBar->pBarBorder));}
							if (pBar->pBarLabelClr) // TODO:    add this block to all label setting parts
							{pcd->clrText = *pBar->pBarLabelClr;	}
							return (CDRF_NOTIFYITEMDRAW|CDRF_NOTIFYPOSTPAINT);
						} // end of CDDS_PREPAINT 
					//==========================================================================
				//IUIBarData* pData = reinterpret_cast<IUIBarData*>(pcd->nmcd.lItemlParam);
					//==========================================================================

					else if(pcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
					{
						IUIBarData* pData = reinterpret_cast<IUIBarData*>(pcd->nmcd.lItemlParam);
				
				// to draw icons oneself we must set ImgListPlaceholderCD as current
				// imageList, get icon sizes from it, find icon rect and fill it with
				// our custom bitmap GradientRect	
						/*if (pData->pImgCustomDraw)
						{
							RECT rcImg;
							DWORD pad = SendMessage(pcd->nmcd.hdr.hwndFrom, TB_GETPADDING, 0, 0);
							HIMAGELIST hCurrentImgListPlaceholderCustomDraw = 
							(HIMAGELIST)SendMessage(pcd->nmcd.hdr.hwndFrom, TB_GETIMAGELIST, 0, 0);
							int cxImgSize, cyImgSize;
							ImageList_GetIconSize(hCurrentImgListPlaceholderCustomDraw, &cxImgSize, &cyImgSize);
							rcImg.top = pcd->nmcd.rc.top + LOWORD(pad);
							rcImg.left = pcd->nmcd.rc.left + HIWORD(pad);
							rcImg.right = rcImg.left + cxImgSize;
							rcImg.bottom = rcImg.top + cyImgSize;
							GradientRect(pcd->nmcd.hdc, &rcImg, *pData->pImgCustomDraw);
						}*/
						
			//pressed = CDIS_SELECTED =
							if (pcd->nmcd.uItemState & CDIS_SELECTED) // TBSTATE_PRESSED --> CDIS_SELECTED								
							{  //-----------------------------------------------------  Bk
								if (pData->pBtn_PressedBk)
								{GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_PressedBk);
								}
								else if (pBar->pBtnTem_PressedBk)
								{GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBtnTem_PressedBk);
								}
								//-----------------------------------------------------  Border
								if (pData->pBtn_PressedBorder)
								{FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_PressedBorder);
								}
								else if (pBar->pBtnTem_PressedBorder)
								{FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBtnTem_PressedBorder);
								}
								//--------------------------------------------- ---------   Label										
								if (pData->pBtn_PressedLabelClr)
								{pcd->clrText = *pData->pBtn_PressedLabelClr;
								}
								else if (pBar->pBtnTem_PressedLabelClr)
								{pcd->clrText = *pBar->pBtnTem_PressedLabelClr;
								}
								if (getStyle(pData->fButton) & BTNS_DROPDOWN)
									{ 														
									RECT rcdd;	
									rcdd.left = pcd->nmcd.rc.right - 18;//GetSysMetrics(CX_MIM); ??
									rcdd.right = pcd->nmcd.rc.right - 1;
									rcdd.top = pcd->nmcd.rc.top + 1;
									rcdd.bottom = pcd->nmcd.rc.bottom - 1;
								/*	if(pData->pBtn_PressedBk)		
									{GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_PressedBk);	}
									else if(pBar->pBtnTem_PressedBk)
									{GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBtnTem_PressedBk);}*/
								//-----------------------------------------------------------
									if(pData->pBtn_PressedBorder)	
									{FrameRect(pcd->nmcd.hdc, &rcdd, *pData->pBtn_PressedBorder);}
									else if(pBar->pBtnTem_PressedBorder)
									{FrameRect(pcd->nmcd.hdc, &rcdd, *pBar->pBtnTem_PressedBorder);}	
								//-----------------------------------------------------------
									/*if(pData->pBtn_PressedLabelClr)	
									{pcd->clrText=*pData->pBtn_PressedLabelClr;	}
									else if(pBar->pBtnTem_PressedLabelClr)
									{pcd->clrText=*pBar->pBtnTem_PressedLabelClr;}*/

									/*pcd->clrBtnFace = CLR_NONE;	
									pcd->clrBtnHighlight = CLR_NONE;					
									pcd->clrHighlightHotTrack = CLR_NONE;*/
									//SelectBrush(pcd->nmcd.hdc,DC_BLACK);
									//SelectObject(pcd->nmcd.hdc, GetStockObject(WHITE_BRUSH));
									//SetDCBrushColor(pcd->nmcd.hdc, *pBar->pBtnTem_PressedBorder);								
								}
							}   // end of CDIS_SELECTED	
	// DISABLED = disabled =
							else if( pcd->nmcd.uItemState & CDIS_DISABLED )
							{
								if(pData->pBtn_DisabledBk) 
								{GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_DisabledBk);	}
								else if(pBar->pBtnTem_DisabledBk)
								{GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBtnTem_DisabledBk);	}
								//-----------------------------------------------------------
								if(pData->pBtn_DisabledBorder)
								{FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_DisabledBorder);	}
								else if(pBar->pBtnTem_DisabledBorder)
								{FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBtnTem_DisabledBorder);	}
								if(pData->pBtn_DisabledLabelClr)
								{pcd->clrText=*pData->pBtn_DisabledLabelClr;}
								else if(pBar->pBtnTem_DisabledLabelClr)
								{pcd->clrText=*pBar->pBtnTem_DisabledLabelClr;}	

							}	
	// CHECKED = checked =
							else if(pcd->nmcd.uItemState & CDIS_CHECKED ) // Checked items
								{								
									if(pData->pBtn_CheckedBk)		
									{GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_CheckedBk);	}
									else if(pBar->pBtnTem_CheckedBk)
									{GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBtnTem_CheckedBk);	}
								//-----------------------------------------------------------
									if(pData->pBtn_CheckedBorder)	
									{FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_CheckedBorder);}
									else if(pBar->pBtnTem_CheckedBorder)
									{FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBtnTem_CheckedBorder);}	
								//-----------------------------------------------------------
									if(pData->pBtn_CheckedLabelClr)	
									{pcd->clrText=*pData->pBtn_CheckedLabelClr;	}
									else if(pBar->pBtnTem_CheckedLabelClr)
									{pcd->clrText=*pBar->pBtnTem_CheckedLabelClr;}
									//---------------------------------------------BTNS_DROPDOWN

									//if (getStyle(pData->fButton) & BTNS_DROPDOWN)
									//{ 					
									//		//pcd->nmcd.rc.right -= 19;
									//	if(pData->pBtn_CheckedBk)		
									//	{GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_CheckedBk);	}
									//	else if(pBar->pBtnTem_CheckedBk)
									//	{GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBtnTem_CheckedBk);}
									////-----------------------------------------------------------
									//	if(pData->pBtn_CheckedBorder)	
									//	{FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_CheckedBorder);}
									//	else if(pBar->pBtnTem_CheckedBorder)
									//	{FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBtnTem_CheckedBorder);}	
									////-----------------------------------------------------------
									//	if(pData->pBtn_CheckedLabelClr)	
									//	{pcd->clrText=*pData->pBtn_CheckedLabelClr;	}
									//	else if(pBar->pBtnTem_CheckedLabelClr)
									//	{pcd->clrText=*pBar->pBtnTem_CheckedLabelClr;}
									//}
								
								}  //end of Checked state 		
	// HOT AND CHECKED = Checked and hovered ===														
							else if (pcd->nmcd.uItemState & CDIS_HOT & CDIS_CHECKED)
								{   
									if (pData->pBtn_CheckedHoveredBk)
									{	GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_CheckedHoveredBk);}
									else if (pBar->pBtnTem_CheckedHoveredBk)
									{	GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBtnTem_CheckedHoveredBk);	}
								
									if (pData->pBtn_CheckedHoveredBorder)
									{	FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_CheckedHoveredBorder);	}
									else if (pBar->pBtnTem_CheckedHoveredBorder)
									{	FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBtnTem_CheckedHoveredBorder);}
								
									if (pData->pBtn_CheckedHoveredLabelClr)
									{	pcd->clrText = *pData->pBtn_CheckedHoveredLabelClr;	}
									else if (pBar->pBtnTem_CheckedHoveredLabelClr)
									{	pcd->clrText = *pBar->pBtnTem_CheckedHoveredLabelClr;}
								}// end Checked, hovered
	// hot and unpressed					
							else if (pcd->nmcd.uItemState & CDIS_HOT & (~CDIS_SELECTED)) 
								{
									if (pData->pBtn_HoveredBk)
									{	GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_HoveredBk);	}
									else if (pBar->pBtnTem_HoveredBk)
									{	GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBtnTem_HoveredBk);}								
									if (pData->pBtn_HoveredBorder)
									{	FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *(pData->pBtn_HoveredBorder));}
									else if (pBar->pBtnTem_HoveredBorder)
									{	FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *(pBar->pBtnTem_HoveredBorder));	}
									if (pData->pBtn_HoveredBorder)
									{	FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *(pData->pBtn_HoveredBorder));}
									else if (pBar->pBtnTem_HoveredBorder)
									{	FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *(pBar->pBtnTem_HoveredBorder));	}
									if (pData->pBtn_HoveredLabelClr)
									{pcd->clrText = *pData->pBtn_HoveredLabelClr;}
									else if (pBar->pBtnTem_HoveredLabelClr)
									{pcd->clrText = *pBar->pBtnTem_HoveredLabelClr;	}

									//if (getStyle(pData->fButton) & BTNS_DROPDOWN)
									//{ 											
									//	pcd->nmcd.rc.right -= 18;
									//	
									//	if (pData->pBtn_HoveredBk)
									//	{GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_HoveredBk);
									//	}
									//	else if (pBar->pBtnTem_HoveredBk)
									//	{GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBtnTem_HoveredBk);
									//	}	
									//	if (pData->pBtn_HoveredBorder)
									//	{FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *(pData->pBtn_HoveredBorder));
									//	}
									//	else if (pBar->pBtnTem_HoveredBorder)
									//	{FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *(pBar->pBtnTem_HoveredBorder));									
									//	}
									//	//SelectObject(pcd->nmcd.hdc,GetStockObject(DC_BRUSH));
									//	 /*SetDCBrushColor(pcd->nmcd.hdc, RGB(255,0,0));
									//	 SelectObject(pcd->nmcd.hdc, GetStockObject(DC_BRUSH));*/
									//	//SendMessageW(pcd->nmcd.hdr.hwndFrom, TB_GETITEMDROPDOWNRECT,
									//	//WPARAM(pcd->nmcd.dwItemSpec), LPARAM(&pcd->nmcd.rc));	
									//	//// UINT wType=DFC_SCROLL; UINT uState=DFCS_FLAT;
									//	//DrawFrameControl(pcd->nmcd.hdc, &pcd->nmcd.rc, DFC_SCROLL, DFCS_HOT);
									//}	// end if (BTNS_DROPDOWN)
								}// end only hovered
	// all others states = default ( forget INDETERMINATE )
								else // all ...
								{	//--------------------------------------------  Bk
									if (pData->pBtn_DefaultBk)
									{GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_DefaultBk);
									}
									else if (pBar->pBtnTem_DefaultBk)
									{GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBtnTem_DefaultBk);
									}

									if (pData->pBtn_DefaultBorder)     // ind btn
									{FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_DefaultBorder);
									}
									else if (pBar->pBtnTem_DefaultBorder)
									{FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBtnTem_DefaultBorder);
									}
									if (pData->pBtn_DefaultLabelClr)
									{pcd->clrText = *pData->pBtn_DefaultLabelClr;
									}
									else if (pBar->pBtnTem_DefaultLabelClr)
									{pcd->clrText = *pBar->pBtnTem_DefaultLabelClr;
									}
									else if (pBar->pBarLabelClr) // TODO: add this block to all label setting parts
									{pcd->clrText = *pBar->pBarLabelClr;
									}// or to the control prepaint stage

									if (getStyle(pData->fButton) & BTNS_DROPDOWN)
									{ 					
										RECT rcdd;	
									rcdd.left = pcd->nmcd.rc.right - 20;//GetMetrics(CX_MIM); ??
									rcdd.right = pcd->nmcd.rc.right;
									rcdd.top = pcd->nmcd.rc.top + 1;
									rcdd.bottom = pcd->nmcd.rc.bottom - 1;
										
										if (pData->pBtn_DefaultBk)
										{GradientRect(pcd->nmcd.hdc, &rcdd, *pData->pBtn_DefaultBk);
										}
										else if (pBar->pBtnTem_DefaultBk)
										{GradientRect(pcd->nmcd.hdc, &rcdd, *pBar->pBtnTem_DefaultBk);
										}	
										if (pData->pBtn_DefaultBorder)
										{FrameRect(pcd->nmcd.hdc, &rcdd, *(pData->pBtn_DefaultBorder));
										}
										else if (pBar->pBtnTem_DefaultBorder)
										{FrameRect(pcd->nmcd.hdc, &rcdd, *(pBar->pBtnTem_DefaultBorder));
										}								
									}// end of BTNS_DROPDOWN
								}//  end of default state  
							
							return ( getDrawStyle(pData->fButton));// trans TBCDRF_DRAWSTYLES
						} // end of CDDS_ITEMPREPAINT						
					
					else if(pcd->nmcd.dwDrawStage == CDDS_ITEMPOSTPAINT )
						{	return CDRF_NOTIFYPOSTPAINT;
						}					
					else if (pcd->nmcd.dwDrawStage == CDDS_POSTPAINT )
						{						
							//SetBkMode(pcd->nmcd.hdc, TRANSPARENT); 
							//HFONT hFontforTbTrial = (HFONT) ::SendMessage(pcd->nmcd.hdr.hwndFrom, WM_GETFONT, 0, 0);
							//SelectObject(pcd->nmcd.hdc, hFontforTbTrial);
							//SetTextColor(pcd->nmcd.hdc, RGB( 250,200,190));
							//TextOutW(pcd->nmcd.hdc,400,45,L"http://nativeruse.com", 21);									
							//DrawTextW(pcd->nmcd.hdc, L":::Trial:::NuiBar:::", 20, &pcd->nmcd.rc, DT_RIGHT);
							//HFONT hFont = (HFONT)GetStockObject(SYSTEM_FONT); 
							//TextOut(pcd->nmcd.hdc, 210, 25, L"Sample SYSTEM_FONT text", 25); 
						}
						return CDRF_DODEFAULT;// or 0;
					}// end of Custom Draw Control
				}// end of from ToolBar
			}// end of if(IUIBar* pIB = reinterpret_cast<IUIBar*>...)))
		}// end of (WM_NONIFY== uMsg)
		//-------------------------------------------------------------------
		return 0;	
	}// end of ParentMsgHandler
};// end of class CUI
////////////////////////////////////////////////////////////////////////////////////////////
	HWND CreatePinaBar(HWND hwParent, HIMAGELIST hImgList, DWORD TbStyle, HMODULE hModule)
{
	CUIBar* pUIBar;
	HWND hwTbRet; 
	pUIBar = new CUIBar(hwParent, hImgList, TbStyle, hModule);

	//pUIBar->pBtn_DefaultBk  =  &rgbBarBkgr_Default; 	
	//pUIBar->pBtn_DefaultBorder = &rgbBarFrame_Default;	  	
	
	return hwTbRet = pUIBar->hwTb; // constructor returns nothing so we must define return value here
}
	///////////////////////////////      Toolbar Colors     /////////////////////////////////
	void SetBarBk ( HWND hwTb, const COLORREF (*pBarBk)[5]) 		
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBarBk = pBarBk;				
			}
		}
}
	void SetBarBorder ( HWND hwTb, const COLORREF *pBarBorder)
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBarBorder = pBarBorder;				
			}
		}
}
	void SetBarLabelColor ( HWND hwTb, const COLORREF *pBarLabelClr )
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBarLabelClr = pBarLabelClr;			// ?????	
			}
		}
}
	void SetBtnTem_DefaultLabelColor( HWND hwTb, const COLORREF *pBtnTem_DefaultLabelClr )
	{
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_DefaultLabelClr = pBtnTem_DefaultLabelClr;				
			}
		}	
	}
	void SetBtnTem_DisabledLabelColor( HWND hwTb, const COLORREF *pBtnTem_DisabledLabelClr )
	{
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_DisabledLabelClr = pBtnTem_DisabledLabelClr;				
			}
		}	
	}
	void SetBtnTem_HoveredLabelColor( HWND hwTb, const COLORREF *pBtnTem_HoveredLabelClr )
	{
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_HoveredLabelClr = pBtnTem_HoveredLabelClr;				
			}
		}	
	}
	void SetBtnTem_CheckedHoveredLabelColor ( HWND hwTb, const COLORREF *pBtnTem_CheckedHoveredLabelClr)
	{
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_CheckedHoveredLabelClr = pBtnTem_CheckedHoveredLabelClr;				
			}
		}	
	}

	void SetBtnTem_CheckedLabelColor ( HWND hwTb, const COLORREF *pBtnTem_CheckedLabelClr)
	{
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_CheckedLabelClr = pBtnTem_CheckedLabelClr;				
			}
		}	
	}

	void SetBtnTem_PressedLabelColor ( HWND hwTb, const COLORREF *pBtnTem_PressedLabelClr)
	{
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_PressedLabelClr = pBtnTem_PressedLabelClr;				
			}
		}	
	}
	void SetBtnTem_MarkedLabelColor ( HWND hwTb, const COLORREF *pBtnTem_MarkedLabelClr)
	{
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_MarkedLabelClr = pBtnTem_MarkedLabelClr;				
			}
		}	
	}
 
	void SetBarColors  ( HWND hwTb,	const COLORREF (*pBarBk)[5], 
									const COLORREF *pBarBorder, 
									const COLORREF *pBarLabelClr   )
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{	
				pUIBar->pBarBk  = pBarBk;
				pUIBar->pBarBorder = pBarBorder;
				pUIBar->pBarLabelClr  = pBarLabelClr;				
			}
		}
}

	void SetBtnTem_HoveredBk    ( HWND hwTb, const COLORREF (*pBtnTem_HoveredBk)[5]) 		
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_HoveredBk = pBtnTem_HoveredBk;				
			}
		}
}
	void SetBtnTem_DefaultBk ( HWND hwTb, const COLORREF (*pBtnTem_DefaultBk)[5]) 		
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_DefaultBk = pBtnTem_DefaultBk;				
			}
		}
}
	void SetBtnTem_DisabledBk ( HWND hwTb, const COLORREF (*pBtnTem_DisabledBk)[5]) 		
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_DisabledBk = pBtnTem_DisabledBk;				
			}
		}
}
	void SetBtnTem_MarkedBk ( HWND hwTb, const COLORREF (*pBtnTem_MarkedBk)[5]) 		
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_MarkedBk = pBtnTem_MarkedBk;				
			}
		}
}
	void SetBtnTem_CheckedBk( HWND hwTb, const COLORREF (*pBtnTem_CheckedBk)[5]) 		
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_CheckedBk = pBtnTem_CheckedBk;				
			}
		}
}
	void SetBtnTem_PressedBk( HWND hwTb, const COLORREF (*pBtnTem_PressedBk)[5]) 		
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_PressedBk = pBtnTem_PressedBk;			
			}
		}
}
	void SetBtnTem_HoveredBorder   ( HWND hwTb, const COLORREF *pBtnTem_HoveredBorder) 		
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_HoveredBorder = pBtnTem_HoveredBorder;				
			}
		}
}
	void SetBtnTem_DefaultBorder ( HWND hwTb, const COLORREF *pBtnTem_DefaultBorder) 		
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_DefaultBorder = pBtnTem_DefaultBorder;				
			}
		}
}
	void SetBtnTem_DisabledBorder ( HWND hwTb, const COLORREF *pBtnTem_DisabledBorder) 		
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_DisabledBorder = pBtnTem_DisabledBorder;				
			}
		}
}
	void SetBtnTem_MarkedBorder ( HWND hwTb, const COLORREF *pBtnTem_MarkedBorder) 		
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_MarkedBorder = pBtnTem_MarkedBorder;				
			}
		}
}
	void SetBtnTem_CheckedBorder( HWND hwTb, const COLORREF *pBtnTem_CheckedBorder) 		
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_CheckedBorder = pBtnTem_CheckedBorder;				
			}
		}
}
	void SetBtnTem_PressedBorder( HWND hwTb, const COLORREF *pBtnTem_PressedBorder) 		
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_PressedBorder = pBtnTem_PressedBorder;				
			}
		}
}
	void SetBtnTem_CheckedHoveredBorder( HWND hwTb, const COLORREF *pBtnTem_CheckedHoveredBorder) 		
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_CheckedHoveredBorder = pBtnTem_CheckedHoveredBorder;				
			}
		}
}
	void SetBtnTem_CheckedHoveredBk( HWND hwTb, const COLORREF (*pBtnTem_CheckedHoveredBk)[5]) 		
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->pBtnTem_CheckedHoveredBk = pBtnTem_CheckedHoveredBk;				
			}
		}
}
/////////////////////////////////      Button id  Colors      /////////////////////////

	void SetBtn_HoveredLabelColor(HWND hwTb, int id, const COLORREF *pBtn_HoveredLabelClr,	bool byIndex)
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{	
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_HoveredLabelClr = pBtn_HoveredLabelClr;				
			}
		}
}

	void SetBtn_DefaultLabelColor  ( HWND hwTb, int id, const COLORREF *pBtn_DefaultLabelClr,	bool byIndex )
{ 
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{	
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_DefaultLabelClr = pBtn_DefaultLabelClr;				
			}
		}
}
	void SetBtn_DisabledLabelColor ( HWND hwTb, int id, const COLORREF *pBtn_DisabledLabelClr,	bool byIndex )
{ 
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{	
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_DisabledLabelClr = pBtn_DisabledLabelClr;				
			}
		}
}
	void SetBtn_DefaultBk     ( HWND hwTb, int id,	const COLORREF (*pBtn_DefaultBk)[5],	bool byIndex)   		
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{		
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_DefaultBk = pBtn_DefaultBk;				
			}
		}
}
	void SetBtn_DefaultBorder ( HWND hwTb, int id,	const COLORREF *pBtn_DefaultBorder,   bool byIndex)
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{		
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_DefaultBorder = pBtn_DefaultBorder;				
			}
		}
}	
	void SetBtn_DefaultColors ( HWND hwTb, int id,	const COLORREF (*pBtn_DefaultBk)[5], 
													const COLORREF *pBtn_DefaultBorder,   bool byIndex)
{  
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{				
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_DefaultBk     = pBtn_DefaultBk;	
				pUIBar->getItemData(id, byIndex)->pBtn_DefaultBorder = pBtn_DefaultBorder;				
			}
		}
}
	void SetBtn_DisabledBk    ( HWND hwTb, int id, const COLORREF (*pBtn_DisabledBk)[5],	bool byIndex)   		
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{		
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_DisabledBk = pBtn_DisabledBk;				
			}
		}
}
	void SetBtn_DisabledBorder ( HWND hwTb, int id, const COLORREF *pBtn_DisabledBorder,   bool byIndex)
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{		
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_DisabledBorder = pBtn_DisabledBorder;				
			}
		}
}	
	void SetBtn_DisabledColors ( HWND hwTb, int id, const COLORREF (*pBtn_DisabledBk)[5], 
													 const COLORREF *pBtn_DisabledBorder,   bool byIndex)
{  
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{				
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_DisabledBk     = pBtn_DisabledBk;	
				pUIBar->getItemData(id, byIndex)->pBtn_DisabledBorder = pBtn_DisabledBorder;				
			}
		}
}

	void SetBtn_HoveredBk ( HWND hwTb, int id, const COLORREF (*pBtn_HoveredBk )[5], bool byIndex) 		
{
	if( IUIBar* pIB = reinterpret_cast<IUIBar*>( GetPropW(hwTb, wcsUIBar) )  )
		{	
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_HoveredBk = pBtn_HoveredBk;				
			}
		}
}
	void SetBtn_HoveredBorder ( HWND hwTb, int id, const COLORREF *pBtn_HoveredBorder, bool byIndex)
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{	
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_HoveredBorder = pBtn_HoveredBorder;				
			}
		}
}	
	void SetBtn_HoveredColors ( HWND hwTb, int id, const COLORREF (*pBtn_HoveredBk)[5], 
												const COLORREF *pBtn_HoveredBorder,   bool byIndex)
{  
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{				
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_HoveredBk = pBtn_HoveredBk;	
				pUIBar->getItemData(id, byIndex)->pBtn_HoveredBorder = pBtn_HoveredBorder;				
			}
		}
}
	void SetBtn_PressedBk ( HWND hwTb, int id, const COLORREF (*pBtn_PressedBk)[5], bool byIndex) 		
{
	if( IUIBar* pIB = reinterpret_cast<IUIBar*>( GetPropW(hwTb, wcsUIBar) )  )
		{	
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex )->pBtn_PressedBk = pBtn_PressedBk;				
			}
		}
}
	void SetBtn_PressedBorder ( HWND hwTb, int id, const COLORREF *pBtn_PressedBorder, bool byIndex)
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{	
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_PressedBorder = pBtn_PressedBorder;				
			}
		}
}

	void SetBtn_PressedLabelColor  ( HWND hwTb, int id, const COLORREF *pBtn_PressedLabelClr,	bool byIndex )
{ 
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{	
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_PressedLabelClr = pBtn_PressedLabelClr;				
			}
		}
}

	void SetBtn_PressedColors ( HWND hwTb, int id, const COLORREF (*pBtn_PressedBk)[5], 
												const COLORREF *pBtn_PressedBorder,   bool byIndex)
{  
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{				
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_PressedBk = pBtn_PressedBk;	
				pUIBar->getItemData(id, byIndex)->pBtn_PressedBorder = pBtn_PressedBorder;				
			}
		}
}	
	
	void SetBtn_CheckedBk ( HWND hwTb, int id, const COLORREF (*pBtn_CheckedBk)[5], bool byIndex) 		
{
	if( IUIBar* pIB = reinterpret_cast<IUIBar*>( GetPropW(hwTb, wcsUIBar) )  )
		{	
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_CheckedBk = pBtn_CheckedBk;				
			}
		}
}
	void SetBtn_CheckedBorder ( HWND hwTb, int id, const COLORREF *pBtn_CheckedBorder, bool byIndex)
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{	
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_CheckedBorder = pBtn_CheckedBorder;				
			}
		}
}
	void SetBtn_CheckedLabelColor  ( HWND hwTb, int id, const COLORREF *pBtn_CheckedLabelClr,	bool byIndex )
{ 
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{	
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_CheckedLabelClr = pBtn_CheckedLabelClr;				
			}
		}
}

	void SetBtn_CheckedColors ( HWND hwTb, int id, const COLORREF (*pBtn_CheckedBk)[5], 
												const COLORREF *pBtn_CheckedBorder,   bool byIndex)
{  
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{				
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_CheckedBk = pBtn_CheckedBk;	
				pUIBar->getItemData(id, byIndex)->pBtn_CheckedBorder = pBtn_CheckedBorder;				
			}
		}
}
	void SetBtn_MarkedBk    ( HWND hwTb, int id, const COLORREF (*pBtn_MarkedBk)[5], bool byIndex) 		
{
	if( IUIBar* pIB = reinterpret_cast<IUIBar*>( GetPropW(hwTb, wcsUIBar) )  )
		{	
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_MarkedBk = pBtn_MarkedBk;				
			}
		}
}
	void SetBtn_MarkedBorder ( HWND hwTb, int id, const COLORREF *pBtn_MarkedBorder, bool byIndex)
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{	
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_MarkedBorder = pBtn_MarkedBorder;				
			}
		}
}

	void SetBtn_MarkedLabelColor  ( HWND hwTb, int id, const COLORREF *pBtn_MarkedLabelClr,	bool byIndex )
{ 
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{	
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_MarkedLabelClr = pBtn_MarkedLabelClr;				
			}
		}
}
	void SetBtn_MarkedColors ( HWND hwTb, int id, const COLORREF (*pBtn_MarkedBk)[5], 
												const COLORREF *pBtn_MarkedBorder,   bool byIndex)
{  
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{	if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{	pUIBar->getItemData(id, byIndex)->pBtn_MarkedBk     = pBtn_MarkedBk;	
				pUIBar->getItemData(id, byIndex)->pBtn_MarkedBorder = pBtn_MarkedBorder;				
			}
		}
}

	void SetBtn_CheckedHoveredLabelColor(HWND hwTb, int id, const COLORREF *pBtn_CheckedHoveredLabelClr,	bool byIndex)
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{	
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_CheckedHoveredLabelClr = pBtn_CheckedHoveredLabelClr;				
			}
		}
}
	void SetBtn_CheckedHoveredBk ( HWND hwTb, int id, const COLORREF (*pBtn_CheckedHoveredBk)[5], bool byIndex) 		
{
	if( IUIBar* pIB = reinterpret_cast<IUIBar*>( GetPropW(hwTb, wcsUIBar) )  )
		{	
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_CheckedHoveredBk = pBtn_CheckedHoveredBk;				
			}
		}
}
	void SetBtn_CheckedHoveredBorder ( HWND hwTb, int id, const COLORREF *pBtn_CheckedHoveredBorder, bool byIndex)
{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{	
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->getItemData(id, byIndex)->pBtn_CheckedHoveredBorder = pBtn_CheckedHoveredBorder;				
			}
		}
}

///////////////////////////////////////////////////////////////////////////
	int PinCmdBtn(HWND hwTb, PinaBtnHandler pHandler,
					int iImg, const wchar_t* sText, const wchar_t* wcsTip,
					int iPos,	PB fButton)
{ 
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
	{
		if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
		{			
		return	pUIBar->insertCmd( pHandler, iImg, sText, wcsTip, iPos, fButton);
		} 		
	} 
}
/////////////////////////////////////////////////////////////////////////////////

	int PinGap( HWND hwTb, int iPos ) 
	{return PinCmdBtn( hwTb, 0, -1, 0, 0, iPos, 0|PB_AUTOSIZE);}

	int PinSep( HWND hwTb, int iPos, int width ) 
	{return PinCmdBtn( hwTb, 0, width, 0, 0, iPos, 0|PB_SEP);}


//////////////////////////////////////////////////////////////////////////////////////////

	int PinWinBtn(	HWND hwTb, PinaBtnHandler pHandler,	HWND hwIns, 
					int width, int height, int offset,	int iImg, 
					const wchar_t* sText,	int iPos,	PB fButton)
	{   
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{   
		if(CUIBar* pUIBar	= dynamic_cast<CUIBar*>(pIB)) 
			{	
			return pUIBar->insertWnd( pHandler, hwIns, width, height, offset,	iImg, sText, iPos,	fButton);
			} 			
		}  
	}
////////////////////////////////////////////////////////////////////////////////////////////////

	void DelBtn(HWND hwTb, int id, bool byIndex)
	{
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{			
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->deleteButton(id, byIndex);
			}
		}
	}
//------------------------------------------------------------------
	// Return zero-based index of the button assosiated with command idCmd

	int GetBtnPos(HWND hwTb, int idCmd)
	{   			
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				return pUIBar->getCmdPos(idCmd);
			}
		} 
	}
//--------------------------------------------------------------------	

	void MoveBtnPosFromTo(HWND hwTb, int iPosFrom, int iPosTo)
	{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				return pUIBar->moveBtnPosFromTo( iPosFrom, iPosTo);
			}
		} 
	}

//--------------------------------------------------------------------
	// by names not by index
	RECT GetBtnRect(HWND hwTb, int id, bool byIndex)
	{
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				return pUIBar->getBtnRc(id, byIndex);	
			}
		}		
	}
//--------------------------------------------------------------------

	void SetBtnPins(HWND hwTb, int id, PB fButton, bool byIndex)
	{
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{				
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->setBtnPins(id, fButton, byIndex);
			}
		}
	}

	PB GetBtnPins(HWND hwTb, int id, bool byIndex)
	{
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{			
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
			  return pUIBar->getItemData( id, byIndex )->fButton;
			}
		}
	}	
//-------------------------------------------------------------------------     
 
	void SetBtnLabel( HWND hwTb, int id, const wchar_t* sLabel, bool byIndex )
	{
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{																	  
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->setButtonText( id, sLabel, byIndex );
			}
		}	
	}

	const wchar_t* GetBtnLabel( HWND hwTb, int id, bool byIndex)
	 {
		 if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
			{			
				if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
				{
					 return pUIBar->getItemData( id, byIndex )->getText();
				}
			}
	 }

  void SetBtnTip( HWND hwTb, int id, const wchar_t* sTip, bool byIndex)// TODO - try byIndex
	{
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))  
		{			
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{	
				pUIBar->getItemData( id, byIndex )->setTip(sTip);
			}
		}
	}

 const wchar_t* GetBtnTip  ( HWND hwTb, int id,  bool byIndex)
	 {
		 if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
			{			
				if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
				{
				  return pUIBar->getItemData( id, byIndex )->getTip();
				}
			} 
	 }
	//-------------------------------------------------------------------------
  void SetBtnImg( HWND hwTb, int id, int iImg, bool byIndex)
	{
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{																	 
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->setButtonImg( id, iImg, byIndex );
			}
		}	
	}
 int  GetBtnImg  ( HWND hwTb, int id, bool byIndex)//=============================== yes
 {  
		TBBUTTONINFOW tbi ={0};
		tbi.cbSize = sizeof(TBBUTTONINFOW);
		tbi.dwMask = TBIF_IMAGE | (byIndex ? TBIF_BYINDEX : 0);	   //	byIndex
		SendMessageW(hwTb, TB_GETBUTTONINFOW, (WPARAM) id, (LPARAM) (TBBUTTONINFOW*) &tbi);
		return tbi.iImage;	
	
 }
///////////////////////////////////////////
 void SetBtnWidth( HWND hwTb, int id, int width, bool byIndex)
	{
	if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{				
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->setButtonWidth( id, width, byIndex );
			}
		}
	}
 //=================================================================================== yes
  int  GetBtnWidth( HWND hwTb, int id, bool byIndex )
 {
	 TBBUTTONINFOW tbi ={0};
		tbi.cbSize = sizeof(TBBUTTONINFOW);
		tbi.dwMask = TBIF_SIZE | (byIndex ? TBIF_BYINDEX : 0);	   //	byIndex
		SendMessageW(hwTb, TB_GETBUTTONINFOW, (WPARAM) id, (LPARAM) (TBBUTTONINFOW*) &tbi);
		return (int) tbi.cx;
 }//=======================================================================================

   BYTE  GetBtnState( HWND hwTb, int id, bool byIndex )
 {
	 TBBUTTONINFOW tbi ={0};
		tbi.cbSize = sizeof(TBBUTTONINFOW);
		tbi.dwMask = 	TBIF_STATE | (byIndex ? TBIF_BYINDEX : 0);	   //	byIndex
		SendMessageW(hwTb, TB_GETBUTTONINFOW, (WPARAM) id, (LPARAM) (TBBUTTONINFOW*) &tbi);
		return tbi.fsState;
 }
	BYTE  GetBtnStyle( HWND hwTb, int id, bool byIndex )
 {
	 TBBUTTONINFOW tbi ={0};
		tbi.cbSize = sizeof(TBBUTTONINFOW);
		tbi.dwMask = 	TBIF_STYLE | (byIndex ? TBIF_BYINDEX : 0);	   //	byIndex
		SendMessageW(hwTb, TB_GETBUTTONINFOW, (WPARAM) id, (LPARAM) (TBBUTTONINFOW*) &tbi);
		return tbi.fsStyle;
 }
///////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------	
	void SetBarImgList(HWND hwTb, HIMAGELIST hImgList) 
	{
		if (IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if (CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB))
			{
				pUIBar->setBarImgList(hImgList);
			}
		}
	}

	HIMAGELIST GetBarImgList(HWND hwTb)
	{
		if (IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if (CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB))
			{
				return pUIBar->getBarImgList();
			}
		}
	}
 void SetBarStyle(HWND hwTb, DWORD dwStyle) //= TbStyle_Default) 
	{
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				pUIBar->setBarStyle( dwStyle );				
			}
		}		
	}

 DWORD GetBarStyle(HWND hwTb) 
	 {	
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				return pUIBar->getBarStyle();
			}
		} 		 
	 }
 //--------------------------------------------------------------------------------
  int GetBarBtnCount(HWND hwTb) 
	 {	
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				return pUIBar->getBarBtnCount();
			}
		} 		 
	 }
   //--------------------------------------------------------------------------------
  int GetBarHiddenBtnCount(HWND hwTb) 
	 {	
		if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		{
			if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
			{
				return pUIBar->getBarHiddenBtnCount();
			}
		} 		 
	 }
  //int GetBarGapCount(HWND hwTb)
  //{
  //if(IUIBar* pIB = reinterpret_cast<IUIBar*>(GetPropW(hwTb, wcsUIBar)))
		//{
		//	if(CUIBar* pUIBar = dynamic_cast<CUIBar*>(pIB)) 
		//	{
		//		return pUIBar->getBarGapCount();
		//	}
		//} 
  //}

// we verify this message and if true will call MsgHandlerPinaBar() 
// else we let it fall down in other handlers of application
 BOOL IsThisMsgForPinaBar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(CUIBar* pUIB = CUIBar::isUIBar(hWnd, uMsg, wParam, lParam)) return TRUE;
	else return FALSE;
}
//--------------------------------------------------------------------------------------
 LRESULT MsgHandlerPinaBar( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	CUIBar* pUIB = CUIBar::isUIBar( hWnd, uMsg, wParam, lParam );
	return  pUIB->ParentMsgHandler( hWnd, uMsg, wParam, lParam );
}
 
 ////////////////////////
 ///////////////////////////////
 ////////////////////////////////////
 //--------------- GRADIENT FILL RECTANGLES ROUTINES -------------------------------------

//const int Aux_GradientRectVertexNumber = 5;

/////////////////////////////////////////////////////////////////////////////////////////
// see Lab_GradientFill for usage example
inline void SetVertex(TRIVERTEX* pVert, int x, int y, COLORREF rgb)
{
	pVert->x = x;
	pVert->y = y;
	pVert->Red    = ((COLOR16)GetRValue(rgb)) << 8;
	pVert->Green  = ((COLOR16)GetGValue(rgb)) << 8;
	pVert->Blue   = ((COLOR16)GetBValue(rgb)) << 8;
	pVert->Alpha  = 0x0000;
}

/////////////////////////////////////////////////////////////////////////////////////////
// see Lab_GradientFill for usage example
inline void GradientRect(HDC hdc, int left, int top, int right, int bottom,
				  COLORREF RgbLT, COLORREF RgbTR, COLORREF RgbBR, COLORREF RgbBL, COLORREF RgbCR)
{
	TRIVERTEX vert [Aux_GradientRectVertexNumber][2];
	GRADIENT_TRIANGLE gt[4];
	// Ten vertices of TRIVERTEX structure
	SetVertex(&vert[0][0], left, bottom, RgbBL); // 0 at address of first element we write LB point and its color
	SetVertex(&vert[0][1], left, top, RgbLT);    // 1

	SetVertex(&vert[1][0], left, top, RgbLT);    // 2
	SetVertex(&vert[1][1], right, top, RgbTR);   // 3

	SetVertex(&vert[2][0], right, top, RgbTR);   // 4
	SetVertex(&vert[2][1], right, bottom, RgbBR);// 5

	SetVertex(&vert[3][0], right, bottom, RgbBR);// 6
	SetVertex(&vert[3][1], left, bottom, RgbBL); // 7

	SetVertex(&vert[4][0], (left+right)/2, (top+bottom)/2, RgbCR);// 8
	SetVertex(&vert[4][1], (left+right)/2, (top+bottom)/2, RgbCR);// 9=8 to simplify code  

	gt[0].Vertex1   = 8; // first vertice we place in the center ( 8 element in vert array)
	gt[0].Vertex2   = 0;
	gt[0].Vertex3   = 1;

	gt[1].Vertex1   = 8;
	gt[1].Vertex2   = 2;
	gt[1].Vertex3   = 3;

	gt[2].Vertex1   = 8;
	gt[2].Vertex2   = 4;
	gt[2].Vertex3   = 5;

	gt[3].Vertex1   = 8;
	gt[3].Vertex2   = 6;
	gt[3].Vertex3   = 7;

	GradientFill(hdc, (TRIVERTEX*)vert, 10, gt, 4, GRADIENT_FILL_TRIANGLE);
}
//GradientFill(
//    __in HDC hdc,			//  A handle to the destination device context.
//    __in_ecount(nVertex) PTRIVERTEX pVertex, 
							//  A pointer to an array of TRIVERTEX structures that each define a vertex.
//    __in ULONG nVertex,	// The number of vertices in pVertex
//    __in PVOID pMesh,		// An array of GRADIENT_TRIANGLE structures in triangle mode
//    __in ULONG nMesh,		// The number of elements (triangles or rectangles) in pMesh
//    __in ULONG ulMode		// The gradient fill mode
//    );
/////////////////////////////////////////////////////////////////////////////////////////
// see Lab_GradientFill for usage example
inline void GradientRect(HDC hdc, RECT* pRc, const COLORREF VertexColor[Aux_GradientRectVertexNumber])
{
	TRIVERTEX vert [Aux_GradientRectVertexNumber][2];
	GRADIENT_TRIANGLE gt[4];

	SetVertex(&vert[0][0], pRc->left, pRc->bottom,	VertexColor[3]);
	SetVertex(&vert[0][1], pRc->left, pRc->top,		VertexColor[0]);

	SetVertex(&vert[1][0], pRc->left,  pRc->top,    VertexColor[0]);
	SetVertex(&vert[1][1], pRc->right, pRc->top,    VertexColor[1]);

	SetVertex(&vert[2][0], pRc->right, pRc->top,    VertexColor[1]);
	SetVertex(&vert[2][1], pRc->right, pRc->bottom, VertexColor[2]);

	SetVertex(&vert[3][0], pRc->right, pRc->bottom, VertexColor[2]);
	SetVertex(&vert[3][1], pRc->left,  pRc->bottom, VertexColor[3]);

	SetVertex(&vert[4][0], (pRc->left+pRc->right)/2, (pRc->top+pRc->bottom)/2, VertexColor[4]);
	SetVertex(&vert[4][1], (pRc->left+pRc->right)/2, (pRc->top+pRc->bottom)/2, VertexColor[4]);

	//SetVertex(&vert[4][0], (pRc->left/2+pRc->right)/2, (pRc->top+pRc->bottom)/2, VertexColor[4]);
	//SetVertex(&vert[4][1], (pRc->left+pRc->right/2)/2, (pRc->top+pRc->bottom)/2, VertexColor[4]);

	gt[0].Vertex1   = 8; // An array of GRADIENT_TRIANGLE structures
	gt[0].Vertex2   = 0;
	gt[0].Vertex3   = 1;

	gt[1].Vertex1   = 8;
	gt[1].Vertex2   = 2;
	gt[1].Vertex3   = 3;

	gt[2].Vertex1   = 8;//9
	gt[2].Vertex2   = 4;
	gt[2].Vertex3   = 5;

	gt[3].Vertex1   = 8;//9
	gt[3].Vertex2   = 6;
	gt[3].Vertex3   = 7;
	// (TRIVERTEX*)vert - the pointer to an array of TRIVERTEX structures that each define a vertex (pVertex)
	//               10 - the number of vertices in pVertex (nVertex)
	//               gt - the array of GRADIENT_TRIANGLE structures in triangle mode (pMesh)
	//                4 - the number of elements (triangles or rectangles) in gt ( or pMesh)
	GradientFill(hdc, (TRIVERTEX*)vert, 10, gt, 4, GRADIENT_FILL_TRIANGLE);
}


/////////////////////////////////////////////////////////////////////////////////////////
inline void FrameRect(HDC hdc, RECT* pRc, COLORREF rgb)
{
	HBRUSH hbrush    = CreateSolidBrush(rgb);
	HBRUSH hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);

	FrameRect(hdc, pRc, hbrush);

	SelectObject(hdc, hbrushOld);
	DeleteObject(hbrushOld);
	DeleteObject(hbrush);
}
/////////////////////////////////////////////////////////////////////////////////////////
inline void FrameRect(HDC hdc, int left, int top, int right, int bottom, COLORREF rgb)
{
	RECT rc = {left, top, right, bottom};

	FrameRect(hdc, &rc, rgb);
}

/////////////////////////////////////////////////////////////////////////////////////////
inline void FillRgn(HDC hdc, HRGN hRgn, COLORREF rgb)
{
	HBRUSH hbrush = CreateSolidBrush(rgb);

	FillRgn(hdc, hRgn, hbrush);

	DeleteObject(hbrush);
}
/////////////////////////////////////////////////////////////////////////////////////////
inline void FrameRgn(HDC hdc, HRGN hRgn, COLORREF rgb, int nWidth =1, int nHeight =1)
{
	HBRUSH hbrush = CreateSolidBrush(rgb);

	FrameRgn(hdc, hRgn, hbrush, nWidth, nHeight);

	DeleteObject(hbrush);
}
/////////////////////////////////////////////////////////////////////////////////////////
inline void FillRect(HDC hdc, const RECT* pRc, COLORREF rgb)
{
	HBRUSH hbrush = CreateSolidBrush(rgb);
	HBRUSH hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);

	FillRect(hdc, pRc, hbrush);

	SelectObject(hdc, hbrushOld);
	DeleteObject(hbrushOld);
	DeleteObject(hbrush);
}
/////////////////////////////////////////////////////////////////////////////////////////
inline void FillRect(HDC hdc, int left, int top, int right, int bottom, COLORREF rgb)
{
	RECT rc = {left, top, right, bottom};

	FillRect(hdc, &rc, rgb);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


////  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// error: conversion from 'float' to 'int', possible loss of data(Did you mean to call 'fabsf' or to #include <cmath> ? )
// so we comment it
//typedef COLORREF TXYrgb(float x, float y);

//////////////////////////////////////////////////////////////////////////////////////
// Example of  color selector function
//inline COLORREF XYrgb(float x, float y)
//{
//	float zz = (pow(2*x-1,int(8)) - pow(2*y-1,int(8)));
//	
//	return	(abs(zz) >1)	? RGB(255, 255,255)
//							: RGB(122 + 133*zz, 255*x, 233*zz) ;
//}

//////////////////////////////////////////////////////////////////////////////////////
// Create DIB Section, fill it using color selector function pointed by pXYrgb, and transfer DIB to DC
//inline HBITMAP PaintRect( HDC hdc,
//					int nXDest,		// x-coord of destination upper-left corner
//					int nYDest,		// y-coord of destination upper-left corner
//					int nWidth,		// width of destination rectangle
//					int nHeight,	// height of destination rectangle
//					TXYrgb* pXYrgb,		// pointer to color selector function
//					DWORD dwRop =SRCCOPY		// raster operation code
//					)
//{
//	COLORREF* pARGB;
//	BITMAPINFO bi;
//	HDC hmdcS;
//	HBITMAP hBmpS;
//
//	//---------------------------------------------------------------------------------------
//	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
//	bi.bmiHeader.biWidth = nWidth;
//	bi.bmiHeader.biHeight = nHeight;
//	bi.bmiHeader.biPlanes = 1; 
//	bi.bmiHeader.biBitCount = 32; 
//	bi.bmiHeader.biCompression = BI_RGB; 
//	bi.bmiHeader.biSizeImage =0; 
//	bi.bmiHeader.biXPelsPerMeter=0; //?
//	bi.bmiHeader.biYPelsPerMeter=0; //?
//	bi.bmiHeader.biClrUsed = 0; 
//	bi.bmiHeader.biClrImportant=0; 
//		
//	//---------------------------------------------------------------------------------------
//	hmdcS= CreateCompatibleDC(NULL);
//
//	hBmpS= CreateDIBSection( hmdcS,		// handle to DC
//							&bi,		// bitmap data
//							DIB_RGB_COLORS,	// data type indicator
//							(void**)&pARGB,	// bit values
//							0,				// handle to file mapping object
//							0				// offset to bitmap bit values
//							);
//
//	HGDIOBJ hObjPrev = SelectObject(hmdcS, hBmpS);
//	
//	//---------------------------------------------------------------------------------
//	for(int ny = 0; ny < nHeight; ny++)
//	{
//		int NY = ny * nWidth;
//		float y = float(ny) / nHeight;
//
//		for(int nx = 0; nx < nWidth; nx++)
//		{
//			*(pARGB + NY + nx) = pXYrgb( float(nx) / nWidth, y);
//		}
//	}
//
//	//---------------------------------------------------------------------------------
//	BitBlt(hdc, nXDest, nYDest, nWidth, nHeight, hmdcS, 0, 0, dwRop);
//
//	//---------------------------------------------------------------------------------
//	SelectObject(hmdcS, hObjPrev);
//	DeleteObject(hBmpS);
//	DeleteDC(hmdcS);
//	
//	//---------------------------------------------------------------------------------
//	return 0;
//
//}

///////////////// Draw triangle //////////////////////////////////////////////////////
// see Lab_GradientFill for usage example

/////////////////////////////////////////////////////////////////////////////////////////////
inline void GradientTri(HDC hdc, 
				 int x1, int y1, COLORREF rgb1, 
				 int x2, int y2, COLORREF rgb2, 
				 int x3, int y3, COLORREF rgb3 )
{
	TRIVERTEX vert [3];
	GRADIENT_TRIANGLE gt[1];

	SetVertex(&vert[0], x1, y1, rgb1 );
	SetVertex(&vert[1], x2, y2, rgb2 );
	SetVertex(&vert[2], x3, y3, rgb3 );

	gt[0].Vertex1   = 0;
	gt[0].Vertex2   = 1;
	gt[0].Vertex3   = 2;

	GradientFill(hdc, (TRIVERTEX*)vert, 3, gt, 1, GRADIENT_FILL_TRIANGLE);
}

///////////////////////////////////////////////////////////////////////////////////////////////
inline void FillRectSlow( HDC hdc, const RECT const* pRc, COLORREF rgb, int nPause)
{
	HPEN  hPen = CreatePen(PS_SOLID, 1, rgb);

	HPEN hPenOld = (HPEN)SelectObject(hdc, hPen);

	if(bSlowRedraw && nPause)
	{
		for(int y = pRc->top; y < pRc->bottom; ++y)
		{
			MoveToEx( hdc, pRc->left, y, 0);

			LineTo( hdc, pRc->right, y);

			if(0== (y % nPause))
				Sleep(nSlowRedraw);
		}
	}
	else
	{
		FillRect( hdc, pRc, rgb);
	}

	SelectObject(hdc, hPenOld);

	DeleteObject(hPen);
}

/////////////////////////////////////////////////////////////////////////////////////////
inline void ShowRgn(HRGN hRgn, HWND hw, COLORREF color , TCHAR* pInfo )//= RGB(0,0,255) = L"ShowRgn:"
{
	if(hw)
	{
		//InvalidateRect(hw, 0, 1);
		//UpdateWindow(hw);

		SetWindowText(hw, pInfo);

		HDC hDC = GetDC(hw);

		FillRgn(hDC,hRgn,color);

		ReleaseDC(hw, hDC);
	}
}

inline HRGN CreateStarRgn(int nPointed, int Sz)
{

	POINT* pShapePt = new POINT[2*nPointed];// code anal: pShapePt not init

	for(int n = 2*nPointed; --n >= 0; )
	{
		(pShapePt+n)->x = Sz/2 + ((n%2)? Sz/2 : Sz/4) * sin( n*(3.141592/nPointed));
		(pShapePt+n)->y = Sz/2 + ((n%2)? Sz/2 : Sz/4) * cos( n*(3.141592/nPointed));
	}

	HRGN hRgn = CreatePolygonRgn( pShapePt, 2*nPointed, ALTERNATE);// code anal: uninitialized memory

	delete [] pShapePt;// was not []

	return hRgn;
}

/////////////////////////////////////////////////////////////////////////////////////////
// x,y are relative left/top corner of the rcHost,
// and only height/width of the rcHost has effect
const int nDocZone = 4; // define doc zone as width/height portion
inline char getDocRc(RECT rcHost, int x, int y)
{
	char nZone = -1;
	int dx = (rcHost.right - rcHost.left);
	int dy = (rcHost.bottom - rcHost.top);
	//---- Set relative scale ---------------------------------------
	x = x * dy;
	y = y * dx;
	//---------------------------------------------------------------
	if((y > x) && (y < (dx*dy - x)))		// left
	{
		if(nDocZone * x < dx*dy) { nZone =0; }
	}
	else if((y < x) && (y < (dx*dy-x))) // top
	{
		if(nDocZone * y < dx*dy) { nZone =1; }
	}
	else if((y < x) && (y > (dx*dy - x))) // right
	{
		if(nDocZone*(dx*dy - x) < dx*dy) { nZone =2; }
	}
	else if((y > x) && (y > (dx*dy-x))) // bottom
	{
		if(nDocZone*(dx*dy - y) < dx*dy) { nZone =3; }
	}
	//---------------------------------------------------------------
	return nZone;
}

/////////////////////////////////////////////////////////////////////////////////////////
// In contrast to MapWindowPoints POINT* pXY are relative to the 
// upper-left corner of the window, not the client area of the window
inline void MapPoints(HWND hwFrom, HWND hwTo, POINT* pXY)
{
	if(hwFrom)
	{
		RECT rcFrom;
		GetWindowRect(hwFrom, &rcFrom);
		pXY->x += rcFrom.left;
		pXY->y += rcFrom.top;
	}

	if(hwTo)
	{
		RECT rcTo;
		GetWindowRect(hwTo, &rcTo);
		pXY->x -= rcTo.left;
		pXY->y -= rcTo.top;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////
// In contrast to MapWindowPoints POINT* pXY are relative to the 
// upper-left corner of the window, not the client area of the window
inline void MapPoints(HWND hwFrom, HWND hwTo, RECT* pRc)
{
	if(hwFrom)
	{
		RECT rcFrom;
		GetWindowRect(hwFrom, &rcFrom);

		pRc->left += rcFrom.left;
		pRc->right += rcFrom.left;
		pRc->top += rcFrom.top;
		pRc->bottom += rcFrom.top;
	}

	if(hwTo)
	{
		RECT rcTo;
		GetWindowRect(hwTo, &rcTo);
		pRc->left -= rcTo.left;
		pRc->right -= rcTo.left;
		pRc->top -= rcTo.top;
		pRc->bottom -= rcTo.top;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


