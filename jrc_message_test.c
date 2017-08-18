#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "jrc_message.h"

bool compare_str_ptrs(char *a, char *b) {
	if(a == NULL) {
		if(b != NULL) {
			return false;
		}
	} else if (a != NULL) {
		if(b == NULL) {
			return false;
		}
		if(0 != strcmp(a, b)) {
			return false;
		}
	}
	return true;
}

bool compare_jrc_messages(jrc_message *msg1, jrc_message *msg2) {
	if(!compare_str_ptrs(msg1->time, msg2->time)) {
		return false
	}
	if( (msg1->type != msg2->type) ||
(msg1->subtype != msg2->subtype) ||
(msg1->subsubtype != msg2->subsubtype) ||
(msg1->undo != msg2->undo) ||
(msg1->num_parts != msg2->num_parts) 
			)

	return true;
}

void test_message_parse() {
	jrc_message expected = {NULL, '\0', '\0', '\0', false, 0, NULL, NULL};
	assert(false && "test_message_parse()");
}

int main() {
	test_message_parse();
}
