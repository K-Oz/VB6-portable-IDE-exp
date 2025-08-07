# Troubleshooting Guide

## Common Issues and Solutions

This guide helps resolve common problems when using the VB6 Portable IDE.

## Startup Issues

### Application Won't Launch

```mermaid
flowchart TD
    A[VB6 Portable won't start] --> B{Error Message?}
    B -->|Permission Denied| C[Run as Administrator]
    B -->|File Not Found| D[Check File Integrity]
    B -->|Registry Error| E[Registry Issues]
    B -->|No Error Message| F[Silent Failure]
    
    C --> C1[Right-click executable]
    C1 --> C2[Select 'Run as administrator']
    C2 --> C3[Accept UAC prompt]
    
    D --> D1[Verify all files present]
    D1 --> D2[Redownload if missing]
    
    E --> E1[Check registry permissions]
    E1 --> E2[Run registry repair]
    
    F --> F1[Check Windows Event Log]
    F1 --> F2[Review error details]
```

#### Solution Steps

1. **Permission Issues**
   ```plantuml
   @startuml
   start
   :Launch VB6 Portable;
   if (Admin Rights?) then (No)
       :Right-click executable;
       :Select "Run as administrator";
       :Confirm UAC prompt;
   else (Yes)
       :Proceed with launch;
   endif
   if (Registry Access?) then (No)
       :Check Windows UAC settings;
       :Verify user account type;
   else (Yes)
       :Continue startup sequence;
   endif
   stop
   @enduml
   ```

