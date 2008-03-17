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

DB *dbp;

typedef struct {
	short dept;

} user_info;

int main(int argc, char **argv, char **envp) {
	int ret;
	db_create(&dbp, NULL, 0);
	ret=dbp->open(dbp, NULL, "users.db", NULL, DB_BTREE, 0, 0);
	if(ret!=0)
		return -1;
	DBC *cursorp;
	dbp->cursor(dbp, NULL, &cursorp, 0);
	DBT key,data;
	bzero(&key, sizeof(key));
	bzero(&data, sizeof(data));
	while ( (ret=cursorp->c_get(cursorp, &key, &data, DB_NEXT)) ==0 ) {
		struct in_addr *addr=key.data;
		short dep=*((short*)data.data);
		printf("%03d.%03d.%03d.%03d:%d\n", addr->s_addr&0xff, (addr->s_addr>>8)&0xff, (addr->s_addr>>16)&0xff,
				(addr->s_addr>>24)&0xff, dep);
	}
	return 0;
}
