// 
//   Copyright (c) Microsoft Corporation.  All rights reserved.
//
/// =======================================================================
// Name    :    main.cpp
// Purpose :    Windows bootstrap application that installs a 
//              product's MSI and then installs the VBA Core bits
//              if necessary.

// includes
#include <windows.h>
#include <crtdbg.h>
#include <tchar.h>
#include <stdio.h>
#include <Shlwapi.h>
#include <WinError.h>
#include <msi.h>

#include "CSingleInstance.hxx"  // CSingleInstance implementation
#include "CError.h"             // CError definition
#include "resource.h"           // string defines
#include "CSettings.h"          // ini-based app globals/settings

#include "main.h"

CSettings g_settings;
DWORD g_dwThread = 0;
HANDLE g_hThread = NULL;
BOOL   g_bAppReboot; // reboot indicated after host app install

// ==========================================================================
// WinMain()
//
// Purpose: application entry point
//
// ==========================================================================
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    UINT    uRetCode = 0;       // bootstrapper return code

    // Initialize reboot flag 
    g_bAppReboot = FALSE;

    // initialize hInstance in global settings
    g_settings.SetHInstance(hInstance);

    try 
    {
        // parse our application settings
        if (!g_settings.Parse())
        {
            CError se( IDS_SETTINGS_INIT_FAILURE, 
                       0, 
                       MB_ICONERROR, 
                       FALSE, 
                       g_settings.GetIniName());

            throw (se);
        }

        // create single instance object
        //
        CSingleInstance si( g_settings.GetProductName() );

        // validate single instance
        //
        // if we are not alone, throw an error
        if( !si.IsUnique() )
        {
            CError se( IDS_NOT_SINGLE_INSTANCE, 
                       0, 
                       MB_ICONERROR, 
                       ERROR_INSTALL_ALREADY_RUNNING );

            throw( se );
        }
        // if there was a problem creating mutex, throw an error
        else if( !si.IsHandleOK() )
        {
            CError se( IDS_SINGLE_INSTANCE_FAIL, 
                       0, 
                       MB_ICONERROR, 
                       ERROR_INSTALL_FAILURE );

            throw( se );
        }

        // put ourselves in install mode
        // if running on Terminal Server.
        SetTSInInstallMode();

        // Is this a new installation, or an update?
        if (INSTALLSTATE_UNKNOWN == MsiQueryProductState(g_settings.GetProductCode()))
        {
            // It is not installed - search for the the MSI and use it if found

            // let's be sure we can locate our msi
            HANDLE handle = CreateFile( g_settings.GetMsi(),
                                        GENERIC_READ,
                                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                                        NULL, 
                                        OPEN_EXISTING,
                                        FILE_ATTRIBUTE_NORMAL,
                                        NULL);

            if (INVALID_HANDLE_VALUE == handle)
            {
                DWORD dwResult = GetLastError();

                if (ERROR_FILE_NOT_FOUND == dwResult)
                {
                    CError se(IDS_MSI_NOT_FOUND, 
                            0, 
                            MB_ICONERROR, 
                            ERROR_FILE_NOT_FOUND, 
                            g_settings.GetMsi());

                    throw (se);
                }
                else
                {
                    CError se( IDS_SETUP_FAILURE, 0, MB_ICONERROR, dwResult);
                    throw (se);
                }
            }
            else
            {
                CloseHandle(handle);
            }

            ExecuteMSICommand(_T("/I"), (LPTSTR) g_settings.GetMsi());
        }
        else
        {
            // It is installed - use the Product Code to maintain
            ExecuteMSICommand(_T("/I"), (LPTSTR) g_settings.GetProductCode());
        }
        
        // Get the installation status for the VBA Core 
        BOOL bInstall, bInstalled;
        GetInstallationStatus(&bInstall, &bInstalled);

        // Initialize the debug flag
        BOOL bCanInstallDebug = TRUE;

        // Only need to change if the installed type is different than the install type
        if (bInstall != bInstalled)
        {
            if (bInstalled)
            {
                UninstallVBACore();
            }

            if (bInstall)
            {
                InstallVBACore();
            }
        }
        else
        {
            if (bInstalled)
            {
                SetInstalledBy(TRUE);
            }
        }

        // now handle the reboot
        if (g_bAppReboot)
        {
            CError se( IDS_REBOOT_QUERY, IDS_DIALOG_CAPTION, MB_YESNO);

            int nResult = se.ShowMessage();

            if (nResult == IDYES)
            {
                InitiateReboot();
            }
        }
    }

    catch (HRESULT)
    {
        // hresult exception msg display is handled
        // by the originator. the exception is rethrown
        // and caught here in order to exit.
    }

    catch( CError se )
    {
        uRetCode = se.m_nRetCode;

        // Don't want to show an error message if the user already knows
        // what the error was
        if (uRetCode != ERROR_INSTALL_USEREXIT)
        {
            se.ShowMessage();
        }
    }

    catch( ... )
    {
        CError se( IDS_SETUP_FAILURE, 0, MB_ICONERROR, ERROR_INSTALL_FAILURE );
        uRetCode = se.m_nRetCode;
        se.ShowMessage();
    }

    return uRetCode;
}

