/* vb9-hyperglyph.c - VB9 Controls as HyperGlyph Cognitive Domains */
/* Where drawing = computing meets geometric cognitive architecture */

#include <u.h>
#include <libc.h>
#include <draw.h>
#include <thread.h>
#include <9p.h>

/* === HyperGlyph Core Structures === */

typedef struct GeometricManifold {
    char *topology;          /* "euclidean", "hyperbolic", "toroidal" */
    int dimension;           /* typically 2 for UI, n for computation */
    double curvature;        /* 0 for euclidean, -1 for hyperbolic */
    void (*distance)(Point, Point, double*);  /* metric tensor */
} GeometricManifold;

typedef struct AgenticMicrokernel {
    char *name;              /* kernel identifier */
    void *grammar;           /* geometric grammar rules */
    void (*parse)(void*);    /* autonomous parsing function */
    int convex_hull[128];    /* constraint boundary points */
    void (*process)(void*, void*);  /* input -> output transformation */
} AgenticMicrokernel;

typedef struct HyperGlyph {
    GeometricManifold *primary;     /* visual manifold */
    GeometricManifold *secondary;   /* computational manifold */
    AgenticMicrokernel *kernels[16]; /* up to 16 microkernels */
    int nkernels;
    
    /* Cross-topology operations */
    void (*fuse)(void*, void*, void*);  /* merge results from manifolds */
    void (*adapt)(int, int);            /* dimension adaptation */
} HyperGlyph;

/* === VB9 Control as Cognitive Domain === */

typedef struct VB9Control {
    /* Identity */
    char *name;              /* /form/controlname */
    int type;                /* BUTTON, TEXTBOX, etc as prime */
    
    /* Geometric substrate */
    HyperGlyph *glyph;       /* multi-topology processor */
    
    /* Visual manifold (Euclidean) */
    Rectangle rect;          /* position in 2D euclidean space */
    Image *visual;           /* rendered appearance */
    
    /* Event manifold (Hyperbolic) */
    Point event_pos;         /* position in hyperbolic event space */
    double event_radius;     /* influence radius (grows exponentially) */
    
    /* Microkernels for behaviors */
    AgenticMicrokernel *draw_kernel;   /* drawing grammar */
    AgenticMicrokernel *click_kernel;  /* click processing */
    AgenticMicrokernel *hover_kernel;  /* hover effects */
    
    /* File system interface */
    int fd_text;             /* /form/control/text */
    int fd_event;            /* /form/control/event */
    int fd_state;            /* /form/control/state */
} VB9Control;

/* === Geometric Distance Functions === */

double euclidean_distance(Point a, Point b) {
    int dx = b.x - a.x;
    int dy = b.y - a.y;
    return sqrt(dx*dx + dy*dy);
}

double hyperbolic_distance(Point a, Point b) {
    /* Poincaré disk model */
    double ax = a.x / 100.0;  /* normalize to unit disk */
    double ay = a.y / 100.0;
    double bx = b.x / 100.0;
    double by = b.y / 100.0;
    
    double dx = bx - ax;
    double dy = by - ay;
    double a_norm = ax*ax + ay*ay;
    double b_norm = bx*bx + by*by;
    
    /* Hyperbolic metric: grows exponentially near boundary */
    return acosh(1 + 2*(dx*dx + dy*dy)/((1-a_norm)*(1-b_norm)));
}

/* === Control Creation with HyperGlyph === */

