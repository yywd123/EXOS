#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void *
malloc(size_t size);

void
free(void *p);

int
puts(const char *str);

#ifdef __cplusplus
}
#endif