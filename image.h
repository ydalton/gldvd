#ifndef IMAGE_H_
#define IMAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

char *png_to_fb(const char *filename, unsigned int *width, unsigned int *height);

#ifdef __cplusplus
}
#endif

#endif
