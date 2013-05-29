
#ifndef MACRO_H
#define MACRO_H

#include<stdlib.h>

#define ALLOCN(t, n) (t*)malloc((n)*sizeof(t))
#define ALLOC(t) ALLOCN(t, 1)

#define ALLOC_TEST(p) \
	if(!p) \
	{ \
		fprintf(stderr,"ERROR: Malloc failed in file %s(%d)", __FILE__, __LINE__); \
		exit(1); \
	} 

#define NEW(t, v) t *v = ALLOC(t); ALLOC_TEST(v);

#endif

