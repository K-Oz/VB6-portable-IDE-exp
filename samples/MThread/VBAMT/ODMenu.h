//---------------------------------------------------------------------------
// ODMenu.h
//---------------------------------------------------------------------------
// Generic Owner Draw menu item support

/*	Copyright (C) Microsoft Corporation, 1999.  All rights reserved.


	This source code is intended only as a supplement to Microsoft
	Development Tools and/or on-line documentation.  See these other
	materials for detailed information regarding Microsoft code samples.
*/

class CODMenu
{
public:
	CODMenu(HINSTANCE hInstRC, UINT idMenu, UINT idText, UINT idBitMain = 0, UINT idBitGray = 0);
	virtual ~CODMenu();
	virtual void AppendMenu(HMENU hmenu, BOOL bPopup = FALSE, HMENU hmenuSub = NULL);
	virtual void OnDrawItem(LPDRAWITEMSTRUCT);
	virtual void OnMeasureItem(LPMEASUREITEMSTRUCT);
	void ResetText(UINT uNewText);
	char GetMenuChar() { return m_chMenu; }

protected:
	HINSTANCE m_hInstRC;
	HBITMAP m_hbitMain;
	HBITMAP m_hbitGray;
	UINT m_idMenu, m_idText, m_idBitMain, m_idBitGray;
	UINT m_nMenuHeight, m_nMenuWidth, m_nspace;
	char m_szText[_MAX_PATH];
	char m_chMenu; // mnemonic
};

typedef enum tagTLState
{
	stNormal,
	stHover,
	stSelected
}TLState;

class CTLODMenu : public CODMenu
{
public:
	CTLODMenu(HINSTANCE hInstRC, UINT idMenu, UINT idText);
	virtual ~CTLODMenu();
	virtual void OnDrawItem(LPDRAWITEMSTRUCT);
	TLState GetCurrentState() { return m_stCurrent; }
private:
	HFONT m_hfntMenu;
	TLState m_stCurrent;
};

#define ODS_HOVER 0x0200