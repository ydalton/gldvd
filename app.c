#include <stdlib.h>
#include <GLFW/glfw3.h>

#include "app.h"

void app_init(void) 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

struct application *app_alloc(void)
{
	struct application *app;

	app = (struct application *) calloc(1, sizeof(*app));

	return app;
}

void app_free(struct application *app)
{
	if(!app)
		return;
	free(app);
}

void app_destroy(struct application *app)
{
	app_free(app);
	glfwTerminate();
}
