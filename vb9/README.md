# VB9 - Visual Basic for Plan 9

**Drawing Computation Since 1995**

VB9 reimplements Visual Basic 6's simplicity on Plan 9's distributed architecture with the core insight: "drawing" and "computing" are the same operation - rendering forms IS executing programs.

## Core Principles

1. **1.4MB Runtime Target** - If VB6 did it in 1.4MB, we can too
2. **Everything is a File** - Forms, controls, events all map to 9P filesystem  
3. **Drawing = Computing** - Form rendering IS remote execution via drawterm
4. **No Dependencies** - Just like VB6, everything needed ships in one small runtime
5. **Visual = Actual** - What you draw in the designer is exactly what runs

## Quick Start

```bash
# Compile the form designer
mk formdesigner

# Create a simple form
./formdesigner myapp.frm

# Compile and run
mk myapp
./myapp
```

## The VB9 Experience

1. **Draw your form** (saves to `/form/myapp`)  
2. **Write your code** (saves to `/code/myapp`)
3. **Compile** (`mk myapp`)
4. **Deploy** (`bind /app/myapp /net/*/myapp`)
5. **It's running EVERYWHERE instantly!**

No npm, no webpack, no Docker, no Kubernetes. Just files, namespaces, and simple protocols that actually work!

## Architecture

```
vb9/
├── src/
│   ├── formdesigner.c    # Visual form designer
│   ├── runtime.c         # Minimal VB9 runtime
│   ├── controls.c        # Button, TextBox, Label, ListBox
│   └── compiler.c        # Form + Code → Plan 9 executable
├── include/
│   ├── vb9.h            # Core VB9 structures
│   └── controls.h       # Control definitions  
├── examples/
│   ├── hello.frm        # Hello World form
│   └── calculator.frm   # Simple calculator
└── mkfile              # Plan 9 build file
```

## Form-to-File Mapping

Every VB9 form maps to Plan 9 filesystem operations:

```c
// VB9 "Text1.Text = value" becomes:
fd = open("/form/text1/value", OWRITE);
write(fd, value, strlen(value));

// VB9 "Command1_Click" becomes:  
serve("/form/command1/click", handleClick);
```

## Development Status

- [x] Basic architecture design
- [ ] Form designer implementation
- [ ] Control system  
- [ ] Event handling
- [ ] Compiler/runtime
- [ ] Plan 9 integration

## Target: VB6 Simplicity + Plan 9 Power

**Then (VB6):** 30 seconds to working executable  
**Now (VB9):** 30 seconds to working distributed system