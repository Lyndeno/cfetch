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
#include <unistd.h>

void format_time(char *, long);
unsigned long kBtoMiB(unsigned long kBytes);

int main(int argc, char *argv[]) {
	bool useIcons = false;

	// Parse Commandline arg
	// I want to use Argp but that won't work on macos or windows apparently.
	// Maybe I should write my own arg parser
	bool isCaseInsensitive = false;
	int opt;
	while ((opt = getopt(argc, argv, "i")) != -1) {
		switch (opt) {
			case 'i': useIcons = true; break;
			default: break;
		}
	}

	fetchline *list_start;

	// Get kernel information
	fetchline *fetch_kernel = init_fetchline("", "Kernel", NULL);
	list_start = fetch_kernel;
	struct utsname local_machine;
	uname(&local_machine);
	sprintf(fetch_kernel->content,"%s %s %s", local_machine.sysname, local_machine.release, local_machine.machine );

	// Get hostname
	char hostname[CONTENT_MAX+1];
	gethostname(hostname, CONTENT_MAX+ 1);
	fetchline *fetch_hostname = init_fetchline("", "Host", hostname);
	append_fetchline(list_start, fetch_hostname);

	// Get uptime
	struct sysinfo machine_info;
	sysinfo(&machine_info);
	fetchline *fetch_uptime = init_fetchline("", "Uptime", NULL);
	format_time(fetch_uptime->content, machine_info.uptime);
	append_fetchline(list_start, fetch_uptime);

	// Get CPU model
	FILE *cpuinfo = fopen("/proc/cpuinfo", "rb");
	char *cpumodel = procParse(cpuinfo, "model name");
	fetchline *fetch_cpuname = init_fetchline("", "CPU", cpumodel);
	append_fetchline(list_start, fetch_cpuname);
	free(cpumodel);
	fclose(cpuinfo);
	
	// Get Mem info
	FILE *meminfo = fopen("/proc/meminfo", "rb");
	char *memtotal = procParse(cpuinfo, "MemTotal");
	char *memavail = procParse(meminfo, "MemAvailable");

	unsigned long totalkB = strtoul(memtotal, NULL, 10);
	unsigned long availkB = strtoul(memavail, NULL, 10);

	char memLine[strlen(memtotal) + strlen(memavail) + 3];
	sprintf(memLine, "%.2fGiB / %.2fGiB", (float)(totalkB - availkB) / 1024 / 1024, (float)totalkB / 1024 / 1024);

	fetchline *fetch_memtotal = init_fetchline("", "Mem", memLine);
	append_fetchline(list_start, fetch_memtotal);
	free(memtotal);
	free(memavail);
	fclose(meminfo);

	align_fetchlist(list_start);
	print_fetch(list_start, useIcons);

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

