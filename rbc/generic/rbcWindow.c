/*
 * rbcWindow.c --
 *
 *      This module implements additional window functionality for
 *      the rbc toolkit, such as transparent Tk windows,
 *      and reparenting Tk windows.
 *
 * Copyright (c) 2009 Samuel Green, Nicholas Hudson, Stanton Sievers, Jarrod Stormo
 * All rights reserved.
 *
 * See "license.terms" for details.
 */

#include "rbcInt.h"

#include <X11/Xlib.h>
#ifndef WIN32
#include <X11/Xproto.h>
#endif

typedef struct TkIdStackStruct TkIdStack;
typedef struct TkSelectionInfoStruct TkSelectionInfo;
typedef struct TkClipboardTargetStruct TkClipboardTarget;

#ifndef WIN32
typedef struct TkWindowStruct TkWindow;
#endif
typedef struct TkWindowEventStruct TkWindowEvent;
typedef struct TkSelHandlerStruct TkSelHandler;
typedef struct TkWinInfoStruct TkWinInfo;
typedef struct TkClassProcsStruct TkClassProcs;
typedef struct TkWindowPrivateStruct TkWindowPrivate;
typedef struct TkWmInfoStruct TkWmInfo;

#ifdef XNQueryInputStyle
#define TK_USE_INPUT_METHODS
#endif

/*
 * This defines whether we should try to use XIM over-the-spot style
 * input.  Allow users to override it.  It is a much more elegant use
 * of XIM, but uses a bit more memory.
 */
#ifndef TK_XIM_SPOT
#   define TK_XIM_SPOT	1
#endif

#ifndef TK_REPARENTED
#define TK_REPARENTED 	0
#endif

