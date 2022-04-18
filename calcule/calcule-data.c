#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include <math.h>*/
#include "calcule-data.h"
#include "../operation/operation.h"
#ifdef __USE_ISOC99
/* IEEE positive infinity.  */
# if __GNUC_PREREQ (3, 3)
#  define INFINITY (__builtin_inff ())
# else
#  define INFINITY HUGE_VALF
# endif

/* IEEE Not A Number.  */
# if __GNUC_PREREQ (3, 3)
#  define NAN (__builtin_nanf (""))
# else
/* This will raise an "invalid" exception outside static initializers,
   but is the best that can be done in ISO C while remaining a
   constant expression.  */
#  define NAN (0.0f / 0.0f)
# endif
#endif /* __USE_ISOC99 */

const unsigned long int BUFFER = 28;

#define CHECK_AND_COMPUT(op_final,string_type, n1, n2, op)\
	char valeur1[BUFFER], valeur2[BUFFER], valeur3[BUFFER], *resultat, *temp;\
	if(n1 != INFINITY && n1 != NAN && n2 != INFINITY && n2 != NAN){\
		sprintf(valeur1, string_type, n1);\
		sprintf(valeur2, string_type, n2);\
		resultat = op(valeur1, valeur2);\
			if((temp = strchr(resultat, '.')) && strlen(temp) > 7)\
				temp[8] = 0;\
		/**((float *)val1) *= *((float *)val2);*/\
		op_final;\
		sprintf(valeur3, string_type, n1);\
		if(equal(valeur3, resultat) != 0){\
			fprintf(stderr,"ERROR: Nombre trop long pour etre converti dans ce format:%s::%s\n", valeur3, resultat);\
			free(resultat);\
			exit(2);\
		}\
		free(resultat);\
	}else op_final;
#define CHECK_AND_COMPUT_DIVISION(op_final,string_type, n1, n2)\
	char valeur1[BUFFER], valeur2[BUFFER], valeur3[BUFFER], *resultat, *temp;\
	if(n1 != INFINITY && n1 != NAN && n2 != INFINITY && n2 != NAN){\
		sprintf(valeur1, string_type, n1);\
		sprintf(valeur2, string_type, n2);\
		resultat = division(valeur1, valeur2, 6, 0);\
		if((temp = strchr(resultat, '.')) && strlen(temp) > 7)\
			temp[8] = 0;\
		/**((float *)val1) *= *((float *)val2);*/\
		op_final;\
		sprintf(valeur3, string_type, n1);\
		if(equal(valeur3, resultat) != 0){\
			fprintf(stderr,"ERROR: Nombre trop long pour etre converti dans ce format:%s::%s\n", valeur3, resultat);\
			free(resultat);\
			exit(2);\
		}\
		free(resultat);\
	}else op_final;

void print_float(void *val, char *format){
	printf(format, *((float *)val));
}
void print_double(void *val, char *format){
	printf(format, *((double *)val));
}
void print_ldouble(void *val, char *format){
	printf(format, *((long double *)val));
}

void faddition(void *val1, void *val2){
	CHECK_AND_COMPUT(*((float *)val1) += *((float *)val2),"%f", *((float *)val1), *((float *)val2),addition);
	//*((float *)val1) += *((float *) val2);
}
void daddition(void *val1, void *val2){
	CHECK_AND_COMPUT(*((double *)val1) += *((double*)val2),"%lf", *((double *)val1), *((double *)val2),addition);
	//*((double *)val1) += *((double *) val2);
}
void ldaddition(void *val1, void *val2){
	CHECK_AND_COMPUT(*((long double *)val1) += *((long double *)val2),"%Lf", *((long double *)val1), *((long double *)val2),addition);
	//*((long double *)val1) += *((long double *) val2);
}

void fsoustraction(void *val1, void *val2){
	CHECK_AND_COMPUT(*((float *)val1) -= *((float *)val2),"%f", *((float *)val1), *((float *)val2),soustraction);
	//*((float *)val1) -= *((float *) val2);
}
void dsoustraction(void *val1, void *val2){
	CHECK_AND_COMPUT(*((double *)val1) -= *((double *)val2),"%lf", *((double *)val1), *((double *)val2),soustraction);
	//*((double *)val1) -= *((double *) val2);
}
void ldsoustraction(void *val1, void *val2){
	CHECK_AND_COMPUT(*((long double *)val1) -= *((long double *)val2),"%Lf", *((long double *)val1), *((long double *)val2),soustraction);
	//*((long double *)val1) -= *((long double *) val2);
}

void fmultiplication(void *val1, void *val2){
	CHECK_AND_COMPUT(*((float *)val1) *= *((float *)val2),"%f", *((float *)val1), *((float *)val2),multiplication);
	/*char valeur1[BUFFER], valeur2[BUFFER], valeur3[BUFFER], *resultat, *temp;
	sprintf(valeur1, "%f", *((float *)val1));
	sprintf(valeur2, "%f", *((float *)val2));
	resultat = multiplication(valeur1, valeur2);
	if((temp = strchr(resultat, '.')))
		temp[8] = 0;
	*((float *)val1) *= *((float *)val2);
	sprintf(valeur3, "%f", *((float *)val1));
	if(equal(valeur3, resultat) != 0){
		printf("ARGHHHH\n");
		free(resultat);
		exit(2);
	}
	free(resultat);*/
}
void dmultiplication(void *val1, void *val2){
	CHECK_AND_COMPUT(*((double *)val1) *= *((double *)val2),"%lf", *((double *)val1), *((double *)val2),multiplication);
	//*((double *)val1) *= *((double *) val2);
}
void ldmultiplication(void *val1, void *val2){
	CHECK_AND_COMPUT(*((long double *)val1) *= *((long double *)val2),"%Lf", *((long double *)val1), *((long double *)val2),multiplication);
	//*((long double *)val1) *= *((long double *)val2);
}

