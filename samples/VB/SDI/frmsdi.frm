VERSION 5.00
Begin VB.Form frmSDI 
   Caption         =   "SDI NotePad - Untitled"
   ClientHeight    =   5370
   ClientLeft      =   315
   ClientTop       =   1470
   ClientWidth     =   6690
   Icon            =   "frmsdi.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   ScaleHeight     =   5370
   ScaleWidth      =   6690
   Begin VB.TextBox txtNote 
      Height          =   3975
      HideSelection   =   0   'False
      Left            =   0
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   0
      Top             =   480
      Width           =   5655
   End
   Begin VB.PictureBox picToolbar 
      Align           =   1  'Align Top
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   405
      Left            =   0
      ScaleHeight     =   345
      ScaleWidth      =   6630
      TabIndex        =   1
      Top             =   0
      Width           =   6690
      Begin VB.Image imgFileNewButton 
         Height          =   330
         Left            =   0
         Picture         =   "frmsdi.frx":014A
         ToolTipText     =   "New File"
         Top             =   0
         Width           =   360
      End
      Begin VB.Image imgFileOpenButton 
         Height          =   330
         Left            =   360
         Picture         =   "frmsdi.frx":02D4
         ToolTipText     =   "Open File"
         Top             =   0
         Width           =   360
      End
      Begin VB.Image imgCutButton 
         Height          =   330
         Left            =   840
         Picture         =   "frmsdi.frx":045E
         ToolTipText     =   "Cut"
         Top             =   0
         Width           =   375
      End
      Begin VB.Image imgCopyButton 
         Height          =   330
         Left            =   1200
         Picture         =   "frmsdi.frx":0640
         ToolTipText     =   "Copy"
         Top             =   0
         Width           =   375
      End
      Begin VB.Image imgPasteButton 
         Height          =   330
         Left            =   1560
         Picture         =   "frmsdi.frx":0822
         ToolTipText     =   "Paste"
         Top             =   0
         Width           =   375
      End
      Begin VB.Image imgFileNewButtonDn 
         Height          =   330
         Left            =   2040
         Picture         =   "frmsdi.frx":0A04
         Top             =   0
         Visible         =   0   'False
         Width           =   375
      End
      Begin VB.Image imgFileNewButtonUp 
         Height          =   330
         Left            =   2400
         Picture         =   "frmsdi.frx":0BE6
         Top             =   0
         Visible         =   0   'False
         Width           =   360
      End
      Begin VB.Image imgFileOpenButtonUp 
         Height          =   330
         Left            =   3120
         Picture         =   "frmsdi.frx":0D70
         Top             =   0
         Visible         =   0   'False
         Width           =   360
      End
      Begin VB.Image imgFileOpenButtonDn 
         Height          =   330
         Left            =   2760
         Picture         =   "frmsdi.frx":0EFA
         Top             =   0
         Visible         =   0   'False
         Width           =   360
      End
      Begin VB.Image imgCutButtonUp 
         Height          =   330
         Left            =   3480
         Picture         =   "frmsdi.frx":1084
         Top             =   0
         Visible         =   0   'False
         Width           =   375
      End
      Begin VB.Image imgCutButtonDn 
         Height          =   330
         Left            =   3840
         Picture         =   "frmsdi.frx":1266
         Top             =   0
         Visible         =   0   'False
         Width           =   375
      End
      Begin VB.Image imgCopyButtonUp 
         Height          =   330
         Left            =   4560
         Picture         =   "frmsdi.frx":1448
         Top             =   0
         Visible         =   0   'False
         Width           =   375
      End
      Begin VB.Image imgCopyButtonDn 
         Height          =   330
         Left            =   4200
         Picture         =   "frmsdi.frx":162A
         Top             =   0
         Visible         =   0   'False
         Width           =   375
      End
      Begin VB.Image imgPasteButtonDn 
         Height          =   330
         Left            =   4920
         Picture         =   "frmsdi.frx":180C
         Top             =   0
         Visible         =   0   'False
         Width           =   375
      End
      Begin VB.Image imgPasteButtonUp 
         Height          =   330
         Left            =   5280
         Picture         =   "frmsdi.frx":19EE
         Top             =   0
         Visible         =   0   'False
         Width           =   375
      End
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuFileNew 
         Caption         =   "&New"
      End
      Begin VB.Menu mnuFileOpen 
         Caption         =   "&Open"
      End
      Begin VB.Menu mnuFileSave 
         Caption         =   "&Save"
      End
      Begin VB.Menu mnuFileSaveAs 
         Caption         =   "Save &As"
      End
      Begin VB.Menu mnuFSep1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "E&xit"
      End
      Begin VB.Menu mnuRecentFile 
         Caption         =   "-"
         Index           =   0
         Visible         =   0   'False
      End
      Begin VB.Menu mnuRecentFile 
         Caption         =   "RecentFile1"
         Index           =   1
         Visible         =   0   'False
      End
      Begin VB.Menu mnuRecentFile 
         Caption         =   "RecentFile2"
         Index           =   2
         Visible         =   0   'False
      End
      Begin VB.Menu mnuRecentFile 
         Caption         =   "RecentFile3"
         Index           =   3
         Visible         =   0   'False
      End
      Begin VB.Menu mnuRecentFile 
         Caption         =   "RecentFile4"
         Index           =   4
         Visible         =   0   'False
      End
      Begin VB.Menu mnuRecentFile 
         Caption         =   "RecentFile5"
         Index           =   5
         Visible         =   0   'False
      End
   End
   Begin VB.Menu mnuEdit 
      Caption         =   "&Edit"
      Begin VB.Menu mnuEditCut 
         Caption         =   "Cu&t"
         Shortcut        =   ^X
      End
      Begin VB.Menu mnuEditCopy 
         Caption         =   "&Copy"
         Shortcut        =   ^C
      End
      Begin VB.Menu mnuEditPaste 
         Caption         =   "&Paste"
         Shortcut        =   ^V
      End
      Begin VB.Menu mnuEditDelete 
         Caption         =   "De&lete"
         Shortcut        =   {DEL}
      End
      Begin VB.Menu mnuEditSep1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuEditSelectAll 
         Caption         =   "Select &All"
      End
      Begin VB.Menu mnuEditTime 
         Caption         =   "Time / &Date"
      End
   End
   Begin VB.Menu mnuSearch 
      Caption         =   "&Search"
      Begin VB.Menu mnuSearchFind 
         Caption         =   "&Find"
      End
      Begin VB.Menu mnuSearchFindNext 
         Caption         =   "Find &Next"
         Shortcut        =   {F3}
      End
   End
   Begin VB.Menu mnuOptions 
      Caption         =   "&Options"
      Begin VB.Menu mnuOptionsToolbar 
         Caption         =   "&Toolbar"
      End
      Begin VB.Menu mnuFonts 
         Caption         =   "&Fonts"
         Begin VB.Menu mnuFontName 
            Caption         =   "FontName"
            Index           =   0
         End
      End
      Begin VB.Menu mnuOptionsLaunch 
         Caption         =   "&Launch New Instance"
      End
   End
   Begin VB.Menu mnuToolsMenuItem 
      Caption         =   "&Tools"
      Begin VB.Menu mnuMacrosMenuItem2 
         Caption         =   "Macro"
         Begin VB.Menu mnuMacrosMenuItem 
            Caption         =   "&Macros                   Alt+F8"
         End
         Begin VB.Menu mnuVbeMenuItem 
            Caption         =   "&Visual Basic Editor  Alt+F11"
         End
      End
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "&Help"
      Begin VB.Menu mnuHelpAbout 
         Caption         =   "&About SDI NotePad..."
      End
   End
