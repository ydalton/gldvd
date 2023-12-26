#ifndef UTIL_H_
#define UTIL_H_

#define GL_UNIFORM_MATRIX_4FV(shader, name, a, b, ptr) 			       \
		(glUniformMatrix4fv(glGetUniformLocation(shader, name), a,     \
				   b, ptr))
#define GL_UNIFORM_4F(shader, name, r, g, b, a) 			       \
		(glUniform4f(glGetUniformLocation(shader, name), r, g, b, a))

#endif /* UTIL_H_ */
