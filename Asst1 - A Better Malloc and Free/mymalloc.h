#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef _MYMALLOC_H
#define _MYMALLOC_H

#define malloc(x) mymalloc(x)

#ifndef MEMSIZE
#define MEMSIZE 5000
#endif

static char MEM[MEMSIZE];

typedef struct Meta{
	int size;
	int is_free;
	struct Meta *prev;
	struct Meta *next;
} Meta;

// head pointer points to the beginning of the array
Meta * HEAD = (void *) MEM;

// function definitions
void initMem();
void organizeMem(Meta * curr, int size);
void *mymalloc(size_t size);
void myfree(void *ptr);


#endif
