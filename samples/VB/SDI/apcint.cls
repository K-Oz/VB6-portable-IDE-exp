VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "APCIntegration"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
' ************************************************************************
'   Copyright (C) Microsoft Corporation, 1999.  All rights reserved.
'
'   This source code is intended only as a supplement to Microsoft
'   Development Tools and/or on-line documentation.  See these other
'   materials for detailed information regarding Microsoft code samples.
' ************************************************************************


' VBA NotePad Integration
' *****************************************
Public gstrFilter As String                ' Filter to be used for the File/Open/Save dialog across the project
' *****************************************
' VBA NotePad Integration

'--------------------------------------------------
'**** VBA Integration code, begin insert
Private WithEvents m_apcHost As MSAPC.Apc
Attribute m_apcHost.VB_VarHelpID = -1
Private WithEvents m_ApcProject As MSAPC.Project
Attribute m_ApcProject.VB_VarHelpID = -1
Private m_stFileName As String             ' File name used by notepad and vba to persist the project
Private Declare Function GetOpenFileName Lib "comdlg32.dll" Alias "GetOpenFileNameA" (ofn As OPENFILENAME) As Boolean
Private Declare Function GetSaveFileName Lib "comdlg32.dll" Alias "GetSaveFileNameA" (ofn As OPENFILENAME) As Boolean
Private Type OPENFILENAME
    lStructSize As Long
    hwndOwner As Long
    hInstance As Long
    stFilter As String
    stCustomFilter As String
    nMaxCustFilter As Long
    nFilterIndex As Long
    strFile As String
    nMaxFile As Long
    stFileTitle As String
    nMaxFileTitle As Long
    stInitialDir As String
    strTitle As String
    flags As Long
    nFileOffset As Integer
    nFileExtension As Integer
    stDefExt As String
    lCustData As Long
    lpfnHook As Long
    lpTemplateName As String
End Type

Public Enum OFN_FLAGS
    OFN_READONLY = &H1
    OFN_OVERWRITEPROMPT = &H2
    OFN_HIDEREADONLY = &H4
    OFN_NOCHANGEDIR = &H8
    OFN_SHOWHELP = &H10
    OFN_ENABLEHOOK = &H20
    OFN_ENABLETEMPLATE = &H40
    OFN_ENABLETEMPLATEHANDLE = &H80
    OFN_NOVALIDATE = &H100
    OFN_ALLOWMULTISELECT = &H200
    OFN_EXTENSIONDIFFERENT = &H400
    OFN_PATHMUSTEXIST = &H800
    OFN_FILEMUSTEXIST = &H1000
    OFN_CREATEPROMPT = &H2000
    OFN_SHAREAWARE = &H4000
    OFN_NOREADONLYRETURN = &H8000
    OFN_NOTESTFILECREATE = &H10000
    OFN_NONETWORKBUTTON = &H20000
    OFN_NOLONGNAMES = &H40000
    OFN_EXPLORER = &H80000
    OFN_NODEREFERENCELINKS = &H100000
    OFN_LONGNAMES = &H200000
    End Enum
'**** VBA Integration code, end insert
'--------------------------------------------------



'--------------------------------------------------
'**** VBA Integration code, begin insert
Private Sub Class_Initialize()
    Set m_apcHost = New Apc
    
    ' VBA NotePad Integration
    ' *****************************************
    ' Initialize project file name and open dialog filter
    m_stFileName = ""
    gstrFilter = "VBA Notepad Files (*.vnp)" & Chr$(0) & "*.VNP" & Chr$(0) & "All Files (*.*)" & Chr$(0) & "*.*" & Chr$(0)
    ' *****************************************
    ' VBA NotePad Integration
    
End Sub
'**** VBA Integration code, end insert
'--------------------------------------------------



