/* Haiku window system support.  Hey Emacs, this is -*- C++ -*-
   Copyright (C) 2021-2022 Free Software Foundation, Inc.

This file is part of GNU Emacs.

GNU Emacs is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

GNU Emacs is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Emacs.  If not, see <https://www.gnu.org/licenses/>.  */

#ifndef _HAIKU_SUPPORT_H
#define _HAIKU_SUPPORT_H

#include <stdint.h>

#ifdef HAVE_FREETYPE
#include <ft2build.h>
#include <fontconfig/fontconfig.h>
#include FT_FREETYPE_H
#include FT_SIZES_H
#endif

#ifdef USE_BE_CAIRO
#include <cairo.h>
#endif

#include <math.h>

#include <kernel/OS.h>

enum haiku_cursor
  {
    CURSOR_ID_NO_CURSOR = 12,
    CURSOR_ID_RESIZE_NORTH = 15,
    CURSOR_ID_RESIZE_EAST = 16,
    CURSOR_ID_RESIZE_SOUTH = 17,
    CURSOR_ID_RESIZE_WEST = 18,
    CURSOR_ID_RESIZE_NORTH_EAST = 19,
    CURSOR_ID_RESIZE_NORTH_WEST = 20,
    CURSOR_ID_RESIZE_SOUTH_EAST = 21,
    CURSOR_ID_RESIZE_SOUTH_WEST = 22,
    CURSOR_ID_RESIZE_NORTH_SOUTH = 23,
    CURSOR_ID_RESIZE_EAST_WEST = 24,
    CURSOR_ID_RESIZE_NORTH_EAST_SOUTH_WEST = 25,
    CURSOR_ID_RESIZE_NORTH_WEST_SOUTH_EAST = 26
  };

enum haiku_alert_type
  {
    HAIKU_EMPTY_ALERT = 0,
    HAIKU_INFO_ALERT,
    HAIKU_IDEA_ALERT,
    HAIKU_WARNING_ALERT,
    HAIKU_STOP_ALERT
  };

enum haiku_event_type
  {
    QUIT_REQUESTED,
    FRAME_RESIZED,
    FRAME_EXPOSED,
    KEY_DOWN,
    KEY_UP,
    ACTIVATION,
    MOUSE_MOTION,
    BUTTON_DOWN,
    BUTTON_UP,
    ICONIFICATION,
    MOVE_EVENT,
    SCROLL_BAR_VALUE_EVENT,
    SCROLL_BAR_PART_EVENT,
    SCROLL_BAR_DRAG_EVENT,
    WHEEL_MOVE_EVENT,
    MENU_BAR_RESIZE,
    MENU_BAR_CLICK,
    MENU_BAR_OPEN,
    MENU_BAR_SELECT_EVENT,
    MENU_BAR_CLOSE,
    FILE_PANEL_EVENT,
    MENU_BAR_HELP_EVENT,
    ZOOM_EVENT,
    DRAG_AND_DROP_EVENT,
    APP_QUIT_REQUESTED_EVENT,
    DUMMY_EVENT,
    MENU_BAR_LEFT
  };

struct haiku_quit_requested_event
{
  void *window;
};

struct haiku_resize_event
{
  void *window;
  float px_heightf;
  float px_widthf;
};

struct haiku_expose_event
{
  void *window;
  int x;
  int y;
  int width;
  int height;
};

struct haiku_drag_and_drop_event
{
  void *window;
  int x, y;
  void *message;
};

struct haiku_app_quit_requested_event
{
  char dummy;
};

struct haiku_dummy_event
{
  char dummy;
};

#define HAIKU_MODIFIER_ALT (1)
#define HAIKU_MODIFIER_CTRL (1 << 1)
#define HAIKU_MODIFIER_SHIFT (1 << 2)
#define HAIKU_MODIFIER_SUPER (1 << 3)

struct haiku_key_event
{
  void *window;
  int modifiers;
  unsigned keysym;
  uint32_t multibyte_char;

  /* Time the keypress occurred, in microseconds.  */
  bigtime_t time;
};

struct haiku_activation_event
{
  void *window;
  int activated_p;
};

