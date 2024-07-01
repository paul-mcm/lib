#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define MAXLEN 18
#define MINLEN 7

int validate_ipv4(char *);

int validate_ipv4(char *addr) {

	int len, i;
	char *mask, *o;
	long long oct;
	const char *errstr;

	/* Validate CIDR addr length */
        len = strlen(addr);
        if (len > MAXLEN || len < MINLEN) {
            printf("invalid length for CIDR addr\n");
            return(-1);
        }

        /* SET VARIABLES */
        mask = strndup(addr, (MAXLEN - 1));
        addr = strsep(&mask, "/");


        /* VALIDATE ADDR */
        for (i = 0; i < 4; i++) {
            if ((o = strsep(&addr, ".")) == NULL) {
                printf("missing octets\n");
                return(-1);
            }
            if ((oct = strtonum(o, 0, 255, &errstr)) == 0 && errno) {
                printf("bad bad octet: %s\n", errstr);
                return(-1);
            }
        }

        /* There should be nothing left in addr */
        if (addr != NULL) {
            printf("Too many octets\n");
            return(-1);
        }

	/* VALIDATE NETMASK */
	if (mask != NULL && *mask == '\0') {
	    printf("Incomplete netmask\n");
	    return(-1);
	}

        if (mask != NULL && (strtonum(mask, 1, 32, &errstr) == 0) ) {
	    printf("Netmask Error: %s\n", errstr);
	    return(-1);
	}

	return 0;
}
