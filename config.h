/* See LICENSE file for copyright and license details. */

#define TERMINAL "st"
#define TERMCLASS "St"

/* appearance */
static const unsigned int borderpx = 3; /* border pixel of windows */
static const unsigned int snap = 32;    /* snap pixel */
static const int showbar = 1;           /* 0 means no bar */
static const int topbar = 1;            /* 0 means bottom bar */
static const char *fonts[] = {"0xProto Nerd Font:style=Regular:size=18"};
/*static const char dmenufont[] = "monospace:size=18";*/
static const char dmenufont[] = "0xProto Nerd Font:style=Regular:size=18";
static const char col_black[] = "#000000";
/*static const char col_gray1[]       = "#222222";*/
/*static const char col_gray2[]       = "#444444";*/
/*static const char col_gray3[]       = "#bbbbbb";*/
/*static const char col_gray4[]       = "#eeeeee";*/
/*static const char col_cyan[]        = "#005577";*/
static char normbgcolor[] = "#000000";     // black
/*static char normbordercolor[] = "#3a3a3a"; // grey23*/
static char normbordercolor[] = "#000000";
static char normfgcolor[] = "#5f5f87";     // mediumpurple4
static char selfgcolor[] = "#000000";      // black
/*static char selbordercolor[] = "#5f5f87";  // mediumpurple4*/
static char selbordercolor[] = "#1c1c1c";
static char selbgcolor[] = "#5f5f87";      // mediumpurple4

const char *colors[][3] = {
    /*               fg           bg           border   */
    [SchemeNorm] = {normfgcolor, normbgcolor, normbordercolor},
    [SchemeSel] = {selfgcolor, selbgcolor, selbordercolor},
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6"};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    {TERMCLASS, NULL, NULL, 0, 0, -1},
    {"FBReader", "FBReader", "FBReader", 1 << 1, 0, -1},
    {"Zathura", NULL, NULL, 1 << 1, 0, -1},
    {"Brave-browser", "brave-browser", "Brave", 1 << 3, 0, -1},
    {"Spotify", "spotify", "Spotify", 1 << 2, 0, -1},
    {"Firefox", "Navigator", "Firefox", 1 << 3, 0, -1},
    {"firefox", "Navigator", "Firefox", 1 << 3, 0, -1},
    {"firefox", "Navigator", "Firefox", 1 << 3, 0, -1},
    {"zen", "Navigator", "Zen Browser", 1 << 3, 0, -1},
    {"zen-beta", "Navigator", "Zen Browser", 1 << 3, 0, -1},
};

/* layout(s) */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile}, /* first entry is default */
    {"><>", NULL}, /* no layout function means floating behavior */
    {"[M]", monocle},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
    {MODKEY, KEY, view, {.ui = 1 << TAG}},                                     \
        {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},             \
        {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                      \
        {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
    {                                                                          \
        .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                   \
    }

#include <X11/XF86keysym.h>
#if defined(__ASUS_AURA__)
#include "asus_aura.c"
#endif

/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", "-m",  dmenumon,       "-fn",
                                 dmenufont,   "-nb", normbgcolor,    "-nf",
                                 normfgcolor, "-sb", selbordercolor, "-sf",
                                 selfgcolor,  NULL};
static const char *termcmd[] = {TERMINAL, NULL};

static const Key keys[] = {
    /* modifier                     key        function        argument */
    {MODKEY, XK_d, spawn, {.v = dmenucmd}},
    {MODKEY | ShiftMask, XK_Return, spawn, {.v = termcmd}},

    {MODKEY, XK_e, spawn, SHCMD("$EREADER")},
    {MODKEY, XK_r, spawn, SHCMD(TERMINAL " -e htop")},
    {MODKEY, XK_w, spawn, SHCMD("$BROWSER")},
    {MODKEY, XK_s, spawn, SHCMD("spotify")},
    {MODKEY | ShiftMask, XK_w, spawn, SHCMD("$BROWSER_SEC")},

    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_o, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY, XK_Return, zoom, {0}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY, XK_q, killclient, {0}},
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_space, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_Left, focusmon, {.i = -1}},
    {MODKEY, XK_Right, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_Left, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_Right, tagmon, {.i = +1}},
    TAGKEYS(XK_1, 0)
    TAGKEYS(XK_2, 1)
    TAGKEYS(XK_3, 2)
    TAGKEYS(XK_4, 3)
    TAGKEYS(XK_5, 4)
    TAGKEYS(XK_6, 5)
    {MODKEY | ShiftMask, XK_q, quit, {0}},

// User defined keys
    {MODKEY, XK_comma, spawn, SHCMD("playerctl previous")},
    {MODKEY, XK_period, spawn, SHCMD("playerctl next")},
    {MODKEY, XK_slash, spawn, SHCMD("playerctl play-pause")},
    {MODKEY, XK_minus, spawn, SHCMD("pamixer -d 5")},
    {MODKEY | ShiftMask, XK_minus, spawn, SHCMD("xbacklight -dec 5")},
    {MODKEY, XK_equal, spawn, SHCMD("pamixer -i 5")},
    {MODKEY | ShiftMask, XK_equal, spawn, SHCMD("xbacklight -inc 5")},

    {MODKEY | ShiftMask, XK_p, spawn,
     SHCMD("scrot -s - | xclip -selection clipboard -t image/png")},
    {0, XK_Print, spawn,
     SHCMD("scrot -s - | xclip -selection clipboard -t image/png")},

/* Media keys for Asus ROG Zephyrus*/
#if defined(__ASUS_AURA__)
    {0, XF86XK_KbdBrightnessUp, increase_kblight, {0}},
    {0, XF86XK_KbdBrightnessDown, decrease_kblight, {0}},
    // Load next keyboard lighting mode
    {0, XF86XK_Launch3, spawn, SHCMD("asusctl led-mode -n")},
    // Load next fan profile
    {0, XF86XK_Launch4, spawn, SHCMD("asusctl profile -n")},
    // Change between Integrated and Dedicated graphics
    {MODKEY | ShiftMask, XK_d, next_gmode, {0}},
#endif

#ifdef XK_3270
    {0, XK_3270_PrintScreen, spawn,
     SHCMD("scrot -s -o | xclip -selection clipboard -t image/png")},
#endif

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function argument */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
