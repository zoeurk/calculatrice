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
	CHAR,
	UCHAR,
	SHORT,
	USHORT,
	INT,
	UINT,
	LINT,
	LUINT,
	LLINT,
	LLUINT,
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

static int c_equal(void *num1, void *num2);
static int c_diff(void *num1, void *num2);
static int c_e_less(void *num1, void *num2);
static int c_less(void *num1, void *num2);
static int c_e_greater(void *num1, void *num2);
static int c_greater(void *num1, void *num2);

static int uc_equal(void *num1, void *num2);
static int uc_diff(void *num1, void *num2);
static int uc_e_less(void *num1, void *num2);
static int uc_less(void *num1, void *num2);
static int uc_e_greater(void *num1, void *num2);
static int uc_greater(void *num1, void *num2);

static int s_equal(void *num1, void *num2);
static int s_diff(void *num1, void *num2);
static int s_e_less(void *num1, void *num2);
static int s_less(void *num1, void *num2);
static int s_e_greater(void *num1, void *num2);
static int s_greater(void *num1, void *num2);

static int us_equal(void *num1, void *num2);
static int us_diff(void *num1, void *num2);
static int us_e_less(void *num1, void *num2);
static int us_less(void *num1, void *num2);
static int us_e_greater(void *num1, void *num2);
static int us_greater(void *num1, void *num2);

static int i_equal(void *num1, void *num2);
static int i_diff(void *num1, void *num2);
static int i_e_less(void *num1, void *num2);
static int i_less(void *num1, void *num2);
static int i_e_greater(void *num1, void *num2);
static int i_greater(void *num1, void *num2);

static int ui_equal(void *num1, void *num2);
static int ui_diff(void *num1, void *num2);
static int ui_e_less(void *num1, void *num2);
static int ui_less(void *num1, void *num2);
static int ui_e_greater(void *num1, void *num2);
static int ui_greater(void *num1, void *num2);

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

int i_equal(void *num1, void *num2);
int i_diff(void *num1, void *num2);
int i_e_less(void *num1, void *num2);
int i_less(void *num1, void *num2);
int i_e_greater(void *num1, void *num2);
int i_greater(void *num1, void *num2);

int ui_equal(void *num1, void *num2);
int ui_diff(void *num1, void *num2);
int ui_e_less(void *num1, void *num2);
int ui_less(void *num1, void *num2);
int ui_e_greater(void *num1, void *num2);
int ui_greater(void *num1, void *num2);

int li_equal(void *num1, void *num2);
int li_diff(void *num1, void *num2);
int li_e_less(void *num1, void *num2);
int li_less(void *num1, void *num2);
int li_e_greater(void *num1, void *num2);
int li_greater(void *num1, void *num2);

int lui_equal(void *num1, void *num2);
int lui_diff(void *num1, void *num2);
int lui_e_less(void *num1, void *num2);
int lui_less(void *num1, void *num2);
int lui_e_greater(void *num1, void *num2);
int lui_greater(void *num1, void *num2);

int lli_equal(void *num1, void *num2);
int lli_diff(void *num1, void *num2);
int lli_e_less(void *num1, void *num2);
int lli_less(void *num1, void *num2);
int lli_e_greater(void *num1, void *num2);
int lli_greater(void *num1, void *num2);

int llui_equal(void *num1, void *num2);
int llui_diff(void *num1, void *num2);
int llui_e_less(void *num1, void *num2);
int llui_less(void *num1, void *num2);
int llui_e_greater(void *num1, void *num2);
int llui_greater(void *num1, void *num2);

static int strings(void *str);
static int strings_eq(void *str1, void *str2);
static int strings_diff(void *str1, void *str2);

