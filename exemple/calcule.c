#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <float.h>
#include <unistd.h>

#include "calcule-data.h"
#include "parsearg.h"
#include "operation.h"

#define BUFFER 56

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
struct two_numbers{
	struct value *start;
	struct value *virgule;
	struct value *end;
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
	C_PARENTHESE = 8,
	ADD = '+',
	LESS = '-',
	MULT = '*',
	DIV = '/'
};
enum OPERATOR{
	COS = 1, /*fait*/
	SIN = 2, /*fait*/
	TAN = 3, /*fait*/
	SQRT = 4, /*fait*/
	EXP = 5, /*fait*/
	LOG = 6, /*fait*/
	LOG10 = 7,/*fait*/
	FABS = 8, /*fait*/
	CEIL = 9, /*fait*/
	FLOOR = 10,/*fait*/
	POW = 11, /*fait*/
	FMOD = 12, /*fait*/
	ACOS = 13, /*fait*/
	ASIN = 14, /*fait*/
	ATAN = 15 /*fait*/
};
enum TYPE{
	FORMAT   = 1,
	U_PI	 = 2,
	V_PI	 = 4
};
/*Ajouter fonction*/
struct function{
	void (*addition)(void *, void *, void *arrondi);
	void (*soustraction)(void *, void *, void *arrondi);
	void (*multiplication)(void *, void *, void *arrondi);
	void (*division)(void *, void *, void *arrondi);
	void (*cosinus)(void *);
	void (*acosinus)(void *);
	void (*sinus)(void *);
	void (*asinus)(void *);
	void (*tangente)(void *);
	void (*atangente)(void *);
	void (*sqrt)(void *);
	void (*exp)(void *);
	void (*ceil)(void *);
	void (*log)(void *);
	void (*log10)(void *);
	void (*fabs)(void *);
	void (*floor)(void *);
	void (*mod)(void *, void *);
	void (*power)(void *, void *);
};
struct arguments{
	struct value *v;
	void *arrondi;
	unsigned long  int type;
	unsigned long int options;
	unsigned long int valsize;
	void (*print)(void *, char *);
	char *format;
	void *pi;
	char *argv;
	union{
		struct{
			void *mmap;
			unsigned long int size;
		}mmap;
		struct file{
			int file;
			int ___;
		}file;
	};
	struct function fn;
};

