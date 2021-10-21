/* Simple/Scriptable X Window Manager.
 * See LICENSE file for details
 */

#include "queue.h"

#include <err.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#include "sxwm.h"
#include "config.h"

void
eventshandler(void)
{
	xcb_generic_event_t *ev;

	uint32_t values[3];
	xcb_get_geometry_reply_t *geom;
	xcb_window_t win = 0;

	/* loop */
	while ((ev = xcb_wait_for_event(conn))) {

		if (!ev)
			errx(1, "xcb connection broken");

		switch (ev->response_type & ~0x80) {
			case XCB_CONFIGURE_REQUEST:
				configurerequest(ev);
			break; 

			case XCB_MAP_REQUEST:
				maprequest(ev);
			break;

		case XCB_DESTROY_NOTIFY: {
			xcb_destroy_notify_event_t *e;
			e = (xcb_destroy_notify_event_t *)ev;

			xcb_kill_client(conn, e->window);
		} break;

#ifdef ENABLE_SLOPPY
		case XCB_ENTER_NOTIFY: {
			xcb_enter_notify_event_t *e;
			e = (xcb_enter_notify_event_t *)ev;
			//focus(e->event, ACTIVE);
			setfocus(e->event);
		} break;
#endif

		/* case XCB_MAP_NOTIFY: {
			xcb_map_notify_event_t *e;
			e = (xcb_map_notify_event_t *)ev;

			if (!e->override_redirect) {
				//xcb_map_window(conn, e->window);
				//focus(e->window, ACTIVE);
				//winfo(e->window, ACTIVE);
			}
		} break; */

		case XCB_BUTTON_PRESS: {
			xcb_button_press_event_t *e;
			e = ( xcb_button_press_event_t *)ev;
			win = e->child;

			if (!win || win == scr->root)
				break;

			values[0] = XCB_STACK_MODE_ABOVE;
			xcb_configure_window(conn, win,
					XCB_CONFIG_WINDOW_STACK_MODE, values);
			geom = xcb_get_geometry_reply(conn,
					xcb_get_geometry(conn, win), NULL);
			if (e->detail == 1) {
				values[2] = 1;
				xcb_warp_pointer(conn, XCB_NONE, win, 0, 0, 0,
					0, geom->width/2, geom->height/2);
			} else {
				values[2] = 3;
				xcb_warp_pointer(conn, XCB_NONE, win, 0, 0, 0,
						0, geom->width, geom->height);
			}
			xcb_grab_pointer(conn, 0, scr->root,
				XCB_EVENT_MASK_BUTTON_RELEASE
				| XCB_EVENT_MASK_BUTTON_MOTION
				| XCB_EVENT_MASK_POINTER_MOTION_HINT,
				XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC,
				scr->root, XCB_NONE, XCB_CURRENT_TIME);
			xcb_flush(conn);
		} break;

		case XCB_MOTION_NOTIFY: {
			xcb_query_pointer_reply_t *pointer;
			pointer = xcb_query_pointer_reply(conn,
					xcb_query_pointer(conn, scr->root), 0);
			if (values[2] == 1) {
				geom = xcb_get_geometry_reply(conn,
					xcb_get_geometry(conn, win), NULL);
				if (!geom)
					break;
				
				values[0] = (pointer->root_x + geom->width / 2
					> scr->width_in_pixels
					- (BORDERPX*2))
					? scr->width_in_pixels - geom->width
					- (BORDERPX*2)
					: pointer->root_x - geom->width / 2;
				values[1] = (pointer->root_y + geom->height / 2
					> scr->height_in_pixels
					- (BORDERPX*2))
					? (scr->height_in_pixels - geom->height
					- (BORDERPX*2))
					: pointer->root_y - geom->height / 2;

				if (pointer->root_x < geom->width/2)
					values[0] = 0;
				if (pointer->root_y < geom->height/2)
					values[1] = 0;

				xcb_configure_window(conn, win,
					XCB_CONFIG_WINDOW_X
					| XCB_CONFIG_WINDOW_Y, values);
				xcb_flush(conn);
			} else if (values[2] == 3) {
				geom = xcb_get_geometry_reply(conn,
					xcb_get_geometry(conn, win), NULL);
				values[0] = pointer->root_x - geom->x;
				values[1] = pointer->root_y - geom->y;
				xcb_configure_window(conn, win,
					XCB_CONFIG_WINDOW_WIDTH
					| XCB_CONFIG_WINDOW_HEIGHT, values);
				xcb_flush(conn);
			}
		} break;

		case XCB_BUTTON_RELEASE:
			//focus(win, ACTIVE);
			setfocus(win);
			xcb_ungrab_pointer(conn, XCB_CURRENT_TIME);
			break;

		case XCB_CONFIGURE_NOTIFY:
			configurenotify(ev);
		break;

		}

		xcb_flush(conn);
		free(ev);
	}
}
void
configurenotify(xcb_generic_event_t *ev)
{
	xcb_configure_notify_event_t *e;
	e = (xcb_configure_notify_event_t *)ev;

	if (e->window == root) {
		sw = e->width;
		sh = e->height;
	}
}
void
configurerequest(xcb_generic_event_t *ev)
{
	struct Client	*c;
	uint32_t	 values[1];

	xcb_configure_request_event_t *e;
	e = (xcb_configure_request_event_t *)ev;

	if ((c = findclient(e->window)) != NULL) {
		if(e->value_mask & XCB_CONFIG_WINDOW_X)
		       c->x = e->x;
		if(e->value_mask & XCB_CONFIG_WINDOW_Y)
		       c->y = e->y;
		if(e->value_mask & XCB_CONFIG_WINDOW_WIDTH)
		       c->w = e->width;
		if(e->value_mask & XCB_CONFIG_WINDOW_HEIGHT)
			c->h = e->height;
		if(e->value_mask & XCB_CONFIG_WINDOW_BORDER_WIDTH)
			c->bw = e->border_width;
		if(e->value_mask & XCB_CONFIG_WINDOW_SIBLING){
			values[0] = e->sibling;
			xcb_configure_window(conn, e->window,
				       XCB_CONFIG_WINDOW_SIBLING, values);
		}

		if(e->value_mask & XCB_CONFIG_WINDOW_STACK_MODE) {
			values[0] = e->stack_mode;
			xcb_configure_window(conn, e->window,
					XCB_CONFIG_WINDOW_STACK_MODE, values);
		}
		configure(c);
	} else { 
		uint16_t	mask = 0;
		uint32_t	conf_values[7];
		unsigned short	i = 0;

		if(e->value_mask & XCB_CONFIG_WINDOW_X) {
			mask |= XCB_CONFIG_WINDOW_X;
			conf_values[i++] = e->x;
		}
		if(e->value_mask & XCB_CONFIG_WINDOW_Y) {
			mask |= XCB_CONFIG_WINDOW_Y;
			conf_values[i++] = e->y;
		}
		if(e->value_mask & XCB_CONFIG_WINDOW_WIDTH) {
			mask |= XCB_CONFIG_WINDOW_WIDTH;
			conf_values[i++] = e->width;
		}
		if(e->value_mask & XCB_CONFIG_WINDOW_HEIGHT) {
			mask |= XCB_CONFIG_WINDOW_HEIGHT;
			conf_values[i++] = e->height;
		}
		if(e->value_mask & XCB_CONFIG_WINDOW_BORDER_WIDTH) {
			mask |= XCB_CONFIG_WINDOW_BORDER_WIDTH;
			conf_values[i++] = e->border_width;
		}
		if(e->value_mask & XCB_CONFIG_WINDOW_SIBLING) {
			mask |= XCB_CONFIG_WINDOW_SIBLING;
			conf_values[i++] = e->sibling;
		}
		if(e->value_mask & XCB_CONFIG_WINDOW_STACK_MODE) {
			mask |= XCB_CONFIG_WINDOW_STACK_MODE;
			conf_values [i++] = e->stack_mode;
		}

		xcb_configure_window(conn, e->window, mask, conf_values);
	}



	xcb_flush(conn);
}

void
maprequest(xcb_generic_event_t *ev)
{
	xcb_map_request_event_t *e;
	e = (xcb_map_request_event_t *)ev;

	xcb_get_window_attributes_reply_t *attr;
	attr = xcb_get_window_attributes_reply(conn,
			xcb_get_window_attributes(conn,
				e->window), NULL);

	if (!attr)
		return;

	xcb_map_window(conn, e->window);
	if (!attr->override_redirect) {
		manage(e->window);
		//focus(e->window, ACTIVE);
		setfocus(e->window);
	}
}
