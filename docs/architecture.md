# System Architecture

## Overview

The VB6 Portable IDE is designed as a self-contained development environment that provides all necessary components for Visual Basic 6.0 development without requiring system-wide installation.

## Architecture Diagram

```mermaid
graph TD
    A[User] --> B[Visual Basic 6 Portable.exe]
    B --> C{First Run?}
    C -->|Yes| D[Registry Setup]
    C -->|No| E[Launch AutoPlay]
    D --> E
    E --> F[AutoPlay Interface]
    F --> G[VB6 IDE Launch]
    G --> H[VB6 Development Environment]
    
    H --> I[Form Designer]
    H --> J[Code Editor]
    H --> K[Project Explorer]
    H --> L[Debug Tools]
    
    M[Portable.VB6 Directory] --> N[Runtime DLLs]
    M --> O[Compiler Tools]
    M --> P[Debug Components]
    
    Q[AutoPlay System] --> R[Audio Assets]
    Q --> S[Visual Assets]
    Q --> T[Plugin System]
    
    B -.-> M
    B -.-> Q
```

## Component Architecture

```plantuml
@startuml
!define RECTANGLE class

package "VB6 Portable IDE System" {
    RECTANGLE "Main Launcher" as launcher {
        +execute()
        +checkFirstRun()
        +initializeRegistry()
    }
    
    RECTANGLE "AutoPlay Engine" as autoplay {
        +loadInterface()
        +handleUserInput()
        +launchVB6()
    }
    
    RECTANGLE "VB6 Runtime" as runtime {
        +loadComponents()
        +initializeIDE()
        +manageMemory()
    }
    
    RECTANGLE "Registry Manager" as registry {
        +setupLicenses()
        +registerComponents()
        +configureEnvironment()
    }
}

package "VB6 Development Tools" {
    RECTANGLE "Form Designer" as forms {
        +createForms()
        +editControls()
        +setProperties()
    }
    
    RECTANGLE "Code Editor" as editor {
        +syntaxHighlighting()
        +intelliSense()
        +debugging()
    }
    
    RECTANGLE "Compiler" as compiler {
        +compileProject()
        +linkExecutable()
        +generateDebugInfo()
    }
    
    RECTANGLE "Debugger" as debugger {
        +setBreakpoints()
        +stepExecution()
        +inspectVariables()
    }
}

launcher --> autoplay
launcher --> registry
autoplay --> runtime
runtime --> forms
runtime --> editor
runtime --> compiler
runtime --> debugger
@enduml
```

## Data Flow

```mermaid
sequenceDiagram
    participant U as User
    participant L as Launcher
    participant R as Registry
    participant A as AutoPlay
    participant V as VB6 IDE
    
    U->>L: Execute VB6 Portable.exe
    L->>R: Check/Setup Registry
    R-->>L: Registry Ready
    L->>A: Launch AutoPlay Interface
    A-->>U: Display Menu
    U->>A: Select "Start VB6"
    A->>V: Initialize VB6 IDE
    V-->>U: VB6 Development Environment
    
    Note over V: User develops VB6 applications
    
    U->>V: Create/Edit Project
    V->>V: Compile & Debug
    V-->>U: Application Output
```

## System Layers

```mermaid
graph TB
    subgraph "User Interface Layer"
        A[AutoPlay Interface]
        B[VB6 IDE Interface]
    end
    
    subgraph "Application Layer"
        C[Main Launcher]
        D[VB6 Runtime Engine]
        E[Development Tools]
    end
    
    subgraph "System Layer"
        F[Registry Configuration]
        G[File System Access]
        H[Windows API]
    end
    
    subgraph "Data Layer"
        I[VB6 Project Files]
        J[Component Libraries]
        K[Resource Assets]
    end
    
    A --> C
    B --> D
    C --> F
    D --> G
    E --> H
    F --> I
    G --> J
    H --> K
```

## Security Model

```plantuml
@startuml
actor User
participant "VB6 Portable" as VB6P
participant "Windows Registry" as REG
participant "File System" as FS
participant "System Resources" as SYS

User -> VB6P : Execute
activate VB6P

note right of VB6P : Requires Admin privileges\nfor initial registry setup

VB6P -> REG : Setup VB6 Licenses
REG --> VB6P : License Registration Complete

VB6P -> FS : Access Portable Directory
FS --> VB6P : File Access Granted

VB6P -> SYS : Load VB6 Components
SYS --> VB6P : Components Loaded

note right of VB6P : Runs in user context\nafter initial setup

VB6P --> User : VB6 IDE Ready
deactivate VB6P
@enduml
```

## Performance Characteristics

### Memory Usage
- **Base Memory**: ~50MB for launcher and AutoPlay
- **VB6 IDE**: ~100-200MB depending on project size
- **Peak Usage**: ~300MB for large projects with debugging

### Disk Usage
- **Portable Package**: ~100MB total
- **Runtime Footprint**: Minimal temp files
- **Project Storage**: User-defined location

### Startup Time
```mermaid
gantt
    title VB6 Portable Startup Sequence
    dateFormat X
    axisFormat %s
    
    section Initialization
    Launch Executable    :done, launch, 0, 1s
    Registry Check      :done, registry, after launch, 2s
    AutoPlay Loading    :done, autoplay, after registry, 1s
    
    section VB6 Launch
    Component Loading   :active, components, after autoplay, 3s
    IDE Initialization  :ide, after components, 2s
    Ready for Use      :milestone, after ide, 0s
```

## Integration Points

The system integrates with Windows through several key interfaces:

1. **Registry Integration**: License and component registration
2. **File System**: Portable directory structure
3. **Windows API**: GUI and system resource access
4. **Process Management**: Multiple VB6 tool processes

## Scalability Considerations

- **Single User**: Designed for individual developer use
- **Project Size**: Handles small to medium VB6 projects efficiently
- **Resource Scaling**: Memory usage scales with project complexity
- **Concurrent Access**: Limited to single IDE instance per portable installation