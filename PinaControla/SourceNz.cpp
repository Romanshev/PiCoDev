// some useful code
#include "stdafx.h"
#include "Uxtheme.h"  // but simply set null theme

// contain visual style def ( burn after using )
#include "Vsstyle.h" ////////////////////////////
#include "Vssym32.h" //////////////////////////////
///////////////////// delete this //////////////////
//==================== CUSTOM DRAW ==========================================

// custom draw return flags
// values under 0x00010000 are reserved for global custom draw values.
// above that are for specific controls
//#define CDRF_DODEFAULT          0x00000000
//#define CDRF_NEWFONT            0x00000002
//#define CDRF_SKIPDEFAULT        0x00000004
//#define CDRF_DOERASE            0x00000008 // draw the background
//#define CDRF_SKIPPOSTPAINT      0x00000100 // don't draw the focus rect
//
//#define CDRF_NOTIFYPOSTPAINT    0x00000010
//#define CDRF_NOTIFYITEMDRAW     0x00000020
//#define CDRF_NOTIFYSUBITEMDRAW  0x00000020  // flags are the same, we can distinguish by context
//#define CDRF_NOTIFYPOSTERASE    0x00000040
//
//// drawstage flags
//// values under 0x00010000 are reserved for global custom draw values.
//// above that are for specific controls
//#define CDDS_PREPAINT           0x00000001
//#define CDDS_POSTPAINT          0x00000002
//#define CDDS_PREERASE           0x00000003
//#define CDDS_POSTERASE          0x00000004
//// the 0x000010000 bit means it's individual item specific
//#define CDDS_ITEM               0x00010000
//#define CDDS_ITEMPREPAINT       (CDDS_ITEM | CDDS_PREPAINT)
//#define CDDS_ITEMPOSTPAINT      (CDDS_ITEM | CDDS_POSTPAINT)
//#define CDDS_ITEMPREERASE       (CDDS_ITEM | CDDS_PREERASE)
//#define CDDS_ITEMPOSTERASE      (CDDS_ITEM | CDDS_POSTERASE)
//#define CDDS_SUBITEM            0x00020000
//
//// itemState flags
//#define CDIS_SELECTED           0x0001
//#define CDIS_GRAYED             0x0002
//#define CDIS_DISABLED           0x0004
//#define CDIS_CHECKED            0x0008
//#define CDIS_FOCUS              0x0010
//#define CDIS_DEFAULT            0x0020
//#define CDIS_HOT                0x0040
//#define CDIS_MARKED             0x0080
//#define CDIS_INDETERMINATE      0x0100
//#if (NTDDI_VERSION >= NTDDI_WINXP)
//#define CDIS_SHOWKEYBOARDCUES   0x0200
//#endif
//#if (NTDDI_VERSION >= NTDDI_VISTA)
//#define CDIS_NEARHOT            0x0400
//#define CDIS_OTHERSIDEHOT       0x0800
//#define CDIS_DROPHILITED        0x1000
//#endif

//=== BTNS_DROPDOWN ====

//	if ( getStyle(pData->fButton) & BTNS_DROPDOWN )
//	{SendMessageW(pcd->nmcd.hdr.hwndFrom, TB_GETITEMDROPDOWNRECT, 
//  WPARAM(pcd->nmcd.dwItemSpec), LPARAM(&pcd->nmcd.rc));

//=== COLORSCHEME ====

//	COLORSCHEME clrscheme = { 0 };
//	clrscheme.dwSize = sizeof(COLORSCHEME);
//	SendMessageW(pcd->nmcd.hdr.hwndFrom, TB_GETCOLORSCHEME,	WPARAM(0), (LPARAM)(COLORSCHEME*)&clrscheme);
//	clrscheme.clrBtnHighlight = RGB(200, 10, 10);
//	clrscheme.clrBtnShadow = RGB(100, 50, 50);
//	SendMessageW(pcd->nmcd.hdr.hwndFrom, TB_SETCOLORSCHEME,
//		WPARAM(0), (LPARAM)(COLORSCHEME*)&clrscheme);
//	// draw DDbtn...ITEMPREPAINT DrawStage

