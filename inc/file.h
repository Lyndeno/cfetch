#ifndef FILE_H
#define FILE_H

#include <stdio.h>

char *procParse(FILE *cpuinfo, char *buffer, char *field);
char *osParse(FILE *cpuinfo, char *buffer, char *field);
char *readFirstline(FILE *f);


#endif
