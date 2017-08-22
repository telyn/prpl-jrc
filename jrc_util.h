#ifndef _JRC_UTIL_H
#define _JRC_UTIL_H

#define jrc_debugf(...) do { \
	if(getenv("JRC_DEBUG")!=NULL) { \
		printf(__VA_ARGS__);\
	} \
} while(false)

char *jrc_str_copy(const char * const str);

// returns a substring of str starting at character n
const char * const jrc_str_right(const char * const str, const int n);

// shift the front element off the array of strings
void jrc_strarr_shift(char **strarr, int num_elems);

#endif