void arguments(int key, char *arg, struct parser_state *state){
	static float f = 0;
	static double d = 0;
	static long double ld = 0;
	struct arguments *a = state->input;
	struct stat stat;
	unsigned long int i;
	switch(key){
		case 'a':f = strtof(arg, NULL);
				 d = strtod(arg, NULL);
				 ld = strtold(arg, NULL);
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
		/*Ajouter fonction*/
		case 'f':
			a->print = print_float;
			a->valsize = sizeof(float);
			a->arrondi = &f;
			a->fn.addition = &faddition;
			a->fn.soustraction = &fsoustraction;
			a->fn.multiplication = &fmultiplication;
			a->fn.division = &fdivision;
			a->fn.cosinus = &fcosinus;
			a->fn.acosinus = &facosinus;
			a->fn.sinus = &fsinus;
			a->fn.asinus = &fasinus;
			a->fn.tangente = &ftangente;
			a->fn.tangente = &fatangente;
			a->fn.sqrt = &fsqrt;
			a->fn.exp = &fexp;
			a->fn.ceil = &fceil;
			a->fn.log = &flog;
			a->fn.log10 = &flog10;
			a->fn.fabs = &ffabs;
			a->fn.floor = &ffloor;
			a->fn.mod = &ffmod;
			a->fn.power = &fpower;
			a->type = FLOAT;
			break;
		case 'l':
			a->print = print_double;
			a->valsize = sizeof(double);
			a->arrondi = &d;
			a->fn.addition = daddition;
			a->fn.soustraction = &dsoustraction;
			a->fn.multiplication = &dmultiplication;
			a->fn.division = &ddivision;
			a->fn.cosinus = &dcosinus;
			a->fn.acosinus = &dacosinus;
			a->fn.sinus = &dsinus;
			a->fn.asinus = &dasinus;
			a->fn.tangente = &dtangente;
			a->fn.tangente = &datangente;
			a->fn.sqrt = &dsqrt;
			a->fn.exp = &dexp;
			a->fn.ceil = &dceil;
			a->fn.log = &dlog;
			a->fn.log10 = &dlog10;
			a->fn.fabs = &dfabs;
			a->fn.floor = &dfloor;
			a->fn.mod = &dfmod;
			a->fn.power = &dpower;
			a->type = DOUBLE;
			break;
		case 'L':
			a->print = print_ldouble;
			a->valsize = sizeof(long double);
			a->arrondi = &ld;
			a->fn.addition = &ldaddition;
			a->fn.soustraction = &ldsoustraction;
			a->fn.multiplication = &ldmultiplication;
			a->fn.division = &lddivision;
			a->fn.cosinus = &ldcosinus;
			a->fn.acosinus = &ldacosinus;
			a->fn.sinus = &ldsinus;
			a->fn.asinus = &ldasinus;
			a->fn.tangente = &ldtangente;
			a->fn.tangente = &ldatangente;
			a->fn.sqrt = &ldsqrt;
			a->fn.exp = &_ldexp_;
			a->fn.ceil = &ldceil;
			a->fn.log = &ldlog;
			a->fn.log10 = &ldlog10;
			a->fn.fabs = &ldfabs;
			a->fn.floor = &ldfloor;
			a->fn.mod = &ldfmod;
			a->fn.power = &ldpower;
			a->type = LDOUBLE;
			break;
		case 'p':
			a->pi = arg;
			a->options |= U_PI;
			break;
		case 'P':
			a->options |= V_PI;
			break;
		case 'F':
			if((a->file.file = open(arg,O_RDONLY)) < 0){
				perror("open()");
				exit(EXIT_FAILURE);
			}
			if(fstat(a->file.file, &stat) < 0){
				perror("stat()");
				exit(EXIT_FAILURE);
			}
			a->mmap.size = stat.st_size;
			a->mmap.mmap = mmap(NULL, stat.st_size , PROT_READ, MAP_SHARED, a->file.file, 0);
			break;
		default:
			if(a->argv == NULL)
				a->argv = arg;
			else{	fprintf(stderr, "trop d'arguments fournies\n");
				exit(EXIT_FAILURE);
			}
			break;
	}
	switch(a->type){
		case FLOAT:
			a->arrondi = &f;
			break;
		case DOUBLE:
			a->arrondi = &d;
			break;
		case LDOUBLE:
			a->arrondi = &ld;
			break;
	}
}
struct info program = {"version: 1.0","zoeurk@gmail.com"};
struct parser_option options[] =	{
					{ "float", 'f', 0, NULL, "Utiliser le type float"},
					{ "double", 'l', 0 , NULL, "Utiliser le type double"},
					{ "ldouble", 'L', 0, NULL, "Utiliser le type long double"},
					{ "arrondi", 'a', 0, "arrondi", "arrondir"},
					{ "format", 'O', 0, "FORMAT", "Nombre de chiffre apres la virgule"},
					{ "file", 'F', 0, "FILE", "lire le fichier"},
					{ "set-pi", 'p', 0, "PI", "Initialiser pi a la valeur de PI"},
					{ "view-pi", 'P', 0, NULL, "Voir la valeur de pi par default"},
					{0}
				};
struct parser args = {options, arguments, "[OPTIONS]", "[\"calcule\"]", "Petit outil de calcule", &program, NULL};