// ==========================================================================
// ExecCmd()
//
// Purpose:
//  Executes command-line
// Inputs:
//  LPCTSTR pszCmd: command to run
// Outputs:
//  DWORD dwExitCode: exit code from the command
// Notes: This routine does a CreateProcess on the input cmd-line
//        and waits for the launched process to exit.
// ==========================================================================
DWORD ExecCmd( LPCTSTR pszCmd )
{
    BOOL  bReturnVal   = false ;
    STARTUPINFO  si ;
    DWORD  dwExitCode ;
    SECURITY_ATTRIBUTES saProcess, saThread ;
    PROCESS_INFORMATION process_info ;

    ZeroMemory(&si, sizeof(si)) ;
    si.cb = sizeof(si) ;

    saProcess.nLength = sizeof(saProcess) ;
    saProcess.lpSecurityDescriptor = NULL ;
    saProcess.bInheritHandle = TRUE ;

    saThread.nLength = sizeof(saThread) ;
    saThread.lpSecurityDescriptor = NULL ;
    saThread.bInheritHandle = FALSE ;

    bReturnVal = CreateProcess(NULL, 
                               (LPTSTR)pszCmd, 
                               &saProcess, 
                               &saThread, 
                               FALSE, 
                               DETACHED_PROCESS, 
                               NULL, 
                               NULL, 
                               &si, 
                               &process_info) ;

    if (bReturnVal)
    {
        CloseHandle( process_info.hThread ) ;
        WaitForSingleObject( process_info.hProcess, INFINITE ) ;
        GetExitCodeProcess( process_info.hProcess, &dwExitCode ) ;
        CloseHandle( process_info.hProcess ) ;
    }
    else
    {
        CError se( IDS_CREATE_PROCESS_FAILURE, 
                   0, 
                   MB_ICONERROR, 
                   ERROR_INSTALL_FAILURE, 
                   pszCmd );

        throw( se );
    }

    return dwExitCode;
}

