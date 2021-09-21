#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "calcule-data.h"


void print_char(void *val, char *format){
	printf(format, *((char *)val));
}
void print_uchar(void *val, char *format){
	printf(format, *((unsigned char *)val));
}
void print_short(void *val, char *format){
	printf(format, *((short int *)val));
}
void print_ushort(void *val, char *format){
	printf(format, *((unsigned short int *)val));
}
void print_int(void *val, char *format){
	printf(format, *((int *)val));
}
void print_uint(void *val, char *format){
	printf(format, *((unsigned int *)val));
}
void print_lint(void *val, char *format){
	printf(format, *((long int *)val));
}
void print_ulint(void *val, char *format){
	printf(format, *((unsigned long int *)val));
}
void print_llint(void *val, char *format){
	printf(format, *((long long int *)val));
}
void print_ullint(void *val, char *format){
	printf(format, *((long long int *)val));
}
void print_float(void *val, char *format){
	printf(format, *((float *)val));
}
void print_double(void *val, char *format){
	printf(format, *((double *)val));
}
void print_ldouble(void *val, char *format){
	printf(format, *((long double *)val));
}

void ccomplement(void *val1){
	*((char *)val1) = ~*((char *)val1);
}
void uccomplement(void *val1){
	*((unsigned char *)val1) = ~*((unsigned char *)val1);
}
void cl_move(void *val1,void *val2){
	if(*((char *)val2) < 0){
		fprintf(stderr,"left move negatif\n");
		exit(EXIT_FAILURE);
	}
	*((char *)val1) = *((char *)val1) << *((char *)val2);
}
void ucl_move(void *val1,void *val2){
	*((unsigned char *)val1) = *((unsigned char *)val1) << *((unsigned char *)val2);
}
void cr_move(void *val1,void *val2){
	if(*((char *)val2) < 0){
		fprintf(stderr,"right move negatif\n");
		exit(EXIT_FAILURE);
	}
	*((char *)val1) = *((char *)val1) >> *((char *)val2);
}
void ucr_move(void *val1,void *val2){
	*((unsigned char *)val1) = *((unsigned char *)val1) >> *((unsigned char *)val2);
}
void cand(void *val1,void *val2){
	*((char *)val1) &= *((char *)val2);
}
void ucand(void *val1,void *val2){
	*((unsigned char *)val1) &= *((unsigned char *)val2);
}
void cor(void *val1,void *val2){
	*((char *)val1) |= *((char *)val2);
}
void ucor(void *val1,void *val2){
	*((unsigned char *)val1) |= *((unsigned char *)val2);
}
void cxor(void *val1,void *val2){
	*((char *)val1) ^= *((char *)val2);
}
void ucxor(void *val1,void *val2){
	*((unsigned char *)val1) ^= *((unsigned char *)val2);
}

void scomplement(void *val1){
	*((short int *)val1) = ~*((short int *)val1);
}
void sl_move(void *val1,void *val2){
	if(*((short int *)val2) < 0){
		fprintf(stderr,"left move negatif\n");
		exit(EXIT_FAILURE);
	}
	*((short int *)val1) = *((short int *)val1) << *((short int *)val2);
}
void sr_move(void *val1,void *val2){
	if(*((short int *)val2) < 0){
		fprintf(stderr,"right move negatif\n");
		exit(EXIT_FAILURE);
	}
	*((short int *)val1) = *((short int *)val1) >> *((short int *)val2);
}
void sand(void *val1,void *val2){
	*((short int *)val1) &= *((short int *)val2);
}
void sor(void *val1,void *val2){
	*((short int *)val1) |= *((short int *)val2);
}
void sxor(void *val1,void *val2){
	*((short int *)val1) ^= *((short int *)val2);
}

