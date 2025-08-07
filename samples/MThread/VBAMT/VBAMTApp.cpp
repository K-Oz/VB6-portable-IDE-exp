/*	Copyright (C) Microsoft Corporation, 1999.  All rights reserved.


	This source code is intended only as a supplement to Microsoft
	Development Tools and/or on-line documentation.  See these other
	materials for detailed information regarding Microsoft code samples.
*/

// VBAMTApp.cpp: implementation of the CVBAMTApp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "VBAMTApp.h"
#include "vbamt.h"
#include "dwinvers.h"
#include "commctrl.h"
#include "odmenu.h"
#include <stdio.h>


extern CVBAMTApp theApp;
WNDPROC CVBAMTApp::s_wndproc = NULL;

BOOL CALLBACK AboutBoxProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
#define APC_ERROR(err) ((err)>=APC_E_INVALIDINDEX && (err)<=APC_E_LAST_ERROR)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVBAMTApp::CVBAMTApp()
{
	m_bFirstTime = FALSE;
	m_bBreakMode = FALSE;
	m_hInst = NULL;
	m_hwnd = NULL;
	m_lcidVBA = 1033; //English
	m_bWinHelp = FALSE;
}

CVBAMTApp::~CVBAMTApp()
{

}

void CVBAMTApp::RegisterModule()
{
	ITypeLib* ptlib = NULL;
	char szModule[_MAX_PATH];
	OLECHAR wszModule[_MAX_PATH];
	
	GetModuleFileName(NULL, szModule, _MAX_PATH);
	MultiByteToWideChar(CP_ACP, 0, szModule, -1, wszModule, _MAX_PATH);

	if(SUCCEEDED(LoadTypeLib(wszModule, &ptlib)))
	{
		RegisterTypeLib(ptlib, wszModule, NULL);
		ptlib->Release();
	}
}


//---------------------------------------------------------------------------
// Helper to parse command-line parameters
//---------------------------------------------------------------------------
HRESULT CVBAMTApp::ParseCmdLine
(
  LPSTR lpCmdLine
)
{
	char* pcCur, *pcCurEnd;
	pcCur = lpCmdLine;

	while(pcCur)
	{
		//find next space or EOL
		pcCurEnd = strchr(pcCur, ' ');
		if(*pcCur == '/' || *pcCur == '-')
		{
			pcCur++;

			if(pcCurEnd)
			{
				*pcCurEnd = '\0';
			}

			if(!stricmp(pcCur, "winhelp"))
			{
				m_bWinHelp = TRUE;
			}
			else
			{
				char cTemp;
				if(strlen(pcCur) == 12)
				{
					cTemp = pcCur[7];
					pcCur[7] = '\0';
					if(!stricmp(pcCur, "vbalang"))
					{
						pcCur = &pcCur[8];
						int iTemp = atoi(pcCur);
						if( iTemp > 1000 )
						  m_lcidVBA = iTemp;
					}
					*(--pcCur) = cTemp;
				}
			}
		
		}
		pcCur = pcCurEnd ? pcCurEnd + 1 : NULL;
	}

	return S_OK;
}

BOOL CVBAMTApp::Start(HINSTANCE hInstance, LPSTR lpCmdLine)
{
	m_hInst = hInstance;

	ParseCmdLine(lpCmdLine);

	RegisterModule();

	m_hAccel = LoadAccelerators(m_hInst, MAKEINTRESOURCE(IDR_MAINMENU));

	m_hwnd = CreateDialog( hInstance, MAKEINTRESOURCE( IDD_MTDIALOG ), NULL, ( DLGPROC )MTDialogProc );

	if(!m_hwnd) DWORD err = GetLastError();
	return m_hwnd != NULL;
}

BOOL CVBAMTApp::InitApc(HWND hwnd)
{
	BSTR bstrLicKey = SysAllocString(wszEvalLicKey);
	if(SUCCEEDED(ApcHost.Create(hwnd, L"VBAMT", NULL, bstrLicKey,m_lcidVBA)))
	{
		m_bFirstTime = TRUE;
		SysFreeString(bstrLicKey);
		return TRUE;
	}
	else
	{
		MessageBox(m_hwnd, "Error Initializing VBA", "VBAMT", MB_OK | MB_ICONEXCLAMATION);
	}
	
	SysFreeString(bstrLicKey);
	return FALSE;
}

