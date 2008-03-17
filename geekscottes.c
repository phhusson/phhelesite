#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include "ezxml.h"

extern void parse_args(char **envp);
extern char **args_w_opt,**opts,**args;
extern int args_ln,arg_w_opt_ln;

//NULL terimnated
char **urls;

int main(int argc, char **argv, char **envp) {
	printf("Content-type: text/html\n\n");
	parse_args(envp);
	int pos=0;
	if(args_ln>=1)
		pos=atoi(args[0]);
	FILE *fd=popen("curl -q http://www.nojhan.net/geekscottes/rss.php  2>/dev/null |grep -oE \"http://www.nojhan.net/(.*).png\" |grep -v favicon", "r");
	int nb_urls=0;
	while(!feof(fd)) {
		urls=realloc(urls,sizeof(*urls)*(nb_urls+1));
		urls[nb_urls]=malloc(128);
		fgets(urls[nb_urls], 128, fd);
		urls[nb_urls]=rindex(urls[nb_urls],'/')+1;
		if(urls[nb_urls]>1) {
			urls[nb_urls][strlen(urls[nb_urls])-1]=0;
			char *tmp=rindex(urls[nb_urls], '.');
			if(tmp)
				tmp[0]=0;
			nb_urls++;
		}
	}
	if(pos<0 || pos >nb_urls)
		pos=0;


	pclose(fd);
	printf("<html>\n<head><meta name=\"info_page\" content=\"index.html\">\n<link rel=\"red\" href=\"index.html\">\n<link rel=\"blue\" href=\"geekscottes.cgi?%d\">\n",pos+1);
	printf("<link rel=\"green\" href=\"geekscottes.cgi?%d\">\n</head>\n", pos-1);
	printf("\t<body>\n\t\t<table width=600 border=0 cellpadding=8 cellspacing=0 align=center>\n");
	int i=0;
	{
		int cur=nb_urls-(i+pos)-1;
		char *file;
		asprintf(&file, "%s.gif", urls[cur]);
		struct stat buf;
		if(stat(file, &buf)==-1) {
			char *cmd;
			asprintf(&cmd, "wget -q http://www.nojhan.net/geekscottes/./strips/%s.png && convert -resize x240 %s.png %s.gif", urls[cur], urls[cur], urls[cur]);
			system(cmd);
		}
		printf("\t\t\t<tr><td><img src=\"%s.gif\" alt=\"%d\"/></td></tr>\n", urls[cur], cur);
	}
	printf("\t\t</table>\n\t\t<table width=600 border=0 cellpadding=8 cellspacing=0 align=center>\n");
	printf("\t\t<tr>\n\t\t\t<td><FONT family=\"Symbol\" color=\"#FF000020\"size=5>\x29</FONT>&nbsp; Sortie&nbsp;</td>\n\t\t\t<td><FONT family=\"Symbol\" color=\"#0000FF20\"size=5>\x29</FONT>&nbsp; Page suivante</td>\n\t\t\t<td>"
			"<FONT family=\"Symbol\" color=\"#00FF0020\"size=5>\x29</FONT>&nbsp; Page precedente</td>\n\t\t</tr>\n\t</table>\n</body>\n</html>\n");
	return 0;
}