struct haiku_mouse_motion_event
{
  void *window;
  bool just_exited_p;
  int x;
  int y;
  bigtime_t time;
  bool dnd_message;
};

struct haiku_menu_bar_left_event
{
  void *window;
  int x, y;
};

struct haiku_menu_bar_click_event
{
  void *window;
  int x, y;
};

struct haiku_button_event
{
  void *window;
  int btn_no;
  int modifiers;
  int x;
  int y;
  bigtime_t time;
};

struct haiku_iconification_event
{
  void *window;
  int iconified_p;
};

struct haiku_move_event
{
  void *window;
  int x;
  int y;
};

struct haiku_wheel_move_event
{
  void *window;
  int modifiers;
  float delta_x;
  float delta_y;
};

struct haiku_menu_bar_select_event
{
  void *window;
  void *ptr;
};

struct haiku_file_panel_event
{
  void *ptr;
};

struct haiku_menu_bar_help_event
{
  void *window;
  int mb_idx;
  void *data;
  bool highlight_p;
};

struct haiku_zoom_event
{
  void *window;
  bool zoomed;
};

enum haiku_font_specification
  {
    FSPEC_FAMILY      = 1,
    FSPEC_STYLE	      = 1 << 1,
    FSPEC_SLANT	      = 1 << 2,
    FSPEC_WEIGHT      = 1 << 3,
    FSPEC_SPACING     = 1 << 4,
    FSPEC_WANTED      = 1 << 5,
    FSPEC_NEED_ONE_OF = 1 << 6,
    FSPEC_WIDTH	      = 1 << 7,
    FSPEC_LANGUAGE    = 1 << 8,
  };

typedef char haiku_font_family_or_style[64];

enum haiku_font_slant
  {
    NO_SLANT = -1,
    SLANT_OBLIQUE,
    SLANT_REGULAR,
    SLANT_ITALIC
  };

enum haiku_font_width
  {
    NO_WIDTH = -1,
    ULTRA_CONDENSED,
    EXTRA_CONDENSED,
    CONDENSED,
    SEMI_CONDENSED,
    NORMAL_WIDTH,
    SEMI_EXPANDED,
    EXPANDED,
    EXTRA_EXPANDED,
    ULTRA_EXPANDED
  };

enum haiku_font_language
  {
    LANGUAGE_CN,
    LANGUAGE_KO,
    LANGUAGE_JP,
    MAX_LANGUAGE /* This isn't a language. */
  };

struct haiku_font_pattern
{
  int specified;
  struct haiku_font_pattern *next;
  /* The next two fields are only temporarily used during the font
     discovery process! Do not rely on them being correct outside
     BFont_find.  */
  struct haiku_font_pattern *last;
  struct haiku_font_pattern *next_family;
  haiku_font_family_or_style family;
  haiku_font_family_or_style style;
  int weight;
  int mono_spacing_p;
  int want_chars_len;
  int need_one_of_len;
  enum haiku_font_slant slant;
  enum haiku_font_width width;
  enum haiku_font_language language;
  uint32_t *wanted_chars;
  uint32_t *need_one_of;

  int oblique_seen_p;
};

struct haiku_scroll_bar_value_event
{
  void *scroll_bar;
  void *window;
  int position;
};

struct haiku_scroll_bar_drag_event
{
  void *scroll_bar;
  void *window;
  int dragging_p;
};

enum haiku_scroll_bar_part
  {
    HAIKU_SCROLL_BAR_UP_BUTTON,
    HAIKU_SCROLL_BAR_DOWN_BUTTON
  };

struct haiku_scroll_bar_part_event
{
  void *scroll_bar;
  void *window;
  enum haiku_scroll_bar_part part;
};

struct haiku_menu_bar_resize_event
{
  void *window;
  int width;
  int height;
};

struct haiku_menu_bar_state_event
{
  void *window;
};

