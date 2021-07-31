#include "fetch.h"
#include "file.h"
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SECONDS_MIN 60
#define SECONDS_HOUR 3600
#define SECONDS_DAY 86400

void fetch_kernel(char *buffer, size_t buffer_size) {
	struct utsname local_machine;
	uname(&local_machine);
	sprintf(buffer ,"%s %s %s", local_machine.sysname, local_machine.release, local_machine.machine );
}

void fetch_hostname(char *buffer, size_t buffer_size) {
	gethostname(buffer, buffer_size);
}

void fetch_uptime(char *buffer, size_t buffer_size) {
	struct sysinfo machine_info;
	sysinfo(&machine_info);

	// TODO: Tweak logic so that words aren't always plural
	if (machine_info.uptime < SECONDS_MIN) {
		sprintf(buffer, "%ld seconds", machine_info.uptime);
	} else if (machine_info.uptime >= SECONDS_MIN && machine_info.uptime < SECONDS_HOUR) {
		long uptime_minutes = machine_info.uptime / SECONDS_MIN;
		long uptime_remaining_seconds = machine_info.uptime - uptime_minutes*SECONDS_MIN;
		sprintf(buffer, "%ld minutes, %ld seconds", uptime_minutes, uptime_remaining_seconds);
	} else if (machine_info.uptime >= SECONDS_HOUR && machine_info.uptime < SECONDS_DAY ) {
		long uptime_hours = machine_info.uptime / SECONDS_HOUR;
		long uptime_minutes = (machine_info.uptime - uptime_hours*SECONDS_HOUR)/SECONDS_MIN;
		long uptime_remaining_seconds = machine_info.uptime - uptime_hours*SECONDS_HOUR - uptime_minutes*SECONDS_MIN; 
		sprintf(buffer, "%ld hours, %ld minutes, %ld seconds", uptime_hours, uptime_minutes, uptime_remaining_seconds);
	}
	else {
		long uptime_days = machine_info.uptime / SECONDS_DAY;
		long uptime_hours = (machine_info.uptime - uptime_days*SECONDS_DAY)/SECONDS_HOUR;
		long uptime_minutes = (machine_info.uptime - uptime_hours*SECONDS_HOUR - uptime_days*SECONDS_DAY)/SECONDS_MIN;
		long uptime_remaining_seconds = machine_info.uptime - uptime_minutes*SECONDS_MIN - uptime_hours*SECONDS_HOUR - uptime_days*SECONDS_DAY;
		sprintf(buffer, "%ld days, %ld hours, %ld minutes, %ld seconds", uptime_days, uptime_hours, uptime_minutes, uptime_remaining_seconds);
	}
}

void fetch_cpumodel(char *buffer, size_t buffer_size) {
	FILE *cpuinfo = fopen("/proc/cpuinfo", "rb");
	procParse(cpuinfo, buffer, "model name");
	fclose(cpuinfo);
}

void fetch_memory(char *buffer, size_t buffer_size) {
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

void fetch_model(char *buffer, size_t buffer_size) {
	FILE *model = fopen("/sys/devices/virtual/dmi/id/product_name", "rb");	
	char *modelname = readFirstline(model);
	strcpy(buffer, modelname);
	free(modelname);
	fclose(model);
}
