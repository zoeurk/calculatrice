#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include <math.h>*/
#include "calcule-data.h"
#include "../operation/operation.h"

const unsigned long int BUFFER = 26;

#define CHECK_AND_COMPUT(op_final,string_type, n1, n2, op, type)\
	char valeur1[BUFFER], valeur2[BUFFER], valeur3[BUFFER], *resultat, *temp, m[BUFFER], *pmin, *pmax;\
	sprintf(m, string_type, *((type *)arrondi));\
	if(n1 != INFINITY && n1 != -INFINITY && n1 != NAN && n1 != -NAN && n2 != INFINITY && n2 != -INFINITY && n2 != NAN && n2 != -NAN){\
		sprintf(valeur1, string_type, n1);\
		sprintf(valeur2, string_type, n2);\
		resultat = op(valeur1, valeur2);\
			if((temp = strchr(resultat, '.')) && strlen(temp) > 6)\
				temp[7] = 0;\
		/**((float *)val1) *= *((float *)val2);*/\
		op_final;\
		sprintf(valeur3, string_type, n1);\
		pmin = soustraction(valeur3, m);\
		pmax = addition(valeur3, m);\
		/*exit(0);*/\
		if(strtype(valeur3) || equal(pmin, resultat) > 0 || equal(pmax, resultat) < 0){\
		if(equal(valeur3,resultat) != 0 && strcmp(valeur3,"nan")!=0 && strcmp(valeur3, "-nan") != 0 && strcmp(valeur3, "inf") != 0 && strcmp(valeur3, "-inf") != 0){\
			fprintf(stderr,"ERROR: Nombre trop long pour etre converti dans ce format: %s != %s\n", valeur3, resultat);\
			free(resultat);\
			exit(2);\
		}\
	}\
		free(pmin);\
		free(pmax);\
		free(resultat);\
	}else op_final;
	
#define CHECK_AND_COMPUT_DIVISION(op_final,string_type, n1, n2, len, type)\
	char valeur1[BUFFER], valeur2[BUFFER], valeur3[BUFFER], *resultat, m[BUFFER], *pmin, *pmax;\
	sprintf(m, string_type, *((type *)arrondi)); \
	if(n1 != INFINITY && n1 != NAN && n1 != -NAN && n2 != INFINITY && n2 != NAN && n2 != -NAN){\
		sprintf(valeur1, string_type, n1);\
		sprintf(valeur2, string_type, n2);\
		resultat = division(valeur1, valeur2, len, 0);\
		/**((float *)val1) *= *((float *)val2);*/\
		op_final;\
		sprintf(valeur3, string_type, n1);\
		pmin = soustraction(valeur3, m);\
		pmax = addition(valeur3, m);\
		if(strtype(valeur3) || equal(pmin, resultat) > 0 || equal(pmax, resultat) < 0){\
			if(strcmp(valeur3,"nan")!=0 && strcmp(valeur3, "-nan") != 0 && strcmp(valeur3, "inf") != 0 && strcmp(valeur3, "-inf") != 0){\
			fprintf(stderr,"ERROR: Nombre trop long pour etre converti dans ce format: %s != %s\n", valeur3, resultat);\
			free(resultat);\
			exit(2);\
		}\
	}\
		free(pmin);\
		free(pmax);\
		free(resultat);\
	}else\
		op_final;\

void print_float(void *val, char *format){
	printf(format, *((float *)val));
}
void print_double(void *val, char *format){
	printf(format, *((double *)val));
}
void print_ldouble(void *val, char *format){
	printf(format, *((long double *)val));
}

