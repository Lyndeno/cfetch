#include <string.h>
#include <stdbool.h>

#define SEPARATOR ": "
#define CONTENT_MAX 64 // TODO: Implement dynamic string allocation for muh minimal resource usage
#define TITLE_MAX 32
#define ICON_MAX 4 // TODO: I have no idea how bytes glyphs like fontawesome and nerdfonts are

typedef struct fetchline {
	char *icon;
	char *title;
	char content[CONTENT_MAX];
	struct fetchline *next;
} fetchline;

fetchline *init_fetchline(char *icon, char *title, char *content);
void print_fetch(fetchline *list_element, bool useIcons);
void free_fetchlist(fetchline *list_start);
void align_fetchlist(fetchline *list_start);