#define HAIKU_THIN 0
#define HAIKU_ULTRALIGHT 20
#define HAIKU_EXTRALIGHT 40
#define HAIKU_LIGHT 50
#define HAIKU_SEMI_LIGHT 75
#define HAIKU_REGULAR 100
#define HAIKU_SEMI_BOLD 180
#define HAIKU_BOLD 200
#define HAIKU_EXTRA_BOLD 205
#define HAIKU_ULTRA_BOLD 210
#define HAIKU_BOOK 400
#define HAIKU_HEAVY 800
#define HAIKU_ULTRA_HEAVY 900
#define HAIKU_BLACK 1000
#define HAIKU_MEDIUM 2000

#ifdef __cplusplus
/* Haiku's built in Height and Width functions for calculating
   rectangle sizes are broken, probably for compatibility with BeOS:
   they do not round up in a reasonable fashion, and they return the
   numerical difference between the end and start sides in both
   directions, instead of the actual size.

   For example:

     BRect (1, 1, 5, 5).IntegerWidth ()

   Will return 4, when in reality the rectangle is 5 pixels wide,
   since the left corner is also a pixel!

   All code in Emacs should use the macros below to calculate the
   dimensions of a BRect, instead of relying on the broken Width and
   Height functions.  */

#define BE_RECT_HEIGHT(rect) (ceil (((rect).bottom - (rect).top) + 1))
#define BE_RECT_WIDTH(rect) (ceil (((rect).right - (rect).left) + 1))
#endif /* __cplusplus */

/* C++ code cannot include lisp.h, but file dialogs need to be able
   to bind to the specpdl and handle quitting correctly.  */

#ifdef __cplusplus
#if SIZE_MAX > 0xffffffff
#define WRAP_SPECPDL_REF 1
#endif
#ifdef WRAP_SPECPDL_REF
typedef struct { ptrdiff_t bytes; } specpdl_ref;
#else
typedef ptrdiff_t specpdl_ref;
#endif

#else
#include "lisp.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif
#include <pthread.h>
#include <OS.h>

#ifdef __cplusplus
typedef void *haiku;

extern void haiku_put_pixel (haiku, int, int, unsigned long);
extern unsigned long haiku_get_pixel (haiku, int, int);
#endif

extern port_id port_application_to_emacs;
extern port_id port_popup_menu_to_emacs;

extern void haiku_io_init (void);
extern void haiku_io_init_in_app_thread (void);

extern void haiku_read_size (ssize_t *, bool);

extern int haiku_read (enum haiku_event_type *, void *, ssize_t);
extern int haiku_read_with_timeout (enum haiku_event_type *, void *, ssize_t,
				    bigtime_t, bool);
extern int haiku_write (enum haiku_event_type, void *);
extern int haiku_write_without_signal (enum haiku_event_type, void *, bool);

extern void rgb_color_hsl (uint32_t, double *, double *, double *);
extern void hsl_color_rgb (double, double, double, uint32_t *);

extern void *BBitmap_new (int, int, int);
extern void *BBitmap_data (void *);
extern int BBitmap_convert (void *, void **);

extern void BBitmap_free (void *);

extern void BBitmap_dimensions (void *, int *, int *,  int *, int *,
				int32_t *, int *);
extern void *BApplication_setup (void);
extern void *BWindow_new (void **);
extern void BWindow_quit (void *);

extern void BWindow_set_offset (void *, int, int);
extern void BWindow_iconify (void *);
extern void BWindow_set_visible (void *, int);
extern void BWindow_retitle (void *, const char *);
extern void BWindow_resize (void *, int, int);
extern void BWindow_activate (void *);
extern void BWindow_center_on_screen (void *);
extern void BWindow_change_decoration (void *, int);
extern void BWindow_set_tooltip_decoration (void *);
extern void BWindow_set_avoid_focus (void *, int);
extern uint32_t BWindow_workspaces (void *);
extern void BWindow_zoom (void *);
extern void BWindow_set_min_size (void *, int, int);
extern void BWindow_set_size_alignment (void *, int, int);
extern void BWindow_sync (void *);
extern void BWindow_send_behind (void *, void *);
extern bool BWindow_is_active (void *);
extern void BWindow_set_override_redirect (void *, bool);
extern void BWindow_dimensions (void *, int *, int *);
extern void BWindow_Flush (void *);

extern void BFont_close (void *);
extern void BFont_dat (void *, int *, int *, int *, int *,
		       int *, int *, int *, int *, int *, int *);
