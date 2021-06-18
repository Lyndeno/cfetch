#include <stdio.h>
#include <sys/utsname.h>
#include <unistd.h>

#define HOST_NAME_MAX 64 // TODO: Figure out how to get this from the system

int main(void) {
	struct utsname local_machine;
	uname(&local_machine);
	printf("%s %s\n", local_machine.sysname, local_machine.release);

	char hostname[HOST_NAME_MAX+1];
	gethostname(hostname, HOST_NAME_MAX + 1);
	printf(hostname);
}