2. **File Integrity Check**
   - Verify all files from the [file structure](technical-reference.md#file-structure)
   - Check file sizes and dates
   - Re-extract from original archive if files are missing

3. **Registry Diagnostics**
   ```bash
   # Run as Administrator in Command Prompt
   reg query "HKEY_CLASSES_ROOT\Licenses\6000720D-F342-11D1-AF65-00A0C90DCA10"
   ```

### AutoPlay Interface Issues

```mermaid
stateDiagram-v2
    [*] --> LaunchAttempt
    LaunchAttempt --> CheckAutoPlay : Launch VB6 Portable
    CheckAutoPlay --> AutoPlayLoaded : Success
    CheckAutoPlay --> AutoPlayFailed : Failure
    
    AutoPlayFailed --> CheckFiles : Diagnose
    CheckFiles --> MissingFiles : Files Missing
    CheckFiles --> CorruptFiles : Files Corrupt
    CheckFiles --> ConfigIssue : Config Problem
    
    MissingFiles --> ReinstallFiles
    CorruptFiles --> RedownloadPackage
    ConfigIssue --> ResetConfig
    
    ReinstallFiles --> LaunchAttempt
    RedownloadPackage --> LaunchAttempt
    ResetConfig --> LaunchAttempt
    
    AutoPlayLoaded --> [*]
```

#### AutoPlay Troubleshooting Steps

| Issue | Symptoms | Solution |
|-------|----------|----------|
| **Black Screen** | AutoPlay window appears but shows nothing | Check graphics compatibility, update display drivers |
| **No Audio** | Interface loads but no sound effects | Verify audio files in AutoPlay/Audio/ directory |
| **Buttons Not Working** | Interface displays but clicks don't work | Check autorun.cdd configuration file |
| **Slow Loading** | AutoPlay takes excessive time to load | Check available memory and close other applications |

## VB6 IDE Issues

### IDE Won't Start from AutoPlay

```plantuml
@startuml
participant "User" as U
participant "AutoPlay" as AP
participant "VB6 Launcher" as VL
participant "VB6.exe" as VB6
participant "Registry" as REG

U -> AP : Click "Start VB6"
AP -> VL : Launch VB6 IDE
VL -> REG : Verify VB6 licenses

alt Licenses Valid
    VL -> VB6 : Start VB6.exe
    VB6 --> U : VB6 IDE appears
else Licenses Invalid
    VL --> U : License error message
    Note right of U : Run registry setup again
end

alt VB6.exe Missing
    VL --> U : File not found error
    Note right of U : Check Portable.VB6 directory
end
@enduml
```

#### Common VB6 Launch Problems

1. **Missing VB6.exe**
   - Location: `Visual Basic 6 Portable/AutoPlay/Docs/Portable.VB6/Vb6.exe`
   - Solution: Verify file exists and has proper permissions

2. **DLL Load Errors**
   ```mermaid
   graph TD
   A[DLL Load Error] --> B{Which DLL?}
   B -->|Vba6.dll| C[VBA Runtime Missing]
   B -->|Dao350.dll| D[Database Support Missing]
   B -->|Mspdb60.dll| E[Debug Support Missing]
   
   C --> F[Check Portable.VB6 directory]
   D --> F
   E --> F
   
   F --> G[Restore missing DLL]
   G --> H[Re-register components]
   ```

3. **License Validation Failures**
   - Check registry entries under `HKEY_CLASSES_ROOT\Licenses\`
   - Run `VbPortable6.reg` file as Administrator
   - Verify license strings match expected values

### Development Environment Issues

```mermaid
sequenceDiagram
    participant Dev as Developer
    participant IDE as VB6 IDE
    participant Comp as Compiler
    participant Debug as Debugger
    
    Dev->>IDE: Open Project
    IDE->>IDE: Load Project Files
    
    alt Project Load Success
        IDE->>Dev: Project Ready
    else Project Load Failure
        IDE->>Dev: Error: Cannot load project
        Note over Dev: Check .vbp file integrity
    end
    
    Dev->>IDE: Compile Project
    IDE->>Comp: Start compilation
    
    alt Compilation Success
        Comp->>IDE: EXE created
        IDE->>Dev: Build successful
    else Compilation Error
        Comp->>IDE: Error details
        IDE->>Dev: Compilation failed
        Note over Dev: Review error messages
    end
```

## Performance Issues

### Slow Performance Diagnosis

```mermaid
flowchart TD
    A[Performance Issues] --> B{Issue Type}
    B -->|Startup Slow| C[Startup Optimization]
    B -->|IDE Slow| D[Runtime Optimization]  
    B -->|Compilation Slow| E[Compiler Optimization]
    
    C --> C1[Close unnecessary programs]
    C --> C2[Check available RAM]
    C --> C3[Disable antivirus real-time scanning temporarily]
    
    D --> D1[Close unused IDE windows]
    D --> D2[Reduce project complexity]
    D --> D3[Clear temporary files]
    
    E --> E1[Use incremental compilation]
    E --> E2[Optimize code before compiling]
    E --> E3[Close other applications]
```

### Memory Usage Optimization

```plantuml
@startuml
package "Memory Management" {
    [VB6 IDE Process] as VB6P
    [AutoPlay Process] as APP
    [System Memory] as MEM
}

VB6P -> MEM : Allocate Base Memory (80MB)
VB6P -> MEM : Project Memory (Variable)
VB6P -> MEM : Debug Memory (if debugging)

APP -> MEM : Interface Memory (35MB)
APP -> MEM : Audio/Graphics Cache

note right of MEM
    Total Memory Usage:
    - Minimum: 115MB
    - Typical: 200-300MB
    - Peak: 500MB+ (large projects)
end note

MEM -> VB6P : Memory Pressure Signal
VB6P -> VB6P : Garbage Collection
VB6P -> MEM : Release Unused Memory
@enduml
```

## Configuration Issues

### Registry Problems

```mermaid
graph TD
    A[Registry Issues] --> B{Problem Type}
    B -->|Access Denied| C[Permission Issues]
    B -->|Missing Keys| D[Incomplete Installation]
    B -->|Corrupt Values| E[Registry Corruption]
    
    C --> C1[Run as Administrator]
    C1 --> C2[Check UAC settings]
    C2 --> C3[Verify user permissions]
    
    D --> D1[Re-run VbPortable6.reg]
    D1 --> D2[Manual registry import]
    D2 --> D3[Verify license keys installed]
    
    E --> E1[Registry cleanup]
    E1 --> E2[Remove VB6 entries]
    E2 --> E3[Fresh registry install]
```

### File Association Problems

Some users may experience issues with VB6 file associations. Here's how to diagnose and fix:

```plantuml
@startuml
start
:File association issue reported;
:Check current associations;
if (VB6 files open correctly?) then (No)
    :Open Default Programs;
    :Set VB6 as default for .vbp, .frm, .bas files;
    if (Still not working?) then (Yes)
        :Check file type registry;
        :Manually associate file types;
    endif
else (Yes)
    :No action needed;
endif
stop
@enduml
```

## Error Messages

### Common Error Codes and Solutions

| Error Code | Message | Cause | Solution |
|------------|---------|-------|----------|
| **429** | ActiveX component can't create object | Missing or unregistered component | Re-register VB6 components, check COM registration |
| **76** | Path not found | Invalid file path in project | Update project file paths, verify file locations |
| **53** | File not found | Missing project file or resource | Restore missing files, update file references |
| **91** | Object variable or With block variable not set | Uninitialized object reference | Initialize objects before use, check Nothing assignments |
| **13** | Type mismatch | Incompatible data types | Verify variable types, check function parameters |

### Debugging Error Messages

```mermaid
sequenceDiagram
    participant User as User
    participant VB6 as VB6 IDE
    participant ErrorHandler as Error Handler
    participant EventLog as Windows Event Log
    
    User->>VB6: Perform action
    VB6->>VB6: Process request
    
    alt Error Occurs
        VB6->>ErrorHandler: Generate error
        ErrorHandler->>User: Display error dialog
        ErrorHandler->>EventLog: Log error details
        
        User->>ErrorHandler: Click "Help" button
        ErrorHandler->>User: Show context help
        
        alt Critical Error
            ErrorHandler->>VB6: Terminate process
        else Recoverable Error
            ErrorHandler->>VB6: Continue operation
        end
    else Success
        VB6->>User: Complete action
    end
```

## Network and Deployment Issues

### Multi-User Environment Problems

```plantuml
@startuml
package "Network Deployment Issues" {
    actor "User A" as UA
    actor "User B" as UB
    actor "Administrator" as ADMIN
    
    node "File Server" as FS {
        [VB6 Portable Master] as MASTER
    }
    
    node "Workstation A" as WSA {
        [Local VB6 Copy A] as LOCALA
        [Registry A] as REGA
    }
    
    node "Workstation B" as WSB {
        [Local VB6 Copy B] as LOCALB
        [Registry B] as REGB
    }
}

ADMIN -> FS : Deploy master copy
FS -> WSA : Copy files
FS -> WSB : Copy files

UA -> LOCALA : First run (as Admin)
LOCALA -> REGA : Configure registry
UB -> LOCALB : First run (as Admin)  
LOCALB -> REGB : Configure registry

note bottom of WSA : Each workstation needs\nindividual setup
@enduml
```

### USB/Portable Drive Issues

Common problems when running from removable media:

1. **Drive Letter Changes**
   - VB6 may store absolute paths
   - Solution: Use relative paths in projects

2. **Write Permissions**
   - Some USB drives are read-only
   - Solution: Check drive properties, format if necessary

3. **Performance Issues**
   - USB 2.0 vs USB 3.0 speed differences
   - Solution: Use faster USB drives or copy to local disk

```mermaid
graph TD
    A[USB Drive Issues] --> B{Issue Type}
    B -->|Drive Letter Change| C[Update Path References]
    B -->|Write Protected| D[Check Drive Properties]
    B -->|Slow Performance| E[Use Faster Drive]
    B -->|Corruption| F[Check Drive Integrity]
    
    C --> C1[Use relative paths in VB6]
    D --> D1[Remove write protection]
    E --> E1[Copy to local disk]
    F --> F1[Run chkdsk /f]
```

## Advanced Troubleshooting

### System Diagnostic Tools

```plantuml
@startuml
package "Diagnostic Tools" {
    [Process Monitor] as PM
    [Registry Editor] as RE  
    [Event Viewer] as EV
    [Task Manager] as TM
    [Dependency Walker] as DW
}

package "VB6 Portable System" {
    [Main Process] as MP
    [VB6 IDE] as IDE
    [Registry Entries] as REG
    [File System] as FS
}

PM -> MP : Monitor file/registry access
RE -> REG : Inspect registry entries
EV -> MP : View error logs
TM -> MP : Monitor resource usage
DW -> IDE : Check DLL dependencies

MP -> FS : File operations
MP -> REG : Registry operations
@enduml
```

### Log Analysis

When troubleshooting complex issues:

1. **Windows Event Log**
   - Application logs for VB6 errors
   - System logs for permission issues

2. **Process Monitor**
   - Track file and registry access
   - Identify permission problems

3. **VB6 Debug Output**
   - Use Debug.Print statements
   - Check Immediate Window for errors

### Recovery Procedures

```mermaid
flowchart TD
    A[System Failure] --> B{Severity Level}
    B -->|Minor Issues| C[Quick Fix]
    B -->|Major Issues| D[Full Recovery]
    B -->|Total Failure| E[Complete Reinstall]
    
    C --> C1[Restart Application]
    C --> C2[Clear Temp Files]
    C --> C3[Reset Configuration]
    
    D --> D1[Registry Repair]
    D --> D2[File Restoration]
    D --> D3[Component Re-registration]
    
    E --> E1[Remove All Files]
    E --> E2[Clean Registry]
    E --> E3[Fresh Installation]
    
    C1 --> F[Test Functionality]
    C2 --> F
    C3 --> F
    D1 --> F
    D2 --> F
    D3 --> F
    E1 --> G[Verify Complete Removal]
    E2 --> G
    E3 --> H[New Installation]
    G --> H
    H --> F
```

## Getting Additional Help

### Support Resources

```mermaid
mindmap
    root((VB6 Portable<br/>Support))
        Documentation
            Installation Guide
            User Guide
            Technical Reference
            This Troubleshooting Guide
        Community Resources
            VB6 Forums
            Stack Overflow
            GitHub Issues
            Developer Communities
        Microsoft Resources
            VB6 Documentation
            Knowledge Base
            Legacy Support
        Diagnostic Tools
            Built-in Windows Tools
            Third-party Utilities
            Online Resources
```

### Creating Bug Reports

When reporting issues:

```plantuml
@startuml
start
:Encounter issue;
:Reproduce the problem;
:Gather system information;
:Document steps to reproduce;
:Include error messages;
:Note system configuration;
:Create detailed report;
stop

note right: Include:\n- Windows version\n- VB6 Portable version\n- Error messages\n- Steps to reproduce\n- Expected vs actual behavior
@enduml
```

### Information to Include

- **System Information**: Windows version, architecture, available memory
- **VB6 Portable Version**: Build date, file sizes, installation location
- **Error Details**: Exact error messages, error codes, timestamps  
- **Reproduction Steps**: Detailed steps to recreate the issue
- **Environment**: Antivirus software, other development tools installed
- **Files**: Relevant log files, screenshots of error messages

This troubleshooting guide covers the most common issues encountered with VB6 Portable IDE. For issues not covered here, refer to the [Technical Reference](technical-reference.md) or create a detailed bug report using the guidelines above.