void faddition(void *val1, void *val2,void *arrondi){
	CHECK_AND_COMPUT(*((float *)val1) += *((float *)val2),"%f", *((float *)val1), *((float *)val2),addition, float);
	//*((float *)val1) += *((float *) val2);
}
void daddition(void *val1, void *val2, void *arrondi){
	CHECK_AND_COMPUT(*((double *)val1) += *((double*)val2),"%lf", *((double *)val1), *((double *)val2),addition, double);
	//*((double *)val1) += *((double *) val2);
}
void ldaddition(void *val1, void *val2, void *arrondi){
	//printf("%Lf::%Lf\n", *((long double *)val1), *((long double *)val2));
	//exit(0);
	CHECK_AND_COMPUT(*((long double *)val1) += *((long double *)val2),"%Lf", *((long double *)val1), *((long double *)val2),addition, long double);
	//*((long double *)val1) += *((long double *) val2);
}

void fsoustraction(void *val1, void *val2, void *arrondi){
	CHECK_AND_COMPUT(*((float *)val1) -= *((float *)val2),"%f", *((float *)val1), *((float *)val2),soustraction, float);
	//*((float *)val1) -= *((float *) val2);
}
void dsoustraction(void *val1, void *val2, void *arrondi){
	CHECK_AND_COMPUT(*((double *)val1) -= *((double *)val2),"%lf", *((double *)val1), *((double *)val2),soustraction, double);
	//*((double *)val1) -= *((double *) val2);
}
void ldsoustraction(void *val1, void *val2, void *arrondi){
	CHECK_AND_COMPUT(*((long double *)val1) -= *((long double *)val2),"%Lf", *((long double *)val1), *((long double *)val2),soustraction, long double);
	//*((long double *)val1) -= *((long double *) val2);
}

void fmultiplication(void *val1, void *val2, void *arrondi){
	CHECK_AND_COMPUT(*((float *)val1) *= *((float *)val2),"%f", *((float *)val1), *((float *)val2),multiplication, float);
}
void dmultiplication(void *val1, void *val2, void *arrondi){
	CHECK_AND_COMPUT(*((double *)val1) *= *((double *)val2),"%lf", *((double *)val1), *((double *)val2),multiplication, double);
	//*((double *)val1) *= *((double *) val2);
}
void ldmultiplication(void *val1, void *val2, void *arrondi){
	CHECK_AND_COMPUT(*((long double *)val1) *= *((long double *)val2),"%Lf", *((long double *)val1), *((long double *)val2),multiplication, long double);
	//*((long double *)val1) *= *((long double *)val2);
}

void fdivision(void *val1, void *val2, void *arrondi){
	CHECK_AND_COMPUT_DIVISION(*((float *)val1) /= *((float *)val2),"%f", *((float *)val1), *((float *)val2), 6, float);
	//*((float *)val1) /= *((float *)val2);
}
void ddivision(void *val1, void *val2, void *arrondi){
	CHECK_AND_COMPUT_DIVISION(*((double *)val1) /= *((double *)val2),"%lf", *((double *)val1), *((double *)val2), 6, double);
	//*((double *)val1) /= *((double *) val2);
}
void lddivision(void *val1, void *val2, void *arrondi){
	CHECK_AND_COMPUT_DIVISION(*((long double *)val1) /= *((long double *)val2),"%Lf", *((long double *)val1), *((long double *)val2), 6, long double);
	//*((long double *)val1) /= *((long double *)val2);
}

void fcosinus(void *val){
	___SINGLE_ARG___(float, "%f", strtof);
	*((float *)val) = cosf(*((float *)val));
}
void dcosinus(void *val){
	//SINGLE_ARG(type, "%lf", strtod);
	___SINGLE_ARG___(double, "%lf", strtod);
	*((double *)val) = cos(*((double *)val));
}
void ldcosinus(void *val){
	___SINGLE_ARG___(long double, "%Lf", strtold);
	//SINGLE_ARG(type, "%Lf", strtold);
	*((long double *)val) = cosl(*((long double *)val));
}

void facosinus(void *val){
	___SINGLE_ARG___(long double, "%Lf", strtold);
	*((float *)val) = acosf(*((float *)val));
}
void dacosinus(void *val){
	___SINGLE_ARG___(long double, "%Lf", strtold);
	*((double *)val) = acos(*((double *)val));
}
void ldacosinus(void *val){
	___SINGLE_ARG___(long double, "%Lf", strtold);
	*((long double *)val) = acosl(*((long double *)val));
}

