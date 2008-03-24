#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <iconv.h>
#include "ezxml.h"

extern void parse_args(char **envp);
extern char **args_w_opt,**opts,**args;
extern int args_ln,arg_w_opt_ln;

int main(int argc, char **argv, char **envp) {
	parse_args(envp);
	char *file;
	if(args_ln<2)
		return 0;
	file=args[0];
	char *cat=args[1];
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
	asprintf(&cmd, "bash get_eurosport.sh '%s' 2>/dev/null ", file);
	printf("<html>\n<head><meta name=\"info_page\" content=\"index.html\">\n\n");
	printf("<link rel=\"red\" href=\"eurosport.cgi?%s\">\n", cat);
	printf("\t</head><body>\n\t\t<table width=600 border=0 cellpadding=8 cellspacing=0 align=center>\n");
	printf("<tr><td>\n");
	char *buffer=malloc(32768);
	char *tbuffer=buffer;
	FILE *fd=popen(cmd, "r");
	char *ret=malloc(2048),*bbuf;
	char *tret=ret;
	size_t ibl,obl;
	iconv_t icvt=iconv_open("UTF-8", "ISO-8859-15");

	while(!feof(fd)) {
		ibl=0;obl=2048;
		bzero(buffer, 32768);
		fgets(buffer, 32768, fd); //100 bytes should be enough when looking at html2text
		ibl=strlen(buffer);
		printf("%s&nbsp;<br>&nbsp;\n", buffer);
		iconv(icvt, &buffer, &ibl, &ret, &obl);
		bbuf=buffer;
		if(ibl==0 && ret) {
			bbuf=ret;
		}
		if(strlen(bbuf)<25 && strlen(bbuf)>5)
			printf("</td></tr>\n<tr><td>");
		if(strstr(buffer, "generated"))
			break;
		buffer=tbuffer;
		ret=tret;

	}
	iconv_close(icvt);
	pclose(fd);
	free(cmd);
	free(ret);
	printf("</td></tr>\n");
	printf("\t\t</table>\n\t\t<table width=600 border=0 cellpadding=8 cellspacing=0 align=center>");
	printf("\t\t<tr><td><FONT family=\"Symbol\" color=\"#FF000020\"size=5>\x29</FONT>&nbsp; Retour&nbsp;</td>"
			"</tr></table>\n</body>\n</html>\n");
	return 0;
}

