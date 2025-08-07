Attribute VB_Name = "Module1"
' ************************************************************************
'   Copyright (C) Microsoft Corporation, 1999.  All rights reserved.
'
'   This source code is intended only as a supplement to Microsoft
'   Development Tools and/or on-line documentation.  See these other
'   materials for detailed information regarding Microsoft code samples.
' ************************************************************************

'*** Global module for MDI Notepad sample application.  ***
'**********************************************************
Option Explicit

' User-defined type to store information about child forms
Type FormState
    Deleted As Integer
    Dirty As Integer
    Color As Long
End Type

Public FState As FormState              ' Array of user-defined types
Public gFindString As String            ' Holds the search text.
Public gFindCase As Integer             ' Key for case sensitive search
Public gFindDirection As Integer        ' Key for search direction.
Public gCurPos As Integer               ' Holds the cursor location.
Public gFirstTime As Integer            ' Key for start position.
Public Const ThisApp = "MDINote"        ' Registry App constant.
Public Const ThisKey = "Recent Files"   ' Registry Key constant.





Sub EditCopyProc()
    ' Copy the selected text onto the Clipboard.
    Clipboard.SetText frmSDI.txtNote.SelText
End Sub

Sub EditCutProc()
    ' Copy the selected text onto the Clipboard.
    Clipboard.SetText frmSDI.txtNote.SelText
    ' Delete the selected text.
    frmSDI.txtNote.SelText = ""
End Sub

Sub EditPasteProc()
    ' Place the text from the Clipboard into the active control.
    frmSDI.txtNote.SelText = Clipboard.GetText()
End Sub

Sub FileNew()
    Dim intResponse As Integer
    
    ' If the file has changed, save it
    If FState.Dirty = True Or frmSDI.m_apcInt.ApcProject.Dirty = True Then
        If MsgBox("Would you like to save changes?", vbYesNo, App.Title) = vbYes Then
            intResponse = FileSave(True)
            If intResponse = False Then Exit Sub
        End If
    End If
    ' Clear the textbox and update the caption.
    frmSDI.m_apcInt.CreateNewCleanProject True
    
    FState.Dirty = False
    
End Sub
Function FileSave(ByVal SaveOldProject As Boolean) As Integer
    Dim strFilename As String

    If frmSDI.Caption = "SDI NotePad - Untitled" Then
        ' The file hasn't been saved yet.
        ' Get the filename, and then call the save procedure, GetFileName.
        strFilename = ""
    Else
        ' The form's Caption contains the name of the open file.
        strFilename = Right(frmSDI.Caption, Len(frmSDI.Caption) - 14)
    End If
    ' Call the save procedure. If Filename = Empty, then
    ' the user chose Cancel in the Save As dialog box; otherwise,
    ' save the file.
    SaveFileAs strFilename, SaveOldProject
    FileSave = True
   
End Function


Sub FindIt()
    Dim intStart As Integer
    Dim intPos As Integer
    Dim strFindString As String
    Dim strSourceString As String
    Dim strMsg As String
    Dim intResponse As Integer
    Dim intOffset As Integer
    
    ' Set offset variable based on cursor position.
    If (gCurPos = frmSDI.txtNote.SelStart) Then
        intOffset = 1
    Else
        intOffset = 0
    End If

    ' Read the public variable for start position.
    If gFirstTime Then intOffset = 0
    ' Assign a value to the start value.
    intStart = frmSDI.txtNote.SelStart + intOffset
        
    ' If not case sensitive, convert the string to upper case
    If gFindCase Then
        strFindString = gFindString
        strSourceString = frmSDI.txtNote.Text
    Else
        strFindString = UCase(gFindString)
        strSourceString = UCase(frmSDI.txtNote.Text)
    End If
            
    ' Search for the string.
    If gFindDirection = 1 Then
        intPos = InStr(intStart + 1, strSourceString, strFindString)
    Else
        For intPos = intStart - 1 To 0 Step -1
            If intPos = 0 Then Exit For
            If Mid(strSourceString, intPos, Len(strFindString)) = strFindString Then Exit For
        Next
    End If

    ' If the string is found...
    If intPos Then
        frmSDI.txtNote.SelStart = intPos - 1
        frmSDI.txtNote.SelLength = Len(strFindString)
    Else
        strMsg = "Cannot find " & Chr(34) & gFindString & Chr(34)
        intResponse = MsgBox(strMsg, 0, App.Title)
    End If
    
    ' Reset the public variables
    gCurPos = frmSDI.txtNote.SelStart
    gFirstTime = False
End Sub


Sub ResizeNote()
    ' Expand text box to fill the form's internal area.
    If frmSDI.picToolbar.Visible Then
        If frmSDI.ScaleHeight - frmSDI.picToolbar.Height > 0 Then
            frmSDI.txtNote.Height = frmSDI.ScaleHeight - frmSDI.picToolbar.Height
        End If
        frmSDI.txtNote.Width = frmSDI.ScaleWidth
        frmSDI.txtNote.Top = frmSDI.picToolbar.Height
    Else
        frmSDI.txtNote.Height = frmSDI.ScaleHeight
        frmSDI.txtNote.Width = frmSDI.ScaleWidth
        frmSDI.txtNote.Top = 0
    End If
End Sub




