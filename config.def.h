/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 10;        /* gaps between windows */
static const unsigned int no_border = 1;        /* 1 means no border for single window */
static const unsigned int snap      = 5;        /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 10;  /* systray spacing */
static const unsigned int systrayiconsize = 14; /* systray icon size in px */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int showbutton         = 0;        /* 0 means no button */
static const int showtags           = 1;        /* 0 means no tags */
static const int showlayout         = 1;        /* 0 means no layout indicator */
static const int showtitle          = 0;        /* 0 means no title */
static const int showstatus         = 1;        /* 0 means no status bar */
static const int showfloating       = 1;        /* 0 means no floating indicator */
static const int topbar             = 0;        /* 0 means bottom bar */
static const int user_bh            = 2;        /* 2 is the default spacing around the bar's font */
static const int horizpadbar        = 4;       /* horizontal padding for statusbar */
static const int vertpadbar         = 12;       /* vertical padding for statusbar */
static const int vertpad            = 0;        /* vertical padding of bar */
static const int sidepad            = 0;        /* horizontal padding of bar */
static const char *fonts[]          = { "JetbrainsMono Nerd Font:size=12:style=bold" };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static const char *colors[][3]      = {
  
	/*                      fg                bg                  border   */
	[SchemeNorm]        = {normfgcolor,       normbgcolor,        normbordercolor },
	[SchemeSel]         = {selfgcolor, 	      selbgcolor,         selbordercolor },
	[SchemeButtonBar]   = {normfgcolor,       normbgcolor,	      normbordercolor }, // also apply on systray
	[SchemeTagsNorm]    = {normfgcolor,       normbgcolor,        normbordercolor }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeLayout]      = {normfgcolor,       normbgcolor,        normbordercolor },
	[SchemeWF]          = {normfgcolor,       normbgcolor,        normbordercolor },
	[SchemeInfoSel]     = {normfgcolor,       normbgcolor,        normbordercolor }, // infobar middle  selected {text,background,not used but cannot be empty}
	[SchemeInfoNorm]    = {normfgcolor,       normbgcolor,        normbordercolor }, // infobar middle  unselected {text,background,not used but cannot be empty}
	[SchemeStatus]      = {normfgcolor,       normbgcolor,        normbordercolor }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeScratchSel]  = {selbgcolor,        normfgcolor,        selbordercolor  },
	[SchemeScratchNorm] = {selbgcolor,        normfgcolor,        normbordercolor },
};

/* autostart applications */
static const char *const autostart[] = {
  "load-wallpaper", NULL,
  "dwmblocks", NULL,
  "dunst", NULL,
  "xcompmgr", NULL,
  "greenclip", "daemon", NULL,
  "ibus-daemon", NULL,
   NULL /* terminate */
};

/* tagging */
static const char buttonbar[] = "󰴈";
static const char *tags[] = { "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX" };
static const char *tagsalt[] = { "", "", "", "󰙯", "", "V", "U", "G", "F" };
static const int momentaryalttags = 1; /* 1 means alttags will show only when key is held down*/



static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class              instance    title           tags mask     isfloating  isterminal  noswallow  monitor   scratch key */
	{ "Pavucontrol",      NULL,       NULL,           0,            1,          0,          0,         -1,       0 },
	{ "Nsxiv",            NULL,       NULL,           0,            1,          0,          0,         -1,       0 },
	{ "neovim",           NULL,       NULL,           1 << 1,       0,          0,          0,         -1,       0 },
	{ "firefox",          NULL,       NULL,           1 << 2,       0,          0,          0,         -1,       0 },
	{ "firefox",          NULL,       "Picture-in-Picture",     ~0,  1,          0,          0,         -1,       0 },
	{ "discord",          NULL,       NULL,           1 << 3,       0,          0,          0,         -1,       0 },
	{ "Spotify",          NULL,       NULL,           1 << 4,       0,          0,          0,         -1,       0 },
	{ "Nemo",             NULL,       NULL,           0,            0,          1,          0,         -1,       0 },
	{ "St",               NULL,       NULL,           0,            0,          1,          0,         -1,       0 },
	{ "Kitty",            NULL,       NULL,           0,            0,          1,          0,         -1,       0 },
	{ "kitty",            NULL,       NULL,           0,            0,          1,          0,         -1,       0 },
	{ NULL,               NULL,       "spterm",       0,            1,          1,          0,         -1,       't' },
	{ NULL,               NULL,       "sptgpt",       0,            1,          1,          0,         -1,       'g' },
	{ NULL,               NULL,       "random",       0,            1,          0,          1,         -1,       's' },
	{ NULL,               NULL,       "spmix",        0,            1,          0,          1,         -1,       'a' },
	{ NULL,               NULL,       "spcldr",       0,            1,          0,          1,         -1,       'c' },
	{ NULL,               NULL,       "sptop",        0,            1,          0,          1,         -1,       'p' },
	{ NULL,               NULL,       "Event Tester", 0,            0,          0,          1,         -1,        0  }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.7; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "󰕰",      tile },    /* first entry is default */
	{ "󰖲",      NULL },    /* no layout function means floating behavior */
	{ "󰝤",      monocle },
};