VB9Control* vb9_hyperglyph_control_new(char *name, int type, Rectangle r) {
    VB9Control *ctrl = mallocz(sizeof(VB9Control), 1);
    
    ctrl->name = strdup(name);
    ctrl->type = type;  /* prime number encoding */
    ctrl->rect = r;
    
    /* Create HyperGlyph with dual topology */
    ctrl->glyph = mallocz(sizeof(HyperGlyph), 1);
    
    /* Visual manifold - Euclidean for display */
    ctrl->glyph->primary = mallocz(sizeof(GeometricManifold), 1);
    ctrl->glyph->primary->topology = "euclidean";
    ctrl->glyph->primary->dimension = 2;
    ctrl->glyph->primary->curvature = 0.0;
    ctrl->glyph->primary->distance = euclidean_distance;
    
    /* Event manifold - Hyperbolic for infinite branching */
    ctrl->glyph->secondary = mallocz(sizeof(GeometricManifold), 1);
    ctrl->glyph->secondary->topology = "hyperbolic";
    ctrl->glyph->secondary->dimension = 2;
    ctrl->glyph->secondary->curvature = -1.0;
    ctrl->glyph->secondary->distance = hyperbolic_distance;
    
    /* Initialize microkernels based on control type */
    vb9_init_control_kernels(ctrl, type);
    
    /* Map to file system */
    vb9_create_control_namespace(ctrl);
    
    return ctrl;
}

/* === Microkernel Initialization === */

void vb9_init_control_kernels(VB9Control *ctrl, int type) {
    /* Drawing kernel - parses geometric grammar for rendering */
    ctrl->draw_kernel = mallocz(sizeof(AgenticMicrokernel), 1);
    ctrl->draw_kernel->name = "draw";
    ctrl->draw_kernel->process = vb9_draw_process;
    
    /* Click kernel - autonomous event handler */
    ctrl->click_kernel = mallocz(sizeof(AgenticMicrokernel), 1);
    ctrl->click_kernel->name = "click";
    ctrl->click_kernel->process = vb9_click_process;
    
    /* Control-specific kernels based on prime type */
    switch(type) {
    case 2:  /* BUTTON - first prime */
        ctrl->click_kernel->parse = button_click_grammar;
        break;
    case 3:  /* TEXTBOX - second prime */
        ctrl->click_kernel->parse = textbox_input_grammar;
        break;
    case 5:  /* LABEL - third prime */
        ctrl->click_kernel->parse = label_static_grammar;
        break;
    case 7:  /* LISTBOX - fourth prime */
        ctrl->click_kernel->parse = listbox_select_grammar;
        break;
    }
    
    /* Attach kernels to HyperGlyph */
    ctrl->glyph->kernels[0] = ctrl->draw_kernel;
    ctrl->glyph->kernels[1] = ctrl->click_kernel;
    ctrl->glyph->nkernels = 2;
}

/* === Drawing as Geometric Computation === */

void vb9_draw_process(AgenticMicrokernel *kernel, VB9Control *ctrl) {
    /* Drawing IS computing on the visual manifold */
    
    /* 1. Parse the geometric grammar for this control */
    if(kernel->parse)
        kernel->parse(ctrl);
    
    /* 2. Compute geodesic from origin to control position */
    Point origin = (Point){0, 0};
    Point target = (Point){ctrl->rect.min.x, ctrl->rect.min.y};
    double visual_distance = ctrl->glyph->primary->distance(origin, target);
    
    /* 3. The distance determines rendering intensity! */
    int intensity = 255 - (int)(visual_distance * 2);
    if(intensity < 0) intensity = 0;
    
    /* 4. Draw the control - the visual IS the computation */
    Image *dst = ctrl->visual ? ctrl->visual : screen;
    draw(dst, ctrl->rect, allocimage(display, Rect(0,0,1,1), 
         RGB24, 1, (intensity<<16)|(intensity<<8)|intensity), nil, ZP);
    
    /* 5. Update the hyperbolic event position based on visual */
    ctrl->event_pos.x = ctrl->rect.min.x;
    ctrl->event_pos.y = ctrl->rect.min.y;
    ctrl->event_radius = hyperbolic_distance(origin, ctrl->event_pos);
}

/* === Event Routing through Hyperbolic Space === */

