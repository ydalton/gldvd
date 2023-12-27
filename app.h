#ifndef APP_H_
#define APP_H_

#define W_WIDTH 	1920
#define W_HEIGHT 	1080
#define W_NAME		"gldvd"
#define DEFAULT_IMAGE	"dvd.png"

#define GL_UNIFORM_MATRIX_4FV(shader, name, a, b, ptr) 			       \
		(glUniformMatrix4fv(glGetUniformLocation(shader, name), a,     \
				   b, ptr))
#define GL_UNIFORM_4F(shader, name, r, g, b, a) 			       \
		(glUniform4f(glGetUniformLocation(shader, name), r, g, b, a))

struct application {
	GLFWwindow *window;
	int width, height;
	const char *name;
	const char *image_name;
	unsigned int program;
	unsigned int fullscreen;
};

#ifdef __cplusplus
extern "C" {
#endif

void app_init(void);
struct application *app_alloc(void);
void app_free(struct application *app);
void app_destroy(struct application *app);

#ifdef __cplusplus
}
#endif

#endif
