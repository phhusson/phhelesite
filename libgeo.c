#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <db.h>
#include "ezxml.h"

static short get_dept_nodb(struct in_addr *ipv4) {
	char *cmd;
	char buffer[256];
	if(!inet_ntop(AF_INET, ipv4, buffer, 256))
		return -3;
	asprintf(&cmd, "curl http://francois04.free.fr/liste_dslam.php'?ip=%s' 2>/dev/null |grep -oE '>[0-9A-Za-z]{3,3}[0-9]{2,2}<' |grep -oE '[0-9]{2,2}'", buffer);
	FILE *fd;
	fd=popen(cmd, "r");
	fgets(buffer, 256, fd);
	short dep=atoi(buffer);
	pclose(fd);
	free(cmd);
	return dep;
}

static DB *dbp;

typedef struct {
	short dept;

} user_info;

short get_dept(char *addr) {
	if(!addr) {
		printf("Couldn't get addr\n");
		return -3;
	}
	struct in6_addr addr6;
	if(!inet_pton(AF_INET6, addr, &addr6))
		return -2;
	//guessed from the ipv6 one
	struct in_addr ipv4_addr;
	//Likely to be screener
	if( (addr6.s6_addr[3]&0xf) != 5) {
		return -1;
	}
	ipv4_addr.s_addr=(addr6.s6_addr[3]&0xf)<<28;
	ipv4_addr.s_addr+=addr6.s6_addr[4]<<20;
	ipv4_addr.s_addr+=addr6.s6_addr[5]<<12;
	ipv4_addr.s_addr+=addr6.s6_addr[6]<<4;
	ipv4_addr.s_addr+=addr6.s6_addr[7]>>4;
	ipv4_addr.s_addr=htonl(ipv4_addr.s_addr);

	DBT key,data;
	bzero(&key, sizeof(key));
	bzero(&data, sizeof(data));

	if(!dbp)
		return get_dept_nodb(&ipv4_addr);

	key.data=&ipv4_addr;
	key.size=sizeof(struct in_addr);

	user_info *d=malloc(sizeof(user_info));
	bzero(d, sizeof(user_info));
	data.data=d;
	data.ulen=sizeof(user_info);
	data.flags=DB_DBT_USERMEM;

	if(dbp->get(dbp, NULL, &key, &data, 0) == 0) {
		return d->dept;
	}
	
	d->dept=get_dept_nodb(&ipv4_addr);
	data.flags=0;
	data.data=d;
	data.size=sizeof(user_info);
	dbp->put(dbp, NULL, &key, &data, 0);
	return d->dept;

}

void init_dept() {
	int ret;
	db_create(&dbp, NULL, 0);
	ret=dbp->open(dbp, NULL, "users.db", NULL, DB_BTREE, DB_CREATE, 0);
	if(ret!=0)
		dbp=NULL;

}

void end_dept() {
	dbp->close(dbp,0);
}
