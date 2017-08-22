#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
		jrc_debugf("compare_jrc_messages: times are different\n");
		return false;
	}
	if( (msg1->type != msg2->type) ||
(msg1->subtype != msg2->subtype) ||
(msg1->subsubtype != msg2->subsubtype) ||
(msg1->undo != msg2->undo) ||
(msg1->num_parts != msg2->num_parts) 
			) {
		jrc_debugf("compare_jrc_messages: ttype, subtype, subsubtypem undo or num_parts differ\n");
		return false;
	}

	// now check each part for string equality
	for(int i = 0; i < msg1->num_parts; i++) {

		bool part_is_same = compare_str_ptrs(msg1->parts[i], msg2->parts[i]);
		if(!part_is_same) {
			jrc_debugf("part %d ('%s' vs '%s') differ\n", i, msg1->parts[i], msg2->parts[i]);
			return false;
		}
	}
	// we do not care about orig_parts

	return true;
}

void test_compare_jrc_message() {
	char *one = "one";
	char *two = "two";
	char *three = "three";

	char **arr_one = malloc(sizeof(char*)*3);
	arr_one[0] = one;
	arr_one[1] = two;
	arr_one[2] = three;
	char **arr_two = malloc(sizeof(char*)*3);
	arr_two[0] = one;
	arr_two[1] = two;
	arr_two[2] = one;

	jrc_message same1 = {"hello", 'a', 'b', 'c', false, 3, arr_one, arr_one};
	jrc_message same2 = {"hello", 'a', 'b', 'c', false, 3, arr_one, arr_one};
	jrc_message diff1 = {"market", 'a', 'b', 'c', false, 3, arr_one, arr_one};
	jrc_message diff2 = {"hello", 'b', 'b', 'c', false, 3, arr_one, arr_one};
	jrc_message diff3 = {"hello", 'a', 'a', 'c', false, 3, arr_one, arr_one};
	jrc_message diff4 = {"hello", 'a', 'b', 'd', false, 3, arr_one, arr_one};
	jrc_message diff5 = {"hello", 'a', 'b', 'c', true, 3, arr_one, arr_one};
	jrc_message diff6 = {"hello", 'a', 'b', 'c', false, 2, arr_one, arr_one};
	jrc_message diff7 = {"hello", 'a', 'b', 'c', false, 3, arr_two, arr_two};

	assert(compare_jrc_messages(&same1, &same2));
	assert(!compare_jrc_messages(&same1, &diff1));
	assert(!compare_jrc_messages(&same1, &diff2));
	assert(!compare_jrc_messages(&same1, &diff3));
	assert(!compare_jrc_messages(&same1, &diff4));
	assert(!compare_jrc_messages(&same1, &diff5));
	assert(!compare_jrc_messages(&same1, &diff6));
	assert(!compare_jrc_messages(&same1, &diff7));

	printf("test_compare_jrc_message OK\n");
}

char *jrc_messages_printf_parts(jrc_message *msg) {
	int len = 0;
	//printf("jrc_messages_printf_parts time!\n");
	for(int i = 0; i < msg->num_parts; i++) {
		len += 3 + strlen(msg->parts[i]);
	}
	char *str = malloc(len);
	
	int cursor =0;
	for(int i = 0; i < msg->num_parts; i++) {
		str[cursor++] = '"';

		for(int c = 0; c < strlen(msg->parts[i]); c++) {
			str[cursor++] = msg->parts[i][c];
		}

		str[cursor++] = '"';
		str[cursor++] = ',';
		
	}
	str[len-1] = '\0';
	
	return str;
}

#define jrc_message_test_case(msg_str,...) do{\
	char *str = msg_str; \
	message_tests++; \
	jrc_debugf("\nParsing %s\n", str); \
	jrc_message expected = __VA_ARGS__; \
	jrc_message actual = jrc_msg_parse(str); \
	jrc_debugf("type: '%c' subtype: '%c' subsubtype: '%c'\n",actual.type, actual.subtype, actual.subsubtype); \
	jrc_debugf("parts: %s\n",  jrc_messages_printf_parts(&actual)); \
	(assert(compare_jrc_messages(&expected, &actual) && (msg_str))); \
	jrc_debugf("parsed correctly.\n"); \
} while(false)

