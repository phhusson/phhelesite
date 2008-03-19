#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "ezxml.h"

extern void parse_args(char **envp);
extern char **args_w_opt,**opts,**args;
extern int args_ln,arg_w_opt_ln;

extern short get_dept(char*);
extern void init_dept();
extern void end_dept();

int main(int argc, char **argv, char **envp) {
	init_dept();
	get_dept(getenv("REMOTE_ADDR"));
	printf("Content-type: text/html\n\n");
	parse_args(envp);
	char *cmd;
	asprintf(&cmd, "curl http://www.rugbyrama.fr/z/rugby/rss.xml 2>/dev/null");
	FILE *fd=popen(cmd, "r");
	ezxml_t tree=ezxml_parse_fp(fd);
	pclose(fd);
	free(cmd);
	if(!tree)
		return 1;
	tree=ezxml_child(tree, "channel");
	ezxml_t item=ezxml_child(tree, "item");
	if(!item)
		return 2;
	printf("<html>\n<head><meta name=\"info_page\" content=\"index.html\">\n\n");
	printf("\t<body>\n\t\t<table width=600 border=0 cellpadding=8 cellspacing=0 align=center>\n");
	int i=0;
	do {
		ezxml_t title=ezxml_child(item, "title");
		char *url=ezxml_child(item, "link")->txt;
		if(!url) continue;
		url=index(url, '/')+3;
		if(!url) continue;
		url=index(url, '/')+1;
		if(!url) continue;
		printf("<tr><td><a href='eurosport2.cgi?%s'>%s</a></td></tr>\n", url, title->txt);
	} while((item=item->next));
	printf("\t\t</table>\n\t\t<table width=600 border=0 cellpadding=8 cellspacing=0 align=center>");
	printf("\t\t<tr><td><FONT family=\"Symbol\" color=\"#FF000020\"size=5>\x29</FONT>&nbsp; Sortie&nbsp;</td><td><FONT family=\"Symbol\" color=\"#0000FF20\"size=5>\x29</FONT>&nbsp; Page suivante</td><td>"
			"<FONT family=\"Symbol\" color=\"#00FF0020\"size=5>\x29</FONT>&nbsp; Page precedente</td></td></tr></table>\n</body>\n</html>\n");
	end_dept();
	return 0;
}

