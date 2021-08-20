#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "parsearg.h"

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "calcule-data.h"

#define BUFFER 38

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
	LDOUBLE = 3,
	CHAR = 4,
	UCHAR = 5,
	SHORT = 6,
	USHORT = 7,
	INT = 8,
	UINT = 9,
	LINT = 10,
	LUINT = 11,
	LLINT = 12,
	LLUINT = 13
};
enum VALUE_TYPE{
	VALUE = 1,
	OPERATOR = 2,
	O_PARENTHESE = 3,
	C_PARENTHESE = 4,
	AND = 5,
	OR = 6,
	XOR = 7,
	L_MOVE = 8,
	R_MOVE = 9,
	COMP = '~',
	ADD = '+',
	LESS = '-',
	MULT = '*',
	DIV = '/',
};
enum OPERATOR{
	COS = 1, /*fait*/
	SIN = 2, /*fait*/
	TAN = 3, /*fait*/
	ACOS = 4, /*fait*/
	ASIN = 5, /*fait*/
	ATAN = 6, /*fait*/
	SQRT = 7, /*fait*/
	EXP = 8, /*fait*/
	LOG = 9, /*fait*/
	LOG10 = 10,/*fait*/
	FABS = 11, /*fait*/
	CEIL = 12, /*fait*/
	FLOOR = 13,/*fait*/
	POW = 14, /*fait*/
	FMOD = 15 /*fait*/
};
enum TYPE{
	FORMAT   = 1,
	U_PI	 = 2,
	V_PI	 = 4
};
/*Ajouter fonction*/
struct function{
	void (*complement)(void *);
	void (*and)(void *, void *);
	void (*or)(void *, void *);
	void (*xor)(void *, void *);
	void (*l_move)(void *,void *);
	void (*r_move)(void *, void *);
	void (*addition)(void *, void *);
	void (*soustraction)(void *, void *);
	void (*multiplication)(void *, void *);
	void (*division)(void *, void *);
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
	struct arguments *a = state->input;
	struct stat stat;
	unsigned long int i;
	switch(key){
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
		case 'T':
			a->print = print_ullint;
			a->valsize = sizeof(unsigned long long int);
			a->fn.complement = &ullicomplement;
			a->fn.l_move = &ullil_move;
			a->fn.r_move = &ullir_move;
			a->fn.and = &ulliand;
			a->fn.or = &ullior;
			a->fn.xor = &ullixor;
			a->fn.addition = &ulliaddition;
			a->fn.soustraction = &ullisoustraction;
			a->fn.multiplication = &ullimultiplication;
			a->fn.division = &ullidivision;
			a->fn.cosinus = NULL;
			a->fn.acosinus = NULL;
			a->fn.sinus = NULL;
			a->fn.asinus = NULL;
			a->fn.tangente = NULL;
			a->fn.tangente = NULL;
			a->fn.sqrt = NULL;
			a->fn.exp = NULL;
			a->fn.ceil = NULL;
			a->fn.log = NULL;
			a->fn.log10 = NULL;
			a->fn.fabs = NULL;
			a->fn.floor = NULL;
			a->fn.mod = &ullifmod;
			a->fn.power = NULL;
			a->type = LLUINT;
			break;
		case 't':
			a->print = print_llint;
			a->valsize = sizeof(long long int);
			a->fn.complement = &llicomplement;
			a->fn.l_move = &llil_move;
			a->fn.r_move = &llir_move;
			a->fn.and = &lliand;
			a->fn.or = &llior;
			a->fn.xor = &llixor;
			a->fn.addition = &lliaddition;
			a->fn.soustraction = &llisoustraction;
			a->fn.multiplication = &llimultiplication;
			a->fn.division = &llidivision;
			a->fn.cosinus = NULL;
			a->fn.acosinus = NULL;
			a->fn.sinus = NULL;
			a->fn.asinus = NULL;
			a->fn.tangente = NULL;
			a->fn.tangente = NULL;
			a->fn.sqrt = NULL;
			a->fn.exp = NULL;
			a->fn.ceil = NULL;
			a->fn.log = NULL;
			a->fn.log10 = NULL;
			a->fn.fabs = NULL;
			a->fn.floor = NULL;
			a->fn.mod = &llifmod;
			a->fn.power = NULL;
			a->type = LLINT;
			break;
		case 'L':
			a->print = print_ulint;
			a->valsize = sizeof(unsigned long int);
			a->fn.complement = &ulicomplement;
			a->fn.l_move = &ulil_move;
			a->fn.r_move = &ulir_move;
			a->fn.and = &uliand;
			a->fn.or = &ulior;
			a->fn.xor = &ulixor;
			a->fn.addition = &uliaddition;
			a->fn.soustraction = &ulisoustraction;
			a->fn.multiplication = &ulimultiplication;
			a->fn.division = &ulidivision;
			a->fn.cosinus = NULL;
			a->fn.acosinus = NULL;
			a->fn.sinus = NULL;
			a->fn.asinus = NULL;
			a->fn.tangente = NULL;
			a->fn.tangente = NULL;
			a->fn.sqrt = NULL;
			a->fn.exp = NULL;
			a->fn.ceil = NULL;
			a->fn.log = NULL;
			a->fn.log10 = NULL;
			a->fn.fabs = NULL;
			a->fn.floor = NULL;
			a->fn.mod = &ulifmod;
			a->fn.power = NULL;
			a->type = LUINT;
			break;
		case 'l':
			a->print = print_lint;
			a->valsize = sizeof(long int);
			a->fn.complement = &licomplement;
			a->fn.l_move = &lil_move;
			a->fn.r_move = &lir_move;
			a->fn.and = &liand;
			a->fn.or = &lior;
			a->fn.xor = &lixor;
			a->fn.addition = &liaddition;
			a->fn.soustraction = &lisoustraction;
			a->fn.multiplication = &limultiplication;
			a->fn.division = &lidivision;
			a->fn.cosinus = NULL;
			a->fn.acosinus = NULL;
			a->fn.sinus = NULL;
			a->fn.asinus = NULL;
			a->fn.tangente = NULL;
			a->fn.tangente = NULL;
			a->fn.sqrt = NULL;
			a->fn.exp = NULL;
			a->fn.ceil = NULL;
			a->fn.log = NULL;
			a->fn.log10 = NULL;
			a->fn.fabs = NULL;
			a->fn.floor = NULL;
			a->fn.mod = &lifmod;
			a->fn.power = NULL;
			a->type = LINT;
			break;
		case 'I':
			a->print = print_uint;
			a->valsize = sizeof(unsigned int);
			a->fn.complement = &uicomplement;
			a->fn.l_move = &uil_move;
			a->fn.r_move = &uir_move;
			a->fn.and = &uiand;
			a->fn.or = &uior;
			a->fn.xor = &uixor;
			a->fn.addition = &uiaddition;
			a->fn.soustraction = &uisoustraction;
			a->fn.multiplication = &uimultiplication;
			a->fn.division = &uidivision;
			a->fn.cosinus = NULL;
			a->fn.acosinus = NULL;
			a->fn.sinus = NULL;
			a->fn.asinus = NULL;
			a->fn.tangente = NULL;
			a->fn.tangente = NULL;
			a->fn.sqrt = NULL;
			a->fn.exp = NULL;
			a->fn.ceil = NULL;
			a->fn.log = NULL;
			a->fn.log10 = NULL;
			a->fn.fabs = NULL;
			a->fn.floor = NULL;
			a->fn.mod = &uifmod;
			a->fn.power = NULL;
			a->type = UINT;
			break;
		case 'i':
			a->print = print_int;
			a->valsize = sizeof(int);
			a->fn.complement = &icomplement;
			a->fn.l_move = &il_move;
			a->fn.r_move = &ir_move;
			a->fn.and = &iand;
			a->fn.or = &ior;
			a->fn.xor = &ixor;
			a->fn.addition = &iaddition;
			a->fn.soustraction = &isoustraction;
			a->fn.multiplication = &imultiplication;
			a->fn.division = &idivision;
			a->fn.cosinus = NULL;
			a->fn.acosinus = NULL;
			a->fn.sinus = NULL;
			a->fn.asinus = NULL;
			a->fn.tangente = NULL;
			a->fn.tangente = NULL;
			a->fn.sqrt = NULL;
			a->fn.exp = NULL;
			a->fn.ceil = NULL;
			a->fn.log = NULL;
			a->fn.log10 = NULL;
			a->fn.fabs = NULL;
			a->fn.floor = NULL;
			a->fn.mod = &ifmod;
			a->fn.power = NULL;
			a->type = INT;
			break;
		case 's':
			a->print = print_short;
			a->valsize = sizeof(short int);
			a->fn.complement = &scomplement;
			a->fn.l_move = &sl_move;
			a->fn.r_move = &sr_move;
			a->fn.and = &sand;
			a->fn.or = &sor;
			a->fn.xor = &sxor;
			a->fn.addition = &saddition;
			a->fn.soustraction = &ssoustraction;
			a->fn.multiplication = &smultiplication;
			a->fn.division = &sdivision;
			a->fn.cosinus = NULL;
			a->fn.acosinus = NULL;
			a->fn.sinus = NULL;
			a->fn.asinus = NULL;
			a->fn.tangente = NULL;
			a->fn.tangente = NULL;
			a->fn.sqrt = NULL;
			a->fn.exp = NULL;
			a->fn.ceil = NULL;
			a->fn.log = NULL;
			a->fn.log10 = NULL;
			a->fn.fabs = NULL;
			a->fn.floor = NULL;
			a->fn.mod = &sfmod;
			a->fn.power = NULL;
			a->type = SHORT;
			break;
		case 'S':
			a->print = print_ushort;
			a->valsize = sizeof(short int);
			a->fn.complement = &uscomplement;
			a->fn.l_move = &usl_move;
			a->fn.r_move = &usr_move;
			a->fn.and = &usand;
			a->fn.or = &usor;
			a->fn.xor = &usxor;
			a->fn.addition = &usaddition;
			a->fn.soustraction = &ussoustraction;
			a->fn.multiplication = &usmultiplication;
			a->fn.division = &usdivision;
			a->fn.cosinus = NULL;
			a->fn.acosinus = NULL;
			a->fn.sinus = NULL;
			a->fn.asinus = NULL;
			a->fn.tangente = NULL;
			a->fn.tangente = NULL;
			a->fn.sqrt = NULL;
			a->fn.exp = NULL;
			a->fn.ceil = NULL;
			a->fn.log = NULL;
			a->fn.log10 = NULL;
			a->fn.fabs = NULL;
			a->fn.floor = NULL;
			a->fn.mod = &usfmod;
			a->fn.power = NULL;
			a->type = USHORT;
			break;
		case 'c':
			a->print = print_char;
			a->valsize = sizeof(char);
			a->fn.complement = &ccomplement;
			a->fn.l_move = &cl_move;
			a->fn.r_move = &cr_move;
			a->fn.and = &cand;
			a->fn.or = &cor;
			a->fn.xor = &cxor;
			a->fn.addition = &caddition;
			a->fn.soustraction = &csoustraction;
			a->fn.multiplication = &cmultiplication;
			a->fn.division = &cdivision;
			a->fn.cosinus = NULL;
			a->fn.acosinus = NULL;
			a->fn.sinus = NULL;
			a->fn.asinus = NULL;
			a->fn.tangente = NULL;
			a->fn.tangente = NULL;
			a->fn.sqrt = NULL;
			a->fn.exp = NULL;
			a->fn.ceil = NULL;
			a->fn.log = NULL;
			a->fn.log10 = NULL;
			a->fn.fabs = NULL;
			a->fn.floor = NULL;
			a->fn.mod = &cfmod;
			a->fn.power = NULL;
			a->type = CHAR;
			break;
		case 'C':
			a->print = print_uchar;
			a->valsize = sizeof(char);
			a->fn.complement = &uccomplement;
			a->fn.l_move = &ucl_move;
			a->fn.r_move = &ucr_move;
			a->fn.and = &ucand;
			a->fn.or = &ucor;
			a->fn.xor = &ucxor;
			a->fn.addition = &ucaddition;
			a->fn.soustraction = &ucsoustraction;
			a->fn.multiplication = &ucmultiplication;
			a->fn.division = &ucdivision;
			a->fn.cosinus = NULL;
			a->fn.acosinus = NULL;
			a->fn.sinus = NULL;
			a->fn.asinus = NULL;
			a->fn.tangente = NULL;
			a->fn.tangente = NULL;
			a->fn.sqrt = NULL;
			a->fn.exp = NULL;
			a->fn.ceil = NULL;
			a->fn.log = NULL;
			a->fn.log10 = NULL;
			a->fn.fabs = NULL;
			a->fn.floor = NULL;
			a->fn.mod = &ucfmod;
			a->fn.power = NULL;
			a->type = UCHAR;
			break;
		case 'f':
			a->print = print_float;
			a->valsize = sizeof(float);
			a->fn.complement = NULL;
			a->fn.l_move = NULL;
			a->fn.r_move = NULL;
			a->fn.and = NULL;
			a->fn.or = NULL;
			a->fn.xor = NULL;
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
		case 'd':
			a->print = print_double;
			a->valsize = sizeof(double);
			a->fn.complement = NULL;
			a->fn.l_move = NULL;
			a->fn.r_move = NULL;
			a->fn.and = NULL;
			a->fn.or = NULL;
			a->fn.xor = NULL;
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
		case 'D':
			a->print = print_ldouble;
			a->valsize = sizeof(long double);
			a->fn.complement = NULL;
			a->fn.l_move = NULL;
			a->fn.r_move = NULL;
			a->fn.and = NULL;
			a->fn.or = NULL;
			a->fn.xor = NULL;
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
}
struct info program = {"version: 1.0","zoeurk@gmail.com"};
struct parser_option options[] =	{
					{ "float", 'f', 0, NULL, "Utiliser le type float"},
					{ "double", 'd', 0 , NULL, "Utiliser le type double"},
					{ "ldouble", 'D', 0, NULL, "Utiliser le type long double"},
					{ "char", 'c', 0, NULL, "Utiliser le type char"},
					{ "unsigned-char", 'C', 0, NULL, "Utiliser le type char"},
					{ "short",'s', 0, NULL, "Utiliser le type short int"},
					{ "unsigned-short",'S', 0, NULL, "Utiliser le type short int"},
					{ "int" ,'i', 0, NULL, "Utiliser le tye int"},
					{ "unsigned-int" ,'I', 0, NULL, "Utiliser le tye int"},
					{ "long-int" ,'l', 0, NULL, "Utiliser le tye long int"},
					{ "unsigned-long-int" ,'L', 0, NULL, "Utiliser le tye unsigned long int"},
					{ "long-long-int" ,'t', 0, NULL, "Utiliser le tye long long int"},
					{ "unsigned-long-long-int" ,'T', 0, NULL, "Utiliser le tye unsigned long long int"},
					{ "format", 'O', 0, "FORMAT", "Nombre de chiffre apres la virgule"},
					{ "file", 'F', 0, "FILE", "lire le fichier"},
					{ "set-pi", 'p', 0, "PI", "Initialiser pi"},
					{ "view-pi", 'P', 0, NULL, "Voir la valeur de pi par default"},
					{0}
				};

struct parser_state ps = {NULL, NULL, 0, 0, 0, 0, 0, 35, 75, 75, NULL, NULL, NULL, NULL};
struct parser args = {options, arguments, "[OPTIONS]", "[\"calcule\"]", "Petit outil de calcule", &program, &ps};

void *___calloc___(void **ptr, unsigned long int size){
	if((*ptr = calloc(1, size)) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	return *ptr;
}
/*A modifier*/
#define ENTRY 16
#define TEMP 16
#define MODULO 15
#define RACINE_CARRE 8
struct value *v = NULL;
struct value *initialisation(char *argv, struct arguments *arg){
	struct value *pv = NULL, *pprev, *pnext;
	char buffer[BUFFER], temp[TEMP], *end,
		/*Ajouter entree*/
		*trigo[ENTRY] = {"PI", "cos", "acos", "sin", "asin", "tan", "atan", "sqrt", "exp", "ceil", "log", "log10", "fabs", "floor", "mod", "pow"},
		*operator[5] = {"<<", ">>", "AND", "OR", "XOR"};
	int i, j = 0, point = 0, wait = 0,
		parenthese = 0, o_parentheses = 0, c_parentheses = 0, split = 0, signe = 0,
		bufset = 0, count = 0, len = 0, virgule = 0, num = 0, init = 0, cont = 0;
	memset(buffer, 0, BUFFER);
	memset(temp ,0, TEMP);
	for(i = 0; argv[i] != 0; i++){
		switch(argv[i])
		{	
			case ' ': case '\t':case '\n':
				cont++;
				continue;
			case ',':
				split = 0;
				init = 1;
				if(virgule == 0){
					ERROR("Erreur de syntaxe vers l'offset %i\n", i);
				}
				if(num == 0 && argv[i-1] != ')'){
					ERROR("Un argument est manquant vers l'offset %i\n", i);
				}
				PI_INTEGRATION(trigo[0], buffer, i-1, arg->pi);
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
					argv[(cont == 0)?i-1:i-cont] != '~' &&
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
			case '~':
				if(arg->type < CHAR){
					fprintf(stderr, "Erreur: Operande dans ce mode.\n");
					exit(EXIT_FAILURE);
				}
				if(pv && pv->type == '~'){
					pprev = pv->prev;
					pnext = pv->next;
					if(pprev){
						pv = pv->prev;
						free(pv->next);
						pv->next = NULL;
					}else{
						free(v);
						v = NULL;
						pv = v;
					}
				}else{
					if(v == NULL)
						v = pv = ___calloc___((void **)&v,sizeof(struct value)+arg->valsize);
					else{
						MAILLON(pv, sizeof(struct value));
					}
					pv->type = '~';
				}
				break;
			case '+':
			case '-':
				//printf("%s;\n", buffer);
				/*BUG*/
				point = 0;
				split = 0;
				if((!v || pv->type == '~'|| pv->type == O_PARENTHESE || pv->type == '+' || pv->type == '-' || pv->type == '*' || pv->type == '/')
					&& strlen(buffer) == 0){
					goto number;
				}
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
				if((strncmp(&argv[j-1],">>",2) && strncmp(&argv[j-1],"<<",2) &&
					strncmp(&argv[j-2], "AND",3) && strncmp(&argv[j-1],"OR",2) && strncmp(&argv[j-2],"XOR",3)) &&
					(argv[j] != ',' && (strcmp(buffer,trigo[0]) != 0) && (j > 1 && (argv[j] < 48 || argv[j] >57) && argv[j] != ')'))
				){
					//printf("%c\n", argv[j-2]);
					if(argv[j] == ' ' || argv[j] == '\t' || argv[j] == '\n'){
						goto next;
					}
					/*BUG*/
					fprintf(stderr,"%s\n", &argv[j-1]);
					ERROR(">>>Erreur de syntaxe vers l'offset %i\n",i);
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
				next:
				wait = 0;
				PI_INTEGRATION(trigo[0], buffer, i-1, arg->pi);
				BUFSET(v, pv, arg->valsize, buffer, end, arg->type);
				pv->type = argv[i];
				num = 0;
				cont = 0;
				break;
			case '.':
				if(arg->type >= CHAR){
					fprintf(stderr,"Erreur: Virgule dans un calcule binaire\n");
					exit(EXIT_FAILURE);
				}
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
				){	if((arg->type == UCHAR || arg->type == UINT || arg->type == LUINT || arg->type == LLUINT) && (argv[i] == '-' || argv[i] == '+')){
						fprintf(stderr, "WARNING: Chiffre signe pour un type non signe\n");
						//exit(EXIT_FAILURE);
					}
					if((argv[i+1] == '\n' || argv[i+1] == '\t' || argv[i+1] == ' ') && argv[i] != '-' && argv[i] != '+')
						split = 1;
					if(strlen(buffer)+2 > BUFFER){
						fprintf(stderr, "buffer trop court: 56 octets.\n");
						exit(EXIT_FAILURE);
					}
					strncat(buffer,&argv[i],1);
					num = 1;
					wait = 1;
					cont = 0;
				}else{ if(pv && pv->type == OPERATOR)
					{
						if(strlen(buffer) == 0){
							ERROR("Erreur de synntaxe vers l'offset %i\n", i);
						}
						//printf("===>%s\n", buffer);
					}
					//printf("%li\n",arg->type);
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
							if(arg->type >= CHAR && j != MODULO){
								//fprintf(stderr,"%i\n", j);
								fprintf(stderr,"Erreur: fonction '%s' non valide dans ce mode\n", buffer);
								exit(EXIT_FAILURE);
							}
							if(strcmp(trigo[0], buffer) == 0){
								num = 1;
								parenthese = 0;
							}
							len = -1;
						}
						signe = 0;
						cont = 0;
					}else{
							if(	((j = 1) && argv[i] == operator[j-1][len]) ||
								((j = 2) && argv[i] == operator[j-1][len]) ||
								((j = 3) && argv[i] == operator[j-1][len]) ||
								((j = 4) && argv[i] == operator[j-1][len]) ||
								((j = 5) && argv[i] == operator[j-1][len])
							){
								//printf("%s:%c\n", temp, argv[i]);
								if(arg->type < CHAR){
									fprintf(stderr,"Erreur: Operateur binaire non supporte dans ce mode\n");
									exit(EXIT_FAILURE);
								}
								temp[len] = argv[i];
								//printf("==>%s\n",temp);
								if(strncmp(operator[j-1],temp, strlen(operator[j-1])) == 0){
									if(strlen(buffer) == 0){
										ERROR("Erreur de syntaxe vers l'offset %i\n", i);
									}
									BUFSET(v, pv,arg->valsize, buffer, end, arg->type);
									switch(j)
									{
										case 1:
											pv->type = L_MOVE;
											break;
										case 2:
											pv->type = R_MOVE;
											break;
										case 3:
											pv->type = AND;
											break;
										case 4:
											pv->type = OR;
											break;
										case 5:
											pv->type = XOR;
											break;
									}
									//printf("%s\n", buffer);
									memset(temp, 0, TEMP);
									len = 0;
									bufset = 1;
									//temp[0] = 0;
									//buffer[0] = 0;
									//if(argv[i+1] == ' ' || argv[i+1] == '\t' || argv[i+1] == '\n'){
										//printf("==========================\n");
										for(i = i+1; argv[i] == ' ' || argv[i] == '\t' || argv[i] == '\n'; i++);;
									//}
									//Modifier ici
									i-=1;
									//if(argv[i] == '('){
									//	printf("parenthese\n");
									//}
									split = 0;
									//exit(0);
									continue;
								}
							}else{
								
								ERROR("Erreur de syntaxe vers l'offset %i\n", i-(int)strlen(temp));
							}
							len++;
						//ERROR("Erreur de syntaxe vers l'offset %i\n", i+1);
					}
				}
				bufset = 1;
				break;
		}
	}
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
		/*if(pv && pv->type == 2 && (pv->operator == AND || pv->operator == OR || pv->operator == XOR || pv->operator == L_MOVE || pv->operator == R_MOVE))
			printf("<%s>\n", buffer);*/
		if(bufset == 1){
			if(strlen(buffer) == 0){
				ERROR("Erreur de syntax vers l'offset: %i\n",i);
			}
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
	/*pv = v;
	while(pv){
		printf("%i", pv->type);
		if(pv->type == 1)
			printf(":%i\n", *((int *)pv->val));
		else	printf("\n");
		pv = pv->next;
	}
	exit(0);
	while(pv){
		printf("%i\n", pv->type);
		pv = pv->next;
	}
	printf("===\n");*/
	//exit(0);
	return v;
}
struct value *calcule(struct value **v, struct function f, unsigned long int *type){
	struct two_numbers two = {NULL, NULL, NULL};
	struct value *pv = *v, *ppv, *pnext, *ppnext, *pprev, *ppprev, *preader, *pcur, *pstart, *vdup = NULL, *pvdup, *ptest;
	int o_parentheses, count;
	while(pv){
		switch(pv->type){
			case VALUE:
				break;
			case '+': case '-':
				/*modifier ici*/
				for(preader = pv; 
					preader && preader->type != '~' && preader->type != '*'  && preader->type != '/' && preader->type != O_PARENTHESE && 
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
						f.addition(pv->prev->val, pv->next->val);
						break;
					case '-':
						//if(pv->prev == NULL){
						//	printf("GRRRR\n");
						//	exit(0); 
						//}
						switch(*type){
							case UCHAR:
								if(*((unsigned char *)pv->prev->val) < *((unsigned char *)pv->next->val)){
									fprintf(stderr, "WARNING: Resultat signe dans un type non signe\n");
								}
								break;
							case USHORT:
								if(*((unsigned short int *)pv->prev->val) < *((unsigned short int *)pv->next->val)){
									fprintf(stderr, "WARNING: Resultat signe dans un type non signe\n");
								}
								break;
							case UINT:
								if(*((unsigned int *)pv->prev->val) < *((unsigned int *)pv->next->val)){
									fprintf(stderr, "WARNING: Resultat signe dans un type non signe\n");
								}
								break;
							case LUINT:
								if(*((unsigned long int *)pv->prev->val) < *((unsigned long int *)pv->next->val)){
									fprintf(stderr, "WARNING: Resultat signe dans un type non signe\n");
								}
								break;
							case LLUINT:
								if(*((unsigned long long int *)pv->prev->val) < *((unsigned long long int *)pv->next->val)){
									fprintf(stderr, "WARNING: Resultat signe dans un type non signe\n");
								}
								break;
						}
						f.soustraction(pv->prev->val, pv->next->val);
						break;
				}
				SUIVANT;
				/*pprev = pv->prev;
				pnext = pv->next->next;
				pprev->next = pnext;
				if(pnext)
					pprev->next->prev = pprev;
				free(pv->next);
				free(pv);
				pv = *v;*/
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
						f.multiplication(pv->prev->val, pv->next->val);
						break;
					case '/':
						//printf("==>%i\n", *((int *)pv->next->val));
						switch(*type){
							case CHAR:
								if(*((char *)pv->next->val) == 0){
									fprintf(stderr,"Division par 0 detectee\n");
									exit(EXIT_FAILURE);
								}
								break;
							case UCHAR:
								if(*((unsigned char *)pv->next->val) == 0){
									fprintf(stderr,"Division par 0 detectee\n");
									exit(EXIT_FAILURE);
								}
								break;
							case SHORT:
								if(*((short int *)pv->next->val) == 0){
									fprintf(stderr,"Division par 0 detectee\n");
									exit(EXIT_FAILURE);
								}
								break;
							case USHORT:
								if(*((unsigned short int *)pv->next->val) == 0){
									fprintf(stderr,"Division par 0 detectee\n");
									exit(EXIT_FAILURE);
								}
								break;
							case INT:
								if(*((int *)pv->next->val) == 0){
									fprintf(stderr,"Division par 0 detectee\n");
									exit(EXIT_FAILURE);
								}
								break;
							case UINT:
								if(*((unsigned int *)pv->next->val) == 0){
									fprintf(stderr,"Division par 0 detectee\n");
									exit(EXIT_FAILURE);
								}
								break;
							case LINT:
								if(*((long int *)pv->next->val) == 0){
									fprintf(stderr,"Division par 0 detectee\n");
									exit(EXIT_FAILURE);
								}
								break;
							case LUINT:
								if(*((unsigned long int *)pv->next->val) == 0){
									fprintf(stderr,"Division par 0 detectee\n");
									exit(EXIT_FAILURE);
								}
								break;
							case FLOAT:
								if(*((float *)pv->next->val) == 0){
									fprintf(stderr,"Division par 0 detectee\n");
									exit(EXIT_FAILURE);
								}
								break;
							case DOUBLE:
								if(*((double *)pv->next->val) == 0){
									fprintf(stderr,"Division par 0 detectee\n");
									exit(EXIT_FAILURE);
								}
								break;
							case LDOUBLE:
								if(*((long double *)pv->next->val) == 0){
									fprintf(stderr,"Division par 0 detectee\n");
									exit(EXIT_FAILURE);
								}
								break;
						}
						//printf("%i\n", pv->prev->type);
						//exit(0);
						f.division(pv->prev->val, pv->next->val);
						break;
				}
				SUIVANT;
				/*pprev = pv->prev;
				pnext = pv->next->next;
				pprev->next = pnext;
				if(pnext)
					pprev->next->prev = pprev;
				free(pv->next);
				free(pv);
				pv = *v;*/
				continue;
			case '~':
				if(pv->next->type == O_PARENTHESE){
					pv = calcule(&pv->next, f, type);
					if(pv->prev)
						pv = pv->prev;
				}
				f.complement(pv->next->val);
				pprev = pv->prev;
				pnext = pv->next;
				if(pprev){
					pprev->next = pnext;
					if(pnext)
						pprev->next->prev = pprev;
					free(pv);
					pv = *v;
				}else{
					pv = *v = (*v)->next;
					free((*v)->prev);
					(*v)->prev = NULL;
					pv = *v;
					
				}
				pv = *v;
				continue;
			case AND:
				for(preader = pv;
					preader != NULL && preader->type != O_PARENTHESE && 
					preader->type != '+' && preader->type != '-' &&
					preader->type != '*' && preader->type != '/' &&
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
				f.and(pv->prev->val, pv->next->val);
				SUIVANT;
				/*pprev = pv->prev;
				pnext = pv->next->next;
				pprev->next = pnext;
				if(pnext)
					pprev->next->prev = pprev;
				free(pv->next);
				free(pv);
				pv = *v;*/
				continue;
			case OR:
				for(preader = pv;
					preader != NULL && preader->type != O_PARENTHESE && 
					preader->type != '+' && preader->type != '-' &&
					preader->type != '*' && preader->type != '/' &&
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
				f.or(pv->prev->val, pv->next->val);
				SUIVANT;
				/*pprev = pv->prev;
				pnext = pv->next->next;
				pprev->next = pnext;
				if(pnext)
					pprev->next->prev = pprev;
				free(pv->next);
				free(pv);
				pv = *v;*/
				continue;
			case XOR:
				for(preader = pv;
					preader != NULL && preader->type != O_PARENTHESE && 
					preader->type != '+' && preader->type != '-' &&
					preader->type != '*' && preader->type != '/' &&
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
				f.xor(pv->prev->val, pv->next->val);
				SUIVANT;
				/*pprev = pv->prev;
				pnext = pv->next->next;
				pprev->next = pnext;
				if(pnext)
					pprev->next->prev = pprev;
				free(pv->next);
				free(pv);
				pv = *v;*/
				continue;
			case L_MOVE:
				for(preader = pv;
					preader != NULL && preader->type != O_PARENTHESE && 
					preader->type != '+' && preader->type != '-' &&
					preader->type != '*' && preader->type != '/' &&
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
				f.l_move(pv->prev->val, pv->next->val);
				SUIVANT;
				/*pprev = pv->prev;
				pnext = pv->next->next;
				pprev->next = pnext;
				if(pnext)
					pprev->next->prev = pprev;
				free(pv->next);
				free(pv);
				pv = *v;*/
				continue;
			case R_MOVE:
				for(preader = pv;
					preader != NULL && preader->type != O_PARENTHESE && 
					preader->type != '+' && preader->type != '-' &&
					preader->type != '*' && preader->type != '/' &&
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
				f.r_move(pv->prev->val, pv->next->val);
				SUIVANT;
				/*pprev = pv->prev;
				pnext = pv->next->next;
				pprev->next = pnext;
				if(pnext)
					pprev->next->prev = pprev;
				free(pv->next);
				free(pv);
				pv = *v;*/
				continue;
			case O_PARENTHESE:
				SINGLE_ARG(pv,o_parentheses != 0, type);
				pv = *v;
				continue;
			case C_PARENTHESE:
				break;
			default:/*Ajouter une entree*/
				switch(pv->operator){
					case COS:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type);
						f.cosinus(pv->val);
						pv = *v;
						continue;
					case ACOS:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type);
						f.acosinus(pv->val);
						pv = *v;
						continue;
					case SIN:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type);
						f.sinus(pv->val);
						pv = *v;
						continue;
					case ASIN:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type);
						f.asinus(pv->val);
						pv = *v;
						continue;
					case TAN:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type);
						f.tangente(pv->val);
						pv = *v;
						continue;
					case ATAN:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type);
						f.atangente(pv->val);
						pv = *v;
						continue;
					case SQRT:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type);
						f.sqrt(pv->val);
						pv = *v;
						continue;
					case EXP:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type);
						f.exp(pv->val);
						pv = *v;
						continue;
					case CEIL:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type);
						f.ceil(pv->val);
						pv = *v;
						continue;
					case LOG:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type);
						f.log(pv->val);
						pv = *v;
						continue;
					case LOG10:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type);
						f.log10(pv->val);
						pv = *v;
						continue;
					case FABS:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type);
						f.fabs(pv->val);
						pv = *v;
						continue;
					case FLOOR:
						TRIGO(o_parentheses != 0,preader, pv, o_parentheses, pnext, pprev, type);
						f.floor(pv->val);
						pv = *v;
						continue;
					case FMOD:
						MULTIPLE_ARGS(preader, pv, pcur, o_parentheses, pnext, pprev, two.start, two.virgule, two.end, type);
						f.mod(pprev->val,pnext->val);
						UPDATE(v, pprev, pnext, two.start, two.virgule, two.end);
						pv = *v;
						continue;
					case POW:
						MULTIPLE_ARGS(preader, pv, pcur, o_parentheses, pnext, pprev, two.start, two.virgule, two.end, type);
						f.power(pprev->val,pnext->val);
						UPDATE(v, pprev, pnext, two.start, two.virgule, two.end);
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
	unsigned long int count = 0;
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
		//free(pv);
		pv = v;
		count++;
	}
	if(count > 1){
		printf("\nPlease Report a bug.\n");
		exit(EXIT_FAILURE);
	}
}
void bye(void){
	struct value *pv;
	while(v){
		pv = v->next;
		free(v);
		v = pv;
	}
}
int main(int argc, char **argv){
	long double ldpi = (long double)  M_PI;
	double dpi = (double) M_PI;
	float fpi = (float) M_PI;
	/*Modifier ici*/
	struct arguments arg = {NULL, FLOAT, 0, sizeof(float), &print_float, "%f", &fpi, NULL, {{NULL,0}},
		/*Ajouter fonction*/
		{NULL, NULL, NULL, NULL, NULL, NULL,&faddition,&fsoustraction,&fmultiplication, &fdivision, &fcosinus,&facosinus, &fsinus, &fasinus, &ftangente, &fatangente,
		&fsqrt, &fexp, &fceil, &flog, &flog10, &ffabs, &ffloor,
		&ffmod, &fpower}};
	void *ptr = NULL;
	char format[56], *r;
	//memset(&ps,0,sizeof(struct parser_state));
	//parser_usage(&args);
	//exit(0);
	atexit(bye);
	ps.argv = argv;
	ps.parser = &args;
	/*ps.err_stream = stderr;
	ps.out_stream = stdout;*/
	parser_parse(&args, argc, argv, &arg);
	if((arg.options&FORMAT) == FORMAT){
		*format = 0;
		strcat(format,"%.");
		strcat(format,arg.format);
		switch(arg.type){
			case LLUINT:
				strcat(format,"llu");
				break;
			case LLINT:
				strcat(format,"ll");
				break;
			case LUINT:
				strcat(format,"ul");
				break;
			case LINT:
				strcat(format, "li");
				break;
			case UINT:
			case USHORT:
			case UCHAR:
				strcat(format, "u");
				break;
			case INT:
			case SHORT:
			case CHAR:
				strcat(format,"i");
				break;
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
			case LLUINT:
				strcat(format,"%llu");
				break;
			case LLINT:
				strcat(format,"%lli");
				break;
			case LUINT:
				strcat(format,"%lu");
				break;
			case LINT:
				strcat(format,"%li");
				break;
			case UINT:
			case USHORT:
			case UCHAR:
				strcat(format,"%u");
				break;
			case INT:
			case SHORT:
			case CHAR:
				strcat(format,"%i");
				break;
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
	}else{
		switch(arg.type){
			case FLOAT:
				//printf("***\n");
				//fpi = strtof(arg.pi, &r);
				fpi = (float)M_PI;
				arg.pi = &fpi;
				//printf("%f\n",fpi);
				break;
			case DOUBLE:
				//dpi = strtod(arg.pi, &r);
				dpi = (double)M_PI;
				arg.pi = &dpi;
				break;
			case LDOUBLE:
				ldpi = (long double)M_PI;
				arg.pi = &ldpi;
				//printf("%LF\n",ldpi);
				break;
		}
	}
	//printf("%LF\n", *((long double *)arg.pi));
	if((arg.options&V_PI) == V_PI){
		printf("%.48Lf\n", ldpi);
		exit(EXIT_SUCCESS);
	}
	if(arg.mmap.mmap)
		arg.argv = arg.mmap.mmap;
	if(arg.argv)
		arg.v = initialisation(arg.argv, &arg);
	if(arg.v)
		ptr = calcule(&arg.v, arg.fn, &arg.type);
	if(ptr == NULL)
		calcule_destroy(arg.v, format, NULL);
	else	calcule_destroy(arg.v, format, arg.print);
	free(arg.v);
	arg.v = v = NULL;
	printf("\n");
	return 0;
}
