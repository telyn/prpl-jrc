#ifndef _JRC_CONNECTION_H
#define _JRC_CONNECTION_H

typedef struct {
	char *username;
	char *password;
	PurpleAccount *acct;
	guint read_handle;
	guint write_handle;

} jrc_connection;

#endif
