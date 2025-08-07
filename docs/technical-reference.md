# Technical Reference

## System Components

### Core Components Overview

```plantuml
@startuml
package "VB6 Portable IDE" {
    component "Main Launcher" as ML {
        port "Entry Point" as EP
        port "Registry Interface" as RI
    }
    
    component "AutoPlay Engine" as AE {
        port "UI Interface" as UI
        port "Media System" as MS
    }
    
    component "VB6 Runtime" as VR {
        port "IDE Interface" as II
        port "Compiler Interface" as CI
        port "Debug Interface" as DI
    }
    
    component "Registry Manager" as RM {
        port "License Manager" as LM
        port "Component Registry" as CR
    }
}

ML::EP --> AE::UI
ML::RI --> RM::LM
AE::MS --> VR::II
VR::CI --> VR::DI
RM::CR --> VR::II
@enduml
```

## File Structure

### Directory Layout

```
VB6-Portable-IDE/
├── Visual Basic 6 Portable.exe          # Main launcher executable
├── Visual Basic 6 Portable/             # Core system directory
│   ├── autorun.exe                       # AutoPlay executable
│   ├── ico.ico                          # Application icon
│   └── AutoPlay/                        # AutoPlay system files
│       ├── Audio/                       # Sound effects
│       │   ├── Click1.ogg              # UI click sound
│       │   └── High1.ogg               # Alert sound
│       ├── Icons/                       # UI icons
│       │   └── ico.ico                  # Interface icons
│       ├── Images/                      # UI graphics
│       │   ├── 12.bmp                   # Background images
│       │   ├── images_1.jpg            # Interface graphics
│       │   ├── algeria.png              # Theme graphics
│       │   ├── ico_alpha_Error_32x32.png # Error icons
│       │   └── images.jpg               # Additional graphics
│       ├── Plugins/                     # Plugin system
│       │   └── SHAPE/                   # Shape plugin
│       │       └── SHAPE.APO           # Plugin object
│       ├── Docs/                        # System documentation
│       │   ├── VbPortable6.reg         # Registry entries
│       │   └── Portable.VB6/           # VB6 runtime files
│       │       ├── Vb6.exe             # VB6 IDE executable
│       │       ├── C2.exe              # C2 compiler
│       │       ├── Link.exe            # Linker utility
│       │       ├── Vb6stkit.dll        # Setup toolkit
│       │       ├── Vb6ext.olb          # VB6 extensions
│       │       ├── Dao350.dll          # Database access
│       │       ├── Mspdb60.dll         # Debug database
│       │       ├── Vbaexe6.lib         # VBA executable lib
│       │       ├── Vb6debug.dll        # Debug runtime
│       │       ├── Vb6.olb             # VB6 object library
│       │       ├── Mrt7enu.dll         # Runtime English
│       │       └── Vba6.dll            # VBA runtime
│       └── autorun.cdd                 # AutoPlay configuration
├── docs/                               # Documentation (this structure)
│   ├── architecture.md                 # System architecture
│   ├── installation.md                # Installation guide
│   ├── user-guide.md                  # User documentation
│   ├── technical-reference.md         # This file
│   └── troubleshooting.md             # Problem resolution
└── README.md                           # Project overview
```

### File Types and Purposes

```mermaid
graph TD
    A[VB6 Portable Files] --> B[Executable Files]
    A --> C[Library Files]
    A --> D[Configuration Files]
    A --> E[Resource Files]
    A --> F[Documentation Files]
    
    B --> B1[.exe - Main executables]
    B --> B2[.dll - Dynamic libraries]
    
    C --> C1[.olb - Object libraries]
    C --> C2[.lib - Static libraries]
    
    D --> D1[.reg - Registry entries]
    D --> D2[.cdd - AutoPlay config]
    
    E --> E1[.ico - Icons]
    E --> E2[.bmp/.jpg/.png - Images]
    E --> E3[.ogg - Audio files]
    
    F --> F1[.md - Documentation]
    F --> F2[README - Project info]
```

## VB6 Runtime Components

### Core VB6 Files