//IUIBarData* pData = reinterpret_cast<IUIBarData*>(pcd->nmcd.lItemlParam);
//  BYTE state = (BYTE)SendMessageW(pcd->nmcd.hdr.hwndFrom, 
//  TB_GETSTATE, WPARAM(pcd->nmcd.dwItemSpec), 0);	

//==== Marked handler =====	

	//if (pcd->nmcd.uItemState & CDIS_MARKED) // Marked items
		//{
		//	if (pData->pBtn_MarkedBk)
		//	{
		//		GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_MarkedBk);
		//	}
		//	else if (pBar->pBarTem_MarkedBk)
		//	{
		//		GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_MarkedBk);
		//	}
		//	//-----------------------------------------------------------
		//	if (pData->pBtn_MarkedBorder)
		//	{
		//		FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_MarkedBorder);
		//	}
		//	else if (pBar->pBarTem_MarkedBorder)
		//	{
		//		FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_MarkedBorder);
		//	}
		//	//-----------------------------------------------------------								
		//}  //	  end of Marked state

//==== Default handler =====

	//else if(pcd->nmcd.uItemState & CDIS_DEFAULT)          
			//{	//---------------------------------------------------------------------  Bk
			//	if (pData->pBtn_DefaultBk)
			//	{
			//		GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_DefaultBk);
			//	}
			//	else if (pBar->pBarTem_DefaultBk)
			//	{
			//		GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_DefaultBk);
			//	}
			//	//--------------------------------------------- ----------------------   Border
			//	//if(pBar->pBarBorder)
			//	////{   FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *(pBar->pBarBorder));}
			//	if (pData->pBtn_DefaultBorder)     // ind btn
			//	{
			//		FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_DefaultBorder);
			//	}
			//	else if (pBar->pBarTem_DefaultBorder)
			//	{
			//		FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_DefaultBorder);
			//	}
			//	//--------------------------------------------- -----------------------   Label
			//	if (pData->pBtn_DefaultLabelClr)
			//	{
			//		pcd->clrText = *pData->pBtn_DefaultLabelClr;
			//	}
			//	else if (pBar->pBarTem_DefaultLabelClr)
			//	{
			//		pcd->clrText = *pBar->pBarTem_DefaultLabelClr;
			//	}
			//	else if (pBar->pBarLabelClr) // TODO:    add this block to all label setting parts
			//	{
			//		pcd->clrText = *pBar->pBarLabelClr;
			//	}// or to the control prepaint stage	
//}//  end of default state   -----
////====================================== Now Custom Drawing   ==============================================

//==== Disabled handler ========

	//if (pcd->nmcd.uItemState & CDIS_DISABLED)
	//{
	//	if (pData->pBtn_DisabledBk)
	//	{
	//		GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_DisabledBk);
	//	}
	//	else if (pBar->pBarTem_DisabledBk)
	//	{
	//		GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_DisabledBk);
	//	}
	//	//-----------------------------------------------------------
	//	if (pData->pBtn_DisabledBorder)
	//	{
	//		FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_DisabledBorder);
	//	}
	//	else if (pBar->pBarTem_DisabledBorder)
	//	{
	//		FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_DisabledBorder);
	//	}
	//	if (pData->pBtn_DisabledLabelClr)
	//	{
	//		pcd->clrText = *pData->pBtn_DisabledLabelClr;
	//	}
	//	else if (pBar->pBarTem_DisabledLabelClr)
	//	{
	//		pcd->clrText = *pBar->pBarTem_DisabledLabelClr;
	//	}
	//}
//===== Custom Draw  ===== v. 1.1

