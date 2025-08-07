# User Guide

## Getting Started

This guide will help you effectively use the VB6 Portable IDE for Visual Basic 6.0 development.

## Launching the IDE

```mermaid
flowchart TD
    A[Double-click VB6 Portable.exe] --> B[AutoPlay Interface Loads]
    B --> C{Choose Action}
    C -->|Start VB6| D[Launch VB6 IDE]
    C -->|Documentation| E[View Help Files]
    C -->|Settings| F[Configure IDE]
    C -->|Exit| G[Close Application]
    
    D --> H[VB6 Development Environment]
    E --> I[Browse Documentation]
    F --> J[Modify Preferences]
```

## AutoPlay Interface

The AutoPlay interface serves as the main launcher for the VB6 Portable IDE.

### Main Menu Options

```plantuml
@startuml
package "AutoPlay Interface" {
    [Start VB6 IDE] as start
    [View Documentation] as docs
    [Sample Projects] as samples
    [Tools & Utilities] as tools
    [Configuration] as config
    [About & Help] as help
    [Exit] as exit
}

actor User

User --> start : Primary development
User --> docs : Reference materials
User --> samples : Example code
User --> tools : Additional utilities
User --> config : Customize settings
User --> help : Support information
User --> exit : Close application
@enduml
```

## VB6 IDE Workflow

### Project Development Cycle

```mermaid
graph TD
    A[New Project] --> B[Design Forms]
    B --> C[Write Code]
    C --> D[Debug & Test]
    D --> E{Ready to Compile?}
    E -->|No| F[Fix Issues]
    F --> C
    E -->|Yes| G[Compile Project]
    G --> H[Test Executable]
    H --> I{Deploy?}
    I -->|Yes| J[Package Application]
    I -->|No| K[Continue Development]
    K --> B
    J --> L[Distribution]
```

### Form Designer Usage

```plantuml
@startuml
participant "Developer" as dev
participant "Form Designer" as fd
participant "Toolbox" as tb
participant "Properties Window" as pw
participant "Code Editor" as ce

dev -> fd : Open new form
fd -> tb : Display controls
dev -> tb : Select control (e.g., Button)
tb -> fd : Add control to form
fd -> pw : Show control properties
dev -> pw : Modify properties (Name, Caption, etc.)
pw -> fd : Update control appearance
dev -> fd : Double-click control
fd -> ce : Open code editor
dev -> ce : Write event handler
ce -> fd : Return to form designer
@enduml
```

## Common Development Tasks

### Creating a New Project

1. **Start VB6 IDE** through AutoPlay interface
2. **Select Project Type**:
   - Standard EXE (most common)
   - ActiveX EXE
   - ActiveX DLL
   - ActiveX Control
3. **Configure Project Properties**
4. **Begin Development**

```mermaid
stateDiagram-v2
    [*] --> ProjectSelection
    ProjectSelection --> StandardEXE
    ProjectSelection --> ActiveXEXE
    ProjectSelection --> ActiveXDLL
    ProjectSelection --> ActiveXControl
    
    StandardEXE --> FormDesign
    ActiveXEXE --> ClassDesign
    ActiveXDLL --> ModuleDesign
    ActiveXControl --> ControlDesign
    
    FormDesign --> CodeWriting
    ClassDesign --> CodeWriting
    ModuleDesign --> CodeWriting
    ControlDesign --> CodeWriting
    
    CodeWriting --> Testing
    Testing --> Debugging
    Debugging --> CodeWriting
    Testing --> Compilation
    Compilation --> [*]
```

### Adding Controls and Components

```plantuml
@startuml
actor Developer
boundary "Toolbox" as TB
control "Component Dialog" as CD
entity "Form" as F
database "Control Library" as CL

Developer -> TB : Browse available controls
TB -> Developer : Display standard controls
Developer -> TB : Right-click for more components
TB -> CD : Open Components dialog
CD -> CL : List available components
Developer -> CD : Select additional components
CD -> TB : Add to toolbox
TB -> Developer : Updated control palette
Developer -> TB : Drag control to form
TB -> F : Add control instance
@enduml
```

