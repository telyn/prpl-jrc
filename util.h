#ifndef _JRC_UTIL_H
#define _JRC_UTIL_H
#include <string.h>

// get the nth token from the given str using strtok
char *jrc_strtokn(char *str, char *delim, int n) {
	char *tok = NULL;
	while(n > 0) {
		tok = strtok(msg.line, delim);
		if(n == NULL) {
			return NULL;
		}
		n--;
	}
	return tok;
}

#endif
