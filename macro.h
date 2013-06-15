
#ifndef MACRO_H
#define MACRO_H

#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>

#define bool int
#define true 1
#define false 0

#define ALLOCN(t, n) (t*)malloc((n)*sizeof(t))
#define ALLOC(t) ALLOCN(t, 1)

#define ALLOC_TEST(p) \
	if(!p)\
	{\
		fprintf(stderr,"ERROR: Malloc failed in file %s(%d)", __FILE__, __LINE__); \
		exit(1); \
	} 

#define NEWN(t, v, n) t *v = ALLOCN(t, n); ALLOC_TEST(v);
#define NEW(t, v) NEWN(t, v, 1)

#endif

