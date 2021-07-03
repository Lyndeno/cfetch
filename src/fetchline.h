#include <string.h>

typedef struct fetchline {
	char *icon;
	char *title;
	char *content;
	struct fetchline *next;
} fetchline;

#define SEPARATOR ": "
#define CONTENT_MAX 64 // TODO: Implement dynamic string allocation for muh minimal resource usage
#define TITLE_MAX 32
#define ICON_MAX 4 // TODO: I have no idea how bytes glyphs like fontawesome and nerdfonts are

fetchline *init_fetchline(void);
void free_fetchline(fetchline *);
void append_fetchline(fetchline *list_element, fetchline *new_fetchline);
void print_fetch(fetchline *list_element);
void free_fetchlist(fetchline *list_start);
void align_fetchlist(fetchline *list_start);