End
Attribute VB_Name = "frmSDI"
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


'*** Main form for the SDI Notepad sample application  ***
'**********************************************************
Option Explicit



'--------------------------------------------------
'**** VBA Integration code, begin insert
Public m_apcInt As APCIntegration
'**** VBA Integration code, end insert
'--------------------------------------------------

'--------------------------------------------------
'**** VBA Integration code, begin insert
Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
    If KeyCode = vbKeyF11 And Shift = vbAltMask Then m_apcInt.showVBE  '[Alt][F11]
    If KeyCode = vbKeyF8 And Shift = vbAltMask Then m_apcInt.showMacroDialog  '[Alt][F8]
End Sub
'**** VBA Integration code, end insert
'--------------------------------------------------





Private Sub mnuHelpAbout_Click()

    ' VBA Notepad Integration
    ' ***********************
    ' Display about information
    Load frmAbout
    frmAbout.lblVersion = frmAbout.lblVersion & " " & m_apcInt.ApcHost.VBAVersion
    frmAbout.Visible = True
    ' ***********************
    ' VBA Notepad Integration
End Sub

'--------------------------------------------------
'**** VBA Integration code, begin insert
Private Sub mnuVbeMenuItem_Click()
    ' Show the VBE now
    m_apcInt.showVBE
End Sub
'**** VBA Integration code, end insert
'--------------------------------------------------



