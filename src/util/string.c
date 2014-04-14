#include "oboe/util/string.h"

uint32_t kstrlen(const char *str) {
	const char *s;
	for (s = str; *s; ++s);
	return (s - str);
}

int kstrcmp(const char *s1, const char *s2) {
	while (*s1 && (*s1 == *s2))
		s1++, s2++;
	return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}