void fsinus(void *val){
	___SINGLE_ARG___(long double, "%Lf", strtold);
	*((float *)val) = sinf(*((float *)val));
}
void dsinus(void *val){
	*((double *)val) = sin(*((double *)val));
}
void ldsinus(void *val){
	___SINGLE_ARG___(long double, "%Lf", strtold);
	*((long double *)val) = sinl(*((long double *)val));
}

void fasinus(void *val){
	___SINGLE_ARG___(float, "%f", strtof);
	*((float *)val) = asinf(*((float *)val));
}
void dasinus(void *val){
	___SINGLE_ARG___(double, "%lf", strtod);
	*((double *)val) = asin(*((double *)val));
}
void ldasinus(void *val){
	___SINGLE_ARG___(long double, "%Lf", strtold);
	*((long double *)val) = asinl(*((long double *)val));
}

void ftangente(void *val){
	___SINGLE_ARG___(float, "%f", strtof);
	*((float *)val) = tanf(*((float *)val));
}
void dtangente(void *val){
	___SINGLE_ARG___(double, "%lf", strtof);
	*((double *)val) = tan(*((double *)val));
}
void ldtangente(void *val){
	___SINGLE_ARG___(long double, "%Lf", strtold);
	*((long double *)val) = tanl(*((long double *)val));
}

void fatangente(void *val){
	___SINGLE_ARG___(float, "%f", strtof);
	*((float *)val) = atanf(*((float *)val));
}
void datangente(void *val){
	___SINGLE_ARG___(double, "%lf", strtold);
	*((double *)val) = atan(*((double *)val));
}
void ldatangente(void *val){
	___SINGLE_ARG___(long double, "%Lf", strtold);
	*((long double *)val) = atanl(*((long double *)val));
}

void fsqrt(void *val){
	float f = *((float *)val);
	___SINGLE_ARG___(float, "%f", strtof);
	*((float *)val) = sqrtf(*((float *)val));
	if(*((float *)val)**((float *)val)!= f){
		fprintf(stderr,"ERROR: Nombre trop long pour etre converti dans ce format.\n");
		exit(2);
	}
}
void dsqrt(void *val){
	double d = *((double *)val);
	___SINGLE_ARG___(double, "%lf", strtod);
	*((double *)val) = sqrt(*((double *)val));
	if(*((double *)val)**((double *)val)!= d){
		fprintf(stderr,"ERROR: Nombre trop long pour etre converti dans ce format.\n");
		exit(2);
	}
}
void ldsqrt(void *val){
	long double ld = *((long double *)val);
	___SINGLE_ARG___(long double, "%Lf", strtold);
	*((long double *)val) = sqrtl(*((long double *)val));
	if(*((long double *)val)**((long double *)val)!= ld){
		fprintf(stderr,"ERROR: Nombre trop long pour etre converti dans ce format.\n");
		exit(2);
	}
}

void fexp(void *val){
	___SINGLE_ARG___(float, "%f", strtof);
	*((float *)val) = expf(*((float *)val));
}
void dexp(void *val){
	___SINGLE_ARG___(double, "%lf", strtod);
	*((double *)val) = exp(*((double *)val));
}
void _ldexp_(void *val){
	___SINGLE_ARG___(long double, "%Lf", strtold);
	*((long double *)val) = expl(*((long double *)val));
}

void fceil(void *val){
	___SINGLE_ARG___(float, "%f", strtof);
	*((float *)val) = ceilf(*((float *)val));
}
void dceil(void *val){
	___SINGLE_ARG___(double, "%lf", strtod);
	*((double *)val) = ceil(*((double *)val));
}
void ldceil(void *val){
	___SINGLE_ARG___(long double, "%Lf", strtold);
	*((long double *)val) = ceill(*((long double *)val));
}

