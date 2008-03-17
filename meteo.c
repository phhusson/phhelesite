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

extern void parse_args(char **envp);
extern char **args_w_opt,**opts,**args;
extern int args_ln,arg_w_opt_ln;

extern short get_dept(char*);
extern void init_dept();
extern void end_dept();
typedef struct {
	char dep;
	char name[32];
} infos_t;

infos_t names[100] ={
	{  1, "macon"		},
	{  2, "amiens"		},
	{  3, "moulins"		},
	{  4, "nice"		},
	{  5, "grenoble"	},
	{  6, "nice"		},
	{  7, "valence"		},
	{  8, "reims"		},
	{  9, "tarbes"		},
	{ 10, "troyes"		},
	{ 11, "carcassonne"	},
	{ 12, "rodez"		},
	{ 13, "marseille"	},
	{ 14, "caen"		},
	{ 15, "aurillac"	},
	{ 16, "angouleme"	},
	{ 17, "la+rochelle"	},
	{ 18, "bourges"		},
	{ 19, "limoges"		},
	//{ 20, ""		}, //Corse
	{ 21, "dijon"		},
	{ 22, "saint+bireuc"	},
	{ 23, "chateauroux"	},
	{ 24, "perigueux"	},
	{ 25, "besancon"	},
	{ 26, "valence" 	},
	{ 27, "evreux"	 	},
	{ 28, "evreux"	 	},
	{ 29, "brest"	 	},
	{ 30, "montpellier" 	},
	{ 31, "toulouse" 	},
	{ 32, "mont+de+marsan" 	},
	{ 33, "bordeaux" 	},
	{ 34, "montpellier" 	},
	{ 35, "rennes"  	},
	{ 36, "chateauroux"  	},
	{ 37, "tours"   	},
	{ 38, "grenoble"   	},
	{ 39, "besancon"   	},
	{ 40, "mont+de+marsan"	},
	{ 41, "tours"		},
	{ 42, "macon"		},
	{ 43, "aurillac"	},
	{ 44, "nantes"		},
	{ 45, "orleans"		},
	{ 46, "cahors"		},
	{ 47, "agen"		},
	{ 48, "mende"		},
	{ 49, "angers"		},
	{ 50, "caen"		},
	{ 51, "reims"		},
	{ 52, "troyes"		},
	{ 53, "le+mans"		},
	{ 54, "nancy"		},
	{ 55, "reims"		},
	{ 56, "saint+brieuc"	},
	{ 57, "metz"		},
	{ 58, "nevers"		},
	{ 59, "lille"		},
	{ 60, "beauvais"	},
	{ 61, "caen"		},
	{ 62, "lille"		},
	{ 63, "clermont+ferrand"},
	{ 64, "pau"		},
	{ 65, "tarbes"		},
	{ 66, "perpignan"	},
	{ 67, "strasbourg"	},
	{ 68, "strasbourg"	},
	{ 69, "lyon"		},
	{ 70, "besancon"	},
	{ 71, "macon"		},
	{ 72, "le+mans"		},
	{ 73, "chambery"	},
	{ 74, "chambery"	},
	{ 75, "paris"		},
	{ 76, "rouen"		},
	{ 77, "paris"		},
	{ 78, "paris"		},
	{ 79, "la+rochelle"	},
	{ 80, "amiens"		},
	{ 81, "carcassonne"	},
	{ 82, "montauban"	},
	{ 83, "nice"		},
	{ 84, "valence"		},
	{ 85, "nantes"		},
	{ 86, "poitiers"	},
	{ 87, "limoges"		},
	{ 88, "nancy"		},
	{ 89, "auxerre"		},
	{ 90, "besancon"	},
	{ 91, "paris"		},
	{ 92, "paris"		},
	{ 93, "paris"		},
	{ 94, "paris"		},
	{ 95, "paris"		},
	{0, ""}
};

int main(int argc, char **argv, char **envp) {
	init_dept();

	printf("Content-type: text/html\n\n");
	parse_args(envp);
	//int dep=get_dept(getenv("REMOTE_ADDR"));
	int dep=get_dept("2a01:e35:2e4b:b2b0:21b:11ff:fec1:b7fb");
	char *dept;
	int i;
	for(i=0;names[i].dep!=0;i++) {
		if(names[i].dep==dep) {
			dept=names[i].name;
			break;
		}
	}
	printf("<html>\n<head><meta name=\"info_page\" content=\"index.html\">\n<link rel=\"green\" href=\"meteo.cgi?1\">\n</head><body>\n");
	printf("<table background=\"/meteo.gif\" width=600 border=0 cellpadding=8 cellspacing=0 align=center>\n");
	//printf("<tr><td>Ville detect&eacute;e <br>la plus proche:</td><td>%s</td></tr>\n", dept);
	char *cmd;
	asprintf(&cmd, "curl 'http://www.my-meteo.fr/meteo+rss+%s.html' 2>/dev/null", dept);
	FILE *fd;
	fd=popen(cmd, "r");
	ezxml_t tree=ezxml_parse_fp(fd);
	if(!tree)
		return -4;
	pclose(fd);
	free(cmd);
	tree=ezxml_child(tree, "channel");
	if(!tree)
		return -5;
	ezxml_t item=ezxml_child(tree, "item");
	if(!item)
		return -6;
	i=0;
	do {
		i++;
		if(i>5)
			break;
		ezxml_t title=ezxml_child(item, "title");
		if(!title)
			return -7;
		char *date=title->txt;
		*(index(date, ':')-1)=0;
		ezxml_t desc=ezxml_child(item, "description");
		if(!title)
			return -8;
		char *txt,*tmp;
		printf("<tr><td>%s</td><td>\n", title->txt);
		txt=desc->txt;
		tmp=txt;
		while((tmp=index(tmp+1, '-'))) {
			if(tmp[1]==' ' && (tmp-1)[0]==' ') {
				tmp[0]=0;
				printf("%s<br>\n", txt);
				txt=tmp+1;
			}
		}
		printf("</td></tr>\n");
	} while((item=item->next));
	printf("</table>\n</body>\n</html>");
	end_dept();
	return 0;
}