//=== Hovered HOT handler  =====

	//if (pcd->nmcd.uItemState & CDIS_HOT)//Checked and hovered
	//{   //----------------------------------------------------------  Bk
	//	if (pData->pBtn_CheckedHoveredBk)
	//	{
	//		GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_CheckedHoveredBk);
	//	}
	//	else if (pBar->pBarTem_CheckedHoveredBk)
	//	{
	//		GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_CheckedHoveredBk);
	//	}
	//	//----------------------------------------------------------- Border
	//	if (pData->pBtn_CheckedHoveredBorder)
	//	{
	//		FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_CheckedHoveredBorder);
	//	}
	//	else if (pBar->pBarTem_CheckedHoveredBorder)
	//	{
	//		FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_CheckedHoveredBorder);
	//	}
	//	//------------------------------------------------------------- Label
	//	if (pData->pBtn_CheckedHoveredLabelClr)
	//	{
	//		pcd->clrText = *pData->pBtn_CheckedHoveredLabelClr;
	//	}
	//	else if (pBar->pBarTem_CheckedHoveredLabelClr)
	//	{
	//		pcd->clrText = *pBar->pBarTem_CheckedHoveredLabelClr;
	//	}
	//}// end Checked, hovered

//=== Checked handler  =====

	//if (pcd->nmcd.uItemState & CDIS_CHECKED)//Checked
		//{
		//	//-----------------------------------------------------------
		//	if (pData->pBtn_CheckedBk)
		//	{
		//		GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_CheckedBk);
		//	}
		//	else if (pBar->pBarTem_CheckedBk)
		//	{
		//		GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_CheckedBk);
		//	}
		//	//-----------------------------------------------------------
		//	if (pData->pBtn_CheckedBorder)
		//	{
		//		FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_CheckedBorder);
		//	}
		//	else if (pBar->pBarTem_CheckedBorder)
		//	{
		//		FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_CheckedBorder);
		//	}
		//	//-----------------------------------------------------------
		//	if (pData->pBtn_CheckedLabelClr)
		//	{
		//		pcd->clrText = *pData->pBtn_CheckedLabelClr;
		//	}
		//	else if (pBar->pBarTem_CheckedLabelClr)
		//	{
		//		pcd->clrText = *pBar->pBarTem_CheckedLabelClr;
		//	}
		//}

//==== Checked and hovered ====

	//if (pcd->nmcd.uItemState & CDIS_CHECKED & CDIS_HOT)
	//{   //----------------------------------------------------------  Bk
	//	if (pData->pBtn_CheckedHoveredBk)
	//	{
	//		GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_CheckedHoveredBk);
	//	}
	//	else if (pBar->pBarTem_CheckedHoveredBk)
	//	{
	//		GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_CheckedHoveredBk);
	//	}
	//	//----------------------------------------------------------- Border
	//	if (pData->pBtn_CheckedHoveredBorder)
	//	{
	//		FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_CheckedHoveredBorder);
	//	}
	//	else if (pBar->pBarTem_CheckedHoveredBorder)
	//	{
	//		FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_CheckedHoveredBorder);
	//	}
	//	//------------------------------------------------------------- Label
	//	if (pData->pBtn_CheckedHoveredLabelClr)
	//	{
	//		pcd->clrText = *pData->pBtn_CheckedHoveredLabelClr;
	//	}
	//	else if (pBar->pBarTem_CheckedHoveredLabelClr)
	//	{
	//		pcd->clrText = *pBar->pBarTem_CheckedHoveredLabelClr;
	//	}
	//}// end Checked, hovered