'--------------------------------------------------
'**** VBA Integration code, begin insert
Public Sub Initialize(appObj As CApplication, hwnd As Long)
    ' VBE must have an hWnd for the host so it
    ' can manage the msg loop, alt+F11, etc.
    m_apcHost.hwnd = hwnd

    ' This ought to be a set statement here, not a direct
       ' assignment, but the Set will fail.
    m_apcHost.ApplicationObject = appObj
    m_apcHost.HostName = App.Title

    ' This is an evaluation key.
    m_apcHost.LicenseKey = "16175148714896599659AFABD8ED3C2A416B45E4CD6F5484BD8CED"

    ' The registry key where IDE customizations will be stored
    m_apcHost.RegistryKey = "Common"

    ' The locale of the VBA IDE for the application
    m_apcHost.Locale = 1033

    ' File filter for the VBA references dialog
    m_apcHost.FileFilter = "All Reference Files (*.olb, *.tlb, *.dll, *.exe, *.ocx)" & Chr$(0) & "*.olb;*.tlb;*.dll;*.exe;*.ocx" & Chr$(0)

    ' VBA NotePad Integration
    ' ***************************************************************
    ' Add the notepad icon to the VBA toolbar
    m_apcHost.Picture = LoadPicture(App.Path & "\np.bmp")
    m_apcHost.BackColor = RGB(255, 0, 255)
    
    ' Create a default project
    Set m_ApcProject = m_apcHost.Projects.Add(axProjectNormal, "VBAProject")
    
    ' ***************************************************************
    ' VBA NotePad Integration
    
            
    ' Set the project's dirty flag to False so no save prompt will occur unless user modifications are made
     m_ApcProject.Dirty = False
End Sub

'**** VBA Integration code, end insert
'--------------------------------------------------



'--------------------------------------------------
'**** VBA Integration code, begin insert
Private Sub m_ApcProject_Save()
    SaveProj
End Sub
'**** VBA Integration code, end insert
'--------------------------------------------------



'--------------------------------------------------
'**** VBA Integration code, begin insert
Public Function FileOpenSave(Optional ByRef flags As Long = 0&, Optional ByVal InitialDir As Variant, Optional ByVal Filter As String = vbNullString, Optional ByVal FilterIndex As Long = 1, Optional ByVal DefaultExt As String = vbNullString, Optional ByVal FileName As String = vbNullString, Optional ByVal DialogTitle As String = vbNullString, Optional ByVal hwnd As Long = -1, Optional ByVal OpenFile As Boolean = True) As String
    Dim ofn As OPENFILENAME
    Dim stFileName As String
    Dim stFileTitle As String
    Dim fResult As Boolean

    ' Give the dialog a caption title.
    If IsMissing(InitialDir) Then InitialDir = CurDir
    If (hwnd = -1) Then hwnd = 0


    ' Allocate string space for the returned strings.
    stFileName = Left$(FileName & String$(256, vbNullChar), 256)
    stFileTitle = String$(256, vbNullChar)

    With ofn
        .lStructSize = Len(ofn)
        .hwndOwner = hwnd
        .stFilter = Filter
        .nFilterIndex = FilterIndex
        .strFile = stFileName
        .nMaxFile = Len(stFileName)
        .stFileTitle = stFileTitle
        .nMaxFileTitle = Len(stFileTitle)
        .strTitle = DialogTitle
        .flags = flags
        .stDefExt = DefaultExt
        .stInitialDir = InitialDir
        .hInstance = 0
        .stCustomFilter = String$(255, vbNullChar)
        .nMaxCustFilter = 255
        .lpfnHook = 0
    End With

    If OpenFile Then
        fResult = GetOpenFileName(ofn)
    Else
        fResult = GetSaveFileName(ofn)
    End If

    If fResult Then
        flags = ofn.flags
        FileOpenSave = Left$(ofn.strFile, InStr(1, ofn.strFile, vbNullChar, vbBinaryCompare) - 1)
    End If
End Function
'**** VBA Integration code, end insert
'--------------------------------------------------



'--------------------------------------------------
'**** VBA Integration code, begin insert
Public Sub showVBE()
    m_apcHost.Ide.Visible = True
End Sub '**** VBA Integration code, end insert
'--------------------------------------------------



'--------------------------------------------------
'**** VBA Integration code, begin insert
Public Sub showMacroDialog()
    If Not (m_apcHost.ExecutingProject Is Nothing) Then
        ' If there is an executing project, use it
        m_apcHost.ExecutingProject.CreateScopes.MacrosDialog.Show
    ElseIf (m_apcHost.Projects.Count) > 0 Then
        ' Only do something if there is some project
        m_apcHost.Projects(1).CreateScopes.MacrosDialog.Show
    End If