void vb9_route_event(VB9Control *ctrl, char *event_type, Point click_pos) {
    /* Events propagate through hyperbolic space! */
    
    /* 1. Compute hyperbolic distance from click to control */
    double h_distance = ctrl->glyph->secondary->distance(click_pos, ctrl->event_pos);
    
    /* 2. Event strength decays exponentially with hyperbolic distance */
    double strength = exp(-h_distance);
    
    /* 3. If strong enough, trigger the microkernel */
    if(strength > 0.1) {  /* threshold for activation */
        if(strcmp(event_type, "click") == 0 && ctrl->click_kernel) {
            ctrl->click_kernel->process(ctrl->click_kernel, ctrl);
        }
    }
    
    /* 4. Write event to file system (strength encoded) */
    char event_data[64];
    snprint(event_data, sizeof(event_data), "%s:%f", event_type, strength);
    write(ctrl->fd_event, event_data, strlen(event_data));
}

/* === Cross-Topology Fusion === */

void vb9_fuse_manifolds(VB9Control *ctrl) {
    /* Merge results from visual and event manifolds */
    
    /* Visual state from Euclidean manifold */
    Rectangle visual_state = ctrl->rect;
    
    /* Event state from Hyperbolic manifold */
    double event_influence = ctrl->event_radius;
    
    /* Fusion: visual position affects event influence radius */
    /* Controls near center have smaller influence (hyperbolic property) */
    double center_distance = euclidean_distance(
        (Point){visual_state.min.x, visual_state.min.y},
        (Point){screen->r.max.x/2, screen->r.max.y/2}
    );
    
    /* Update both manifolds based on fusion */
    ctrl->event_radius = event_influence * (1.0 + center_distance/100.0);
    
    /* The fusion IS the computation! */
}

/* === File System as Cognitive Interface === */

void vb9_create_control_namespace(VB9Control *ctrl) {
    char path[256];
    
    /* Base directory for control */
    snprint(path, sizeof(path), "/form/%s", ctrl->name);
    create(path, OREAD, DMDIR|0755);
    
    /* Text file - connects to visual manifold */
    snprint(path, sizeof(path), "/form/%s/text", ctrl->name);
    ctrl->fd_text = create(path, ORDWR, 0644);
    
    /* Event file - connects to hyperbolic manifold */
    snprint(path, sizeof(path), "/form/%s/event", ctrl->name);
    ctrl->fd_event = create(path, OWRITE, 0644);
    
    /* State file - cross-topology fusion result */
    snprint(path, sizeof(path), "/form/%s/state", ctrl->name);
    ctrl->fd_state = create(path, ORDWR, 0644);
}

/* === Example: Button as HyperGlyph === */

void demo_hyperglyph_button(void) {
    /* Create a button that exists in multiple topologies */
    
    Rectangle r = Rect(100, 100, 200, 130);
    VB9Control *button = vb9_hyperglyph_control_new("QuantumButton", 2, r);
    
    /* The button now exists in:
     * 1. Euclidean space for visual rendering
     * 2. Hyperbolic space for event routing
     * 3. File system for computational interface
     */
    
    /* Drawing the button computes on visual manifold */
    button->draw_kernel->process(button->draw_kernel, button);
    
    /* Clicking triggers hyperbolic event propagation */
    Point click = (Point){150, 115};
    vb9_route_event(button, "click", click);
    
    /* Cross-topology fusion updates both manifolds */
    vb9_fuse_manifolds(button);
    
    /* The button IS a cognitive domain! */
    print("Button exists across %d topologies\n", 2);
    print("Visual distance from origin: %f\n", 
          euclidean_distance((Point){0,0}, button->rect.min));
    print("Event influence radius: %f\n", button->event_radius);
}

/* 
 * This reveals the truth: VB9 controls aren't UI elements,
 * they're cognitive domains on geometric manifolds!
 * 
 * Drawing = Computing because:
 * - Visual rendering IS geodesic computation
 * - Event routing IS hyperbolic propagation
 * - File operations IS cross-topology fusion
 * 
 * Each control is a HyperGlyph with autonomous microkernels!
 */