struct info program = {"version: 1.0","zoeurk@gmail.com"};
struct parser_option options[] =	{
					{ "char", 'c', 0, NULL, "Utiliser le type char"},
					{ "unsigned-char", 'C', 0, NULL, "Utiliser le type unsigned char"},
					{ "short", 's', 0, NULL, "Utiliser le type short int"},
					{ "unsigned-short", 'S', 0, NULL, "Utiliser le type unsigned short int"},
					{ "int", 'i', 0, NULL, "Utiliser le type int"},
					{ "unsigned-int", 'I', 0, NULL, "Utiliser le type unsigned int"},
					{ "long-int" ,'l', 0, NULL, "Utiliser le tye long int"},
					{ "unsigned-long-int" ,'L', 0, NULL, "Utiliser le tye unsigned long int"},
					{ "long-long-int" ,'t', 0, NULL, "Utiliser le tye long long int"},
					{ "unsigned-long-long-int" ,'T', 0, NULL, "Utiliser le tye unsigned long long int"},
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
		case CHAR:\
			if(strchr(o.var1,'.')){\
				fprintf(stderr,"Chiffre a virgule interdit dans ce mode.\n");\
				exit(EXIT_FAILURE);\
			}\
			*((char *)var1) = (char)atoi(o.var1);\
			if(strchr(o.var2,'.')){\
				fprintf(stderr,"Chiffre a virgule interdit dans ce mode.\n");\
				exit(EXIT_FAILURE);\
			}\
			*((char *)var2) = (char)atoi(o.var2);\
			break;\
		case UCHAR:\
			if(o.var1[0] == '-' || o.var1[0] == '+' || o.var2[0] == '-' || o.var2[0] == '+'){\
				fprintf(stderr,"WARNING: nombre signe detecte, le resultat peut etre impredictible\n");\
			}\
			if(strchr(o.var1,'.')){\
				fprintf(stderr,"Chiffre a virgule interdit dans ce mode.\n");\
				exit(EXIT_FAILURE);\
			}\
			*((unsigned char *)var1) = (unsigned char)atoi(o.var1);\
			if(strchr(o.var2,'.')){\
				fprintf(stderr,"Chiffre a virgule interdit dans ce mode.\n");\
				exit(EXIT_FAILURE);\
			}\
			*((unsigned char *)var2) = atoi(o.var2);\
			break;\
		case USHORT:\
			if(o.var1[0] == '-' || o.var1[0] == '+' || o.var2[0] == '-' || o.var2[0] == '+'){\
				fprintf(stderr,"WARNING: nombre signe detecte, le resultat peut etre impredictible\n");\
			}\
			if(strchr(o.var1,'.')){\
				fprintf(stderr,"Chiffre a virgule interdit dans ce mode.\n");\
				exit(EXIT_FAILURE);\
			}\
			*((unsigned short *)var1) = (unsigned short)atoi(o.var1);\
			if(strchr(o.var2,'.')){\
				fprintf(stderr,"Chiffre a virgule interdit dans ce mode.\n");\
				exit(EXIT_FAILURE);\
			}\
			*((int *)var2) = atoi(o.var2);\
			break;\
		case SHORT:\
			if(strchr(o.var1,'.')){\
				fprintf(stderr,"Chiffre a virgule interdit dans ce mode.\n");\
				exit(EXIT_FAILURE);\
			}\
			*((short int *)var1) = (short int)atoi(o.var1);\
			if(strchr(o.var2,'.')){\
				fprintf(stderr,"Chiffre a virgule interdit dans ce mode.\n");\
				exit(EXIT_FAILURE);\
			}\
			*((short int *)var2) = (short int)atoi(o.var2);\
			break;\
		case INT:\
			if(strchr(o.var1,'.')){\
				fprintf(stderr,"Chiffre a virgule interdit dans ce mode.\n");\
				exit(EXIT_FAILURE);\
			}\
			*((int *)var1) = atoi(o.var1);\
			if(strchr(o.var2,'.')){\
				fprintf(stderr,"Chiffre a virgule interdit dans ce mode.\n");\
				exit(EXIT_FAILURE);\
			}\
			*((int *)var2) = atoi(o.var2);\
			break;\
		case UINT:\
			if(o.var1[0] == '-' || o.var1[0] == '+' || o.var2[0] == '-' || o.var2[0] == '+'){\
				fprintf(stderr,"WARNING: nombre signe detecte, le resultat peut etre impredictible\n");\
			}\
			if(strchr(o.var1,'.')){\
				fprintf(stderr,"Chiffre a virgule interdit dans ce mode.\n");\
				exit(EXIT_FAILURE);\
			}\
			*((unsigned int *)var1) = (unsigned int)atoi(o.var1);\
			if(strchr(o.var2,'.')){\
				fprintf(stderr,"Chiffre a virgule interdit dans ce mode.\n");\
				exit(EXIT_FAILURE);\
			}\
			*((unsigned int *)var2) = (unsigned int)atoi(o.var2);\
			break;\
		case LUINT:\
			if(o.var1[0] == '-' || o.var1[0] == '+' || o.var2[0] == '-' || o.var2[0] == '+'){\
				fprintf(stderr,"WARNING: nombre signe detecte, le resultat peut etre impredictible\n");\
			}\
			if(strchr(o.var1,'.')){\
				fprintf(stderr,"Chiffre a virgule interdit dans ce mode.\n");\
				exit(EXIT_FAILURE);\
			}\
			*((long unsigned int *)var1) = (long unsigned int)atol(o.var1);\
			if(strchr(o.var2,'.')){\
				fprintf(stderr,"Chiffre a virgule interdit dans ce mode.\n");\
				exit(EXIT_FAILURE);\
			}\
			*((long unsigned int *)var2) = (long unsigned int)atol(o.var2);\
			break;\
		case LLINT:\
			if(strchr(o.var1,'.')){\
				fprintf(stderr,"Chiffre a virgule interdit dans ce mode.\n");\
				exit(EXIT_FAILURE);\
			}\
			*((long long int *)var1) = (long long int)atoll(o.var1);\
			if(strchr(o.var2,'.')){\
				fprintf(stderr,"Chiffre a virgule interdit dans ce mode.\n");\
				exit(EXIT_FAILURE);\
			}\
			*((long long int *)var2) = (long long int)atoll(o.var2);\
			break;\
		case LLUINT:\
			if(o.var1[0] == '-' || o.var1[0] == '+' || o.var2[0] == '-' || o.var2[0] == '+'){\
				fprintf(stderr,"WARNING: nombre signe detecte, le resultat peut etre impredictible\n");\
			}\
			if(strchr(o.var1,'.')){\
				fprintf(stderr,"Chiffre a virgule interdit dans ce mode.\n");\
				exit(EXIT_FAILURE);\
			}\
			*((long long unsigned int *)var1) = (long long unsigned int)atoi(o.var1);\
			if(strchr(o.var2,'.')){\
				fprintf(stderr,"Chiffre a virgule interdit dans ce mode.\n");\
				exit(EXIT_FAILURE);\
			}\
			*((unsigned int *)var2) = (long long unsigned int)atol(o.var2);\
			break;\
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
		if(pret->operator == 0)\
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
		case CHAR:
			f.diff = c_diff;
			f.equal = c_equal;
			f.less = c_less ;
			f.e_less = c_e_less ;
			f.greater = c_greater;
			f.e_greater = c_e_greater;
			size = sizeof(char);
			o.size = size;
			var1 = ___calloc___(&var1, 2*size);
			//var1 = malloc(2*size);
			var2 = var1+size;
			break;
		case SHORT:
			f.diff = s_diff;
			f.equal = s_equal;
			f.less = s_less ;
			f.e_less = s_e_less ;
			f.greater = s_greater;
			f.e_greater = s_e_greater;
			size = sizeof(short int);
			o.size = size;
			var1 = ___calloc___(&var1, 2*size);
			//var1 = malloc(2*size);
			var2 = var1+size;
			break;
		case INT:
			f.diff = i_diff;
			f.equal = i_equal;
			f.less = i_less ;
			f.e_less = i_e_less ;
			f.greater = i_greater;
			f.e_greater = i_e_greater;
			size = sizeof(int);
			o.size = size;
			var1 = ___calloc___(&var1, 2*size);
			//var1 = malloc(2*size);
			var2 = var1+size;
			break;
		case UCHAR:
			f.diff = uc_diff;
			f.equal = uc_equal;
			f.less = uc_less ;
			f.e_less = uc_e_less ;
			f.greater = uc_greater;
			f.e_greater = uc_e_greater;
			size = sizeof(unsigned char);
			o.size = size;
			var1 = ___calloc___(&var1, 2*size);
			//var1 = malloc(2*size);
			var2 = var1+size;
			break;
		case USHORT:
			f.diff = us_diff;
			f.equal = us_equal;
			f.less = us_less ;
			f.e_less = us_e_less ;
			f.greater = us_greater;
			f.e_greater = us_e_greater;
			size = sizeof(unsigned short int);
			o.size = size;
			var1 = ___calloc___(&var1, 2*size);
			//var1 = malloc(2*size);
			var2 = var1+size;
			break;
		case UINT:
			f.diff = ui_diff;
			f.equal = ui_equal;
			f.less = ui_less ;
			f.e_less = ui_e_less ;
			f.greater = ui_greater;
			f.e_greater = ui_e_greater;
			size = sizeof(unsigned int);
			o.size = size;
			var1 = ___calloc___(&var1, 2*size);
			//var1 = malloc(2*size);
			var2 = var1+size;
			break;
		case LINT:
			f.diff = li_diff;
			f.equal = li_equal;
			f.less = li_less ;
			f.e_less = li_e_less ;
			f.greater = li_greater;
			f.e_greater = li_e_greater;
			size = sizeof(long int);
			o.size = size;
			var1 = ___calloc___(&var1, 2*size);
			//var1 = malloc(2*size);
			var2 = var1+size;
			break;
		case LUINT:
			f.diff = lui_diff;
			f.equal = lui_equal;
			f.less = lui_less ;
			f.e_less = lui_e_less ;
			f.greater = lui_greater;
			f.e_greater = lui_e_greater;
			size = sizeof(long unsigned int);
			o.size = size;
			var1 = ___calloc___(&var1, 2*size);
			//var1 = malloc(2*size);
			var2 = var1+size;
			break;
		case LLINT:
			f.diff = lli_diff;
			f.equal = lli_equal;
			f.less = lli_less ;
			f.e_less = lli_e_less ;
			f.greater = lli_greater;
			f.e_greater = lli_e_greater;
			size = sizeof(long long int);
			o.size = size;
			var1 = ___calloc___(&var1, 2*size);
			//var1 = malloc(2*size);
			var2 = var1+size;
			break;
		case LLUINT:
			f.diff = llui_diff;
			f.equal = llui_equal;
			f.less = llui_less ;
			f.e_less = llui_e_less ;
			f.greater = llui_greater;
			f.e_greater = llui_e_greater;
			size = sizeof(long long unsigned int);
			o.size = size;
			var1 = ___calloc___(&var1, 2*size);
			//var1 = malloc(2*size);
			var2 = var1+size;
			break;
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
			if(strncmp(r,matching[i], strlen(matching[i])) == 0){
				break;
			}
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
							fprintf(stderr, "\'-%c\': Option unknown.\n", *(r+1));
							exit(EXIT_FAILURE);
						}
						o.var1 = r;
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

	//parser_usage(state->parser);
	//exit(0);
	switch(key){
		case 'C':
			a->nbrtype = UCHAR;
			break;
		case 'S':
			a->nbrtype = USHORT;
			break;
		case 'I':
			a->nbrtype = UINT;
			break;
		case 'c':
			a->nbrtype = CHAR;
			break;
		case 's':
			a->nbrtype = SHORT;
			break;
		case 'i':
			a->nbrtype = INT;
			break;
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
struct parser_state ps = {NULL, NULL, 0, 0, 0, 0, 0, 35, 75, 75, NULL, NULL, NULL, NULL};
struct parser args = {options, arguments, "[OPTIONS]", NULL, "Petit outil de test", &program, &ps};

int main(int argc, char **argv){
	struct arguments arg = {FLOAT, NULL, NULL, 0, 0};
	struct retour *pret;
	int i_ret,___i_ret___ = 0;
	//memset(&ps,0,sizeof(struct parser_state));
	//ps.arg_colonne = 100;
	memset(file, 0, 28);
	atexit(bye);
	ps.argv = argv;
	ps.parser = &args;
	/*ps.err_stream = stderr;
	ps.out_stream = stdout;*/
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


int c_equal(void *num1, void *num2){
	if(*((char *)num1) == *((char *)num2))
		return 1;
	return 0;
}
int c_diff(void *num1, void *num2){
	if(*((char *)num1) != *((char *)num2))
		return 1;
	return 0;
}
int c_e_less(void *num1, void *num2){
	if(*((char *)num1) <= *((char *)num2))
		return 1;
	return 0;
}
int c_less(void *num1, void *num2){
	if(*((char *)num1) < *((char *)num2))
		return 1;
	return 0;
}
int c_e_greater(void *num1, void *num2){
	if(*((char *)num1) >= *((char *)num2))
		return 1;
	return 0;
}
int c_greater(void *num1, void *num2){
	if(*((char *)num1) > *((char *)num2))
		return 1;
	return 0;
}
int uc_equal(void *num1, void *num2){
	if(*((unsigned char *)num1) == *((unsigned char *)num2))
		return 1;
	return 0;
}
int uc_diff(void *num1, void *num2){
	if(*((char *)num1) != *((unsigned char *)num2))
		return 1;
	return 0;
}
int uc_e_less(void *num1, void *num2){
	if(*((unsigned char *)num1) <= *((unsigned char *)num2))
		return 1;
	return 0;
}
int uc_less(void *num1, void *num2){
	if(*((unsigned char *)num1) < *((unsigned char *)num2))
		return 1;
	return 0;
}

int uc_e_greater(void *num1, void *num2){
	if(*((unsigned char *)num1) >= *((unsigned char *)num2))
		return 1;
	return 0;
}
int uc_greater(void *num1, void *num2){
	if(*((unsigned char *)num1) > *((unsigned char *)num2))
		return 1;
	return 0;
}

int s_equal(void *num1, void *num2){
	if(*((short int *)num1) == *((short int *)num2))
		return 1;
	return 0;
}
int s_diff(void *num1, void *num2){
	if(*((short int *)num1) != *((short int *)num2))
		return 1;
	return 0;
}
int s_e_less(void *num1, void *num2){
	if(*((short int *)num1) <= *((short int *)num2))
		return 1;
	return 0;
}
int s_less(void *num1, void *num2){
	if(*((short int *)num1) < *((short int *)num2))
		return 1;
	return 0;
}
int s_e_greater(void *num1, void *num2){
	if(*((short int *)num1) >= *((short int *)num2))
		return 1;
	return 0;
}
int s_greater(void *num1, void *num2){
	if(*((short int *)num1) > *((short int *)num2))
		return 1;
	return 0;
}
int us_equal(void *num1, void *num2){
	if(*((unsigned short int *)num1) == *((unsigned short int *)num2))
		return 1;
	return 0;
}
int us_diff(void *num1, void *num2){
	if(*((unsigned short int *)num1) != *((unsigned short int *)num2))
		return 1;
	return 0;
}
int us_e_less(void *num1, void *num2){
	if(*((unsigned short int *)num1) <= *((unsigned short int *)num2))
		return 1;
	return 0;
}
int us_less(void *num1, void *num2){
	if(*((unsigned short int *)num1) < *((unsigned short int *)num2))
		return 1;
	return 0;
}
int us_e_greater(void *num1, void *num2){
	if(*((unsigned short int *)num1) >= *((unsigned short int *)num2))
		return 1;
	return 0;
}
int us_greater(void *num1, void *num2){
	if(*((unsigned short int *)num1) > *((unsigned short int *)num2))
		return 1;
	return 0;
}

int i_equal(void *num1, void *num2){
	if(*((int *)num1) == *((int *)num2))
		return 1;
	return 0;
}
int i_diff(void *num1, void *num2){
	if(*((int *)num1) != *((int *)num2))
		return 1;
	return 0;
}
int i_e_less(void *num1, void *num2){
	if(*((int *)num1) <= *((int *)num2))
		return 1;
	return 0;
}
int i_less(void *num1, void *num2){
	if(*((int *)num1) < *((int *)num2))
		return 1;
	return 0;
}
int i_e_greater(void *num1, void *num2){
	if(*((int *)num1) >= *((int *)num2))
		return 1;
	return 0;
}
int i_greater(void *num1, void *num2){
	if(*((int *)num1) > *((int *)num2))
		return 1;
	return 0;
}
int ui_equal(void *num1, void *num2){
	if(*((int *)num1) == *((int *)num2))
		return 1;
	return 0;
}

int ui_diff(void *num1, void *num2){
	if(*((unsigned int *)num1) != *((unsigned int *)num2))
		return 1;
	return 0;
}
int ui_e_less(void *num1, void *num2){
	if(*((unsigned int *)num1) <= *((unsigned int *)num2))
		return 1;
	return 0;
}
int ui_less(void *num1, void *num2){
	if(*((unsigned int *)num1) < *((unsigned int *)num2))
		return 1;
	return 0;
}
int ui_e_greater(void *num1, void *num2){
	if(*((unsigned int *)num1) >= *((unsigned int *)num2))
		return 1;
	return 0;
}
int ui_greater(void *num1, void *num2){
	if(*((unsigned int *)num1) >= *((unsigned int *)num2))
		return 1;
	return 0;
}
int li_equal(void *num1, void *num2){
	if(*((long int *)num1) == *((long int *)num2))
		return 1;
	return 0;
}
int li_diff(void *num1, void *num2){
	if(*((long int *)num1) != *((long int *)num2))
		return 1;
	return 0;
}
int li_e_less(void *num1, void *num2){
	if(*((long int *)num1) <= *((long int *)num2))
		return 1;
	return 0;
}
int li_less(void *num1, void *num2){
	if(*((long int *)num1) < *((long int *)num2))
		return 1;
	return 0;
}
int li_e_greater(void *num1, void *num2){
	if(*((long int *)num1) >= *((long int *)num2))
		return 1;
	return 0;
}
int li_greater(void *num1, void *num2){
	if(*((long int *)num1) > *((long int *)num2))
		return 1;
	return 0;
}
int lui_equal(void *num1, void *num2){
	if(*((long unsigned int *)num1) == *((long unsigned int *)num2))
		return 1;
	return 0;
}
int lui_diff(void *num1, void *num2){
	if(*((long unsigned int *)num1) != *((long unsigned int *)num2))
		return 1;
	return 0;
}
int lui_e_less(void *num1, void *num2){
	if(*((long unsigned int *)num1) <= *((long unsigned int *)num2))
		return 1;
	return 0;
}
int lui_less(void *num1, void *num2){
	if(*((long unsigned int *)num1) < *((long unsigned int *)num2))
		return 1;
	return 0;
}
int lui_e_greater(void *num1, void *num2){
	if(*((long unsigned int *)num1) >= *((long unsigned int *)num2))
		return 1;
	return 0;
}
int lui_greater(void *num1, void *num2){
	if(*((long unsigned int *)num1) > *((long unsigned int *)num2))
		return 1;
	return 0;
}
int lli_equal(void *num1, void *num2){
	if(*((long long int *)num1) == *((long long int *)num2))
		return 1;
	return 0;
}
int lli_diff(void *num1, void *num2){
	if(*((long long int *)num1) != *((long long int *)num2))
		return 1;
	return 0;
}
int lli_e_less(void *num1, void *num2){
	if(*((long long int *)num1) <= *((long long int *)num2))
		return 1;
	return 0;
}
int lli_less(void *num1, void *num2){
	if(*((long long int *)num1) < *((long long int *)num2))
		return 1;
	return 0;
}
int lli_e_greater(void *num1, void *num2){
	if(*((long long int *)num1) >= *((long long int *)num2))
		return 1;
	return 0;
}
int lli_greater(void *num1, void *num2){
	if(*((long long int *)num1) > *((long long int *)num2))
		return 1;
	return 0;
}
int llui_equal(void *num1, void *num2){
	if(*((long long unsigned *)num1) == *((long long unsigned *)num2))
		return 1;
	return 0;
}
int llui_diff(void *num1, void *num2){
	if(*((long unsigned int *)num1) != *((long long unsigned *)num2))
		return 1;
	return 0;
}
int llui_e_less(void *num1, void *num2){
	if(*((long long unsigned *)num1) <= *((long long unsigned *)num2))
		return 1;
	return 0;
}
int llui_less(void *num1, void *num2){
	if(*((long long unsigned *)num1) < *((long long unsigned *)num2))
		return 1;
	return 0;
}
int llui_e_greater(void *num1, void *num2){
	if(*((long long unsigned *)num1) >= *((long long unsigned *)num2))
		return 1;
	return 0;
}
int llui_greater(void *num1, void *num2){
	if(*((long long unsigned *)num1) > *((long long unsigned *)num2))
		return 1;
	return 0;
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