//==== Selected (pressed) ====

	//if (pcd->nmcd.uItemState & CDIS_SELECTED) // TBSTATE_PRESSED --> CDIS_SELECTED								
	//{  //-----------------------------------------------------------------------  Bk
	//	if (pData->pBtn_PressedBk)
	//	{
	//		GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_PressedBk);
	//	}
	//	else if (pBar->pBarTem_PressedBk)
	//	{
	//		GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_PressedBk);
	//	}
	//	//----------------------------------------------------------------------  Border
	//	if (pData->pBtn_PressedBorder)
	//	{
	//		FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_PressedBorder);
	//	}
	//	else if (pBar->pBarTem_PressedBorder)
	//	{
	//		FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_PressedBorder);
	//	}
	//	//--------------------------------------------- -----------------------   Label										
	//	if (pData->pBtn_PressedLabelClr)
	//	{
	//		pcd->clrText = *pData->pBtn_PressedLabelClr;
	//	}
	//	else if (pBar->pBarTem_PressedLabelClr)
	//	{
	//		pcd->clrText = *pBar->pBarTem_PressedLabelClr;
	//	}
	//}   // end of CDIS_SELECTED

		//else if (NM_CUSTOMDRAW == LPNMHDR(lParam)->code)
		//{
		//	NMTBCUSTOMDRAW *pcd = (NMTBCUSTOMDRAW*)lParam;
		//	pcd->hbrMonoDither = 0;// is to draw checked Bk- todo pBar->pbrMonoDither!!!!!
		//	//pcd->clrBtnFace = CLR_NONE;
		//	pcd->clrBtnFace = RGB(234, 10, 10);
		//	//pcd->clrHighlightHotTrack = CLR_DEFAULT;
		//	pcd->clrHighlightHotTrack = RGB(234, 10, 10);
		//	//pcd->clrBtnHighlight = CLR_NONE;
		//	pcd->clrBtnHighlight = RGB(234, 10, 10);
		//	//pcd->nHLStringBkMode = OPAQUE;
		//	//pcd->nStringBkMode = OPAQUE;
		//	//pcd->hbrLines = CreateSolidBrush(RGB(200, 50, 20));    //0;
		//	//SelectObject(pcd->nmcd.hdc, pcd->hbrLines);						
		//	//pcd->hpenLines = CreatePen(PS_DOT, 1, RGB(0, 255, 0)); //0;
		//	//SelectObject(pcd->nmcd.hdc, pcd->hpenLines);
		//	// COLORSCHEME cs get it by TB_GETCOLORSCHEME

		//	//==== Control Prepaint Draw Stage    =============================================================================
		//	if (pcd->nmcd.dwDrawStage == CDDS_PREPAINT)
		//	{
		//		if (pBar->pBarBk)
		//		{
		//			GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *(pBar->pBarBk));
		//		}
		//		if (pBar->pBarBorder)
		//		{
		//			FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *(pBar->pBarBorder));
		//		}
		//		return (CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT);
		//	} // end of CDDS_PREPAINT 
		//	//========================================================================================================
		//	else if (pcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
		//	{
		//		IUIBarData* pData = reinterpret_cast<IUIBarData*>(pcd->nmcd.lItemlParam);
		//		//  BYTE state = (BYTE)SendMessageW(pcd->nmcd.hdr.hwndFrom, 
		//		//  TB_GETSTATE, WPARAM(pcd->nmcd.dwItemSpec), 0);	

		//		if (pcd->nmcd.uItemState & CDIS_DISABLED)
		//		{
		//			if (pData->pBtn_DisabledBk)
		//			{
		//				GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_DisabledBk);
		//			}
		//			else if (pBar->pBarTem_DisabledBk)
		//			{
		//				GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_DisabledBk);
		//			}
		//			//-----------------------------------------------------------
		//			if (pData->pBtn_DisabledBorder)
		//			{
		//				FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_DisabledBorder);
		//			}
		//			else if (pBar->pBarTem_DisabledBorder)
		//			{
		//				FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_DisabledBorder);
		//			}
		//			if (pData->pBtn_DisabledLabelClr)
		//			{
		//				pcd->clrText = *pData->pBtn_DisabledLabelClr;
		//			}
		//			else if (pBar->pBarTem_DisabledLabelClr)
		//			{
		//				pcd->clrText = *pBar->pBarTem_DisabledLabelClr;
		//			}
		//		}
		//		else if (pcd->nmcd.uItemState & CDIS_CHECKED) // Checked items
		//		{
		//			//=== Checked and hovered ==================						
		//			if (pcd->nmcd.uItemState & CDIS_HOT)//Checked and hovered
		//			{   //----------------------------------------------------------  Bk
		//				if (pData->pBtn_CheckedHoveredBk)
		//				{
		//					GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_CheckedHoveredBk);
		//				}
		//				else if (pBar->pBarTem_CheckedHoveredBk)
		//				{
		//					GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_CheckedHoveredBk);
		//				}
		//				//----------------------------------------------------------- Border
		//				if (pData->pBtn_CheckedHoveredBorder)
		//				{
		//					FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_CheckedHoveredBorder);
		//				}
		//				else if (pBar->pBarTem_CheckedHoveredBorder)
		//				{
		//					FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_CheckedHoveredBorder);
		//				}
		//				//------------------------------------------------------------- Label
		//				if (pData->pBtn_CheckedHoveredLabelClr)
		//				{
		//					pcd->clrText = *pData->pBtn_CheckedHoveredLabelClr;
		//				}
		//				else if (pBar->pBarTem_CheckedHoveredLabelClr)
		//				{
		//					pcd->clrText = *pBar->pBarTem_CheckedHoveredLabelClr;
		//				}
		//			}// end Checked, hovered
		//			else{
		//				//-----------------------------------------------------------
		//				if (pData->pBtn_CheckedBk)
		//				{
		//					GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_CheckedBk);
		//				}
		//				else if (pBar->pBarTem_CheckedBk)
		//				{
		//					GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_CheckedBk);
		//				}
		//				//-----------------------------------------------------------
		//				if (pData->pBtn_CheckedBorder)
		//				{
		//					FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_CheckedBorder);
		//				}
		//				else if (pBar->pBarTem_CheckedBorder)
		//				{
		//					FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_CheckedBorder);
		//				}
		//				//-----------------------------------------------------------
		//				if (pData->pBtn_CheckedLabelClr)
		//				{
		//					pcd->clrText = *pData->pBtn_CheckedLabelClr;
		//				}
		//				else if (pBar->pBarTem_CheckedLabelClr)
		//				{
		//					pcd->clrText = *pBar->pBarTem_CheckedLabelClr;
		//				}
		//			}//end of Checked state 
		//		} //end of Checked and/or Hovered state  	
		//		// hot: pressed-or-unpressed						
		//		else if (pcd->nmcd.uItemState & CDIS_HOT) // hot: pressed-or-unpressed
		//		{
		//			//pressed - cdis_selected - allways hovered ===
		//			if (pcd->nmcd.uItemState & CDIS_SELECTED) // TBSTATE_PRESSED --> CDIS_SELECTED								
		//			{  //-----------------------------------------------------------------------  Bk
		//				if (pData->pBtn_PressedBk)
		//				{
		//					GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_PressedBk);
		//				}
		//				else if (pBar->pBarTem_PressedBk)
		//				{
		//					GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_PressedBk);
		//				}
		//				//----------------------------------------------------------------------  Border
		//				if (pData->pBtn_PressedBorder)
		//				{
		//					FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_PressedBorder);
		//				}
		//				else if (pBar->pBarTem_PressedBorder)
		//				{
		//					FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_PressedBorder);
		//				}
		//				//--------------------------------------------- -----------------------   Label										
		//				if (pData->pBtn_PressedLabelClr)
		//				{
		//					pcd->clrText = *pData->pBtn_PressedLabelClr;
		//				}
		//				else if (pBar->pBarTem_PressedLabelClr)
		//				{
		//					pcd->clrText = *pBar->pBarTem_PressedLabelClr;
		//				}
		//			}   // end of CDIS_SELECTED
		//			// only hovered, not checked-hovered or pressed						
		//			else
		//			{// only hovered, not checked-hovered or pressed
		//				if (pData->pBtn_HoveredBk)
		//				{
		//					GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_HoveredBk);
		//				}
		//				else if (pBar->pBarTem_HoveredBk)
		//				{
		//					GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_HoveredBk);
		//				}
		//				//-----------------------------------------------------------   Border
		//				if (pData->pBtn_HoveredBorder)
		//				{
		//					FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *(pData->pBtn_HoveredBorder));
		//					////////////////////////// rcdd
		//					//FrameRect(pcd->nmcd.hdc, &rcdd, *(pData->pBtn_HoveredBorder));
		//				}
		//				else if (pBar->pBarTem_HoveredBorder)
		//				{
		//					FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *(pBar->pBarTem_HoveredBorder));
		//					////////////////////////// rcdd											
		//					//FrameRect(pcd->nmcd.hdc, &rcdd, *(pBar->pBarTem_HoveredBorder));
		//				}

		//				/*if(pData->pBtn_HoveredBorder)
		//				{FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *(pData->pBtn_HoveredBorder));}
		//				else if(pBar->pBarTem_HoveredBorder)
		//				{FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *(pBar->pBarTem_HoveredBorder));}*/
		//				//-----------------------------------------------------------  Label								
		//				if (pData->pBtn_HoveredLabelClr)
		//				{
		//					pcd->clrText = *pData->pBtn_HoveredLabelClr;
		//				}
		//				else if (pBar->pBarTem_HoveredLabelClr)
		//				{
		//					pcd->clrText = *pBar->pBarTem_HoveredLabelClr;
		//				}
		//			}// end only hovered
		//		} //  end of all hovered			


		//		//==== Normal - Default - handler CDIS_DEFAULT =============
		//		else  //if(pcd->nmcd.uItemState & CDIS_DEFAULT)          
		//		{	//---------------------------------------------------------------------  Bk
		//			if (pData->pBtn_DefaultBk)
		//			{
		//				GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_DefaultBk);
		//			}
		//			else if (pBar->pBarTem_DefaultBk)
		//			{
		//				GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_DefaultBk);
		//			}
		//			//--------------------------------------------- ----------------------   Border
		//			//if(pBar->pBarBorder)
		//			////{   FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *(pBar->pBarBorder));}
		//			if (pData->pBtn_DefaultBorder)     // ind btn
		//			{
		//				FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_DefaultBorder);
		//			}
		//			else if (pBar->pBarTem_DefaultBorder)
		//			{
		//				FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_DefaultBorder);
		//			}

		//			//--------------------------------------------- -----------------------   Label
		//			if (pData->pBtn_DefaultLabelClr)
		//			{
		//				pcd->clrText = *pData->pBtn_DefaultLabelClr;
		//			}
		//			else if (pBar->pBarTem_DefaultLabelClr)
		//			{
		//				pcd->clrText = *pBar->pBarTem_DefaultLabelClr;
		//			}
		//			else if (pBar->pBarLabelClr) // TODO:    add this block to all label setting parts
		//			{
		//				pcd->clrText = *pBar->pBarLabelClr;
		//			}// or to the control prepaint stage								

		//		}//  end of default state   ---------------------------------------------												
		//		//return (CDRF_DODEFAULT | getDrawStyle(pData->fButton)); 
		//		//CDRF_DODEFAULT can not be used with any other flags
		//		//if (getStyle(pData->fButton) & BTNS_DROPDOWN)
		//		//{ return (CDRF_NOTIFYSUBITEMDRAW | getDrawStyle(pData->fButton)); }
		//		//else 
		//		return (getDrawStyle(pData->fButton));// trans TBCDRF_DRAWSTYLES
		//	} // end of CDDS_ITEMPREPAINT
		//	if (pcd->nmcd.dwDrawStage == CDDS_SUBITEM | CDDS_ITEMPREPAINT)
		//	{
		//		RECT rcdd;
		//		SendMessageW(pcd->nmcd.hdr.hwndFrom, TB_GETITEMDROPDOWNRECT,
		//			WPARAM(pcd->nmcd.dwItemSpec), LPARAM(&pcd->nmcd.rc));


		//		//rcdd.left = (pcd->nmcd.rc).right -10;// middle point 
		//		//rcdd.right = (pcd->nmcd.rc).right+2;
		//		//rcdd.top = (pcd->nmcd.rc).top;							
		//		//rcdd.bottom = (pcd->nmcd.rc).bottom;

		//		if (pcd->nmcd.uItemState & CDIS_HOT)
		//		{
		//			if (pData->pBtn_HoveredBorder)
		//			{
		//				FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *(pData->pBtn_HoveredBorder));
		//			}
		//			else if (pBar->pBarTem_HoveredBorder)
		//			{
		//				FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *(pBar->pBarTem_HoveredBorder));
		//			}
		//			if (pData->pBtn_HoveredLabelClr)
		//			{
		//				pcd->clrText = *pData->pBtn_HoveredLabelClr;
		//			}
		//			else if (pBar->pBarTem_HoveredLabelClr)
		//			{
		//				pcd->clrText = *pBar->pBarTem_HoveredLabelClr;
		//			}
		//		}
		//		else if (pcd->nmcd.uItemState & CDIS_DEFAULT)
		//		{
		//			if (pData->pBtn_DefaultBorder)
		//			{
		//				FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *(pData->pBtn_DefaultBorder));
		//			}
		//			else if (pBar->pBarTem_DefaultBorder)
		//			{
		//				FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *(pBar->pBarTem_DefaultBorder));
		//			}

		//		}
		//		//return getDrawStyle(pData->fButton);
		//	}
		//	//}
		//	else if (pcd->nmcd.dwDrawStage == CDDS_ITEMPOSTPAINT)
		//	{
		//		//	//=========== back all setting to Default ?
		//		//	
		//		//		if(pData->pBtn_DefaultBk) 
		//		//		{	GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_DefaultBk);}							
		//		//		else if(pBar->pBarTem_DefaultBk) 
		//		//		{	GradientRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_DefaultBk);}
		//		//		//---------------------------------------------------------------------  Border
		//		//		if(pData->pBtn_DefaultBorder) 
		//		//		{ FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pData->pBtn_DefaultBorder);}								
		//		//		else if(pBar->pBarTem_DefaultBorder)  
		//		//		{	FrameRect(pcd->nmcd.hdc, &pcd->nmcd.rc, *pBar->pBarTem_DefaultBorder);}
		//		//		//----------------------------------------------------------------------  Labels
		//		//		if(pData->pBtn_DefaultLabelClr)
		//		//		{ pcd->clrText=*pData->pBtn_DefaultLabelClr;}
		//		//		else if(pBar->pBarTem_DefaultLabelClr) 
		//		//		{ pcd->clrText=*pBar->pBarTem_DefaultLabelClr;}
		//		//	//===========
		//		//	return CDRF_DODEFAULT;

		//		return CDRF_NOTIFYPOSTPAINT;
		//	}	// end of Item PostPaint ds		
		//	else if (pcd->nmcd.dwDrawStage == CDDS_POSTPAINT)
		//	{
		//		//InvalidateRect(
		//		//SetBkMode(pcd->nmcd.hdc, TRANSPARENT); 
		//		//HFONT hFontforTbTrial = (HFONT) ::SendMessage(pcd->nmcd.hdr.hwndFrom, WM_GETFONT, 0, 0);
		//		//SelectObject(pcd->nmcd.hdc, hFontforTbTrial);
		//		//SetTextColor(pcd->nmcd.hdc, RGB( 250,200,190));
		//		//TextOutW(pcd->nmcd.hdc,400,45,L"http://nativeruse.com", 21);									
		//		//DrawTextW(pcd->nmcd.hdc, L":::Trial:::NuiBar:::", 20, &pcd->nmcd.rc, DT_RIGHT);
		//		/*HFONT hFont = (HFONT)GetStockObject(SYSTEM_FONT);
		//		TextOut(pcd->nmcd.hdc, 210, 25, L"Sample SYSTEM_FONT text", 25); */
		//	}
		//	return CDRF_DODEFAULT;
		//}// end of Custom Draw  ----------------------------------------------------


