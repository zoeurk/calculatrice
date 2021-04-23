#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsearg.h"

#define BUFFER 4096
#define PI 3.141592653589793

#define MAILLON(pv, size)\
pv->next = ___calloc___((void **)&pv->next,sizeof(struct value) + valsize);\
pv->next->prev = pv;\
pv = pv->next;\

#define INIT_BUFSET(pv,buffer,end, ___type___) \
pv->val = (void *)pv + sizeof(struct value);\
switch(___type___){\
case FLOAT:\
*((float *)pv->val) = strtof(buffer,&end);\
break;\
case DOUBLE:\
*((double *)pv->val) = strtod(buffer,&end);\
break;\
case LDOUBLE:\
*((long double  *)pv->val) = strtold(buffer,&end);\
break;\
}

#define ERROR(msg,arg)\
fprintf(stderr,msg,arg);\
exit(EXIT_FAILURE);

#define _ERROR_(msg)\
fprintf(stderr,msg);

#define BUFSET(v, pv,valsize, buffer, end,___type___)\
if(bufset){ \
	if(v == NULL)\
	v = pv = ___calloc___((void **)&v,sizeof(struct value)+valsize);\
	else{\
		MAILLON(pv, sizeof(value)+valsize);\
	}\
	INIT_BUFSET(pv,buffer,end, ___type___);\
	pv->type = VALUE;\
	memset(buffer, 0, BUFFER);\
	bufset = 0;\
}\
if(pv == NULL)\
	v = pv = ___calloc___((void **)&v,sizeof(struct value)+valsize);\
MAILLON(pv, sizeof(struct value));

static void calcule_float(void *val1, void *val2, char op);
static void calcule_double(void *val1, void *val2, char op);
static void calcule_ldouble(void *val1, void *val2, char op);
static void print_float(void *val, char *format);
static void print_double(void *val, char *format);
static void print_ldouble(void *val, char *format);

struct value{
	int value;
	int type;
	union {
		union{
			int operator;
			int count;
		};
		int	___;
		void 	*val;
	};
	struct value *next;
	struct value *prev;
};
enum VALUE{
	FLOAT = 1,
	DOUBLE = 2,
	LDOUBLE = 4,
};
enum VALUE_TYPE{
	VALUE = 1,
	OPERATOR = 2,
	O_PARENTHESE = 4,
	C_PARENTHESE = 8
};
enum OPERATOR{
	ADD = 1,
	LESS = 2,
	MULT = 4,
	DIV = 8
};
enum TYPE{
	I_DEGREE = 1,
	O_DEGREE = 2,
	I_RADIAN = 4,
	O_RADIAN = 8,
	FORMAT   = 16,
	U_PI	 = 32,
	V_PI	 = 64
};
struct arguments{
	struct value *v;
	unsigned long  int type;
	unsigned long int options;
	unsigned long int valsize;
	void (*calcule)(void *, void *, char);
	void (*print)(void *, char *);
	char *format;
	void *pi;
	char *argv;
};
void arguments(int key, char *arg, struct parser_state *state){
	struct arguments *a = state->input;
	unsigned long int i;
	switch(key){
		case 'd':
			a->options -= ((a->options&I_RADIAN) == I_RADIAN)? I_RADIAN :0;
			a->options |= I_DEGREE;
			break;
		case 'D':
			a->options -= ((a->options&O_RADIAN) == O_RADIAN)? O_RADIAN :0;
			a->options |= O_DEGREE;
			break;
		case 'r':
			a->options -= ((a->options&I_DEGREE) == I_DEGREE)? I_DEGREE :0;
			a->options |= I_RADIAN;
			break;
		case 'R':
			a->options -= ((a->options&O_DEGREE) == O_DEGREE)? O_DEGREE :0;
			a->options |= O_RADIAN;
			break;
		case 'O':
			for(i = 0; i < strlen(arg); i++){
				if(arg[i] < 48 || arg[i] > 57){
					fprintf(stderr, "Bad value for format: \'%s'.\n",arg);
					exit(EXIT_FAILURE);
				}
			}
			a->options |= FORMAT;
			a->format = arg;
			break;
		case 'f':
			a->print = print_float;
			a->calcule = calcule_float;
			a->valsize = sizeof(float);
			a->type = FLOAT;
			break;
		case 'l':
			a->print = print_double;
			a->calcule = calcule_double;
			a->valsize = sizeof(double);
			a->type = DOUBLE;
			break;
		case 'L':
			a->print = print_ldouble;
			a->calcule = calcule_ldouble;
			a->valsize = sizeof(long double);
			a->type = LDOUBLE;
			break;
		case 'p':
			a->pi = arg;
			a->options |= U_PI;
			break;
		case 'P':
			a->options |= V_PI;
			break;
		default:
			a->argv = arg;
			break;
	}
}
struct info program = {"version: 1.0","zoeurk@gmail.com"};
struct parser_option options[] =	{
					{ "float", 'f', 0, NULL, "Utiliser le type float"},
					{ "double", 'l', 0 , NULL, "Utiliser le type double"},
					{ "ldouble", 'L', 0, NULL, "Utiliser le type long double"},
					{ "format", 'O', 0, "FORMAT", "nombre de chiffre apres la virgule"},
					{ "in_degree", 'd', 0, NULL, "les entrees sont en degre"},
					{ "out_degree", 'D', 0, NULL, "la sortie est en degre"},
					{ "in_radian", 'r', 0,NULL, "les entrees sont en radian"},
					{ "out_radian", 'R', 0, NULL, "la sortie est en radian"},
					{ "set-pi", 'p', 0, "PI", "utiliser pi"},
					{ "view-pi", 'P', 0, NULL, "voir pi"},
					{0}
				};
