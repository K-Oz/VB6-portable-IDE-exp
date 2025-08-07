VERSION 5.00
Begin VB.Form frmAbout 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "About SDI NotePad"
   ClientHeight    =   2175
   ClientLeft      =   2340
   ClientTop       =   1935
   ClientWidth     =   5100
   ClipControls    =   0   'False
   Icon            =   "frmAbout.frx":0000
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1501.224
   ScaleMode       =   0  'User
   ScaleWidth      =   4789.164
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton cmdOK 
      Caption         =   "OK"
      Height          =   375
      Left            =   3840
      TabIndex        =   0
      Top             =   120
      Width           =   1095
   End
   Begin VB.Label Label5 
      Caption         =   "Copyright (C) 1999 by Microsoft Corporation, All Rights Reserved."
      Height          =   255
      Left            =   240
      TabIndex        =   5
      Top             =   1800
      Width           =   4815
   End
   Begin VB.Label lblVersion 
      Caption         =   "VBA Version"
      Height          =   375
      Left            =   1440
      TabIndex        =   4
      Top             =   1200
      Width           =   2175
   End
   Begin VB.Label Label3 
      Caption         =   "Version 1.0"
      Height          =   255
      Left            =   1440
      TabIndex        =   3
      Top             =   960
      Width           =   1815
   End
   Begin VB.Label Label2 
      Caption         =   "VBA Integration Sample."
      Height          =   255
      Left            =   1440
      TabIndex        =   2
      Top             =   600
      Width           =   2055
   End
   Begin VB.Label Label1 
      Caption         =   "Microsoft SDI Notepad."
      Height          =   255
      Left            =   1440
      TabIndex        =   1
      Top             =   360
      Width           =   1935
   End
   Begin VB.Image Image1 
      Height          =   600
      Left            =   360
      Picture         =   "frmAbout.frx":014A
      Stretch         =   -1  'True
      Top             =   600
      Width           =   600
   End
End
Attribute VB_Name = "frmAbout"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
' ************************************************************************
'   Copyright (C) Microsoft Corporation, 1999.  All rights reserved.
'
'   This source code is intended only as a supplement to Microsoft
'   Development Tools and/or on-line documentation.  See these other
'   materials for detailed information regarding Microsoft code samples.
' ************************************************************************

' *** About Dialog ***
' ********************
Option Explicit


Private Sub cmdOK_Click()

    Me.Visible = False
    Unload Me
    
End Sub
