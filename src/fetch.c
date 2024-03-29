#include "fetch.h"
#include "file.h"
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fetchline.h"

#define SECONDS_MIN 60
#define SECONDS_HOUR 3600
#define SECONDS_DAY 86400

#define COLOUR_RESET "\e[0m"

void fetch_kernel(char *buffer, const size_t buffer_size) {
	struct utsname local_machine;
	uname(&local_machine);
	sprintf(buffer ,"%s %s", local_machine.sysname, local_machine.release );
}

void fetch_architecture(char *buffer, const size_t buffer_size) {
	struct utsname local_machine;
	uname(&local_machine);
	sprintf(buffer, "%s", local_machine.machine);
}

void fetch_distro(char *buffer, const size_t buffer_size) {
	FILE *distroinfo = fopen("/etc/os-release", "rb");
	osParse(distroinfo, buffer, "PRETTY_NAME");
	fclose(distroinfo);
}

void fetch_colourblocks(size_t indent, char *buffer, const size_t buffer_size) {
	size_t offset = 0;
	for (int i = 0; i < 16; i++) {
		if (i == 8) {
			offset += sprintf(buffer + offset, "%s\n", COLOUR_RESET);
		}
		if (i == 0 || i == 8) {
			for (int j = 0; j < indent + strlen(SEPARATOR); j++) {
				offset += sprintf(buffer + offset, " ");
			}
		}
		if (i < 8) {
			offset += sprintf(buffer + offset, "\e[%dm\e[%dm   ", i + 30, i + 40);
		} else {
			offset += sprintf(buffer + offset, "\e[38;5;%dm\e[48;5;%dm   ", i, i);
		}
	}
	offset += sprintf(buffer + offset, "%s", COLOUR_RESET);
}

void fetch_colour(char *buffer, const size_t buffer_size) {
	FILE *distroinfo = fopen("/etc/os-release", "rb");
	strcpy(buffer, "\e[");
	osParse(distroinfo, buffer + strlen(buffer), "ANSI_COLOR");
	size_t old_length = strlen(buffer);
	buffer[old_length] = 'm';
	buffer[old_length + 1] = '\0';
	fclose(distroinfo);
}

void fetch_hostname(char *buffer, const size_t buffer_size) {
	gethostname(buffer, buffer_size);
}

void fetch_uptime(char *buffer, const size_t buffer_size) {
	struct sysinfo machine_info;
	sysinfo(&machine_info);

	char plural[] = "s";
	char singular[] = "";
	char *plurality = NULL;
	
	size_t bytes_written = 0;

	// TODO: Tweak logic so that words aren't always plural
	long uptime_days = machine_info.uptime / SECONDS_DAY;
	long uptime_hours = (machine_info.uptime - uptime_days*SECONDS_DAY)/SECONDS_HOUR;
	long uptime_minutes = (machine_info.uptime - uptime_hours*SECONDS_HOUR - uptime_days*SECONDS_DAY)/SECONDS_MIN;
	long uptime_seconds = machine_info.uptime - uptime_minutes*SECONDS_MIN - uptime_hours*SECONDS_HOUR - uptime_days*SECONDS_DAY;
	if (uptime_days > 0) {
		if (uptime_days != 1) { plurality = plural; } else { plurality = singular; }
		bytes_written += sprintf(buffer, "%ld day%s, ", uptime_days, plurality);
	} 
	if (uptime_hours > 0) {
		if (uptime_hours != 1) { plurality = plural; } else { plurality = singular; }
		bytes_written += sprintf(buffer + bytes_written, "%ld hour%s, ", uptime_hours, plurality);
	}
	if (uptime_minutes != 0) {
		if (uptime_minutes > 1) { plurality = plural; } else { plurality = singular; }
		bytes_written += sprintf(buffer + bytes_written, "%ld minute%s, ", uptime_minutes, plurality);
	}
	if (uptime_seconds != 1) { plurality = plural; } else { plurality = singular; }
	sprintf(buffer + bytes_written, "%ld second%s", uptime_seconds, plurality);
}

void fetch_cpumodel(char *buffer, const size_t buffer_size) {
	FILE *cpuinfo = fopen("/proc/cpuinfo", "rb");
	procParse(cpuinfo, buffer, "model name");
	fclose(cpuinfo);
}

void fetch_memory(char *buffer, const size_t buffer_size) {
	FILE *meminfo = fopen("/proc/meminfo", "rb");
	// TODO: CHange these arbitrary values to something a little more known
	char memtotal[16]; procParse(meminfo, memtotal, "MemTotal");
	char memavail[16]; procParse(meminfo, memavail, "MemAvailable");
	unsigned long totalkB = strtoul(memtotal, NULL, 10);
	unsigned long availkB = strtoul(memavail, NULL, 10);

	sprintf(buffer, "%.2fGiB / %.2fGiB", (float)(totalkB - availkB) / 1024 / 1024, (float)totalkB / 1024 / 1024);
	fclose(meminfo);
}

void fetch_model(char *buffer, const size_t buffer_size) {
	FILE *model = fopen("/sys/devices/virtual/dmi/id/product_name", "rb");	
	if (model != NULL) {char *modelname = readFirstline(model);
		strcpy(buffer, modelname);
		free(modelname);
		fclose(model);
	} else {
		sprintf(buffer, "");
	};
}
