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
	int id;
	if(args_ln<1)
		return 0;
	id=atoi(args[0]);
	if(id<0)
		return 0;
	printf("Content-type: text/html\n\n");
	char *cmd;
	asprintf(&cmd, "curl 'http://www.euronews.net/index.php?article=%d&lng=2&option=1' |grep -oE '<p class=\"small1\">(.*)</p>' |sed -e 's/<p class=\"small1\">//g' |sed -e 's/<\\/p>//g' 2>/dev/null |iconv -f UTF-8 -t ISO-8859-15", id);
	char *buffer=malloc(32768);
	FILE *fd=popen(cmd, "r");
	fread(buffer, 32768, 1, fd);
	pclose(fd);
	free(cmd);
	printf("<html>\n<head><meta name=\"info_page\" content=\"index.html\">\n\n");
	printf("<link rel=\"red\" href=\"euronews.cgi\">\n");
	printf("\t</head><body>\n\t\t<table width=600 border=0 cellpadding=8 cellspacing=0 align=center>\n");
	printf("<tr><td>%s</td></tr>\n", buffer);
	printf("\t\t</table>\n\t\t<table width=600 border=0 cellpadding=8 cellspacing=0 align=center>");
	printf("\t\t<tr><td><FONT family=\"Symbol\" color=\"#FF000020\"size=5>\x29</FONT>&nbsp; Retour&nbsp;</td>"
			"</tr></table>\n</body>\n</html>\n");
	return 0;
}

