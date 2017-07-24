#include "prpl.h"
#include "accountopt.h"
#include "version.h"

static PurplePlugin *_jrc_protocol = NULL;

static void jrc_prpl_login(PurpleAccount * acct) {
}

static void jrc_prpl_close(PurpleConnection * gc) {
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
}

PURPLE_INIT_PLUGIN(jrc,jrc_purple_init,info)
