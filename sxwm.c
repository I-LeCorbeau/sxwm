/* Simple/Scriptable X Window Manager.
 * See LICENSE file for details
 */

#include <sys/stat.h>
#include <sys/types.h>

#include <err.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "sxwm.h"

/* global variables */
xcb_connection_t	*conn;
xcb_screen_t		*scr;
xcb_window_t		 root;
xcb_window_t		 focuswin;
struct Monitor		*mons;
int			 sw, sh;
int			 x, y, w, h;
char			 config_path[MAXLEN];
//int			 bh;

#include "config.h"

void
cleanup(void)
{
	/* graceful exit */
	if (conn != NULL)
		xcb_disconnect(conn);
}

void
configure(struct Client *c)
{
	xcb_configure_notify_event_t config_event;
	config_event.response_type = XCB_CONFIGURE_NOTIFY;
	config_event.event = c->win;
	config_event.window = c->win;
	config_event.x = c->x;
	config_event.y = c->y;
	config_event.width = c->w;
	config_event.height = c->h;
	config_event.border_width = c->bw;
	config_event.above_sibling = XCB_NONE;
	config_event.override_redirect = false;
	xcb_send_event(conn, false, c->win, XCB_EVENT_MASK_STRUCTURE_NOTIFY, (char*)&config_event);
}

struct Monitor *
createmon(void)
{
	struct Monitor *m;

	m = xmalloc(sizeof(struct Monitor));

	return m;
}

void
moveresize(xcb_window_t win)
{

	uint32_t values[] = { x, y, w, h };

	//x = GAPPX;
	//y = GAPPX + BARPADDING;
	//w = scr->width_in_pixels - (GAPPX * 2) - (BORDERPX * 2) ;
	//h = scr->height_in_pixels - (GAPPX * 2) - (BORDERPX * 2) - BARPADDING;

	xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y
			| XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);
}

void
manage(xcb_window_t win)
{
	struct Client *c;
	uint32_t values[2];
	c = xmalloc(sizeof(struct Client));
	c->win = win;

	/* Get Geometry  */
	c->x = getgeom(win, ATTR_X);
	c->y = getgeom(win, ATTR_Y);
	c->w = getgeom(win, ATTR_W);
	c->h = getgeom(win, ATTR_H);

	/* subscribe to events */
	values[0] = XCB_EVENT_MASK_ENTER_WINDOW;
	values[1] = XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY;
	xcb_change_window_attributes(conn, c->win, XCB_CW_EVENT_MASK, values);

	/* border width */
	values[0] = BORDERPX;
	xcb_configure_window(conn, c->win, XCB_CONFIG_WINDOW_BORDER_WIDTH, values);

	/* window's initial position */
	values[0] = GAPPX;
	values[1] = BARPADDING + GAPPX;
	xcb_configure_window(conn, c->win, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, values);
	//xcb_map_window(conn, win);
	configure(c);
#ifdef ISMAXED
	/* If maximized mode is enabled by default, resize window */
	values[0] = scr->width_in_pixels - (GAPPX * 2) - (BORDERPX * 2);
	values[1] = scr->height_in_pixels - (GAPPX * 2) - (BORDERPX * 2) - BARPADDING;
	xcb_configure_window(conn, c->win, XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);
#endif

}

int
getattrs(xcb_window_t win, int attr)
{
	xcb_get_window_attributes_cookie_t	 c;
	xcb_get_window_attributes_reply_t	*r;

	c = xcb_get_window_attributes(conn, win);
	r = xcb_get_window_attributes_reply(conn, c, NULL);

	if (r == NULL)
		return -1;

	switch (attr) {
		case ATTR_M:
			attr = r->map_state;
			break;
		case ATTR_I:
			attr = r->override_redirect;
			break;
	}

	free(r);
	return attr;
}

int
getgeom(xcb_window_t win, int attr)
{
	xcb_get_geometry_cookie_t geom;
	xcb_get_geometry_reply_t *r;

	geom = xcb_get_geometry(conn, win);
	r = xcb_get_geometry_reply(conn, geom, NULL);

	if (r == NULL)
		return -1;

	switch (attr) {
		case ATTR_X:
			attr = r->x;
			break;
		case ATTR_Y:
			attr = r->y;
			break;
		case ATTR_W:
			attr = r->width;
			break;
		case ATTR_H:
			attr = r->height;
			break;
		case ATTR_B:
			attr = r->border_width;
			break;
	}

	free(r);
	return attr;
}

