#include <stdio.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <math.h>

#define HOST_NAME_MAX 64 // TODO: Figure out how to get this from the system

int main(void) {
	struct utsname local_machine;
	uname(&local_machine);
	printf(" Kernel: %s %s %s\n", local_machine.sysname, local_machine.release, local_machine.machine);

	char hostname[HOST_NAME_MAX+1];
	gethostname(hostname, HOST_NAME_MAX + 1);
	printf("   Host: %s\n",hostname);

	struct sysinfo machine_info;
	sysinfo(&machine_info);
	printf(" Uptime: %ld\n", machine_info.uptime);
	// TODO: Memory reported from sysinfo is inaccurate, consider parsing /proc/meminfo
	//printf("Buffer ram: %f\n", (machine_info.bufferram) / powf(1024,3));
	//printf("mem unit: %u", machine_info.mem_unit);
	//printf("RAM: %f / %f\n", (float)(machine_info.totalram - machine_info.freeram - machine_info.bufferram) / powf(1024,3), (float)machine_info.totalram / powf(1024,3));
}
