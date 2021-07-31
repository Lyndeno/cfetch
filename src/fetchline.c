#include "fetchline.h"
#include <stdio.h>

void align_fetchlist(fetchlist *fetcharray, size_t count) {
	size_t max_length = 0;
	size_t current_length;
	char buffer[TITLE_MAX];
	char padding[TITLE_MAX];

	for (size_t i = 0; i < count; i++) {
		current_length = strlen(fetcharray[i].title);
		if (current_length > max_length) {
			max_length = current_length;
		}
	}
	for (size_t i = 0; i < count; i++) {
		current_length = strlen(fetcharray[i].title);
		if (current_length < max_length) {
			for (size_t j = 0; j <= max_length - current_length; j++) {
				if (j == max_length - current_length) {
					padding[j] = '\0';
				} else {
					padding[j] = ' ';
				}
			}
			strcpy(buffer, fetcharray[i].title);
			sprintf(fetcharray[i].title, "%s%s", padding, buffer);
		}
	}
}
