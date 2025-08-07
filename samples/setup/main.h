// 
//   Copyright (c) Microsoft Corporation.  All rights reserved.
// 
/// ==========================================================================
// Name    :    main.h
// Purpose :    #ncludes, prototypes, globals, and const 
//              definitions for setup sample

// prototypes

void    UninstallVBACore( void );
void    InstallVBACore( void );
DWORD   ExecCmd( LPCTSTR pszCmd );
BOOL    InitiateReboot();
BOOL    ValidateProductSuite (LPSTR lpszSuiteToValidate);
void    SetTSInInstallMode();
BOOL    IsTerminalServicesEnabled();
void    ExecuteMSICommand(LPTSTR tszOptions, LPTSTR tszProductIdentifier);
void    GetInstallationStatus(BOOL *bInstall, BOOL *bInstalled);
BOOL    SetInstalledBy(BOOL bAdd);
void    OpenKey(HKEY hkeyParent, LPTSTR tszChildKeyName, PHKEY phkeyResult);
void    CreateKey(HKEY hkeyParent, LPTSTR tszChildKeyName, PHKEY phkeyResult);
void    CloseKey(PHKEY phkey);

// msi install cmd-line
const TCHAR g_tszMsiCmdLine[]                = _T("Msiexec %s \"%s\" REBOOT=ReallySuppress");

// reg key for VBA info
// this key resides in HKEY_LOCAL_MACHINE
const TCHAR g_tszVBARootKey[]			= _T("SOFTWARE\\Microsoft\\VBA");
const TCHAR g_tszVBA64Key[] 			= _T("VBA6.4");
const TCHAR g_tszVBAInstallValue[]      = _T("Install");
const TCHAR g_tszVBAInstalledValue[]    = _T("Installed");
const TCHAR g_tszVBAInstalledByKey[]    = _T("InstalledBy");
const TCHAR g_tszVBALocalesKey[]        = _T("Locales");

// VBA Core Product Codes
const TCHAR g_tszVBACoreProductCode[] = _T("{90F50409-6000-11D3-8CFE-0150048383C9}");
typedef struct 
{
    TCHAR tszProductCode[39];
    TCHAR tszLocaleCode[5];
} VBALocaleInformation;
const VBALocaleInformation g_VBALocaleInformation[] = {
    {_T("{90F60404-6000-11D3-8CFE-0150048383C9}"),    _T("1028")},
    {_T("{90F60407-6000-11D3-8CFE-0150048383C9}"),    _T("1031")},
    {_T("{90F60409-6000-11D3-8CFE-0150048383C9}"),    _T("1033")},
    {_T("{90F6040C-6000-11D3-8CFE-0150048383C9}"),    _T("1036")},
    {_T("{90F60410-6000-11D3-8CFE-0150048383C9}"),    _T("1040")},
    {_T("{90F60411-6000-11D3-8CFE-0150048383C9}"),    _T("1041")},
    {_T("{90F60412-6000-11D3-8CFE-0150048383C9}"),    _T("1042")},
    {_T("{90F60413-6000-11D3-8CFE-0150048383C9}"),    _T("1043")},
    {_T("{90F60416-6000-11D3-8CFE-0150048383C9}"),    _T("1046")},
    {_T("{90F6041D-6000-11D3-8CFE-0150048383C9}"),    _T("1053")},
    {_T("{90F60804-6000-11D3-8CFE-0150048383C9}"),    _T("2052")},
    {_T("{90F60C0A-6000-11D3-8CFE-0150048383C9}"),    _T("3082")}
};
const DWORD g_dwLocaleCount = 12;

// Terminal Server settings string
const TCHAR   g_tszTSChangeUserToInstall[] = _T("change user /INSTALL");
