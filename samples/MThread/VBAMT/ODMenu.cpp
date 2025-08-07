//---------------------------------------------------------------------------
// ODMenu.cpp
//---------------------------------------------------------------------------
// Generic Owner Draw menu item support

/*	Copyright (C) Microsoft Corporation, 1999.  All rights reserved.


	This source code is intended only as a supplement to Microsoft
	Development Tools and/or on-line documentation.  See these other
	materials for detailed information regarding Microsoft code samples.
*/
#include "stdafx.h"
#include "odmenu.h"
#include <mbstring.h>

#ifndef ASSERT
#ifdef _DEBUG
#include <assert.h>
#define ASSERT(x, y) assert(x);
#else
#define ASSERT(x, y)
#endif
#endif

CODMenu::CODMenu(HINSTANCE hInstRC, UINT idMenu, UINT idText, UINT idBitMain, UINT idBitGray)
{
	m_hInstRC = hInstRC;
	m_idMenu = idMenu;
	m_idText = idText;
	m_idBitMain = idBitMain;
	m_idBitGray = idBitGray;

	if(m_idBitMain)
		m_hbitMain = LoadBitmap(m_hInstRC, MAKEINTRESOURCE(m_idBitMain));
	else
		m_hbitMain = NULL;

	if(m_idBitGray)
		m_hbitGray = LoadBitmap(m_hInstRC, MAKEINTRESOURCE(m_idBitGray));
	else
		m_hbitGray = NULL;

	LoadString(m_hInstRC, m_idText, m_szText, _MAX_PATH);

	//figure out mnemonic
	unsigned char* pAmp = _mbschr((unsigned char*)m_szText, '&');
	ASSERT(pAmp, "No mnemonic in menu!");

	m_chMenu = *(++pAmp);

	//now figure out how big everything should be
	HDC hdcTemp;
	HFONT hfntMenu, hfntPrev;
	RECT rcText;
	memset(&rcText, 0, sizeof(RECT));
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
	hfntMenu = CreateFontIndirect(&(ncm.lfMenuFont));
	hdcTemp = CreateCompatibleDC(NULL);
	hfntPrev = (HFONT)SelectObject(hdcTemp, hfntMenu);

	DrawText(hdcTemp, m_szText, -1, &rcText, DT_CALCRECT | DT_EXPANDTABS );

	m_nspace = GetSystemMetrics(SM_CYBORDER);
	m_nMenuHeight = GetSystemMetrics(SM_CYMENU);
	m_nMenuWidth = (GetSystemMetrics(SM_CXBORDER) * 4) + m_nMenuHeight + (rcText.right - rcText.left);

	DeleteObject(hfntMenu);
	SelectObject(hdcTemp, hfntPrev);
	DeleteDC(hdcTemp);
}

CODMenu::~CODMenu()
{
	if(m_hbitMain)
		DeleteObject(m_hbitMain);

	if(m_hbitGray)
		DeleteObject(m_hbitGray);
}

void CODMenu::ResetText(UINT uNewText)
{
	m_idText = uNewText;
	LoadString(m_hInstRC, m_idText, m_szText, _MAX_PATH);

	//figure out mnemonic
	unsigned char* pAmp = _mbschr((unsigned char*)m_szText, '&');
	ASSERT(pAmp, "No mnemonic in menu!");

	m_chMenu = *(++pAmp);
}


void CODMenu::AppendMenu(HMENU hmenu, BOOL bPopup, HMENU hmenuSub)
{
	::AppendMenu(hmenu, MF_OWNERDRAW | (bPopup ? MF_POPUP : 0), bPopup ? (UINT)hmenuSub : m_idMenu, (LPCTSTR)this);
}

void CODMenu::OnMeasureItem(LPMEASUREITEMSTRUCT pmis)
{
	pmis->itemWidth = m_nMenuWidth;
	pmis->itemHeight = m_nMenuHeight;
}