void uscomplement(void *val1){
	*((unsigned short int *)val1) = ~*((unsigned short int *)val1);
}
void usl_move(void *val1,void *val2){
	*((unsigned short int *)val1) = *((unsigned short int *)val1) << *((unsigned short int *)val2);
}
void usr_move(void *val1,void *val2){
	*((unsigned short int *)val1) = *((unsigned short int *)val1) >> *((unsigned short int *)val2);
}
void usand(void *val1,void *val2){
	*((unsigned short int *)val1) &= *((unsigned short int *)val2);
}
void usor(void *val1,void *val2){
	*((unsigned short int *)val1) |= *((unsigned short int *)val2);
}
void usxor(void *val1,void *val2){
	*((unsigned short int *)val1) ^= *((unsigned short int *)val2);
}

void icomplement(void *val1){
	*((int *)val1) = ~*((int *)val1);
}
void il_move(void *val1,void *val2){
	if(*((int *)val2) < 0){
		fprintf(stderr,"left move negatif\n");
		exit(EXIT_FAILURE);
	}
	*((int *)val1) = *((int *)val1) << *((int *)val2);
}
void ir_move(void *val1,void *val2){
	if(*((int *)val2) < 0){
		fprintf(stderr,"right move negatif\n");
		exit(EXIT_FAILURE);
	}
	*((int *)val1) = *((int *)val1) >> *((int *)val2);
}
void iand(void *val1,void *val2){
	*((int *)val1) &= *((int *)val2);
}
void ior(void *val1,void *val2){
	*((int *)val1) |= *((int *)val2);
}
void ixor(void *val1,void *val2){
	*((int *)val1) ^= *((int *)val2);
}
void uixor(void *val1,void *val2){
	*((unsigned int *)val1) ^= *((unsigned int *)val2);
}

void uicomplement(void *val1){
	*((unsigned int *)val1) = ~*((unsigned int *)val1);
}
void uil_move(void *val1,void *val2){
	*((unsigned int *)val1) = *((unsigned int *)val1) << *((unsigned int *)val2);
}
void uir_move(void *val1,void *val2){
	*((unsigned int *)val1) = *((unsigned int *)val1) >> *((unsigned int *)val2);
}
void uiand(void *val1,void *val2){
	*((unsigned int *)val1) &= *((unsigned int *)val2);
}
void uior(void *val1,void *val2){
	*((unsigned int *)val1) |= *((unsigned int *)val2);
}
void uxor(void *val1,void *val2){
	*((unsigned int *)val1) ^= *((unsigned int *)val2);
}

void licomplement(void *val1){
	*((long int *)val1) = ~*((long int *)val1);
}
void lil_move(void *val1,void *val2){
	if(*((long int *)val2) < 0){
		fprintf(stderr,"left move negatif\n");
		exit(EXIT_FAILURE);
	}
	*((long int *)val1) = *((long *)val1) << *((long int *)val2);
}
void lir_move(void *val1,void *val2){
	if(*((long int *)val2) < 0){
		fprintf(stderr,"right move negatif\n");
		exit(EXIT_FAILURE);
	}
	*((long int *)val1) = *((long int *)val1) >> *((long int *)val2);
}
void liand(void *val1,void *val2){
	*((long int *)val1) &= *((long int *)val2);
}
void lior(void *val1,void *val2){
	*((long int *)val1) |= *((long int *)val2);
}
void lixor(void *val1,void *val2){
	*((long int *)val1) ^= *((long int *)val2);
}

void ulicomplement(void *val1){
	*((unsigned long int *)val1) = ~*((unsigned long int *)val1);
}
void ulil_move(void *val1,void *val2){
	*((unsigned long int *)val1) = *((unsigned long *)val1) << *((unsigned long int *)val2);
}
void ulir_move(void *val1,void *val2){
	*((unsigned long int *)val1) = *((unsigned long int *)val1) >> *((unsigned long int *)val2);
}
void uliand(void *val1,void *val2){
	*((unsigned long int *)val1) &= *((unsigned long int *)val2);
}
void ulior(void *val1,void *val2){
	*((unsigned long int *)val1) |= *((unsigned long int *)val2);
}
void ulixor(void *val1,void *val2){
	*((unsigned long int *)val1) ^= *((unsigned long int *)val2);
}

