#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./include/iprange.h"

int ip_range(struct cidr_ip_range *ips)
{
	int hbits, bocti, boctv, i, range, max, min = 0; range = 1;
	long long nmask;
	char *mask, **oct, *octets[4], *addr;
	const char *errstr;

	if (strlen(ips->ip) > 18) {
	    printf("bad input\n");
	    return(-1);
	}

	memset(ips->low, '\0', sizeof(ips->low));
	memset(ips->high, '\0', sizeof(ips->low));

	/* SET VARIABLES */
	mask = strndup(ips->ip, 18);  /* dup whole buff, e.g., ip/mask */
	addr = strsep(&mask, "/");

	oct = octets;	
	while ((*oct = strsep(&addr, ".")) != NULL)
	    oct++;

	nmask = strtonum(mask, 1, 32, &errstr);
	hbits = 32 - nmask;
	bocti = (nmask / 8); /* array index of octet with net/host boundary */
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

	/* PUT LOWER RANGE IN STRUCT */
	snprintf(octets[bocti], 4, "%d", min);

	for (i = 0; i < 4; i++) {
	    if (i > 0)
		strlcat(ips->low, ".", sizeof(ips->low)); 
	    if (i > (bocti))
		octets[i] = "0";		
	    strlcat(ips->low, octets[i], sizeof(ips->low));
	}

	/* PUT UPPER RANGE IN STRUCT */
	snprintf(octets[bocti], 4, "%d", max);

	for (i = 0; i < 4; i++) {
	    if (i > 0)
		strlcat(ips->high, ".", sizeof(ips->high));
	    if (i > (bocti)) 
		octets[i] = "255";
	    strlcat(ips->high, octets[i], sizeof(ips->high));
	}
	return 0;
}
