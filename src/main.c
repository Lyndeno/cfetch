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

#define BUFFER_SIZE 64

#define SECONDS_MIN 60
#define SECONDS_HOUR 3600
#define SECONDS_DAY 86400
void format_time(char *, long);
unsigned long kBtoMiB(unsigned long kBytes);
char *readFirstline(FILE *f);

void fetch_kernel(char *buffer);
void fetch_hostname(char *buffer);
void fetch_uptime(char *buffer);
void fetch_cpumodel(char *buffer);
void fetch_memory(char *buffer);
void fetch_model(char *buffer);

typedef struct fetchentry {
	char icon[ICON_MAX];
	char title[TITLE_MAX];
	void (*fetchfunc)(char *buffer);
} fetchentry;

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

	fetchentry fetchlist[] = {
		{"", "Kernel", &fetch_kernel},
		{"", "Host", &fetch_hostname},
		{"", "Uptime", &fetch_uptime},
		{"", "CPU", &fetch_cpumodel},
		{"", "Mem", &fetch_memory},
		{"", "Model", &fetch_model}
	};

	char buffer[BUFFER_SIZE];

	// Get kernel information
	fetch_kernel(buffer);
	fetchline *list_start = init_fetchline(fetchlist[0].icon, fetchlist[0].title, buffer);
	fetchline *list_end = list_start;

	for (size_t i = 1; i < sizeof(fetchlist) / sizeof(fetchentry); i++) {
		fetchlist[i].fetchfunc(buffer);
		append_fetchline(&list_end, fetchlist[i].icon, fetchlist[i].title, buffer);
	}

	align_fetchlist(list_start);
	print_fetch(list_start, useIcons);

	free_fetchlist(list_start);
}

void format_time(char *buffer, long uptime_seconds) {
	// TODO: Tweak logic so that words aren't always plural
	if (uptime_seconds < SECONDS_MIN) {
		sprintf(buffer, "%ld seconds", uptime_seconds);
	} else if (uptime_seconds >= SECONDS_MIN && uptime_seconds < SECONDS_HOUR) {
		long uptime_minutes = uptime_seconds / SECONDS_MIN;
		long uptime_remaining_seconds = uptime_seconds - uptime_minutes*SECONDS_MIN;
		sprintf(buffer, "%ld minutes, %ld seconds", uptime_minutes, uptime_remaining_seconds);
	} else if (uptime_seconds >= SECONDS_HOUR && uptime_seconds < SECONDS_DAY ) {
		long uptime_hours = uptime_seconds / SECONDS_HOUR;
		long uptime_minutes = (uptime_seconds - uptime_hours*SECONDS_HOUR)/SECONDS_MIN;
		long uptime_remaining_seconds = uptime_seconds - uptime_hours*SECONDS_HOUR - uptime_minutes*SECONDS_MIN; 
		sprintf(buffer, "%ld hours, %ld minutes, %ld seconds", uptime_hours, uptime_minutes, uptime_remaining_seconds);
	}
	else {
		long uptime_days = uptime_seconds / SECONDS_DAY;
		long uptime_hours = (uptime_seconds - uptime_days*SECONDS_DAY)/SECONDS_HOUR;
		long uptime_minutes = (uptime_seconds - uptime_hours*SECONDS_HOUR - uptime_days*SECONDS_DAY)/SECONDS_MIN;
		long uptime_remaining_seconds = uptime_seconds - uptime_minutes*SECONDS_MIN - uptime_hours*SECONDS_HOUR - uptime_days*SECONDS_DAY;
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

void fetch_hostname(char *buffer) {
	gethostname(buffer, BUFFER_SIZE);
}

void fetch_uptime(char *buffer) {
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
