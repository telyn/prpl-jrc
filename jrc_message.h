#ifndef _JRC_MESSAGE_H
#define _JRC_MESSAGE_H
#include <stdbool.h>

#include "jrc_util.h"
#include "jrc_constants.h"

typedef struct {
	char * time;
	char type;
	char subtype;
	char subsubtype;
	bool undo;
	int num_parts;
	// parts should not contain type, subtype, or subsubtype
	char **parts;

	// ptr needed to free() during destroy
	char **orig_parts;
} jrc_message;

bool jrc_msg_has_subtype(const char type, const char possibleSubtype, const int num_parts);
bool jrc_msg_has_separate_subtype(const char type, const char possibleSubType, const int num_parts);
bool jrc_msg_has_subsubtype(const char type, const char subtype);

void jrc_msg_debug(const jrc_message * const msg);
jrc_message jrc_msg_parse(const char * const);

void jrc_msg_destroy(jrc_message * msg);

#endif
