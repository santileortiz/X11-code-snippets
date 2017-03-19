#include <xcb/xcb.h>
// Struct definitions in comments here are provided for quick reference, updated
// ones are found in <xcb_git_repo>/xcb/libxcb/src/xproto.h

/* Function to get the following information:
 *
 * typedef struct xcb_query_pointer_reply_t {
 *     uint8_t      response_type;
 *     uint8_t      same_screen;
 *     uint16_t     sequence;
 *     uint32_t     length;
 *     xcb_window_t root;
 *     xcb_window_t child;
 *     int16_t      root_x;
 *     int16_t      root_y;
 *     int16_t      win_x;
 *     int16_t      win_y;
 *     uint16_t     mask;
 *     uint8_t      pad0[2];
 * } xcb_query_pointer_reply_t;
 *
 */
void poll_pointer (xcb_connection_t *c, xcb_drawable_t  window)
{
    xcb_generic_error_t *err = NULL;
    xcb_query_pointer_cookie_t ck = xcb_query_pointer (c, window);
    xcb_query_pointer_reply_t *ptr_state = xcb_query_pointer_reply (c, ck, &err);
    if (err != NULL) {
        printf ("Error while polling pointer.\n");
    }

    bool left_button_state = (XCB_KEY_BUT_MASK_BUTTON_1 & ptr_state->mask) ? 1 : 0;
    bool middle_button_state = (XCB_KEY_BUT_MASK_BUTTON_2 & ptr_state->mask) ? 1 : 0;
    bool right_button_state = (XCB_KEY_BUT_MASK_BUTTON_3 & ptr_state->mask) ? 1 : 0;
    free (ptr_state);
}

// An atom is just a uint32_t that references a string inside the X11 server.
// Some of them are predefined and have macros defined in XCB for their values.
// But in general, any string can be turned into an atom, this function does
// that.
xcb_atom_t get_atom (xcb_connection_t *c, const char *value)
{
    xcb_atom_t res;
    xcb_generic_error_t *err = NULL;
    xcb_intern_atom_cookie_t ck = xcb_intern_atom (c, 0, strlen(value), value);
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply (c, ck, &err);
    if (err != NULL) {
        printf ("Error while requesting atom.\n");
    }
    res = reply->atom;
    free(reply);
    return res;
}