struct parser args = {options, arguments, "[OPTIONS]", "calcule", "Petit outil de calcule", &program, NULL};

void *___calloc___(void **ptr, unsigned long int size){
	if((*ptr = calloc(1, size)) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	return *ptr;
}
struct value *initialisation(char *argv, struct arguments *arg){
	struct value *v = NULL, *pv = NULL;
	char buffer[BUFFER], *end;
	unsigned long int valsize = arg->valsize;
	int i, point = 0, o_parentheses = 0, c_parentheses = 0, bufset = 0, count = 0;
	memset(buffer, 0, BUFFER);
	for(i = 0; argv[i] != 0; i++){
		switch(argv[i])
		{	
			case ' ': case '\t':case '\n':
				continue;
			case '(':
				count++;
				if(i > 0 &&
					argv[i-1] != '+' && 
					argv[i-1] != '/' && 
					argv[i-1] != '*' && 
					argv[i-1] != '+' && 
					argv[i-1] != '-' && 
					argv[i-1] != '(' )
				{
					ERROR("Erreur de syntaxe vers l'offset %i\n", i+1);
				}
				if(v == NULL)
					v = pv = ___calloc___((void *)&v,sizeof(struct value));
				else{
					MAILLON(pv, sizeof(struct value));
				}
				pv->type = O_PARENTHESE;
				pv->count = count;
				o_parentheses++;
				break;
			case ')':
				if((i == 0) || ((i > 1 && (argv[i-1] < 48 || argv[i-1] >57)) && argv[i] != ')')){
					fprintf(stderr, "Erreur de syntaxe vers l'offset %i\n", i+1);
					exit(EXIT_FAILURE);
				}
				if(o_parentheses < c_parentheses +1){
					ERROR("Trop de parentheses fermees a l'offset %i\n", i);
				}
				if(bufset){
					if(v == NULL)
						v = pv = ___calloc___((void **)&v,sizeof(struct value)+arg->valsize);
					else{
						MAILLON(pv,sizeof(struct value) + arg->valsize)
					}
					INIT_BUFSET(pv,buffer,end,arg->type);
					pv->type = VALUE;
					memset(buffer, 0, BUFFER);
					bufset = 0;
				}
				MAILLON(pv,sizeof(struct value));
				pv->type = C_PARENTHESE;
				c_parentheses++;
				break;
			case '+':
			case '-':
				if(i == 0 && argv[i+1] != '(')
					goto number;
				else{	if(i == 0 && argv[i+1] == '('){
						if(argv[i] == '-')
							strcpy(buffer,"-1");
						else	strcpy(buffer,"+1");
						bufset = 1;
						BUFSET(v, pv,arg->valsize, buffer, end, arg->type);
						pv->type = OPERATOR;
						pv->operator = '*';
						break;
					}else{
						if(i > 0 && argv[i-1] == '+' && argv[i-1] == '-' && argv[i-1] == '*' && argv[i-1] == '/'){
							goto number;
						}
					}
					if(i > 0 && (argv[i-1] == '+' || argv[i-1] == '-' || argv[i-1] == '*' || argv[i-1] == '/' || argv[i-1] == '('))
						goto number;
				}
			case '*':
			case '/':
				if(i == 0){
					ERROR("Erreur de syntaxe vers l'offset %i\n",i +1);
				}
				if(i > 1 && (argv[i-1] < 48 || argv[i-1] >57) && argv[i-1] != ')')
				{
					ERROR("Erreur de syntaxe vers l'offset %i\n",i +1);
				}
				BUFSET(v, pv, arg->valsize, buffer, end, arg->type);
				pv->type = OPERATOR;
				pv->operator = argv[i];
				break;
			case '.':
				if(point == 1){
					ERROR("Erreur de syntaxe vers l'offset %i\n", i+1);
				}else	point = 1;
			default:
				number:
				if((argv[i] > 47 && argv[i] < 58) || argv[i] == '.' || argv[i] == '-' || argv[i] == '+')
					strncat(buffer,&argv[i],1);
				else{	ERROR("Erreur de syntaxe vers l'offset %i\n", i+1);
				}
				bufset = 1;
				break;
		}
	}
	if(pv && bufset == 1){
		MAILLON(pv,sizeof(struct value) + arg->valsize);
		INIT_BUFSET(pv,buffer,end, arg->type);
		pv->type = VALUE;
	}else{
		if(v == NULL){
			v = pv = ___calloc___((void **)&v,sizeof(struct value)+arg->valsize);
		}
		INIT_BUFSET(pv,buffer,end,arg->type);
		pv->type = VALUE;
	}
	if(c_parentheses > o_parentheses){
		_ERROR_("Trop de parentheses fermees\n");
	}
	if(c_parentheses < o_parentheses){
		_ERROR_("Trop de parentheses ouvertes\n");
		exit(EXIT_FAILURE);
	}
	return v;
}
struct value *calcule(struct value *v, void (*calcule)(void *, void *, char)){
	struct value *pv = NULL, *ppv, *in_use[2] = {0,0}, *pnext, *pprev, *poperator;
	char op = 0;
	int o_parentheses = 0, bufset = 0, count = 0;
	pv = v;
	o_parentheses = 0;
	while(pv){
		switch(pv->type){
			case VALUE:	
					if(in_use[0] == NULL){
						in_use[0] = pv;
					}else{	if(in_use[1] == NULL)
							in_use[1] = pv;
					}
					if(bufset == 1 && in_use[0] && in_use[1]){
						calcule(in_use[0]->val, in_use[1]->val, op);
						if(pv->prev)
							pprev = pv->prev->prev;
						pnext = pv->next;
						pprev->next = pnext;
						if(pprev->next)
							pprev->next->prev = pprev;
						pv = v;
						free(in_use[1]->prev);
						free(in_use[1]);
						in_use[1] = NULL;
						in_use[0] = NULL;
						bufset = 0;
						continue;
					}
					break;
			case OPERATOR:
					if(pv->next && pv->next->type == O_PARENTHESE){
						in_use[0] = NULL;
						in_use[1] = NULL;
						pv = pv->next;
						goto O_PARENTHESE;
					}
					switch(pv->operator){
						case '+': case '-':
							ppv = pv;
							while(ppv &&
								ppv->operator != '*' && ppv->operator != '/' &&
								ppv->type != O_PARENTHESE && ppv->type != C_PARENTHESE
							){
								ppv = ppv->next;
							}
							if(ppv == NULL){
								bufset = 1;
								op = pv->operator;
							}else{
								if(ppv->operator == '*' || ppv->operator == '/'){
									pv = ppv->prev;
									in_use[0] = pv;
								}else{
									if(ppv->type == O_PARENTHESE){
										pv = ppv;
										goto O_PARENTHESE;
									}else	if(ppv->type == C_PARENTHESE){
											bufset = 1;
											op = pv->operator;
										}
								}
							}
							break;
						case '*': case '/':
							op = pv->operator;
							bufset = 1;
							break;
					}
					break;
			case C_PARENTHESE:
					if(pv->type == C_PARENTHESE){
						in_use[0] = in_use[1] = NULL;
						count = 0;
						ppv = pv;
						while(ppv && ppv->type != O_PARENTHESE){
							ppv = ppv->prev;
							count++;
						}
						if(ppv && count == 2){
							if(ppv->prev == NULL){
								v = pv = ppv->next;
								pnext = pv->next;
								pv->next = ppv->next->next->next;
								if(pv->next)
									pv->next->prev = pv;
								free(pnext);
								free(pv->prev);
								pv->prev = NULL;
								pv = v;
								continue;
							}else{	if(ppv->next){
									pnext = v;
									pprev = ppv->prev;
									pnext = ppv->next;
									free(ppv);
									pprev->next = pnext;
									pprev->next->prev = pprev;
									if(pnext->next->next){
										poperator = pnext->next->next;
										pnext->next = poperator;
										free(poperator->prev);
										pnext->next->prev = pnext;
									}else{	free(pnext->next);
										pnext->next = NULL;
									}
									pv = v;
									continue;
								}
							}
						}
					}
					o_parentheses--;
					break;
			case O_PARENTHESE:
					O_PARENTHESE:
					o_parentheses++;
					break;
		}
		pv = pv->next;
	}
	return v;
}
void calcule_destroy(struct value *v, void (*print)(void *, char *), char *format){
	struct value *pv = v;
	unsigned long int count = 0;
	while(pv){
		v = pv->next;
		if(pv->type == O_PARENTHESE)
			printf("(");
		if(pv->type == VALUE)
			print(pv->val, format);
		if(pv->type == OPERATOR)
			printf("%c",pv->operator);
		if(pv->type == C_PARENTHESE)
			printf(")");
		free(pv);
		pv = v;
		count++;
	}
	if(count > 1){
		fprintf(stderr,"Please Report a bug.\n");
		exit(EXIT_FAILURE);
	}
}
int main(int argc, char **argv){
	long double ldpi = (long double) PI;
	double dpi = (double) PI;
	float fpi = (float) PI;
	struct arguments arg = {NULL, FLOAT, I_DEGREE|O_DEGREE, sizeof(float), &calcule_float, &print_float, "%Ld", NULL, NULL};
	/*struct value *v;*/
	char format[8], *r;
	parser_parse(&args, argc, argv, &arg);
	if((arg.options&FORMAT) == FORMAT){
		*format = 0;
		strcat(format,"%.");
		strcat(format,arg.format);
		switch(arg.type){
			case FLOAT:
				strcat(format,"f");
				break;
			case DOUBLE:
				strcat(format,"lf");
				break;
			case LDOUBLE:
				strcat(format,"Lf");
				break;
		}
		arg.format = format;
	}else{
		*format = 0;
		switch(arg.type){
			case FLOAT:
				strcat(format,"%f");
				break;
			case DOUBLE:
				strcat(format,"%lf");
				break;
			case LDOUBLE:
				strcat(format,"%Lf");
				break;
		}
		arg.format = format;
	}
	if((arg.options&U_PI) == U_PI){
		switch(arg.type){
			case FLOAT:
				fpi = strtof(arg.pi, &r);
				arg.pi = &fpi;
				break;
			case DOUBLE:
				dpi = strtod(arg.pi, &r);
				arg.pi = &fpi;
				break;
			case LDOUBLE:
				ldpi = strtold(arg.pi, &r);
				arg.pi = &fpi;
				break;
		}
	}
	if((arg.options&V_PI) == V_PI){
		switch(arg.type){
			case FLOAT:
				fpi = *((float *)arg.pi);
				printf(arg.format,*((float *)arg.pi));
				printf("\n");
				break;
			case DOUBLE:
				arg.pi = &dpi;
				printf(arg.format,*((double *)arg.pi));
				printf("\n");
				break;
			case LDOUBLE:
				arg.pi = &ldpi;
				printf(arg.format,*((long double *)arg.pi));
				printf("\n");
				break;
		}
	}
	if(arg.argv == NULL){
		exit(EXIT_SUCCESS);
	}
	arg.v = initialisation(arg.argv, &arg);
	if(arg.v){
		arg.v = calcule(arg.v, arg.calcule);
		/*conversion en degree*/
		if((arg.options&I_RADIAN) == I_RADIAN && (arg.options&O_DEGREE) == O_DEGREE){
			switch(arg.type){
				case FLOAT:
					*((float *)arg.v->val)  = *((float *)arg.v->val)* PI/180;
					break;
				case DOUBLE:
					*((double *)arg.v->val)  = *((double *)arg.v->val)* PI/180;
					break;
				case LDOUBLE:
					*((long double *)arg.v->val)  = *((long double *)arg.v->val)* PI/180;
					break;
			}
		}else{
			/*conversion en radian*/
			if((arg.options&O_RADIAN) == O_RADIAN && (arg.options&I_DEGREE) == I_DEGREE){
				switch(arg.type){
					case FLOAT:
						*((float *)arg.v->val)  = *((float *)arg.v->val)*180/ PI;
						break;
					case DOUBLE:
						*((double *)arg.v->val)  = *((double *)arg.v->val)*180/ PI;
						break;
					case LDOUBLE:
						*((long double *)arg.v->val)  = *((long double *)arg.v->val)*180/ PI;
						break;
				}
			}
		}
		calcule_destroy(arg.v, arg.print, arg.format);
	}
	printf("\n");
	return 0;
}
void calcule_float(void *val1, void *val2, char op){
	switch(op){
		case '+':
			*((float *)val1) += *((float *)val2);
			break;
		case '-':
			*((float *)val1) -= *((float *)val2);
			break;
		case '*':
			*((float *)val1) *= *((float *)val2);
			break;
		case '/':
			*((float *)val1) /= *((float *)val2);
			break;
	}
}
void calcule_double(void *val1, void *val2, char op){
	switch(op){
		case '+':
			*((double *)val1) += *((double *)val2);
			break;
		case '-':
			*((double *)val1) -= *((double *)val2);
			break;
		case '*':
			*((double *)val1) *= *((double *)val2);
			break;
		case '/':
			*((double *)val1) /= *((double *)val2);
			break;
	}
}
void calcule_ldouble(void *val1, void *val2, char op){
	switch(op){
		case '+':
			*((long double *)val1) += *((long double *)val2);
			break;
		case '-':
			*((long double *)val1) -= *((long double *)val2);
			break;
		case '*':
			*((long double *)val1) *= *((long double *)val2);
			break;
		case '/':
			*((long double *)val1) /= *((long double *)val2);
			break;
	}
}
void print_float(void *val, char * format){
	printf(format, *((float *)val));
}
void print_double(void *val, char *format){
	printf(format, *((double *)val));
}
void print_ldouble(void *val, char *format){
	printf(format, *((long double *)val));
}
