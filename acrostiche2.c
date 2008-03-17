#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "ezxml.h"

extern void parse_args(char **envp);
extern char **args_w_opt,**opts,**args;
extern int args_ln,arg_w_opt_ln;

int main(int argc, char **argv, char **envp) {
	parse_args(envp);
	int i=0;
	int nb=0;
	char *arg;
	int version=0;
	if(args_ln<2) 
		return 0;
	version=atoi(args[1]);
	if(version!=0 && version!=1)
		return 0;
	printf("Content-type: text/html\n\n");
	char *cmd;
	asprintf(&cmd, "curl http://acrostiche.org/telesite/animaux/\"%s\" 2>/dev/null", args[0]);
	FILE *fd=popen(cmd, "r");
	char *buffer=malloc(2048);
	printf("<html>\n<head><meta name=\"info_page\" content=\"index.html\">\n");
	if(version==0)
		printf("<link rel=\"red\" href=\"index.html\">\n");
	printf("<link rel=\"green\" href=\"acrostiche.cgi?%d\">\n", version);
	printf("\t<body background='s1zqh2ks.gif'>\n\t\t<table width=600 border=0 cellpadding=8 cellspacing=0 align=center>\n<tr><td>\n");
	while(!feof(fd)) {
		fgets(buffer, 2048, fd);
		if(strlen(buffer)<=1)
			continue;
		*(buffer+strlen(buffer)-1)=0;
		printf("%s\n", buffer);
		bzero(buffer, 2048);
	}
	printf("\t\t</tr></td></table>\n");
	printf("\t\t<table width=600 border=0 cellpadding=8 cellspacing=0 align=center>\n<tr>");
	if(version==0)
		printf("\t\t\t<td><FONT family=\"Symbol\" color=\"#FF000020\"size=5>\x29</FONT>&nbsp; Accueil&nbsp;</td>");
	printf("\t\t\t<td><FONT family=\"Symbol\" color=\"#00FF0020\"size=5>\x29</FONT>&nbsp; Retour&nbsp;</td></tr>");
	printf("\t\t</table>\n");
	printf("\t</body></html>\n");
	pclose(fd);
	printf("%s\n", buffer);
	free(cmd);
	free(buffer);
#if 0
	if(!tree)
		return 1;
	tree=ezxml_child(tree, "channel");
	ezxml_t item=ezxml_child(tree, "item");
	if(!item)
		return 2;
	printf("<html>\n<head><meta name=\"info_page\" content=\"index.html\">\n<link rel=\"red\" href=\"index.html\">\n<link rel=\"blue\" href=\"banane.cgi?%d&%d\">\n",nb,1-i);
	printf("<link rel=\"green\" href=\"acrostiche2.cgi?%d&%d\">\n</head>\n", nb,-1-i);
	printf("\t<body>\n\t\t<table width=600 border=0 cellpadding=8 cellspacing=0 align=center>\n");
	do {
		i++;
		if(i!=1)
			continue;
		ezxml_t desc=ezxml_child(item, "description");
		char *texte=strdup(desc->txt);
		char *nb=strstr(texte, "<a href='");
		nb[0]=0;
		nb+=34;
		nb[4]=0;
		printf("<tr><td>Blague n&ordm;%s&nbsp;<br/></tr></td><tr><td>%s</td></tr>\n", nb, texte);
		free(texte);
	} while((item=item->next));
	if(i<=1)
		printf("<tr><td>FIN, pour en voir d'autre allez sur http://www.labanane.org !</td></tr>\n");
	printf("\t\t</table>\n\t\t<table width=600 border=0 cellpadding=8 cellspacing=0 align=center>");
	printf("\t\t<tr><td><FONT family=\"Symbol\" color=\"#FF000020\"size=5>\x29</FONT>&nbsp; Accueil&nbsp;</td><td><FONT family=\"Symbol\" color=\"#0000FF20\"size=5>\x29</FONT>&nbsp; Page suivante</td><td>"
			"<FONT family=\"Symbol\" color=\"#00FF0020\"size=5>\x29</FONT>&nbsp; Page precedente</td></td></tr></table>\n</body>\n</html>\n");
#endif
	return 0;
}