// ==========================================================================
// UninstallVBACore
//
// Purpose:
//  Checks whether the VBA Core bits are used by any other applications.
//  If so, it removes this application from the list and returns FALSE.
//  If not, it removes the VBA Core bits and returns TRUE.
// ==========================================================================
void UninstallVBACore( void )
{
    HKEY hkeyVBARoot        = NULL;
    HKEY hkeyVBA64          = NULL;
    
    try 
    {
        OpenKey(HKEY_LOCAL_MACHINE, (LPTSTR) g_tszVBARootKey, &hkeyVBARoot);
        OpenKey(hkeyVBARoot, (LPTSTR) g_tszVBA64Key, &hkeyVBA64);

        if (!SetInstalledBy(FALSE))
        {
            // Last one to uninstall - actually uninstall the bits this time

            // First, uninstall localized resources
            for (int i = 0; i < g_dwLocaleCount; i++)
            {
                if (INSTALLSTATE_DEFAULT == MsiQueryProductState(g_VBALocaleInformation[i].tszProductCode))
                {
                    ExecuteMSICommand((LPTSTR) g_settings.GetUninstallString(), (LPTSTR) g_VBALocaleInformation[i].tszProductCode);
                }
            }
            // Now, do the main one.
            ExecuteMSICommand((LPTSTR) g_settings.GetUninstallString(), (LPTSTR) g_tszVBACoreProductCode);

            // Close key and clean the registry
            CloseKey(&hkeyVBA64);
            SHDeleteKey(hkeyVBARoot, g_tszVBA64Key);
        }

    }
    catch( HRESULT hr )
    {
	    CloseKey(&hkeyVBA64);
	    CloseKey(&hkeyVBARoot);

        CError se;

        se.ShowHResultMessage(IDS_SETUP_FAILURE, 
                                0, 
                                MB_OK, 
                                hr, 
                                g_settings.GetProductName());

        throw hr;
    }

	CloseKey(&hkeyVBA64);
	CloseKey(&hkeyVBARoot);
}

// ==========================================================================
// InstallVBACore
//
// Purpose:
//  Installs the VBA Core bits on the machine if needed.
//  Adds this product as an installer of the core bits.
//
// ==========================================================================
void InstallVBACore( void )
{
    LONG lResult;
    HKEY hkeyVBARoot        = NULL;
    HKEY hkeyVBA64          = NULL;
    HKEY hkeyVBALocales     = NULL;
    
    try 
    {
        // Open subkeys as needed - most should exist by now, create those that might not
        CreateKey(HKEY_LOCAL_MACHINE, (LPTSTR) g_tszVBARootKey, &hkeyVBARoot);
        CreateKey(hkeyVBARoot, (LPTSTR) g_tszVBA64Key, &hkeyVBA64);
        CreateKey(hkeyVBA64, (LPTSTR) g_tszVBALocalesKey, &hkeyVBALocales);

        // First, install the package, if needed
        if (INSTALLSTATE_DEFAULT != MsiQueryProductState(g_tszVBACoreProductCode))
        {
            TCHAR szBuffer[MAX_PATH+1];
            StrCpyN(szBuffer, g_settings.GetVBALocation(), LENGTH(szBuffer) - 1);
            StrCatBuff(szBuffer, _T("\\vbaof11.msi"), LENGTH(szBuffer) - 1);

            ExecuteMSICommand((LPTSTR) g_settings.GetInstallString(), szBuffer);
        }

        // Now, check what locales need to be installed
        for (int i = 0; i < g_dwLocaleCount; i++)
        {
            lResult = RegQueryValueEx( hkeyVBALocales,
                                       g_VBALocaleInformation[i].tszLocaleCode,
                                       NULL,
                                       NULL,
                                       NULL,
                                       0
                                       );

            if (ERROR_SUCCESS == lResult)
            {
                // This locale should be installed - is it?
                if (INSTALLSTATE_DEFAULT != MsiQueryProductState(g_VBALocaleInformation[i].tszProductCode))
                {
                    // It's not installed - install it.
                    TCHAR szBuffer[MAX_PATH+1];
                    StrCpyN(szBuffer, g_settings.GetVBALocation(), LENGTH(szBuffer) - 1);
                    StrCatBuff(szBuffer, _T("\\"), LENGTH(szBuffer) - 1);
                    StrCatBuff(szBuffer, g_VBALocaleInformation[i].tszLocaleCode, LENGTH(szBuffer) - 1);
                    StrCatBuff(szBuffer, _T("\\vbaof11i.msi"), LENGTH(szBuffer) - 1);

                    ExecuteMSICommand((LPTSTR) g_settings.GetInstallString(), szBuffer);
                }
            }
            else
            {
                // This locale should not be installed - is it?
                if (INSTALLSTATE_DEFAULT == MsiQueryProductState(g_VBALocaleInformation[i].tszProductCode))
                {
                    // It is installed - remove it.
                    TCHAR szBuffer[MAX_PATH+1];
                    StrCpyN(szBuffer, g_settings.GetVBALocation(), LENGTH(szBuffer) - 1);
                    StrCatBuff(szBuffer, _T("\\"), LENGTH(szBuffer) - 1);
                    StrCatBuff(szBuffer, g_VBALocaleInformation[i].tszLocaleCode, LENGTH(szBuffer) - 1);
                    StrCatBuff(szBuffer, _T("\\vbaof11i.msi"), LENGTH(szBuffer) - 1);

                    ExecuteMSICommand((LPTSTR) g_settings.GetUninstallString(), szBuffer);
                }
            }
        }
        SetInstalledBy(TRUE);

        // Mark the registry properly.
        RegDeleteValue(hkeyVBA64, g_tszVBAInstallValue);
        DWORD dwInstalled = 1;
        lResult = RegSetValueEx( hkeyVBA64,
                            g_tszVBAInstalledValue,
                            NULL,
                            REG_DWORD,
                            (LPBYTE) &dwInstalled,
                            sizeof(dwInstalled)
                            );

        if (ERROR_SUCCESS != lResult)
        {
            throw HRESULT_FROM_WIN32(lResult);
        }
    }
    catch( HRESULT hr )
    {
        CloseKey(&hkeyVBA64);
        CloseKey(&hkeyVBALocales);
        CloseKey(&hkeyVBARoot);

        CError se;

        se.ShowHResultMessage(IDS_SETUP_FAILURE, 
                                0, 
                                MB_OK, 
                                HRESULT_FROM_WIN32(lResult), 
                                g_settings.GetProductName());

        throw hr;
    }

    CloseKey(&hkeyVBA64);
    CloseKey(&hkeyVBALocales);
    CloseKey(&hkeyVBARoot);
}

