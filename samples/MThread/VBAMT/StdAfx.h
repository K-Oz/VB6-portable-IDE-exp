/*	Copyright (C) Microsoft Corporation, 1999.  All rights reserved.


	This source code is intended only as a supplement to Microsoft
	Development Tools and/or on-line documentation.  See these other
	materials for detailed information regarding Microsoft code samples.
*/

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#define APC_IMPORT_MIDL
#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
// APC stuff
#include <apcCpp.h>
#include "vbamti.h"

// the following file contains the evaluation license key in the
// variables wszEvalLicKey and szEvalLicKey. 
// Properly licensed applications that redistribute VBA 
// will receive their own licence key, which must be used instead.
#include "evalkey.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
