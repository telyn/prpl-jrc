#include "prpl.h"
#include "jrc_connection.h"
#include "accountopt.h"
#include "version.h"

#define JRC_ID "jrc"

#ifdef _WIN32
#define sys_read  wpurple_read
#define sys_write wpurple_write
#define sys_close wpurple_close
extern "C" {
  int wpurple_close(int fd);
  int wpurple_write(int fd, const void *buf, unsigned int size);
  int wpurple_read(int fd, void *buf, unsigned int size);
}
#else
#include <unistd.h>
#define sys_read  read
#define sys_write write
#define sys_close close
#endif

const char JRC_DEFAULT_SERVER[] = "main.srv.telynz.uk";
const int JRC_DEFAULT_PORT = 41528;

static PurplePlugin *_jrc_protocol = NULL;

static gboolean jrc_prpl_send_keepalive(gpointer data) {
	PurpleConnection *gc = (PurpleConnection*)data;
}


// this is called some time after purple_proxy_connect and is a callback given to the same.
// it's responsible for checking whether the connection has been successfully made (i.e. source is a valid file descriptor), sending the initial login data and setting up the event loop callbacks (purple_input_add and purple_timeout_add_seconds)
static void jrc_prpl_connect_cb(gpointer data, gint source, const gchar * error_message) {
	PurpleConnection *gc = (PurpleConnection*)data;
	jrc_connection *jrc_conn = (jrc_connection *)purple_connection_get_protocol_data(gc);

	PurpleAccount *acct = purple_connection_get_account(gc);
}

// called at the beginning of a login attempt - i.e. immediately after the checkbox is ticked in pidgin
// this is responsible for setting up the data structures needed during a connection, and beginning
// a connection with purple_proxy_connect.
static void jrc_prpl_login(PurpleAccount * acct) {
	PurpleConnection *gc = purple_account_get_connection(acct);

	purple_connection_update_progress(gc, "Connecting", 0, STEPS);


	const char *username = purple_account_get_username(acct);
	const char *password = purple_account_get_password(acct);
	const char *server = purple_account_get_string(acct, "server", "");
	const int port = purple_account_get_int(acct, "port", 41528);

	jrc_connection *jrc_conn = (jrc_connection*)g_new0(jrc_connection, 1);
	jrc_conn->username = username;
	jrc_conn->password = password;
	jrc_conn->acct = acct;


	purple_connection_set_protocol_data(gc, conn);

	if (purple_proxy_connect(gc, acct, server, port, jrc_prpl_connect_cb, gc) == NULL) {
		purple_connection_error_reason(gc, PURPLE_CONNECTION_ERROR_NETWORK_ERROR, sprintf("Unable to connect to %s:%d", server, port));
	}
	
}

static void jrc_prpl_close(PurpleConnection * gc) {
	jrc_connection *jrc_conn = (jrc_connection *)purple_connection_get_protocol_data(gc);

	if (jrc_conn->read_handle)
		purple_input_remove(jrc_conn->read_handle);
	
	if (jrc_conn->write_handle)
		purple_input_remove(jrc_conn->write_handle);

	if (jrc_conn->fd)
		sys_close(jrc_conn->fd);

	purple_connection_set_protocol_data(gc, 0)
}

static const char *jrc_prpl_list_icon(PurpleAccount * acct, PurpleBuddy * buddy)
{
	return "jrc";
}