// ==========================================================================
// GetInstallationStatus
//
// Purpose:
//  Retrieve the installation status from the registry after running the
//  main MSI. 
//
// ==========================================================================
void GetInstallationStatus(BOOL *pbInstall, BOOL *pbInstalled)
{
    LONG lResult;
    HKEY hkeyVBARoot        = NULL;
    HKEY hkeyVBA64          = NULL;
    DWORD dwResult, dwSize;

    try
    {
        OpenKey(HKEY_LOCAL_MACHINE, (LPTSTR) g_tszVBARootKey, &hkeyVBARoot);
        OpenKey(hkeyVBARoot, (LPTSTR) g_tszVBA64Key, &hkeyVBA64);

        dwSize = sizeof(DWORD);
        lResult = RegQueryValueEx( hkeyVBA64,
                                   g_tszVBAInstallValue,
                                   NULL,
                                   NULL,
                                   (LPBYTE) &dwResult,
                                   &dwSize
                                   );

        if (ERROR_SUCCESS != lResult)
        {
            *pbInstall = FALSE;
        }
        else
        {
            *pbInstall = (dwResult == 1);
        }

        dwSize = sizeof(DWORD);
        lResult = RegQueryValueEx( hkeyVBA64,
                                   g_tszVBAInstalledValue,
                                   NULL,
                                   NULL,
                                   (LPBYTE) &dwResult,
                                   &dwSize
                                   );

        if (ERROR_SUCCESS != lResult)
        {
            *pbInstalled = FALSE;
        }
        else
        {
            *pbInstalled = (dwResult == 1);
        }
    }
    catch(HRESULT)
    {
        // If we get here, one of the OpenKey calls threw, so there isn't a 
        // VBA6.4 subkey. Hence, both of the install status values
        // must be FALSE.
        CloseKey(&hkeyVBA64);
        CloseKey(&hkeyVBARoot);

        *pbInstall = FALSE;
        *pbInstalled = FALSE;
    }

    CloseKey(&hkeyVBA64);
    CloseKey(&hkeyVBARoot);
}

