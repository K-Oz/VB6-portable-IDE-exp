/* 9p-integration.c - Plan 9 File System Integration for VB9 */
/* This is where drawing becomes computing! */

#include "../include/vb9.h"
#include "../include/controls.h"
#include <fcall.h>
#include <thread.h>
#include <9p.h>

/* File system state for each control */
typedef struct ControlFS {
    Control *ctrl;
    char    *mountpoint;    /* /form/controlname */
    Srv     *srv;           /* 9P server */
    Tree    *tree;          /* file tree */
} ControlFS;

/* File types in control namespace */
enum {
    Qroot,
    Qtext,
    Qvalue,  
    Qvisible,
    Qenabled,
    Qposition,
    Qclick,
    Qchange
};

/* File table for each control */
Tab controltab[] = {
    "text",      Qtext,     0,      0666,
    "value",     Qvalue,    0,      0666,
    "visible",   Qvisible,  0,      0666,
    "enabled",   Qenabled,  0,      0666, 
    "position",  Qposition, 0,      0666,
    "click",     Qclick,    0,      0222,  /* write-only event */
    "change",    Qchange,   0,      0222,  /* write-only event */
    nil,
};

/* 9P file operations */
static void fsread(Req *r);
static void fswrite(Req *r);
static char* fswalk1(Fid *fid, char *name, Qid *qid);

static Srv controlsrv = {
    .read = fsread,
    .write = fswrite,
    .walk1 = fswalk1,
};

/* Serve control as Plan 9 file system - the magic happens here! */
int
vb9_serve_control(Control *ctrl, char *mountpoint)
{
    ControlFS *cfs;
    
    if(ctrl == nil || mountpoint == nil)
        return -1;
        
    cfs = mallocz(sizeof(ControlFS), 1);
    if(cfs == nil)
        return -1;
        
    cfs->ctrl = ctrl;
    cfs->mountpoint = strdup(mountpoint);
    
    /* Create file tree for this control */
    cfs->tree = alloctree(nil, nil, DMDIR|0555, nil);
    closefile(createfile(cfs->tree->root, "text", nil, 0666, nil));
    closefile(createfile(cfs->tree->root, "value", nil, 0666, nil));
    closefile(createfile(cfs->tree->root, "visible", nil, 0666, nil));
    closefile(createfile(cfs->tree->root, "enabled", nil, 0666, nil));
    closefile(createfile(cfs->tree->root, "position", nil, 0666, nil));
    closefile(createfile(cfs->tree->root, "click", nil, 0222, nil));   /* write-only */
    closefile(createfile(cfs->tree->root, "change", nil, 0222, nil)); /* write-only */
    
    /* Start 9P server */
    cfs->srv = &controlsrv;
    cfs->srv->tree = cfs->tree;
    
    threadcreate(srv9pfd, cfs->srv, 8192);
    
    return 0;
}

/* Read from control files - expose computational state */
static void
fsread(Req *r)
{
    Control *ctrl = r->fid->file->aux;  /* Control stored in file aux */
    char buf[256];
    int n = 0;
    
    if(ctrl == nil) {
        respond(r, "no control");
        return;
    }
    
    switch((uintptr)r->fid->qid.path) {
    case Qtext:
        n = snprint(buf, sizeof(buf), "%s", ctrl->text ? ctrl->text : "");
        break;
        
    case Qvalue:
        n = snprint(buf, sizeof(buf), "%s", ctrl->value ? ctrl->value : "");
        break;
        
    case Qvisible:
        n = snprint(buf, sizeof(buf), "%d", ctrl->visible);
        break;
        
    case Qenabled:
        n = snprint(buf, sizeof(buf), "%d", ctrl->enabled);
        break;
        
    case Qposition:
        n = snprint(buf, sizeof(buf), "%d,%d,%d,%d", 
                   ctrl->r.min.x, ctrl->r.min.y, 
                   ctrl->r.max.x, ctrl->r.max.y);
        break;
        
    default:
        respond(r, "file not readable");
        return;
    }
    
    readbuf(r, buf, n);
    respond(r, nil);
}

/* Write to control files - modify computational state */
static void
fswrite(Req *r)
{
    Control *ctrl = r->fid->file->aux;
    char *data;
    int x, y, x2, y2;
    
    if(ctrl == nil) {
        respond(r, "no control");
        return;
    }
    
    data = r->ifcall.data;
    
    switch((uintptr)r->fid->qid.path) {
    case Qtext:
        /* Setting text property */
        if(ctrl->text) free(ctrl->text);
        ctrl->text = malloc(r->ifcall.count + 1);
        memmove(ctrl->text, data, r->ifcall.count);
        ctrl->text[r->ifcall.count] = 0;
        
        /* Trigger redraw */
        if(g_runtime && g_runtime->activeform)
            vb9_draw_form(g_runtime->activeform);
        break;
        
    case Qvisible:
        ctrl->visible = (data[0] == '1');
        if(g_runtime && g_runtime->activeform)
            vb9_draw_form(g_runtime->activeform);
        break;
        
    case Qenabled:
        ctrl->enabled = (data[0] == '1');
        break;
        
    case Qposition:
        /* Parse "x,y,x2,y2" */
        if(tokenize(data, ",") == 4) {
            x = atoi(data);
            y = atoi(data + strlen(data) + 1);
            x2 = atoi(data + strlen(data) + strlen(data) + 2);
            y2 = atoi(data + strlen(data) + strlen(data) + strlen(data) + 3);
            ctrl->r = Rect(x, y, x2, y2);
            
            if(g_runtime && g_runtime->activeform)
                vb9_draw_form(g_runtime->activeform);
        }
        break;
        
    case Qclick:
        /* Execute click event - THIS IS THE MAGIC! */
        /* Writing to click file executes the button! */
        if(ctrl->click)
            ctrl->click(ctrl);
        break;
        
    case Qchange:
        /* Execute change event */
        if(ctrl->change)
            ctrl->change(ctrl);
        break;
        
    default:
        respond(r, "file not writable");
        return;
    }
    
    r->ofcall.count = r->ifcall.count;
    respond(r, nil);
}

/* Walk to file in control namespace */
static char*
fswalk1(Fid *fid, char *name, Qid *qid)
{
    File *f;
    
    f = walkfile(fid->file, name);
    if(f == nil)
        return "file not found";
        
    fid->file = f;
    *qid = f->qid;
    return nil;
}

/* Handle 9P requests - the computational interface */
void
vb9_handle_9p_request(Control *ctrl, char *op, char *data)
{
    /* This would be called by the 9P server */
    /* op is the operation (read/write), data is the content */
    
    if(strcmp(op, "click") == 0) {
        /* Someone wrote to /form/controlname/click */
        if(ctrl->click)
            ctrl->click(ctrl);
    }
    else if(strcmp(op, "text") == 0) {
        /* Someone wrote new text */
        vb9_control_set_text(ctrl, data);
    }
    /* etc... */
}

/* Utility: Create namespace for form */
int
vb9_create_form_namespace(VB9Form *form, char *namespace)
{
    char path[256];
    int i;
    
    if(form == nil || namespace == nil)
        return -1;
    
    /* Create base directory */
    snprint(path, sizeof(path), "%s/%s", namespace, form->name);
    
    /* Serve each control as subdirectory */
    for(i = 0; i < form->ncontrols; i++) {
        snprint(path, sizeof(path), "%s/%s/%s", 
                namespace, form->name, form->controls[i].name);
        vb9_serve_control(&form->controls[i], path);
    }
    
    return 0;
}

/* Example usage:
 * 
 * VB9Form *form = vb9_form_new("Calculator", "VB9 Calc");
 * vb9_button_new(form, "btn_1", "1");
 * vb9_textbox_new(form, "display", "0");
 * 
 * // Create namespace: /form/Calculator/
 * vb9_create_form_namespace(form, "/form");
 * 
 * // Now you can:
 * echo "2" > /form/Calculator/display/text
 * echo "click" > /form/Calculator/btn_1/click  
 * cat /form/Calculator/display/text
 * 
 * // The form updates in real-time!
 * // Drawing IS computing!
 */