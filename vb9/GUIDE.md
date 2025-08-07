# VB9 Development Guide

## The VB6 to VB9 Experience

**VB6 (1998):**
1. Draw form
2. Add controls  
3. Double-click button
4. Write: `MsgBox "Hello"`
5. Hit F5
6. Ship the .exe

**VB9 (2024):**
1. Draw form (maps to `/form/myapp`)
2. Add controls (each control = file in namespace)
3. Double-click button (writes to `/form/button1/click`)
4. Write: `print("Hello")`
5. Run: `mk && ./myapp`
6. Deploy: `bind /app/myapp /net/*/myapp` (runs everywhere!)

## Quick Examples

### Hello World (30 seconds)

```c
#include "vb9.h"

void main(void) {
    VB9Runtime *rt = vb9_init();
    VB9Form *form = vb9_form_new("HelloForm", "Hello VB9");
    
    vb9_label_new(form, "Label1", "Hello World!");
    vb9_button_new(form, "OkButton", "OK");
    
    vb9_form_show(form);
    vb9_run(rt);
    vb9_shutdown(rt);
}
```

### Simple Calculator

```c
VB9Form *calc = vb9_form_new("Calculator", "VB9 Calc");
Control *display = vb9_textbox_new(calc, "Display", "0");
Control *btn_1 = vb9_button_new(calc, "btn_1", "1");
// ... position controls ...
vb9_form_show(calc);
```

## Plan 9 Integration - The Magic

### Phase 2: Everything is a File

In Plan 9, each VB9 control becomes a file:

```bash
# Your form creates this namespace:
/form/myapp/
├── label1/
│   ├── text        # "Hello World!"  
│   ├── visible     # "1"
│   └── position    # "50,30"
├── button1/
│   ├── caption     # "Click Me"
│   ├── click       # event handler
│   └── enabled     # "1"
└── textbox1/
    ├── text        # current content
    └── change      # event handler
```

### Event Handling via Files

```c
// VB6: Private Sub Command1_Click()
// VB9: Button click writes to /form/button1/click

void handle_button_click(Control *btn) {
    int fd = open("/form/button1/click", OREAD);
    char handler[256];
    read(fd, handler, sizeof(handler));
    // Execute the handler code
    close(fd);
}
```

### Distributed Forms with drawterm

```bash
# On machine A: Create form
vb9-designer myapp.frm

# On machine B: Run via drawterm  
drawterm -a machineA -c machineA
# Form renders on machine B, executes on machine A
# Drawing IS remote computation!
```

## File System Architecture

### Control-to-File Mapping

```c
// VB9 property access maps to file operations:

// VB6: Text1.Text = "hello"  
// VB9: 
fd = open("/form/text1/text", OWRITE);
write(fd, "hello", 5);
close(fd);

// VB6: caption = Button1.Caption
// VB9:
fd = open("/form/button1/caption", OREAD);
read(fd, caption, sizeof(caption));
close(fd);
```

### Event System

```c
// Each control serves its events as files
serve("/form/button1/click", handle_click);
serve("/form/text1/change", handle_change);
serve("/form/listbox1/select", handle_select);

// Event handlers read from their respective files
void handle_click(Req *r) {
    // Execute user code when button is clicked
    respond(r, nil);
}
```

## Build and Deploy

### Single Machine
```bash
mk formdesigner    # Build the designer
mk hello          # Build hello world
./hello           # Run locally
```

### Distributed (Plan 9 cluster)
```bash
# Build on any machine
mk myapp

# Deploy everywhere
bind /app/myapp /net/cpu1/myapp
bind /app/myapp /net/cpu2/myapp  
bind /app/myapp /net/cpu3/myapp

# Now myapp runs on all machines!
```

### Via drawterm (remote display)
```bash
# VB9 form runs on server, displays on client
drawterm -a server -c server
# All drawing operations are remote computation
```

## Size Comparison

```
VB6 Runtime:     1.4MB
VB9 Runtime:     <1MB (target)
Windows 11:      30GB+
Plan 9:          200MB (full OS)
node_modules:    2GB (for one app!)

VB6 Hello.exe:   24KB
VB9 hello:       ~20KB (target)  
React Hello:     150MB (with Electron)
```

## Development Philosophy

### VB6 Principles We Keep
- Visual design = actual program
- 30-second learning curve  
- Double-click to add event handler
- Compile to single executable
- No external dependencies

### Plan 9 Powers We Add
- Everything is a file
- Network transparency
- Distributed execution
- Real security (not theater)
- Simple protocols that work

### Modern Complexity We Reject
- Build systems (just `mk`)
- Package managers (OS has everything)
- Containers (processes just work)
- Frameworks (OS IS the framework)
- "DevOps" (it just works)

## The Revolution

**Then:** Needed Windows + VB6 IDE + runtime DLLs  
**Now:** Just Plan 9 + VB9 (one small file)

**Then:** Application runs on one machine  
**Now:** Application IS the distributed system

**Then:** Forms display locally  
**Now:** Forms can render anywhere (drawterm magic)

**Then:** File = data storage  
**Now:** File = computational interface

We're not building new software. We're revealing the computational substrate that was always there! 🚀