// ==========================================================================
// SetInstalledBy
//
// Purpose:
//  Adds or removes this product from the InstalledBy key.
//  Returns true if there is at least one entry in the key after this
//  operation is completed.
// ==========================================================================
BOOL SetInstalledBy(BOOL bAdd)
{
    LONG lResult;
    HKEY hkeyVBARoot        = NULL;
    HKEY hkeyVBA64          = NULL;
    HKEY hkeyVBAInstalledBy = NULL;
    BOOL bResult = TRUE;

    try
    {
        // Open subkeys as needed - most should exist by now, create those that might not
        CreateKey(HKEY_LOCAL_MACHINE, (LPTSTR) g_tszVBARootKey, &hkeyVBARoot);
        CreateKey(hkeyVBARoot, (LPTSTR) g_tszVBA64Key, &hkeyVBA64);
        CreateKey(hkeyVBA64, (LPTSTR) g_tszVBAInstalledByKey, &hkeyVBAInstalledBy);

        if (bAdd)
        {
            // Add to the list - first check if it's there already.
            lResult = RegQueryValueEx( hkeyVBAInstalledBy,
                                    g_settings.GetProductCode(),
                                    NULL,
                                    NULL,
                                    NULL,
                                    0
                                    );

            if (ERROR_SUCCESS != lResult)
            {
                // This product has not installed the VBA core bits before
                DWORD dwDummy = 0;
                lResult = RegSetValueEx( hkeyVBAInstalledBy,
                                    g_settings.GetProductCode(),
                                    NULL,
                                    REG_DWORD,
                                    (LPBYTE) &dwDummy,
                                    sizeof(dwDummy)
                                    );

                if (ERROR_SUCCESS != lResult)
                {
                    throw HRESULT_FROM_WIN32(lResult);
                }
            }
        }
        else
        {
            // Remove from the list - first check if it's there already.
            lResult = RegQueryValueEx( hkeyVBAInstalledBy,
                                    g_settings.GetProductCode(),
                                    NULL,
                                    NULL,
                                    NULL,
                                    0
                                    );

            if (ERROR_SUCCESS == lResult)
            {
                // This product has installed the VBA core bits before
                lResult = RegDeleteValue( hkeyVBAInstalledBy, g_settings.GetProductCode());

                if (ERROR_SUCCESS != lResult)
                {
                    throw HRESULT_FROM_WIN32(lResult);
                }
            }
        }

        DWORD dwCount;
        // Check how many more values are listed.
        lResult = RegQueryInfoKey(hkeyVBAInstalledBy, 
                                            NULL, 
                                            NULL, 
                                            NULL, 
                                            NULL, 
                                            NULL, 
                                            NULL, 
                                            &dwCount, 
                                            NULL, 
                                            NULL, 
                                            NULL, 
                                            NULL);

        if (ERROR_SUCCESS != lResult)
        {
            throw HRESULT_FROM_WIN32(lResult);
        }

        bResult = (dwCount > 0);
    }
    catch(HRESULT hr)
    {
        CloseKey(&hkeyVBAInstalledBy);
        CloseKey(&hkeyVBA64);
        CloseKey(&hkeyVBARoot);

        throw hr;
    }

    CloseKey(&hkeyVBAInstalledBy);
    CloseKey(&hkeyVBA64);
    CloseKey(&hkeyVBARoot);

    return bResult;
}

// ==========================================================================
// OpenKey
// 
// Purpose:
//  Helper function for opening keys. Throws on failure.
// ==========================================================================
void OpenKey(HKEY hkeyParent, LPTSTR tszChildKeyName, PHKEY phkeyResult)
{
    LONG lResult;

    lResult = RegOpenKeyEx( hkeyParent,             // handle to open key
                            tszChildKeyName,        // name of subkey to open
                            NULL,
                            KEY_READ | KEY_WRITE,
                            phkeyResult             // handle to open key
                            );

    if (ERROR_SUCCESS != lResult)
    {
        throw HRESULT_FROM_WIN32(lResult);
    }
}

// ==========================================================================
// CreateKey
// 
// Purpose:
//  Helper function for creating keys. Throws on failure.
// ==========================================================================
void CreateKey(HKEY hkeyParent, LPTSTR tszChildKeyName, PHKEY phkeyResult)
{
    LONG lResult;

    lResult = RegCreateKeyEx( hkeyParent,               // handle to open key
                              tszChildKeyName,          // name of subkey to open
                              NULL,
                              NULL,
                              REG_OPTION_NON_VOLATILE,
                              KEY_READ | KEY_WRITE,
                              NULL,
                              phkeyResult,              // handle to open key
                              NULL
                              );

    if (ERROR_SUCCESS != lResult)
    {
        throw HRESULT_FROM_WIN32(lResult);
    }
}

