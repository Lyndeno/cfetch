#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <math.h>

#define SEPARATOR ": "
#define CONTENT_MAX 64 // TODO: Implement dynamic string allocation for muh minimal resource usage
#define TITLE_MAX 32

void format_time(char *, long);

typedef struct fetchline {
	char *title;
	char *content;
	struct fetchline *next;
} fetchline;

fetchline *init_fetchline(void);
void free_fetchline(fetchline *);
void append_fetchline(fetchline *, fetchline *);

int main(void) {
	struct utsname local_machine;
	uname(&local_machine);
	printf(" Kernel: %s %s %s\n", local_machine.sysname, local_machine.release, local_machine.machine);

	char hostname[CONTENT_MAX+1];
	gethostname(hostname, CONTENT_MAX+ 1);
	printf("   Host: %s\n",hostname);

	struct sysinfo machine_info;
	sysinfo(&machine_info);
	//printf(" Uptime: %ld\n", machine_info.uptime);
	char *uptime_buffer;
	uptime_buffer = (char *)malloc(50*sizeof(char));
	format_time(uptime_buffer, machine_info.uptime);
	printf(" Uptime: %s\n", uptime_buffer); 
	free(uptime_buffer);
	// TODO: Memory reported from sysinfo is inaccurate, consider parsing /proc/meminfo
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
