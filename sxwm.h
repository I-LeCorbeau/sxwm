/* Simple/Scriptable X Window Manager.
 * See LICENSE file for details
 */

#ifndef SXWM_H
#define SXWM_H

#include "queue.h"

#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include <xcb/xcb_event.h>

#define CLEANMASK(m)	(m)
#define MAX(A, B)	((A) > (B) ? (A) : (B))
#define MIN(A, B)	((A) < (B) ? (A) : (B))
#define WIDTH(X) 	((X)->w + 2 * (X)->bw)
#define HEIGHT(X)	((X)->h + 2 * (X)->bw)

#define WM_NAME		"sxwm"
#define CONFIG_NAME	WM_NAME "rc"
#define CONFIG_HOME_ENV	"XDG_CONFIG_HOME"

#define MAXLEN		256

/* Sane MOD names */
#define SUPER	XCB_MOD_MASK_4
#define ALT	XCB_MOD_MASK_1
#define CTRL	XCB_MOD_MASK_CONTROL
#define SHIFT	XCB_MOD_MASK_SHIFT

/* enums */
enum { INACTIVE, ACTIVE };
enum {
	ATTR_W = 1 << 0,
	ATTR_H = 1 << 1,
	ATTR_X = 1 << 2,
	ATTR_Y = 1 << 3,
	ATTR_B = 1 << 4,
	ATTR_M = 1 << 5,
	ATTR_I = 1 << 6,
	ATTR_MAX
};

/* enum {
	LIST_HIDDEN = 1 << 0,
	LIST_IGNORE = 1 << 1,
	LIST_ALL = 1 << 2
}; */

/* Structs */
struct Client {
	TAILQ_ENTRY(Client)	 entry;
	struct Monitor		*mon;
	xcb_window_t	 	 win;
	int16_t			 x, y;
	uint16_t		 w, h;
	int			 bw;
	//struct Client	*next;
};
TAILQ_HEAD(Client_q, Client);

struct Monitor {
	TAILQ_ENTRY(Monitor)	 entry;
	int			 which;
	xcb_window_t	 root;
	int		 num;
	//int		 by, bh;
	int16_t		 mx, my;
	uint16_t	 mw, mh;
	int16_t		 wx, wy;
	uint16_t	 ww, wh;
	struct Client_q	clientq;
	//struct Monitor	*next;
};
TAILQ_HEAD(Monitor_q, Monitor);

/* Extern */
extern xcb_connection_t		*conn;		/* Connection to X Server */
extern xcb_screen_t		*scr;		/* Default screen */
extern xcb_window_t		 root;		/* The root window */
extern xcb_window_t		 win;		/* Client window */
extern struct Monitor_q		 Monitorq;
extern int			 sw, sh;	/* Screen width and height. Is it even needed? */
extern int			 x, y;		/* Window X and Y coordinates Is it even needed? */
extern char			 config_path[];

/* Function declarations */
void			 cleanup(void);
void			 configure(struct Client *c);
void			 configurenotify(xcb_generic_event_t *ev);
void			 configurerequest(xcb_generic_event_t *ev);
struct Monitor		*createmon(void);
int			 getattrs(xcb_window_t win, int);
int			 getgeom(xcb_window_t win, int);
void			 eventshandler(void);
int			 exists(xcb_window_t);
xcb_window_t		 isfocused(void);
struct Client		*manage(xcb_window_t);
struct Client		*findclient(xcb_window_t);
void			 maprequest(xcb_generic_event_t *ev);
void			 moveresize(xcb_window_t);
struct Monitor		*findmon(xcb_window_t);
void			 setupmon(int);
//void			 setborder(xcb_window_t win);
void			 setfocus(xcb_window_t win);
int			 setup(void);
void			 split(xcb_window_t win);
void			*xmalloc(size_t);
void			 run_config(int);

#endif
