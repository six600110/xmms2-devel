#ifndef __XMMS_PLAYLIST_H__
#define __XMMS_PLAYLIST_H__

#include <glib.h>

#include "xmms/object.h"

/*
 * Public definitions
 */

typedef enum {
	XMMS_PLAYLIST_APPEND,
	XMMS_PLAYLIST_PREPEND,
	XMMS_PLAYLIST_INSERT,
	XMMS_PLAYLIST_DELETE
} xmms_playlist_actions_t;

typedef enum {
	XMMS_PLAYLIST_CHANGED_ADD,
	XMMS_PLAYLIST_CHANGED_SET_POS,
	XMMS_PLAYLIST_CHANGED_SHUFFLE,
	XMMS_PLAYLIST_CHANGED_REMOVE,
	XMMS_PLAYLIST_CHANGED_CLEAR,
	XMMS_PLAYLIST_CHANGED_MOVE
} xmms_playlist_changed_actions_t;


#define xmms_playlist_entry_foreach_prop(a,b,c) g_hash_table_foreach(a->properties, b, c)

#define XMMS_PLAYLIST_ENTRY_PROPERTY_ARTIST "artist"
#define XMMS_PLAYLIST_ENTRY_PROPERTY_ALBUM "album"
#define XMMS_PLAYLIST_ENTRY_PROPERTY_TITLE "title"
#define XMMS_PLAYLIST_ENTRY_PROPERTY_YEAR "date"
#define XMMS_PLAYLIST_ENTRY_PROPERTY_TRACKNR "tracknr"
#define XMMS_PLAYLIST_ENTRY_PROPERTY_GENRE "genre"
#define XMMS_PLAYLIST_ENTRY_PROPERTY_BITRATE "bitrate"
#define XMMS_PLAYLIST_ENTRY_PROPERTY_COMMENT "comment"
#define XMMS_PLAYLIST_ENTRY_PROPERTY_DURATION "duration"
#define XMMS_PLAYLIST_ENTRY_PROPERTY_CHANNEL "channel"
#define XMMS_PLAYLIST_ENTRY_PROPERTY_SAMPLERATE "samplerate"

/*
 * Private defintions
 */

#define XMMS_MAX_URI_LEN 1024
#define XMMS_MEDIA_DATA_LEN 1024

/** Playlist structure */
typedef struct xmms_playlist_St {
	xmms_object_t object;
	/** The current list first node. */
	GList *list;
	/** Next song that will be retured by xmms_playlist_get_next */
	GList *currententry;

	GHashTable *id_table;
	guint nextid;

	GMutex *mutex;
	GCond *cond;
	gboolean is_waiting;
} xmms_playlist_t;

typedef struct xmms_playlist_entry_St {
	gchar *uri;
	gchar *mimetype;
	
	guint id;
	
	guint ref;

	GHashTable *properties;
} xmms_playlist_entry_t;

typedef struct xmms_playlist_changed_msg_St {
	gint type;
	guint id;
	gpointer arg;
} xmms_playlist_changed_msg_t;

/*
 * Public functions
 */

xmms_playlist_t * xmms_playlist_init ();

gboolean xmms_playlist_add (xmms_playlist_t *playlist, xmms_playlist_entry_t *file, gint options);
guint xmms_playlist_entries_total (xmms_playlist_t *playlist);
guint xmms_playlist_entries_left (xmms_playlist_t *playlist);
gboolean xmms_playlist_set_current_position (xmms_playlist_t *playlist, guint id);
gint xmms_playlist_get_current_position (xmms_playlist_t *playlist);
xmms_playlist_entry_t * xmms_playlist_get_byid (xmms_playlist_t *playlist, guint id);
xmms_playlist_entry_t * xmms_playlist_get_next_entry (xmms_playlist_t *playlist);
xmms_playlist_entry_t * xmms_playlist_get_prev_entry (xmms_playlist_t *playlist);
xmms_playlist_entry_t * xmms_playlist_get_current_entry (xmms_playlist_t *playlist);
GList * xmms_playlist_list (xmms_playlist_t *playlist);
void xmms_playlist_wait (xmms_playlist_t *playlist);
void xmms_playlist_sort (xmms_playlist_t *playlist, gchar *property);
void xmms_playlist_shuffle (xmms_playlist_t *playlist);
void xmms_playlist_clear (xmms_playlist_t *playlist);
gboolean xmms_playlist_id_remove (xmms_playlist_t *playlist, guint id);

xmms_playlist_entry_t *xmms_playlist_entry_alloc ();

/*
 * Entry modifications
 */

xmms_playlist_entry_t * xmms_playlist_entry_new (gchar *uri);

void xmms_playlist_entry_set_prop (xmms_playlist_entry_t *entry, gchar *key, gchar *value);
void xmms_playlist_entry_set_uri (xmms_playlist_entry_t *entry, gchar *uri);
void xmms_playlist_entry_mimetype_set (xmms_playlist_entry_t *entry, const gchar *mimetype);
const gchar *xmms_playlist_entry_mimetype_get (xmms_playlist_entry_t *entry);
gchar *xmms_playlist_entry_get_uri (const xmms_playlist_entry_t *entry);
guint xmms_playlist_entry_id_get (xmms_playlist_entry_t *entry);
gchar *xmms_playlist_entry_get_prop (const xmms_playlist_entry_t *entry, gchar *key);
gint xmms_playlist_entry_get_prop_int (const xmms_playlist_entry_t *entry, gchar *key);
void xmms_playlist_entry_copy_property (xmms_playlist_entry_t *entry, xmms_playlist_entry_t *newentry);
void xmms_playlist_entry_print (xmms_playlist_entry_t *entry);
gboolean xmms_playlist_entry_is_wellknown (gchar *property);
void xmms_playlist_entry_ref (xmms_playlist_entry_t *entry);
void xmms_playlist_entry_unref (xmms_playlist_entry_t *entry);

#endif