void CODMenu::OnDrawItem( LPDRAWITEMSTRUCT pdis )
{
	
	HDC hdcTemp, hdcPaint;
	RECT rcDest = pdis->rcItem;
	RECT rcSrc = rcDest;
	RECT rcText, rcBit;
	HBITMAP hbitSrc, hbitPrev;
	HFONT hfnt, hfntPrev;
	OffsetRect(&rcSrc, -rcDest.left, -rcDest.top);
	rcText = rcSrc;
	rcText.left += (rcText.bottom - rcText.top) + (m_nspace * 3);
	rcText.right -= (m_nspace * 8);
	memset(&rcBit, 0, sizeof(RECT));
	rcBit.right = rcSrc.bottom - rcSrc.top;
	rcBit.bottom = rcSrc.bottom - rcSrc.top;

	// this is the DC of the screen.
	hdcPaint = pdis->hDC;
	//hdcTemp is an im memory DC.
	// we'll do all the work on it first.
	hdcTemp = CreateCompatibleDC(hdcPaint);
	hbitSrc = CreateCompatibleBitmap(hdcPaint, rcSrc.right - rcSrc.left, rcSrc.bottom - rcSrc.top);
	hfnt = (HFONT)GetCurrentObject(hdcPaint, OBJ_FONT);
	hbitPrev = (HBITMAP)SelectObject(hdcTemp, hbitSrc);
	hfntPrev = (HFONT)SelectObject(hdcTemp, hfnt);

	//first fill the rect with the right color, based on system settings
	if(pdis->itemAction & ODA_DRAWENTIRE | pdis->itemAction & ODA_SELECT)
	{
		HBRUSH hbrsh;
		LOGBRUSH lbrsh;
		lbrsh.lbStyle = BS_SOLID;
		lbrsh.lbHatch = 0;
		if(pdis->itemState & ODS_SELECTED)
			lbrsh.lbColor = GetSysColor(COLOR_HIGHLIGHT);
		else
			lbrsh.lbColor = GetSysColor(COLOR_MENU);
		hbrsh = CreateBrushIndirect(&lbrsh);
		FillRect(hdcTemp, &rcSrc, hbrsh);
		DeleteObject(hbrsh);
	}

	if(m_hbitMain)
	{
		// now draw the little bitmap
		HDC hdcMask, hdcSrc, hdcBit;
		HBITMAP hbitMask, hbitBit, hbitBitPrev, hbitMaskPrev, hbitSrcPrev;
		HBRUSH hbrshBit;
		LOGBRUSH lbrsh;
		lbrsh.lbStyle = BS_SOLID;
		lbrsh.lbHatch = 0;
		lbrsh.lbColor = GetSysColor(COLOR_MENU);
		hbrshBit = CreateBrushIndirect(&lbrsh);

		// the final version of the bitmap after masking
		hdcBit = CreateCompatibleDC(hdcPaint);
		// the original bitmap
		hdcSrc = CreateCompatibleDC(hdcPaint);
		// the mask to make the background "tranparent"
		hdcMask = CreateCompatibleDC(hdcPaint);
		// the mask bitmap is monochrome
		hbitMask = CreateBitmap(16,16, 1, 1, NULL);
		hbitBit = CreateCompatibleBitmap(hdcPaint, 20, 20);

		hbitBitPrev = (HBITMAP)SelectObject(hdcBit, hbitBit);
		hbitSrcPrev = (HBITMAP)SelectObject(hdcSrc, m_hbitMain);
		hbitMaskPrev = (HBITMAP)SelectObject(hdcMask, hbitMask);

		// set the backround color to the "tranparent" color, to create the mask
		SetBkColor(hdcSrc, RGB(0, 128, 128));
		// now the mask masks out all "transparent" bits
		BitBlt(hdcMask, 0, 0, 20, 20, hdcSrc, 0, 0, SRCCOPY);

		// set the Background color back to what it was
		SetBkColor(hdcSrc, RGB(255,255,255));

		// now we're ready to draw the bitmap.
		// We want the background to always be the color of the menu,
		// so fill the rect with that first.
		RECT rc;
		rc.left = 0; rc.top = 0; rc.right = 20; rc.bottom = 20;
		FillRect(hdcBit, &rc, hbrshBit);

		// These three operations combine the src and the mask 
		// with the background so the "transparent" color is gone.
		BitBlt(hdcBit, 2, 2, 18, 18, hdcSrc, 0, 0, SRCINVERT);
		BitBlt(hdcBit, 2, 2, 18, 18, hdcMask, 0, 0, SRCAND);
		BitBlt(hdcBit, 2, 2, 18, 18, hdcSrc, 0, 0, SRCINVERT);

		// Now blt the masked bitmap into the proper rectangle on the
		// in memory DC.
		StretchBlt(hdcTemp, rcBit.left, rcBit.top, rcBit.right - rcBit.left,
			rcBit.bottom - rcBit.top, hdcBit, 0, 0, 20, 20, SRCCOPY);

		if(pdis->itemState & ODS_SELECTED && !(pdis->itemState & ODS_CHECKED))
		{
			// We want to draw an edge around the bitmap, but
			// DrawEdge doesn't use the right colors,
			// so we'll draw it ourselves.
			HPEN hpenT, hpenL, hpenB, hpenR, hpenPrev;
			hpenT = CreatePen(PS_SOLID, GetSystemMetrics(SM_CYEDGE) / 2, GetSysColor( COLOR_3DHILIGHT ));
			hpenL = CreatePen(PS_SOLID, GetSystemMetrics(SM_CXEDGE) / 2, GetSysColor( COLOR_3DHILIGHT ));
			hpenB = CreatePen(PS_SOLID, GetSystemMetrics(SM_CYEDGE) / 2, GetSysColor( COLOR_3DSHADOW ));
			hpenR = CreatePen(PS_SOLID, GetSystemMetrics(SM_CXEDGE) / 2, GetSysColor( COLOR_3DSHADOW ));
			hpenPrev = (HPEN)SelectObject(hdcTemp, hpenT);
			MoveToEx(hdcTemp, rcBit.right - m_nspace, rcBit.top + m_nspace, NULL);
			LineTo(hdcTemp, rcBit.left + m_nspace, rcBit.top + m_nspace);
			SelectObject(hdcTemp, hpenL);
			LineTo(hdcTemp, rcBit.left + m_nspace, rcBit.bottom - m_nspace);
			SelectObject(hdcTemp, hpenB);
			LineTo(hdcTemp, rcBit.right - m_nspace, rcBit.bottom - m_nspace);
			SelectObject(hdcTemp, hpenR);
			LineTo(hdcTemp, rcBit.right - m_nspace, rcBit.top + m_nspace);
			SelectObject(hdcTemp, hpenPrev);
			DeleteObject(hpenT);
			DeleteObject(hpenL);
			DeleteObject(hpenB);
			DeleteObject(hpenR);
		}

		if(pdis->itemState & ODS_CHECKED)
		{
			//draw the edge the other way
			HPEN hpenT, hpenL, hpenB, hpenR, hpenPrev;
			hpenT = CreatePen(PS_SOLID, GetSystemMetrics(SM_CYEDGE) / 2, GetSysColor( COLOR_3DSHADOW ));
			hpenL = CreatePen(PS_SOLID, GetSystemMetrics(SM_CXEDGE) / 2, GetSysColor( COLOR_3DSHADOW ));
			hpenB = CreatePen(PS_SOLID, GetSystemMetrics(SM_CYEDGE) / 2, GetSysColor( COLOR_3DHILIGHT ));
			hpenR = CreatePen(PS_SOLID, GetSystemMetrics(SM_CXEDGE) / 2, GetSysColor( COLOR_3DHILIGHT ));
			hpenPrev = (HPEN)SelectObject(hdcTemp, hpenT);
			MoveToEx(hdcTemp, rcBit.right - m_nspace, rcBit.top + m_nspace, NULL);
			LineTo(hdcTemp, rcBit.left + m_nspace, rcBit.top + m_nspace);
			SelectObject(hdcTemp, hpenL);
			LineTo(hdcTemp, rcBit.left + m_nspace, rcBit.bottom - m_nspace);
			SelectObject(hdcTemp, hpenB);
			LineTo(hdcTemp, rcBit.right - m_nspace, rcBit.bottom - m_nspace);
			SelectObject(hdcTemp, hpenR);
			LineTo(hdcTemp, rcBit.right - m_nspace, rcBit.top + m_nspace);
			SelectObject(hdcTemp, hpenPrev);
			DeleteObject(hpenT);
			DeleteObject(hpenL);
			DeleteObject(hpenB);
			DeleteObject(hpenR);
		}


		SelectObject(hdcBit, hbitBitPrev);
		SelectObject(hdcSrc, hbitSrcPrev);
		SelectObject(hdcMask, hbitMaskPrev);

		DeleteObject(hbitMask);
		DeleteObject(hbitBit);
		DeleteObject(hbrshBit);

		DeleteDC(hdcBit);
		DeleteDC(hdcSrc);
		DeleteDC(hdcMask);
	}
	
	// now we will draw the text of the menuitem
	// first set up the foreground and background colors,
	// based on the system settings.
	if(pdis->itemState & ODS_SELECTED)
	{
		SetBkColor(hdcTemp, GetSysColor(COLOR_HIGHLIGHT));
		SetTextColor(hdcTemp, GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	else
	{
		SetBkColor(hdcTemp, GetSysColor(COLOR_MENU));
		SetTextColor(hdcTemp, GetSysColor(COLOR_MENUTEXT));
	}

	// In order to make the Accelerator directives line up corectly
	// split the string at the tab to left justify the main text,
	// and right justify the accelerators.
	unsigned char* pTab = NULL;
	char* pleft, *pright;

	pleft = new char[strlen(m_szText) + 1];
	strcpy(pleft, m_szText);
	pTab = _mbschr((unsigned char*)pleft, '\t');
	if(pTab)
	{
		pright = (char*)(pTab + 1);
		*pTab = '\0';
	}

	//use DrawText instead of TextOut because it understands mnemonics.
	DrawText(hdcTemp, pleft, -1, &rcText, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
	if(pTab)
		DrawText(hdcTemp, pright, -1, &rcText, DT_VCENTER | DT_SINGLELINE | DT_RIGHT );

	delete [] pleft;

	// finally paint the whole thing to the screen.
	BitBlt(hdcPaint, rcDest.left, rcDest.top, rcDest.right - rcDest.left, rcDest.bottom - rcDest.top, hdcTemp, 0, 0, SRCCOPY);

	SelectObject(hdcTemp, hfntPrev);
	SelectObject(hdcTemp, hbitPrev);

	
	DeleteObject(hbitSrc);
	DeleteDC(hdcTemp);

}

CTLODMenu::CTLODMenu(HINSTANCE hInstRC, UINT idMenu, UINT idText) : CODMenu(hInstRC, idMenu, idText)
{
	m_stCurrent = stNormal;
	//now re-figure out how big everything should be
	HDC hdcTemp;
	HFONT hfntPrev;
	RECT rcText;
	memset(&rcText, 0, sizeof(RECT));
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
	m_hfntMenu = CreateFontIndirect(&(ncm.lfMenuFont));
	hdcTemp = CreateCompatibleDC(NULL);
	hfntPrev = (HFONT)SelectObject(hdcTemp, m_hfntMenu);

	DrawText(hdcTemp, m_szText, -1, &rcText, DT_CALCRECT | DT_EXPANDTABS );

	m_nspace = GetSystemMetrics(SM_CYBORDER);
	m_nMenuHeight = GetSystemMetrics(SM_CYMENU) + m_nspace * 2;
	m_nMenuWidth = (GetSystemMetrics(SM_CXBORDER) * 2) + (rcText.right - rcText.left);

	SelectObject(hdcTemp, hfntPrev);
	DeleteDC(hdcTemp);
}

CTLODMenu::~CTLODMenu()
{
	DeleteObject(m_hfntMenu);
}

void CTLODMenu::OnDrawItem( LPDRAWITEMSTRUCT pdis )
{
	
	HDC hdcTemp, hdcPaint;
	RECT rcDest = pdis->rcItem;
	RECT rcSrc = rcDest;
	RECT rcText;
	HBITMAP hbitSrc, hbitPrev;
	HFONT hfntTemp, hfntPrev, hfntPrevTemp;
	OffsetRect(&rcSrc, -rcDest.left, -rcDest.top);
	rcText = rcSrc;
	rcText.left += (m_nspace * 4);
	rcText.right -= (m_nspace * 4);
		// this is the DC of the screen.
	hdcPaint = pdis->hDC;
	hfntPrev = (HFONT)SelectObject(hdcPaint, m_hfntMenu);
	//hdcTemp is an im memory DC.
	// we'll do all the work on it first.
	hdcTemp = CreateCompatibleDC(hdcPaint);
	hbitSrc = CreateCompatibleBitmap(hdcPaint, rcSrc.right - rcSrc.left, rcSrc.bottom - rcSrc.top);
	hfntTemp = (HFONT)GetCurrentObject(hdcPaint, OBJ_FONT);
	hfntPrevTemp = (HFONT)SelectObject(hdcTemp, hfntTemp);
	hbitPrev = (HBITMAP)SelectObject(hdcTemp, hbitSrc);
	

	//first fill the rect with the right color, based on system settings
	if(pdis->itemAction & ODA_DRAWENTIRE | pdis->itemAction & ODA_SELECT)
	{
		HBRUSH hbrsh;
		LOGBRUSH lbrsh;
		lbrsh.lbStyle = BS_SOLID;
		lbrsh.lbHatch = 0;
		lbrsh.lbColor = GetSysColor(COLOR_MENU);
		hbrsh = CreateBrushIndirect(&lbrsh);
		FillRect(hdcTemp, &rcSrc, hbrsh);
		DeleteObject(hbrsh);
	}

	if(pdis->itemState & ODS_SELECTED)
	{
		//draw the edge the other way
		HPEN hpenT, hpenL, hpenB, hpenR, hpenPrev;
		hpenT = CreatePen(PS_SOLID, GetSystemMetrics(SM_CYEDGE) / 2, GetSysColor( COLOR_3DSHADOW ));
		hpenL = CreatePen(PS_SOLID, GetSystemMetrics(SM_CXEDGE) / 2, GetSysColor( COLOR_3DSHADOW ));
		hpenB = CreatePen(PS_SOLID, GetSystemMetrics(SM_CYEDGE) / 2, GetSysColor( COLOR_3DHILIGHT ));
		hpenR = CreatePen(PS_SOLID, GetSystemMetrics(SM_CXEDGE) / 2, GetSysColor( COLOR_3DHILIGHT ));
		hpenPrev = (HPEN)SelectObject(hdcTemp, hpenT);
		MoveToEx(hdcTemp, rcSrc.right - m_nspace, rcSrc.top + m_nspace, NULL);
		LineTo(hdcTemp, rcSrc.left + m_nspace, rcSrc.top + m_nspace);
		SelectObject(hdcTemp, hpenL);
		LineTo(hdcTemp, rcSrc.left + m_nspace, rcSrc.bottom - m_nspace);
		SelectObject(hdcTemp, hpenB);
		LineTo(hdcTemp, rcSrc.right - m_nspace, rcSrc.bottom - m_nspace);
		SelectObject(hdcTemp, hpenR);
		LineTo(hdcTemp, rcSrc.right - m_nspace, rcSrc.top + m_nspace);
		SelectObject(hdcTemp, hpenPrev);
		DeleteObject(hpenT);
		DeleteObject(hpenL);
		DeleteObject(hpenB);
		DeleteObject(hpenR);
	}

	if(pdis->itemState & ODS_HOVER)
	{
		//draw the edge the other way
		HPEN hpenT, hpenL, hpenB, hpenR, hpenPrev;
		hpenT = CreatePen(PS_SOLID, GetSystemMetrics(SM_CYEDGE) / 2, GetSysColor( COLOR_3DHILIGHT ));
		hpenL = CreatePen(PS_SOLID, GetSystemMetrics(SM_CXEDGE) / 2, GetSysColor( COLOR_3DHILIGHT ));
		hpenB = CreatePen(PS_SOLID, GetSystemMetrics(SM_CYEDGE) / 2, GetSysColor( COLOR_3DSHADOW ));
		hpenR = CreatePen(PS_SOLID, GetSystemMetrics(SM_CXEDGE) / 2, GetSysColor( COLOR_3DSHADOW ));
		hpenPrev = (HPEN)SelectObject(hdcTemp, hpenT);
		MoveToEx(hdcTemp, rcSrc.right - m_nspace, rcSrc.top + m_nspace, NULL);
		LineTo(hdcTemp, rcSrc.left + m_nspace, rcSrc.top + m_nspace);
		SelectObject(hdcTemp, hpenL);
		LineTo(hdcTemp, rcSrc.left + m_nspace, rcSrc.bottom - m_nspace);
		SelectObject(hdcTemp, hpenB);
		LineTo(hdcTemp, rcSrc.right - m_nspace, rcSrc.bottom - m_nspace);
		SelectObject(hdcTemp, hpenR);
		LineTo(hdcTemp, rcSrc.right - m_nspace, rcSrc.top + m_nspace);
		SelectObject(hdcTemp, hpenPrev);
		DeleteObject(hpenT);
		DeleteObject(hpenL);
		DeleteObject(hpenB);
		DeleteObject(hpenR);
	}
	
	// now we will draw the text of the menuitem
	// first set up the foreground and background colors,
	// based on the system settings.
	SetBkColor(hdcTemp, GetSysColor(COLOR_MENU));
	SetTextColor(hdcTemp, GetSysColor(COLOR_MENUTEXT));
	
	//use DrawText instead of TextOut because it understands mnemonics.
	DrawText(hdcTemp, m_szText, -1, &rcText, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	
	// finally paint the whole thing to the screen.
	BitBlt(hdcPaint, rcDest.left, rcDest.top, rcDest.right - rcDest.left, rcDest.bottom - rcDest.top, hdcTemp, 0, 0, SRCCOPY);

	SelectObject(hdcTemp, hfntPrevTemp);
	SelectObject(hdcTemp, hbitPrev);
	SelectObject(hdcPaint, hfntPrev);

	DeleteObject(hbitSrc);
	DeleteDC(hdcTemp);

	m_stCurrent = pdis->itemState & ODS_HOVER ? stHover : pdis->itemState & ODS_SELECTED ? stSelected : stNormal;

}
