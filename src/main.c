#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "fetchline.h"
#include <stdbool.h>
#include "proc.h"

void format_time(char *, long);

int main(void) {
	fetchline *list_start;

	// Get kernel information
	fetchline *fetch_kernel = init_fetchline();
	list_start = fetch_kernel;
	struct utsname local_machine;
	uname(&local_machine);
	sprintf(fetch_kernel->title, "Kernel");
	sprintf(fetch_kernel->icon, "");
	sprintf(fetch_kernel->content,"%s %s %s", local_machine.sysname, local_machine.release, local_machine.machine );

	// Get hostname
	fetchline *fetch_hostname = init_fetchline();
	char hostname[CONTENT_MAX+1];
	gethostname(hostname, CONTENT_MAX+ 1);
	sprintf(fetch_hostname->title, "Host");
	sprintf(fetch_hostname->icon, "");
	sprintf(fetch_hostname->content, "%s", hostname);
	append_fetchline(list_start, fetch_hostname);

	// Get uptime
	struct sysinfo machine_info;
	sysinfo(&machine_info);
	fetchline *fetch_uptime = init_fetchline();
	sprintf(fetch_uptime->title, "Uptime");
	sprintf(fetch_uptime->icon, "");
	format_time(fetch_uptime->content, machine_info.uptime);
	append_fetchline(list_start, fetch_uptime);

	// Get CPU model
	FILE *cpuinfo = fopen("/proc/cpuinfo", "rb");
	fetchline *fetch_cpuname = init_fetchline();
	sprintf(fetch_cpuname->title, "CPU");
	sprintf(fetch_cpuname->icon, "");
	char *cpumodel = procParse(cpuinfo, "model name");
	sprintf(fetch_cpuname->content, "%s", cpumodel);
	append_fetchline(list_start, fetch_cpuname);
	free(cpumodel);
	fclose(cpuinfo);
	

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


