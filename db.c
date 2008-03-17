#include <stdio.h>
#include <db.h>
#include <strings.h>

int main(int argc, char **argv, char **envp) {
	DB *dbp;            /* DB structure handle */
	int ret;            /* function return value */
	/* Initialize the structure. This
	 * database is not opened in an environment,
	 * so the environment pointer is NULL. */
	ret = db_create(&dbp, NULL, 0);
	if (ret != 0) {
		/* Error handling goes here */
	}
	/* open the database */
	ret = dbp->open(dbp,          /* DB structure pointer */
		NULL,        /* Transaction pointer */
		"my_db.db",  /* On-disk file that holds the database. */
		NULL,        /* Optional logical database name */
		DB_BTREE,    /* Database access method */
		DB_CREATE,       /* Open flags */
		0);
	printf("%d\n", sizeof(data));
	DBT key,data;
	bzero(&key, sizeof(key));
	bzero(&data, sizeof(key));
	int t1,t2;t1=0;t2=1;
	char desc1[]="plop",desc2[]="test";
	key.data=&t1;key.size=sizeof(t1);
	data.data=desc1;key.size=sizeof(desc1);
	dbp->put(dbp, NULL, &key, &data, 0);

	bzero(&key, sizeof(key));
	bzero(&data, sizeof(key));
	key.data=&t2;key.size=sizeof(t2);
	data.data=desc2;key.size=sizeof(desc2);
	dbp->put(dbp, NULL, &key, &data, 0);

	dbp->close(dbp, 0);
	return 0;
}