void flog(void *val){
	___SINGLE_ARG___(float, "%f", strtof);
	*((float *)val) = logf(*((float *)val));
}
void dlog(void *val){
	___SINGLE_ARG___(double, "%lf", strtod);
	*((double *)val) = log(*((double *)val));
}
void ldlog(void *val){
	___SINGLE_ARG___(long double, "%Lf", strtold);
	*((long double *)val) = logl(*((long double *)val));
}

void flog10(void *val){
	___SINGLE_ARG___(float, "%f", strtof);
	*((float *)val) = log10f(*((float *)val));
}
void dlog10(void *val){
	___SINGLE_ARG___(double, "%lf", strtod);
	*((double *)val) = log10(*((double *)val));
}
void ldlog10(void *val){
	___SINGLE_ARG___(long double, "%Lf", strtold);
	*((long double *)val) = log10l(*((long double *)val));
}

void ffabs(void *val){
	___SINGLE_ARG___(float, "%f", strtof);
	*((float *)val) = fabsf(*((float *)val));
}
void dfabs(void *val){
	___SINGLE_ARG___(double, "%lf", strtod);
	*((double *)val) = fabs(*((double *)val));
}
void ldfabs(void *val){
	___SINGLE_ARG___(long double, "%Lf", strtold);
	*((long double *)val) = fabsl(*((long double *)val));
}

void ffloor(void *val){
	___SINGLE_ARG___(float, "%f", strtof);
	*((float *)val) = floorf(*((float *)val));
}
void dfloor(void *val){
	___SINGLE_ARG___(double, "%lf", strtod);
	*((double *)val) = floor(*((double *)val));
}
void ldfloor(void *val){
	___SINGLE_ARG___(long double, "%Lf", strtold);
	*((long double *)val) = floorl(*((long double *)val));
}

void ffmod(void *val1, void *val2){
	DOUBLE_ARG(double, strtod, "%f");
	*((float *)val1) = fmodf(*((float *)val1),*((float *)val2));
}
void dfmod(void *val1, void *val2){
	DOUBLE_ARG(double, strtod, "%lf");
	*((double *)val1) = fmod(*((double *)val1),*((double *)val2));
}
void ldfmod(void *val1, void *val2){
	DOUBLE_ARG(long double, strtold, "%Lf");
	/*char buf[BUFFER];
	long double test;
	sprintf(buf, "%Ld", *((long double *)val1));
	test = strtold(buf,NULL);
	if(test != *((long double *)val1)){
		fprintf(stderr,"ERROR: Nombre trop long pour etre calcule dans ce format.\n");
		exit(EXIT_FAILURE);
	}
	sprintf(buf, "%Ld", *((long double *)val2));
	test = strtold(buf,NULL);
	if(test != *((long double *)val2)){
		fprintf(stderr,"ERROR: Nombre trop long pour etre calcule dans ce format.\n");
		exit(EXIT_FAILURE);
	}*/
	*((long double *)val1) = fmodl(*((long double *)val1),*((long double *)val2));
}