void fdivision(void *val1, void *val2){
	CHECK_AND_COMPUT_DIVISION(*((float *)val1) /= *((float *)val2),"%f", *((float *)val1), *((float *)val2));
	//*((float *)val1) /= *((float *)val2);
}
void ddivision(void *val1, void *val2){
	CHECK_AND_COMPUT_DIVISION(*((double *)val1) /= *((double *)val2),"%lf", *((double *)val1), *((double *)val2));
	//*((double *)val1) /= *((double *) val2);
}
void lddivision(void *val1, void *val2){
	CHECK_AND_COMPUT_DIVISION(*((long double *)val1) /= *((long double *)val2),"%Lf", *((long double *)val1), *((long double *)val2));
	//*((long double *)val1) /= *((long double *)val2);
}

void fcosinus(void *val){
	*((float *)val) = cosf(*((float *)val));
}
void dcosinus(void *val){
	*((double *)val) = cos(*((double *)val));
}
void ldcosinus(void *val){
	*((long double *)val) = cosl(*((long double *)val));
}

void facosinus(void *val){
	*((float *)val) = acosf(*((float *)val));
}
void dacosinus(void *val){
	*((double *)val) = acos(*((double *)val));
}
void ldacosinus(void *val){
	*((long double *)val) = acosl(*((long double *)val));
}

void fsinus(void *val){
	*((float *)val) = sinf(*((float *)val));
}
void dsinus(void *val){
	*((double *)val) = sin(*((double *)val));
}
void ldsinus(void *val){
	*((long double *)val) = sinl(*((long double *)val));
}

void fasinus(void *val){
	*((float *)val) = asinf(*((float *)val));
}
void dasinus(void *val){
	*((double *)val) = asin(*((double *)val));
}
void ldasinus(void *val){
	*((long double *)val) = asinl(*((long double *)val));
}

void ftangente(void *val){
	*((float *)val) = tanf(*((float *)val));
}
void dtangente(void *val){
	*((double *)val) = tan(*((double *)val));
}
void ldtangente(void *val){
	*((long double *)val) = tanl(*((long double *)val));
}

void fatangente(void *val){
	*((float *)val) = atanf(*((float *)val));
}
void datangente(void *val){
	*((double *)val) = atan(*((double *)val));
}
void ldatangente(void *val){
	*((long double *)val) = atanl(*((long double *)val));
}

void fsqrt(void *val){
	*((float *)val) = sqrtf(*((float *)val));
}
void dsqrt(void *val){
	*((double *)val) = sqrt(*((double *)val));
}
void ldsqrt(void *val){
	*((long double *)val) = sqrtl(*((long double *)val));
}

void fexp(void *val){
	*((float *)val) = expf(*((float *)val));
}
void dexp(void *val){
	*((double *)val) = exp(*((double *)val));
}
void _ldexp_(void *val){
	*((long double *)val) = expl(*((long double *)val));
}

void fceil(void *val){
	*((float *)val) = ceilf(*((float *)val));
}
void dceil(void *val){
	*((double *)val) = ceil(*((double *)val));
}
void ldceil(void *val){
	*((long double *)val) = ceill(*((long double *)val));
}

void flog(void *val){
	*((float *)val) = logf(*((float *)val));
}
void dlog(void *val){
	*((double *)val) = log(*((double *)val));
}
void ldlog(void *val){
	*((long double *)val) = logl(*((long double *)val));
}

void flog10(void *val){
	*((float *)val) = log10f(*((float *)val));
}
void dlog10(void *val){
	*((double *)val) = log10(*((double *)val));
}
void ldlog10(void *val){
	*((long double *)val) = log10l(*((long double *)val));
}

void ffabs(void *val){
	*((float *)val) = fabsf(*((float *)val));
}
void dfabs(void *val){
	*((double *)val) = fabs(*((double *)val));
}
void ldfabs(void *val){
	*((long double *)val) = fabsl(*((long double *)val));
}

void ffloor(void *val){
	*((float *)val) = floorf(*((float *)val));
}
void dfloor(void *val){
	*((double *)val) = floor(*((double *)val));
}
void ldfloor(void *val){
	*((long double *)val) = floorl(*((long double *)val));
}

void ffmod(void *val1, void *val2){
	*((float *)val1) = fmodf(*((float *)val1),*((float *)val2));
}
void dfmod(void *val1, void *val2){
	*((double *)val1) = fmod(*((double *)val1),*((double *)val2));
}
void ldfmod(void *val1, void *val2){
	*((long double *)val1) = fmodl(*((long double *)val1),*((long double *)val2));
}

void fpower(void *val1, void *val2){
	*((float *)val1) = powf(*((float *)val1),*((float *)val2));
}
void dpower(void *val1, void *val2){
	*((double *)val1) = powf(*((double *)val1),*((double *)val2));
}
void ldpower(void *val1, void *val2){
	*((long double *)val1) = powf(*((long double *)val1),*((long double *)val2));
}
