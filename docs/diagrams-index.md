# Diagrams Index

This document provides an index of all Mermaid and PlantUML diagrams included in the VB6 Portable IDE documentation.

## Mermaid Diagrams

### Architecture Diagrams

**System Overview** ([architecture.md](architecture.md))
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
```

**Data Flow** ([architecture.md](architecture.md))
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
```

**System Layers** ([architecture.md](architecture.md))
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
```

### Installation Diagrams

**Installation Process** ([installation.md](installation.md))
```mermaid
flowchart TD
    A[Download VB6 Portable] --> B[Extract to Desired Location]
    B --> C[Run as Administrator]
    C --> D{Registry Setup Required?}
    D -->|Yes| E[Accept UAC Prompt]
    D -->|No| F[Launch AutoPlay]
```

**Configuration States** ([installation.md](installation.md))
```mermaid
stateDiagram-v2
    [*] --> InitialLaunch
    InitialLaunch --> CheckingLicenses
    CheckingLicenses --> LicensesFound : Valid
    CheckingLicenses --> InstallingLicenses : Missing
    InstallingLicenses --> RegisteringComponents
```

### User Guide Diagrams

**Development Workflow** ([user-guide.md](user-guide.md))
```mermaid
graph TD
    A[New Project] --> B[Design Forms]
    B --> C[Write Code]
    C --> D[Debug & Test]
    D --> E{Ready to Compile?}
    E -->|No| F[Fix Issues]
    F --> C
    E -->|Yes| G[Compile Project]
```

**Compilation Options** ([user-guide.md](user-guide.md))
```mermaid
flowchart TD
    A[Compile Menu] --> B{Compile Type}
    B -->|Make EXE| C[Create Executable]
    B -->|Make OCX| D[Create ActiveX Control]
    B -->|Make DLL| E[Create DLL Library]
```

### Technical Reference Diagrams

**File Types Structure** ([technical-reference.md](technical-reference.md))
```mermaid
graph TD
    A[VB6 Portable Files] --> B[Executable Files]
    A --> C[Library Files]
    A --> D[Configuration Files]
    A --> E[Resource Files]
    A --> F[Documentation Files]
```

**Memory Architecture** ([technical-reference.md](technical-reference.md))
```mermaid
graph TB
    subgraph "Process Memory Space"
        A[Code Segment] --> A1[VB6 Portable.exe]
        B[Data Segment] --> B1[Global Variables]
        C[Heap] --> C1[Dynamic Objects]
        D[Stack] --> D1[Local Variables]
    end
```

### Troubleshooting Diagrams

**Startup Issues** ([troubleshooting.md](troubleshooting.md))
```mermaid
flowchart TD
    A[VB6 Portable won't start] --> B{Error Message?}
    B -->|Permission Denied| C[Run as Administrator]
    B -->|File Not Found| D[Check File Integrity]
    B -->|Registry Error| E[Registry Issues]
```

**Performance Diagnosis** ([troubleshooting.md](troubleshooting.md))
```mermaid
flowchart TD
    A[Performance Issues] --> B{Issue Type}
    B -->|Startup Slow| C[Startup Optimization]
    B -->|IDE Slow| D[Runtime Optimization]  
    B -->|Compilation Slow| E[Compiler Optimization]
```

## PlantUML Diagrams

### Component Diagrams

**System Components** ([architecture.md](architecture.md))
```plantuml
@startuml
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
}
@enduml
```

**Component Dependencies** ([technical-reference.md](technical-reference.md))
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
}

IDE --> VBA : Runtime Support
IDE --> Debug : Debugging
@enduml
```

### Sequence Diagrams

**Installation Sequence** ([installation.md](installation.md))
```plantuml
@startuml
actor User
participant "VB6 Portable.exe" as VB6
participant "Windows Registry" as REG
participant "AutoPlay" as AP

User -> VB6 : Launch with elevated privileges
VB6 -> REG : Check for VB6 licenses
REG --> VB6 : License status

alt Licenses not found
    VB6 -> REG : Install VB6 license keys
    REG --> VB6 : Registration complete
end

VB6 -> AP : Launch AutoPlay interface
AP --> User : Display VB6 Portable menu
@enduml
```

**Development Sequence** ([user-guide.md](user-guide.md))
```plantuml
@startuml
participant "Developer" as dev
participant "Form Designer" as fd
participant "Code Editor" as ce

dev -> fd : Open new form
fd -> dev : Display form canvas
dev -> fd : Add controls
fd -> ce : Open code editor
dev -> ce : Write event handlers
@enduml
```