void *___calloc___(void **ptr, unsigned long int size){
	if((*ptr = calloc(1, size)) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	return *ptr;
}
/*ZERO(str1, str2)\
	for(str1 = &str2[strlen(str2)-1];str2 != str1 && *str1 == '0'; *str1 = 0, str1--);;\
	if(str1 == '.')*str1 = 0;\
	if(*str1 == str && *str1 == 0){\
		*str1 = '0';\
		*(str1+1) = 0;\
	}*/

void zero(char *str, int type){
	float f;
	double d;
	long double ld;
	char buffer[BUFFER], *pbuf = NULL;
	if(strchr(str,'.') != NULL){
		for(pbuf = &str[strlen(str)-1];str != pbuf && *pbuf == '0'; *pbuf = 0, pbuf--);;
		if(*pbuf == '.')*pbuf = 0;
		if(pbuf == str && *pbuf == 0){
			*pbuf = '0';
			*(pbuf+1) = 0;
		}
		//ZERO(pbuf, str);
	}
	switch(type){
		case FLOAT:
				f = strtof(str, NULL);
				sprintf(buffer,"%f", f);
				if(strchr(buffer,'.') != NULL){
					for(pbuf = &buffer[strlen(buffer)-1];pbuf != buffer && *pbuf == '0';*pbuf = 0, pbuf--);;
					if(*pbuf == '.') *pbuf = 0;
					if(pbuf == buffer && *pbuf == 0){
						*pbuf = '0';
						*(pbuf+1) = 0;
					}
					//ZERO(pbuf, buffer);
					if(strcmp(str, buffer) != 0 && strcmp(str,"PI") != 0 && *str != 0){
						fprintf(stderr, "ERROR: Nombre trop long pour etre converti dans ce format: %s != %s\n", str, buffer);
						exit(2);
					}
				}
				break;
		case DOUBLE:
				d = strtod(str, NULL);
				sprintf(buffer,"%lf", d );
				if(strchr(buffer,'.') != NULL && strcmp("PI",str) != 0){
					for(pbuf = &buffer[strlen(buffer)-1];pbuf != buffer && *pbuf == '0';*pbuf = 0, pbuf--);;
					if(*pbuf == '.') *pbuf = 0;
					if(pbuf == buffer && *pbuf == 0){
						*pbuf = '0';
						*(pbuf+1) = 0;
					}
					//ZERO(pbuf, buffer);
					if(strcmp(str, buffer) != 0 && pbuf != 0 && *str != 0){
						fprintf(stderr, "ERROR: Nombre trop long pour etre converti dans ce format: %s != %s\n", str, buffer);
						exit(2);
					}
				}
				break;
		case LDOUBLE:
				ld = strtold(str, NULL);
				sprintf(buffer,"%Lf", ld);
				if(strchr(buffer,'.') != NULL){
					//ZERO(pbuf, buffer);
					for(pbuf = &buffer[strlen(buffer)-1];pbuf != buffer && *pbuf == '0';*pbuf = 0, pbuf--);;
					if(*pbuf == '.') *pbuf = 0;
					if(pbuf == buffer && *pbuf == 0){
						*pbuf = '0';
						*(pbuf+1) = 0;
					}
					//if(*pbuf == 0)*pbuf = '0';
					if(strcmp(str, buffer) != 0 && strcmp("PI",str) != 0 && *pbuf != 0 && *str != 0){
						fprintf(stderr, "ERROR: Nombre trop long pour etre converti dans ce format: %s != %s\n", str, buffer);
						exit(2);
					}
				}
				break;
	}
}
/*A modifier*/
#define ENTRY 16
struct value *initialisation(char *argv, struct arguments *arg){
	struct value *v = NULL, *pv = NULL;
	char buffer[BUFFER], buf[BUFFER], *end,
		/*Ajouter entree*/
		*trigo[ENTRY] = {"PI", "cos", "acos", "sin", "asin", "tan", "atan", "sqrt", "exp", "ceil", "log", "log10", "fabs", "floor", "mod", "pow"};
	int i, j = 0, point = 0, wait = 0,
		parenthese = 0, o_parentheses = 0, c_parentheses = 0, split = 0, signe = 0,
		bufset = 0, count = 0, len = 0, virgule = 0, num = 0, init = 0, cont = 0;
	memset(buffer, 0, BUFFER);
	for(i = 0; argv[i] != 0; i++){
		//printf("%c\n", argv[i]);
		switch(argv[i])
		{	
			case ' ': case '\t':case '\n':
				cont++;
				continue;
			case ',':
				//if(strlen(buffer) && (pv && pv->type&(FLOAT|DOUBLE|LDOUBLE)) != 0)
					zero(buffer, pv->type);
				split = 0;
				init = 1;
				if(virgule == 0){
					ERROR("Erreur de syntaxe vers l'offset %i\n", i);
				}
				if(num == 0 && argv[i-1] != ')'){
					ERROR("Un argument est manquant vers l'offset %i\n", i);
				}
				PI_INTEGRATION(trigo[0], buffer, i-1, arg->pi);if(strlen(buffer) && (pv && pv->type&(FLOAT|DOUBLE|LDOUBLE)) != 0)
				zero(buffer, pv->type);
				num = 0;
				wait = 0;
				BUFSET(v, pv, arg->valsize, buffer, end, arg->type);
				pv->type = OPERATOR;
				pv->operator = argv[i];
				memset(buffer, 0, BUFFER);
				bufset = 0;
				virgule--;
				break;
			case '(':
				if(buffer[0] == '-'){
					buffer[1] = '1';
					BUFSET(v, pv, arg->valsize, buffer, end, arg->type);
					pv->type = '*';
				}
				split = 0;
				init = 0;
				count++;
				if(i > 0 &&
					argv[(cont == 0)?i-1:i-cont] != '+' && 
					argv[(cont == 0)?i-1:i-cont] != '/' && 
					argv[(cont == 0)?i-1:i-cont] != '*' && 
					argv[(cont == 0)?i-1:i-cont] != '+' && 
					argv[(cont == 0)?i-1:i-cont] != '-' && 
					argv[(cont == 0)?i-1:i-cont] != '(' &&
					argv[(cont == 0)?i-1:i-cont] != ',' &&
					argv[(cont == 0)?i-1:i-cont] != '\n' &&
					argv[(cont == 0)?i-1:i-cont] != '\t' &&
					argv[(cont == 0)?i-1:i-cont] != ' ' &&
					len != -1)
				{
					ERROR("Erreur de syntaxe vers l'offset %i\n", i);
				}
				cont = 0;
				if(parenthese == 1)
					parenthese = 0;
				if(v == NULL)
					v = pv = ___calloc___((void *)&v,sizeof(struct value));
				else{
					MAILLON(pv, sizeof(struct value));
				}
				if(len == -1){
					for(j = 0; j < ENTRY; j++){
						if(strcmp(buffer,trigo[j]) == 0)
							break;
					}
					pv->type = OPERATOR;
					/*Ajouter*/
					switch(j){
						case 1:
							pv->operator = COS;
							break;
						case 2:
							pv->operator = ACOS;
							break;
						case 3:
							pv->operator = SIN;
							break;
						case 4:
							pv->operator = ASIN;
							break;
						case 5:
							pv->operator = TAN;
							break;
						case 6:
							pv->operator = ATAN;
							break;
						case 7:
							pv->operator = SQRT;
							break;
						case 8:
							pv->operator = EXP;
							break;
						case 9:
							pv->operator = CEIL;
							break;
						case 10:
							pv->operator = LOG;
							break;
						case 11:
							pv->operator = LOG10;
							break;
						case 12:
							pv->operator = FABS;
							break;
						case 13:
							pv->operator = FLOOR;
							break;
						case 14:
							/*virgule pour 2 arguments*/
							pv->operator = FMOD;
							virgule++;
							break;
						case 15:
							pv->operator = POW;
							virgule++;
							break;
						default:
							ERROR("Erreur de syntaxe vers l'offset %i\n", i - (int)strlen(buffer));
					}
					len = 0;
					memset(buffer,0,BUFFER);
					MAILLON(pv, sizeof(struct value));
				}
				pv->type = O_PARENTHESE;
				pv->count = count;
				o_parentheses++;
				break;
			case ')':
				split = 0;
				cont = 0;
				init = 0;
				for(j = i-1; j > 0 && (argv[j] == ' '|| argv[j] == '\t' || argv[j] == '\n'); j--);;
				if((num == 0 && argv[j] != ')')){
					ERROR("Un argument est manquant vers l'offset %i\n", i);
				}
				num--;
				if((i == 0) || ((i > 1 && (argv[i-1] < 48 || argv[i-1] >57)) && argv[i] != ')')){
					ERROR("Erreur de syntaxe vers l'offset %i\n", i);
				}
				if(o_parentheses < c_parentheses +1){
					_ERROR_("Trop de parentheses fermees\n");
				}
				for(i = i;argv[i+1] == ' ' || argv[i+1] == '\n' || argv[i+1] == '\t'; i++);;
				if(
					argv[i+1] != '/' && 
					argv[i+1] != '*' && 
					argv[i+1] != '+' && 
					argv[i+1] != '-' && 
					argv[i+1] != ')' &&
					argv[i+1] != ',' && 
					argv[i+1] != 0
				)
				{
					ERROR("Erreur de syntaxe vers l'offset %i\n", i);
				}
				for(j = i-1; j > 0 && (argv[j] == ' '|| argv[j] == '\t' || argv[j] == '\n'); j--);;
				if(i > 0 &&	
					(
						argv[j] == '/' || 
						argv[j] == '*' || 
						argv[j] == '+' || 
						argv[j] == '-' || 
						argv[j] == '(' ||
						argv[j] == ',' || 
						argv[j] == 0
					)
				)
				{
					ERROR("Erreur de syntaxe vers l'offset %i\n", i);
				}
				PI_INTEGRATION(trigo[0], buffer, i-1, arg->pi);
				/*BUG*/
				zero(buffer,arg->type);
				wait = 0;
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
				point = 0;
				split = 0;
				if((!v || pv->type == 4 || pv->type == '+' || pv->type == '-' || pv->type == '*' || pv->type == '/')
					&& strlen(buffer) == 0){
					goto number;
				}
				if((arg->type&(FLOAT|DOUBLE|LDOUBLE)) != 0)
					/*BUG*/
					zero(buffer, arg->type);
				if(cont){
					BUFSET(v, pv,arg->valsize, buffer, end, arg->type);
					pv->type = argv[i];
					bufset = 1;
					cont = 0;
				}
				init = 1;
				//printf("*");
				for(j = i-1; j > 0 && (argv[j] == ' '|| argv[j] == '\t' || argv[j] == '\n'); j--);;
				if(buffer[0] == '-' && argv[j] == '-'){
					buffer[0] = 0;
					continue;
				}
				if((argv[j] == '(' && (argv[j-1] == '-' || argv[j-1] == '+'))){
					switch(argv[j-1]){
						case '-':
							strcpy(buffer,"-1");
							break;
						case '+':
							strcpy(buffer,"+1");
							break;
					}
					PI_INTEGRATION(trigo[0], buffer, i-1, arg->pi);
					BUFSET(v, pv, arg->valsize, buffer, end, arg->type);
					pv->type = '*';
					continue;
				}
				if(argv[j] != ',' && (strcmp(buffer,trigo[0]) != 0) && (j > 1 && (argv[j] < 48 || argv[j] >57) && argv[j] != ')'))
				{	if(argv[j] == ' ' || argv[j] == '\t' || argv[j] == '\n'){
						goto next;
					}
					/*BUG*/
					ERROR("Erreur de syntaxe vers l'offset %i\n",i);
				}
				if(argv[j] == ','){
					j++;
					while(argv[j] == ' ' || argv[j] == '\t' || argv[j] == '\n')
						j++;
					if(argv[j] == '-')
						buffer[0] = '-';
					break;
				}
				goto next;
			case '*':
			case '/':
				point = 0;
				split = 0;
				init = 1;
				if(i == 0){
					ERROR("Erreur de syntaxe vers l'offset %i\n",i);
				}
				for(j = i-1; j > 0 && (argv[j] == ' '|| argv[j] == '\t' || argv[j] == '\n'); j--);;
				if((strcmp(buffer,trigo[0]) != 0) && (j > 1 && (argv[j] < 48 || argv[j] >57) && argv[j] != ')'))
				{	if(argv[j] == ' ' || argv[j] == '\t' || argv[j] == '\n'){
						goto next;
					}
					ERROR("Erreur de syntaxe vers l'offset %i\n",i);
				}
				zero(buffer, arg->type);
				next:
				wait = 0;
				PI_INTEGRATION(trigo[0], buffer, i-1, arg->pi);
				if((arg->type&(FLOAT|DOUBLE|LDOUBLE)) != 0 && *buffer != 0)
					zero(buffer, arg->type);
				//printf("====>%s\n", buffer);
				BUFSET(v, pv, arg->valsize, buffer, end, arg->type);
				pv->type = argv[i];
				num = 0;
				cont = 0;
				break;
			case '.':
				/*Faisait bugger*/
				if(point == 1){
					ERROR("Erreur de syntaxe vers l'offset %i\n", i);
				}else	point = 1;
			default:
				if((argv[i] < 49 || argv[i] > 57) && argv[i] != '.' && argv[i] != ' ' && argv[i] != '\n' && argv[i] != '\t' && (
							((j = 1) && argv[i] == trigo[j-1][len])	&&
							((j = 2) && argv[i] == trigo[j-1][len]) &&
							((j = 3) && argv[i] == trigo[j-1][len]) &&
							((j = 4) && argv[i] == trigo[j-1][len]) &&
							((j = 5) && argv[i] == trigo[j-1][len]) &&
							((j = 6) && argv[i] == trigo[j-1][len]) &&
							((j = 7) && argv[i] == trigo[j-1][len]) &&
							((j = 8) && argv[i] == trigo[j-1][len]) &&
							((j = 9) && argv[i] == trigo[j-1][len]) &&
							((j = 10) && argv[i] == trigo[j-1][len]) &&
							((j = 11) && argv[i] == trigo[j-1][len]) &&
							((j = 12) && argv[i] == trigo[j-1][len]) &&
							((j = 13) && argv[i] == trigo[j-1][len]) &&
							((j = 14) && argv[i] == trigo[j-1][len]) &&
							((j = 15) && argv[i] == trigo[j-1][len]) &&
							((j = 16) && argv[i] == trigo[j-1][len])
					)
				) {
					ERROR("Erreur de syntaxe vers l'offset %i\n", i);
				}
				number:
				init = 0;
				if((split == 0 && parenthese == 0 && ((argv[i] > 47 && argv[i] < 58) || argv[i] == '.' || 
					(
							((argv[i] == '-' || argv[i] == '+') && strlen(buffer) == 0)
					)) && len == 0)
				){
					if((argv[i+1] == '\n' || argv[i+1] == '\t' || argv[i+1] == ' ') && argv[i] != '-' && argv[i] != '+')
						split = 1;
						/*ICI*/
					if(strlen(buffer)+2 > BUFFER){
						fprintf(stderr, "buffer trop court.\n");
						exit(EXIT_FAILURE);
					}
					strncat(buffer,&argv[i],1);
					if(pv){
						switch(pv->type){
							case FLOAT:
								sprintf(buf,"%f", strtof(buffer,NULL));
								if(equal(buf, buffer)){
									fprintf(stderr, "Nombre trop long pour ce format:%s != %s\n", buffer, buf);
									exit(EXIT_FAILURE);
								}
								break;
							case DOUBLE:
								sprintf(buf,"%lf", strtod(buffer,NULL));
								if(equal(buf, buffer)){
									fprintf(stderr, "Nombre trop long pour ce format: %s != %s\n", buffer, buf);
									exit(EXIT_FAILURE);
								}
								break;
							case LDOUBLE:
								sprintf(buf,"%Lf", strtold(buffer,NULL));
								if(equal(buf, buffer)){
									fprintf(stderr, "Nombre trop long pour ce format: %s != %s\n", buffer, buf);
									exit(EXIT_FAILURE);
								}
						}
					}
					num = 1;
					wait = 1;
					cont = 0;
				}else{
					signe = (buffer[0] == '-' || buffer[0] == '+') ? 1 : 0;
					if(signe == 1){
						strncat (buffer, "1", 2);
						BUFSET(v, pv, arg->valsize, buffer, end, arg->type);
						pv->type = '*';
						memset(buffer,0,BUFFER);
						signe = 0;
						wait = 0;
					}
					if(	(/*Ajouter*/
							((j = 1) && argv[i] == trigo[j-1][len])	||
							((j = 2) && argv[i] == trigo[j-1][len]) ||
							((j = 3) && argv[i] == trigo[j-1][len]) ||
							((j = 4) && argv[i] == trigo[j-1][len]) ||
							((j = 5) && argv[i] == trigo[j-1][len]) ||
							((j = 6) && argv[i] == trigo[j-1][len]) ||
							((j = 7) && argv[i] == trigo[j-1][len]) ||
							((j = 8) && argv[i] == trigo[j-1][len]) ||
							((j = 9) && argv[i] == trigo[j-1][len]) ||
							((j = 10) && argv[i] == trigo[j-1][len]) ||
							((j = 11) && argv[i] == trigo[j-1][len]) ||
							((j = 12) && argv[i] == trigo[j-1][len]) ||
							((j = 13) && argv[i] == trigo[j-1][len]) ||
							((j = 14) && argv[i] == trigo[j-1][len]) ||
							((j = 15) && argv[i] == trigo[j-1][len]) ||
							((j = 16) && argv[i] == trigo[j-1][len])
						) && wait == 0
					){	
						if(cont && len > 0){
							ERROR("Erreur de syntaxe vers l'offset %i\n", i+1);
						}
						parenthese = 1;
						buffer[len] = trigo[j-1][len];
						len++;
						if(len == (int)strlen(trigo[j-1]) && ((argv[i+1] == '(' || argv[i+1] == ' ' || argv[i+1] == '\n' || argv[i+1] == '\t')
							|| strcmp(buffer,trigo[0]) == 0)
						){
							if(strcmp(trigo[0], buffer) == 0){
								num = 1;
								parenthese = 0;
							}
							len = -1;
						}
						signe = 0;
						cont = 0;
					}else{
						ERROR("Erreur de syntaxe vers l'offset %i\n", i+1);
					}
				}
				bufset = 1;
				break;
		}
	}
	if(bufset){
		if((arg->type&(FLOAT|DOUBLE|LDOUBLE)) != 0)
			zero(buffer, arg->type);
	}
	//printf("%s\n", buffer);
	if(v){
		if(c_parentheses > o_parentheses){
			_ERROR_("Trop de parentheses fermees\n");
		}
		if(c_parentheses < o_parentheses){
			_ERROR_("Trop de parentheses ouvertes\n");
		}
		if(virgule){
			ERROR("Un argument est manquant vers l'offset %i\n", i - (int)strlen(buffer));
		}
		if(init == 1){
			ERROR("Erreur de syntax vers l'offset %i\n", i - (int)strlen(buffer));
		}
		if(parenthese){
			_ERROR_("Erreur de syntax\n");
		}
		if(bufset == 1 ){
			MAILLON(pv,sizeof(struct value) + arg->valsize);
			PI_INTEGRATION(trigo[0],buffer,i-1, arg->pi);
			INIT_BUFSET(pv,buffer,end, arg->type);
			pv->type = VALUE;
		}
	}else{
		if(virgule){
			ERROR("Un argument est manquant vers l'offset %i\n", i - (int)strlen(buffer));
		}
		if(init == 1){
			ERROR("Erreur de syntax vers l'offset %i\n", i - (int)strlen(buffer));
		}
		if(parenthese){
			_ERROR_("Erreur de syntax\n");
		}
		v = pv = ___calloc___((void **)&v,sizeof(struct value)+arg->valsize);
		PI_INTEGRATION(trigo[0],buffer,i-1, arg->pi);
		INIT_BUFSET(pv,buffer,end, arg->type);
		pv->type = VALUE;
	}
	return v;
}
struct value *calcule(struct value **v, struct function f, unsigned long int *type, void *arrondi){
	struct two_numbers two = {NULL, NULL, NULL};
	struct value *pv = *v, *ppv, *pnext, *ppnext, *pprev, *preader, *pcur, *pstart, *vdup = NULL, *pvdup;
	int o_parentheses, count;
	while(pv){
	//exit(0);
		switch(pv->type){
			case VALUE:
				break;
			case '+': case '-':
				/*modifier ici*/
				for(preader = pv; 
					preader && preader->type != '*' && preader->type != '/' && preader->type != O_PARENTHESE && 
					preader->operator != COS && preader->operator != ACOS && 
					preader->operator != SIN && preader->operator != ASIN && 
					preader->operator != TAN && preader->operator != ATAN &&
					preader->operator != SQRT && preader->operator != EXP &&
					preader->operator != CEIL && preader->operator != LOG &&
					preader->operator != LOG10 && preader->operator != FABS &&
					preader->operator != FLOOR &&
					preader->operator != FMOD && preader->operator != POW; 
					preader = preader->next);
				if(preader){
					pv = preader;
					preader = NULL;
					continue;
				}
				switch(pv->type){
					case '+':
						f.addition(pv->prev->val, pv->next->val, arrondi);
						break;
					case '-':
						f.soustraction(pv->prev->val, pv->next->val, arrondi);
						break;
				}
				pprev = pv->prev;
				pnext = pv->next->next;
				pprev->next = pnext;
				if(pnext)
					pprev->next->prev = pprev;
				free(pv->next);
				free(pv);
				pv = *v;
				continue;
			case '*': case '/':
				/*modifier ici*/
				for(preader = pv;
					preader != NULL && preader->type != O_PARENTHESE && 
					preader->operator != COS && preader->operator != ACOS &&
					preader->operator != SIN && preader->operator != ASIN && 
					preader->operator != TAN && preader->operator != ATAN &&
					preader->operator != SQRT && preader->operator != EXP &&
					preader->operator != CEIL && preader->operator != LOG &&
					preader->operator != LOG10 && preader->operator != FABS &&
					preader->operator != FLOOR &&
					preader->operator != FMOD && preader->operator != POW;
					preader = preader->next);
				if(preader){
					pv = preader;
					preader = NULL;
					continue;
				}
				switch(pv->type){
					case '*':
						//printf("%f\n", *((float *)arrondi));
						//exit(0);
						f.multiplication(pv->prev->val, pv->next->val, arrondi);
						break;
					case '/':
						switch(*type){
							case FLOAT:
								if(*((float *)pv->next->val) == 0){
									fprintf(stderr,"Division par 0 detectee");
									return NULL;
								}
								break;
							case DOUBLE:
								if(*((double *)pv->next->val) == 0){
									fprintf(stderr,"Division par 0 detectee");
									return NULL;
								}
								break;
							case LDOUBLE:
								if(*((long double *)pv->next->val) == 0){
									fprintf(stderr,"Division par 0 detectee");
									return NULL;
								}
								break;
						}
						f.division(pv->prev->val, pv->next->val, arrondi);
						break;
				}
				pprev = pv->prev;
				pnext = pv->next->next;
				pprev->next = pnext;
				if(pnext)
					pprev->next->prev = pprev;
				free(pv->next);
				free(pv);
				pv = *v;
				continue;
			case O_PARENTHESE:
				SINGLE_ARG(pv,o_parentheses != 0, type, arrondi);
				pv = *v;
				continue;
			case C_PARENTHESE:
				break;
			default:/*Ajouter une entree*/
				switch(pv->operator){
					case COS:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type, arrondi);
						f.cosinus(pv->val);
						pv = *v;
						continue;
					case ACOS:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type, arrondi);
						f.acosinus(pv->val);
						pv = *v;
						continue;
					case SIN:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type, arrondi);
						f.sinus(pv->val);
						pv = *v;
						continue;
					case ASIN:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type, arrondi);
						f.asinus(pv->val);
						pv = *v;
						continue;
					case TAN:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type, arrondi);
						f.tangente(pv->val);
						pv = *v;
						continue;
					case ATAN:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type, arrondi);
						f.atangente(pv->val);
						pv = *v;
						continue;
					case SQRT:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type, arrondi);
						f.sqrt(pv->val);
						pv = *v;
						continue;
					case EXP:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type, arrondi);
						f.exp(pv->val);
						pv = *v;
						continue;
					case CEIL:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type, arrondi);
						f.ceil(pv->val);
						pv = *v;
						continue;
					case LOG:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type, arrondi);
						f.log(pv->val);
						pv = *v;
						continue;
					case LOG10:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type, arrondi);
						f.log10(pv->val);
						pv = *v;
						continue;
					case FABS:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type, arrondi);
						f.fabs(pv->val);
						pv = *v;
						continue;
					case FLOOR:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type, arrondi);
						f.floor(pv->val);
						pv = *v;
						continue;
					case FMOD:
						MULTIPLE_ARGS(preader, pv, pcur, o_parentheses, pnext, pprev, two.start, two.virgule, two.end, type, arrondi);
						f.mod(pprev->val,pnext->val);
						UPDATE(v, ppv,pprev, pnext, two.start, two.virgule, two.end);
						pv = *v;
						continue;
					case POW:
						MULTIPLE_ARGS(preader, pv, pcur, o_parentheses, pnext, pprev, two.start, two.virgule, two.end, type, arrondi);
						f.power(pprev->val,pnext->val);
						UPDATE(v, ppv,pprev, pnext, two.start, two.virgule, two.end);
						pv = *v;
						continue;
				}

		}
		pv = pv->next;
	}
	return *v;
}
void calcule_destroy(struct value *v, char *format, void (*print)(void *, char*)){
	struct value *pv = v;
	/*unsigned long int count = 0;*/
	while(pv){
		v = pv->next;
		/*if(pv->type == O_PARENTHESE)
			printf("(");*/
		if(pv->type == VALUE && print != NULL)
			print(pv->val,format);
		/*if(pv->type == OPERATOR)
			printf("\noperator:%i:%c\n",pv->operator, pv->operator);
		if(pv->type == C_PARENTHESE)
			printf(")");*/
		free(pv);
		pv = v;
		/*count++;*/
	}
	/*if(count > 1){
		printf("\nPlease Report a bug.\n");
		exit(EXIT_FAILURE);
	}*/
}
int main(int argc, char **argv){
	long double ldpi = (long double) M_PI, ld = 0;
	double dpi = (double) M_PI, d = 0;
	float fpi = (float) M_PI;
	float f = 0;
	/*Modifier ici*/
	struct arguments arg = {NULL, NULL,FLOAT, 0, sizeof(float), &print_float, "%f", NULL, NULL, {{NULL,0}},
		/*Ajouter fonction*/
		{&faddition,&fsoustraction,&fmultiplication, &fdivision, &fcosinus,&facosinus, &fsinus, &fasinus, &ftangente, &fatangente,
		&fsqrt, &fexp, &fceil, &flog, &flog10, &ffabs, &ffloor,
		&ffmod, &fpower}};
	void *ptr = NULL;
	char format[56], *r;
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
				arg.pi = &dpi;
				break;
			case LDOUBLE:
				ldpi = strtold(arg.pi, &r);
				arg.pi = &ldpi;
				break;
		}
	}
	if((arg.options&V_PI) == V_PI){
		printf("%.48Lf\n", ldpi);
		exit(EXIT_SUCCESS);
	}
	if(arg.mmap.mmap)
		arg.argv = arg.mmap.mmap;
	if(arg.argv)
		arg.v = initialisation(arg.argv, &arg);
	if(arg.v){
		if(arg.arrondi == NULL){
			switch(arg.type){
				case FLOAT:
					arg.arrondi = &f;
					break;
				case DOUBLE:
					arg.arrondi = &d;
					break;
				case LDOUBLE:
					arg.arrondi = &ld;
					break;
			}
		}
		//printf("******%f\n", *((float *)arg.arrondi));
		//exit(0)
		ptr = calcule(&arg.v, arg.fn, &arg.type, arg.arrondi);
	}
	if(ptr == NULL)
		calcule_destroy(arg.v, format, NULL);
	else	calcule_destroy(arg.v, format, arg.print);
	printf("\n");
	return 0;
}
