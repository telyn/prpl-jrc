#include "jrc_message.h"

char *jrc_msg_param(const jrc_message *msg, int param) {
	return jrc_strtokn(msg.line, "\t", param);
}

bool jrc_msg_has_subtype(const jrc_message *msg) {
	switch(msg.type) {
		case JRC_MSG_WHOIS:
			return (jrc_msg_param(msg,0)[0] == JRC_MSG_NAMES);
		case JRC_MSG_PASSWORD:
			return true;
		case JRC_MSG_NUMERICINFO:
		case JRC_MSG_GENERALINFO:
			return (jrc_message_params(msg) < 2) || (jrc_msg_param(msg,0)[0] == JRC_MSG_ROOMUSERLIST);
		case JRC_MSG_SERVERMESSAGE:
			return true;
		default:
			return false;
	}
	return false;
}

bool jrc_msg_has_separate_subtype(const jrc_message *msg) {
	switch(msg.type) {
		case JRC_MSG_WHOIS:
			return (msg.line[0] != JRC_MSG_NAME);
		case JRC_MSG_GENERALINFO:
			return (msg.line[0] != JRC_MSG_ROOMUSERLIST) && (jrc_msg_params(msg) >= 2);
	}
	return false;
}

bool jrc_msg_has_subsubtype(const jrc_message *msg) {
	if(msg.type == JRC_MSG_WHOIS) && (msg.subtype == JRC_MSG_INFO) {
			return true;
		}
	return false;
}

void jrc_msg_debug(const jrc_message *) {

}
jrc_message jrc_msg_process(const char *str) {
	jrc_message msg = {
		NULL,
		'\0',
		'\0',
		'\0',
		false,
		str
	};

}
