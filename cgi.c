/*
 * Tool made by Pierre-Hugues HUSSON <phhusson@free.fr>
 * for Alexis Robert <alexis@robertlan.eu.org>
 *
 * CGI Script which send XML requests to 127.0.0.1:15000 (you can change it yourself)
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 1500

int main(int argc, char **argv, char **envp) {
	printf("Content-type: text/html\n\n");
	char *argument;
	char *type;
	//There are two ways to send arguments; POST or GET.
	//The REQUEST_METHOD, in standard CGI call (for instance with apache), variable
	//is either GET or POST to know which one is used.
	type=getenv("REQUEST_METHOD");
	if(strcmp(type, "POST")==0) {
		//If it's post, then arguments are sent to stdin
		std::string read_buffer;
		std::cin >> read_buffer;
		argument=(char*)read_buffer.c_str();
	} else {
		//If it's get, then arguments are put in the QUERY_STRING variable
		argument=strdup(getenv("QUERY_STRING"));
	}
	//Now the argument is of the form var=arg&var2 where & can be either & or &amp;
	char *tmp;
	char *delimiter;
	int arg_w_opts_ln=0,args_ln=0;
	char **args_w_opt=NULL,**opts=NULL,**args=NULL;
	//The separator is & or &amp; ?
	delimiter=(strstr((const char*)argument, "&amp;")!=NULL) ? (char*)"&amp;": (char*)"&";
	//This hardly readable code is to put the variable and arguments in an array
	while((tmp=strtok(argument, "&"))!=NULL) {
		if(argument)
			argument=NULL;
		if(index(tmp, '=')!=NULL)  {
			char *opt,*arg;
			opt=tmp;
			arg=index(tmp, '=');
			arg[0]='\0';
			arg++;
			args_w_opt=(char**)realloc((void*)args_w_opt, (arg_w_opts_ln+1)*sizeof(char*));
			args_w_opt[arg_w_opts_ln]=opt;
			opts=(char**)realloc((void*)opts, (arg_w_opts_ln+1)*sizeof(char*));
			opts[arg_w_opts_ln]=arg;
			arg_w_opts_ln++;
		} else {
			args=(char**)realloc((void*)args, (args_ln+1)*sizeof(char *));
			args[args_ln]=tmp;
			args_ln++;
		}
	}
	int i;
	for(i=0;i<args_ln;i++) 
			fprintf(stdout, "<%s type='%s'/>\n", args[i],type);
	for(i=0;i<arg_w_opts_ln;i++)
			fprintf(stdout, "<%s type='%s'>%s</%s>\n", args_w_opt[i],type,opts[i],args_w_opt[i]);
	return 0;
}
