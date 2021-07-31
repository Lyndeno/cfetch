#include <string.h>
#include <stdbool.h>

#define SEPARATOR ": "
#define CONTENT_MAX 64 // TODO: Implement dynamic string allocation for muh minimal resource usage
#define TITLE_MAX 32
#define ICON_MAX 4 // TODO: I have no idea how bytes glyphs like fontawesome and nerdfonts are

typedef struct fetchlist {
	char icon[ICON_MAX];
	char title[TITLE_MAX];
	void (*fetchfunc)(char *buffer);
} fetchlist;

void align_fetchlist(fetchlist *fetcharray, size_t count);
