#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <math.h>

#define HOST_NAME_MAX 64 // TODO: Figure out how to get this from the system

void format_time(char *, long);

int main(void) {
	struct utsname local_machine;
	uname(&local_machine);
	printf(" Kernel: %s %s %s\n", local_machine.sysname, local_machine.release, local_machine.machine);

	char hostname[HOST_NAME_MAX+1];
	gethostname(hostname, HOST_NAME_MAX + 1);
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