void llicomplement(void *val1){
	*((long long int *)val1) = ~*((long long int *)val1);
}
void llil_move(void *val1,void *val2){
	if(*((long long int *)val2) < 0){
		fprintf(stderr,"left move negatif\n");
		exit(EXIT_FAILURE);
	}
	*((long long int *)val1) = *((long long *)val1) << *((long long int *)val2);
}
void llir_move(void *val1,void *val2){
	if(*((long long int *)val2) < 0){
		fprintf(stderr,"right move negatif\n");
		exit(EXIT_FAILURE);
	}
	*((long long int *)val1) = *((long long int *)val1) >> *((long long int *)val2);
}
void lliand(void *val1,void *val2){
	*((long long int *)val1) &= *((long long int *)val2);
}
void llior(void *val1,void *val2){
	*((long long int *)val1) |= *((long long int *)val2);
}
void llixor(void *val1,void *val2){
	*((long long int *)val1) ^= *((long long int *)val2);
}

void ullicomplement(void *val1){
	*((unsigned long long int *)val1) = ~*((unsigned long long int *)val1);
}
void ullil_move(void *val1,void *val2){
	*((unsigned long long int *)val1) = *((unsigned long long *)val1) << *((unsigned long long int *)val2);
}
void ullir_move(void *val1,void *val2){
	*((unsigned long long int *)val1) = *((unsigned long long int *)val1) >> *((unsigned long long int *)val2);
}
void ulliand(void *val1,void *val2){
	*((unsigned long long int *)val1) &= *((unsigned long long int *)val2);
}
void ullior(void *val1,void *val2){
	*((unsigned long long int *)val1) |= *((unsigned long long int *)val2);
}
void ullixor(void *val1,void *val2){
	*((unsigned long long int *)val1) ^= *((unsigned long long int *)val2);
}

