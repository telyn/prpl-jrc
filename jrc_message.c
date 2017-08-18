#include "jrc_message.h"
#include <stdlib.h>
#include <string.h>

bool jrc_msg_has_subtype(const char type, const char possibleSubtype, const int num_parts)  {
	switch(type) {
		case JRC_MSG_WHOIS:
			return (possibleSubtype == JRC_MSG_NAME);
		case JRC_MSG_PASSWORD:
			return true;
		case JRC_MSG_NUMERICINFO:
		case JRC_MSG_GENERALINFO:
			return (num_parts < 2) || (possibleSubtype == JRC_MSG_ROOMUSERLIST);
		case JRC_MSG_SERVERMESSAGE:
			return true;
		default:
			return false;
	}
	return false;
}

bool jrc_msg_has_separate_subtype(const char type, const char possibleSubtype, const int num_parts) {
	switch(type) {
		case JRC_MSG_WHOIS:
			return (possibleSubtype != JRC_MSG_NAME);
		case JRC_MSG_GENERALINFO:
			return (possibleSubtype != JRC_MSG_ROOMUSERLIST) && (num_parts >= 2);
	}
	return false;
}

bool jrc_msg_has_subsubtype(const char type, const char subtype) {
	if( (type == JRC_MSG_WHOIS) && (subtype == JRC_MSG_INFO) ) {
			return true;
		}
	return false;
}

void jrc_msg_debug(const jrc_message * const msg) {

}

void jrc_msg_shift_part(jrc_message *msg) {
	jrc_strarr_shift(msg->parts, msg->num_parts);
	msg->num_parts--;
}

jrc_message jrc_msg_parse(const char * const msgstr) {
	jrc_message msg = {
		NULL,
		'\0',
		'\0',
		'\0',
		false,
		0,
		NULL
	};
	char *new_str = jrc_str_copy(msgstr);

	{ // count number of parts
		int idx = 0;
		char c = new_str[0];
		while(c != '\0') {
			if(c == '\t') {
				msg.num_parts++;
			}
			idx++;
			c = new_str[idx];
		}
	}

	{ // fill msg.parts
		int malloc_parts = msg.num_parts+1;
		msg.parts = msg.orig_parts = malloc(malloc_parts*sizeof(char*));

		for(int i=0; i < msg.num_parts; i++) {
			msg.parts[i] = strsep(&new_str, "\t");
		}
	}

	{ // process type
		msg.type = msg.parts[0][0];
		msg.parts[0] = (char *)jrc_str_right(msg.parts[0], 1);
	}

	{ // at this point we need to make sure that weird whois messages get processed in a more normal fashion - so we massage the weird one so they look more like a normal message with a separate-subcommand
		if (msg.type == JRC_MSG_WHOIS) {
			if( (0 == strcmp(msg.parts[1], "entrywhois")) || 
					(0 == strcmp(msg.parts[1], "autowhois")) ||
					(0 == strcmp(msg.parts[1], "whois")) ) {
				jrc_msg_shift_part(&msg);
			}
		}
	}

	{ // process subtype & subsubtype (they always come together)
		if(jrc_msg_has_separate_subtype(msg.type, msg.parts[0][0], msg.num_parts)) { 
			msg.subtype = msg.parts[1][0];
			msg.parts[1] = (char *)jrc_str_right(msg.parts[1], 1);
			if(0 == strlen(msg.parts[0])) {
				jrc_msg_shift_part(&msg);
			}
			if(jrc_msg_has_subsubtype(msg.type, msg.subtype) && msg.num_parts >= 2) {
				msg.subsubtype = msg.parts[1][0];
				msg.parts[1] = (char *)jrc_str_right(msg.parts[1], 1);
				if(0 == strlen(msg.parts[0])) {
					jrc_msg_shift_part(&msg);
				}
			}
		} else if(jrc_msg_has_subtype(msg.type, msg.parts[0][0], msg.num_parts)) {
			msg.subtype = msg.parts[0][0];
			msg.parts[0] = (char *)jrc_str_right(msg.parts[0], 1);
			if(jrc_msg_has_subsubtype(msg.type, msg.subtype) && msg.num_parts >= 2) {
				msg.subsubtype = msg.parts[0][0];
				msg.parts[0] = (char *)jrc_str_right(msg.parts[0], 1);
		}
		}
	}

	// TODO: parse WHOIS+INFO+USERINFO into a dictionary of some kind

	return msg;
}

void jrc_msg_destroy(jrc_message *msg) {
	msg->parts = NULL;
	if(msg->orig_parts != NULL) {
		free(msg->orig_parts);
		msg->orig_parts = NULL;
	}
	if(msg->time != NULL) {
		free((void*)msg->time);
	}
}