/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* scratchpads */
/*First arg only serves to match against key in rules*/
static const char *sptermcmd[] = {"t", "st", "-t", "spterm" , "-g", "140x30", "-e", "tmux", "new-session", "-A", "-s", "tmux", NULL};
static const char *sptgptcmd[] = {"g", "st", "-t", "sptgpt", "-g", "140x30", "-e", "tgpt", "-m", NULL};
static const char *sptopcmd[]  = {"p", "st", "-t", "sptop", "-g", "140x30", "-e", "btop", NULL};
static const char *spmixcmd[]  = {"a", "st", "-t", "spmix", "-g", "140x30", "-e", "pulsemixer", NULL};
static const char *spcldrcmd[] = {"c", "st", "-t", "spcldr", "-g", "140x30", "-e", "calcurse", NULL};
static const char *sprandomcmd[] = { "s",  NULL };

/* commands */
static const char *termcmd[]  = { "st", NULL };

static const char *volumecmd[3][5] = {
	{ "wpctl", "set-volume", "@DEFAULT_SINK@", "0.05+", NULL },
	{ "wpctl", "set-volume", "@DEFAULT_SINK@", "0.05-", NULL },
	{ "wpctl", "set-mute", "@DEFAULT_SINK@", "toggle", NULL },
};
static const char *playerctlcmd[3][3] = {
	{ "playerctl", "play-pause", NULL },
	{ "playerctl", "next", NULL },
	{ "playerctl", "previous", NULL },
};



#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