static PurplePluginProtocolInfo proto_info = {
	(PurpleProtocolOptions)0,			/* options */
	NULL,			/* user_splits, initialized in jrc_prpl_init() */
	NULL,			/* protocol_options, initialized in jrc_prpl_init() */
	{			/* icon_spec, a PurpleBuddyIconSpec */
		"png,gif,bmp,tiff,jpg",			/* format */
		1,			/* min_width */
		1,			/* min_height */
		4096,			/* max_width */
		4096,			/* max_height */
		8*1024*1024,	/* max_filesize */
		PURPLE_ICON_SCALE_SEND,	/* scale_rules */
	},
	jrc_prpl_list_icon,	/* list_icon */
	NULL,			/* list_emblem */
	NULL,	/* status_text */
	NULL,	/* tooltip_text */
	NULL,	/* status_types */
	NULL,			/* blist_node_menu */
	NULL,	/* chat_info */
	NULL,	/* chat_info_defaults */
	jrc_prpl_login,		/* login */
	jrc_prpl_close,		/* close */
	NULL,		/* send_im */
	NULL,			/* set_info */
	NULL,	/* send_typing */
	NULL,	/* get_info */
	NULL,	/* set_status */
	NULL,			/* set_idle */
	NULL,			/* change_passwd */
	NULL,	/* add_buddy */
	NULL,	/* add_buddies */
	NULL,	/* remove_buddy */
	NULL,	/* remove_buddies */
	NULL,			/* add_permit */
	NULL,	/* add_deny */
	NULL,			/* rem_permit */
	NULL,	/* rem_deny */
	NULL,			/* set_permit_deny */
	NULL,	/* join_chat */
	NULL,			/* reject_chat */
	NULL,	/* get_chat_name */
	NULL,	/* chat_invite */
	NULL,			/* chat_leave */
	NULL,			/* chat_whisper */
	NULL,	/* chat_send */
	NULL,			/* keepalive */
	NULL,			/* register_user */
	NULL,			/* get_cb_info */
	NULL,			/* get_cb_ajrc_y */
	NULL,			/* alias_buddy */
	NULL,	/* group_buddy */
	NULL,	/* rename_group */
	NULL,			/* buddy_free */
	NULL,	/* convo_closed */
	purple_normalize_nocase,	/* normalize */
	NULL,	/* set_buddy_icon */
	NULL,			/* remove_group */
	NULL,			/* get_cb_real_name */
	NULL,			/* set_chat_topic */
	NULL,			/* find_blist_chat */
	NULL,			/* roomlist_get_list */
	NULL,			/* roomlist_cancel */
	NULL,			/* roomlist_expand_category */
	NULL,	/* can_receive_file */
	NULL,	/* send_file */
	NULL,			/* new_xfer */
	NULL,	/* offline_message */
	NULL,			/* whiteboard_prpl_ops */
	NULL,			/* send_raw */
	NULL,			/* roomlist_room_serialize */
	NULL,			/* unregister_user */
	NULL,			/* send_attention */
	NULL,			/* get_attention_types */
	sizeof(PurplePluginProtocolInfo),	/* struct_size */
	NULL,			/* get_account_text_table */
	NULL,			/* initiate_media */
	NULL,			/* get_media_caps */
	NULL,			/* get_moods */
	NULL,			/* set_public_alias */
	NULL,			/* get_public_alias */
	NULL,			/* add_buddy_with_invite */
	NULL			/* add_buddies_with_invite */
};

static PurplePluginInfo info = {
	PURPLE_PLUGIN_MAGIC,	/* magic */
	PURPLE_MAJOR_VERSION,	/* major_version */
	PURPLE_MINOR_VERSION,	/* minor_version */
	PURPLE_PLUGIN_PROTOCOL,	/* type */
	NULL,			/* ui_requirement */
	0,			/* flags */
	NULL,			/* dependencies */
	PURPLE_PRIORITY_DEFAULT,/* priority */
	"prpl-telyn-jrc",	/* id */
	"JRChat",		/* name */
	"0.1.0",		/* version */
	"JRChat protocol for libpurple",	/* summary */
	"JRChat protocol for libpurple",	/* description */
	"Telyn (telyn@telynz.uk)",	/* author */
	"http://telynz.uk",	/* homepage */
	NULL,			/* load */
	NULL,			/* unload */
	NULL,			/* destroy */
	NULL,			/* ui_info */
	&proto_info,		/* extra_info */
	NULL,			/* prefs_info */
	NULL,		/* actions */
	NULL,			/* padding... */
	NULL,
	NULL,
	NULL,
};

static void jrc_purple_init(PurplePlugin *plugin) {
	PurpleAccountOption *option;

	proto_info.user_splits = NULL;

	option = purple_account_option_string_new("Server", "server", JRC_DEFAULT_SERVER);
	proto_info.protocol_options = g_list_append(proto_info.protocol_options, option);

	option = purple_account_option_int_new("Port", "port", JRC_DEFAULT_PORT);
	proto_info.protocol_options = g_list_append(proto_info.protocol_options, option);
}

PURPLE_INIT_PLUGIN(jrc,jrc_purple_init,info)
