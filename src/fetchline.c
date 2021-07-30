#include "fetchline.h"
#include <stdlib.h>
#include <stdio.h>

void print_fetch(fetchline *list_element, bool useIcons) {
	// Fix this and make it cleaner
	if (useIcons) {
		while (list_element != NULL) {
			printf("%s %s%s%s\n", list_element->icon, list_element->title, SEPARATOR, list_element->content);
			list_element = list_element->next;
		}	
	} else {
		while (list_element != NULL) {
			printf("%s%s%s\n", list_element->title, SEPARATOR, list_element->content);
			list_element = list_element->next;
		}	
	}
}

fetchline *init_fetchline(char *icon, char *title, char *content) {
	fetchline *new_fetchline = (fetchline *)malloc(sizeof(fetchline));
	new_fetchline->icon = icon;
	new_fetchline->title = title;
	new_fetchline->next = NULL;

	if (content != NULL) {
		strcpy(new_fetchline->content, content);
	}

	return new_fetchline;
}

void free_fetchlist(fetchline *list_start) {
	fetchline *next_fetchline;
	while (list_start != NULL) {
		next_fetchline = list_start->next;
		free(list_start);
		list_start = next_fetchline;
	}
}

// TODO: Currently no checking for lengths, could possibly end up with strings too long and have bad things happen
void align_fetchlist(fetchline *list_start) {
	fetchline *current_line = list_start;
	size_t max_length = 0;
	size_t current_length;
	char buffer[TITLE_MAX];
	char padding[TITLE_MAX];

	while (current_line != NULL) {
		current_length = strlen(current_line->title);
		if (current_length > max_length) {
			max_length = current_length;
		}
		current_line = current_line->next;
	}
	current_line = list_start;
	while (current_line != NULL) {
		current_length = strlen(current_line->title);
		if (current_length < max_length) {
			for (size_t i = 0; i <= max_length - current_length; i++) {
				if (i == max_length - current_length) {
					padding[i] = '\0';
				} else {
					padding[i] = ' ';
				}
			}
			strcpy(buffer, current_line->title);
			sprintf(current_line->title, "%s%s", padding, buffer);
		}
		current_line = current_line->next;
	}
}