HRESULT CVBAMTApp::ApcHost_OnCreate()
{
	PICTDESC pd;
	IPictureDisp* pPictDisp;

	HBITMAP hbit = LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_APP));
	pd.cbSizeofstruct = sizeof(PICTDESC);
	pd.picType = PICTYPE_BITMAP;
	pd.bmp.hbitmap = hbit;
	pd.bmp.hpal = NULL;

	if(SUCCEEDED(OleCreatePictureIndirect(&pd, IID_IPictureDisp, TRUE, (LPVOID*)&pPictDisp)))
	{
		ApcHost->APC_PUT(Picture)(pPictDisp);
		ApcHost->APC_PUT(BackColor)((::OLE_COLOR)RGB(0, 128, 128));
		pPictDisp->Release();
	}

	ApcHost->AddMTProjectType(L"VBAMT1", L"HKEY_CURRENT_USER\\Software\\Microsoft\\VBAMT\\MT1Dlls",
		L"{a8c1b5d2-b316-11d1-96e9-00a0c91110eb}", L"{a8c1b5d0-b316-11d1-96e9-00a0c91110eb}",1, 0, FALSE);

	ApcHost->AddMTProjectType(L"VBAMT2", L"HKEY_CURRENT_USER\\Software\\Microsoft\\VBAMT\\MT2Dlls",
		L"{8b4b6351-b318-11d1-96e9-00a0c91110eb}", L"{a8c1b5d0-b316-11d1-96e9-00a0c91110eb}",1, 0, FALSE);

	ApcHost->APC_PUT(HelpStyle)(m_bWinHelp ? axHelpWindows : axHelpHTML);

	return CApcHost<CVBAMTApp>::ApcHost_OnCreate();
}

BOOL CVBAMTApp::ShowIde()
{
HRESULT hr;

	if( theApp.ApcHost || theApp.InitApc(m_hwnd) )
	{
		IApcIde* pIde;
		hr = theApp.ApcHost->get_Ide(&pIde);
		if(SUCCEEDED(hr))
		{
			using namespace VBIDE;
			using namespace Office;
			VBE* pVBE = NULL;
			_VBProjects* pProjs = NULL;
			_VBProject* pProj = NULL;
			_VBComponents* pComps = NULL;
			_VBComponent* pComp = NULL;
			CommandBars* pCmdBars = NULL;
			CommandBarControl* pCtrl = NULL;
			if(m_bFirstTime )
			{
				CProjDlg dlg;
				dlg.DoModal();
				pIde->put_Visible(VARIANT_TRUE);
				pIde->Release();
				if(dlg.m_bOK)
				{
					if(dlg.m_bNew)
					{
						if(SUCCEEDED(ApcHost->get_VBE(&pVBE)))
						{
							if(SUCCEEDED(pVBE->get_VBProjects((VBProjects**)&pProjs)))
							{
								if(SUCCEEDED(pProjs->Add(vbext_pt_StandAlone, (VBProject**)&pProj)))
								{
									if(SUCCEEDED(pProj->get_VBComponents((VBComponents**)&pComps)))
									{
										if(SUCCEEDED(pComps->AddCustom(L"VBAMT.Designer.1", (VBComponent**)&pComp)))
										{
											pComp->Release();
										}
										pComps->Release();
									}
									pProj->Release();
								}
								pProjs->Release();
							}
							pVBE->Release();
						}
					}
					if(dlg.m_bOpen)
					{
						if(SUCCEEDED(ApcHost->get_VBE(&pVBE)))
						{
							if(SUCCEEDED(pVBE->get_CommandBars(&pCmdBars)))
							{
								VARIANT varType, varID, varTag, varVisible;
								VariantInit(&varType);
								VariantInit(&varID);
								VariantInit(&varTag);
								VariantInit(&varVisible);

								varType.vt = VT_ERROR;
								varID.vt = VT_I4;
								varID.lVal = 23; // ID for Open Project
								varTag.vt = VT_ERROR;
								varVisible.vt = VT_ERROR;
								if(SUCCEEDED(pCmdBars->FindControl(varType, varID, varTag, varVisible, &pCtrl)))
								{
									pCtrl->Execute();
									pCtrl->Release();
								}
								pCmdBars->Release();
							}
							pVBE->Release();
						}
					}
				}
				m_bFirstTime = FALSE;
			}
			else
			{
				pIde->put_Visible(VARIANT_TRUE);
				pIde->Release();
			}
		}
		else if( APC_ERROR(hr) )
		{
			MessageBox(m_hwnd, "VBA initialization failed.", "VBAMT", MB_OK | MB_ICONEXCLAMATION);
	  	}


	}
	return TRUE;
}


