# VB9 - Visual Basic for Plan 9

**Drawing Computation Since 1995**

VB9 reimplements Visual Basic 6's simplicity on Plan 9's distributed architecture with the core insight: "drawing" and "computing" are the same operation - rendering forms IS executing programs.

## Core Principles

1. **1.4MB Runtime Target** - If VB6 did it in 1.4MB, we can too
2. **Everything is a File** - Forms, controls, events all map to 9P filesystem  
3. **Drawing = Computing** - Form rendering IS remote execution via drawterm
4. **Prime Computational Space** - Controls are prime numbers, forms are their products
5. **Event = File = Computation** - Writing to files IS executing code
6. **No Dependencies** - Just like VB6, everything needed ships in one small runtime
7. **Visual = Actual** - What you draw in the designer is exactly what runs

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
│   ├── hello.c        # Hello World form
│   ├── calculator.c   # Simple calculator
│   └── primordial.c   # Primordial button demonstration
├── test/
│   ├── test_signatures.c      # Prime signature system test
│   └── event_file_mapping.c   # Event-to-File mapping demo
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

## Prime Computational Space

VB9 reveals that controls are prime numbers in computational space:

```c
enum {
    VB9_BUTTON   = 2,    /* first prime - fundamental interaction */
    VB9_TEXTBOX  = 3,    /* second prime - input/output */
    VB9_LABEL    = 5,    /* third prime - information display */
    VB9_LISTBOX  = 7,    /* fourth prime - selection/enumeration */
};

// Form signature = product of all control type primes
uint64_t form_signature(VB9Form *f) {
    uint64_t sig = 1;
    for(int i = 0; i < f->ncontrols; i++) {
        sig *= f->controls[i].type;  // Control types ARE primes
    }
    return sig;  // This IS the program!
}
```

Each form has a unique prime factorization - this IS its computational identity!

- Hello form (Label + Button): `5 * 2 = 10`
- Calculator (TextBox + 16 Buttons): `3 * 2^16 = 196608`
- Every unique form has a unique signature

## Development Status

- [x] Basic architecture design
- [x] Form designer implementation
- [x] Control system with prime number types
- [x] Event handling via 9P file system
- [x] Prime signature calculation for forms
- [x] Automatic namespace creation when drawing
- [x] Event-to-File mapping demonstration
- [x] Primordial button concept implementation
- [ ] Compiler/runtime optimization
- [ ] Full Plan 9 integration
- [ ] Font-as-bytecode integration (ttffs)

## Target: VB6 Simplicity + Plan 9 Power

**Then (VB6):** 30 seconds to working executable  
**Now (VB9):** 30 seconds to working distributed system