#ifndef WIN32
typedef struct TkStressedCmapStruct TkStressedCmap;
typedef struct TkGrabEventStruct TkGrabEvent;
typedef struct TkColormapStruct TkColormap;
typedef struct TkMainInfoStruct TkMainInfo;
typedef struct TkEventHandlerStruct TkEventHandler;
typedef struct TkCaret {
    struct TkWindow *winPtr;	/* the window on which we requested caret
				 * placement */
    int x;			/* relative x coord of the caret */
    int y;			/* relative y coord of the caret */
    int height;			/* specified height of the window */
} TkCaret;
typedef struct TkDisplayStruct {
    Display *display;		/* Xlib's info about display. */
    struct TkDisplayStruct *nextPtr; /* Next in list of all displays. */
    char *name;			/* Name of display (with any screen
				 * identifier removed).  Malloc-ed. */
    Time lastEventTime;		/* Time of last event received for this
				 * display. */

    /*
     * Information used primarily by tk3d.c:
     */

    int borderInit;		/* 0 means borderTable needs initializing. */
    Tcl_HashTable borderTable;	/* Maps from color name to TkBorder
				 * structure. */

    /*
     * Information used by tkAtom.c only:
     */

    int atomInit;		/* 0 means stuff below hasn't been
				 * initialized yet. */
    Tcl_HashTable nameTable;	/* Maps from names to Atom's. */
    Tcl_HashTable atomTable;	/* Maps from Atom's back to names. */

    /*
     * Information used primarily by tkBind.c:
     */

    int bindInfoStale;		/* Non-zero means the variables in this
				 * part of the structure are potentially
				 * incorrect and should be recomputed. */
    unsigned int modeModMask;	/* Has one bit set to indicate the modifier
				 * corresponding to "mode shift".  If no
				 * such modifier, than this is zero. */
    unsigned int metaModMask;	/* Has one bit set to indicate the modifier
				 * corresponding to the "Meta" key.  If no
				 * such modifier, then this is zero. */
    unsigned int altModMask;	/* Has one bit set to indicate the modifier
				 * corresponding to the "Meta" key.  If no
				 * such modifier, then this is zero. */
    enum {
	LU_IGNORE, LU_CAPS, LU_SHIFT
    } lockUsage;		/* Indicates how to interpret lock modifier. */
    int numModKeyCodes;		/* Number of entries in modKeyCodes array
				 * below. */
    KeyCode *modKeyCodes;	/* Pointer to an array giving keycodes for
				 * all of the keys that have modifiers
				 * associated with them.  Malloc'ed, but
				 * may be NULL. */

    /*
     * Information used by tkBitmap.c only:
     */

    int bitmapInit;		/* 0 means tables above need initializing. */
    int bitmapAutoNumber;	/* Used to number bitmaps. */
    Tcl_HashTable bitmapNameTable;
				/* Maps from name of bitmap to the first
				 * TkBitmap record for that name. */
    Tcl_HashTable bitmapIdTable;/* Maps from bitmap id to the TkBitmap
				 * structure for the bitmap. */
    Tcl_HashTable bitmapDataTable;
				/* Used by Tk_GetBitmapFromData to map from
				 * a collection of in-core data about a
				 * bitmap to a reference giving an auto-
				 * matically-generated name for the bitmap. */

    /*
     * Information used by tkCanvas.c only:
     */

    int numIdSearches;
    int numSlowSearches;

    /*
     * Used by tkColor.c only:
     */

    int colorInit;		/* 0 means color module needs initializing. */
    TkStressedCmap *stressPtr;	/* First in list of colormaps that have
				 * filled up, so we have to pick an
				 * approximate color. */
    Tcl_HashTable colorNameTable;
				/* Maps from color name to TkColor structure
				 * for that color. */
    Tcl_HashTable colorValueTable;
				/* Maps from integer RGB values to TkColor
				 * structures. */

    /*
     * Used by tkCursor.c only:
     */

    int cursorInit;		/* 0 means cursor module need initializing. */
    Tcl_HashTable cursorNameTable;
				/* Maps from a string name to a cursor to the
				 * TkCursor record for the cursor. */
    Tcl_HashTable cursorDataTable;
				/* Maps from a collection of in-core data
				 * about a cursor to a TkCursor structure. */
    Tcl_HashTable cursorIdTable;
				/* Maps from a cursor id to the TkCursor
				 * structure for the cursor. */
    char cursorString[20];	/* Used to store a cursor id string. */
    Font cursorFont;		/* Font to use for standard cursors.
				 * None means font not loaded yet. */

    /*
     * Information used by tkError.c only:
     */

    struct TkErrorHandler *errorPtr;
				/* First in list of error handlers
				 * for this display.  NULL means
				 * no handlers exist at present. */
    int deleteCount;		/* Counts # of handlers deleted since
				 * last time inactive handlers were
				 * garbage-collected.  When this number
				 * gets big, handlers get cleaned up. */

    /*
     * Used by tkEvent.c only:
     */

    struct TkWindowEvent *delayedMotionPtr;
				/* Points to a malloc-ed motion event
				 * whose processing has been delayed in
				 * the hopes that another motion event
				 * will come along right away and we can
				 * merge the two of them together.  NULL
				 * means that there is no delayed motion
				 * event. */

    /*
     * Information used by tkFocus.c only:
     */

    int focusDebug;		/* 1 means collect focus debugging
				 * statistics. */
    struct TkWindow *implicitWinPtr;
				/* If the focus arrived at a toplevel window
				 * implicitly via an Enter event (rather
				 * than via a FocusIn event), this points
				 * to the toplevel window.  Otherwise it is
				 * NULL. */
    struct TkWindow *focusPtr;	/* Points to the window on this display that
				 * should be receiving keyboard events.  When
				 * multiple applications on the display have
				 * the focus, this will refer to the
				 * innermost window in the innermost
				 * application.  This information isn't used
				 * under Unix or Windows, but it's needed on
				 * the Macintosh. */

    /*
     * Information used by tkGC.c only:
     */

    Tcl_HashTable gcValueTable;	/* Maps from a GC's values to a TkGC structure
				 * describing a GC with those values. */
    Tcl_HashTable gcIdTable;	/* Maps from a GC to a TkGC. */
    int gcInit;			/* 0 means the tables below need
				 * initializing. */

    /*
     * Information used by tkGeometry.c only:
     */

    Tcl_HashTable maintainHashTable;
				/* Hash table that maps from a master's
				 * Tk_Window token to a list of slaves
				 * managed by that master. */
    int geomInit;

    /*
     * Information used by tkGet.c only:
     */

    Tcl_HashTable uidTable;	/* Stores all Tk_Uids used in a thread. */
    int uidInit;		/* 0 means uidTable needs initializing. */

    /*
     * Information used by tkGrab.c only:
     */

    struct TkWindow *grabWinPtr;
				/* Window in which the pointer is currently
				 * grabbed, or NULL if none. */
    struct TkWindow *eventualGrabWinPtr;
				/* Value that grabWinPtr will have once the
				 * grab event queue (below) has been
				 * completely emptied. */
    struct TkWindow *buttonWinPtr;
				/* Window in which first mouse button was
				 * pressed while grab was in effect, or NULL
				 * if no such press in effect. */
    struct TkWindow *serverWinPtr;
				/* If no application contains the pointer then
				 * this is NULL.  Otherwise it contains the
				 * last window for which we've gotten an
				 * Enter or Leave event from the server (i.e.
				 * the last window known to have contained
				 * the pointer).  Doesn't reflect events
				 * that were synthesized in tkGrab.c. */
    TkGrabEvent *firstGrabEventPtr;
				/* First in list of enter/leave events
				 * synthesized by grab code.  These events
				 * must be processed in order before any other
				 * events are processed.  NULL means no such
				 * events. */
    TkGrabEvent *lastGrabEventPtr;
				/* Last in list of synthesized events, or NULL
				 * if list is empty. */
    int grabFlags;		/* Miscellaneous flag values.  See definitions
				 * in tkGrab.c. */

    /*
     * Information used by tkGrid.c only:
     */

    int gridInit;		/* 0 means table below needs initializing. */
    Tcl_HashTable gridHashTable;/* Maps from Tk_Window tokens to
				 * corresponding Grid structures. */

    /*
     * Information used by tkImage.c only:
     */

    int imageId;		/* Value used to number image ids. */

    /*
     * Information used by tkMacWinMenu.c only:
     */

    int postCommandGeneration;

    /*
     * Information used by tkOption.c only.
     */



    /*
     * Information used by tkPack.c only.
     */

    int packInit;		/* 0 means table below needs initializing. */
    Tcl_HashTable packerHashTable;
				/* Maps from Tk_Window tokens to
				 * corresponding Packer structures. */


    /*
     * Information used by tkPlace.c only.
     */

    int placeInit;		/* 0 means tables below need initializing. */
    Tcl_HashTable masterTable;	/* Maps from Tk_Window toke to the Master
				 * structure for the window, if it exists. */
    Tcl_HashTable slaveTable;	/* Maps from Tk_Window toke to the Slave
				 * structure for the window, if it exists. */

    /*
     * Information used by tkSelect.c and tkClipboard.c only:
     */


    struct TkSelectionInfo *selectionInfoPtr;
    /* First in list of selection information
				 * records.  Each entry contains information
				 * about the current owner of a particular
				 * selection on this display. */
    Atom multipleAtom;		/* Atom for MULTIPLE.  None means
				 * selection stuff isn't initialized. */
    Atom incrAtom;		/* Atom for INCR. */
    Atom targetsAtom;		/* Atom for TARGETS. */
    Atom timestampAtom;		/* Atom for TIMESTAMP. */
    Atom textAtom;		/* Atom for TEXT. */
    Atom compoundTextAtom;	/* Atom for COMPOUND_TEXT. */
    Atom applicationAtom;	/* Atom for TK_APPLICATION. */
    Atom windowAtom;		/* Atom for TK_WINDOW. */
    Atom clipboardAtom;		/* Atom for CLIPBOARD. */
    Atom utf8Atom;
    Tk_Window clipWindow;	/* Window used for clipboard ownership and to
				 * retrieve selections between processes. NULL
				 * means clipboard info hasn't been
				 * initialized. */
    int clipboardActive;	/* 1 means we currently own the clipboard
				 * selection, 0 means we don't. */
    struct TkMainInfo *clipboardAppPtr;
				/* Last application that owned clipboard. */
    struct TkClipboardTarget *clipTargetPtr;
				/* First in list of clipboard type information
				 * records.  Each entry contains information
				 * about the buffers for a given selection
				 * target. */

    /*
     * Information used by tkSend.c only:
     */

    Tk_Window commTkwin;	/* Window used for communication
				 * between interpreters during "send"
				 * commands.  NULL means send info hasn't
				 * been initialized yet. */
    Atom commProperty;		/* X's name for comm property. */
    Atom registryProperty;	/* X's name for property containing
				 * registry of interpreter names. */
    Atom appNameProperty;	/* X's name for property used to hold the
				 * application name on each comm window. */

    /*
     * Information used by tkXId.c only:
     */

    struct TkIdStack *idStackPtr;
				/* First in list of chunks of free resource
				 * identifiers, or NULL if there are no free
				 * resources. */
    XID(*defaultAllocProc) (Display *display);
				/* Default resource allocator for display. */
    struct TkIdStack *windowStackPtr;
				/* First in list of chunks of window
				 * identifers that can't be reused right
				 * now. */

    Tcl_TimerToken idCleanupScheduled;
				/* If set, it means a call to WindowIdCleanup
				 * has already been scheduled, 0 means it
				 * hasn't. */
    /*
     * Information used by tkUnixWm.c and tkWinWm.c only:
     */
    struct TkWmInfo *firstWmPtr; /* Points to first top-level window. */
    struct TkWmInfo *foregroundWmPtr;
				/* Points to the foreground window. */

    /*
     * Information maintained by tkWindow.c for use later on by tkXId.c:
     */


    int destroyCount;		/* Number of Tk_DestroyWindow operations
				 * in progress. */
    unsigned long lastDestroyRequest;
				/* Id of most recent XDestroyWindow request;
				 * can re-use ids in windowStackPtr when
				 * server has seen this request and event
				 * queue is empty. */

    /*
     * Information used by tkVisual.c only:
     */

    TkColormap *cmapPtr;	/* First in list of all non-default colormaps
				 * allocated for this display. */

    /*
     * Miscellaneous information:
     */

#ifdef TK_USE_INPUT_METHODS
    XIM inputMethod;		/* Input method for this display */
#if TK_XIM_SPOT
    XFontSet inputXfs;		/* XFontSet cached for over-the-spot XIM. */
#endif /* TK_XIM_SPOT */
#endif /* TK_USE_INPUT_METHODS */
    Tcl_HashTable winTable;	/* Maps from X window ids to TkWindow ptrs. */
    int refCount;		/* Reference count of how many Tk applications
                                 * are using this display. Used to clean up
                                 * the display when we no longer have any
                                 * Tk applications using it.
                                 */
    /*
     * The following field were all added for Tk8.3
     */
    int mouseButtonState;       /* current mouse button state for this
                                 * display */
    Window warpWindow;
    int warpX;
    int warpY;
    /*
     * The following field(s) were all added for Tk8.4
     */
    long deletionEpoch;		/* Incremented by window deletions */
    unsigned int flags;		/* Various flag values:  these are all
				 * defined in below. */
    TkCaret caret;		/* information about the caret for this
				 * display.  This is not a pointer. */
} TkDisplay;