'--------------------------------------------------
'**** VBA Integration code, begin insert
Private Sub mnuMacrosMenuItem_Click()
    ' The macros dialog will only be viewable if you have a project loaded
    m_apcInt.showMacroDialog
End Sub
'**** VBA Integration code, end insert
'--------------------------------------------------





Private Sub Form_Load()

'--------------------------------------------------
'**** VBA Integration code, begin insert
    Dim appObj As CApplication

    Set m_apcInt = New APCIntegration
    ' If you allready have an existing application object in your
    '  original source, replace "new CApplication" here with a
    '  reference to it.
    Set appObj = New CApplication
    m_apcInt.Initialize appObj, Me.hwnd
'**** VBA Integration code, end insert
'--------------------------------------------------


    Dim i As Integer        ' Counter variable.
    
    ' Application starts here (Load event of Startup form).
    Show
    ' Always set the working directory to the directory containing the application.
    ChDir App.Path
    FState.Dirty = False
    
    ' Set public variable gFindDirection which determines which direction
    ' the FindIt function will search in.
    gFindDirection = 1
        
    ' Assign the name of the first font to a font
    ' menu entry, then loop through the fonts
    ' collection, adding them to the menu
    mnuFontName(0).Caption = Screen.Fonts(0)
    For i = 1 To Screen.FontCount - 1
        Load mnuFontName(i)
        mnuFontName(0).Caption = Screen.Fonts(i)
    Next
End Sub

Private Sub Form_QueryUnload(cancel As Integer, unloadmode As Integer)
    Dim strMsg As String
    Dim strFilename As String
    Dim intResponse As Integer

    ' Check to see if the text has been changed.
    If FState.Dirty Then
        strFilename = Me.Caption
        strMsg = "The document [" & strFilename & "] has changed."
        strMsg = strMsg & vbCrLf
        strMsg = strMsg & "Do you want to save the changes?"
        intResponse = MsgBox(strMsg, 51, frmSDI.Caption)
        Select Case intResponse
            Case 6      ' User chose Yes.
                If Right(Me.Caption, 8) = "Untitled" Then
                    ' The file hasn't been saved yet.
                    strFilename = "untitled.vnp"
                Else
                    ' The form's Caption contains the name of the open file.
                    strFilename = Me.Caption
                End If
                ' Call the save procedure. If strFilename = Empty, then
                ' the user chose Cancel in the Save As dialog box; otherwise,
                ' save the file.
                If strFilename <> "" Then
                    SaveFileAs strFilename, True
                End If
            Case 7      ' User chose No. Unload the file.
                cancel = False
            Case 2      ' User chose Cancel. Cancel the unload.
                cancel = True
        End Select
    End If
    
    ' VBA Notepad Integration
    ' ********************************************************************
    ' Make sure we also verify if the VBA Project is saved on un_load
    
    ' If save is cancel from above then just exit
    If cancel = True Then Exit Sub
    
    m_apcInt.QueryUnload cancel, unloadmode
    
    ' ********************************************************************
    ' VBA Notepad Integration
    
End Sub

Private Sub Form_Resize()
    ' Call the resize procedure
    ResizeNote
End Sub

Private Sub imgCopyButton_Click()
    ' Refresh the image.
    imgCopyButton.Refresh
    ' Call the copy procedure
    EditCopyProc
End Sub

Private Sub imgCopyButton_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    ' Show the picture for the down state.
    imgCopyButton.Picture = imgCopyButtonDn.Picture
End Sub

Private Sub imgCopyButton_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    ' If the button is pressed, display the up bitmap when the
    ' mouse is dragged outside the button's area; otherwise
    ' display the down bitmap.
    Select Case Button
    Case 1
        If X <= 0 Or X > imgCopyButton.Width Or Y < 0 Or Y > imgCopyButton.Height Then
            imgCopyButton.Picture = imgCopyButtonUp.Picture
        Else
            imgCopyButton.Picture = imgCopyButtonDn.Picture
        End If
    End Select
End Sub

Private Sub imgCopyButton_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    ' Show the picture for the up state.
    imgCopyButton.Picture = imgCopyButtonUp.Picture
End Sub

Private Sub imgCutButton_Click()
    ' Refresh the image.
    imgCutButton.Refresh
    ' Call the cut procedure
    EditCutProc
End Sub

