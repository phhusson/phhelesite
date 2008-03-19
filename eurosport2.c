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
	char *file;
	if(args_ln<1)
		return 0;
	file=args[0];
	int i;
	for(i=0;i<=strlen(file);i++) {
		//Check don't get faked
		if(file[i]<'A' && file[i]>'Z')
			if(file[i]<'a' && file [i] > 'z')
				if(file[i]!='.' && file[i]!='/')
					return -1;
	}
	printf("Content-type: text/html\n\n");
	char *cmd;
	asprintf(&cmd, "wget -q -O - 'http://www.eurosport.fr/%s' 2>/dev/null |grep storyfulltitleboxcontainer|html2text|grep -Ev '(\\[|\\])' |tail -n +3", file);
	printf("%s\n", cmd);
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

