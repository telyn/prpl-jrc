#include <stdlib.h>
#include <string.h>
#include "jrc_util.h"

char *jrc_str_copy(const char * const str) {
	char *new = malloc(strlen(str)*sizeof(char));
	strcpy(new, str);
	return new;
}

// returns a substring of str starting at character n
const char * const jrc_str_right(const char * const str, const int n) {
	return str+sizeof(char)*n;
}

// shift the front element off the array of strings
void jrc_strarr_shift(char **strarr, int num_elems) {
	int i;
	for(i=1; i < num_elems; i++) {
		strarr[i-1] = strarr[i];
	}
	strarr[i] = NULL;
}