void CVBAMTApp::KillAllThreads()
{
	KillListBoxThreads(IDC_MT1LIST);
	KillListBoxThreads(IDC_MT2LIST);
}

void CVBAMTApp::KillListBoxThreads(UINT uID)
{
	HWND hwndList = GetDlgItem( m_hwnd, uID );
	UINT uCount = SendMessage(hwndList, LB_GETCOUNT, 0, 0);
	if(uCount != LB_ERR)
	{
		for(UINT i = 0; i < uCount; i++)
		{
			UINT item = SendMessage(hwndList, LB_GETITEMDATA, ( WPARAM )0, 0 );
			if (item != LB_ERR )
			{
				CVbaMt1* pMT = (CVbaMt1*)item;
				SendMessage(hwndList, LB_SETITEMDATA, ( WPARAM )0, 0 );
				
				SendMessage(hwndList, LB_DELETESTRING, ( WPARAM )0, 0 );
				
				HANDLE hThread = pMT->Destroy();
				
				// the following loop is necessary to allow RPC calls through while debugging
				// as the debugged objects are running on this thread.
				DWORD dwRet = MsgWaitForMultipleObjects(1, &hThread, FALSE, INFINITE, QS_ALLEVENTS);
				while( dwRet == WAIT_OBJECT_0 + 1)
				{
					MSG msg;
					PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
					if ( !TranslateMessage( &msg ))
						DispatchMessage( &msg );

					dwRet = MsgWaitForMultipleObjects(1, &hThread, FALSE, INFINITE, QS_ALLEVENTS);
				}
				CloseHandle(hThread);
			}
		}
	}
}

HWND CVBAMTApp::GetCurList()
{
	TC_ITEM tci;
	tci.mask = TCIF_PARAM;

	int tab = TabCtrl_GetCurSel(GetDlgItem(m_hwnd, IDC_TAB));
	TabCtrl_GetItem(GetDlgItem(m_hwnd, IDC_TAB), tab, &tci);

	return (HWND)tci.lParam;
}

BOOL CVBAMTApp::MT1Active()
{
	return (TabCtrl_GetCurSel(GetDlgItem(m_hwnd, IDC_TAB)) == 0);
}

RECT* CVBAMTApp::GetListBoxRect(HWND hwnd, RECT& rc)
{
	GetWindowRect(hwnd, &rc);
	ScreenToClient(m_hwnd, (POINT*)&rc);
	ScreenToClient(m_hwnd, ((POINT*)&rc) + 1);
	return &rc;
}


void CVBAMTApp::SetButtonStates()
{
	HWND hwndList = GetCurList();
	int sel = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
	HWND hwndCreate = GetDlgItem(m_hwnd, IDC_MT1CREATE);
	HWND hwndResume = GetDlgItem(m_hwnd, IDC_MT1START);
	HWND hwndSuspend = GetDlgItem(m_hwnd, IDC_MT1STOP);
	HWND hwndDestroy = GetDlgItem(m_hwnd, IDC_MT1DESTROY);

	if( sel == LB_ERR )
	{
		// no selection
		EnableWindow(hwndCreate, TRUE);
		EnableWindow(hwndResume, FALSE);
		EnableWindow(hwndSuspend, FALSE);
		EnableWindow(hwndDestroy, FALSE);
	}
	else
	{
		CVbaMt* pMT = (CVbaMt*)SendMessage( hwndList, LB_GETITEMDATA, ( WPARAM )sel, 0 );
		EnableWindow(hwndCreate, TRUE);
		EnableWindow(hwndResume, !pMT->IsStarted());
		EnableWindow(hwndSuspend, pMT->IsStarted());
		EnableWindow(hwndDestroy, m_bBreakMode ? FALSE : TRUE);
	}

	SetFocus(hwndList);
}

