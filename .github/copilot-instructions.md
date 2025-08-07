# VB6 Portable IDE

VB6 Portable IDE is a Windows-only portable development environment containing the Visual Basic 6.0 IDE and runtime components packaged as an installer/extractor application.

Always reference these instructions first and fallback to search or bash commands only when you encounter unexpected information that does not match the info here.

## Working Effectively

### Bootstrap and Setup
- Install Wine for testing Windows executables:
  - `sudo dpkg --add-architecture i386`
  - `sudo apt-get update` -- takes 2-3 minutes. NEVER CANCEL. Set timeout to 5+ minutes.
  - `sudo apt-get install -y wine32:i386` -- takes 10-15 minutes. NEVER CANCEL. Set timeout to 30+ minutes.
  - `sudo apt-get install -y xvfb` -- for headless GUI testing
- Verify Wine installation: `wine --version`

### Running the Application
- Main installer/extractor: `"Visual Basic 6 Portable.exe"` (6MB Windows PE32 executable)
- Run with virtual display: `DISPLAY=:99 xvfb-run -a wine "Visual Basic 6 Portable.exe"`
- **CRITICAL**: This is a GUI installer that requires user interaction. Set timeout to 60+ minutes. NEVER CANCEL during installation.
- VB6 IDE location: `"Visual Basic 6 Portable/AutoPlay/Docs/Portable.VB6/Vb6.exe"`
- Run VB6 IDE: `cd "Visual Basic 6 Portable/AutoPlay/Docs/Portable.VB6" && DISPLAY=:99 xvfb-run -a wine Vb6.exe`

### No Build Process
- **CRITICAL**: This repository contains pre-built Windows binaries. There is NO source code to compile.
- DO NOT attempt to build anything - all files are already compiled Windows executables and DLLs.
- Changes to functionality require modifying the Windows executables directly, which is not supported.

## Platform Requirements
- **Windows-only application**: Requires Windows or Wine emulation to run
- **Cannot run natively on Linux/macOS**: Must use Wine or Windows VM
- **GUI required**: All executables are Windows GUI applications that need display

## Validation

### Testing Scenarios  
After making any changes to the repository structure:

1. **Installation Test**:
   - Run: `DISPLAY=:99 xvfb-run -a timeout 300 wine "Visual Basic 6 Portable.exe"`
   - **Expected**: Installer GUI starts (may timeout - this is normal for unattended execution)
   - **Expected Warnings**: ALSA audio warnings are normal in headless mode - ignore them
   - **Time**: 5+ minutes. NEVER CANCEL. Set timeout to 10+ minutes.

2. **VB6 IDE Test** (if extracted):
   - Run: `cd "Visual Basic 6 Portable/AutoPlay/Docs/Portable.VB6" && DISPLAY=:99 xvfb-run -a timeout 120 wine Vb6.exe`
   - **Expected**: VB6 IDE attempts to start (will fail without user interaction)
   - **Expected Warnings**: ALSA audio warnings are normal in headless mode - ignore them
   - **Time**: 2+ minutes. NEVER CANCEL. Set timeout to 5+ minutes.

3. **File Integrity Test**:
   - Check main executable: `file "Visual Basic 6 Portable.exe"`
   - **Expected**: "PE32 executable (GUI) Intel 80386, for MS Windows, 4 sections"
   - Check VB6 IDE: `file "Visual Basic 6 Portable/AutoPlay/Docs/Portable.VB6/Vb6.exe"`
   - **Expected**: "PE32 executable (GUI) Intel 80386 (stripped to external PDB), for MS Windows, 3 sections"
   - Verify file size: `ls -lah "Visual Basic 6 Portable.exe"`
   - **Expected**: Approximately 6.0M bytes

### Manual Validation Requirements
- **CRITICAL**: Cannot perform full end-to-end testing in headless environment
- **Manual testing required on Windows**: To fully validate VB6 IDE functionality
- **Screenshot testing impossible**: GUI applications cannot be fully automated without Windows desktop

## Common Tasks

