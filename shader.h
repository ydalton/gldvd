#ifndef SHADER_H_
#define SHADER_H_

#ifdef __cplusplus
extern "C" {
#endif

unsigned int compile_shader(const char *src, int type);
unsigned int create_program(const char *vert_src, const char *frag_src);

#ifdef __cplusplus
}
#endif

#endif /* SHADER_H_ */