### Debugging Workflow

```mermaid
sequenceDiagram
    participant D as Developer
    participant IDE as VB6 IDE
    participant DB as Debugger
    participant APP as Application
    
    D->>IDE: Set breakpoints
    D->>IDE: Start debugging (F5)
    IDE->>DB: Initialize debug session
    DB->>APP: Launch with debug info
    APP->>DB: Hit breakpoint
    DB->>IDE: Pause execution
    IDE->>D: Show current state
    
    loop Debug Session
        D->>IDE: Inspect variables
        D->>IDE: Step through code
        IDE->>DB: Execute debug commands
        DB->>APP: Control execution
        APP->>DB: Return state
        DB->>IDE: Update display
    end
    
    D->>IDE: Continue/Stop debugging
    IDE->>DB: End debug session
    DB->>APP: Terminate or continue
```

## Advanced Features

### Project Management

```mermaid
graph TB
    A[Project Explorer] --> B[Forms]
    A --> C[Modules]
    A --> D[Class Modules]
    A --> E[User Controls]
    A --> F[References]
    A --> G[Resources]
    
    B --> B1[Form1.frm]
    B --> B2[Form2.frm]
    
    C --> C1[Module1.bas]
    C --> C2[Utilities.bas]
    
    D --> D1[Class1.cls]
    D --> D2[DataHandler.cls]
    
    F --> F1[VB Runtime]
    F --> F2[External DLLs]
    F --> F3[COM Components]
```

### Code Editor Features

The VB6 IDE includes several productivity features:

#### IntelliSense Support

```plantuml
@startuml
participant "Developer" as dev
participant "Code Editor" as editor
participant "IntelliSense" as intel
participant "Object Browser" as ob

dev -> editor : Type object name
editor -> intel : Request completions
intel -> ob : Query available members
ob -> intel : Return member list
intel -> editor : Display dropdown
editor -> dev : Show available options
dev -> editor : Select completion
editor -> editor : Insert completed code
@enduml
```

#### Syntax Highlighting

