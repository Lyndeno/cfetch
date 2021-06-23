#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "fetchline.h"

void format_time(char *, long);

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


