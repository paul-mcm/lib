#define MAXLEN 18
#define MINLEN 9
#define ADDRLEN 16

struct cidr_ip_range {
        char ip[MAXLEN + 1];
        char low[ADDRLEN];
        char high[ADDRLEN];
};

int ip_range(struct cidr_ip_range *);
void format_addr(char **, char *, int, char *);