| File | Purpose | Description |
|------|---------|-------------|
| `Vb6.exe` | Main IDE | Visual Basic 6.0 Integrated Development Environment |
| `C2.exe` | Compiler | Native code compiler backend |
| `Link.exe` | Linker | Links compiled objects into executables |
| `Vba6.dll` | VBA Runtime | Visual Basic for Applications runtime |
| `Vb6debug.dll` | Debug Support | Debugging and breakpoint support |
| `Vb6stkit.dll` | Setup Toolkit | Application distribution tools |

### Component Dependencies

```plantuml
@startuml
package "VB6 Core System" {
    [Vb6.exe] as IDE
    [Vba6.dll] as VBA
    [Vb6debug.dll] as Debug
}

package "Compilation System" {
    [C2.exe] as Compiler
    [Link.exe] as Linker
    [Vbaexe6.lib] as VBALib
}

package "Object Libraries" {
    [Vb6.olb] as VB6OLB
    [Vb6ext.olb] as VB6Ext
}

package "Database Support" {
    [Dao350.dll] as DAO
    [Mspdb60.dll] as DebugDB
}

package "Localization" {
    [Mrt7enu.dll] as EnglishRT
}

IDE --> VBA : Runtime Support
IDE --> Debug : Debugging
IDE --> VB6OLB : Type Information
IDE --> VB6Ext : Extensions

Compiler --> Linker : Object Files
Compiler --> VBALib : VBA Linking
Compiler --> DebugDB : Debug Info

IDE --> Compiler : Compilation Requests
IDE --> DAO : Database Access
VBA --> EnglishRT : Localized Resources
@enduml
```

## Registry Configuration

### License Keys Structure

```mermaid
graph TD
    A[HKEY_CLASSES_ROOT\Licenses] --> B[6000720D-F342-11D1-AF65-00A0C90DCA10]
    A --> C[74872840-703A-11d1-A3AF-00A0C90F26FA]
    A --> D[74872841-703A-11d1-A3AF-00A0C90F26FA]
    
    B --> B1[VB6 IDE License]
    C --> C1[Component License 1]
    D --> D1[Component License 2]
    
    B1 --> B2["kefeflhlhlgenelerfleheietfmflelljeqf"]
    C1 --> C2["mninuglgknogtgjnthmnggjgsmrmgniglish"]
    D1 --> D2["klglsejeilmereglrfkleeheqkpkelgejgqf"]
```

### Registry Deployment Sequence

```plantuml
@startuml
participant "VB6 Portable" as VB6P
participant "Windows Registry" as REG
participant "License Manager" as LM

VB6P -> REG : Check existing licenses
REG --> VB6P : License status

alt No licenses found
    VB6P -> LM : Load VbPortable6.reg
    LM -> REG : Apply license keys
    REG --> LM : Keys registered
    LM --> VB6P : License setup complete
    
    VB6P -> REG : Register VB6 components
    REG --> VB6P : Components registered
end

VB6P -> VB6P : Launch AutoPlay system
@enduml
```

## Memory Architecture

### Memory Layout

```mermaid
graph TB
    subgraph "Process Memory Space"
        A[Code Segment] --> A1[VB6 Portable.exe]
        A --> A2[autorun.exe]
        A --> A3[Vb6.exe]
        
        B[Data Segment] --> B1[Global Variables]
        B --> B2[Static Data]
        B --> B3[Initialized Data]
        
        C[Heap] --> C1[Dynamic Objects]
        C --> C2[Form Instances]
        C --> C3[Control Objects]
        
        D[Stack] --> D1[Local Variables]
        D --> D2[Function Calls]
        D --> D3[Return Addresses]
    end
    
    subgraph "Shared Memory"
        E[DLL Code] --> E1[System DLLs]
        E --> E2[VB6 Runtime DLLs]
        E --> E3[Third-party DLLs]
    end
```

### Memory Management Strategy

```plantuml
@startuml
start
:Application Launch;
:Initialize Main Process;
:Load Core DLLs;

fork
    :AutoPlay Interface;
    :Allocate UI Memory;
    :Load Graphics/Audio;
fork again
    :VB6 Runtime;
    :Load VB6 Components;
    :Initialize IDE;
end fork

:User Interaction;

while (IDE Active?) is (Yes)
    :Process User Input;
    :Manage Form Objects;
    :Handle Code Compilation;
    if (Memory Pressure?) then (Yes)
        :Garbage Collection;
        :Release Unused Objects;
    endif
endwhile (No)

:Cleanup Resources;
:Unload DLLs;
:Terminate Process;
stop
@enduml
```