BOOL CVBAMTApp::OnInitDialog(HWND hwnd)
{
	// set up the tab control
	TC_ITEM tci1, tci2;
	HWND hwndCreate = GetDlgItem(hwnd, IDC_MT1CREATE);
	HWND hwndResume = GetDlgItem(hwnd, IDC_MT1START);
	HWND hwndSuspend = GetDlgItem(hwnd, IDC_MT1STOP);
	HWND hwndDestroy = GetDlgItem(hwnd, IDC_MT1DESTROY);
	char szTab1[40], szTab2[40];

	LoadString(m_hInst, IDS_TAB1, szTab1, 40);
	LoadString(m_hInst, IDS_TAB2, szTab2, 40);
	tci1.mask = TCIF_TEXT | TCIF_PARAM;
	tci1.pszText = szTab1;
	tci1.lParam = (LPARAM)GetDlgItem(hwnd, IDC_MT1LIST);
	tci2.mask = TCIF_TEXT | TCIF_PARAM;
	tci2.pszText = szTab2;
	tci2.lParam = (LPARAM)GetDlgItem(hwnd, IDC_MT2LIST);

	TabCtrl_InsertItem(GetDlgItem(hwnd, IDC_TAB), 0, &tci1);
	TabCtrl_InsertItem(GetDlgItem(hwnd, IDC_TAB), 1, &tci2);

	//setup the menu items
	HMENU hmenuMain = CreateMenu();
	HMENU hmenuFile = CreatePopupMenu();
	HMENU hmenuTools = CreatePopupMenu();
	HMENU hmenuHelp = CreatePopupMenu();
	CODMenu* pmExit, *pmVBE, *pmAbout;
	CTLODMenu* pmFile, *pmTools, *pmHelp;

	pmExit = new CODMenu(m_hInst, ID_FILE_EXIT, IDS_EXIT);
	pmExit->AppendMenu(hmenuFile);

	pmVBE = new CODMenu(m_hInst, ID_TOOLS_VBE, IDS_VBE, IDB_VBE);
	pmVBE->AppendMenu(hmenuTools);

	pmAbout = new CODMenu(m_hInst, ID_HELP_ABOUT, IDS_ABOUT, IDB_APP);
	pmAbout->AppendMenu(hmenuHelp);

	pmFile = new CTLODMenu(m_hInst, 0, IDS_FILE);
	pmFile->AppendMenu(hmenuMain, TRUE, hmenuFile);

	pmTools = new CTLODMenu(m_hInst, 0, IDS_TOOLS);
	pmTools->AppendMenu(hmenuMain, TRUE, hmenuTools);

	pmHelp = new CTLODMenu(m_hInst, 0, IDS_HELP);
	pmHelp->AppendMenu(hmenuMain, TRUE, hmenuHelp);

	SetMenu(hwnd, hmenuMain);

	EnableWindow(hwndCreate, TRUE);
	EnableWindow(hwndResume, FALSE);
	EnableWindow(hwndSuspend, FALSE);
	EnableWindow(hwndDestroy, FALSE);

	//Set the window icons
	SendMessage(hwnd, WM_SETICON, (LPARAM)ICON_BIG, (WPARAM)LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_APP)));
	SendMessage(hwnd, WM_SETICON, (LPARAM)ICON_SMALL, (WPARAM)LoadImage(m_hInst, MAKEINTRESOURCE(IDI_APP), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR));

	// need to subclass the window to handle WM_MENUCHAR 'cause we are a Dlg Box
	s_wndproc = (WNDPROC)SetWindowLong(hwnd, GWL_WNDPROC, (LONG)MenuProc);

	return TRUE;
}