### Repository Structure
```
.
├── README.md                           # Basic project description
├── Visual Basic 6 Portable.exe        # Main installer/extractor (6MB)
└── Visual Basic 6 Portable/           # Extracted VB6 IDE files
    ├── autorun.exe                     # AutoPlay launcher
    ├── ico.ico                         # Icon file
    └── AutoPlay/                       # AutoPlay CD structure
        ├── Audio/                      # Sound files (.ogg)
        ├── Docs/                       # Documentation and VB6 files
        │   ├── VbPortable6.reg         # Registry entries for VB6
        │   └── Portable.VB6/           # VB6 IDE and runtime files
        │       ├── Vb6.exe            # Visual Basic 6 IDE (main executable)
        │       ├── Vb6ide.dll         # VB6 IDE library
        │       ├── Vba6.dll           # VB6 runtime
        │       └── [other DLLs]       # Supporting libraries
        ├── Icons/                      # Icon resources
        ├── Images/                     # Image resources (.bmp, .jpg, .png)  
        ├── Plugins/                    # AutoPlay plugins
        └── autorun.cdd                 # AutoPlay configuration (binary)
```

### File Types and Extensions
- **Windows Executables (.exe)**: Main installer, VB6 IDE, and utilities
- **Dynamic Libraries (.dll)**: VB6 runtime and support libraries  
- **Object Libraries (.olb)**: VB6 type libraries
- **Registry Files (.reg)**: Windows registry entries for VB6
- **AutoPlay Files (.cdd, .APO)**: CD autoplay configuration
- **Media Files (.ogg, .bmp, .jpg, .png, .ico)**: UI resources

### Key Files and Their Purpose
- `Visual Basic 6 Portable.exe`: Self-extracting installer/launcher
- `Vb6.exe`: Visual Basic 6.0 IDE main executable
- `Vb6ide.dll`: Core VB6 IDE functionality
- `Vba6.dll`: VB runtime engine
- `VbPortable6.reg`: Registry settings required for portable operation
- `autorun.cdd`: AutoPlay CD menu configuration

## Limitations

### Development Constraints  
- **No source code available**: Cannot modify application logic or UI
- **Windows-only testing**: Full validation requires Windows environment
- **Binary-only changes**: Can only modify file structure, not functionality
- **No CI/CD pipeline**: No automated build or test workflows exist
- **Manual testing required**: Cannot fully automate validation scenarios

### What You CANNOT Do
- Build or compile the application (no source code exists)
- Run the full application on Linux without Wine
- Modify the VB6 IDE functionality  
- Test complete user workflows in headless mode
- Automatically extract the installer contents

### What You CAN Do
- Verify file integrity and structure
- Test that executables can be launched with Wine
- Modify documentation and repository structure
- Add GitHub workflow files for basic validation
- Check that Wine can load the Windows executables

## Troubleshooting

### Common Issues
- **ALSA audio warnings**: Normal in headless environment - ignore all ALSA error messages
- **X connection broken**: Expected when running GUI apps with timeout - indicates attempted startup
- **Wine configuration warnings**: Wine may create `~/.wine` directory on first run - this is normal
- **Timeout errors**: GUI applications require user interaction - timeouts are expected in automated testing

### Expected Error Messages
When running Wine commands, these messages are NORMAL and should be ignored:
```
ALSA lib confmisc.c:855:(parse_card) cannot find card '0'
X connection to :99 broken (explicit kill or server shutdown)
wine: created the configuration directory '/home/runner/.wine'
```

## Expected Command Execution Times
- `sudo apt-get update`: 2-3 minutes
- `sudo apt-get install wine32:i386`: 10-15 minutes  
- `wine "Visual Basic 6 Portable.exe"`: 5+ minutes (GUI installer)
- `wine Vb6.exe`: 2+ minutes (VB6 IDE startup)

**NEVER CANCEL** any of these operations. Always set timeouts of at least double the expected time.

### When Commands Succeed
- Commands that return exit code 0 after timeout are usually successful attempts to start GUI
- File integrity commands should always succeed with exact expected output
- Wine version command should show wine-9.0 or similar