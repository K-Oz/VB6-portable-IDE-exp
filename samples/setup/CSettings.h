// 
//   Copyright (c) Microsoft Corporation.  All rights reserved.
//
/// ==========================================================================
// Name    :    CSettings.h
// Purpose :    simple wrapper class for handling app
//              settings by means of an .ini     

#ifndef SETTINGS_H
#define SETTINGS_H

#include <windows.h>
#include <tchar.h>

//defines
//
#define EMPTY_BUFFER  { _T( '\0' ) }
#define END_OF_STRING   _T( '\0' )
#define BACKSLASH       _T( '\\' )

#define MAX_MSG 4096

const TCHAR g_szSettingsFile[]         = _T("setup.ini");
const TCHAR g_szSetup[]                = _T("Setup");
const TCHAR g_szMsiKey[]               = _T("Msi");
const TCHAR g_szProductNameKey[]       = _T("ProductName");
const TCHAR g_szProductCodeKey[]       = _T("ProductCode");
const TCHAR g_szSilentProgressKey[]    = _T("SilentProgress");
const TCHAR g_szVBALocationKey[]       = _T("VBALocation");

// ==========================================================================
// class CSettings
//
// Purpose:
//  This class wraps an ini file specific to this sample
// ==========================================================================
class CSettings
{
public:
    // Constructors
    CSettings(LPCTSTR pszIniName = g_szSettingsFile);
    
    BOOL Parse();

    // Getters/Setters
    inline LPCTSTR GetMsi()              { return m_szMsi;                }
    inline LPCTSTR GetIniName()          { return m_szIniName;            }
    inline LPCTSTR GetProductName()      { return m_szProductName;        }
	inline LPCTSTR GetProductCode()      { return m_szProductCode;        }
	inline LPCTSTR GetVBALocation()      { return m_szVBALocation;        }
    inline LPCTSTR GetInstallString()    { return m_szVBAInstallString;   }
    inline LPCTSTR GetUninstallString()  { return m_szVBAUninstallString; }
    
    inline HINSTANCE GetHInstance()           { return m_hAppInst;  }
    inline void SetHInstance(HINSTANCE hInst) { m_hAppInst = hInst; }

private:
    
    HINSTANCE   m_hAppInst;
    TCHAR       m_szIniName[MAX_PATH+1];
    TCHAR       m_szMsi[MAX_PATH+1];
    TCHAR       m_szProductName[MAX_PATH];
	TCHAR       m_szProductCode[MAX_PATH];
	TCHAR       m_szVBALocation[MAX_PATH+1];
    TCHAR       m_szVBAInstallString[10];
    TCHAR       m_szVBAUninstallString[10];
};

// global settings object
extern CSettings g_settings;

#endif // SETTINGS_H