Attribute VB_Name = "Module2"
' ************************************************************************
'   Copyright (C) Microsoft Corporation, 1999.  All rights reserved.
'
'   This source code is intended only as a supplement to Microsoft
'   Development Tools and/or on-line documentation.  See these other
'   materials for detailed information regarding Microsoft code samples.
' ************************************************************************


'*** Standard module with procedures for working with   ***
'*** files. Part of the SDI Notepad sample application. ***
'**********************************************************
Option Explicit

Sub FileOpenProc()
    
    Dim intResponse As Integer
    Dim strOpenFileName As String
    
    ' VBA Notepad Integration
    ' *****************************************************************************
    ' Verify if we need to save if either the VBA project or the notepad text is dirty
    
    If FState.Dirty = True Or frmSDI.m_apcInt.ApcProject.Dirty = True Then
        If MsgBox("Would you like to save changes?", vbYesNo, App.Title) = vbYes Then
            intResponse = FileSave(True)
            If intResponse = False Then Exit Sub
        End If
    End If
    
    ' *****************************************************************************
    ' VBA Notepad Integration
    
    On Error Resume Next
    OpenFile
    
End Sub



Function OnRecentFilesList(FileName) As Integer
    Dim i         ' Counter variable.

    For i = 1 To 4
        If frmSDI.mnuRecentFile(i).Caption = FileName Then
            OnRecentFilesList = True
            Exit Function
        End If
    Next i
    OnRecentFilesList = False
End Function

Sub OpenFile()
    Dim fIndex As Integer
    
    ' Change the mouse pointer to an hourglass.
    Screen.MousePointer = 11
    
    ' VBA Notepad Integration
    ' **************************************************
    ' Delegate the open to the host application object implemented in APCIntegration class module
    frmSDI.m_apcInt.OpenProj
    ' **************************************************
    ' VBA Notepad Integration
    
    FState.Dirty = False
    
    ' Reset the mouse pointer.
    Screen.MousePointer = 0
End Sub

Sub SaveFileAs(FileName, ByVal SaveOldProject As Boolean)
    On Error Resume Next
    
    ' VBA Notepad Integration
    ' **************************************************
    ' Delegate the save to the the host application object implemented in APCIntegration class module
    ' We pass SaveOldProject to distinguish between a Save and a SaveAs in the SaveProj routine
    
    If Not SaveOldProject Then
        frmSDI.m_apcInt.SaveProj FileName
    Else
        frmSDI.m_apcInt.SaveProj
    End If
    ' **************************************************
    ' VBA Notepad Integration
    
    If Err Then
        MsgBox Error, 48, App.Title
    Else
        frmSDI.Caption = "SDI NotePad - " & FileName
        ' Reset the dirty flag.
        FState.Dirty = False
    End If
End Sub


