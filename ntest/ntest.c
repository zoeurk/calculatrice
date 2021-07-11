#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/parsearg.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#include <time.h>

#include <sys/mman.h>

enum TYPE{
	FLOAT,
	DOUBLE,
	LDOUBLE
};
enum OBJ{
	STRING = 1,
	NUMBER = 0
};
enum OPERATOR{
	RETOUR = 0,
	INVERT = 1,
	AND = 2,
	OR = 4,
	O_PARENTHESE = 16,
	C_PARENTHESE = 32
};
enum EXPRESSION{
	EQUAL = 0,
	DIFF = 1,
	E_GREATER = 2,
	E_LESS = 3,
	GREATER = 4,
	LESS = 5,
	STRINGS_EQ = 6,
	STRINGS_DIFF = 7,
	STR = 8,
	NOT_STR = 9,
	NUM = 10,
	NOT_NUM = 11 
};

struct fn{
	int (*diff)(void *, void *);
	int (*equal)(void *, void*);
	int (*e_less)(void*, void *);
	int (*less)(void *, void *);
	int (*e_greater)(void *, void *);
	int (*greater)(void *, void *);
	int (*strings)(void *);
	int (*strings_eq)(void *, void *);
	int (*strings_diff)(void *, void *);
};

static int f_equal(void *num1, void *num2);
static int f_diff(void *num1, void *num2);
static int f_e_less(void *num1, void *num2);
static int f_less(void *num1, void *num2);
static int f_e_greater(void *num1, void *num2);
static int f_greater(void *num1, void *num2);

static int d_equal(void *num1, void *num2);
static int d_diff(void *num1, void *num2);
static int d_e_less(void *num1, void *num2);
static int d_less(void *num1, void *num2);
static int d_e_greater(void *num1, void *num2);
static int d_greater(void *num1, void *num2);

static int ld_equal(void *num1, void *num2);
static int ld_diff(void *num1, void *num2);
static int ld_e_less(void *num1, void *num2);
static int ld_less(void *num1, void *num2);
static int ld_e_greater(void *num1, void *num2);
static int ld_greater(void *num1, void *num2);

static int strings(void *str);
static int strings_eq(void *str1, void *str2);
static int strings_diff(void *str1, void *str2);

struct info program = {"version: 1.0","zoeurk@gmail.com"};
struct parser_option options[] =	{
					{ "float", 'f', 0, NULL, "Utiliser le type float"},
					{ "double", 'd', 0 , NULL, "Utiliser le type double"},
					{ "ldouble", 'D', 0, NULL, "Utiliser le type long double"},
					{ "file", 'F', 0, "FILE", "Traiter le fichier"},
					{0}
				};
struct objet{
	int type, size;
	char *var1, *var2;
};
struct retour{
	int operator;
	int ret;
	struct retour *next;
	struct retour *prev;
};
struct arguments{
	unsigned long int nbrtype;
	char *string;
	void *mmap;
	int fd;
	int pad;
};

