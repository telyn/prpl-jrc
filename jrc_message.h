#ifndef _JRC_MESSAGE_H
#define _JRC_MESSAGE_H

typedef struct {
	const char const *time
	const char type;
	const char subtype;
	const char subsubtype;
	const bool undo;
	const char const *line;
} jrc_message;

bool jrc_msg_has_subtype(jrc_message *);
bool jrc_msg_has_separate_subtype(jrc_message *);
bool jrc_msg_has_subsubtype(jrc_message *);

void jrc_msg_debug(jrc_message *);
void jrc_msg_process(jrc_message *, char *)

void jrc_msg_destroy(jrc_message *);

#endif
