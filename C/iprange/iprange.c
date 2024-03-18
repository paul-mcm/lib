#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/iprange.h"

int ip_range(struct cidr_ip_range *ips)
{
	int hbits, bocti, boctv, i, len, max, min = 0, range = 1;
	long long nmask, oct;
	char *mask, *octets[4], *addr;
	const char *errstr;

	memset(ips->low, '\0', sizeof(ips->low));
	memset(ips->high, '\0', sizeof(ips->low));

	/* Validate CIDR addr length */
	len = strlen(ips->ip);
	if (len > MAXLEN || len < MINLEN) {
	    printf("invalid length for CIDR addr\n");
	    return(-1);
	}

	/* SET VARIABLES */
	mask = strndup(ips->ip, (MAXLEN - 1));
	addr = strsep(&mask, "/");

	/* VALIDATE ADDR */
	for (i = 0; i < 4; i++) {
	    if ((octets[i] = strsep(&addr, ".")) == NULL) {
	 	printf("missing octets\n");
		return(-1);
	    }
	    if ((oct = strtonum(octets[i], 0, 255, &errstr)) == 0 && errno) {
		printf("bad octet: %s\n", errstr);
		return(-1);
	    }
	}
	/* There should be nothing left in addr */
	if (addr != NULL) {
	    printf("Too many octets\n");
	    return(-1);
	}

	/* VALIDATE NETMASK */
	if (mask == NULL || *mask == '\0') {
	    printf("bad netmask\n");
	    return(-1);
	} else if (( nmask = strtonum(mask, 1, 32, &errstr)) == 0) {
	    printf("Netmask Error: %s\n", errstr);
	    return(-1);
	}

	hbits = 32 - nmask;
	bocti = (nmask / 8);
	boctv = strtonum(octets[bocti], 0, 255, &errstr);

	if (nmask % 8 == 0) {
	    min = 0;
	    range = 256;
	} else {
	    range <<= hbits % 8;
	    min = boctv;
	    min >>= hbits % 8;
	    min <<= hbits %8;
	}

	max = min + range - 1;

	snprintf(octets[bocti], 4, "%d", min);
	format_addr(octets, ips->low, bocti, "0");

 	snprintf(octets[bocti], 4, "%d", max);
 	format_addr(octets, ips->high, bocti, "255");

	return 0;
}

void format_addr(char **o, char *addr, int b, char *v) {
	int i;
	for (i = 0; i < 4; i++) {
	    if (i > 0)
		strlcat(addr, ".", ADDRLEN);
	    if (i > b)
		o[i] = v;
	    strlcat(addr, o[i], ADDRLEN);;
	}
}

