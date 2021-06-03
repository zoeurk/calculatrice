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
			*((type *)var1) = fonction(o.var1, &str);\
			if(str && strlen(str) > 0){\
				fprintf(stderr, "WARNING: Mauvais caractere detectee: <%s>.\n", str);\
			}\
			*((type *)var2) = fonction(o.var2, &str);\
			if(str && strlen(str) > 0){\
				fprintf(stderr, "WARNING: Mauvais caractere detectee: <%s>.\n", str);\
			}\
			*((type *)var2) = fonction(o.var2, &str);\

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
	while((*r >47 && *r < 58) || *r == '-' || *r =='.'){\
		r++;\
		offset++;\
	}\
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
	*r = 0; \
	CONVERT(type);\
	pret->ret = fn(var1,var2);\
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

#define calcule(calcule)\
	if(pret->next->ret != -1){\
		calcule;\
		pprev = pret->prev;\
		pnext = pret->next;\
		pprev->next = pnext->next;\
		if(pprev->next)\
			pprev->next->prev = pprev;\
		free(pret->next);\
		free(pret);\
		pret = ret;\
		continue;\
	}else{\
		if(pret->next->operator == INVERT){\
			pret = pret->next;\
			goto INVERT;\
		}\
		if(pret->next->operator == O_PARENTHESE){\
			pret = pret->next;\
			goto PARENTHESE;\
		}\
	}\
	pret = ret;


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
	struct retour *pret= NULL;
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
			var1 = malloc(2*size);
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
			var1 = malloc(2*size);
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
			var1 = malloc(2*size);
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
		/**r = *(r+back);*/
		switch(i){
			case EQUAL:end = 0;
				STRING_DEF;
				NUMBERS(1, f.equal);
				break;
			case DIFF:
				STRING_DEF;
				NUMBERS(1, f.diff);
				break;
			case E_GREATER:
				STRING_DEF;
				NUMBERS(1, f.e_greater);
				break;
			case E_LESS:
				STRING_DEF;
				NUMBERS(1, f.e_less);
				break;
			case GREATER:
				STRING_DEF;
				NUMBERS(0, f.greater)
				break;
			case LESS:
				STRING_DEF;
				NUMBERS(0, f.less);
				break;
			case STRINGS_EQ:
				end = 0;
				//printf("%s\n",o.var1);
				READ_STRING(f.strings_eq);
				break;
			case STRINGS_DIFF:
				READ_STRING(f.strings_diff);
				break;
			case STR:
				//f.strings(o.var1);
				STRING_EXIST(1, 0);
				pret->ret = f.strings(str);
				o.var1 = NULL;
				break;
			case NOT_STR:/*la chaine de caractere N'exist PAS*/
				//f.strings(o.var1);
				STRING_EXIST(0, 1);
				pret->ret = !f.strings(str);
				o.var1 = NULL;
				break;
			case NUM:/*-n: est nombre*/
				STRING_EXIST(1, 0);
				//if(f.strings(o.var1)){
					NUMERIQUE(1, 0)
				//}
				o.var1 = NULL;
				break;
			case NOT_NUM:/*-N: N'est PAS nombre*/
				STRING_EXIST(0, 1);
				//if(f.strings(o.var1)){
					NUMERIQUE(0, 1);
				//}
				o.var1 = NULL;
				break;
			case 12:/*INVERT*/
				str = r;
				while(*r != '!' && *r != '(' && *r != 0 && *r != '&' && *r != '|' && *r != ')'){
					r++;
					offset++;
				}
				/*if(*r != '!'){
					if(*r == '('){
						r--;
						offset--;
					}else{
						fprintf(stderr,"Charactere inverse (!) mal place.\n");
						exit(EXIT_FAILURE);
					}
				}else{*/
					while(*str == '!' || (*(str+1) != 0 && *(str+1) != '&' && *(str+1) != '|' && *(str+1) == ')')){
						str++;
					}
					if((*(str+1) == 0 || *(str+1) == '&' ||  *(str+1) == '|' || *(str+1) == ')')){
						fprintf(stderr,"Charactere inverse (!) mal place.\n");
						exit(EXIT_FAILURE);
					}
					//printf("<%c>", *str);
				//}
				ALLOC;
				pret->ret = -1;
				pret->operator = INVERT;
				break;
			case 13:/*AND*/
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
						fprintf(stderr, "\'-%c\': Option unknown.\n", *(r+1));
						exit(EXIT_FAILURE);
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
	/*for(pret = ret; r != NULL; pret = pret->next){
		if(pret == NULL)break;
		printf("%i;%i\n", pret->ret, pret->operator);
	}*/
	return ret;
}
void comput(struct retour **r){
	struct retour *pret = *r, *pnext, *pprev;
	int invert = 0;
	while(pret){
		switch(pret->operator){
			case 0: 
				break;
			case AND:
				calcule(pret->prev->ret = pret->prev->ret&pret->next->ret);
				continue;
			case OR:
				calcule(pret->prev->ret = pret->prev->ret|pret->next->ret);
				continue;
			case INVERT:
				INVERT:
				pprev = pret->prev;
				pnext = pret->next;
				if(pprev){
					pprev->next = pnext;
					if(pnext)
						pprev->next->prev = pprev;
				}
				if(!pprev){
					pret = ret = pnext;
					if(pret){
						free(pret->prev);
						pret->prev = NULL;
					}
				}else{
					pprev->next = pnext;
					if(pnext)
						pprev->next->prev = pprev;
					free(pret);
					pret = pprev;
				}
				pret = pret->next->next;
				invert = !invert;
				pret = ret;
				continue;
			case O_PARENTHESE:
				PARENTHESE:
				pret = ret = ret->next;
				pprev = pret->prev;
				free(pret->prev);
				pret->prev = pprev;
				pret = ret;
				continue;
			case C_PARENTHESE:
				pnext = pret->next;
				pret = pret->prev;
				free(pret->next);
				pret->next = pnext;
				if(pnext)
					pret->next->prev = pret;
				pret = ret;
				continue;
		}
		pret = pret->next;
	}
	if(invert)
		ret->ret = !ret->ret;
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
			if(a->string == NULL)
				a->string = arg;
			else{
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
	int i_ret;
	memset(file, 0, 28);
	atexit(bye);
	parser_parse(&args, argc, argv, &arg);
	if(arg.mmap == NULL)
		ret = reader(arg.string, arg.nbrtype);
	else	ret = reader(arg.mmap, arg.nbrtype);
	if(arg.fd)
		close(arg.fd);
	comput(&ret);
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
	if(strcmp((char *)str1, (char *)str2) == 0)
		return 1;
	return 0;
}
static int strings_diff(void *str1, void *str2){
	if(strcmp((char *)str1, (char *)str2) != 0)
		return 1;
	return 0;
}