static const Key keys[] = {
	/* modifier                     key                       function        argument */
	{ MODKEY,                       XK_w,                     spawn,          SHCMD("set-wallpaper") },
	{ MODKEY,                       XK_z,                     spawn,          SHCMD("rofi -show drun -theme ~/.config/rofi/launcher.rasi -show-icons") },
	{ MODKEY,                       XK_n,                     spawn,          SHCMD("st -t neovim -c neovim -e nvim") },
	{ Mod1Mask,                     XK_f,                     spawn,          SHCMD("rofi -show filebrowser -theme ~/.config/rofi/launcher.rasi -show-icons") },
	{ Mod1Mask,                     XK_c,                     spawn,          SHCMD("rofi -show calc -theme ~/.config/rofi/launcher.rasi") },
	{ MODKEY,                       XK_e,                     spawn,          SHCMD("rofi -modi emoji -show emoji -theme ~/.config/rofi/launcher.rasi") },
	{ MODKEY,                       XK_x,                     spawn,          SHCMD("rofi -show p -modi p:~/.config/rofi/off.sh -theme ~/.config/rofi/launcher.rasi")},
	{ MODKEY,                       XK_v,                     spawn,          SHCMD("clipboard_dmenu")},
	{ MODKEY,                       XK_Print,       					spawn,          SHCMD("screenshot_dmenu")},
	{ MODKEY,                       XK_Return,                spawn,          {.v = termcmd } },
	{0,                             XF86XK_AudioRaiseVolume,  spawn,          {.v = volumecmd[0]} },
	{0,                             XF86XK_AudioLowerVolume,  spawn,          {.v = volumecmd[1]} },
 	{0,                             XF86XK_AudioMute,         spawn,          {.v = volumecmd[2]} },
	{0,                             XF86XK_AudioPlay,         spawn,          {.v = playerctlcmd[0]} },
 	{0,                             XF86XK_AudioNext,         spawn,          {.v = playerctlcmd[1]} },
 	{0,                             XF86XK_AudioPrev,         spawn,          {.v = playerctlcmd[2]} },
 	{ MODKEY,                       XK_F5,					          xrdb,           {.v = NULL } },
	{ MODKEY,                       XK_b,                     togglebar,      {0} },
  { MODKEY,			                  XK_s,		                  togglesticky,	  {0} },
  { MODKEY,                       XK_j,                     focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                     focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_u,                     focusurgent,    {0} },
	{ MODKEY,                       XK_h,     				        shiftview,      { .i = -1 } },
	{ MODKEY,                       XK_l,     				        shiftview,      { .i = +1 } },
	{ MODKEY|ShiftMask,             XK_j,                     pushdown,       {0} },
	{ MODKEY|ShiftMask,             XK_k,                     pushup,         {0} },
	{ MODKEY,                       XK_Insert,                incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_Delete,                incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_h,                     setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_l,                     setmfact,       {.f = +0.05} },
	{ MODKEY|ControlMask,           XK_h,                     setcfact,       {.f = -0.20} },
	{ MODKEY|ControlMask,           XK_l,                     setcfact,       {.f = +0.20} },
	{ MODKEY|ControlMask,           XK_o,                     setcfact,       {.f =  0.00} },
	{ MODKEY,                       XK_r,                     reorganizetags, {0} },
	{ MODKEY|ShiftMask,             XK_r,                     distributetags, {0} },
	{ MODKEY|ShiftMask,             XK_Return,                zoom,           {0} },
	{ MODKEY,                       XK_Tab,                   view,           {0} },
	{ MODKEY, 			                XK_q,                     killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_x,                     killunsel,      {0} },
	{ MODKEY,                       XK_t,					            setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,					            setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,                     setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,				          setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,				          togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,                     togglefullscr,          {0} },
	{ MODKEY,                       XK_0,      				        view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      				        tag,            {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_comma,	  	            focusmon,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,		            focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,				          tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,				        tagmon,         {.i = +1 } },
	{ Mod1Mask,                     XK_b,                     togglealttag,   {0} },
	{ MODKEY,                       XK_minus,				          setgaps,        {.i = -4 } },
	{ MODKEY,                       XK_equal,				          setgaps,        {.i = +4 } },
	{ MODKEY|ShiftMask,             XK_minus,				          setgaps,        {.i = 0 } },
 	{ MODKEY|ShiftMask,             XK_equal,				          setgaps,        {.i = gappx } },
	TAGKEYS(                        XK_1,									    0 )
	TAGKEYS(                        XK_2,									    1 )
	TAGKEYS(                        XK_3,									    2 )
	TAGKEYS(                        XK_4,									    3 )
	TAGKEYS(                        XK_5,									    4 )
	TAGKEYS(                        XK_6,									    5 )
	TAGKEYS(                        XK_7,									    6 )
	TAGKEYS(                        XK_8,									    7 )
	TAGKEYS(                        XK_9,									    8 )
	{ MODKEY|ShiftMask,             XK_Escape,				  quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_Escape,				  quit,           {1} },

  /* scratchpad binds */
	{ MODKEY,                       XK_grave,  togglescratch,  {.v = sptermcmd } },
	{ MODKEY,                       XK_g,      togglescratch,  {.v = sptgptcmd } },
	{ MODKEY,                       XK_a,      togglescratch,  {.v = spmixcmd } },
	{ MODKEY,                       XK_c,      togglescratch,  {.v = spcldrcmd } },
	{ MODKEY,                       XK_p,      togglescratch,  {.v = sptopcmd } },
	{ MODKEY,                       XK_o,      togglescratch,  {.v = sprandomcmd } },
	{ MODKEY|ShiftMask,             XK_o,      setscratch,     {.v = sprandomcmd } },
	{ MODKEY|ControlMask,           XK_o,      removescratch,  {.v = sprandomcmd } },
	/* program binds */
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function           argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,         {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,         {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,              {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,      {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,      {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,      {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,         {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating,    {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,       {0} },
	{ ClkTagBar,            0,              Button1,        view,              {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,        {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,               {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,         {0} },
};