## Security Model

### Permission Requirements

```mermaid
graph TD
    A[VB6 Portable Launch] --> B{First Run?}
    B -->|Yes| C[Requires Admin Rights]
    B -->|No| D[Standard User Rights]
    
    C --> E[Registry Write Access]
    C --> F[System File Access]
    C --> G[Component Registration]
    
    D --> H[Project File Access]
    D --> I[Compilation Operations]
    D --> J[Debug Operations]
    
    E --> K[License Installation]
    F --> K
    G --> K
    
    H --> L[Normal Operation]
    I --> L
    J --> L
    
    K --> L
    L --> M[Development Ready]
```

### Security Boundaries

```plantuml
@startuml
package "System Security Context" {
    [Windows Kernel] as kernel
    [Security Subsystem] as security
}

package "VB6 Portable Process" {
    [Main Process] as main
    [Registry Access] as reg
    [File System Access] as fs
    [Network Access] as net
}

package "User Context" {
    [Project Files] as proj
    [User Data] as data
    [Temp Files] as temp
}

security --> main : Process Token
main --> reg : Registry API
main --> fs : File API
main --> net : Network API (if needed)

fs --> proj : Read/Write Project Files
fs --> data : User Document Access
fs --> temp : Temporary File Creation

kernel --> security : Security Policy Enforcement
@enduml
```

## Performance Characteristics

### Startup Performance

```mermaid
gantt
    title VB6 Portable Startup Performance
    dateFormat X
    axisFormat %s
    
    section Process Launch
    Executable Load       :done, load, 0, 500ms
    DLL Resolution       :done, dll, after load, 300ms
    Registry Check       :done, reg, after dll, 200ms
    
    section AutoPlay Init
    Interface Loading    :done, ui, after reg, 400ms
    Resource Loading     :done, res, after ui, 300ms
    Audio System        :done, audio, after res, 100ms
    
    section VB6 Launch
    IDE Initialization   :active, ide, after audio, 800ms
    Component Loading    :comp, after ide, 600ms
    Ready State         :milestone, after comp, 0s
```

### Memory Usage Patterns

```mermaid
graph LR
    A[Startup: 20MB] --> B[AutoPlay: 35MB]
    B --> C[IDE Launch: 80MB]
    C --> D[Project Load: 120MB]
    D --> E[Active Development: 150-300MB]
    E --> F[Compilation: Peak +50MB]
    F --> G[Debug Session: Peak +100MB]
    
    H[Memory Cleanup] --> I[Idle: 80MB]
    G --> H
    E --> H
```

## API Reference

### Main Launcher API

```plantuml
@startuml
class VB6PortableLauncher {
    +Initialize() : Boolean
    +CheckFirstRun() : Boolean
    +SetupRegistry() : Boolean
    +LaunchAutoPlay() : Boolean
    +Cleanup() : Boolean
    -ValidateLicenses() : Boolean
    -RegisterComponents() : Boolean
}

class RegistryManager {
    +InstallLicenses() : Boolean
    +RegisterVB6Components() : Boolean
    +CleanupRegistry() : Boolean
    +ValidateInstallation() : Boolean
}

class AutoPlayEngine {
    +Initialize() : Boolean
    +LoadInterface() : Boolean
    +ProcessUserInput() : Integer
    +LaunchVB6IDE() : Boolean
    +ShowDocumentation() : Boolean
}

VB6PortableLauncher --> RegistryManager : uses
VB6PortableLauncher --> AutoPlayEngine : launches
@enduml
```

### Error Codes

| Code | Category | Description |
|------|----------|-------------|
| 0 | Success | Operation completed successfully |
| 1001 | Registry | Failed to access registry |
| 1002 | Registry | License installation failed |
| 1003 | Registry | Component registration failed |
| 2001 | File System | Missing core files |
| 2002 | File System | Permission denied |
| 2003 | File System | Disk space insufficient |
| 3001 | VB6 Runtime | VB6.exe not found |
| 3002 | VB6 Runtime | DLL load failure |
| 3003 | VB6 Runtime | Incompatible version |
| 4001 | AutoPlay | Interface load failure |
| 4002 | AutoPlay | Resource missing |
| 4003 | AutoPlay | Audio system failure |