BOOL CVBAMTApp::OnCommand(LPARAM cmd)
{
	switch( LOWORD(cmd) )
	{
	case IDC_MT1CREATE:
		{
			char szThreadID[ 12 ];
			CVbaMt* pMT;
			if( MT1Active() )
			{
				pMT = CVbaMt1::Create();
			}
			else
			{
				pMT = CVbaMt2::Create();
			}

			if(!pMT)
			{
				MessageBox(m_hwnd, "Thread creation failed.", "VBAMT", MB_OK | MB_ICONSTOP);
				return TRUE;
			}
			pMT->Start();
			
			wsprintf( szThreadID, "%#x", pMT->GetThreadID());
			HWND hwndMTList = GetCurList();
			UINT idx = SendMessage( hwndMTList, LB_ADDSTRING, 0, ( LPARAM )szThreadID );
			SendMessage( hwndMTList, LB_SETITEMDATA, ( WPARAM )idx, ( LPARAM )pMT );
			SendMessage( hwndMTList, LB_SETCURSEL, (WPARAM)idx, 0);
			SetButtonStates();
			RECT rc;
			InvalidateRect(m_hwnd, GetListBoxRect(hwndMTList, rc), FALSE);
		
			return TRUE;
		}
	case IDC_MT1STOP:
		{
			HWND hwndMTList = GetCurList();
			UINT idx = SendMessage( hwndMTList, LB_GETCURSEL, 0, 0 );
			if ( idx != LB_ERR )
			{
				UINT item = SendMessage( hwndMTList, LB_GETITEMDATA, ( WPARAM )idx, 0 );
				if ( item != LB_ERR )
				{
					CVbaMt* pMT = ( CVbaMt* )item;
					pMT->Stop();
				}
			}
			SetButtonStates();
			return TRUE;
		}
	case IDC_MT1START:
		{
			HWND hwndMTList = GetCurList();
			UINT idx = SendMessage( hwndMTList, LB_GETCURSEL, 0, 0 );
			if ( idx != LB_ERR )
			{
				UINT item = SendMessage( hwndMTList, LB_GETITEMDATA, ( WPARAM )idx, 0 );
				if ( item != LB_ERR )
				{
					CVbaMt* pMT = ( CVbaMt* )item;
					pMT->Start();
				}
			}
			SetButtonStates();
			return TRUE;
		}
	case IDC_MT1DESTROY:
		{
			HWND hwndMTList = GetCurList();
			UINT idx = SendMessage( hwndMTList, LB_GETCURSEL, 0, 0 );
			if ( idx != LB_ERR )
			{
				UINT item = SendMessage(hwndMTList, LB_GETITEMDATA, ( WPARAM )idx, 0 );
				if (item != LB_ERR )
				{
					CVbaMt* pMT = (CVbaMt*)item;
					SendMessage(hwndMTList, LB_SETITEMDATA, ( WPARAM )idx, 0 );
					
					SendMessage(hwndMTList, LB_DELETESTRING, ( WPARAM )idx, 0 );
					UINT count = SendMessage(hwndMTList, LB_GETCOUNT, 0, 0);
					SendMessage(hwndMTList, LB_SETCURSEL, (WPARAM)	idx >= count ? count - 1 : idx, 0);
					SetButtonStates();
					
					HANDLE hThread = pMT->Destroy();

					// the following loop is necessary to allow RPC calls through while debugging
					// as the debugged objects are running on this thread.
					DWORD dwRet = MsgWaitForMultipleObjects(1, &hThread, FALSE, INFINITE, QS_ALLEVENTS);
					while( dwRet == WAIT_OBJECT_0 + 1)
					{
						MSG msg;
						PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
						if ( !TranslateMessage( &msg ))
							DispatchMessage( &msg );

						dwRet = MsgWaitForMultipleObjects(1, &hThread, FALSE, INFINITE, QS_ALLEVENTS);
					}
					CloseHandle(hThread);
					RECT rc;
					InvalidateRect(m_hwnd, GetListBoxRect(hwndMTList, rc), FALSE);
				}
			}
			return TRUE;
		}
	case IDC_MT1LIST:
	case IDC_MT2LIST:
		{
			if(HIWORD(cmd) == LBN_SELCHANGE)
			{
				SetButtonStates();
				return TRUE;
			}
			break;
		}
	case ID_TOGGLE_TAB:
		{
			int curtab = TabCtrl_GetCurSel(GetDlgItem(m_hwnd, IDC_TAB));
			HWND hwndList = GetCurList();
			SetWindowLong(hwndList, GWL_STYLE, GetWindowLong(hwndList, GWL_STYLE) & ~WS_VISIBLE);
			TabCtrl_SetCurSel(GetDlgItem(m_hwnd, IDC_TAB), curtab ? 0 : 1);
			hwndList = GetCurList();
			SetWindowLong(hwndList, GWL_STYLE, GetWindowLong(hwndList, GWL_STYLE) | WS_VISIBLE);
			RECT rc;
			InvalidateRect(m_hwnd, GetListBoxRect(hwndList, rc), FALSE);
			SetButtonStates();
			return TRUE;
		}
	case ID_MT1:
		{
			// accelerator to switch tabs
			TabCtrl_SetCurSel(GetDlgItem(m_hwnd, IDC_TAB), 0);
			SetWindowLong(GetDlgItem(m_hwnd, IDC_MT2LIST), GWL_STYLE, GetWindowLong(GetDlgItem(m_hwnd, IDC_MT2LIST), GWL_STYLE) & ~WS_VISIBLE);
			SetWindowLong(GetDlgItem(m_hwnd, IDC_MT1LIST), GWL_STYLE, GetWindowLong(GetDlgItem(m_hwnd, IDC_MT1LIST), GWL_STYLE) | WS_VISIBLE);
			RECT rc;
			InvalidateRect(m_hwnd, GetListBoxRect(GetCurList(), rc), FALSE);
			SetButtonStates();
			return TRUE;
		}
	case ID_MT2:
		{
			// accelerator to switch tabs
			TabCtrl_SetCurSel(GetDlgItem(m_hwnd, IDC_TAB), 1);
			SetWindowLong(GetDlgItem(m_hwnd, IDC_MT1LIST), GWL_STYLE, GetWindowLong(GetDlgItem(m_hwnd, IDC_MT1LIST), GWL_STYLE) & ~WS_VISIBLE);
			SetWindowLong(GetDlgItem(m_hwnd, IDC_MT2LIST), GWL_STYLE, GetWindowLong(GetDlgItem(m_hwnd, IDC_MT2LIST), GWL_STYLE) | WS_VISIBLE);
			RECT rc;
			InvalidateRect(m_hwnd, GetListBoxRect(GetCurList(), rc), FALSE);
			SetButtonStates();
			return TRUE;
		}
	case ID_FILE_EXIT:
		{
			VARIANT_BOOL bTerm = VARIANT_FALSE;
			BOOL bEnd = TRUE;
			
			KillAllThreads();
			if(ApcHost)
			{
				if( SUCCEEDED(ApcHost->CanTerminate(&bTerm)) && 
				    bTerm == VARIANT_FALSE )
					return TRUE;
				
				ApcHost.WmClose(bEnd);
				if(bEnd == FALSE)
				{
					return TRUE;
				}
			}
			
			if(ApcHost)
			{
				ApcHost.Destroy();
			}
			DestroyWindow(m_hwnd);
			PostQuitMessage(0);
			return TRUE;
		}
	case ID_TOOLS_VBE:
		{
			ShowIde();
			return TRUE;
		}
	case ID_HELP_ABOUT:
		{
			if(ApcHost)
				ApcHost->APC_RAW(BeginModalDialog)();
			DialogBox(m_hInst, MAKEINTRESOURCE(IDD_ABOUT), m_hwnd, (DLGPROC)AboutBoxProc);
			if(ApcHost)
				ApcHost->APC_RAW(EndModalDialog)();
			return TRUE;
			
		}
	default:
		return FALSE;
	}

	return FALSE;
}