void *___calloc___(void **ptr, unsigned long int size){
	if((*ptr = calloc(1, size)) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	return *ptr;
}

void *var1 = NULL;
struct retour *ret = NULL;
char file[28];
#define ERROR(msg, offset)\
	fprintf(stderr, msg, offset);\
	exit(EXIT_FAILURE);\

#define ___CONVERT___(type, fonction)\
	if(o.var1){\
		*((type *)var1) = fonction(o.var1, &str);\
		if(str && strlen(str) > 0){\
			fprintf(stderr, "WARNING: Mauvais caractere detectee: <%s>.\n", str);\
		}\
	}\
	if(o.var2){\
		*((type *)var2) = fonction(o.var2, &str);\
		if(str && strlen(str) > 0){\
			fprintf(stderr, "WARNING: Mauvais caractere detectee: <%s>.\n", str);\
		}\
		*((type *)var2) = fonction(o.var2, &str);\
	}

#define CONVERT(type)\
	switch(type){\
		case FLOAT:\
			___CONVERT___(float, strtof)\
			break;\
		case DOUBLE:\
			___CONVERT___(double, strtof)\
			break;\
		case LDOUBLE:\
			___CONVERT___(long double, strtof)\
			break;\
	}

#define ALLOC\
	if(ret == NULL){\
		pret = ret = ___calloc___((void **)&ret, sizeof(struct retour));\
	}else{\
		pret->next = ___calloc___((void **)&pret->next,sizeof(struct retour));\
		pret->next->prev = pret;\
		pret = pret->next;\
	}

#define QUOTE(q, ch, var)\
	o.type = STRING;\
	q = !q;\
	if(q == 1){\
		/*printf("***\n");*/\
		___quote___ = offset+4;\
		*r = 0;\
		r++;\
		offset++;\
		var = r;\
		while(*r != 0 && *r != ch && *r != '~' && *r != '&' && *r != '|' && *r != ')'){\
			if(*r == '\\'){\
				r++;\
				offset++;\
				___quote___++;\
			}\
			offset++;\
			___quote___++;\
			r++;\
		}\
		if(*r == ch)\
			q = !q;\
		if(q){\
			printf("Quote(/Double quote nom fermee vers l'offset: %lu\n", ___quote___);\
			exit(EXIT_FAILURE);\
		}\
		*r = 0;\
	}

#define REMOVE_SPACE\
	while(*r == ' ' || *r == '\t' || *r == '\n'){\
		r++;\
		offset++;\
	}

#define NUMBERS(inc, fn)\
	ALLOC;\
	REMOVE_SPACE;\
	for(j = 0,r = r; *r = 0, j < inc; j++, r++);;\
	r++;\
	offset += (inc+1);\
	REMOVE_SPACE;\
	o.var2 = r;\
	while(((*r >47 && *r < 58) || *r == '-' || *r =='.') && *r != 0){\
		r++;\
		offset++;\
	}\
	/*if(*r == 0){\
		fprintf(stderr, "=>Erreur de syntaxe vers l'offset: %lu\n", offset);\
		exit(EXIT_FAILURE);\
	}*/ \
	if((*r < 48 || *r > 57) && *r != ' ' && *r != '\t' && *r != '\n' && *r != ')' && *r !=0 ){\
		*(r+1) = 0;\
		printf("Numerique attendu chaine de caracter declaree: '%s'.\n", o.var2);\
		exit(EXIT_FAILURE);\
	} \
	if(*r == ')'){\
		*r = 0;\
		CONVERT(type);\
		pret->ret = fn(var1,var2);\
		goto PARENTHESE;\
	}\
	/*printf("%s,%s\n",o.var1, o.var2);*/\
	*r = 0; \
	if(o.var1 != NULL && strlen(o.var1) > 0 && o.var2 != NULL && strlen(o.var2) > 0){\
		CONVERT(type);\
	/*};*/\
		pret->ret = fn(var1,var2);\
	}else{\
		pret->ret = 0;\
	}\
	o.var1 = o.var2 = NULL;\
	o.type = 0;

#define STRING_DEF\
	if(o.type == STRING){\
		*r = 0;\
		printf("Numerique attendu chaine de caracter declaree: '%s'\n", o.var1);\
		exit(EXIT_FAILURE);\
	}

#define ___QUOTE___(var, on, end, init)\
	if(*r == '\''){\
		var;\
		___quote___ = offset;\
		QUOTE(quote,'\'', on);\
		*r = 0;\
	}else{\
		if(*r == '"'){\
			var;\
			___quote___ = offset;\
			QUOTE(quote,'"',on);\
			*r = 0;\
		}else{\
			end;\
			init;\
			while(*r != ' ' && *r != '~' && *r != 0 && *r != ')' && *r != '|' && *r != '&'){\
				r++;\
				offset++;\
				___quote___++;\
			}\
			*r = 0;\
		}\
	}\

#define READ_STRING(fn)\
	REMOVE_SPACE;\
	___QUOTE___(r = r, o.var1, end = end, o.var1 = o.var1);\
	r += 2;\
	REMOVE_SPACE;\
	o.var2 = r;\
	/*printf("%s\n", o.var2);*/\
	___QUOTE___(o.var2 = r+1,o.var2, end = 0,o.var2 = r);\
	/*printf("%s\n", o.var2);\
	str = &o.var2[strlen(o.var2)-1];\
	while(*str == ' ' || *str == '\t' || *str == '\n'){\
		*str= 0;\
		str--;\
	}*/\
	REMOVE_SPACE; \
	r++;\
	offset++;\
	ALLOC;\
	pret->ret = fn(o.var1, o.var2);\
	if(o.var2[strlen(o.var2)+1] == ')'){\
		parentheses--;\
		ALLOC;\
		pret->ret = -1;\
		pret->operator = C_PARENTHESE;\
	}\
	o.var1 = o.var2 = NULL;\
	o.type = NUMBER;

#define STRING_EXIST(ret1, ret2)\
	r += 2;\
	offset += 2;\
	REMOVE_SPACE;\
	ALLOC;\
	o.var1 = r;\
	str = NULL;\
	if(*r == ')' || *r == '&' || *r == '|'){\
		r--;\
		offset--;\
	}else{\
		str = r;\
		while(*(r+1) != ')' && *(r+1) != '&' && *(r+1) != '|' && *(r+1) != 0){\
			r++;\
			offset++;\
		}\
		str = r;\
		while(*str == '\t' || *str == ' ' || *str == '\n'){\
			*str = 0;\
			str--;\
		}\
	}\
	if(str && *str){\
		pret->ret = ret1;\
	}else	pret->ret = ret2; \

#define NUMERIQUE(ret1, ret2)\
	switch(pret->ret){\
		case 0:\
			break;\
		case 1:\
			str = o.var1;\
			_end_ = 0;\
			while(*str != 0){\
				if((*str < 48 || *str > 57) && *str != '.' && *str != '-' && *str != 0){\
					_end_ = 1;\
					break;\
				}\
				str++;\
			}\
			if(!_end_)\
				pret->ret = ret1;\
			else\
				pret->ret = ret2;\
			break;\
	}

#define INVERTION\
	while(pret->next->operator == INVERT){\
		pprev = pret;\
		pnext = pret->next->next;\
		free(pret->next);\
		pprev->next = pnext;\
		pprev->next->prev = pprev;\
		pret = pprev;\
		invert = !invert;\
	}

#define CALCULE(calcule, init)\
	if(pret->prev->ret == init){\
		pnext = pret->next;\
		pprev = pret->prev;\
		pnext = pret->next;\
		pprev->next = pnext;\
		pprev->next->prev = pprev;\
		if(pnext->operator == 1){\
			pop = pnext;\
			pprev = pnext->prev;\
			pnext = pnext->next;\
			pprev->next = pnext;\
			pprev->next->prev = pprev;\
			free(pop);\
		}\
		free(pret);\
		pret = pnext;\
		do{\
			if(pret->operator == O_PARENTHESE)\
				par++;\
			if(pret->operator ==C_PARENTHESE)\
				par--;\
			pprev = pret->prev;\
			pnext = pret->next;\
			pprev->next = pnext;\
			free(pret);\
			if(pnext)\
				pprev->next->prev = pprev;\
			pret = pnext;\
		}while(par != 0);\
		pret = ret;\
	}else{\
		INVERTION;\
		if(pret->next->ret == -1){\
			pprev = pret;\
			pnext = pret->next->next;\
			free(pret->next);\
			pprev->next = pnext;\
			if(pnext)\
				pprev->next->prev = pprev;\
			pret = comput(&pprev);\
			if(invert){\
				pret->ret = !pret->ret;\
				invert = 0;\
			}\
			pret = ret;\
		}else{\
			calcule;\
			if(invert){\
				pret->prev->ret = !pret->prev->ret;\
				invert = 0;\
			}\
			pprev = pret->prev;\
			pnext = pret->next->next;\
			pprev->next = pnext;\
			if(pnext)\
				pprev->next->prev = pprev;\
			free(pret->next);\
			free(pret);\
			pret = ret;\
		}\
	}

int test_numerique(char *buffer){
	char *pbuf = buffer;
	while(*pbuf != 0){
		if((*pbuf < 48 || *pbuf >57) && *pbuf != '-' && *pbuf != '.'){
			return -1;
		}
		pbuf++;
	}
	return 0;
}
struct retour *reader(char *string, unsigned long int type){
	struct fn f;
	struct retour *pret= NULL, *pprev;
	struct objet o = {0, 0, NULL, NULL};
	void *var2 = NULL;
	char *matching[17] = {"==", "!=" ,">=", "<=", ">", "<", "~=", "!~", "-z", "-Z", "-n", "-N", "!", "&&", "||","(",")"};
	char *r = NULL, *str = NULL, quote = 0, dquote = 0;
	unsigned long int size, offset, i, ___quote___ = 0, parentheses = 0, end = 0;
	int j, _end_;
	switch(type){
		case FLOAT:
			f.diff = f_diff;
			f.equal = f_equal;
			f.less = f_less ;
			f.e_less = f_e_less ;
			f.greater = f_greater;
			f.e_greater = f_e_greater;
			size = sizeof(float);
			o.size = size;
			var1 = ___calloc___(&var1, 2*size);
			//var1 = malloc(2*size);
			var2 = var1+size;
			break;
		case DOUBLE:
			f.diff = d_diff;
			f.equal = d_equal;
			f.less = d_less ;
			f.e_less = d_e_less ;
			f.greater = d_greater;
			f.e_greater = d_e_greater;
			size = sizeof(double);
			o.size = size;
			var1 = ___calloc___(&var1, 2*size);
			//var1 = calloc(2*size);
			var2 = var1+size;
			break;
		case LDOUBLE:
			f.diff = ld_diff;
			f.equal = ld_equal;
			f.less = ld_less;
			f.e_less = ld_e_less;
			f.greater = ld_greater;
			f.e_greater = ld_e_greater;
			size = sizeof(long double);
			o.size = size;
			var1 = ___calloc___(&var1, 2*size);
			var2 = var1+size;
			break;
	}
	f.strings = &strings;
	f.strings_diff = strings_diff;
	f.strings_eq = strings_eq;
	for(r = string, offset = 1; *r != 0; r++, offset++){
		for(i = 0; i < 17; i++){
			if(strncmp(r,matching[i], strlen(matching[i])) == 0)
				break;
		}
		/*if(o.var1 && i == 17)
			exit(EXIT_FAILURE);*/
		switch(i){
			case EQUAL:
				end = 0;
				STRING_DEF;
				NUMBERS(1, f.equal);
				break;
			case DIFF:
				STRING_DEF;
				NUMBERS(1, f.diff);
				o.var1 = NULL;
				break;
			case E_GREATER:
				STRING_DEF;
				NUMBERS(1, f.e_greater);
				o.var1 = NULL;
				break;
			case E_LESS:
				STRING_DEF;
				NUMBERS(1, f.e_less);
				o.var1 = NULL;
				break;
			case GREATER:
				STRING_DEF;
				NUMBERS(0, f.greater)
				o.var1 = NULL;
				break;
			case LESS:
				STRING_DEF;
				NUMBERS(0, f.less);
				o.var1 = NULL;
				break;
			case STRINGS_EQ:
				end = 0;
				//printf("%s\n",o.var1);
				READ_STRING(f.strings_eq);
				o.var1 = NULL;
				break;
			case STRINGS_DIFF:
				READ_STRING(f.strings_diff);
				o.var1 = NULL;
				break;
			case STR:
				if(o.var1){
					ERROR("Erreur vers l'offset: %lu\n", offset);
				}
				//f.strings(o.var1);
				STRING_EXIST(1, 0);
				pret->ret = f.strings(str);
				o.var1 = NULL;
				break;
			case NOT_STR:/*la chaine de caractere N'exist PAS*/
				if(o.var1){
					ERROR("Erreur vers l'offset: %lu\n", offset);
				}
				//f.strings(o.var1);
				STRING_EXIST(0, 1);
				pret->ret = !f.strings(str);
				o.var1 = NULL;
				break;
			case NUM:/*-n: est nombre*/
				if(o.var1){
					ERROR("Erreur vers l'offset: %lu\n", offset);
				}
				STRING_EXIST(1, 0);
				//if(f.strings(o.var1)){
					NUMERIQUE(1, 0)
				//}
				o.var1 = NULL;
				break;
			case NOT_NUM:/*-N: N'est PAS nombre*/
				if(o.var1){
					ERROR("Erreur vers l'offset: %lu\n", offset);
				}
				STRING_EXIST(0, 1);
				pret->ret = f.strings(str);
				//if(f.strings(o.var1)){
					NUMERIQUE(0, 1);
				//}
				o.var1 = NULL;
				break;
			case 12:/*INVERT*/
				ALLOC;
				pret->ret = -1;
				pret->operator = INVERT;
				break;
			case 13:/*AND*/
				if(pret && (pret->operator == AND || pret->operator == OR)){
					ERROR("Erreur de syntaxe vers l'offset %lu.\n", offset);
				}
				if(ret == NULL){
					ERROR("Erreur de syntaxe vers l'offset %lu.\n", offset);
				}else{
					pret->next = ___calloc___((void **)&pret->next,sizeof(struct retour));
					pret->next->prev = pret;
					pret = pret->next;
				}
				//printf("%s\n",r);
				pret->ret = -1;
				pret->operator = AND;
				r++;
				offset++;
				o.var1 = NULL;
				break;
			case 14: /*OR*/
				if(pret && (pret->operator == AND || pret->operator == OR)){
					ERROR("Erreur de syntaxe vers l'offset %lu.\n", offset);
				}
				if(ret == NULL){
					ERROR("Erreur de syntaxe vers l'offset %lu.\n", offset);
				}else{
					pret->next = ___calloc___((void **)&pret->next,sizeof(struct retour));
					pret->next->prev = pret;
					pret = pret->next;
				}
				pret->ret = -1;
				pret->operator = OR;
				r++;
				offset++;
				o.var1 = NULL;
				break;
			case 15:
				*r = 0;
				ALLOC;
				pret->ret = -1;
				pret->operator = O_PARENTHESE;
				parentheses++;
				break;
			case 16: PARENTHESE:
				*r = 0;
				if((long int)parentheses -1<0){
					fprintf(stderr, "Trop de parentheses fermee.\n");
					exit(EXIT_FAILURE);
				}
				ALLOC;
				pret->ret = -1;
				pret->operator = C_PARENTHESE;
				if(pret->prev->operator == O_PARENTHESE){
					fprintf(stderr, "Erreur de syntaxe vers l'offset: %lu.\n", offset-2);
					exit(EXIT_FAILURE);
				}
				parentheses--;
				break;
			default:
				switch(*r){
					case ' ':case '\t':case '\n':
						if(!quote && !dquote && o.type != STRING && o.var1 !=NULL){
							end = 1;
							*r = 0;
						}
						//offset++;
						break;
					case '\\':
						o.type = STRING;
						offset++;
						r++;
						//r+=strlen(o.var1);
						break;
					case '"':
						if(!quote){
							QUOTE(dquote,'"', o.var1);
						}
						break;
					case '\'':
						if(!dquote){
							QUOTE(quote,'\'', o.var1);
						}
						break;
					case '-':
						if((*(r+1) < 48 || *(r+1) > 57) || *(r+1) == '.' ){
						//if(!dquote){
							fprintf(stderr, "\'-%c\': Option unknown.\n", *(r+1));
							exit(EXIT_FAILURE);
						}
						//}
						break;
					default:
						if(o.var1 == NULL){
							o.var1 = r;
						}
						/*if(end == 1){
							fprintf(stderr, "=>Erreur de syntaxe vers l'offset: %lu.\n", offset);
							exit(EXIT_FAILURE);
						}*/
						if((*r < 48 || *r > 57) && *r != '.' && *r != '-')
							o.type = STRING;
						break;
				}
		}

	}
	pprev = pret = ret;
	_end_ = 0;
	/*if(pret && pret->ret == -1){
		fprintf(stderr,"Erreur de syntaxe.\n");
		exit(EXIT_FAILURE);
	}*/
	if(pret && (pret->operator == AND || pret->operator == OR)){
		fprintf(stderr,"Erreur de syntaxe.\n");
		exit(EXIT_FAILURE);
	}
	if(parentheses){
		fprintf(stderr,"Erreur: parenthese manquante.\n");
		exit(EXIT_FAILURE);
	}
	if(quote || dquote){
		fprintf(stderr, "Quote(/Double Quote) non terminee.\n");
		exit(EXIT_FAILURE);
	}
	return ret;
}
struct retour *comput(struct retour **r){
	struct retour *pret = *r, *pnext, *pprev, *pop;
	int invert = 0;
	long int par = 0;
	pret = *r;
	while(pret){
		switch(pret->operator){
			case 0: 
				break;
			case AND:
				CALCULE( pret->prev->ret &= pret->next->ret, 0);
				continue;
			case OR:
				CALCULE( pret->prev->ret |= pret->next->ret, 1);
				continue;
			case INVERT:
				pprev = pret->prev;
				pnext = pret->next;
				if(pprev){
					pprev->next = pnext;
					if(pnext)
						pprev->next->prev = pprev;
				}else{
					ret = pret = pret->next;
					free(pret->prev);
					ret->prev = pret->prev = NULL;
				}
				invert = !invert;
				continue;
			case O_PARENTHESE:
				pprev = pret->prev;
				pnext = pret->next;
				if(pprev == NULL){
					pret = ret =ret->next;
					free(pret->prev);
					pret->prev = NULL;
					ret->prev = NULL;
				}else{
					pprev->next = pnext;
					if(pnext)
						pprev->next->prev = pprev;
					free(pret);
					pret = pnext;
				}
				pret = comput(&pret);
				pret = ret;
				continue;
			case C_PARENTHESE:
				pprev = pret->prev;
				pnext = pret->next;
				pprev->next = pnext;
				if(pnext)
					pprev->next->prev =pprev;
				free(pret);
				return pprev;
		}
		//if(pret)
		pret = pret->next;
	}
	if(invert)
		ret->ret = !ret->ret;
	return ret;
}
void arguments(int key, char *arg, struct parser_state *state){
	struct arguments *a = state->input;
	unsigned long int w;
	int fd, fd_, random;
	struct stat s;
	char buffer[1024];
	switch(key){
		case 'f':
			a->nbrtype = FLOAT;
			break;
		case 'd':
			a->nbrtype = DOUBLE;
			break;
		case 'D':
			a->nbrtype = LDOUBLE;
			break;
		case 'F':
			if((fd = open(arg,O_RDONLY)) < 0){
				perror("open()");
				exit(EXIT_FAILURE);
			}
			if(fstat(fd,&s) < 0){
				perror("fstat()");
				exit(EXIT_FAILURE);
			}
			srand(time(NULL));
			do{
				start:
				random = rand();
				sprintf(file , ".compare-%u.tmp", random);
				if((fd_ = open(file, O_RDONLY)) > -1){
					memset(file, 0, 28);
					close(fd_);
					goto start;
				}
				if((a->fd = open(file, O_RDWR | O_CREAT, 0644)) < 0){
					perror("open()");
					exit(EXIT_FAILURE);
				}
				break;
			}while(1);
			while((w = read(fd,buffer,1024))){
				write(a->fd,buffer,w);
			}
			close(fd);
			if((a->mmap = mmap(NULL,s.st_size,PROT_READ | PROT_WRITE, MAP_SHARED, a->fd, 0)) == NULL){
				perror("mmap()");
				exit(EXIT_FAILURE);
			}
			break;
		default:
			if(a->string == NULL){
				a->string = arg;
			}else{
				fprintf(stderr, "Trop d'arguments\n");
				exit(EXIT_FAILURE);
			}
	}
}
void bye(void){
	struct retour *r;
	if(var1)
		free(var1);
	while(ret){
		r = ret->next;
		free(ret);
		ret = r;
	}
	if(strlen(file))
		remove(file);
}
struct parser args = {options, arguments, "[OPTIONS]", NULL, "Petit outil de test", &program, NULL};

int main(int argc, char **argv){
	struct arguments arg = {FLOAT, NULL, NULL, 0, 0};
	struct retour *pret;
	int i_ret,___i_ret___ = 0;
	memset(file, 0, 28);
	atexit(bye);
	parser_parse(&args, argc, argv, &arg);
	if(arg.string || arg.mmap){
		if(arg.mmap == NULL){
			ret = reader(arg.string, arg.nbrtype);
		}
		else{
			ret = reader(arg.mmap, arg.nbrtype);
		}
	}
	if(arg.fd)
		close(arg.fd);
	comput(&ret);
	pret = ret;
	while(pret){
		___i_ret___++;
		pret = pret->next;
	}
	if(___i_ret___ > 1){
		fprintf(stderr, "Erreur de syntaxe\n");
		exit(EXIT_FAILURE);
	}
	if(!ret)
		return -2;
	i_ret = !ret->ret;
	return i_ret;
}
int f_equal(void *num1, void *num2){
	if(*((float *)num1) == *((float *)num2))
		return 1;
	return 0;
}
int f_diff(void *num1, void *num2){
	if(*((float *)num1) != *((float *)num2))
		return 1;
	return 0;
}
int f_e_less(void *num1, void *num2){
	if(*((float *)num1) <= *((float *)num2))
		return 1;
	return 0;
}
int f_less(void *num1, void *num2){
	if(*((float *)num1) < *((float *)num2))
		return 1;
	return 0;
}

int f_e_greater(void *num1, void *num2){
	if(*((float *)num1) >= *((float *)num2))
		return 1;
	return 0;
}
int f_greater(void *num1, void *num2){
	if(*((float *)num1) > *((float *)num2))
		return 1;
	return 0;
}

int d_equal(void *num1, void *num2){
	if(*((double *)num1) == *((double *)num2))
		return 1;
	return 0;
}
int d_diff(void *num1, void *num2){
	if(*((double *)num1) != *((double *)num2))
		return 1;
	return 0;
}
int d_e_less(void *num1, void *num2){
	if(*((double *)num1) <= *((double *)num2))
		return 1;
	return 0;
}
int d_less(void *num1, void *num2){
	if(*((double *)num1) < *((double *)num2))
		return 1;
	return 0;
}
int d_e_greater(void *num1, void *num2){
	if(*((double *)num1) >= *((double *)num2))
		return 1;
	return 0;
}
int d_greater(void *num1, void *num2){
	if(*((double *)num1) > *((double *)num2))
		return 1;
	return 0;
}

int ld_equal(void *num1, void *num2){
	if(*((long double *)num1) == *((long double *)num2))
		return 1;
	return 0;
}
int ld_diff(void *num1, void *num2){
	if(*((long double *)num1) != *((long double *)num2))
		return 1;
	return 0;
}
int ld_e_less(void *num1, void *num2){
	if(*((long double *)num1) <= *((long double *)num2))
		return 1;
	return 0;
}
int ld_less(void *num1, void *num2){
	if(*((long double *)num1) < *((long double *)num2))
		return 1;
	return 0;
}
int ld_e_greater(void *num1, void *num2){
	if(*((long double *)num1) >= *((long double *)num2))
		return 1;
	return 0;
}
int ld_greater(void *num1, void *num2){
	if(*((long double *)num1) > *((long double *)num2))
		return 1;
	return 0;
}
static int strings(void *str){
        if(str && strlen(str) > 0)
                return 1;
        return 0;
}

static int strings_eq(void *str1, void *str2){
	if(!str1 || !str2)
		return 0;
	if(strcmp((char *)str1, (char *)str2) == 0)
		return 1;
	return 0;
}
static int strings_diff(void *str1, void *str2){
	if(strcmp((char *)str1, (char *)str2) != 0)
		return 1;
	return 0;
}