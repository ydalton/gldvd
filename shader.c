#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>

unsigned int compile_shader(const char *src, int type)
{
	unsigned int shader;
	int compile_ok;

	shader = glCreateShader(type);

	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);
	if(!compile_ok) {
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

unsigned int create_program(const char *vert_src, const char *frag_src)
{
	unsigned int program, frag_shader, vert_shader;
	int link_ok;
	char *log;

	vert_shader = compile_shader(vert_src, GL_VERTEX_SHADER);
	if(!vert_shader) {
		fprintf(stderr, "Failed to compile vertex shader!\n");
		return 0;
	}

	frag_shader = compile_shader(frag_src, GL_FRAGMENT_SHADER);
	if(!frag_shader) {
		fprintf(stderr, "Failed to compile fragment shader!\n");
		return 0;
	}

	program = glCreateProgram();
	glAttachShader(program, frag_shader);
	glAttachShader(program, vert_shader);
	glLinkProgram(program);

	log = (char *) calloc(512, sizeof(char));

	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if(!link_ok) {
		glGetProgramInfoLog(program, 512, NULL, log);
		fprintf(stderr, "Failed to link shader: %s\n", log);
		glDeleteProgram(program);
		program = 0;
	}

	free(log);
	
	glDeleteShader(frag_shader);
	glDeleteShader(vert_shader);

	return program;
}
