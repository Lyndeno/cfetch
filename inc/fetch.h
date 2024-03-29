#include <stddef.h>
#include <stdlib.h>
void fetch_kernel(char *buffer, size_t buffer_size);
void fetch_architecture(char *buffer, size_t buffer_size);
void fetch_hostname(char *buffer, size_t buffer_size);
void fetch_uptime(char *buffer, size_t buffer_size);
void fetch_cpumodel(char *buffer, size_t buffer_size);
void fetch_distro(char *buffer, size_t buffer_size);
void fetch_colourblocks(size_t indent, char *buffer, size_t buffer_size);
void fetch_colour(char *buffer, size_t buffer_size);
void fetch_memory(char *buffer, size_t buffer_size);
void fetch_model(char *buffer, size_t buffer_size);