#endif

struct TkWindowStruct {
    Display *display;
    TkDisplay *dispPtr;
    int screenNum;
    Visual *visual;
    int depth;
    Window window;
    TkWindow *childList;
    TkWindow *lastChildPtr;
    TkWindow *parentPtr;
    TkWindow *nextPtr;
    TkMainInfo *infoPtr;
    char *pathName;
    Tk_Uid nameUid;
    Tk_Uid classUid;
    XWindowChanges changes;
    unsigned int dirtyChanges;
    XSetWindowAttributes atts;
    unsigned long dirtyAtts;
    unsigned int flags;
    TkEventHandler *handlerList;
#ifdef TK_USE_INPUT_METHODS
    XIC inputContext;
#endif /* TK_USE_INPUT_METHODS */
    ClientData *tagPtr;
    int nTags;
    int optionLevel;
    TkSelHandler *selHandlerList;
    Tk_GeomMgr *geomMgrPtr;
    ClientData geomData;
    int reqWidth, reqHeight;
    int internalBorderWidth;
    TkWinInfo *wmInfoPtr;
    TkClassProcs *classProcsPtr;
    ClientData instanceData;
    TkWindowPrivate *privatePtr;
};

static void DoConfigureNotify (Tk_FakeWin *winPtr);
static void UnlinkWindow (TkWindow *winPtr);

#ifdef WIN32
/*
 *----------------------------------------------------------------------
 *
 * GetWindowHandle --
 *
 *      Returns the XID for the Tk_Window given.  Starting in Tk 8.0,
 *      the toplevel widgets are wrapped by another window.
 *      Currently there's no way to get at that window, other than
 *      what is done here: query the X window hierarchy and grab the
 *      parent.
 *
 * Results:
 *      Returns the X Window ID of the widget.  If it's a toplevel,
 *      then the XID of the wrapper is returned.
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *----------------------------------------------------------------------
 */
