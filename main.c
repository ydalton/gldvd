#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include "image.h"
#include "app.h"

static void usage(int code, const char *name)
{
	printf("usage: %s [-f] [-i IMAGE]\n", name);
	exit(code);
}

int main(int argc, char **argv)
{
	GLFWmonitor *mon = NULL;
	struct application *app;
	int c;

	app = app_alloc();
	app->width = W_WIDTH;
	app->height = W_HEIGHT;
	app->name = W_NAME;
	app->image_name = DEFAULT_IMAGE;
	app->fullscreen = 0;

	while((c = getopt(argc, argv, "fi:")) != -1) {
		switch(c) {
		case 'f':
			app->fullscreen = 1;
			break;
		case 'i':
			if(file_is_png(optarg))
				app->image_name = optarg;
			else {
				fprintf(stderr, "Specified image is not a PNG. Quitting...\n");
				exit(-1);
			}
			break;
		case '?':
			usage(-1, app->name);
			break;
		default:
			usage(-1, app->name);
		}
	}

	app_init();

	if(app->fullscreen)
		mon = glfwGetPrimaryMonitor();

	app->window = glfwCreateWindow(app->width, app->height, app->name, 
				       mon, NULL);
	if(!app->window) {
		fprintf(stderr, "Failed to create window!\n");
		return -1;
	}

	app_loop(app);
	app_destroy(app);

	return 0;
}