// ==========================================================================
// CloseKey
//
// Purpose:
//  Helper function for closing keys.
// ==========================================================================
void CloseKey(PHKEY phkey)
{
    if (NULL != *phkey)
        RegCloseKey(*phkey);
    *phkey = NULL;
}


// ==========================================================================
// ExecuteMSICommand
// 
// Purpose:
//  Handles the execution of an MSI command and associated error handling
//
// ==========================================================================
void ExecuteMSICommand(LPTSTR tszOptions, LPTSTR tszProductIdentifier)
{
    // Allocate a buffer - make it big enough for the longest possible command string
    // 2 add'l chars for zero-term and space embedded between msi name & cmd-line
    // 3 * MAX_PATH is the storage allocated for the msi paths and command
    TCHAR szBuffer[ (3 * MAX_PATH) + 2];
    DWORD dwResult;

    _sntprintf( szBuffer, LENGTH(szBuffer)-1, g_tszMsiCmdLine, tszOptions, tszProductIdentifier);
    dwResult = ExecCmd(szBuffer);

    if ( ERROR_SUCCESS_REBOOT_REQUIRED == dwResult || ERROR_SUCCESS == dwResult )
    {
        g_bAppReboot = g_bAppReboot || (dwResult == ERROR_SUCCESS_REBOOT_REQUIRED);
    } 
    else 
    {
        CError se( IDS_SETUP_FAILURE, 0, MB_ICONERROR, dwResult );
        throw (se);
    }
}

// ==========================================================================
// InitiateReboot()
//
// Purpose: initiates a system reboot
//
// ==========================================================================
BOOL InitiateReboot()
{
    HANDLE hToken;              // handle to process token 
    TOKEN_PRIVILEGES tkp;       // pointer to token structure     
     
    try 
    {
        // Get the current process token handle so we can get shutdown 
        // privilege. 
         
        if (!OpenProcessToken(GetCurrentProcess(), 
                TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        {
            return FALSE;
        }
         
        // Get the LUID for shutdown privilege. 
         
        LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, 
                &tkp.Privileges[0].Luid); 
         
        tkp.PrivilegeCount = 1;  // one privilege to set    
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
         
        // Get shutdown privilege for this process. 
         
        AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
            (PTOKEN_PRIVILEGES) NULL, 0); 
         
        // Cannot test the return value of AdjustTokenPrivileges. 
         
        if (GetLastError() != ERROR_SUCCESS) 
        {
            return FALSE;
        }
    }

    catch (...)
    {
    }
    return ExitWindowsEx( EWX_REBOOT, 0);
}

// ==========================================================================
// SetTSInInstallMode()
//
// Purpose: checks if Terminal Services is enabled and if so
//          switches machine to INSTALL mode
// ==========================================================================
void SetTSInInstallMode()
{
    if (IsTerminalServicesEnabled())
    {
        ExecCmd(g_tszTSChangeUserToInstall);
    }
}

//Detecting If Terminal Services is Installed
// code is taken directly from  
// http://msdndevstg/library/psdk/termserv/termserv_7mp0.htm


/* -------------------------------------------------------------
   Note that the ValidateProductSuite and IsTerminalServices
   functions use ANSI versions of Win32 functions to maintain
   compatibility with Windows 95/98.
   ------------------------------------------------------------- */

