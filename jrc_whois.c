#include "jrc_whois.h"
#include <string.h>

int jrc_whois_len(jrc_whois *whois) {
	len = 15; // 7 fields * 2 - one key/value separator, one record separator. plus null
	if(whois.name != NULL) {
		len += strlen(whois.name);
	}
	if(whois.room != NULL) {
		len += strlen(whois.room);
	}
	if(whois.ip != NULL) {
		len += strlen(whois.ip);
	}
	if(whois.access != NULL) {
		len += strlen(whois.access);
	}
	if(whois.client != NULL) {
		len += strlen(whois.client);
	}
	if(whois.idle != NULL) {
		len += strlen(whois.idle);
	}
	if(whois.login != NULL) {
		len += strlen(whois.login);
	}
	return len;
}

void jrc_whois_destroy(jrc_whois *whois) {
	if(whois.name != NULL) {
		free(whois.name)
	}
	if(whois.name != NULL) {
		free(whois.name)
	}
	if(whois.name != NULL) {
		free(whois.name)
	}
	if(whois.name != NULL) {
		free(whois.name)
	}
	if(whois.name != NULL) {
		free(whois.name)
	}
	if(whois.name != NULL) {
		free(whois.name)
	}
	if(whois.name != NULL) {
		free(whois.name)
	}
}

jrc_whois jrc_whois_decode(char *whois) {
	jrc_whois whois = {
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
	};
	// TODO
	return whois;
}

void jrc_whois_encode(jrc_whois *whois, char *buf) {
	// TODO
}
