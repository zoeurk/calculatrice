#define MAILLON(pv, size)\
pv->next = ___calloc___((void **)&pv->next,size);\
pv->next->prev = pv;\
pv = pv->next;

#define INIT_BUFSET(pv,buffer,end, ___type___) \
pv->val = (void *)pv + sizeof(struct value);\
switch(___type___){\
	/*case CHAR:\
		*((char *)pv->val) = (char)atoi(buffer);\
		break;*/\
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

#define PI_INTEGRATION(pi, buffer, offset, _pi_)\
if(len == -1 && strcmp(pi, buffer) == 0 && _pi_ == NULL){\
	switch(arg->type){\
		case FLOAT:\
			sprintf(buffer,"%f", M_PI);\
			break;\
		case DOUBLE:\
			sprintf(buffer,"%lf", M_PI);\
			break;\
		case LDOUBLE:\
			sprintf(buffer,"%Lf", (long double)M_PI);\
			break;\
	}\
	len = 0;\
}else{\
	if(len == -1){\
		switch(arg->type){\
			case FLOAT:\
				sprintf(buffer,"%f", *((float *)_pi_));\
				break;\
			case DOUBLE:\
				sprintf(buffer,"%lf", *((double *)_pi_));\
				break;\
			case LDOUBLE:\
				sprintf(buffer,"%Lf", *((long double *)_pi_));\
				break;\
		}\
		len = 0;\
	}\
}

#define ERROR(msg,arg)\
fprintf(stderr,msg,arg);\
exit(EXIT_FAILURE);

#define _ERROR_(msg)\
fprintf(stderr,msg);\
exit(EXIT_FAILURE)

#define BUFSET(v, pv,valsize, buffer, end,___type___)\
if(bufset){ \
	if(v == NULL)\
		v = pv = ___calloc___((void **)&v,sizeof(struct value)+valsize);\
	else{\
		MAILLON(pv, sizeof(struct value)+valsize);\
	}\
	INIT_BUFSET(pv,buffer,end, ___type___);\
	pv->type = VALUE;\
	memset(buffer, 0, BUFFER);\
	bufset = 0;\
}\
if(pv == NULL)\
	v = pv = ___calloc___((void **)&v,sizeof(struct value)+valsize);\
MAILLON(pv, sizeof(struct value));

#define PARENTHESE(preader, parentheses)\
if(preader->type == O_PARENTHESE)\
	parentheses++;\
if(preader->type == C_PARENTHESE)\
	parentheses--;

#define SEARCH(eval, ptr, diff, pstart, pcur)\
do{	pstart = ptr->next;\
	PARENTHESE(preader, o_parentheses);\
	if(preader->val == vdup->val){\
		pcur = ptr;\
	}else{\
		if(ptr->prev != NULL && ptr != diff){\
			pprev = ptr->prev;\
			pnext = ptr->next;\
			pprev->next = pnext;\
			if(pnext)\
				pprev->next->prev = pprev;\
			free(ptr);\
		}\
	}\
	ptr = pstart;\
}while(eval);

#define DUPLIQUE(pv,eval, init)\
for(preader = pv->next, o_parentheses = init; eval ; preader = (preader->next != NULL) ? preader->next : preader){\
	PARENTHESE(preader, o_parentheses);\
	if(o_parentheses != 0){\
		if(vdup == NULL){\
			pvdup = vdup = ___calloc___((void **)&vdup, sizeof(struct value));\
		}else{\
			MAILLON(pvdup, sizeof(struct value));\
		}\
		pvdup->value = preader->value;\
		pvdup->type = preader->type;\
		pvdup->val = preader->val;\
		pvdup->operator = preader->operator;\
	}\
}

#define SINGLE_ARG(pv,eval,type)\
DUPLIQUE(pv,eval,1)\
vdup = calcule(&vdup, f, type);\
preader = pv; \
o_parentheses = 0;\
do{	pstart = preader->next;\
	PARENTHESE(preader, o_parentheses);\
	if(preader->val == vdup->val)\
		pcur = preader;\
	else	if(preader->prev != NULL && preader != pv)\
			free(preader);\
	preader = pstart;\
}while(o_parentheses != 0);\
pprev = pv->prev;\
pnext = preader;\
free(pv);\
if(pprev){\
	pprev->next = pcur;\
	pprev->next->prev = pprev;\
}else{\
	*v = pv = pcur;\
}\
if(pnext){\
	pcur->next = preader;\
	pcur->next->prev = pcur;\
}else{\
	pcur->next = NULL;\
}\
free(vdup);\
vdup = NULL;

#define TRIGO(eval, preader, pv, o_parentheses, pnext, pprev, type)\
if(pv->prev == NULL){\
	preader = pv->next->next;\
	for(preader = preader->next, o_parentheses = 1,count = 0;\
		o_parentheses != 0;\
		preader = (o_parentheses != 0) ? preader->next : preader, count++\
	){\
		PARENTHESE(preader, o_parentheses);\
	}\
	if(count == 1){\
		*v = (*v)->next;\
		free(pv);\
		pv = *v;\
		*v = (*v)->next;\
		free(pv);\
		pv = *v;\
		pcur = pprev = pv;\
		pnext = pprev->next->next;\
		free(pprev->next);\
		pprev->next = pnext;\
		if(pnext)\
			pprev->next->prev = pprev;\
	}else{\
		*v = (*v)->next;\
		free((*v)->prev);\
		(*v)->prev = NULL;\
		pv = *v;\
		SINGLE_ARG(pv,eval, type);\
	}\
}else{\
	preader = pv->next->next->next;\
	for(preader = preader, o_parentheses = 1,count = 0;\
		o_parentheses != 0;\
		preader = (preader->next != NULL) ? preader->next: preader, count++\
	){\
		PARENTHESE(preader, o_parentheses);\
	}\
	if(count > 1){\
		SINGLE_ARG(pv->next,eval, type);\
		pprev = pv->prev;\
		pnext = pv->next;\
		pprev->next = pnext;\
		if(pnext)\
			pprev->next->prev = pprev;\
		free(pv);\
		pv = pcur;\
	}else{\
		pprev = pv->prev;\
		pnext = pv->next->next;\
		pcur = pv->next->next;\
		pprev->next = pnext;\
		if(pnext)\
			pprev->next->prev = pprev;\
		pprev = pv->next->next;\
		pnext = pv->next->next->next->next;\
		free(pv->next->next->next);\
		free(pv->next);\
		pprev->next = pnext;\
		if(pnext)\
			pprev->next->prev = pprev;\
		free(pv);\
		pv = pcur;\
	}\
}

#define MULTIPLE_ARGS(preader, pv, pcur, o_parentheses, pnext, pprev, start, virgule, end, type)\
	pv = pcur = pv->next;\
	o_parentheses = 0;\
	while(pcur)\
	{	PARENTHESE(pcur, o_parentheses);\
		if(pcur->type == O_PARENTHESE && o_parentheses == 1)\
			start = pcur;\
		if(pcur->type == C_PARENTHESE && o_parentheses == 0)\
			end = pcur;\
		if(pcur->operator == ',' && o_parentheses == 1)\
			virgule = pcur;\
		pcur = pcur->next;\
	}\
	DUPLIQUE(start, preader != virgule,1);\
	vdup = calcule(&vdup, f, type);\
	preader = start->next;\
	o_parentheses = 1;\
	SEARCH(virgule != preader, preader, preader, pstart, pcur);\
	free(vdup);\
	vdup = NULL;\
	DUPLIQUE(virgule, preader != end, 1);\
	vdup = calcule(&vdup, f, type);\
	preader = virgule->next;\
	SEARCH(preader != end, preader, preader, pstart, pcur);\
	pnext = pcur;\
	free(vdup);\
	vdup = NULL;\
	pvdup = NULL;\
	pprev = start; \
	while(pprev->type != 1)\
		pprev = pprev->next;\
	pnext = virgule;\
	while(pnext->type != 1)\
	pnext = pnext->next;

#define UPDATE(v, pprev, pnext, start, virgule, end)\
	ppv = *v;\
	if(ppv == start->prev){\
		pnext = ppv;\
		while(pnext->type != VALUE){\
			ppnext = pnext->next;\
			free(pnext);\
			pnext = ppnext;\
		}\
		ppv = *v = pnext;\
		pnext = ppv->next;\
		ppv->next->prev = ppv;\
		while(pnext != end){\
			pprev = pnext->next;\
			free(pnext);\
			pnext = pprev;\
		}\
		ppv->next = end->next;\
		if(ppv->next)\
			ppv->next->prev = ppv;\
		free(end);\
	}else{\
		ppprev = start->prev->prev;\
		free(start->prev);\
		free(start);\
		ppnext = end->next;\
		free(end->prev);\
		free(end);\
		ppprev->next = virgule->prev;\
		free(virgule);\
		ppprev->next->prev = ppprev;\
		ppprev->next->next = ppnext;\
		if(ppnext)\
			ppprev->next->next->prev = ppprev->next;\
		ppv = (*v);\
	}



void caddition(void *val1, void *val2);
void faddition(void *val1, void *val2);
void daddition(void *val1, void *val2);
void ldaddition(void *val1, void *val2);

void csoustraction(void *val1, void *val2);
void fsoustraction(void *val1, void *val2);
void dsoustraction(void *val1, void *val2);
void ldsoustraction(void *val1, void *val2);

void cmultiplication(void *val1, void *val2);
void fmultiplication(void *val1, void *val2);
void dmultiplication(void *val1, void *val2);
void ldmultiplication(void *val1, void *val2);

void cdivision(void *val1, void *val2);
void fdivision(void *val1, void *val2);
void ddivision(void *val1, void *val2);
void lddivision(void *val1, void *val2);

void print_float(void *val,char *format);
void print_double(void *val, char *format);
void print_ldouble(void *val, char *format);
void print_char(void *val, char *format);

void ccomplement(void *val1);

void fcosinus(void *val);
void dcosinus(void *val);
void ldcosinus(void *val);

void facosinus(void *val);
void dacosinus(void *val);
void ldacosinus(void *val);

void fsinus(void *val);
void dsinus(void *val);
void ldsinus(void *val);

void fasinus(void *val);
void dasinus(void *val);
void ldasinus(void *val);

void ftangente(void *val);
void dtangente(void *val);
void ldtangente(void *val);

void fatangente(void *val);
void datangente(void *val);
void ldatangente(void *val);

void fsqrt(void *val);
void dsqrt(void *val);
void ldsqrt(void *val);

void fexp(void *val);
void dexp(void *val);
void _ldexp_(void *val);

void fceil(void *val);
void dceil(void *val);
void ldceil(void *val);

void flog(void *val);
void dlog(void *val);
void ldlog(void *val);

void flog10(void *val);
void dlog10(void *val);
void ldlog10(void *val);

void ffabs(void *val);
void dfabs(void *val);
void ldfabs(void *val);

void ffloor(void *val);
void dfloor(void *val);
void ldfloor(void *val);

void ffmod(void *val1, void *val2);
void dfmod(void *val1, void *val2);
void ldfmod(void *val1, void *val2);

void fpower(void *val1, void *val2);
void dpower(void *val1, void *val2);
void ldpower(void *val1, void *val2);