- **Keywords**: Blue text (Dim, If, For, etc.)
- **Comments**: Green text (lines starting with ')
- **Strings**: Red text (quoted text)
- **Errors**: Red underline for syntax errors

### Compilation Options

```mermaid
flowchart TD
    A[Compile Menu] --> B{Compile Type}
    B -->|Make EXE| C[Create Executable]
    B -->|Make OCX| D[Create ActiveX Control]
    B -->|Make DLL| E[Create DLL Library]
    
    C --> F[Optimization Settings]
    D --> F
    E --> F
    
    F --> G[Native Code]
    F --> H[P-Code]
    
    G --> I[Fast Code]
    G --> J[Small Code]
    G --> K[Favor Pentium Pro]
    
    H --> L[Interpreted Execution]
    
    I --> M[Generate EXE]
    J --> M
    K --> M
    L --> M
```

## Working with Files and Projects

### File Types in VB6

| Extension | Description | Purpose |
|-----------|-------------|---------|
| `.vbp` | Project File | Main project definition |
| `.frm` | Form File | User interface forms |
| `.bas` | Module File | Code modules |
| `.cls` | Class Module | Object-oriented classes |
| `.ctl` | User Control | Custom controls |
| `.exe` | Executable | Compiled application |
| `.ocx` | ActiveX Control | Compiled ActiveX component |
| `.dll` | Dynamic Library | Compiled DLL component |

### Project Structure Best Practices

```mermaid
graph TD
    A[Project Root] --> B[Source Files]
    A --> C[Resources]
    A --> D[Documentation]
    A --> E[Build Output]
    
    B --> F[Forms Directory]
    B --> G[Modules Directory]
    B --> H[Classes Directory]
    
    C --> I[Images]
    C --> J[Icons]
    C --> K[Help Files]
    
    D --> L[ReadMe.txt]
    D --> M[Change Log]
    D --> N[API Documentation]
    
    E --> O[Debug Build]
    E --> P[Release Build]
```

## Tips and Tricks

### Keyboard Shortcuts

| Shortcut | Action |
|----------|---------|
| `F5` | Run/Start Debugging |
| `Ctrl+Break` | Stop Execution |
| `F8` | Step Into |
| `Shift+F8` | Step Over |
| `Ctrl+Shift+F8` | Step Out |
| `F9` | Toggle Breakpoint |
| `Ctrl+G` | Go to Line |
| `Ctrl+F` | Find |
| `Ctrl+H` | Replace |
| `F2` | Object Browser |

### Performance Optimization

```plantuml
@startuml
start
:Write VB6 Code;
:Profile Performance;
if (Performance OK?) then (yes)
    :Code Complete;
    stop
else (no)
    :Identify Bottlenecks;
    if (UI Related?) then (yes)
        :Optimize Form Loading;
        :Reduce Control Count;
        :Use DoEvents Sparingly;
    else (no)
        if (Algorithm Related?) then (yes)
            :Optimize Loops;
            :Use Efficient Data Structures;
            :Cache Calculations;
        else (no)
            :Optimize Database Access;
            :Reduce API Calls;
            :Minimize String Operations;
        endif
    endif
endif
:Retest Performance;
stop
@enduml
```

### Error Handling

```vb
Private Sub Example_ErrorHandling()
    On Error GoTo ErrorHandler
    
    ' Your code here
    Dim result As Integer
    result = 10 / 0  ' This will cause an error
    
    Exit Sub
    
ErrorHandler:
    Select Case Err.Number
        Case 11  ' Division by zero
            MsgBox "Cannot divide by zero!"
        Case Else
            MsgBox "Error " & Err.Number & ": " & Err.Description
    End Select
    Resume Next
End Sub
```

### Memory Management

```mermaid
sequenceDiagram
    participant App as Application
    participant Obj as Object
    participant Mem as Memory Manager
    
    App->>Obj: Create Object (Set obj = New Class)
    Obj->>Mem: Allocate Memory
    Mem-->>Obj: Memory Allocated
    Obj-->>App: Object Reference
    
    Note over App,Obj: Use Object
    
    App->>Obj: Set obj = Nothing
    Obj->>Mem: Request Deallocation
    Mem->>Mem: Garbage Collection
    Mem-->>App: Memory Freed
```

## Troubleshooting Common Issues

### Compilation Errors

```mermaid
flowchart TD
    A[Compilation Error] --> B{Error Type}
    B -->|Syntax Error| C[Check Code Syntax]
    B -->|Missing Reference| D[Add Required References]
    B -->|Type Mismatch| E[Check Variable Types]
    B -->|File Not Found| F[Verify File Paths]
    
    C --> G[Fix Syntax Issues]
    D --> H[Project > References]
    E --> I[Declare Variables Properly]
    F --> J[Update File References]
    
    G --> K[Retry Compilation]
    H --> K
    I --> K
    J --> K
```

### Runtime Errors

Common runtime issues and solutions:

1. **Error 91 - Object variable not set**
   - Ensure objects are properly initialized
   - Check for Nothing references

2. **Error 13 - Type mismatch**
   - Verify data types in assignments
   - Check function parameter types

3. **Error 9 - Subscript out of range**
   - Validate array bounds
   - Check collection indices

### Getting Help

```plantuml
@startuml
actor Developer
boundary "Help System" as HS
control "MSDN Library" as MSDN
control "Online Resources" as OR
control "Community" as COM

Developer -> HS : Press F1 or Help menu
HS -> MSDN : Context-sensitive help
MSDN -> Developer : Display help topic

Developer -> OR : Search online
OR -> Developer : Tutorials and examples

Developer -> COM : Post questions
COM -> Developer : Community answers
@enduml
```

This user guide provides comprehensive coverage of using the VB6 Portable IDE effectively. For specific technical details, refer to the [Technical Reference](technical-reference.md) documentation.