void test_message_parse() {
	int message_tests = 0;
	// these test cases are all written in the following form:
	// sent by // message purpose - description of parts in half-assed form
	
	// client // Initial login - WHOIS, client, NAME, username
	jrc_message_test_case("^JRNet 1.1.636\tAtest-user", {NULL, '^', 'A', '\0', false, 2, (char*[]){"JRNet 1.1.636","test-user"}});

	// server // SERVERMESSAGE SERVERCHALLENGE challenge number // permission denied // server
	jrc_message_test_case("w~1498", {NULL, 'w', '~', '\0', false, 1, (char*[]){"1498"}});

	// client // SERVERCHALLENGE challenge response
	jrc_message_test_case("~23202", {NULL, '~', '\0', '\0', false, 1, (char*[]){"23202"}}); 

	 // client // GENERALINFO USERINFO // gets list of users on server
	jrc_message_test_case("Ha", {NULL, 'H', 'a', '\0', false, 1, (char*[]){""}});

	// client // GENERALINFO ROOMINFO // gets list of rooms on server
	jrc_message_test_case("Hb", {NULL, 'H', 'b', '\0', false, 1, (char*[]){""}});

	// client // NUMERICALINFO MAIL // get mail count
	jrc_message_test_case("GT", {NULL, 'G', 'T', '\0', false, 1, (char*[]){""}});

	// client // JOIN Creatures // join the Creatures room
	jrc_message_test_case("DCreatures", {NULL, 'D', '\0', '\0', false, 1, (char*[]){"Creatures"}});

	// server // GENERALINFO USERINFO test-user // test-user is on the server
	jrc_message_test_case("H\tatest-user", {NULL, 'H', 'a', '\0', false, 1, (char*[]){"test-user"}});

	// server // GENERALINFO ROOMLIST Creatures, Creatures RPG // Creatures and Creatures RPG are the rooms on the server
	jrc_message_test_case("H\tbCreatures\tCreatures RPG", {NULL, 'H', 'b', '\0', false, 2, (char*[]){"Creatures","Creatures RPG"}});

	//server // NUMERICINFO MAIL 0 // no messages
	jrc_message_test_case("GT0", {NULL, 'G','T','\0', false, 1, (char*[]){"0"}});
	
	// server // JOIN test-user Creatures // test-user has joined creatures
	jrc_message_test_case("Dtest-user\tCreatures", {NULL, 'D','\0','\0',false, 2, (char*[]){"test-user","Creatures"}});
	
	// client // GENERALINFO ROOMUSERLIST Creatures // what users are in Creatures?
	// node-jrc ignores Hc. I don't know why. It's either because it believes it should be H\tc or because I
	// deliberately prevented it.
	// It probably shouldn't ignore it.
	// TODO: explore whether this should have the empty part shifted off the front.
	jrc_message_test_case("Hc\tCreatures", {NULL, 'H','c','\0',false, 2, (char*[]){"","Creatures"}});

	// client // WHOIS autowhois INFO ROOMUSERINFO // typical autowhois - gets a lot of info about all users in the same room as the sender
	jrc_message_test_case("^autowhois\tIc", {NULL, '^', 'I','c', false, 2, (char*[]){"autowhois",""}});

	// server // WHOIS autowhois INFO USERINFO test-user Room: Creatures, IP: 127.0.0.1, Access: User, Client: JRNet 1.1.636, Idle: 0m0s, Login: 0m0s // response autowhois  
	jrc_message_test_case("^\tautowhois\tIatest-user\tRoom\2Creatures\3IP\002127.0.0.1\3Access\2Us\3Client\2JRNet 1.1.636\3Idle\0020m0s\3Login\0020m0s", {NULL, '^','I','a', false, 3, (char*[]){"autowhois", "test-user", "Room\2Creatures\3IP\002127.0.0.1\3Access\2Us\3Client\2JRNet 1.1.636\3Idle\0020m0s\3Login\0020m0s"}});

	
	// client // MESSAGE Hello
	jrc_message_test_case("EHello", {NULL, 'E', '\0','\0',false, 1, (char*[]){"Hello"}, NULL});
	
	// client // MESSAGE test-user Hello // test-user has said hello
	jrc_message_test_case("Etest-user\tHello", {NULL, 'E','\0','\0', false, 2, (char*[]){"test-user","Hello"}});


	printf("test_message_parse OK - %d messages tested\n\n", message_tests);
}

int main() {
	test_compare_jrc_message();
	test_message_parse();
}