## Configuration Options

### AutoPlay Configuration

```mermaid
graph TD
    A[autorun.cdd] --> B[Interface Settings]
    A --> C[Media Configuration]  
    A --> D[Plugin Settings]
    A --> E[Behavior Options]
    
    B --> B1[Window Size]
    B --> B2[Theme Selection]
    B --> B3[Button Layout]
    
    C --> C1[Audio Enabled]
    C --> C2[Graphics Quality]
    C --> C3[Animation Speed]
    
    D --> D1[Plugin Paths]
    D --> D2[Plugin Enabled]
    D --> D3[Plugin Settings]
    
    E --> E1[Auto-launch VB6]
    E --> E2[Remember Position]
    E --> E3[Exit Behavior]
```

### VB6 IDE Configuration

VB6 IDE settings are managed through the standard VB6 configuration system:

- **Editor Settings**: Stored in registry under VB6 IDE keys
- **Window Layouts**: Saved with user profile
- **Compiler Options**: Per-project settings in .vbp files
- **Debug Settings**: Global and project-specific options

## Deployment Scenarios

### Single User Deployment

```plantuml
@startuml
participant "Installer" as I
participant "File System" as FS
participant "Registry" as R
participant "VB6 Portable" as VB6

I -> FS : Extract files to target directory
FS --> I : Extraction complete

I -> VB6 : First launch (as Administrator)
VB6 -> R : Install licenses and register components
R --> VB6 : Registration complete
VB6 --> I : Setup complete

note right of VB6 : Subsequent launches\nrun as standard user
@enduml
```

### Multi-user Deployment

```plantuml
@startuml
package "Network Share" {
    [Master Installation] as master
}

package "Workstation 1" {
    [Local Copy] as local1
    [User Registry 1] as reg1
}

package "Workstation 2" {
    [Local Copy] as local2
    [User Registry 2] as reg2
}

package "Workstation N" {
    [Local Copy] as localN
    [User Registry N] as regN
}

master --> local1 : Deploy Files
master --> local2 : Deploy Files  
master --> localN : Deploy Files

local1 --> reg1 : Configure Registry
local2 --> reg2 : Configure Registry
localN --> regN : Configure Registry

note bottom : Each workstation requires\nindividual registry setup
@enduml
```

## Extensibility

### Plugin Architecture

```plantuml
@startuml
package "Plugin System" {
    interface "IPlugin" as IP {
        +Initialize() : Boolean
        +Execute() : Integer
        +Cleanup() : Boolean
        +GetInfo() : String
    }
    
    class "PluginManager" as PM {
        +LoadPlugins() : Boolean
        +ExecutePlugin(name: String) : Integer
        +UnloadPlugins() : Boolean
        -plugins: Collection
    }
    
    class "ShapePlugin" as SP {
        +Initialize() : Boolean
        +Execute() : Integer
        +Cleanup() : Boolean
        +GetInfo() : String
    }
}

IP <|-- SP
PM --> IP : manages
PM --> SP : loads

note right of SP : Example plugin:\nSHAPE.APO
@enduml
```

## Troubleshooting Reference

### Diagnostic Commands

```mermaid
flowchart TD
    A[Issue Detected] --> B{Issue Type?}
    B -->|Startup Failure| C[Check Registry]
    B -->|VB6 Won't Launch| D[Verify VB6 Files]
    B -->|AutoPlay Issues| E[Check AutoPlay Config]
    B -->|Performance Issues| F[Monitor Resources]
    
    C --> C1[Run regedit.exe]
    C --> C2[Check HKCR\Licenses]
    
    D --> D1[Verify Portable.VB6 directory]
    D --> D2[Check DLL dependencies]
    
    E --> E1[Examine autorun.cdd]
    E --> E2[Test audio/graphics]
    
    F --> F1[Task Manager monitoring]
    F --> F2[Performance counters]
```

This technical reference provides comprehensive details for system administrators, developers, and advanced users working with the VB6 Portable IDE system.