End Sub '**** VBA Integration code, end insert
'--------------------------------------------------
Public Sub QueryUnload(cancel As Integer, unloadmode As Integer, Optional AskSaveChanges As Boolean = True)
    ' make sure APC has been initialized
    If m_apcHost Is Nothing Then Exit Sub

    ' Allow the user to cancel from a VBA Stand Alone Project
    If Not m_apcHost.CanTerminate Then
        cancel = True
        Exit Sub
    End If

    ' ask the user if they would like to save changes to the VBA Project
    If AskSaveChanges And Not (m_ApcProject Is Nothing) Then
        If m_ApcProject.Dirty Then
            Dim msgResult As VbMsgBoxResult

            msgResult = MsgBox("Save changes to  " & m_ApcProject.Name & "?", vbYesNoCancel, App.Title)

            If msgResult = vbYes Then
                SaveProj
            ElseIf msgResult = vbCancel Then
                cancel = True
            End If
        End If
    End If

    ' Stop execution of VBA code
    If Not cancel Then m_apcHost.End
    
End Sub

'--------------------------------------------------
'**** VBA Integration code, begin insert
Friend Property Get ApcHost() As MSAPC.Apc
     Set ApcHost = m_apcHost
 End Property
'**** VBA Integration code, end insert
'--------------------------------------------------



'--------------------------------------------------
'**** VBA Integration code, begin insert
Friend Property Get ApcProject() As MSAPC.Project
     Set ApcProject = m_ApcProject
End Property
'**** VBA Integration code, end insert
'--------------------------------------------------



'--------------------------------------------------
'**** VBA Integration code, begin insert
    ' This proc is not hooked up anywhere but shows
    ' how to load up a VBA project. You can hook
    ' it up to a File|Open... menu item, etc.
Friend Sub OpenProj()

    Dim stg As MSAPC.Storage
    Dim stm As MSAPC.Stream ' VBA Notepad Integration : Stream used for the Text of the notepad
    Dim bDefaultProjectClosed As Boolean ' VBA Notepad Integration : This flag is used to identify that
    ' a project needs to be closed before opening a new open.
    
    Dim stFilter As String
    Dim stFileName As String

    On Local Error GoTo ErrHandler ' VBA Notepad Integration : Error handler to take care of openning non .vnp files
    
    bDefaultProjectClosed = False ' VBA Notepad Integration : Set it to false by default
    
    If Not (m_ApcProject Is Nothing) And Not (m_apcHost Is Nothing) Then
        If Len(m_stFileName) > 0 Then
            If m_ApcProject.Dirty Then Me.SaveProj
            If m_ApcProject.Dirty Then Exit Sub
            m_ApcProject.Close
            
            bDefaultProjectClosed = True ' VBA Notepad Integration : No need to close the project because we already did that.
            
            Set m_ApcProject = Nothing
            m_stFileName = vbNullString
        End If

        stFileName = FileOpenSave(OFN_NOCHANGEDIR, CurDir$, gstrFilter, , ".vnp", , , m_apcHost.hwnd, True)
                
        ' VBA Notepad Integration
        ' ****************************************************************
        ' Make sure we have a file name before closing the default project if we need to
        If m_stFileName = "" And bDefaultProjectClosed = False And stFileName <> "" Then
            m_ApcProject.Close False ' Close default project
            bDefaultProjectClosed = True ' Flag that we have a close file so that incase we error out we can recover
        End If
        ' ****************************************************************
        ' VBA Notepad Integration
        
        If Len(stFileName) > 0 Then
            
            Set stg = OpenStorage(stFileName)
        
            ' VBA Notepad Integration
            ' ****************************************************************
            Set m_ApcProject = m_apcHost.Projects.Open(axProjectNormal + axProjectThrowAwayCompiledState, stg)
            
            'Also need to get the PlainTextFrom Storage which contains the text for the notepad
            
            Set stm = stg.OpenStream("PlainTextStream")
            frmSDI.txtNote.Text = stm.Get
            Set stm = stg.OpenStream("BackColor")
            frmSDI.txtNote.BackColor = stm.Get
            Set stm = stg.OpenStream("ForeColor")
            frmSDI.txtNote.ForeColor = stm.Get
            
            ' ****************************************************************
            ' VBA Notepad Integration
            m_stFileName = stFileName
            
        End If
    End If
    
    ' VBA Notepad Integration
    ' ****************************************************************
    ' Change the form's caption if we successfully opened a file
    If m_stFileName <> "" Then
        frmSDI.Caption = "SDI NotePad - " & UCase(m_stFileName)
    End If
    
    Exit Sub
