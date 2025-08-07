# VB9 Demo: From VB6 Legacy to Plan 9 Future

## The Connection: VB6 → VB9

This repository contains both:
- **VB6 Portable IDE** (Windows binaries in `/Visual Basic 6 Portable/`)
- **VB9 Implementation** (Plan 9 source code in `/vb9/`)

The connection demonstrates the evolution from VB6's simplicity to VB9's distributed power.

## Demo 1: Size Comparison

**VB6 Original (1998):**
```
Visual Basic 6 Portable.exe:    6.0MB (entire IDE!)
Vb6.exe (IDE):                 1.8MB 
Vb6ide.dll:                    1.0MB
Total VB6 Runtime:             ~1.4MB
```

**VB9 Implementation (2024):**
```
vb9/include/vb9.h:             3.5KB (core structures)
vb9/src/runtime.c:             5.8KB (minimal runtime)
vb9/src/controls.c:            8.2KB (all controls)
vb9/src/formdesigner.c:        1.7KB (visual designer)
vb9/examples/hello.c:          1.1KB (hello world)
Total VB9 Source:              ~20KB (will compile to <1MB)
```

**Modern Equivalent:**
```
React Hello World:             150MB+ (with Electron)
node_modules/:                 2GB+ (dependencies)
Visual Studio Code:            500MB+ (just the IDE)
```

## Demo 2: Simplicity Comparison

**VB6 Hello World (1998):**
1. Start VB6 IDE
2. Draw form
3. Add label: "Hello World!"  
4. Add button: "OK"
5. Double-click button → `End`
6. Press F5 → Working exe!

**VB9 Hello World (2024):**
1. Edit hello.c (30 lines)
2. `mk hello` → Compile
3. `./hello` → GUI appears
4. Click button → "Hello World!"
5. Distributed: `bind /app/hello /net/*/hello`

**React Hello World (2024):**
1. `npm init react-app hello` (5 minutes)
2. `cd hello && npm install` (500MB download)
3. Edit App.js (configure webpack, babel, etc.)
4. `npm start` (dev server, hot reload, etc.)
5. Build: `npm run build` (optimized bundle)
6. Deploy: Docker container, Kubernetes, CI/CD pipeline...

## Demo 3: The Magic - Drawing IS Computing

### VB6 Limitation: Local Only
```vb
' VB6 - runs only on local Windows machine
Private Sub Command1_Click()
    Text1.Text = "Hello from " & Environ("COMPUTERNAME")
End Sub
```

### VB9 Power: Distributed Computing
```c
// VB9 - runs anywhere on the network!
void button_click(Control *ctrl) {
    // This could execute on remote CPU
    char hostname[64];
    gethostname(hostname, sizeof(hostname));
    
    // Update display (might be on different machine!)
    vb9_control_set_text(display, hostname);
    
    // Drawing the text IS remote computation!
}
```

### The Breakthrough: File System Interface
```bash
# Control your VB9 app from command line!
echo "New text" > /form/myapp/textbox1/text
echo "click" > /form/myapp/button1/click  
cat /form/myapp/display/text

# The app updates in real-time!
# The command line IS the programming interface!
```

## Demo 4: Practical Migration Path

### Legacy VB6 Code:
```vb
Private Sub Calculate_Click()
    Dim result As Double
    result = Val(Text1.Text) + Val(Text2.Text)
    Label1.Caption = "Result: " & result
End Sub
```

### VB9 Equivalent:
```c
void calculate_click(Control *btn) {
    double val1 = atof(vb9_control_get_text(text1));
    double val2 = atof(vb9_control_get_text(text2));
    double result = val1 + val2;
    
    char buf[64];
    snprint(buf, sizeof(buf), "Result: %.2f", result);
    vb9_control_set_text(label1, buf);
}
```

### Plan 9 Magic - Same Logic, Distributed:
```bash
# Run calculator logic on server CPU
echo "5" > /net/server/form/calc/text1/text
echo "3" > /net/server/form/calc/text2/text  
echo "click" > /net/server/form/calc/button1/click

# Result appears on your local display!
cat /net/server/form/calc/label1/text
# "Result: 8.00"
```

## Demo 5: The Revolution

**What VB6 Started:**
- Visual programming (drag & drop)
- Rapid application development  
- Compile to native executable
- Simple event model

**What VB9 Completes:**
- Visual programming + file system = ultimate simplicity
- Rapid distributed development
- Compile to Plan 9 executable (runs anywhere)
- Event model = file operations (network transparent!)

**The Insight:**
Drawing a button isn't just creating a GUI widget.  
Drawing a button IS creating a computational interface.  
The button doesn't just "look clickable" - it IS clickable from anywhere on the network.

When you draw a form in VB9, you're not just designing a user interface.  
You're architecting a distributed computational system!

## Try It Yourself

1. **Experience VB6 Nostalgia**: Extract and run the VB6 Portable IDE
2. **Build VB9 Future**: `cd vb9 && mk hello && ./hello`
3. **See the Magic**: Each VB9 control becomes a file you can manipulate
4. **Go Distributed**: Deploy your forms across Plan 9 network

The future was always there. We're just revealing it! 🚀