extern int BFont_have_char_p (void *, int32_t);
extern int BFont_have_char_block (void *, int32_t, int32_t);
extern void BFont_char_bounds (void *, const char *, int *, int *, int *);
extern void BFont_nchar_bounds (void *, const char *, int *, int *,
				int *, int32_t);
extern struct haiku_font_pattern *BFont_find (struct haiku_font_pattern *);


extern void BView_StartClip (void *);
extern void BView_EndClip (void *);
extern void BView_SetHighColor (void *, uint32_t);
extern void BView_SetHighColorForVisibleBell (void *, uint32_t);
extern void BView_SetLowColor (void *, uint32_t);
extern void BView_SetPenSize (void *, int);
extern void BView_SetFont (void *, void *);
extern void BView_MovePenTo (void *, int, int);
extern void BView_DrawString (void *, const char *, ptrdiff_t);
extern void BView_DrawChar (void *, char);
extern void BView_FillRectangle (void *, int, int, int, int);
extern void BView_FillRectangleAbs (void *, int, int, int, int);
extern void BView_FillTriangle (void *, int, int, int, int, int, int);
extern void BView_StrokeRectangle (void *, int, int, int, int);
extern void BView_SetViewColor (void *, uint32_t);
extern void BView_ClipToRect (void *, int, int, int, int);
extern void BView_ClipToInverseRect (void *, int, int, int, int);
extern void BView_StrokeLine (void *, int, int, int, int);
extern void BView_CopyBits (void *, int, int, int, int, int, int, int, int);
extern void BView_InvertRect (void *, int, int, int, int);
extern void BView_DrawBitmap (void *, void *, int, int, int, int, int, int,
			      int, int);
extern void BView_DrawBitmapWithEraseOp (void *, void *, int, int, int, int);
extern void BView_DrawMask (void *, void *, int, int, int, int,	int, int,
			    int, int, uint32_t);

extern void BView_resize_to (void *, int, int);
extern void BView_set_view_cursor (void *, void *);
extern void BView_move_frame (void *, int, int, int, int);
extern void BView_scroll_bar_update (void *, int, int, int, int, bool);

extern void *BBitmap_transform_bitmap (void *, void *, uint32_t, double,
				       int, int);

extern void BScreen_px_dim (int *, int *);
extern void BScreen_res (double *, double *);

/* Functions for creating and freeing cursors.  */
extern void *BCursor_create_default (void);
extern void *BCursor_from_id (enum haiku_cursor);
extern void *BCursor_create_modeline (void);
extern void *BCursor_create_i_beam (void);
extern void *BCursor_create_progress_cursor (void);
extern void *BCursor_create_grab (void);
extern void BCursor_delete (void *);

extern void *BScrollBar_make_for_view (void *, int, int, int, int, int, void *);
extern void BScrollBar_delete (void *);
extern int BScrollBar_default_size (int);

extern void BView_invalidate (void *);
extern void BView_draw_lock (void *, bool, int, int, int, int);
extern void BView_invalidate_region (void *, int, int, int, int);
extern void BView_draw_unlock (void *);

extern void BBitmap_import_fringe_bitmap (void *, unsigned short *, int, int);
extern void BBitmap_import_mono_bits (void *, void *, int, int);

extern void haiku_font_pattern_free (struct haiku_font_pattern *);

extern int BFont_open_pattern (struct haiku_font_pattern *, void **, float);
extern void BFont_populate_fixed_family (struct haiku_font_pattern *);
extern void BFont_populate_plain_family (struct haiku_font_pattern *);

extern void BView_publish_scroll_bar (void *, int, int, int, int);
extern void BView_forget_scroll_bar (void *, int, int, int, int);
extern bool BView_inside_scroll_bar (void *, int, int);
extern void BView_get_mouse (void *, int *, int *);
extern void BView_convert_to_screen (void *, int *, int *);
extern void BView_convert_from_screen (void *, int *, int *);

extern void BView_emacs_delete (void *);
extern uint32_t haiku_current_workspace (void);

extern void *BPopUpMenu_new (const char *);

extern void BMenu_add_item (void *, const char *, void *, bool,
			    bool, bool, void *, const char *,
			    const char *);