static HWND
GetWindowHandle (Tk_Window tkwin)
{
    HWND hWnd;
    Window window;

    window = Tk_WindowId(tkwin);
    if (window == None) {
        Tk_MakeWindowExist(tkwin);
    }
    hWnd = Tk_GetHWND(Tk_WindowId(tkwin));
    if (Tk_IsTopLevel(tkwin)) {
        hWnd = GetParent(hWnd);
    }
    return hWnd;
}

#else

/*
 *--------------------------------------------------------------
 *
 * Rbc_GetParent --
 *
 *      TODO: Description
 *
 * Results:
 *      TODO: Results
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *--------------------------------------------------------------
 */
Window
Rbc_GetParent(
    Display *display,
    Window window)
{
    Window root, parent;
    Window *dummy;
    unsigned int count;

    if (XQueryTree(display, window, &root, &parent, &dummy, &count) > 0) {
        XFree(dummy);
        return parent;
    }
    return None;
}

/*
 *--------------------------------------------------------------
 *
 * GetWindowId --
 *
 *      TODO: Description
 *
 * Results:
 *      TODO: Results
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *--------------------------------------------------------------
 */
static Window
GetWindowId (Tk_Window tkwin)
{
    Window window;

    Tk_MakeWindowExist(tkwin);
    window = Tk_WindowId(tkwin);
#if (TK_MAJOR_VERSION > 4)
    if (Tk_IsTopLevel(tkwin)) {
        Window parent;

        parent = Rbc_GetParent(Tk_Display(tkwin), window);
        if (parent != None) {
            window = parent;
        }
        window = parent;
    }
#endif /* TK_MAJOR_VERSION > 4 */
    return window;
}

#endif /* WIN32 */

/*
 *----------------------------------------------------------------------
 *
 * DoConfigureNotify --
 *
 *      Generate a ConfigureNotify event describing the current
 *      configuration of a window.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      An event is generated and processed by Tk_HandleEvent.
 *
 *----------------------------------------------------------------------
 */
static void
DoConfigureNotify(
    Tk_FakeWin *winPtr) /* Window whose configuration was just
                         * changed. */
{
    XEvent event;

    event.type = ConfigureNotify;
    event.xconfigure.serial = LastKnownRequestProcessed(winPtr->display);
    event.xconfigure.send_event = False;
    event.xconfigure.display = winPtr->display;
    event.xconfigure.event = winPtr->window;
    event.xconfigure.window = winPtr->window;
    event.xconfigure.x = winPtr->changes.x;
    event.xconfigure.y = winPtr->changes.y;
    event.xconfigure.width = winPtr->changes.width;
    event.xconfigure.height = winPtr->changes.height;
    event.xconfigure.border_width = winPtr->changes.border_width;
    if (winPtr->changes.stack_mode == Above) {
        event.xconfigure.above = winPtr->changes.sibling;
    } else {
        event.xconfigure.above = None;
    }
    event.xconfigure.override_redirect = winPtr->atts.override_redirect;
    Tk_HandleEvent(&event);
}

/*
 *--------------------------------------------------------------
 *
 * Rbc_MakeTransparentWindowExist --
 *
 *      Similar to Tk_MakeWindowExist but instead creates a
 *      transparent window to block for user events from
 *      sibling windows.
 *
 *      Differences from Tk_MakeWindowExist.
 *
 *        1. This is always a "busy" window. There's never a
 *           platform-specific class procedure to execute
 *           instead.
 *        2. The window is transparent and never will contain
 *           children, so colormap information is irrelevant.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      When the procedure returns, the internal window
 *      associated with tkwin is guaranteed to exist.  This
 *      may require the window's ancestors to be created too.
 *
 *--------------------------------------------------------------
 */
void
Rbc_MakeTransparentWindowExist(
    Tk_Window tkwin, /* Token for window. */
    Window parent,   /* Parent window. */
    int isBusy)
{
    TkWindow *winPtr = (TkWindow *) tkwin;
    TkWindow *winPtr2;
    Tcl_HashEntry *hPtr;
    int notUsed;
    TkDisplay *dispPtr;
#ifdef WIN32
    HWND hParent;
    int style;
    DWORD exStyle;
    HWND hWnd;
#else
    long int mask;
#endif /* WIN32 */

    if (winPtr->window != None) {
        return;			/* Window already exists. */
    }
#ifdef notdef
    if ((winPtr->parentPtr == NULL) || (winPtr->flags & TK_TOP_LEVEL)) {
        parent = XRootWindow(winPtr->display, winPtr->screenNum);
        /* TODO: Make the entire screen busy */
    } else {
        if (Tk_WindowId(winPtr->parentPtr) == None) {
            Tk_MakeWindowExist((Tk_Window)winPtr->parentPtr);
        }
    }
#endif

    /* Create a transparent window and put it on top.  */

#ifdef WIN32
    hParent = (HWND) parent;
    style = (WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
    exStyle = (WS_EX_TRANSPARENT | WS_EX_TOPMOST);
#define TK_WIN_CHILD_CLASS_NAME_A "TkChild"
    hWnd = CreateWindowExA(exStyle, TK_WIN_CHILD_CLASS_NAME_A, NULL, style,
                          Tk_X(tkwin), Tk_Y(tkwin), Tk_Width(tkwin), Tk_Height(tkwin),
                          hParent, NULL, (HINSTANCE) Tk_GetHINSTANCE(), NULL);
    winPtr->window = Tk_AttachHWND(tkwin, hWnd);
#else
    mask = (!isBusy) ? 0 : (CWDontPropagate | CWEventMask);
    /* Ignore the important events while the window is mapped.  */
#define USER_EVENTS  (EnterWindowMask | LeaveWindowMask | KeyPressMask | \
	KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask)
#define PROP_EVENTS  (KeyPressMask | KeyReleaseMask | ButtonPressMask | \
	ButtonReleaseMask | PointerMotionMask)

    winPtr->atts.do_not_propagate_mask = PROP_EVENTS;
    winPtr->atts.event_mask = USER_EVENTS;
    winPtr->changes.border_width = 0;
    winPtr->depth = 0;

    winPtr->window = XCreateWindow(winPtr->display, parent,
                                   winPtr->changes.x, winPtr->changes.y,
                                   (unsigned)winPtr->changes.width,	/* width */
                                   (unsigned)winPtr->changes.height,	/* height */
                                   (unsigned)winPtr->changes.border_width,	/* border_width */
                                   winPtr->depth,		/* depth */
                                   InputOnly,		/* class */
                                   winPtr->visual,		/* visual */
                                   mask,			/* valuemask */
                                   &(winPtr->atts)		/* attributes */ );
