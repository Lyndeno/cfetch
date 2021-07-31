#include "file.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/*
 * Takes an input file, expects /proc/cpuinfo with field names and values separated by semicolons
 * Returns pointer to field contents, NULL if field could not be found
 */
char *procParse(FILE *cpuinfo, char *buffer, char *field) {
	char *cpuEntry = NULL;
	size_t size = 0;
	while(getline(&cpuEntry, &size, cpuinfo) != -1) {
		if (strstr(cpuEntry, field) != NULL) {
			// Clean this up
			char *entryText = strstr(cpuEntry, ":") + 1;
			while ( *entryText == ' ') {
				entryText++;
			}

			// Replace newline character with null terminator
			*strstr(entryText, "\n") = '\0';

			// Allocate exact memory for final string
			strcpy(buffer, entryText);

			// Free line pointer
			free(cpuEntry);
			return buffer;
		}	
	}
	// Return NULL if desired entry was not found
	return NULL;
}

char *readFirstline(FILE *f) {
	char *line = NULL;
	size_t size = 0;
	getline(&line, &size, f);
	*strstr(line, "\n") = '\0';
	return line;
}
