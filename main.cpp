#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <setjmp.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "image.h"
#include "util.h"

#include "vert.glsl.h"
#include "frag.glsl.h"

#define W_WIDTH 	1920
#define W_HEIGHT 	1080
#define W_NAME		"gldvd"
#define DEFAULT_IMAGE	"dvd.png"

static jmp_buf jmpbuf;

struct application {
	GLFWwindow *window;
	int width, height;
	const char *name;
	const char *image_name;
	unsigned int program;
	unsigned int fullscreen;
};

struct box {
	float *x, *y, *size_x, *size_y;
};

static void app_init(void) 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

static struct application *app_alloc(void)
{
	struct application *app;

	app = (struct application *) calloc(1, sizeof(*app));

	return app;
}

static void app_free(struct application *app)
{
	if(!app)
		return;
	free(app);
}

void app_resize(GLFWwindow *win, int width, int height) 
{
	(void) win;
	glViewport(0, 0, width, height);
}

static void app_destroy(struct application *app)
{
	app_free(app);
	glfwTerminate();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action,
			 int mods)
{
	(void) window;
	(void) scancode;
	(void) mods;
	/* press q to quit */
	if(key == GLFW_KEY_Q && action == GLFW_PRESS)
		longjmp(jmpbuf, 1);
}

static void change_color(glm::vec3 &col)
{
	float time = glfwGetTime() * 3;

	col = glm::vec3(abs(cos(time*2)), abs(sin(time/3)), abs(cos(time*7)));
}

static void app_loop(struct application *app)
{
	GLFWwindow *win = app->window;
	struct box *dvd;
	unsigned int vbo, vao, texture;
	unsigned int width, height;
	char *buf;

	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 movement = glm::vec3(0.031f, 0.037f, 0.0f);
	glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

	float verts[] = {
		0.5f,   0.5f, 0.0f, 1.0f, 1.0f,
		0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
	};

	glfwMakeContextCurrent(win);

	if(glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialized GLEW!\n");
		exit(EXIT_FAILURE);
	}

	glViewport(0, 0, app->width, app->height);

	glfwSetFramebufferSizeCallback(app->window, app_resize);
	glfwSetKeyCallback(app->window, key_callback);
	if(app->fullscreen)
		glfwSetInputMode(app->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	buf = png_to_fb(app->image_name, &width, &height);
	if(!buf) {
		fprintf(stderr, "Failed to load image!\n");
		return;
	}

	app->program = create_program(vert_glsl, frag_glsl);
	if(!app->program) {
		fprintf(stderr, "Failed to create program!\n");
		return;
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenTextures(1, &texture);

	/* texture stuff */
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, 
		     GL_UNSIGNED_BYTE, buf);
	glGenerateMipmap(GL_TEXTURE_2D);

	free(buf);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 
			      NULL);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 
			      (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glUseProgram(app->program);

	glBindVertexArray(vao);

	model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));

	dvd = (struct box *) calloc(1, sizeof(*dvd));
	dvd->size_x = &model[0][0];
	dvd->size_y = dvd->size_x;
	dvd->x = &model[3][0];
	dvd->y = &model[3][1];

#define COLLISION(x, size_x) ((x + size_x/2) > 1.0f || (x - size_x/2) < -1.0f)

	while(!glfwWindowShouldClose(win) && !setjmp(jmpbuf)) {
		glfwPollEvents();
		model = glm::translate(model, movement);
		GL_UNIFORM_MATRIX_4FV(app->program, "model", 1, GL_FALSE, 
				      glm::value_ptr(model));
		GL_UNIFORM_4F(app->program, "ourColor", color.x, color.y, color.z, 
			      1.0f);

		if(COLLISION(*(dvd->x), *(dvd->size_x))) {
			movement[0] *= -1.0f;
			change_color(color);
		}
		if(COLLISION(*(dvd->y), *(dvd->size_y))) {
			movement[1] *= -1.0f;
			change_color(color);
		}
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(win);
	}

	glDeleteProgram(app->program);
	glDeleteTextures(1, &vao);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

#undef COLLISION


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
