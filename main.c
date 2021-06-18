#include <stdio.h>
#include <sys/utsname.h>

int main(void) {
	struct utsname local_machine;
	uname(&local_machine);
	printf(local_machine.sysname);
//printf(local_machine->nodename);
	printf(local_machine.release);
//	printf(local_machine.version);
}
