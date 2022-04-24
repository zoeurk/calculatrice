#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsearg.h"

#define ARG_COLONNE 28
#define EXPLAIN_COLONNE 58
#define SHORT_USAGE 58

#define ___ERRORMESSAGE___(stream, msg, arguments)\
fprintf(stream, msg, arguments);\
exit(EXIT_FAILURE);

#define PARSER_STATE(parser, state)\
if(!parser->state){\
state.out_stream = stdout;\
state.err_stream = stderr;\
}else{\
memcpy(&state, parser->state, sizeof(struct parser_state));\
}

int printable(int i){
	if(
		/*(i >47 && i < 58) || 
		(i > 64 && i < 91) ||
		(i > 96 && i < 124)*/
		i > 32 && i < 127

	)return 1;
	return 0;
}
void upper(int *len, int inc,int max){
	if(*len + inc > max){
		printf("\n\t");
		*len = 0;
	}
}
void defaults(FILE *out, unsigned int *colonne, char shortoption, char *longoption, char *explain){
	unsigned int j, k, l;
	if(shortoption)
		fprintf(out, " -%c, --", shortoption);
	else	fprintf(out, "     --");
	for(j = 0, k = 7; j < strlen(longoption);j++, k++){
		if(k < colonne[0]){
			fprintf(out,"%c", longoption[j]);
		}else{
			fprintf(out,"\n %c", longoption[j]);
			k = 0;
		}
	}
	l = k-2;
	for(j = 0, k = 0; j < colonne[0]-l; j++, k++){
		fprintf(out," ");
	}
	for(j = 0, k = 0; j < strlen(explain);j++, k++){
		if(k < colonne[1]){
			fprintf(out,"%c", explain[j]);
		}else{
			fprintf(out, "\n");
			for(l = 0; l < colonne[0]+2; l++)
				fprintf(out, " ");
			fprintf(out,"%c", explain[j]);
			k = 0;
		}
	}
}
void parser_usage(struct parser *parser){
	struct parser_state state;
	unsigned int colonne[2] = {ARG_COLONNE,EXPLAIN_COLONNE}, i, j, k, l, m, n, z = 0/*, c = 0, c_*/;
	char shortoption[3] = {'?', 0, 'V'},
		*option[3] = {"help", "usage", "version"},
		*explain[3] = {"show this message", "show short help", "show version"};
	memset(&state, 0, sizeof(struct parser_state));
	PARSER_STATE(parser, state);
	if(parser->doc || parser->args_doc){
		if(parser->state && parser->state->name)fprintf(state.out_stream, "Usage: %s", parser->state->name);
		if(parser->doc)fprintf(state.out_stream," %s", parser->doc);
		if(parser->args_doc)fprintf(state.out_stream, " %s", parser->args_doc);
		printf("\n");
		
	}
	/*fprintf(stderr,"==========\n");
	exit(0);*/
	if(parser->state){
		if(parser->state->arg_colonne)
			colonne[0] = parser->state->arg_colonne;
		if(parser->state->explain_colonne)
			colonne[1] = parser->state->explain_colonne;
	}
	if(parser->about)
		fprintf(state.out_stream,"%s\n\n", parser->about);
	fprintf(state.out_stream,"OPTIONS:\n");
	/*for(i = 0; options[i].longoption != 0; i++){
		if((c_ = strlen(options[i].longoption)+5) > c)
		{	c = c_;
			if(options[i].args)
				c += strlen(options[i].args);
		}
	}
	c++;*/
	/*n = 0;*/
	for(i = 0;z = 0, options[i].longoption != 0; i++){
		if(printable(options[i].shortoption) 
		)	fprintf(state.out_stream," -%c, --", options[i].shortoption);
		else	fprintf(state.out_stream,"     --");
		for(j = 0, k = 7,z = 0; j < strlen(options[i].longoption);j++, k++){
			if(k < colonne[0])
				fprintf(state.out_stream,"%c", options[i].longoption[j]);
			else{
				fprintf(state.out_stream,"\n %c", options[i].longoption[j]);
				k = 0;
				z = 1;
			}
		}
		if(options[i].args){
			if((options[i].flags&OPTION_ARG_OPTIONAL) == OPTION_ARG_OPTIONAL){
				printf("[");
				k++;
			}
			fprintf(state.out_stream,"=");
			k++;
			for(k = k, j = 0, n = 0;j < strlen(options[i].args)+((options[i].flags&OPTION_ARG_OPTIONAL) == OPTION_ARG_OPTIONAL); j++, k++, n++){
				if(k < colonne[0])
					fprintf(state.out_stream,"%c",options[i].args[j]);
				else{
					fprintf(state.out_stream,"\n %c", options[i].args[j]);
					k = 0;
				}
			}
			if((options[i].flags&OPTION_ARG_OPTIONAL) == OPTION_ARG_OPTIONAL){
				fprintf(state.out_stream,"] ");
				k++;
			}
			printf(" ");
			m = 0;
		}else	m = 1;
		if(k != 0)
			for(k = k+z;k < colonne[0] + m +1; k++)
				fprintf(state.out_stream," "); 
		for(k = 0, j = 0; j < strlen(options[i].explain); j++, k++){
			if(k < colonne[1])
				fprintf(state.out_stream,"%c", options[i].explain[j]);
			else{
				for(j = j; j < strlen(options[i].explain) && options[i].explain[j] != ' '; j++)
					fprintf(state.out_stream,"%c",options[i].explain[j]);
				fprintf(state.out_stream,"\n");
				for(l = 0; l < colonne[0]+1; l++)
					fprintf(state.out_stream," ");
				fprintf(state.out_stream,"%c", options[i].explain[j]);
				k = 0;
			}
		}
		fprintf(state.out_stream,"\n");
	}
	/*help, usage...*/
	for(i = 0; i < 3;i++){
		if(i != 2 || (i == 2 && parser->program && parser->program->program_version)){
			defaults(state.out_stream, colonne, shortoption[i], option[i], explain[i]);
			fprintf(state.out_stream,"\n");
		}
	}
	if(parser->program && parser->program->program_bug_address)
		fprintf(state.out_stream, "\nReport bug to: %s\n", parser->program->program_bug_address);
}
void parser_short_usage(struct parser *parser){
	struct parser_state state;
	int i, j, k = 0, l = SHORT_USAGE;
	PARSER_STATE(parser,state);
	if(parser->state && parser->state->explain_colonne)
		l = parser->state->short_usage;
	fprintf(state.out_stream,"Usage: %s ", state.name);
	k += 7 + strlen(state.name);
	for(i = 0, j = 0; options[i].longoption != 0; i++){
		if(j == 0 && options[i].args == NULL){
			fprintf(state.out_stream,"[-");
			j = 1;
			k += 2;
		}
		if(options[i].args == NULL){
			upper(&k, 1, l);
			fprintf(state.out_stream,"%c",options[i].shortoption);
			k++;
		}
		if(options[i+1].longoption == 0 && parser->program && parser->program->program_version){
			upper(&k, 1, l);
			fprintf(state.out_stream, "V");
			k++;
		}
	}
	if(j){
		upper(&k, 2, l);
		fprintf(state.out_stream, "?] ");
		k+=2;
	}
	for(i = 0, j = 0; options[i].longoption != 0; i++){
		if(options[i].flags&OPTION_ARG_OPTIONAL){
			upper(&k, 10+strlen(options[i].args), l);
			if(printable(options[i].shortoption))
				fprintf(state.out_stream,"[-%c [%s]] ",options[i].shortoption, options[i].args);
			k += 10+strlen(options[i].args);
		}
	}
	for(i = 0, j = 0; options[i].longoption != 0; i++){
		if((options[i].flags&OPTION_ARG_OPTIONAL) == 0 && options[i].args && options[i].shortoption){
			upper(&k, 7+strlen(options[i].args),l);
			if(printable(options[i].shortoption))
				fprintf(state.out_stream,"[-%c %s] ",options[i].shortoption, options[i].args);
			k += 7+strlen(options[i].args);
		}
	}
	for(i = 0;options[i].longoption != 0; i++){
		if(options[i].args == NULL){
			upper(&k, 6+strlen(options[i].longoption), l);
			fprintf(state.out_stream,"[--%s] ", options[i].longoption);
			k += 5+strlen(options[i].longoption);
		}
	}
	for(i = 0, j = 0; options[i].longoption != 0; i++){
		if(options[i].flags&OPTION_ARG_OPTIONAL){
			upper(&k, 10+strlen(options[i].longoption) + strlen(options[i].args), l);
			fprintf(state.out_stream,"[--%s[=%s]] ",options[i].longoption, options[i].args);
			k += 10+strlen(options[i].longoption) + strlen(options[i].args);
		}
	}
	for(i = 0, j = 0; options[i].longoption != 0; i++){
		if((options[i].flags&OPTION_ARG_OPTIONAL) == 0 && options[i].args){
			upper(&k, strlen(options[i].longoption) + strlen(options[i].args) +7, l);
			fprintf(state.out_stream,"[--%s=%s] ",options[i].longoption, options[i].args);
			k += strlen(options[i].longoption) + strlen(options[i].args) +7;
		}
	}
	upper(&k, 9, l);
	fprintf(state.out_stream,"[--help] ");
	k += 9;
	upper(&k, 10 ,l);
	fprintf(state.out_stream,"[--usage] ");
	k += 10;
	if(parser->program && parser->program->program_version){
		upper(&k, 12, l);
		fprintf(state.out_stream,"[--version] ");
		k += 12;
	}else{
		upper(&k, 10, l);
		k += 10;
	}
	if(parser->args_doc)
		fprintf(state.out_stream, "%s\n", parser->args_doc);
	else
		fprintf(state.out_stream, "\n");
}
void parser_version(struct parser *parser){
	struct parser_state state;
	PARSER_STATE(parser,state);
	fprintf(state.out_stream,"%s\n", parser->program->program_version);
}
void parser_parse(struct parser *parser, int argc, char **argv, /*unsigned int flags, unsigned int index,*/ void *input){
	struct parser_state state;
	/*struct parser p;*/
	int j, k;
	char *temporary = NULL;
	int ok = 0, end = 0;
	unsigned long int len = 0;
	memset(&state, 0, sizeof(struct parser_state));
	if(!parser->state){
		state.argv = argv;
		state.argc = argc;
		state.name = argv[0];
		state.input = input;
		state.err_stream = stderr;
		state.out_stream = stdout;
		state.parser = parser;
		parser->state = &state;
	}else{
		/*state.argv = parser->state->argv;
		state.argc = parser->state->argc;
		state.name = parser->state->name;
		state.input = parser->state->input;
		state.err_stream = parser->state->err_stream;
		state.out_stream = parser->state->out_stream;*/
		memcpy(&state, parser->state, sizeof(struct parser_state));
		if(parser->state->err_stream == NULL)
			parser->state->err_stream = stderr;
		if(parser->state->out_stream == NULL)
			parser->state->out_stream = stdout;
		if(parser->state->argv == NULL)
			parser->state->argv = argv;
		if(parser->state->argc == 0)
			parser->state->argc = argc;
		if(parser->state->name == NULL)
			parser->state->name = argv[0];
		state.parser = parser;
	}
	/*memcpy(&p,parser, sizeof(struct parser));
	p.state = &state;*/
	for(state.arg_num = 1, len = 0, temporary = NULL; ok = 0, state.arg_num < (unsigned int)state.argc; state.arg_num++){
		if(state.arg_num < (unsigned int)argc)
			state.next = state.arg_num+1;
		/*else	state.next = 0;*/
		if(strlen(state.argv[state.arg_num]) > 1 && strncmp(state.argv[state.arg_num],"--",2) == 0){
			if(end == 1){
				ok = 1;
				goto end;
			}
			if(strlen(state.argv[state.arg_num]) == 2){
				if(state.quoted == 0 && (unsigned int)state.argc > state.arg_num +1)state.quoted = state.arg_num + 1;
				ok = 1;
				end = 1;
				goto end;
			}
			for(j = 0; ok == 0 && options[j].longoption != 0; j++){
				len = ((temporary = strchr(&state.argv[state.arg_num][2], '='))) ? strlen(temporary) : 0;
				if((ok = (strncmp(options[j].longoption, &state.argv[state.arg_num][2],strlen(&state.argv[state.arg_num][2]) - len) == 0 &&
					strncmp(options[j].longoption, &state.argv[state.arg_num][2],strlen(options[j].longoption)) == 0)))
				{	
					if(options[j].args){
						if((options[j].flags&OPTION_ARG_OPTIONAL) == OPTION_ARG_OPTIONAL){
							if(temporary)
								parser->parse_opt(options[j].shortoption,++temporary, &state);
							else	if(state.arg_num+1 < (unsigned int)state.argc && *state.argv[state.arg_num+1] == '-'){
									parser->parse_opt(options[j].shortoption,state.argv[state.arg_num], &state);
								}
								else	if(state.arg_num+1 < (unsigned int)state.argc && *state.argv[state.arg_num+1] != '-'){
										parser->parse_opt(options[j].shortoption,state.argv[state.arg_num+1], &state);
									}else
										parser->parse_opt(options[j].shortoption,state.argv[state.arg_num+1], &state);
						}else{
							if(temporary)
								parser->parse_opt(options[j].shortoption,++temporary, &state);
							else{
								if(state.arg_num+1 < (unsigned int)state.argc && *state.argv[state.arg_num+1] != '-'){
									parser->parse_opt(options[j].shortoption,state.argv[state.arg_num+1], &state);
									state.arg_num++;
								}else{	___ERRORMESSAGE___(state.err_stream,
										"Argument required for \'%s\'\nTry --help or --usage for more information.\n",
										state.argv[state.arg_num]);
								}
							}
						}
					}else
						parser->parse_opt(options[j].shortoption, NULL, &state);
				}
			}
			if(ok == 0 && strncmp(state.argv[state.arg_num],"--",strlen(state.argv[state.arg_num])) != 0){
				if(strcmp(state.argv[state.arg_num],"--help") == 0){
					parser_usage(state.parser);
					exit(EXIT_SUCCESS);
				}
				if(strcmp(state.argv[state.arg_num],"--usage") == 0){
					parser_short_usage(state.parser);
					exit(EXIT_SUCCESS);
				}
				if(parser->program && parser->program->program_version && strcmp(state.argv[state.arg_num],"--version") == 0){
					parser_version(state.parser);
					exit(EXIT_SUCCESS);
				}
				___ERRORMESSAGE___(state.err_stream, "Unknow argument:\'%s\'\nTry --help or --usage for more information.\n", state.argv[state.arg_num]);
			}
		}else{	if(end == 1){
				ok = 1;
				goto end;
			}
			if(*state.argv[state.arg_num] == '-' && strlen(state.argv[state.arg_num]) >1){
				for(j = 1; ok = 0, state.argv[state.arg_num][j] != 0; j++){
					for(k = 0; ok == 0 && options[k].longoption != 0; k++){
						if((ok = (state.argv[state.arg_num][j] == options[k].shortoption))){
							if(options[k].args){
								if((options[k].flags&OPTION_ARG_OPTIONAL) == OPTION_ARG_OPTIONAL){
									if(strlen(&state.argv[state.arg_num][j]) > 1){
										parser->parse_opt(options[k].shortoption, &state.argv[state.arg_num][j+1], &state);
										j += strlen(&state.argv[state.arg_num][j+1]);
									}else
										if((unsigned int)state.argc > state.arg_num+1 && *state.argv[state.arg_num+1] != '-'){
											parser->parse_opt(options[k].shortoption, state.argv[state.arg_num+1], &state);
											state.arg_num++;
											j += strlen(&state.argv[state.arg_num][j+1]);
										}else
											parser->parse_opt(options[k].shortoption, NULL, &state);
								}else{
									if(strlen(&state.argv[state.arg_num][j]) > 1){
										parser->parse_opt(options[k].shortoption, &state.argv[state.arg_num][j+1], &state);
										j += strlen(&state.argv[state.arg_num][j+1]);
									}else{	
										if((unsigned int)state.argc > state.arg_num+1 && *state.argv[state.arg_num+1] != '-'){
											parser->parse_opt(options[k].shortoption, state.argv[state.arg_num+1], &state);
											state.arg_num++;
											j += strlen(&state.argv[state.arg_num][j+1]);
										}else{
											___ERRORMESSAGE___(state.err_stream,
												"Missing argument for :\'-%c\'\nTry --help or --usage for more inforamtion.\n",
												state.argv[state.arg_num][j]);
										}
									}
								}
							}else
								parser->parse_opt(options[k].shortoption, NULL, &state);
						}
					}
					if(ok == 0){
						if(state.argv[state.arg_num][j] == '?'){
							parser_usage(state.parser);
							exit(EXIT_SUCCESS);
						}
						if(parser->program && parser->program->program_version && state.argv[state.arg_num][j] == 'V'){
							parser_version(state.parser);
							exit(EXIT_SUCCESS);
						}
						___ERRORMESSAGE___(state.err_stream, "Unknow argument:\'-%c\'\nTry --help or --usage for mor information.\n",
							state.argv[state.arg_num][j]);
					}
				}
			}else	if(*state.argv[state.arg_num] == '\\'){
						parser->parse_opt(0, &state.argv[state.arg_num][1], &state);
					}else{
						/*if(state.arg_num > 0){
							printf("***********%s\n",state.argv[state.arg_num]);*/
							parser->parse_opt(0, state.argv[state.arg_num], &state);
						/*}*/
					}
					end:;
		}
	}
}

