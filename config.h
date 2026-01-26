/* See LICENSE file for copyright and license details. */

#define SESSION_FILE "/tmp/dwm-session"
#define TERMINAL "st"
#define TERMCLASS "St"

/* appearance */
static unsigned int borderpx  = 3;        /* border pixel of windows */
static unsigned int snap      = 10;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 5;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int swallowfloating    = 1;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 5;       /* vert inner gap between windows */
static const unsigned int gappoh    = 5;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 6;        /* 2 is the default spacing around the bar's font */
static const int vertpad            = 5;       /* vertical padding of bar */
static const int sidepad            = 15;       /* horizontal padding of bar */
static char font[]            = "monospace:size=10";
static const char *fonts[]          = { "monospace:size=10", "ttf-font-awesome:size=10:antialias:true", "FontAwesome5Brands:size=8:antialias:true", "FontAwesome5Free:size=8:antialias:true", "FontAwesome5Free:style=Solid:size=8:antialias:true"};
static char dmenufont[]       = "monospace:size=12";
static char normbgcolor[]       = "#292929";
static char normbordercolor[]       = "#292929";
static char normfgcolor[]       = "#bbbbbb";
static char selfgcolor[]       = "#dddddd";
static char selbordercolor[]        = "#5a7aa3";
static char selbgcolor[]            = "#181e29";
static char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
	[SchemeSel]  = { selfgcolor, selbgcolor,  selbordercolor  },
};

/* tagging */
#define MAX_TAGNAME_LEN 14		/* excludes TAG_PREPEND */
/* #define TAG_PREPEND "%1i "		 formatted as 2 chars */
#define TAG_PREPEND ""		/* formatted as 2 chars */
#define MAX_TAGLEN 16			/* altogether */
static char tags[][MAX_TAGLEN] = { "", "", "", "", "5", "6", "7", "8", "9" };
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const int momentaryalttags = 1; /* 1 means alttags will show only when key is held down*/

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor		scratchkey*/
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           -1,        -1,			0},
	{ "Stata/MP 17.0",    NULL,     NULL,           0,         0,          0,           -1,        -1,			0},
	{ "Data Editor (Browse)",    NULL,     NULL,           0,         0,          0,           -1,        -1,			0},
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1,			0},
	{ "st",      NULL,     NULL,           0,         1,          0,           1,        -1,			's'},
	{ "nvim",      NULL,     NULL,           0,         1,          0,           0,        -1,			'v'},
	{ "KeePassXC",      NULL,     NULL,           0,         1,          0,           0,        -1,			'k'},
	{ "htop",      NULL,     NULL,           0,         1,          0,           0,        -1,			'z'},
	{ NULL,      NULL,     "Event Tester", 0,        	 0,          0,           1,        -1,			0}, /* xev */
};

/* layout(s) */
static float mfact     = 0.6; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

#include <X11/XF86keysym.h> /* for function key bindings */
/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "-1"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *rebootcmd[]  = { "sudo", "shutdown", "-r", "+0", NULL };
static const char *shutdowncmd[]  = { "sudo", "shutdown", "+0", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *brightnessup[] = {"light", "-A", "5", NULL} ;
static const char *brightnessdown[] = {"light", "-U", "5", NULL} ;

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "font",               STRING,  &font },
		{ "dmenufont",          STRING,  &dmenufont },
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          		INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	 	FLOAT,   &mfact },
};

static const char *stscratch[] = {"s", "st", "-t", "terminal", NULL};
static const char *kpscratch[] = {"k", "keepassxc", "-t", "pass", NULL};
static const char *zscratch[] = {"z", "htop", "-t", "pass", NULL};

#include "movestack.c"
static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_space,      spawn,          {.v = dmenucmd } },
	{ MODKEY,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY|Mod1Mask,              XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_i,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_o,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_6,      incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_7,      incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_8,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_9,      incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ControlMask,						XK_comma,  cyclelayout,    {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
	{ MODKEY|ControlMask,           XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_s,  togglescratch,  {.v = stscratch } },
	{ MODKEY,                       XK_p,  togglescratch,  {.v = kpscratch } },
	{ MODKEY,             					XK_z,  togglescratch,  {.v = zscratch } },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,             					XK_a,      togglealttag,   {0} },
	{ MODKEY,                       XK_n,      nametag,   {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY,             					XK_q,      killclient,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} },
	{ MODKEY|ShiftMask,             XK_Delete, spawn,          {.v = rebootcmd} },
	{ MODKEY|ShiftMask,             XK_Escape, spawn,          {.v = shutdowncmd} },
	/* commented out pulseaudio vol change functions for pipewire */
	{ 0,                            XF86XK_AudioMute,		              spawn,	  SHCMD("amixer -q sset Master toggle") }, /* mute */
	{ 0,                            XF86XK_AudioRaiseVolume,      		spawn,	  SHCMD("amixer -q sset Master 3%+") }, /* vol up */
	{ 0,                            XF86XK_AudioLowerVolume,      		spawn,	  SHCMD("amixer -q sset Master 3%-") }, /* vol down */
	{ 0,       						XF86XK_MonBrightnessUp, 			spawn,    { .v = brightnessup } },
	{ 0,       						XF86XK_MonBrightnessDown, 			spawn, { .v = brightnessdown } },
	{ MODKEY,			         					XK_w,		                          spawn,	  SHCMD("$BROWSER") }, /* default web browser */
	{ MODKEY,			         					XK_c,		                          spawn,	  SHCMD("google-chrome-stable") }, /* chrome */
	{ MODKEY|ShiftMask,			        XK_w,		                          spawn,	  SHCMD("librewolf") }, /* librewolf */
	{ MODKEY|ShiftMask,			        XK_r,		                          spawn,	  SHCMD(TERMINAL " -e ranger") }, /* file browser (ranger)*/
	{ MODKEY|ShiftMask,			        XK_f,		                          spawn,	  SHCMD(TERMINAL " -e lf") }, /* file browser (lf)*/
	{ MODKEY,			         					XK_e,		                          spawn,	  SHCMD(TERMINAL " -e emacs -nw --no-splash") }, /* emacs */
	{ MODKEY,			         					XK_v,		                          spawn,	  SHCMD(TERMINAL " -e nvim") }, /* neovim */
	{ MODKEY,			         					XK_g,		                          spawn,	  SHCMD(TERMINAL " -e gemini --include-directories ~/Dropbox/notes/obsidian") }, /* gemini CLI */
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
