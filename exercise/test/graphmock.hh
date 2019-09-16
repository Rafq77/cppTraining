/*
 * Copyright (c) 2014-2017 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef GRAPHMOCK_HH_SEEN_
#define GRAPHMOCK_HH_SEEN_

//#define HAS_GROUP_MOVE_CTOR 1
//#define HAS_GROUP_MOVE_ASSIGNMENT 1
//#define HAS_CANVAS_MOVE_CTOR 1
//#define HAS_CANVAS_MOVE_ASSIGNMENT 1
//#define HAS_WIN_MOVE_CTOR 1
//#define HAS_WIN_MOVE_ASSIGNMENT 1
//#define HAS_CANVAS_GET_SHAPE 1
//#define HAS_GROUP_GET_SHAPE 1


inline namespace graphMock
{
// X mocks
typedef int Display;
typedef int Window;
typedef int Visual;
typedef int Atom;
typedef int Bool;

enum XMaskConstants : long { ExposureMask, ButtonPressMask };
enum XEventType { Expose, ButtonPress, ClientMessage };

struct XEvent
{
    XEventType type;

    struct XExposeEvent
    {
        int count;
        Window window;
    };
    XExposeEvent xexpose;
};
inline bool operator==(XEvent const &a, XEvent const &b)
{
    return a.type == b.type && a.xexpose.window == b.xexpose.window;
}

int XNextEvent(Display *, XEvent *);

Display *XOpenDisplay(char const *name);
int XCloseDisplay(Display *dpy);

Window XCreateSimpleWindow(
    Display *
    , Window
    , int x, int y
    , unsigned int width, unsigned int height
    , unsigned int border_width
    , unsigned long border, unsigned long background);
int XDestroyWindow(Display *, Window);
int XMaskEvent(Display *, long, XEvent *);


inline int DefaultScreen(Display *)
{
    return 0;
}

inline Window RootWindow(Display *, int)
{
    return 0;
}

inline Visual *DefaultVisual(Display *, int)
{
    return nullptr;
}

inline unsigned long BlackPixel(Display *, int)
{
    return 0;
}
inline unsigned long WhitePixel(Display *, int)
{
    return 0;
}

inline int XStoreName(Display *, Window, char const *)
{
    return 0;
}

inline int XSelectInput(Display *, Window, long)
{
    return 0;
}

inline int XSetWMProtocols(Display *, Window, Atom *, int)
{
    return 0;
}

inline Atom XInternAtom(Display *, char const *, Bool)
{
    return 0;
}

inline int XMapWindow(Display *, Window)
{
    return 0;
}

inline int XFlush(Display *)
{
    return 0;
}


// Win32 mocks
#define CALLBACK
typedef int *HWND;
typedef int *HDC;

typedef int HINSTANCE;
typedef int LRESULT;
typedef int RECT;
typedef int WPARAM;
typedef int LPARAM;
typedef int BOOL;
typedef int MSG;
typedef int HBRUSH;
typedef int ATOM;
typedef int PAINTSTRUCT;

typedef unsigned int UINT;
typedef unsigned int aa;
typedef int (*WNDPROC)(HWND, UINT, int, int);

struct WNDCLASS
{
    UINT style;
    WNDPROC lpfnWndProc;
    int cbClsExtra;
    int cbWndExtra;
    HINSTANCE hInstance;
    int *hIcon;
    int hCursor;
    int hbrBackground;
    char const *lpszMenuName;
    char const *lpszClassName;
};

enum Win32Consts : unsigned int
{
    WS_CAPTION
    , WS_SYSMENU
    , WS_THICKFRAME
    , CW_USEDEFAULT
    , SW_SHOWDEFAULT
    , FALSE
    , CS_HREDRAW
    , CS_VREDRAW
    , CS_OWNDC
    , IDC_ARROW
    , WM_CREATE
    , WM_NCCREATE
    , WM_PAINT
    , WM_DESTROY
    , WM_NCDESTROY
    , WM_GETMINMAXINFO
    , COLOR_WINDOW
};


int RegisterClass(WNDCLASS*);
void UnregisterClass(char const *, int);
HWND CreateWindow(char const *, char const *,
                  UINT, int, int, int, int, int*, int*, int, int*);
void DestroyWindow(HWND);
HDC GetDC(HWND);
void ReleaseDC(HWND, HDC);

inline void ShowWindow(HWND, UINT) {}
inline void InvalidateRect(HWND, int *, UINT) {}
inline void UpdateWindow(HWND) {}
inline int GetMessage(int *, int, int, int) { return 1; }
inline void DispatchMessage(int *) {}
inline int GetModuleHandle(int) { return 1; }
inline int LoadCursor(int *, UINT) { return 1; }
inline int DefWindowProc(HWND, int, int, int) { return 1; }
inline void GetClientRect(HWND, int*) {}
inline HDC BeginPaint(HWND, int*) { return nullptr; }
inline void EndPaint(HWND, int*) {}
inline void PostQuitMessage(int) {}



// Cairo mocks
typedef int Drawable;
typedef int cairo_surface_t;
typedef struct _cairo cairo_t;

struct _cairo
{
};

cairo_surface_t *cairo_xlib_surface_create(
    Display *
    , Drawable
    , Visual *
    , int width
    , int height);
cairo_surface_t *cairo_win32_surface_create(HDC);
void cairo_surface_destroy(cairo_surface_t *);
cairo_t *cairo_create(cairo_surface_t *);
void cairo_destroy(cairo_t *);

inline void cairo_save(cairo_t *)
{
}
inline void cairo_restore(cairo_t *)
{
}

inline void cairo_show_page(cairo_t *)
{
}

inline void cairo_new_sub_path(cairo_t *)
{
}

inline void cairo_set_source_rgb(cairo_t *, double /*red*/, double /*green*/, double /*blue*/)
{
}

inline void cairo_set_line_width(cairo_t *, double /*width*/)
{
}

inline void cairo_arc(
    cairo_t *
    , double /*xc*/, double /*yc*/
    , double /*radius*/
    , double /*angle1*/, double /*angle2*/
    )
{
}

inline void cairo_rectangle(
    cairo_t *
    , double /*x*/, double /*y*/
    , double /*width*/, double /*height*/
    )
{
}

inline void cairo_move_to(
    cairo_t *
    , double /*x*/, double /*y*/
    )
{
}

inline void cairo_fill(cairo_t *)
{
}

inline void cairo_stroke(cairo_t *)
{
}

void cairo_show_text(cairo_t *, char const *);
} // namespace graphMock
#endif /* GRAPHMOCK_HH_SEEN_ */