void fpower(void *val1, void *val2){
	//float val = *((float *)val1);
	char buf[BUFFER], *pbuf;
	float test, result;
	sprintf(buf, "%f", *((float *)val2));
	if((pbuf = strchr(buf,'.')) && equal(pbuf+1,"0") != 0){
		*pbuf = 0;
		test = strtof(buf,NULL);
		result = powf(*((float *)val1),test);
		if(fmodf(result, *((float *)val1)) != 0){
			fprintf(stderr,"ERROR: Nombre trop long pour etre calcule dans ce format.\n");
			exit(EXIT_FAILURE);
		}
		//*((float *)val1) = result;
		*pbuf = '.';
		*(pbuf-1) = '0';
		test = strtof(pbuf-1, NULL);
		//printf("%f::%f\n",*((float *)val1), test);
		*((float *)val1) = powf(*((float *)val1), *((float *)val2));
	}else{
		test = *((float *)val1);
		*((float *)val1)  = powf(*((float *)val1), *((float *)val2));
		if(fmodf(*((float *)val1),test) != 0){
			fprintf(stderr,"ERROR: Nombre trop long pour etre calcule dans ce format.\n");
			exit(EXIT_FAILURE);
		}
		//powf(*((float *)val1), *((float *)val2)
	}
	//*((float *)val1) = powf(*((float *)val1),*((float *)val2));
	/*if(fmodf(*((float *)val1),val) != 0){
		if(*((float *)val1) > 0)
			*((float *)val1) = 1.0/0.0;
		else *((float *)val1) = -1.0/0.0;
	}*/
}
void dpower(void *val1, void *val2){
	char buf[BUFFER], *pbuf;
	double test, result;
	sprintf(buf, "%lf", *((double *)val2));
	if((pbuf = strchr(buf,'.')) && equal(pbuf+1,"0") != 0){
		*pbuf = 0;
		test = strtod(buf,NULL);
		result = pow(*((double *)val1),test);
		if(fmodf(result, *((double *)val1)) != 0){
			fprintf(stderr,"ERROR: Nombre trop long pour etre calcule dans ce format.\n");
			exit(EXIT_FAILURE);
		}
		//*((float *)val1) = result;
		*pbuf = '.';
		*(pbuf-1) = '0';
		test = strtof(pbuf-1, NULL);
		//printf("%f::%f\n",*((float *)val1), test);
		*((double *)val1) = pow(*((double *)val1), *((double *)val2));
	}else{
		test = *((double *)val1);
		*((double *)val1)  = pow(*((double *)val1), *((double *)val2));
		if(fmodf(*((double *)val1),test) != 0){
			fprintf(stderr,"ERROR: Nombre trop long pour etre calcule dans ce format.\n");
			exit(EXIT_FAILURE);
		}
		//powf(*((float *)val1), *((float *)val2)
	}
	//*((float *)val1) = powf(*((float *)val1),*((float *)val2));
	/*if(fmodf(*((float *)val1),val) != 0){
		if(*((float *)val1) > 0)
			*((float *)val1) = 1.0/0.0;
		else *((float *)val1) = -1.0/0.0;
	}*/
	//double val = *((double *)val1);
	//*((double *)val1) = pow(*((double *)val1),*((double *)val2));
	/*if(fmod(*((double *)val1),val) != 0){
		if(*((double *)val1) > 0)
			*((double *)val1) = (double)1.0/0.0;
		else *((double *)val1) = (double)-1.0/0.0;
	}*/
}
void ldpower(void *val1, void *val2){
	char buf[BUFFER], *pbuf;
	long double test, result;
	sprintf(buf, "%Lf", *((long double *)val2));
	if((pbuf = strchr(buf,'.')) && equal(pbuf+1,"0") != 0){
		*pbuf = 0;
		test = strtod(buf,NULL);
		result = powl(*((long double *)val1),test);
		if(fmodl(result, *((long double *)val1)) != 0){
			fprintf(stderr,"ERROR: Nombre trop long pour etre calcule dans ce format.\n");
			exit(EXIT_FAILURE);
		}
		//*((float *)val1) = result;
		*pbuf = '.';
		*(pbuf-1) = '0';
		test = strtold(pbuf-1, NULL);
		//printf("%f::%f\n",*((float *)val1), test);
		*((long double *)val1) = powl(*((long double *)val1), *((long double *)val2));
	}else{
		test = *((long double *)val1);
		*((long double *)val1)  = pow(*((long double *)val1), *((long double *)val2));
		if(fmodl(*((long double *)val1),test) != 0){
			fprintf(stderr,"ERROR: Nombre trop long pour etre calcule dans ce format.\n");
			exit(EXIT_FAILURE);
		}
		//powf(*((float *)val1), *((float *)val2)
	}
	//long double val = *((long double *)val1);
	//*((long double *)val1) = powl(*((long double *)val1),*((long double *)val2));
	/*if(fmodl(*((long double *)val1),val) != 0)
		*((long double *)val1) = (long double)NAN;
	*/
}
