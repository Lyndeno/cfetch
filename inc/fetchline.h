#include <string.h>
#include <stdbool.h>

#define SEPARATOR ": "
#define TITLE_MAX 32
#define ICON_MAX 4 // TODO: I have no idea how bytes glyphs like fontawesome and nerdfonts are

typedef struct fetchlist {
	char icon[ICON_MAX];
	char title[TITLE_MAX];
	void (*fetchfunc)(char *, size_t);
} fetchlist;

void align_fetchlist(fetchlist *fetcharray, size_t count);