Private Sub imgCutButton_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    ' Show the picture for the down state.
    imgCutButton.Picture = imgCutButtonDn.Picture
End Sub

Private Sub imgCutButton_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    ' If the button is pressed, display the up bitmap when the
    ' mouse is dragged outside the button's area; otherwise,
    ' display the down bitmap.
    Select Case Button
    Case 1
        If X <= 0 Or X > imgCutButton.Width Or Y < 0 Or Y > imgCutButton.Height Then
            imgCutButton.Picture = imgCutButtonUp.Picture
        Else
            imgCutButton.Picture = imgCutButtonDn.Picture
        End If
    End Select
End Sub

Private Sub imgCutButton_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    ' Show the picture for the up state.
    imgCutButton.Picture = imgCutButtonUp.Picture
End Sub

Private Sub imgFileNewButton_Click()
    ' Refresh the image.
    imgFileNewButton.Refresh
    ' Call the new file procedure
    FileNew
End Sub

Private Sub imgFileNewButton_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    ' Show the picture for the down state.
    imgFileNewButton.Picture = imgFileNewButtonDn.Picture
End Sub

Private Sub imgFileNewButton_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    ' If the button is pressed, display the up bitmap when the
    ' mouse is dragged outside the button's area; otherwise,
    ' display the down bitmap.
    Select Case Button
    Case 1
        If X <= 0 Or X > imgFileNewButton.Width Or Y < 0 Or Y > imgFileNewButton.Height Then
            imgFileNewButton.Picture = imgFileNewButtonUp.Picture
        Else
            imgFileNewButton.Picture = imgFileNewButtonDn.Picture
        End If
    End Select
End Sub

Private Sub imgFileNewButton_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    ' Show the picture for the up state.
    imgFileNewButton.Picture = imgFileNewButtonUp.Picture
End Sub

Private Sub imgFileOpenButton_Click()
    ' Refresh the image.
    imgFileOpenButton.Refresh
    ' Call the file open procedure
    FileOpenProc
End Sub

Private Sub imgFileOpenButton_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    ' Show the picture for the down state.
    imgFileOpenButton.Picture = imgFileOpenButtonDn.Picture
End Sub

Private Sub imgFileOpenButton_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    ' If the button is pressed, display the up bitmap when the
    ' mouse is dragged outside the button's area; otherwise,
    ' display the down bitmap.
    Select Case Button
    Case 1
        If X <= 0 Or X > imgFileOpenButton.Width Or Y < 0 Or Y > imgFileOpenButton.Height Then
            imgFileOpenButton.Picture = imgFileOpenButtonUp.Picture
        Else
            imgFileOpenButton.Picture = imgFileOpenButtonDn.Picture
        End If
    End Select
End Sub

Private Sub imgFileOpenButton_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    ' Show the picture for the up state.
    imgFileOpenButton.Picture = imgFileOpenButtonUp.Picture

End Sub

Private Sub imgPasteButton_Click()
    ' Refresh the image.
    imgPasteButton.Refresh
    ' Call the paste procedure
    EditPasteProc
End Sub

Private Sub imgPasteButton_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    ' Show the picture for the down state.
    imgPasteButton.Picture = imgPasteButtonDn.Picture
End Sub

Private Sub imgPasteButton_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    ' If the button is pressed, display the up bitmap when the
    ' mouse is dragged outside the button's area; otherwise,
    ' display the down bitmap.
    Select Case Button
    Case 1
        If X <= 0 Or X > imgPasteButton.Width Or Y < 0 Or Y > imgPasteButton.Height Then
            imgPasteButton.Picture = imgPasteButtonUp.Picture
        Else
            imgPasteButton.Picture = imgPasteButtonDn.Picture
        End If
    End Select
End Sub

Private Sub imgPasteButton_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    ' Show the picture for the up state.
    imgPasteButton.Picture = imgPasteButtonUp.Picture
End Sub

Private Sub mnuEditCopy_Click()
    ' Call the copy procedure
    EditCopyProc
End Sub

Private Sub mnuEditCut_Click()
    ' Call the cut procedure
    EditCutProc
End Sub

Private Sub mnuEditDelete_Click()
' If the mouse pointer is not at the end of the notepad...
On Local Error GoTo ExitSub
    If txtNote.SelStart <> Len(Screen.ActiveControl.Text) Then
        ' If nothing is selected, extend the selection by one.
        If txtNote.SelLength = 0 Then
            txtNote.SelLength = 1
            ' If the mouse pointer is on a blank line, extend the selection by two.
            If Asc(txtNote.SelText) = 13 Then
                txtNote.SelLength = 2
            End If
        End If
        ' Delete the selected text.
        txtNote.SelText = ""
    End If
