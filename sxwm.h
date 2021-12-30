/* Simple/Scriptable X Window Manager.
 * See LICENSE file for details
 */

#ifndef SXWM_H
#define SXWM_H

#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include <xcb/xcb_event.h>

#define CLEANMASK(m)	(m)

#define WM_NAME		"sxwm"
#define CONFIG_NAME	WM_NAME "rc"
#define CONFIG_HOME_ENV	"XDG_CONFIG_HOME"

#define MAXLEN		256

/* Sane MOD names */
#define SUPER	XCB_MOD_MASK_4
#define ALT	XCB_MOD_MASK_1
#define CTRL	XCB_MOD_MASK_CONTROL
#define SHIFT	XCB_MOD_MASK_SHIFT

/* Extern */
extern xcb_connection_t		*conn;		/* Connection to X Server */
extern xcb_screen_t		*scr;		/* Default screen */
extern xcb_window_t		 root;		/* The root window */
extern xcb_window_t		 win;		/* Client window */
extern int			 sw, sh;	/* Screen width and height. Is it even needed? */
extern int			 x, y;		/* Window X and Y coordinates Is it even needed? */
extern char			 config_path[];

/* Function declarations */
void		 buttonrelease(xcb_generic_event_t *ev);
void		 cleanup(void);
void		 configurenotify(xcb_generic_event_t *ev);
void		 configurerequest(xcb_generic_event_t *ev);
void		 destroynotify(xcb_generic_event_t *ev);
void		 enternotify(xcb_generic_event_t *ev);
void		 eventshandler(void);
void		 manage(xcb_window_t win);
void		 maprequest(xcb_generic_event_t *ev);
void		 setfocus(xcb_window_t win);
int		 setup(void);
void		 run_config(int);

#endif