#endif /* WIN32 */

    dispPtr = winPtr->dispPtr;
    hPtr = Tcl_CreateHashEntry(&(dispPtr->winTable), (char *)winPtr->window,
                               &notUsed);
    Tcl_SetHashValue(hPtr, winPtr);
    winPtr->dirtyAtts = 0;
    winPtr->dirtyChanges = 0;
#ifdef TK_USE_INPUT_METHODS
    winPtr->inputContext = NULL;
#endif /* TK_USE_INPUT_METHODS */
    if (!(winPtr->flags & TK_TOP_LEVEL)) {
        /*
         * If any siblings higher up in the stacking order have already
         * been created then move this window to its rightful position
         * in the stacking order.
         *
         * NOTE: this code ignores any changes anyone might have made
         * to the sibling and stack_mode field of the window's attributes,
         * so it really isn't safe for these to be manipulated except
         * by calling Tk_RestackWindow.
         */
        for (winPtr2 = winPtr->nextPtr; winPtr2 != NULL;
                winPtr2 = winPtr2->nextPtr) {
            if ((winPtr2->window != None) && !(winPtr2->flags & TK_TOP_LEVEL)) {
                XWindowChanges changes;
                changes.sibling = winPtr2->window;
                changes.stack_mode = Below;
                XConfigureWindow(winPtr->display, winPtr->window,
                                 CWSibling | CWStackMode, &changes);
                break;
            }
        }
    }

    /*
     * Issue a ConfigureNotify event if there were deferred configuration
     * changes (but skip it if the window is being deleted;  the
     * ConfigureNotify event could cause problems if we're being called
     * from Tk_DestroyWindow under some conditions).
     */
    if ((winPtr->flags & TK_NEED_CONFIG_NOTIFY)
            && !(winPtr->flags & TK_ALREADY_DEAD)) {
        winPtr->flags &= ~TK_NEED_CONFIG_NOTIFY;
        DoConfigureNotify((Tk_FakeWin *) tkwin);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Rbc_FindChild --
 *
 *      Performs a linear search for the named child window in a given
 *      parent window.
 *
 *      This can be done via Tcl, but not through Tk's C API.  It's
 *      simple enough, if you peek into the Tk_Window structure.
 *
 * Results:
 *      The child Tk_Window. If the named child can't be found, NULL
 *      is returned.
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *----------------------------------------------------------------------
 */
Tk_Window
Rbc_FindChild(
    Tk_Window parent,
    char *name)
{
    register TkWindow *winPtr;
    TkWindow *parentPtr = (TkWindow *)parent;

    for (winPtr = parentPtr->childList; winPtr != NULL;
            winPtr = winPtr->nextPtr) {
        if (strcmp(name, winPtr->nameUid) == 0) {
            return (Tk_Window)winPtr;
        }
    }
    return NULL;
}

/*
 *----------------------------------------------------------------------
 *
 * Rbc_FirstChildWindow --
 *
 *      Performs a linear search for the named child window in a given
 *      parent window.
 *
 *      This can be done via Tcl, but not through Tk's C API.  It's
 *      simple enough, if you peek into the Tk_Window structure.
 *
 * Results:
 *      The child Tk_Window. If the named child can't be found, NULL
 *      is returned.
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *----------------------------------------------------------------------
 */
Tk_Window
Rbc_FirstChild (Tk_Window parent)
{
    TkWindow *parentPtr = (TkWindow *)parent;
    return (Tk_Window)parentPtr->childList;
}

/*
 *--------------------------------------------------------------
 *
 * Rbc_NextChild --
 *
 *      TODO: Description
 *
 * Results:
 *      The child Tk_Window. If the named child can't be found, NULL
 *      is returned.
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *--------------------------------------------------------------
 */
Tk_Window
Rbc_NextChild (Tk_Window tkwin)
{
    TkWindow *winPtr = (TkWindow *)tkwin;

    if (winPtr == NULL) {
        return NULL;
    }
    return (Tk_Window)winPtr->nextPtr;
}

/*
 *----------------------------------------------------------------------
 *
 * UnlinkWindow --
 *
 *      This procedure removes a window from the childList of its
 *      parent.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      The window is unlinked from its childList.
 *
 *----------------------------------------------------------------------
 */
static void
UnlinkWindow (TkWindow *winPtr) /* Child window to be unlinked. */
{
    TkWindow *prevPtr;

    prevPtr = winPtr->parentPtr->childList;
    if (prevPtr == winPtr) {
        winPtr->parentPtr->childList = winPtr->nextPtr;
        if (winPtr->nextPtr == NULL) {
            winPtr->parentPtr->lastChildPtr = NULL;
        }
    } else {
        while (prevPtr->nextPtr != winPtr) {
            prevPtr = prevPtr->nextPtr;
            if (prevPtr == NULL) {
                Tcl_Panic("UnlinkWindow couldn't find child in parent");
            }
        }
        prevPtr->nextPtr = winPtr->nextPtr;
        if (winPtr->nextPtr == NULL) {
            winPtr->parentPtr->lastChildPtr = prevPtr;
        }
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Rbc_RelinkWindow --
 *
 *      Relinks a window into a new parent.  The window is unlinked
 *      from its original parent's child list and added onto the end
 *      of the new parent's list.
 *
 *      FIXME:  If the window has focus, the focus should be moved
 *      to an ancestor.  Otherwise, Tk becomes confused
 *      about which Toplevel turns on focus for the window.
 *      Right now this is done at the Tcl layer.  For example,
 *      see rbc::CreateTearoff in tabset.tcl.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      The window is unlinked from its childList.
 *
 *----------------------------------------------------------------------
 */
void
Rbc_RelinkWindow(
    Tk_Window tkwin, /* Child window to be linked. */
    Tk_Window newParent,
    int x,
    int y)
{
    TkWindow *winPtr, *parentWinPtr;

    if (Rbc_ReparentWindow(Tk_Display(tkwin), Tk_WindowId(tkwin),
                           Tk_WindowId(newParent), x, y) != TCL_OK) {
        return;
    }
    winPtr = (TkWindow *)tkwin;
    parentWinPtr = (TkWindow *)newParent;

    winPtr->flags &= ~TK_REPARENTED;
    UnlinkWindow(winPtr);	/* Remove the window from its parent's list */

    /* Append the window onto the end of the parent's list of children */
    winPtr->parentPtr = parentWinPtr;
    winPtr->nextPtr = NULL;
    if (parentWinPtr->childList == NULL) {
        parentWinPtr->childList = winPtr;
    } else {
        parentWinPtr->lastChildPtr->nextPtr = winPtr;
    }
    parentWinPtr->lastChildPtr = winPtr;
}

/*
 *----------------------------------------------------------------------
 *
 * Rbc_RelinkWindow --
 *
 *      Relinks a window into a new parent.  The window is unlinked
 *      from its original parent's child list and added onto the end
 *      of the new parent's list.
 *
 *      FIXME:  If the window has focus, the focus should be moved
 *      to an ancestor.  Otherwise, Tk becomes confused
 *      about which Toplevel turns on focus for the window.
 *      Right now this is done at the Tcl layer.  For example,
 *      see rbc::CreateTearoff in tabset.tcl.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      The window is unlinked from its childList.
 *
 *----------------------------------------------------------------------
 */
void
Rbc_RelinkWindow2(
    Tk_Window tkwin, /* Child window to be linked. */
    Window window,
    Tk_Window newParent,
    int x,
    int y)
{
#ifdef notdef
    TkWindow *winPtr, *parentWinPtr;
#endif
    if (Rbc_ReparentWindow(Tk_Display(tkwin), window,
                           Tk_WindowId(newParent), x, y) != TCL_OK) {
        return;
    }
#ifdef notdef
    winPtr = (TkWindow *)tkwin;
    parentWinPtr = (TkWindow *)newParent;

    winPtr->flags &= ~TK_REPARENTED;
    UnlinkWindow(winPtr);	/* Remove the window from its parent's list */

    /* Append the window onto the end of the parent's list of children */
    winPtr->parentPtr = parentWinPtr;
    winPtr->nextPtr = NULL;
    if (parentWinPtr->childList == NULL) {
        parentWinPtr->childList = winPtr;
    } else {
        parentWinPtr->lastChildPtr->nextPtr = winPtr;
    }
    parentWinPtr->lastChildPtr = winPtr;
#endif
}

/*
 *--------------------------------------------------------------
 *
 * Rbc_UnlinkWindow --
 *
 *      TODO: Description
 *
 * Results:
 *      TODO: Results
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *--------------------------------------------------------------
 */
void
Rbc_UnlinkWindow (Tk_Window tkwin) /* Child window to be linked. */
{
    TkWindow *winPtr;
    Window root;

    root = XRootWindow(Tk_Display(tkwin), Tk_ScreenNumber(tkwin));
    if (Rbc_ReparentWindow(Tk_Display(tkwin), Tk_WindowId(tkwin),
                           root, 0, 0) != TCL_OK) {
        return;
    }
    winPtr = (TkWindow *)tkwin;
    winPtr->flags &= ~TK_REPARENTED;
#ifdef notdef
    UnlinkWindow(winPtr);	/* Remove the window from its parent's list */
#endif
}

/*
 *----------------------------------------------------------------------
 *
 * Rbc_Toplevel --
 *
 *      Climbs up the widget hierarchy to find the top level window of
 *      the window given.
 *
 * Results:
 *      Returns the Tk_Window of the toplevel widget.
 *
 *----------------------------------------------------------------------
 */
Tk_Window
Rbc_Toplevel (register Tk_Window tkwin)
{
    while (!Tk_IsTopLevel(tkwin)) {
        tkwin = Tk_Parent(tkwin);
    }
    return tkwin;
}

/*
 *--------------------------------------------------------------
 *
 * Rbc_RootCoordinates --
 *
 *      TODO: Description
 *
 * Results:
 *      TODO: Results
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *--------------------------------------------------------------
 */
void
Rbc_RootCoordinates(
    Tk_Window tkwin,
    int x,
    int y,
    int *rootXPtr,
    int *rootYPtr)
{
    int vx, vy, vw, vh;
    int rootX, rootY;

    Tk_GetRootCoords(tkwin, &rootX, &rootY);
    x += rootX;
    y += rootY;
    Tk_GetVRootGeometry(tkwin, &vx, &vy, &vw, &vh);
    x += vx;
    y += vy;
    *rootXPtr = x;
    *rootYPtr = y;
}

/*
 *--------------------------------------------------------------
 *
 * Rbc_RootX --
 *
 *      TODO: Description
 *
 * Results:
 *      TODO: Results
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *--------------------------------------------------------------
 */
int
Rbc_RootX (Tk_Window tkwin)
{
    int x;

    for (x = 0; tkwin != NULL;  tkwin = Tk_Parent(tkwin)) {
        x += Tk_X(tkwin) + Tk_Changes(tkwin)->border_width;
        if (Tk_IsTopLevel(tkwin)) {
            break;
        }
    }
    return x;
}

/*
 *--------------------------------------------------------------
 *
 * Rbc_RootY --
 *
 *      TODO: Description
 *
 * Results:
 *      TODO: Results
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *--------------------------------------------------------------
 */
int
Rbc_RootY (Tk_Window tkwin)
{
    int y;

    for (y = 0; tkwin != NULL;  tkwin = Tk_Parent(tkwin)) {
        y += Tk_Y(tkwin) + Tk_Changes(tkwin)->border_width;
        if (Tk_IsTopLevel(tkwin)) {
            break;
        }
    }
    return y;
}

#ifdef WIN32
/*
 *----------------------------------------------------------------------
 *
 * Rbc_GetRealWindowId --
 *
 *      Returns the XID for the Tk_Window given.  Starting in Tk 8.0,
 *      the toplevel widgets are wrapped by another window.
 *      Currently there's no way to get at that window, other than
 *      what is done here: query the X window hierarchy and grab the
 *      parent.
 *
 * Results:
 *      Returns the X Window ID of the widget.  If it's a toplevel,
 *      then the XID of the wrapper is returned.
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *----------------------------------------------------------------------
 */
Window
Rbc_GetRealWindowId (Tk_Window tkwin)
{
    return (Window) GetWindowHandle(tkwin);
}

/*
 *----------------------------------------------------------------------
 *
 * Rbc_GetToplevel --
 *
 *      Retrieves the toplevel window which is the nearest ancestor of
 *      of the specified window.
 *
 * Results:
 *      Returns the toplevel window or NULL if the window has no
 *      ancestor which is a toplevel.
 *
 * Side effects:
 *      None.
 *
 *----------------------------------------------------------------------
 */
Tk_Window
Rbc_GetToplevel (Tk_Window tkwin) /* Window for which the toplevel
                                   * should be deterined. */
{
    while (!Tk_IsTopLevel(tkwin)) {
        tkwin = Tk_Parent(tkwin);
        if (tkwin == NULL) {
            return NULL;
        }
    }
    return tkwin;
}

/*
 *----------------------------------------------------------------------
 *
 * Rbc_RaiseToLevelWindow --
 *
 *      TODO: Description
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *----------------------------------------------------------------------
 */
void
Rbc_RaiseToplevel (Tk_Window tkwin)
{
    SetWindowPos(GetWindowHandle(tkwin), HWND_TOP, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE);
}

/*
 *----------------------------------------------------------------------
 *
 * Rbc_MapToplevel --
 *
 *      TODO: Description
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *----------------------------------------------------------------------
 */
void
Rbc_MapToplevel (Tk_Window tkwin)
{
    ShowWindow(GetWindowHandle(tkwin), SW_SHOWNORMAL);
}

/*
 *----------------------------------------------------------------------
 *
 * Rbc_UnmapToplevel --
 *
 *      TODO: Description
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *----------------------------------------------------------------------
 */
void
Rbc_UnmapToplevel (Tk_Window tkwin)
{
    ShowWindow(GetWindowHandle(tkwin), SW_HIDE);
}

/*
 *----------------------------------------------------------------------
 *
 * Rbc_MoveResizeToplevel --
 *
 *      TODO: Description
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *----------------------------------------------------------------------
 */
void
Rbc_MoveResizeToplevel(
    Tk_Window tkwin,
    int x,
    int y,
    int width,
    int height)
{
    SetWindowPos(GetWindowHandle(tkwin), HWND_TOP, x, y, width, height, 0);
}

/*
 *--------------------------------------------------------------
 *
 * Rbc_ReparentWindow --
 *
 *      TODO: Description
 *
 * Results:
 *      TODO: Results
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *--------------------------------------------------------------
 */
int
Rbc_ReparentWindow(
    Display *display,
    Window window,
    Window newParent,
    int x,
    int y)
{
    XReparentWindow(display, window, newParent, x, y);
    return TCL_OK;
}

#else  /* WIN32 */

/*
 *----------------------------------------------------------------------
 *
 * Rbc_GetRealWindowId --
 *
 *      Returns the XID for the Tk_Window given.  Starting in Tk 8.0,
 *      the toplevel widgets are wrapped by another window.
 *      Currently there's no way to get at that window, other than
 *      what is done here: query the X window hierarchy and grab the
 *      parent.
 *
 * Results:
 *      Returns the X Window ID of the widget.  If it's a toplevel, then
 *      the XID of the wrapper is returned.
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *----------------------------------------------------------------------
 */
Window
Rbc_GetRealWindowId (Tk_Window tkwin)
{
    return GetWindowId(tkwin);
}

/*
 *----------------------------------------------------------------------
 *
 * Rbc_RaiseToplevel --
 *
 *      TODO: Description
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *----------------------------------------------------------------------
 */
void
Rbc_RaiseToplevel (Tk_Window tkwin)
{
    XRaiseWindow(Tk_Display(tkwin), GetWindowId(tkwin));
}

/*
 *----------------------------------------------------------------------
 *
 * Rbc_LowerToplevel --
 *
 *      TODO: Description
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *----------------------------------------------------------------------
 */
void
Rbc_LowerToplevel (Tk_Window tkwin)
{
    XLowerWindow(Tk_Display(tkwin), GetWindowId(tkwin));
}

/*
 *----------------------------------------------------------------------
 *
 * Rbc_ResizeToplevel --
 *
 *      TODO: Description
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *----------------------------------------------------------------------
 */
void
Rbc_ResizeToplevel(
    Tk_Window tkwin,
    int width,
    int height)
{
    XResizeWindow(Tk_Display(tkwin), GetWindowId(tkwin), width, height);
}

/*
 *----------------------------------------------------------------------
 *
 * Rbc_MoveResizeToplevel --
 *
 *      TODO: Description
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *----------------------------------------------------------------------
 */
void
Rbc_MoveResizeToplevel(
    Tk_Window tkwin,
    int x,
    int y,
    int width,
    int height)
{
    XMoveResizeWindow(Tk_Display(tkwin), GetWindowId(tkwin), x, y,
                      width, height);
}

/*
 *----------------------------------------------------------------------
 *
 * Rbc_MoveToplevel --
 *
 *      TODO: Description
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *----------------------------------------------------------------------
 */
void
Rbc_MoveToplevel(
    Tk_Window tkwin,
    int x,
    int y)
{
    XMoveWindow(Tk_Display(tkwin), GetWindowId(tkwin), x, y);
}

/*
 *----------------------------------------------------------------------
 *
 * Rbc_MapToplevel --
 *
 *      TODO: Description
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *----------------------------------------------------------------------
 */
void
Rbc_MapToplevel (Tk_Window tkwin)
{
    XMapWindow(Tk_Display(tkwin), GetWindowId(tkwin));
}

/*
 *----------------------------------------------------------------------
 *
 * Rbc_UnmapToplevel --
 *
 *      TODO: Description
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *----------------------------------------------------------------------
 */
void
Rbc_UnmapToplevel (Tk_Window tkwin)
{
    XUnmapWindow(Tk_Display(tkwin), GetWindowId(tkwin));
}

/*
 *----------------------------------------------------------------------
 *
 * XReparentWindowErrorProc --
 *
 *      TODO: Description
 *
 * Results:
 *      TODO: Results
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *----------------------------------------------------------------------
 */
static int
XReparentWindowErrorProc(
    ClientData clientData,
    XErrorEvent *errEventPtr)
{
    int *errorPtr = clientData;

    *errorPtr = TCL_ERROR;
    return 0;
}

/*
 *----------------------------------------------------------------------
 *
 * Rbc_ReparentWindow --
 *
 *      TODO: Description
 *
 * Results:
 *      TODO: Results
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *----------------------------------------------------------------------
 */
int
Rbc_ReparentWindow(
    Display *display,
    Window window,
    Window newParent,
    int x,
    int y)
{
    Tk_ErrorHandler handler;
    int result;
    int any = -1;

    result = TCL_OK;
    handler = Tk_CreateErrorHandler(display, any, X_ReparentWindow, any,
                                    XReparentWindowErrorProc, &result);
    XReparentWindow(display, window, newParent, x, y);
    Tk_DeleteErrorHandler(handler);
    XSync(display, False);
    return result;
}

#endif /* WIN32 */

#if (TK_MAJOR_VERSION == 4)
static int initialized = FALSE;
static Tcl_HashTable windowTable;

/*
 *--------------------------------------------------------------
 *
 * Rbc_SetWindowInstanceData --
 *
 *      TODO: Description
 *
 * Results:
 *      TODO: Results
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *--------------------------------------------------------------
 */
void
Rbc_SetWindowInstanceData(
    Tk_Window tkwin,
    ClientData instanceData)
{
    Tcl_HashEntry *hPtr;
    int isNew;

    if (!initialized) {
        Tcl_InitHashTable(&windowTable, TCL_ONE_WORD_KEYS);
        initialized = TRUE;
    }
    hPtr = Tcl_CreateHashEntry(&windowTable, (char *)tkwin, &isNew);
    assert(isNew);
    Tcl_SetHashValue(hPtr, instanceData);
}

/*
 *--------------------------------------------------------------
 *
 * Rbc_GetWindowInstanceData --
 *
 *      TODO: Description
 *
 * Results:
 *      TODO: Results
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *--------------------------------------------------------------
 */
ClientData
Rbc_GetWindowInstanceData (Tk_Window tkwin)
{
    Tcl_HashEntry *hPtr;

    hPtr = Tcl_FindHashEntry(&windowTable, (char *)tkwin);
    if (hPtr == NULL) {
        return NULL;
    }
    return Tcl_GetHashValue(hPtr);
}

/*
 *--------------------------------------------------------------
 *
 * Rbc_DeleteWindowInstanceData --
 *
 *      TODO: Description
 *
 * Results:
 *      TODO: Results
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *--------------------------------------------------------------
 */
void
Rbc_DeleteWindowInstanceData (Tk_Window tkwin)
{
    Tcl_HashEntry *hPtr;

    hPtr = Tcl_FindHashEntry(&windowTable, (char *)tkwin);
    assert(hPtr);
    Tcl_DeleteHashEntry(&windowTable, hPtr);
}

#else

/*
 *--------------------------------------------------------------
 *
 * Rbc_SetWindowInstanceData --
 *
 *      TODO: Description
 *
 * Results:
 *      TODO: Results
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *--------------------------------------------------------------
 */
void
Rbc_SetWindowInstanceData(
    Tk_Window tkwin,
    ClientData instanceData)
{
    TkWindow *winPtr = (TkWindow *)tkwin;

    winPtr->instanceData = instanceData;
}

/*
 *--------------------------------------------------------------
 *
 * Rbc_GetWindowInstanceData --
 *
 *      TODO: Description
 *
 * Results:
 *      TODO: Results
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *--------------------------------------------------------------
 */
ClientData
Rbc_GetWindowInstanceData (Tk_Window tkwin)
{
    TkWindow *winPtr = (TkWindow *)tkwin;

    return winPtr->instanceData;
}

/*
 *--------------------------------------------------------------
 *
 * Rbc_DeleteWindowInstanceData --
 *
 *      TODO: Description
 *
 * Results:
 *      TODO: Results
 *
 * Side effects:
 *      TODO: Side Effects
 *
 *--------------------------------------------------------------
 */
void
Rbc_DeleteWindowInstanceData (Tk_Window tkwin)
{
    
}

#endif