### Use Case Diagrams

**System Usage** ([architecture.md](architecture.md))
```plantuml
@startuml
actor "Developer" as dev
actor "Administrator" as admin

usecase "Launch VB6 IDE" as launch
usecase "Develop Applications" as develop
usecase "Configure System" as config
usecase "Install Licenses" as license

dev --> launch
dev --> develop
admin --> config
admin --> license

launch <-- config
develop <-- launch
license <-- config
@enduml
```

### Deployment Diagrams

**Network Deployment** ([technical-reference.md](technical-reference.md))
```plantuml
@startuml
package "Network Share" {
    [Master Installation] as master
}

package "Workstation 1" {
    [Local Copy] as local1
    [User Registry 1] as reg1
}

master --> local1 : Deploy Files
local1 --> reg1 : Configure Registry
@enduml
```

**Security Model** ([architecture.md](architecture.md))
```plantuml
@startuml
actor User
participant "VB6 Portable" as VB6P
participant "Windows Registry" as REG
participant "File System" as FS

User -> VB6P : Execute
VB6P -> REG : Setup VB6 Licenses
REG --> VB6P : License Registration Complete
VB6P -> FS : Access Portable Directory
FS --> VB6P : File Access Granted
@enduml
```

### Class Diagrams

**API Reference** ([technical-reference.md](technical-reference.md))
```plantuml
@startuml
class VB6PortableLauncher {
    +Initialize() : Boolean
    +CheckFirstRun() : Boolean
    +SetupRegistry() : Boolean
    +LaunchAutoPlay() : Boolean
    +Cleanup() : Boolean
}

class RegistryManager {
    +InstallLicenses() : Boolean
    +RegisterVB6Components() : Boolean
    +CleanupRegistry() : Boolean
}

VB6PortableLauncher --> RegistryManager : uses
@enduml
```

**Plugin Architecture** ([technical-reference.md](technical-reference.md))
```plantuml
@startuml
interface "IPlugin" as IP {
    +Initialize() : Boolean
    +Execute() : Integer
    +Cleanup() : Boolean
}

class "PluginManager" as PM {
    +LoadPlugins() : Boolean
    +ExecutePlugin(name: String) : Integer
}

class "ShapePlugin" as SP {
    +Initialize() : Boolean
    +Execute() : Integer
}

IP <|-- SP
PM --> IP : manages
@enduml
```

## Diagram Usage Guidelines

### For Documentation Writers

1. **Consistency**: Use similar styling and naming conventions across diagrams
2. **Clarity**: Keep diagrams focused on specific aspects of the system
3. **Maintenance**: Update diagrams when system architecture changes
4. **Integration**: Reference diagrams from appropriate documentation sections

### For Developers

1. **Understanding System Flow**: Use sequence diagrams to understand process interactions
2. **Architecture Comprehension**: Reference component diagrams for system structure
3. **Troubleshooting**: Use flowchart diagrams to debug issues systematically
4. **Planning Changes**: Use existing diagrams as templates for documenting modifications

### For System Administrators

1. **Deployment Planning**: Reference deployment diagrams for installation procedures
2. **Issue Resolution**: Use troubleshooting flowcharts for systematic problem-solving
3. **Security Understanding**: Reference security diagrams for permission requirements
4. **Performance Analysis**: Use performance diagrams to identify bottlenecks

## Rendering the Diagrams

### Mermaid Diagrams
- Can be rendered in GitHub, GitLab, and most modern markdown viewers
- Use online editors like mermaid.live for testing
- Integrate with documentation platforms that support Mermaid

### PlantUML Diagrams
- Require PlantUML processor for rendering
- Use online editors like plantuml.com for quick viewing
- Can be integrated into CI/CD pipelines for automatic generation
- Support export to PNG, SVG, and other formats

### Tools and Integration

**Online Tools**:
- [Mermaid Live Editor](https://mermaid.live)
- [PlantUML Online Server](http://www.plantuml.com/plantuml/)
- [Draw.io](https://draw.io) (supports both formats)

**IDE Integration**:
- VS Code extensions for both Mermaid and PlantUML
- IntelliJ/WebStorm plugins available
- Atom and Sublime Text extensions

**Documentation Platforms**:
- GitHub/GitLab native Mermaid support
- GitBook, Notion, and other platforms
- Static site generators (Jekyll, Hugo, etc.)

This comprehensive diagram index helps navigate the visual documentation and ensures consistency across all technical diagrams in the VB6 Portable IDE documentation suite.