BOOL CVBAMTApp::OnMouseMove(UINT msg, LPARAM lp)
{
	POINTS pnts = MAKEPOINTS(lp);
	POINT pnt;
	pnt.x = pnts.x;
	pnt.y = pnts.y;
	if( msg == WM_MOUSEMOVE )
		MapWindowPoints(m_hwnd, HWND_DESKTOP, &pnt, 1);
	int item = MenuItemFromPoint(m_hwnd, GetMenu(m_hwnd), pnt);
	UINT state = 0;
	if(item >= 0)
		state = GetMenuState(GetMenu(m_hwnd), item, MF_BYPOSITION);
	//redraw all of the items if necessary
	if(!(state & MF_HILITE))
	{
		int nCount = GetMenuItemCount(GetMenu(m_hwnd));
		for( int i = 0; i < nCount; i++)
		{
			DRAWITEMSTRUCT dis;
			memset(&dis, 0, sizeof(DRAWITEMSTRUCT));
			MENUITEMINFO mii;
			memset(&mii, 0, sizeof(MENUITEMINFO));
			RECT rcWnd, rcClient;
			GetWindowRect(m_hwnd, &rcWnd);
			GetClientRect(m_hwnd, &rcClient);

			mii.cbSize = sizeof(MENUITEMINFO);
			mii.fMask = MIIM_ID | MIIM_DATA;
			GetMenuItemInfo(GetMenu(m_hwnd), i, TRUE, &mii);

			if((item == i ? stHover : stNormal) != ((CTLODMenu*)mii.dwItemData)->GetCurrentState())
			{
				dis.CtlType = ODT_MENU;
				dis.itemID = mii.wID;
				dis.itemAction = ODA_DRAWENTIRE;
				dis.itemState = item == i ? ODS_HOVER : 0;
				dis.hwndItem = (HWND)GetMenu(m_hwnd);
				dis.hDC = GetWindowDC(m_hwnd);
				GetMenuItemRect(m_hwnd, GetMenu(m_hwnd), i, &dis.rcItem);
				MapWindowPoints(HWND_DESKTOP, m_hwnd, (POINT*)&dis.rcItem, 2);
				MapWindowPoints(m_hwnd, HWND_DESKTOP, (POINT*)&rcClient, 2);
				OffsetRect(&dis.rcItem, rcClient.left - rcWnd.left, rcClient.top - rcWnd.top);
				dis.itemData = mii.dwItemData;

				SendMessage(m_hwnd, WM_DRAWITEM, 0, (LPARAM)&dis);

				ReleaseDC(m_hwnd, dis.hDC);
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CVBAMTApp::OnNotify(LPNMHDR pnmh)
{
	switch(pnmh->code)
	{
	case TCN_SELCHANGE:
		{
			HWND hwndList = GetCurList();
			RECT rc;
			SetWindowLong(hwndList, GWL_STYLE, GetWindowLong(hwndList, GWL_STYLE) | WS_VISIBLE);
			InvalidateRect(m_hwnd, GetListBoxRect(hwndList, rc), FALSE);
			SetButtonStates();
			return TRUE;
		}
	case TCN_SELCHANGING:
		{
			HWND hwndList = GetCurList();
			SetWindowLong(hwndList, GWL_STYLE, GetWindowLong(hwndList, GWL_STYLE) & ~WS_VISIBLE);
			return TRUE;
		}
	default:
		return FALSE;
	}


}

BOOL CVBAMTApp::OnSysCommand(WPARAM wp)
{
	switch(wp)
	{
	case SC_CLOSE:
		{
			VARIANT_BOOL bTerm = VARIANT_FALSE;
			BOOL bEnd = TRUE;
			
			KillAllThreads();
			if(ApcHost)
			{
				if( SUCCEEDED(ApcHost->CanTerminate(&bTerm)) &&
				    bTerm == VARIANT_FALSE )
					return TRUE;
				
				ApcHost.WmClose(bEnd);
				if(bEnd == FALSE)
				{
					return TRUE;
				}
			}
			
			if(ApcHost)
			{
				ApcHost.Destroy();
			}
			DestroyWindow(m_hwnd);
			PostQuitMessage(0);
			return TRUE;
		}
	default:
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// abstract: The MT Test Dialog procedure
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK CVBAMTApp::MTDialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	
	switch( uMsg )
	{
	case WM_INITDIALOG:
		{
			theApp.OnInitDialog(hwndDlg);
			break;
		}
	case WM_ENABLE:
		{
			theApp.ApcHost.WmEnable(wParam);
			return FALSE;
		}
	case WM_ACTIVATE:
		{
			theApp.ApcHost.WmActivate(wParam);
			return FALSE;
		}
	case WM_COMMAND:
		{
			return theApp.OnCommand(wParam);
		}
	case WM_SYSCOMMAND:
		{
			return theApp.OnSysCommand(wParam);
		}
	case WM_NOTIFY:
		{
			return theApp.OnNotify((LPNMHDR)lParam);
		}
	case WM_DRAWITEM:
		{
			((CODMenu*)((LPDRAWITEMSTRUCT)lParam)->itemData)->OnDrawItem((LPDRAWITEMSTRUCT)lParam);
			return TRUE;
		}
	case WM_MEASUREITEM:
		{
			((CODMenu*)((LPMEASUREITEMSTRUCT)lParam)->itemData)->OnMeasureItem((LPMEASUREITEMSTRUCT)lParam);
			return TRUE;
		}
	case WM_MOUSEMOVE:
	case WM_NCMOUSEMOVE:
		{
			return theApp.OnMouseMove(uMsg, lParam);
		}
	}
	return FALSE;
}

LRESULT CALLBACK CVBAMTApp::MenuProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if(msg == WM_MENUCHAR)
	{
		HMENU hmenu = (HMENU)lp;
		int nCount = GetMenuItemCount(hmenu);
		for(int i = 0; i < nCount; i++)
		{
			MENUITEMINFO mii;
			memset(&mii, 0, sizeof(MENUITEMINFO));
			mii.cbSize = sizeof(MENUITEMINFO);
			mii.fMask = MIIM_DATA | MIIM_TYPE | MIIM_ID;
			GetMenuItemInfo(hmenu, (UINT)i, TRUE, &mii);
			if(!(mii.fType & MF_SEPARATOR))
			{
				if((char)LOWORD(wp) == (char)tolower(((CODMenu*)mii.dwItemData)->GetMenuChar()))
				{
					return MAKELRESULT(i, MNC_EXECUTE);
				}
			}
		}
	}
	return s_wndproc(hwnd, msg, wp, lp);
}

BOOL CALLBACK AboutBoxProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	//setup the version strings
	if(uMsg == WM_INITDIALOG)
	{
		BSTR bstrVBAVersion;
		char szVBAVersion[100];
		char szVersion[100];

		strcpy(szVersion, "Version 1.0.");
		strncat(szVersion, vszVersNum, sizeof(szVersion) - strlen(szVersion));

		if(theApp.ApcHost)
		{
			theApp.ApcHost->APC_GET(VBAVersion)(&bstrVBAVersion);

			_snprintf(szVBAVersion, sizeof(szVBAVersion) / sizeof(szVBAVersion[0]), "VBA Version %S", bstrVBAVersion);

			SysFreeString(bstrVBAVersion);
		}
		else
			strcpy(szVBAVersion, "VBA not started");

		SetDlgItemText(hwndDlg, IDC_ABOUTBOX_VERSION, szVersion);
		SetDlgItemText(hwndDlg, IDC_ABOUTBOX_VBAVERSION, szVBAVersion);

	}
	if(uMsg == WM_COMMAND && (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL))
	{
		EndDialog(hwndDlg, 0);
		return TRUE;
	}

	return FALSE;
}

// ApcHost_OnTranslateMessage - called by ApcHost_OnMessageLoop
HRESULT CVBAMTApp::ApcHost_OnTranslateMessage(MSG* pmsg, BOOL* pfConsumed)
{
	if(TranslateAccelerator(m_hwnd, m_hAccel, pmsg)||
		IsDialogMessage(m_hwnd, pmsg))
		*pfConsumed = TRUE;
	else
		*pfConsumed = FALSE;

	return NOERROR;
}

CProjDlg::CProjDlg()
{
	m_bOpen = FALSE;
	m_bNew = FALSE;
	m_bOK = FALSE;
	m_bCancel = FALSE;
}

void CProjDlg::DoModal()
{
	theApp.ApcHost->APC_RAW(BeginModalDialog)();
	DialogBoxParam(theApp.m_hInst, MAKEINTRESOURCE(IDD_PROJECT), theApp.m_hwnd, (DLGPROC)ProjBoxProc, (LPARAM)this);
	theApp.ApcHost->APC_RAW(EndModalDialog)();
}

BOOL CALLBACK CProjDlg::ProjBoxProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	static CProjDlg* pDlg = NULL;
	if(uMsg == WM_INITDIALOG)
	{
		pDlg = (CProjDlg*)lParam;
		SendMessage(GetDlgItem(hwndDlg, IDC_NEWPROJ), BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
	}
	if(uMsg == WM_COMMAND)
	{
		LRESULT NewProj = SendMessage(GetDlgItem(hwndDlg, IDC_NEWPROJ), BM_GETCHECK, 0, 0);
		switch (LOWORD(wParam))
		{
		case IDOK:
			pDlg->m_bOK = TRUE;
			pDlg->m_bNew = NewProj == BST_CHECKED;
			pDlg->m_bOpen = NewProj != BST_CHECKED;
			EndDialog(hwndDlg, 0);
			return TRUE;
		case IDCANCEL:
			pDlg->m_bCancel = TRUE;
			EndDialog(hwndDlg, 0);
			return TRUE;
		default:
			return FALSE;
		}
	}
	return FALSE;
}