void caddition(void *val1, void *val2){
	*((char *)val1) += *((char *) val2);
}
void ucaddition(void *val1, void *val2){
	*((unsigned char *)val1) += *((unsigned char *) val2);
}
void saddition(void *val1, void *val2){
	*((short int *)val1) += *((short int *) val2);
}
void usaddition(void *val1, void *val2){
	*((unsigned short int *)val1) += *((unsigned short int *) val2);
}
void iaddition(void *val1, void *val2){
	*((int *)val1) += *((int *) val2);
}
void uiaddition(void *val1, void *val2){
	*((unsigned int *)val1) += *((unsigned int *) val2);
}
void liaddition(void *val1, void *val2){
	*((long int *)val1) += *((long int *) val2);
}
void uliaddition(void *val1, void *val2){
	*((unsigned long int *)val1) += *((unsigned long int *) val2);
}
void lliaddition(void *val1, void *val2){
	*((long long int *)val1) += *((long long int *) val2);
}
void ulliaddition(void *val1, void *val2){
	*((unsigned long long int *)val1) += *((unsigned long long int *) val2);
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

void csoustraction(void *val1, void *val2){
	*((char *)val1) -= *((char *) val2);
}
void ucsoustraction(void *val1, void *val2){
	*((unsigned char *)val1) -= *((unsigned char *) val2);
}
void ssoustraction(void *val1, void *val2){
	*((short int *)val1) -= *((short int *) val2);
}
void ussoustraction(void *val1, void *val2){
	*((unsigned short int *)val1) -= *((unsigned short int *) val2);
}
void isoustraction(void *val1, void *val2){
	*((int *)val1) -= *((int *) val2);
}
void uisoustraction(void *val1, void *val2){
	*((unsigned int *)val1) -= *((unsigned int *) val2);
}
void lisoustraction(void *val1, void *val2){
	*((long int *)val1) -= *((long int *) val2);
}
void ulisoustraction(void *val1, void *val2){
	*((unsigned long int *)val1) -= *((unsigned long int *) val2);
}
void llisoustraction(void *val1, void *val2){
	*((long long int *)val1) -= *((long long int *) val2);
}
void ullisoustraction(void *val1, void *val2){
	*((unsigned long long int *)val1) -= *((unsigned long long int *) val2);
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

void cmultiplication(void *val1, void *val2){
	*((char *)val1) *= *((char *)val2);
}
void ucmultiplication(void *val1, void *val2){
	*((unsigned char *)val1) *= *((unsigned char *)val2);
}
void smultiplication(void *val1, void *val2){
	*((short int *)val1) *= *((short int *)val2);
}
void usmultiplication(void *val1, void *val2){
	*((unsigned short int *)val1) *= *((unsigned short int *)val2);
}
void imultiplication(void *val1, void *val2){
	*((int *)val1) *= *((int *)val2);
}
void uimultiplication(void *val1, void *val2){
	*((unsigned int *)val1) *= *((unsigned int *)val2);
}
void limultiplication(void *val1, void *val2){
	*((long int *)val1) *= *((long int *)val2);
}
void ulimultiplication(void *val1, void *val2){
	*((unsigned long int *)val1) *= *((unsigned long int *)val2);
}
void llimultiplication(void *val1, void *val2){
	*((long long int *)val1) *= *((long long int *)val2);
}
void ullimultiplication(void *val1, void *val2){
	*((unsigned long long int *)val1) *= *((unsigned long long int *)val2);
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

void cdivision(void *val1, void *val2){
	*((char *)val1) /= *((char *)val2);
}
void ucdivision(void *val1, void *val2){
	*((unsigned char *)val1) /= *((unsigned char *)val2);
}
void sdivision(void *val1, void *val2){
	*((short int *)val1) /= *((short int *)val2);
}
void usdivision(void *val1, void *val2){
	*((unsigned short int *)val1) /= *((unsigned short int *)val2);
}
void idivision(void *val1, void *val2){
	*((int *)val1) /= *((int *)val2);
}
void uidivision(void *val1, void *val2){
	*((unsigned int *)val1) /= *((unsigned int *)val2);
}
void lidivision(void *val1, void *val2){
	*((long int *)val1) /= *((long int *)val2);
}
void ulidivision(void *val1, void *val2){
	*((unsigned long int *)val1) /= *((unsigned long int *)val2);
}
void llidivision(void *val1, void *val2){
	*((long long int *)val1) /= *((long long int *)val2);
}
void ullidivision(void *val1, void *val2){
	*((unsigned long long int *)val1) /= *((unsigned long long int *)val2);
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

void cfmod(void *val1, void *val2){
	*((char *)val1) %= *((char *)val2);
}
void ucfmod(void *val1, void *val2){
	*((unsigned char *)val1) %= *((unsigned char *)val2);
}
void sfmod(void *val1, void *val2){
	*((short int *)val1) %= *((short int *)val2);
}
void usfmod(void *val1, void *val2){
	*((unsigned short int *)val1) %= *((unsigned short int *)val2);
}
void ifmod(void *val1, void *val2){
	*((int *)val1) %= *((int *)val2);
}
void uifmod(void *val1, void *val2){
	*((unsigned int *)val1) %= *((unsigned int *)val2);
}
void lifmod(void *val1, void *val2){
	*((long int *)val1) %= *((long int *)val2);
}
void ulifmod(void *val1, void *val2){
	*((unsigned long int *)val1) %= *((unsigned long int *)val2);
}
void llifmod(void *val1, void *val2){
	*((long long int *)val1) %= *((long long int *)val2);
}
void ullifmod(void *val1, void *val2){
	*((unsigned long long int *)val1) %= *((unsigned long long int *)val2);
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
