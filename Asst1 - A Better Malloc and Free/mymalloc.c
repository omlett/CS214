#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"

// Initialize HEAD.
void initMem(){
	
	HEAD->next = NULL;
	HEAD->prev = NULL;
	HEAD->is_free = 1;
	HEAD->size = MEMSIZE-sizeof(Meta);
}

/* function that take large piece of memory and allocates only the 
portion needed and creates a Meta component for the rest */
void organizeMem(Meta * curr, int size){
	curr->is_free = 0;
	//we need to find the address at which the next Meta compnent will live
	Meta * new = (void *) ((void *)curr+sizeof(Meta)+size);
	new->size = (curr->size)-size-sizeof(Meta);
	new->is_free = 1;
	curr->size = size;
	new->next = curr->next;
	new->prev = curr;
	curr->next = new;
}

void *mymalloc(size_t size){

	// Zero space requested
	if (size <= 0){
		fprintf(stderr,"ERROR: zero/negative request. FILE: %s, LINE %d\n", __FILE__, __LINE__);
		return NULL;
	}
	// >5000 space requested
	if (size > (MEMSIZE-sizeof(Meta))){
		fprintf(stderr,"ERROR: overload request. FILE: %s, LINE %d\n", __FILE__, __LINE__);
		return NULL;
	}

	//create Meta pointers to traverse with
	Meta *curr, *prev;
	//returned pointer
	void * memptr;

	// mem has yet to be initialized
	if (!(HEAD->size)){
		initMem();
	}

	curr = HEAD;
	// first traverse and find empty space 
	while ((curr != NULL) && (curr->next != NULL) && ((curr->is_free == 0)||(curr->size < size))){
		//iterate through array
		prev = curr;
		if (curr->next == NULL){
			curr = NULL;
		}
		else{
			curr = curr->next;
		}
	}
	// if space size == sizeof(Meta)+ size
	if (curr == NULL){
		// NSF error
		fprintf(stderr,"ERROR: insufficient space. FILE: %s, LINE %d\n", __FILE__, __LINE__);
		return NULL;
	}
	if ((curr->size) == size){
		//initialize Metadata 
		curr->is_free = 0;
		curr->size = size;
		memptr = (void *)(curr + sizeof(Meta));
		return memptr;
	}

	// Sufficient Space Found.
	else if((curr->size) > (size + sizeof(Meta))){
		organizeMem(curr, size);
		memptr = (void *)(++curr);
		return memptr;
	}

	else{
		// NSF error
		fprintf(stderr,"ERROR: insufficient space. FILE: %s, LINE %d\n", __FILE__, __LINE__);
		return NULL;
	}
}

void myfree(void *ptr){

	// Basic Error Checks
	if (ptr == NULL){
		fprintf(stderr,"ERROR: null free request. FILE: %s, LINE %d\n", __FILE__, __LINE__);
		return;
	}
	
	if ((ptr < &MEM) || (ptr > (&MEM+MEMSIZE))){
		fprintf(stderr,"ERROR: out of bounds free request. FILE: %s, LINE %d\n", __FILE__, __LINE__);
		return;
	}

	Meta *toFree;
	toFree = (Meta *)(ptr - sizeof(Meta));
	Meta *curr, *previous, *nxt;
	curr = HEAD;


	// Determine Pointer's Validity
	while((toFree != curr) && (curr != NULL)){

		if ((curr->next) == NULL){

			curr = NULL;

		}
		else{

			curr = curr->next;

		}
	}


	if(toFree == curr){
		if ((toFree->is_free) == 0){
			if (toFree->prev != NULL){
				previous = toFree->prev;
			}
			else {
				previous = NULL;
			}
			if (toFree->next != NULL){
				nxt = toFree->next;
			}
			else {
				nxt = NULL;
			}
			if (nxt != NULL){
				// Next Block is Free; Consolidate.
				if (nxt->is_free == 1){
					if (nxt->next != NULL){
						toFree->size += (sizeof(struct Meta) + nxt->size);
						toFree->next = nxt->next;
						nxt->next->prev = toFree;
						toFree->is_free = 1;
					}
				}
			}
			else{
				toFree->is_free = 1;
			}
		
			// Previous Block is Free; Consolidate.
			if ((previous != NULL) && (previous->is_free) == 1){
				previous->size += (sizeof(struct Meta) + toFree->size);
				previous->next = toFree->next;
				toFree->next->prev = previous;
			}
		}
		else {
			fprintf(stderr,"ERROR: double free request. FILE: %s, LINE %d\n", __FILE__, __LINE__);
			return;
		}
	}
	else{
		fprintf(stderr,"ERROR: invalid free request. FILE: %s, LINE %d\n", __FILE__, __LINE__);
		return;
	}
}

