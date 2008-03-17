#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>

char **args_w_opt,**opts,**args;
int args_ln,arg_w_opt_ln;

void parse_args( char **envp) {
	char *argument;
	char *type;
	//There are two ways to send arguments; POST or GET.
	//The REQUEST_METHOD, in standard CGI call (for instance with apache), variable
	//is either GET or POST to know which one is used.
	type=getenv("REQUEST_METHOD");
	if(type && strcmp(type, "POST")==0) {
		return;
		//If it's post, then arguments are sent to stdin
		//std::string read_buffer;
		//std::cin >> read_buffer;
		//argument=(char*)read_buffer.c_str();
	} else {
		//If it's get, then arguments are put in the QUERY_STRING variable
		
		argument=getenv("QUERY_STRING");
		if(!argument)
			return;
		argument=strdup(argument);
	}
	//Now the argument is of the form var=arg&var2 where & can be either & or &amp;
	char *tmp;
	char *delimiter;
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
			args_w_opt=(char**)realloc((void*)args_w_opt, (arg_w_opt_ln+1)*sizeof(char*));
			args_w_opt[arg_w_opt_ln]=opt;
			opts=(char**)realloc((void*)opts, (arg_w_opt_ln+1)*sizeof(char*));
			opts[arg_w_opt_ln]=arg;
			arg_w_opt_ln++;
		} else {
			args=(char**)realloc((void*)args, (args_ln+1)*sizeof(char *));
			args[args_ln]=tmp;
			args_ln++;
		}
	}
}
