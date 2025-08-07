# Installation Guide

## System Requirements

### Minimum Requirements
- **Operating System**: Windows XP SP3 or later
- **Memory**: 512MB RAM
- **Disk Space**: 150MB free space
- **Permissions**: Administrative privileges (for initial setup)

### Recommended Requirements
- **Operating System**: Windows 7 or later
- **Memory**: 1GB RAM or more
- **Disk Space**: 500MB free space
- **Permissions**: Standard user (after initial setup)

## Installation Process

```mermaid
flowchart TD
    A[Download VB6 Portable] --> B[Extract to Desired Location]
    B --> C[Run as Administrator]
    C --> D{Registry Setup Required?}
    D -->|Yes| E[Accept UAC Prompt]
    D -->|No| F[Launch AutoPlay]
    E --> G[Registry Configuration]
    G --> F
    F --> H[VB6 Portable Ready]
    
    I[First Time User] --> J[Follow Setup Wizard]
    I --> K[Configure Preferences]
    
    J --> H
    K --> H
```

## Step-by-Step Installation

### Step 1: Download and Extract

1. Download the VB6 Portable IDE package
2. Extract the contents to your desired location (e.g., `C:\PortableApps\VB6` or USB drive)
3. Ensure the directory structure remains intact

```
VB6-Portable-IDE/
├── Visual Basic 6 Portable.exe
├── Visual Basic 6 Portable/
│   ├── autorun.exe
│   ├── ico.ico
│   └── AutoPlay/
└── README.md
```

### Step 2: Initial Launch

```plantuml
@startuml
actor User
participant "Windows Explorer" as WE
participant "VB6 Portable.exe" as VB6
participant "Windows Registry" as REG
participant "AutoPlay" as AP

User -> WE : Navigate to installation folder
User -> WE : Right-click VB6 Portable.exe
WE -> User : Show context menu
User -> WE : Select "Run as administrator"
WE -> VB6 : Launch with elevated privileges

VB6 -> REG : Check for VB6 licenses
REG --> VB6 : License status

alt Licenses not found
    VB6 -> REG : Install VB6 license keys
    REG --> VB6 : Registration complete
    VB6 -> REG : Register VB6 components
    REG --> VB6 : Components registered
end

VB6 -> AP : Launch AutoPlay interface
AP --> User : Display VB6 Portable menu
@enduml
```

### Step 3: Configuration

The AutoPlay interface will guide you through:

1. **License Validation**: Automatic setup of VB6 licensing
2. **Component Registration**: Registration of VB6 DLLs and controls
3. **Environment Configuration**: Path and registry settings

```mermaid
stateDiagram-v2
    [*] --> InitialLaunch
    InitialLaunch --> CheckingLicenses
    CheckingLicenses --> LicensesFound : Valid
    CheckingLicenses --> InstallingLicenses : Missing
    InstallingLicenses --> RegisteringComponents
    LicensesFound --> RegisteringComponents
    RegisteringComponents --> ConfiguringEnvironment
    ConfiguringEnvironment --> Ready
    Ready --> [*]
```

## Installation Verification

### Verify Installation

1. Launch `Visual Basic 6 Portable.exe`
2. AutoPlay interface should appear
3. Click "Start VB6" or equivalent option
4. VB6 IDE should launch successfully

### Common Installation Checks

```mermaid
graph LR
    A[Check File Integrity] --> B{All files present?}
    B -->|Yes| C[Check Registry]
    B -->|No| D[Reinstall Package]
    C --> E{Licenses registered?}
    E -->|Yes| F[Test VB6 Launch]
    E -->|No| G[Run as Administrator]
    F --> H{IDE Launches?}
    H -->|Yes| I[Installation Complete]
    H -->|No| J[Check Troubleshooting]
    G --> C
    D --> A
    J --> K[Review Error Logs]
```

## Network Installation

For organizations deploying across multiple machines:

```plantuml
@startuml
package "Network Share" {
    [VB6 Portable Master] as master
}

package "Workstation 1" {
    [Local Copy 1] as local1
}

package "Workstation 2" {
    [Local Copy 2] as local2
}

package "Workstation N" {
    [Local Copy N] as localN
}

master --> local1 : Copy & Configure
master --> local2 : Copy & Configure
master --> localN : Copy & Configure

note right of master
    Master installation with
    pre-configured settings
end note

note bottom of local1
    Each workstation requires
    individual registry setup
end note
@enduml
```

## USB/Portable Drive Installation

```mermaid
sequenceDiagram
    participant U as User
    participant USB as USB Drive
    participant PC as Target PC
    participant VB6 as VB6 Portable
    
    U->>USB: Copy VB6 Portable to USB
    U->>PC: Insert USB Drive
    PC->>USB: Mount drive
    U->>VB6: Execute from USB location
    
    Note over VB6: Registry setup on target PC
    
    VB6->>PC: Configure local registry
    VB6-->>U: Ready for development
    
    Note over U,VB6: Portable development environment ready
```

## Uninstallation

To remove VB6 Portable IDE:

### Complete Removal

```mermaid
flowchart TD
    A[Close VB6 IDE] --> B[Exit AutoPlay]
    B --> C[Delete Installation Directory]
    C --> D{Remove Registry Entries?}
    D -->|Yes| E[Run Registry Cleanup]
    D -->|No| F[Keep VB6 Licenses]
    E --> G[Uninstallation Complete]
    F --> G
```

### Registry Cleanup (Optional)

If you want to completely remove VB6 licensing:

1. Open Registry Editor (`regedit.exe`) as Administrator
2. Navigate to `HKEY_CLASSES_ROOT\Licenses\`
3. Remove VB6-related license entries
4. Navigate to `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VisualStudio\6.0\`
5. Remove VB6 configuration entries

**⚠️ Warning**: Only perform registry cleanup if you're certain you won't need VB6 licensing for other applications.

## Troubleshooting Installation

### Common Issues

| Issue | Symptom | Solution |
|-------|---------|----------|
| **Permission Denied** | Cannot launch executable | Run as Administrator |
| **Missing DLL** | Component load errors | Verify file integrity |
| **Registry Access** | License validation fails | Check UAC settings |
| **AutoPlay Won't Start** | Black screen on launch | Check Windows compatibility |

### Advanced Troubleshooting

```plantuml
@startuml
start

:Launch VB6 Portable;

if (Administrative privileges?) then (No)
    :Request elevation;
    :Retry launch;
else (Yes)
    if (Registry accessible?) then (No)
        :Check UAC settings;
        :Verify user permissions;
    else (Yes)
        if (All files present?) then (No)
            :Redownload package;
            :Extract completely;
        else (Yes)
            if (AutoPlay launches?) then (No)
                :Check Windows compatibility;
                :Update Windows;
            else (Yes)
                :Installation successful;
                stop
            endif
        endif
    endif
endif

stop
@enduml
```

## Post-Installation Configuration

### Environment Variables

The portable IDE may set temporary environment variables:
- `VB6_ROOT`: Points to portable installation directory
- `VB6_TEMP`: Temporary files location
- `VB6_PROJECT`: Default project directory

### Path Configuration

```mermaid
graph TB
    A[VB6 Portable Directory] --> B[Add to PATH temporarily]
    B --> C[VB6 Tools Available]
    C --> D[Compiler Access]
    C --> E[Debug Tools Access]
    C --> F[Help System Access]
    
    G[Project Directory] --> H[Set as Working Directory]
    H --> I[Source Files]
    H --> J[Output Files]
    H --> K[Resource Files]
```

This completes the installation process. The VB6 Portable IDE should now be ready for development work.