ExitSub:
End Sub

Private Sub mnuEditPaste_Click()
    ' Call the paste procedure.
    EditPasteProc
End Sub

Private Sub mnuEditSelectAll_Click()
    ' Use SelStart & SelLength to select the text.
     On Local Error GoTo ErrHandler
    txtNote.SelStart = 0
    txtNote.SelLength = Len(txtNote.Text)
    Exit Sub
ErrHandler:
    MsgBox "Unable to select text. Text too long."
    
End Sub

Private Sub mnuEditTime_Click()
    ' Insert the current time and date.
    txtNote.SelText = Now
End Sub

Private Sub mnuFileExit_Click()
    ' End the application.
        
    Unload Me
End Sub

Public Sub mnuFileNew_Click()
    
    ' Call the new form procedure
    FileNew
End Sub

Private Sub mnuFileOpen_Click()
    
    
    ' Call the file open procedure.
    FileOpenProc
    
    
End Sub

Private Sub mnuFileSave_Click()
    'Call the file save procedure
    FileSave True
End Sub

Private Sub mnuFileSaveAs_Click()
    Dim strSaveFileName As String
    Dim strDefaultName As String
    
    ' Assign the form caption to the variable.
    strDefaultName = Right$(Me.Caption, Len(Me.Caption) - 14)
    If Me.Caption = "SDI NotePad - Untitled" Then
        ' The file hasn't been saved yet.
        ' Get the filename, and then call the save procedure, strSaveFileName.
        
        ' VBA Notepad Integration
        ' *********************************************************************************
        ' Use one standard open dialog
        
        strSaveFileName = frmSDI.m_apcInt.FileOpenSave(OFN_OVERWRITEPROMPT, CurDir$, m_apcInt.gstrFilter, , ".vnp", , App.Title, Me.hwnd, False)  'FS
        
        
        ' *********************************************************************************
        ' VBA Notepad Integration
        
        
        If strSaveFileName <> "" Then SaveFileAs strSaveFileName, False
    
    Else
        ' The form's Caption contains the name of the open file.
        strSaveFileName = frmSDI.m_apcInt.FileOpenSave(OFN_OVERWRITEPROMPT, CurDir$, m_apcInt.gstrFilter, , ".vnp", , App.Title, Me.hwnd, False)  'FS
        If strSaveFileName <> "" Then SaveFileAs strSaveFileName, False
    
    End If
End Sub

Private Sub mnuFontName_Click(Index As Integer)
    ' Assign the selected font to the textbox fontname property.
    txtNote.FontName = mnuFontName(Index).Caption
End Sub

Private Sub mnuOptions_Click()
    ' Toggle the Checked property to match the .Visible property.
    mnuOptionsToolbar.Checked = picToolbar.Visible
End Sub

Private Sub mnuOptionsLaunch_Click()
    Dim strApp As String
    
    ' Shell a new instance of the notepad.
    strApp = App.Path & "\" & App.EXEName
    Shell strApp, 1
End Sub

Private Sub mnuOptionsToolbar_Click()
    ' Toggle the visible property of the toolbar
    picToolbar.Visible = Not picToolbar.Visible
    ' Change the check to match the current state
    mnuOptionsToolbar.Checked = picToolbar.Visible
    ' Call the resize procedure
    ResizeNote
End Sub



Private Sub mnuSearchFind_Click()
    ' If there is text in the textbox, assign it to
    ' the textbox on the Find form, otherwise assign
    ' the last findtext value.
    If txtNote.SelText <> "" Then
        frmFind.txtFind.Text = txtNote.SelText
    Else
        frmFind.txtFind.Text = gFindString
    End If
    ' Set the public variable to start at the beginning.
    gFirstTime = True
    ' Set the case checkbox to match the public variable
    If (gFindCase) Then
        frmFind.chkCase = 1
    End If
    ' Display the Find form.
    frmFind.Show vbModal
End Sub

Private Sub mnuSearchFindNext_Click()
    ' If the public variable isn't empty, call the
    ' find procedure, otherwise call the find menu
    If Len(gFindString) > 0 Then
        FindIt
    Else
        mnuSearchFind_Click
    End If
End Sub



Private Sub txtNote_Change()
    ' Set the public variable to show that text has changed.
    FState.Dirty = True
End Sub





