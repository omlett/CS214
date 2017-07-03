#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef _LIBNETFILES_H
#define _LIBNETFILES_H


// function definitions
int netserverinit(struct hostent * serverIPAddress);
void error(char *msg);

#endif
