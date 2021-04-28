#include <math.h>
#include <stdio.h>
#include "calcule-data.h"


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
	*((float *)val1) += *((float *) val2);
}
void daddition(void *val1, void *val2){
	*((double *)val1) += *((double *) val2);
}
void ldaddition(void *val1, void *val2){
	*((long double *)val1) += *((long double *) val2);
}

void fsoustraction(void *val1, void *val2){
	*((float *)val1) -= *((float *) val2);
}
void dsoustraction(void *val1, void *val2){
	*((double *)val1) -= *((double *) val2);
}
void ldsoustraction(void *val1, void *val2){
	*((long double *)val1) -= *((long double *) val2);
}

void fmultiplication(void *val1, void *val2){
	*((float *)val1) *= *((float *)val2);
}
void dmultiplication(void *val1, void *val2){
	*((double *)val1) *= *((double *) val2);
}
void ldmultiplication(void *val1, void *val2){
	*((long double *)val1) *= *((long double *)val2);
}

void fdivision(void *val1, void *val2){
	*((float *)val1) /= *((float *)val2);
}
void ddivision(void *val1, void *val2){
	*((double *)val1) /= *((double *) val2);
}
void lddivision(void *val1, void *val2){
	*((long double *)val1) /= *((long double *)val2);
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
