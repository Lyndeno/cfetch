#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fetchline.h"
#include <stdbool.h>
#include "fetch.h"

#define BUFFER_SIZE 512
#define COLOUR_RESET "\e[0m"

int main(int argc, char *argv[]) {
	bool useIcons = false;
	bool useColours = true;

	// Parse Commandline arg
	// I want to use Argp but that won't work on macos or windows apparently.
	// Maybe I should write my own arg parser
	int opt;
	while ((opt = getopt(argc, argv, "ic")) != -1) {
		switch (opt) {
			case 'i': useIcons = true; break;
			case 'c': useColours = false; break;
			default: break;
		}
	}

	fetchlist fetcharray[] = {
		{"", "Distro", &fetch_distro},
		{"", "Kernel", &fetch_kernel},
		{"", "Architecture", &fetch_architecture},
		{"", "Hostname", &fetch_hostname},
		{"", "Uptime", &fetch_uptime},
		{"", "CPU", &fetch_cpumodel},
		{"", "Memory", &fetch_memory},
		{"", "Model", &fetch_model}
	};

	char buffer[BUFFER_SIZE];
	char colour[32];
	size_t indent;

	if (useColours) fetch_colour(colour, sizeof(colour)/sizeof(char));
	else colour[0] = '\0';

	indent = align_fetchlist(fetcharray, sizeof(fetcharray) / sizeof(fetchlist));

	for (size_t i = 0; i < sizeof(fetcharray) / sizeof(fetchlist); i++) {
		buffer[0] = '\0';
		fetcharray[i].fetchfunc(buffer, sizeof(buffer)/sizeof(char));

		if(useIcons) printf("%s%s%s ", colour, fetcharray[i].icon, COLOUR_RESET);
		printf("%s%s%s%s%s\n", colour, fetcharray[i].title, COLOUR_RESET, SEPARATOR, buffer);
	}
	fetch_colourblocks(indent, buffer, sizeof(buffer) / sizeof(char));
	printf("%s\n", buffer);
}
