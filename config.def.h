/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"

/* appearance */
static unsigned int borderpx  = 1;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 1;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static const double activeopacity   = 1.0f;     /* Window opacity when it's focused (0 <= opacity <= 1) */
static const double inactiveopacity = 0.9f;   /* Window opacity when it's inactive (0 <= opacity <= 1) */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
#define MAX_TAGNAME_LEN 14		/* excludes TAG_PREPEND */
#define TAG_PREPEND "%1i:"		/* formatted as 2 chars */
#define MAX_TAGLEN 16			/* altogether */
static char tags[][MAX_TAGLEN] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "stalonetray",	NULL,	NULL,	0,		1,	-1 },
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
};


#include <X11/XF86keysym.h> /* for function key bindings */

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \
/*	{ MOD, XK_q,     ACTION##stack, {.i = 0 } }, \ */
/*	{ MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
/*	{ MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
/*	{ MOD, XK_x,     ACTION##stack, {.i = -1 } }, */


/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };



/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
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

static Key keys[] = {
	/* modifier                    		 key       		                          function        argument */
	{ MODKEY,                      		 XK_space, 		                          spawn,          {.v = dmenucmd } },
	{ MODKEY,      	               		 XK_Return,		                          spawn,          {.v = termcmd } },
	{ MODKEY,                      		 XK_b,     		                          togglebar,      {0} },
  /* stacker functions */
	STACKKEYS(                         	 MODKEY,                                	  focus)
	STACKKEYS(                         	 MODKEY|ShiftMask,                      	  push)
	{ MODKEY,                      		 XK_i,     		                          incnmaster,     {.i = +1 } },
	{ MODKEY,                      		 XK_d,     		                          incnmaster,     {.i = -1 } },
	{ MODKEY,                      		 XK_h,     		                          setmfact,       {.f = -0.05} },
	{ MODKEY,                      		 XK_l,     		                          setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,    	       		 XK_Return,                             	  togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                      		 XK_Tab,   		                          view,           {0} },
	{ MODKEY,                      		 XK_t,     		                          setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,            		 XK_f,     		                          setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                      		 XK_m,     		                          setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,                	 XK_t,                                  	  setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,                 	 XK_f,                                  	  setlayout,      {.v = &layouts[4]} },
	{ MODKEY,	                     	 XK_f,     		                          fullscreen,     {0} },
	{ MODKEY,                      		 XK_p,  		                          setlayout,      {0} },
	{ MODKEY|ShiftMask,            		 XK_space, 		                          togglefloating, {0} },
	{ MODKEY,                      		 XK_s,     		                          togglesticky,   {0} },
	{ MODKEY,                      		 XK_0,     		                          view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,            		 XK_0,     		                          tag,            {.ui = ~0 } },
	{ MODKEY,                      		 XK_comma, 		                          focusmon,       {.i = -1 } },
	{ MODKEY,                      		 XK_period,		                          focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,            		 XK_comma, 		                          tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,            		 XK_period,		                          tagmon,         {.i = +1 } },
	TAGKEYS(                       		 XK_1,     		                                          0)
	TAGKEYS(                       		 XK_2,     		                                          1)
	TAGKEYS(                       		 XK_3,     		                                          2)
	TAGKEYS(                       		 XK_4,     		                                          3)
	TAGKEYS(                       		 XK_5,     		                                          4)
	TAGKEYS(                       		 XK_6,     		                                          5)
	TAGKEYS(                       		 XK_7,     		                                          6)
	TAGKEYS(                       		 XK_8,     		                                          7)
	TAGKEYS(                       		 XK_9,     		                                          8)
	{ MODKEY|ShiftMask,            		 XK_q,     		                          quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask,		 XK_q,     		                          quit,           {1} },
	{ MODKEY,		  		 XK_q,			                          killclient,	  {0} }, /* quit window */
	{ MODKEY,				 XK_n,			                          nametag,	  {0} }, /* apply name to current tag */
	{ 0,                               	 XF86XK_AudioMute,		                  spawn,	  SHCMD("pamixer -t") }, /* mute */
	{ 0,                               	 XF86XK_AudioRaiseVolume,              		  spawn,	  SHCMD("pamixer --allow-boost -i 3") }, /* vol up */
	{ 0,                               	 XF86XK_AudioLowerVolume,              		  spawn,	  SHCMD("pamixer --allow-boost -d 3") }, /* vol down */
	{ MODKEY,			         XK_w,		                             	  spawn,	  SHCMD("$BROWSER") }, /* browser */
	{ MODKEY|ShiftMask,		         XK_w,		                              	  spawn,	  SHCMD(TERMINAL " -e sudo nmtui") }, /* networkmanager */
	{ MODKEY,			         XK_r,		                              	  spawn,	  SHCMD(TERMINAL " -e ranger") }, /* ranger */
	{ MODKEY,			         XK_c,		                              	  spawn,	  SHCMD(TERMINAL " -e ferdi") }, /* ferdi */
	{ MODKEY|ShiftMask,		         XK_c,		                              	  spawn,	  SHCMD(TERMINAL "xstata") }, /* stata */
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
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
