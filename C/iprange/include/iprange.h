#define MAXLEN 19

struct cidr_ip_range {
        char ip[MAXLEN];
        char low[16];
        char high[16];
};

int ip_range(struct cidr_ip_range *);

