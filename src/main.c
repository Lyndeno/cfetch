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
char *readFirstline(FILE *f);

void fetch_kernel(char *buffer);
void fetch_uptime(char *buffer);
void fetch_cpumodel(char *buffer);
void fetch_memory(char *buffer);
void fetch_model(char *buffer);

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

	char buffer[64];

	// Get kernel information
	fetch_kernel(buffer);
	fetchline *list_start = init_fetchline("", "Kernel", buffer);
	fetchline *list_end = list_start;

	// Get hostname
	gethostname(buffer, sizeof(buffer));
	list_end = append_fetchline(list_end, "", "Host", buffer);

	// Get uptime
	fetch_uptime(buffer);
	list_end = append_fetchline(list_end, "", "Uptime", buffer);

	// Get CPU model
	fetch_cpumodel(buffer);
	list_end = append_fetchline(list_end, "", "CPU", buffer);
	
	// Get Mem info
	fetch_memory(buffer);
	list_end = append_fetchline(list_end, "", "Mem", buffer);

	fetch_model(buffer);
	list_end = append_fetchline(list_end, "", "Model", buffer);

	align_fetchlist(list_start);
	print_fetch(list_start, useIcons);

	free_fetchlist(list_start);
	// TODO: Memory reported from sysinfo is inaccurate, consider parsing /proc/meminfo
}

void format_time(char *buffer, long uptime_seconds) {
	// TODO: Tweak logic so that words aren't always plural
	if (uptime_seconds < 60) {
		sprintf(buffer, "%ld seconds", uptime_seconds);
	} else if (uptime_seconds >= 60 && uptime_seconds < 3600) {
		long uptime_minutes = uptime_seconds / 60;
		long uptime_remaining_seconds = uptime_seconds - uptime_minutes*60;
		sprintf(buffer, "%ld minutes, %ld seconds", uptime_minutes, uptime_remaining_seconds);
	} else if (uptime_seconds >= 3600 && uptime_seconds < 3600*24 ) {
		long uptime_hours = uptime_seconds / 3600;
		long uptime_minutes = (uptime_seconds - uptime_hours*3600)/60;
		long uptime_remaining_seconds = uptime_seconds - uptime_hours*3600 - uptime_minutes*60; 
		sprintf(buffer, "%ld hours, %ld minutes, %ld seconds", uptime_hours, uptime_minutes, uptime_remaining_seconds);
	}
	else {
		long uptime_days = uptime_seconds / (3600 * 24);
		long uptime_hours = (uptime_seconds - uptime_days*3600*24)/3600;
		long uptime_minutes = (uptime_seconds - uptime_hours*3600 - uptime_days*3600*24)/60;
		long uptime_remaining_seconds = uptime_seconds - uptime_minutes*60 - uptime_hours*3600 - uptime_days*3600*24;
		sprintf(buffer, "%ld days, %ld hours, %ld minutes, %ld seconds", uptime_days, uptime_hours, uptime_minutes, uptime_remaining_seconds);
	}
}

char *readFirstline(FILE *f) {
	char *line = NULL;
	size_t size = 0;
	getline(&line, &size, f);
	*strstr(line, "\n") = '\0';
	return line;
}

void fetch_kernel(char *buffer) {
	struct utsname local_machine;
	uname(&local_machine);
	sprintf(buffer ,"%s %s %s", local_machine.sysname, local_machine.release, local_machine.machine );
}

void fetch_uptime(char* buffer) {
	struct sysinfo machine_info;
	sysinfo(&machine_info);
	format_time(buffer, machine_info.uptime);
}

void fetch_cpumodel(char *buffer) {
	FILE *cpuinfo = fopen("/proc/cpuinfo", "rb");
	procParse(cpuinfo, buffer, "model name");
	fclose(cpuinfo);
}

void fetch_memory(char *buffer) {
	FILE *meminfo = fopen("/proc/meminfo", "rb");
	// TODO: CHange these arbitrary values to something a little more known
	char memtotal[16]; procParse(meminfo, memtotal, "MemTotal");
	char memavail[16]; procParse(meminfo, memavail, "MemAvailable");
	unsigned long totalkB = strtoul(memtotal, NULL, 10);
	unsigned long availkB = strtoul(memavail, NULL, 10);

	char memLine[strlen(memtotal) + strlen(memavail) + 3];
	sprintf(buffer, "%.2fGiB / %.2fGiB", (float)(totalkB - availkB) / 1024 / 1024, (float)totalkB / 1024 / 1024);
	fclose(meminfo);
}

void fetch_model(char *buffer) {
	FILE *model = fopen("/sys/devices/virtual/dmi/id/product_name", "rb");	
	char *modelname = readFirstline(model);
	strcpy(buffer, modelname);
	free(modelname);
	fclose(model);
}