BOOL IsTerminalServicesEnabled() 
{
  BOOL    bResult = FALSE;
  DWORD   dwVersion;
  OSVERSIONINFOEXA osVersion;
  DWORDLONG dwlCondition = 0;
  HMODULE hmodK32 = NULL;
  HMODULE hmodNtDll = NULL;
  typedef ULONGLONG (WINAPI *PFnVerSetCondition) (ULONGLONG, ULONG, UCHAR);
  typedef BOOL (WINAPI *PFnVerifyVersionA) (POSVERSIONINFOEXA, DWORD, DWORDLONG);
  PFnVerSetCondition pfnVerSetCondition;
  PFnVerifyVersionA pfnVerifyVersionA;

  dwVersion = GetVersion();

  // Are we running Windows NT?

  if (!(dwVersion & 0x80000000)) 
  {
    // Is it Windows 2000 or greater?
    
    if (LOBYTE(LOWORD(dwVersion)) > 4) 
    {
      // In Windows 2000, use the VerifyVersionInfo and 
      // VerSetConditionMask functions. Don't static link because 
      // it won't load on earlier systems.

      hmodNtDll = GetModuleHandleA( "ntdll.dll" );
      if (hmodNtDll) 
      {
        pfnVerSetCondition = (PFnVerSetCondition) GetProcAddress( 
            hmodNtDll, "VerSetConditionMask");
        if (pfnVerSetCondition != NULL) 
        {
          dwlCondition = (*pfnVerSetCondition) (dwlCondition, 
              VER_SUITENAME, VER_AND);

          // Get a VerifyVersionInfo pointer.

          hmodK32 = GetModuleHandleA( "KERNEL32.DLL" );
          if (hmodK32 != NULL) 
          {
            pfnVerifyVersionA = (PFnVerifyVersionA) GetProcAddress(
               hmodK32, "VerifyVersionInfoA") ;
            if (pfnVerifyVersionA != NULL) 
            {
              ZeroMemory(&osVersion, sizeof(osVersion));
              osVersion.dwOSVersionInfoSize = sizeof(osVersion);
              osVersion.wSuiteMask = VER_SUITE_TERMINAL;
              bResult = (*pfnVerifyVersionA) (&osVersion,
                  VER_SUITENAME, dwlCondition);
            }
          }
        }
      }
    }
    else  // This is Windows NT 4.0 or earlier.

      bResult = ValidateProductSuite( "Terminal Server" );
  }

  return bResult;
}


// ==========================================================================
// ValidateProductSuite()
//
// Purpose:
//  Terminal Services detection code for systems running
//  Windows NT 4.0 and earlier.
// ==========================================================================
BOOL ValidateProductSuite (LPSTR lpszSuiteToValidate) 
{
  BOOL fValidated = FALSE;
  LONG lResult;
  HKEY hKey = NULL;
  DWORD dwType = 0;
  DWORD dwSize = 0;
  LPSTR lpszProductSuites = NULL;
  LPSTR lpszSuite;

  // Open the ProductOptions key.

  lResult = RegOpenKeyA(
      HKEY_LOCAL_MACHINE,
      "System\\CurrentControlSet\\Control\\ProductOptions",
      &hKey
  );
  if (lResult != ERROR_SUCCESS)
      goto exit;

  // Determine required size of ProductSuite buffer.

  lResult = RegQueryValueExA( hKey, "ProductSuite", NULL, &dwType, 
      NULL, &dwSize );
  if (lResult != ERROR_SUCCESS || !dwSize)
      goto exit;

  // Allocate buffer.

  lpszProductSuites = (LPSTR) LocalAlloc( LPTR, dwSize );
  if (!lpszProductSuites)
      goto exit;

  // Retrieve array of product suite strings.

  lResult = RegQueryValueExA( hKey, "ProductSuite", NULL, &dwType,
      (LPBYTE) lpszProductSuites, &dwSize );
  if (lResult != ERROR_SUCCESS || dwType != REG_MULTI_SZ)
      goto exit;

  // Search for suite name in array of strings.

  lpszSuite = lpszProductSuites;
  while (*lpszSuite) 
  {
      if (lstrcmpA( lpszSuite, lpszSuiteToValidate ) == 0) 
      {
          fValidated = TRUE;
          break;
      }
      lpszSuite += (lstrlenA( lpszSuite ) + 1);
  }

exit:
  if (lpszProductSuites)
      LocalFree( lpszProductSuites );

  if (hKey)
      RegCloseKey( hKey );

  return fValidated;
}

