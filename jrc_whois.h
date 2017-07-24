#ifndef _JRC_WHOIS
#define _JRC_WHOIS

typedef struct {
	char *name;
	char *room;
	char *ip;
	char *access;
	char *client;
	char *idle;
	char *login;
} jrc_whois;

jrc_whois jrc_whois_decode(char *whois);
int jrc_whois_len(jrc_whois *whois);
void jrc_whois_encode(jrc_whois *whois, char *buf)

#endif