extern void BMenu_add_separator (void *);
extern void *BMenu_new_submenu (void *, const char *, bool);
extern void *BMenu_new_menu_bar_submenu (void *, const char *);
extern int BMenu_count_items (void *);
extern void *BMenu_item_at (void *, int);
extern void *BMenu_run (void *, int, int, void (*) (void *, void *),
			void (*) (void), void (*) (void),
			struct timespec (*) (void), void *);
extern void BPopUpMenu_delete (void *);
extern void *BMenuBar_new (void *);
extern void BMenu_delete_all (void *);
extern void BMenuBar_delete (void *);
extern void BMenu_item_set_label (void *, const char *);
extern void *BMenu_item_get_menu (void *);
extern void BMenu_delete_from (void *, int, int);

extern void haiku_ring_bell (void);

extern void *BAlert_new (const char *, enum haiku_alert_type);
extern void *BAlert_add_button (void *, const char *);
extern void BAlert_set_offset_spacing (void *);
extern int32 BAlert_go (void *, void (*) (void), void (*) (void),
			void (*) (void));
extern void BButton_set_enabled (void *, int);
extern void BView_set_tooltip (void *, const char *);
extern void BView_show_tooltip (void *);
extern void BView_set_and_show_sticky_tooltip (void *, const char *,
					       int, int);

extern void BAlert_delete (void *);

extern void EmacsWindow_parent_to (void *, void *);
extern void EmacsWindow_unparent (void *);
extern void EmacsWindow_move_weak_child (void *, void *, int, int);
extern void EmacsWindow_make_fullscreen (void *, int);
extern void EmacsWindow_unzoom (void *);

extern void be_get_version_string (char *, int);
extern int be_get_display_planes (void);
extern int be_get_display_color_cells (void);
extern void be_warp_pointer (int, int);

extern void EmacsView_set_up_double_buffering (void *);
extern void EmacsView_disable_double_buffering (void *);
extern void EmacsView_flip_and_blit (void *);
extern int EmacsView_double_buffered_p (void *);

extern char *be_popup_file_dialog (int, const char *, int,
				   int, void *, const char *,
				   const char *, void (*) (void),
				   void (*) (void), void (*) (void));

extern void record_c_unwind_protect_from_cxx (void (*) (void *), void *);
extern specpdl_ref c_specpdl_idx_from_cxx (void);
extern void c_unbind_to_nil_from_cxx (specpdl_ref);

#ifdef HAVE_NATIVE_IMAGE_API
extern int be_can_translate_type_to_bitmap_p (const char *);
extern void *be_translate_bitmap_from_file_name (const char *);
extern void *be_translate_bitmap_from_memory (const void *, size_t);
#endif

extern bool BMenuBar_start_tracking (void *);
extern size_t BBitmap_bytes_length (void *);

#ifdef USE_BE_CAIRO
extern cairo_t *EmacsView_cairo_context (void *);
extern void BView_cr_dump_clipping (void *, cairo_t *);
extern void EmacsWindow_begin_cr_critical_section (void *);
extern void EmacsWindow_end_cr_critical_section (void *);
#endif

extern void BMenu_add_title (void *, const char *);

extern int be_plain_font_height (void);
extern int be_string_width_with_plain_font (const char *);
extern int be_get_display_screens (void);
extern bool be_use_subpixel_antialiasing (void);
extern const char *be_find_setting (const char *);
extern haiku_font_family_or_style *be_list_font_families (size_t *);

extern void BMessage_delete (void *);

extern bool be_drag_message (void *, void *, bool, void (*) (void),
			     void (*) (void), void (*) (void),
			     bool (*) (void));
extern bool be_drag_and_drop_in_progress (void);

extern void be_replay_menu_bar_event (void *, struct haiku_menu_bar_click_event *);

#ifdef __cplusplus
extern void *find_appropriate_view_for_draw (void *);
}

extern _Noreturn void gui_abort (const char *);
#endif /* _cplusplus */

#endif /* _HAIKU_SUPPORT_H_ */

// Local Variables:
// eval: (setf (alist-get 'inextern-lang c-offsets-alist) 0)
// End:
