VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CApplication"
Attribute VB_GlobalNameSpace = True
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Description = "Main host application object representing the SDI notepad exposed properties and methods."
' ************************************************************************
'   Copyright (C) Microsoft Corporation, 1999.  All rights reserved.
'
'   This source code is intended only as a supplement to Microsoft
'   Development Tools and/or on-line documentation.  See these other
'   materials for detailed information regarding Microsoft code samples.
' ************************************************************************


' *** This module contains the implementation of the application object ***
' *** model that the host exposes.
'**************************************************************************

Option Explicit



'--------------------------------------------------
'**** VBA Integration code, begin insert
Public Property Get Application() As CApplication
Attribute Application.VB_Description = "Main host application object representing the SDI notepad exposed properties and methods."
    Set Application = Me
End Property
'**** VBA Integration code, end insert
'--------------------------------------------------



'--------------------------------------------------
'**** VBA Integration code, begin insert
Public Property Get Name() As String
Attribute Name.VB_Description = "Host document name."
    Name = frmSDI.m_apcInt.ApcHost.HostName
End Property
'**** VBA Integration code, end insert
'--------------------------------------------------



'--------------------------------------------------
'**** VBA Integration code, begin insert
Public Property Get Parent() As CApplication
Attribute Parent.VB_Description = "Parent object."
    Set Parent = Me
End Property
'**** VBA Integration code, end insert
'--------------------------------------------------



'--------------------------------------------------
'**** VBA Integration code, begin insert
Public Property Get VBE() As Object
Attribute VBE.VB_Description = "The VBE object."
    Set VBE = frmSDI.m_apcInt.ApcHost.VBE
End Property
'**** VBA Integration code, end insert
'--------------------------------------------------




' VBA Notepad Integration
' *************************************
' Additional Exposed properties and methods specific to the notepad application object



' Clears the notepad text
Sub Clear()
Attribute Clear.VB_Description = "Clears the document text."
    frmSDI.txtNote.Text = ""
End Sub

' Select all text in the notepad
Sub SelectAll()
Attribute SelectAll.VB_Description = "Select all text in the document."
    frmSDI.txtNote.SelStart = 0
    frmSDI.txtNote.SelLength = Len(frmSDI.txtNote.Text)
End Sub

' Set the text in the notepad to NewText.
Property Let Text(NewText As String)
Attribute Text.VB_Description = "Current text in the document"
    If Len(NewText) > 2000 Then
        MsgBox "Adding text failed. Must be less than 2000 characters."
    ElseIf Len(frmSDI.txtNote) + Len(NewText) > 65000 Then
        MsgBox "Adding text failed. Adding this would exceed 65000 characters."
    Else
        frmSDI.txtNote.Text = NewText
        FState.Dirty = True
    End If
End Property

' Get the current text from the notepad
Property Get Text() As String
    Text = frmSDI.txtNote.Text
End Property

' Set the background color for the notepad to NewBackColor
Property Let BackColor(NewBackColor As Long)
Attribute BackColor.VB_Description = "BackColor for the host document."
    If NewBackColor < 0 Then
        frmSDI.txtNote.BackColor = RGB(250, 250, 250)
    Else
        frmSDI.txtNote.BackColor = NewBackColor
    End If
    FState.Dirty = True
End Property

' Get the notepad background color
Property Get BackColor() As Long
    BackColor = frmSDI.txtNote.BackColor
End Property

' Set the fore color for the notepad to NewForeColor
Property Let ForeColor(NewForeColor As Long)
Attribute ForeColor.VB_Description = "ForeColor for the host document."
    If NewForeColor < 0 Then
        frmSDI.txtNote.ForeColor = 0
    Else
        frmSDI.txtNote.ForeColor = NewForeColor
    End If
    FState.Dirty = True
End Property

' Get the notepad forecolor
Property Get ForeColor() As Long
    ForeColor = frmSDI.txtNote.ForeColor
End Property

' *************************************
' VBA Notepad Integration




