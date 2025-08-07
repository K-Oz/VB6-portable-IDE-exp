/*	Copyright (C) Microsoft Corporation, 1999.  All rights reserved.


	This source code is intended only as a supplement to Microsoft
	Development Tools and/or on-line documentation.  See these other
	materials for detailed information regarding Microsoft code samples.
*/

// VBAMTApp.h: interface for the CVBAMTApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VBAMTAPP_H__E4C9A869_CF6F_11D2_8068_00C04F8EC1B3__INCLUDED_)
#define AFX_VBAMTAPP_H__E4C9A869_CF6F_11D2_8068_00C04F8EC1B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
using namespace MSAPC;

class CVBAMTApp : public CApcHost<CVBAMTApp> 
{
public:
	BOOL ShowIde();
	virtual HRESULT ApcHost_OnCreate();
	virtual HRESULT ApcHost_OnTranslateMessage(MSG* pmsg, BOOL* pfConsumed);
	virtual HRESULT ApcHost_BeforePause() { m_bBreakMode = TRUE; SetButtonStates(); return NOERROR; }
	virtual HRESULT ApcHost_AfterPause() { m_bBreakMode = FALSE; SetButtonStates(); return NOERROR; }
	BOOL OnInitDialog(HWND hwnd);
	BOOL OnCommand(LPARAM cmd);
	BOOL OnMouseMove(UINT msg, LPARAM cmd);
	BOOL OnNotify(LPNMHDR pnmh);
	BOOL OnSysCommand(WPARAM wp);
	static BOOL CALLBACK MTDialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static LRESULT CALLBACK MenuProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	BOOL Start(HINSTANCE hInstance, LPSTR lpCmdLine);
	BOOL InitApc(HWND);
	void KillAllThreads();
	
	HWND m_hwnd;
	HINSTANCE m_hInst;
	CVBAMTApp();
	virtual ~CVBAMTApp();

private:
	void RegisterModule();
	BOOL m_bFirstTime;
	BOOL m_bBreakMode;
	LCID m_lcidVBA;
	BOOL m_bWinHelp;

	HACCEL m_hAccel;
	void KillListBoxThreads(UINT uID);
	HWND GetCurList();
	void SetButtonStates();
	BOOL MT1Active();
	RECT* GetListBoxRect(HWND hwnd, RECT& rc);
	HRESULT ParseCmdLine(LPSTR lpCmdLine);

	static WNDPROC s_wndproc;
};

class CProjDlg
{
public:
	CProjDlg();
	void DoModal();
	static BOOL CALLBACK ProjBoxProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );

	BOOL m_bOpen;
	BOOL m_bNew;
	BOOL m_bCancel;
	BOOL m_bOK;
};

#endif // !defined(AFX_VBAMTAPP_H__E4C9A869_CF6F_11D2_8068_00C04F8EC1B3__INCLUDED_)
