#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#define SEPARATOR ": "
#define CONTENT_MAX 64 // TODO: Implement dynamic string allocation for muh minimal resource usage
#define TITLE_MAX 32
#define ICON_MAX 8 // TODO: I have no idea how bytes glyphs like fontawesome and nerdfonts are

void format_time(char *, long);

typedef struct fetchline {
	char *icon;
	char *title;
	char *content;
	struct fetchline *next;
} fetchline;

fetchline *init_fetchline(void);
void free_fetchline(fetchline *);
void append_fetchline(fetchline *list_element, fetchline *new_fetchline);
void print_fetch(fetchline *list_element);
void free_fetchlist(fetchline *list_start);
void align_fetchlist(fetchline *list_start);

int main(void) {
	fetchline *list_start;

	fetchline *fetch_kernel = init_fetchline();
	list_start = fetch_kernel;
	struct utsname local_machine;
	uname(&local_machine);
	sprintf(fetch_kernel->title, "Kernel");
	sprintf(fetch_kernel->icon, "");
	sprintf(fetch_kernel->content,"%s %s %s", local_machine.sysname, local_machine.release, local_machine.machine );

	fetchline *fetch_hostname = init_fetchline();
	char hostname[CONTENT_MAX+1];
	gethostname(hostname, CONTENT_MAX+ 1);
	sprintf(fetch_hostname->title, "Host");
	sprintf(fetch_hostname->icon, "");
	sprintf(fetch_hostname->content, "%s", hostname);
	append_fetchline(list_start, fetch_hostname);

	struct sysinfo machine_info;
	sysinfo(&machine_info);
	fetchline *fetch_uptime = init_fetchline();
	sprintf(fetch_uptime->title, "Uptime");
	sprintf(fetch_uptime->icon, "");
	format_time(fetch_uptime->content, machine_info.uptime);
	append_fetchline(list_start, fetch_uptime);

	align_fetchlist(list_start);
	print_fetch(list_start);

	free_fetchlist(list_start);
	// TODO: Memory reported from sysinfo is inaccurate, consider parsing /proc/meminfo
}

void print_fetch(fetchline *list_element) {
	while (list_element != NULL) {
		printf("%s %s%s%s\n", list_element->icon, list_element->title, SEPARATOR, list_element->content);
		list_element = list_element->next;
	}	
}

void format_time(char *buffer, long uptime_seconds) {
	// TODO: Move buffer allocation out of this function, we don't want memory leaks later
	if (uptime_seconds < 60) {
		sprintf(buffer, "%ld seconds", uptime_seconds);
	} else if (uptime_seconds >= 60 && uptime_seconds < 3600) {
		long uptime_minutes = uptime_seconds / 60;
		long uptime_remaining_seconds = uptime_seconds - uptime_minutes*60;
		sprintf(buffer, "%ld minutes, %ld seconds", uptime_minutes, uptime_remaining_seconds);
	}
	else {
		long uptime_hours = uptime_seconds / 3600;
		long uptime_minutes = (uptime_seconds - uptime_hours*3600)/60;
		long uptime_remaining_seconds = uptime_seconds - uptime_hours*3600 - uptime_minutes*60; 
		sprintf(buffer, "%ld hours, %ld minutes, %ld seconds", uptime_hours, uptime_minutes, uptime_remaining_seconds);
	}
}

fetchline *init_fetchline(void) {
	fetchline *new_fetchline = (fetchline *)malloc(sizeof(fetchline));
	new_fetchline->content = (char *)malloc(CONTENT_MAX*sizeof(char));
	new_fetchline->title = (char *)malloc(TITLE_MAX*sizeof(char));
	new_fetchline->icon = (char *)malloc(ICON_MAX*sizeof(char));
	new_fetchline->next = NULL;

	return new_fetchline;
}

void free_fetchline(fetchline *garbage) {
	free(garbage->content);
	free(garbage->title);
	free(garbage);
}

void append_fetchline(fetchline *list_element, fetchline *new_fetchline) {
	while (list_element->next != NULL) {
		list_element = list_element->next;
	}
	list_element->next = new_fetchline;
}

void free_fetchlist(fetchline *list_start) {
	fetchline *next_fetchline;
	while (list_start != NULL) {
		next_fetchline = list_start->next;
		free_fetchline(list_start);
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
