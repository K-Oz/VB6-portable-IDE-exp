// 
//   Copyright (c) Microsoft Corporation.  All rights reserved.
//
/// ==========================================================================
// Name    :    CSettings.cpp
// Purpose :    simple wrapper class for handling app
//              settings by means of an .ini file

// includes
#include <windows.h>
#include <shlwapi.h>
#include <stdio.h>

#include "CSettings.h"
#include "CError.h"
#include "resource.h"

// ==========================================================================
// CSetting::CSetting()
//
// Purpose:
//  parameterless CSetting object construction
// ==========================================================================
CSettings::CSettings(LPCTSTR pszIniName)
{
    m_hAppInst = NULL;

    TCHAR * pszWalk = NULL;

    *m_szMsi              = END_OF_STRING;
    *m_szProductName      = END_OF_STRING;
	*m_szProductCode      = END_OF_STRING;

    StrCpyN(m_szVBAInstallString, _T("/I"), LENGTH(m_szVBAInstallString));
    StrCpyN(m_szVBAUninstallString, _T("/qb /x"), LENGTH(m_szVBAUninstallString));

    GetModuleFileName( g_settings.GetHInstance(), 
                       m_szIniName, 
                       LENGTH(m_szIniName) );
    
    pszWalk = _tcsrchr( m_szIniName, BACKSLASH );

    if (pszWalk)
    {
        pszWalk++; // preserve trailing '\'
        *pszWalk = END_OF_STRING;

    }

	// Initialize the Location paths
	StrCpyN(m_szVBALocation, m_szIniName, LENGTH(m_szVBALocation));
    StrCpyN(m_szMsi, m_szIniName, LENGTH(m_szMsi));

    // pszIniName is never NULL due to ctor init
    StrCatBuff(m_szIniName, pszIniName, LENGTH(m_szIniName)); 
}

BOOL CSettings::Parse()
{
    BOOL bSucceeded = TRUE;

    DWORD dwCount = 0;
    UINT  uiResult;
    TCHAR szBuffer[MAX_PATH + 1];

    // note that we continue to parse even
    // on encountering a problem. this is 
    // because some values are initialized
    // even in the absence of an ini file
    // and are required for msg box captions, etc.

    // Retrieve the Msi name that we will install
    // This is a required key
    dwCount = GetPrivateProfileString(g_szSetup,
                                     g_szMsiKey,
                                     NULL,
                                     szBuffer,
                                     LENGTH(szBuffer),
                                     m_szIniName);

    // Msi is a required key, fail if not present
    if (dwCount == 0)
    {
        bSucceeded = FALSE;
    }
    else
    {
    	StrCatBuff(m_szMsi, szBuffer, LENGTH(m_szMsi));
    }

    // Retrieve the general application name.
    // If this key is not present, we will load
    // a resource-based default value
    dwCount = GetPrivateProfileString(g_szSetup,
                                     g_szProductNameKey,
                                     NULL,
                                     m_szProductName,
                                     LENGTH(m_szProductName),
                                     m_szIniName);

    if (dwCount == 0)
    {
        ::LoadString( g_settings.GetHInstance(), 
                      IDS_DEFAULT_PRODUCTNAME, 
                      m_szProductName, 
                      LENGTH(m_szProductName) ) ;

        if ( *m_szProductName == END_OF_STRING)
        {
            bSucceeded = FALSE;
        }
    }

    // Retrieve the product ID.
    // This is a required key.
    dwCount = GetPrivateProfileString(g_szSetup,
                                     g_szProductCodeKey,
                                     NULL,
                                     m_szProductCode,
                                     LENGTH(m_szProductCode),
                                     m_szIniName);

	// Product ID is a required key. Fail if not present.
    if (dwCount == 0)
    {
        bSucceeded = FALSE;
    }

    // Retrieve the VBA Location
    // This is a required key.
    dwCount = GetPrivateProfileString(g_szSetup,
                                     g_szVBALocationKey,
                                     NULL,
                                     szBuffer,
                                     LENGTH(szBuffer),
                                     m_szIniName);

	// VBALocation is an optional key. Default to current directory if not present.
    if (dwCount != 0)
    {
    	StrCatBuff(m_szVBALocation, szBuffer, LENGTH(m_szVBALocation));
    }
   	StrCatBuff(m_szVBALocation, _T("vba"), LENGTH(m_szVBALocation));

    uiResult = GetPrivateProfileInt( g_szSetup,
                                     g_szSilentProgressKey,
                                     0,
                                     m_szIniName);

	// Optional key
    if (uiResult != 0)
    {
        StrCpyN(m_szVBAInstallString, _T("/qn /I"), LENGTH(m_szVBAInstallString));
        StrCpyN(m_szVBAUninstallString, _T("/qn /x"), LENGTH(m_szVBAUninstallString));
    }


	return bSucceeded;
}