void
setfocus(xcb_window_t win)
{
	uint32_t values[1];

	/* set focus */
	xcb_set_input_focus(conn, XCB_INPUT_FOCUS_POINTER_ROOT, win,
			XCB_CURRENT_TIME);

	/* draw borders */
	values[0] = FOCUSCOL;
	xcb_change_window_attributes(conn, win, XCB_CW_BORDER_PIXEL, values);

	xcb_flush(conn);
}

int
setup(void)
{
	/* init xcb and grab events */
	//uint32_t mask = 0;
	//uint32_t values[2];
	//xcb_void_cookie_t cookie;
	//xcb_generic_error_t *error;

	if (xcb_connection_has_error(conn = xcb_connect(NULL, NULL)))
		return -1;
	
	scr = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;
	root = scr->root;

	sw = scr->width_in_pixels;
	sh = scr->height_in_pixels;

	xcb_grab_button(conn, 0, scr->root, XCB_EVENT_MASK_BUTTON_PRESS |
			XCB_EVENT_MASK_BUTTON_RELEASE, XCB_GRAB_MODE_ASYNC,
			XCB_GRAB_MODE_ASYNC, scr->root, XCB_NONE, 1, MOD);

	xcb_grab_button(conn, 0, scr->root, XCB_EVENT_MASK_BUTTON_PRESS |
			XCB_EVENT_MASK_BUTTON_RELEASE, XCB_GRAB_MODE_ASYNC,
			XCB_GRAB_MODE_ASYNC, scr->root, XCB_NONE, 3, MOD);
	/* Subscribe to events */
	uint32_t cw_values[] = {
		XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY | XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
			XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW |
			XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_PROPERTY_CHANGE
	};
	xcb_change_window_attributes_checked(conn, root, XCB_CW_EVENT_MASK, cw_values);

	xcb_flush(conn);

	return 0;
}

struct Client
*wintoclient(xcb_window_t win)
{
	struct Client *c;
	struct Monitor *m;

	for (m = mons; m; m = m->next)
		for (c = m->clients; c; c = c->next)
			if (c->win == w)
				return c;
	return NULL;

}

void *
xmalloc(size_t siz)
{
	void	*p;

	if (siz == 0)
		errx(1, "xmalloc: zero size");
	if ((p = malloc(siz)) == NULL)
		err(1, "malloc");

	return p;
}

/* taken from BSPWM */
void
run_config(int run_level)
{
	if (fork() == 0) {
		if (conn != NULL) {
			close(xcb_get_file_descriptor(conn));
		}
		setsid();
		char arg1[2];
		snprintf(arg1, 2, "%i", run_level);
		execl(config_path, config_path, arg1, (char *) NULL);
		err(1, "Couldn't execute the configuration file.\n");
	}
}

int main(int argc, char *argv[])
{
	int 	run_level = 0;
	config_path[0]	  = '\0';
	int	opt;

	/* graceful exit */
	atexit(cleanup);

	while ((opt = getopt(argc, argv, "hvc")) != -1) {
		switch (opt) {
			case 'h':
				printf(WM_NAME " [-v|-v|-c CONFIG_PATH]\n");
				exit(EXIT_SUCCESS);
				break;
			case 'v':
				printf("%s\n", VERSION);
				exit(EXIT_SUCCESS);
				break;
			case 'c':
				snprintf(config_path, sizeof(config_path), "%s", optarg);
				break;
		}
	}

	if (config_path[0] == '\0') {
		char *config_home = getenv(CONFIG_HOME_ENV);
		if (config_home != NULL) {
			snprintf(config_path, sizeof(config_path), "%s/%s/%s", config_home, WM_NAME, CONFIG_NAME);
		} else {
			snprintf(config_path, sizeof(config_path), "%s/%s/%s/%s", getenv("HOME"), ".config", WM_NAME, CONFIG_NAME);
		}
	}

	if (setup() < 0)
		errx(EXIT_FAILURE, "error connecting to X");

	run_config(run_level);

#ifdef OPENBSD
	if (pledge("stdio rpath proc exec", NULL) == -1)
		err(1, "pledge");
#endif /* OpenBSD */
	eventshandler();

	return EXIT_FAILURE;
}

/* vim: set noet sw=8 sts=8: */
