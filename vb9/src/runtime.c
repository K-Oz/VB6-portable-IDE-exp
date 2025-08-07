/* runtime.c - Minimal VB9 runtime - Drawing IS Computing */

#include "../include/vb9.h"
#include "../include/controls.h"

/* Global runtime context */
static VB9Runtime *g_runtime = nil;

/* Initialize VB9 runtime - connect to the computation substrate */
VB9Runtime*
vb9_init(void)
{
    VB9Runtime *rt;
    
    rt = mallocz(sizeof(VB9Runtime), 1);
    if(rt == nil)
        sysfatal("vb9_init: malloc failed: %r");
    
    /* Connect to /dev/draw - this IS the computational substrate */
    if(initdraw(nil, nil, "VB9 Runtime") < 0)
        sysfatal("vb9_init: initdraw failed: %r");
    
    rt->display = display;
    rt->nforms = 0;
    rt->activeform = nil;
    rt->defaultfont = font;
    rt->background = display->white;
    
    /* Initialize event handling */
    einit(Emouse|Ekeyboard);
    
    g_runtime = rt;
    return rt;
}

/* Shutdown runtime - disconnect from computation substrate */
void
vb9_shutdown(VB9Runtime *rt)
{
    int i;
    
    if(rt == nil)
        return;
        
    /* Destroy all forms */
    for(i = 0; i < rt->nforms; i++) {
        if(rt->forms[i])
            vb9_form_destroy(rt->forms[i]);
    }
    
    /* Disconnect from display */
    closedisplay(rt->display);
    
    free(rt);
    g_runtime = nil;
}

/* Main event loop - render the computation */
void
vb9_run(VB9Runtime *rt)
{
    Event ev;
    int running = 1;
    
    if(rt == nil)
        return;
        
    while(running) {
        switch(event(&ev)) {
        case Emouse:
            if(ev.mouse.buttons & 1)  /* Left click */
                vb9_handle_click(rt, ev.mouse.xy);
            break;
            
        case Ekeyboard:
            if(ev.kbdc == 'q' || ev.kbdc == Kdel)  /* Quit */
                running = 0;
            else
                vb9_handle_key(rt, ev.kbdc);
            break;
        }
    }
}

/* Create new form - a computational container */
VB9Form*
vb9_form_new(char *name, char *title)
{
    VB9Form *form;
    
    form = mallocz(sizeof(VB9Form), 1);
    if(form == nil)
        return nil;
        
    form->name = strdup(name);
    form->title = strdup(title);
    form->r = Rect(50, 50, 400, 300);  /* Default size like VB6 */
    form->ncontrols = 0;
    form->font = g_runtime->defaultfont;
    form->visible = 0;
    
    /* Add to runtime */
    if(g_runtime && g_runtime->nforms < 16) {
        g_runtime->forms[g_runtime->nforms] = form;
        g_runtime->nforms++;
    }
    
    return form;
}

/* Show form - render the computation into visibility */
void
vb9_form_show(VB9Form *form)
{
    if(form == nil)
        return;
        
    form->visible = 1;
    g_runtime->activeform = form;
    
    /* Create window image - this IS the computational space */
    form->image = allocwindow(display, screen, form->r, Refbackup, DNofill);
    if(form->image == nil)
        sysfatal("vb9_form_show: allocwindow failed: %r");
        
    /* Draw the form - rendering IS execution */
    vb9_draw_form(form);
}

/* Hide form */
void
vb9_form_hide(VB9Form *form)
{
    if(form == nil)
        return;
        
    form->visible = 0;
    if(form->image) {
        freeimage(form->image);
        form->image = nil;
    }
    
    if(g_runtime->activeform == form)
        g_runtime->activeform = nil;
}

/* Destroy form */
void
vb9_form_destroy(VB9Form *form)
{
    int i;
    
    if(form == nil)
        return;
        
    vb9_form_hide(form);
    
    /* Free controls */
    for(i = 0; i < form->ncontrols; i++) {
        if(form->controls[i].name)
            free(form->controls[i].name);
        if(form->controls[i].text)
            free(form->controls[i].text);
        if(form->controls[i].value)
            free(form->controls[i].value);
    }
    
    free(form->name);
    free(form->title);
    free(form);
}

/* Handle mouse clicks - interaction with the computational substrate */
void
vb9_handle_click(VB9Runtime *rt, Point p)
{
    VB9Form *form;
    Control *ctrl;
    int i;
    
    if(rt == nil || rt->activeform == nil)
        return;
        
    form = rt->activeform;
    
    /* Find control under click point */
    for(i = 0; i < form->ncontrols; i++) {
        ctrl = &form->controls[i];
        if(ptinrect(p, ctrl->r) && ctrl->visible && ctrl->enabled) {
            if(ctrl->click)
                ctrl->click(ctrl);
            break;
        }
    }
}

/* Handle keyboard input */
void
vb9_handle_key(VB9Runtime *rt, Rune r)
{
    /* TODO: Handle keyboard input for active textbox */
    USED(rt);
    USED(r);
}

/* Draw form - render the computational structure */
void
vb9_draw_form(VB9Form *form)
{
    int i;
    
    if(form == nil || form->image == nil)
        return;
        
    /* Clear form background */
    draw(form->image, form->image->r, display->white, nil, ZP);
    
    /* Draw title bar - simple like VB6 */
    string(form->image, addpt(form->image->r.min, Pt(5, 5)),
           display->black, ZP, form->font, form->title);
    
    /* Draw all controls - each control renders its computation */
    for(i = 0; i < form->ncontrols; i++) {
        if(form->controls[i].visible && form->controls[i].draw)
            form->controls[i].draw(&form->controls[i]);
    }
    
    /* Flush to screen */
    flushimage(display, 1);
}

/* Redraw all visible forms */
void
vb9_redraw_all(VB9Runtime *rt)
{
    int i;
    
    if(rt == nil)
        return;
        
    for(i = 0; i < rt->nforms; i++) {
        if(rt->forms[i] && rt->forms[i]->visible)
            vb9_draw_form(rt->forms[i]);
    }
}

/* Utility functions */
Rectangle
vb9_rect(int x, int y, int w, int h)
{
    return Rect(x, y, x+w, y+h);
}

Point
vb9_point(int x, int y)
{
    return Pt(x, y);
}

Point
vb9_string_size(Font *font, char *s)
{
    return Pt(stringwidth(font, s), font->height);
}