ErrHandler:
    MsgBox "Error Openning the Project: " & Err.Description, vbCritical
    ' If the error occured after closing the project then open a empty project
    If bDefaultProjectClosed = True Then
        CreateNewCleanProject False
    End If
    
    ' ****************************************************************
    ' VBA Notepad Integration


End Sub
'**** VBA Integration code, end insert
'--------------------------------------------------



'--------------------------------------------------
'**** VBA Integration code, begin insert
    ' You can hook this proc up to a File|Save menu item
Friend Sub SaveProj(Optional ByVal stFileName As String)
    Dim stFilter As String
    Dim stg As MSAPC.Storage
    Dim stm As MSAPC.Stream ' VBA Notepad Integration : Stream used for the Text of the notepad
    Dim hwnd As Long

    If Not (m_ApcProject Is Nothing) And Not (m_apcHost Is Nothing) Then
        If Len(stFileName) = 0 Then
            If Len(m_stFileName) = 0 Then
                stFileName = FileOpenSave(OFN_OVERWRITEPROMPT, CurDir$, gstrFilter, , ".vnp", , App.Title, m_apcHost.hwnd, False) 'FS
            Else
                m_ApcProject.Save
                
                ' VBA Notepad Integration
                ' *****************************************************************
                ' Save the plain text & document settings from the notepad
                Set stm = m_ApcProject.Storage.OpenStream("PlainTextStream")
                stm.Put frmSDI.txtNote.Text
                Set stm = m_ApcProject.Storage.OpenStream("BackColor")
                stm.Put frmSDI.txtNote.BackColor
                Set stm = m_ApcProject.Storage.OpenStream("ForeColor")
                stm.Put frmSDI.txtNote.ForeColor
                ' *****************************************************************
                ' VBA Notepad Integration
                
                Exit Sub
            End If
        End If

        
            
        Set stg = MSAPC.CreateStorage(stFileName)
        m_ApcProject.SaveAs stg
        
        ' VBA Notepad Integration
        ' *****************************************************************
        ' Create a new stream and save the text if a new file
        If m_stFileName = "" Then
            
            Set stm = stg.CreateStream("PlainTextStream")
            stm.Put frmSDI.txtNote.Text
            Set stm = stg.CreateStream("BackColor")
            stm.Put frmSDI.txtNote.BackColor
            Set stm = stg.CreateStream("ForeColor")
            stm.Put frmSDI.txtNote.ForeColor
                
        End If
        ' *****************************************************************
        ' VBA Notepad Integration
        
        m_stFileName = stFileName
    End If
End Sub
'**** VBA Integration code, end insert
'--------------------------------------------------

' VBA NotePad Integration
' ***********************************
' This helper routine is used to create a new clean project
' It is used mostly in the File/New scenarios

Sub CreateNewCleanProject(ByVal bCloseProject As Boolean)

    ' Close old project
    If bCloseProject Then
        m_ApcProject.Close False
    End If
    
    ' Create a new project
    Set m_ApcProject = m_apcHost.Projects.Add(axProjectNormal, "VBAProject")
    
    ' Reset dirty flag
    m_ApcProject.Dirty = False
    
    ' Reset the notepad form
    frmSDI.txtNote.Text = ""
    frmSDI.Caption = "SDI NotePad - Untitled"
    frmSDI.txtNote.ForeColor = &H80000008 'Black
    frmSDI.txtNote.BackColor = &H80000005 'White
    ' Reset file name
    m_stFileName = ""

End Sub

' ***********************************
' VBA NotePad Integration


