#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fetchline.h"
#include <stdbool.h>
#include "fetch.h"

#define BUFFER_SIZE 64

int main(int argc, char *argv[]) {
	bool useIcons = false;

	// Parse Commandline arg
	// I want to use Argp but that won't work on macos or windows apparently.
	// Maybe I should write my own arg parser
	int opt;
	while ((opt = getopt(argc, argv, "i")) != -1) {
		switch (opt) {
			case 'i': useIcons = true; break;
			default: break;
		}
	}

	fetchlist fetcharray[] = {
		{"", "Kernel", &fetch_kernel},
		{"", "Arch", &fetch_architecture},
		{"", "Host", &fetch_hostname},
		{"", "Uptime", &fetch_uptime},
		{"", "CPU", &fetch_cpumodel},
		{"", "Mem", &fetch_memory},
		{"", "Model", &fetch_model}
	};

	char buffer[BUFFER_SIZE];

	align_fetchlist(fetcharray, sizeof(fetcharray) / sizeof(fetchlist));

	for (size_t i = 0; i < sizeof(fetcharray) / sizeof(fetchlist); i++) {
		fetcharray[i].fetchfunc(buffer, sizeof(buffer)/sizeof(char));

		if(useIcons) printf("%s ", fetcharray[i].icon);
		printf("%s%s%s\n", fetcharray[i].title, SEPARATOR, buffer);
	}
}
