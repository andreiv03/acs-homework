#include <string.h>

char* strcpy(char* destination, const char* source) {
	const char* start = destination;

	while (*source != '\0') {
		*destination = *source;
		destination = destination + 1;
		source = source + 1;
	}

	*destination = '\0';
	destination = (char*)start;

	return destination;
}

char* strncpy(char* destination, const char* source, size_t len) {
	const char* start = destination;
	size_t index = 0;

	while (*source != '\0' && index < len) {
		*destination = *source;
		destination = destination + 1;
		source = source + 1;
		index = index + 1;
	}

	if (index < len)
		*destination = '\0';

	destination = (char*)start;
	return destination;
}

char* strcat(char* destination, const char* source) {
	const char* start = destination;

	while (*destination != '\0')
		destination = destination + 1;

	while (*source != '\0') {
		*destination = *source;
		destination = destination + 1;
		source = source + 1;
	}

	*destination = '\0';
	destination = (char*)start;

	return destination;
}

char* strncat(char* destination, const char* source, size_t len) {
	const char* start = destination;
	size_t index = 0;

	while (*destination != '\0')
		destination = destination + 1;

	while (*source != '\0' && index < len) {
		*destination = *source;
		destination = destination + 1;
		source = source + 1;
		index = index + 1;
	}

	*destination = '\0';
	destination = (char*)start;

	return destination;
}

int strcmp(const char* str1, const char* str2) {
	while (*str1 != '\0' || *str2 != '\0') {
		if (*str1 != *str2)
			return *str1 - *str2;

		str1 = str1 + 1;
		str2 = str2 + 1;
	}

	return 0;
}

int strncmp(const char* str1, const char* str2, size_t len) {
	size_t index = 0;

	while ((*str1 != '\0' || *str2 != '\0') && index < len - 1) {
		if (*str1 != *str2)
			return *str1 - *str2;

		str1 = str1 + 1;
		str2 = str2 + 1;
		index = index + 1;
	}

	return 0;
}

size_t strlen(const char* str) {
	size_t i = 0;

	for (; *str != '\0'; str++, i++)
		;

	return i;
}

char* strchr(const char* str, int c) {
	while (*str != '\0') {
		if (*str == c)
			return (char*)str;

		str = str + 1;
	}

	return NULL;
}

char* strrchr(const char* str, int c) {
	const char* start = str;

	while (*str != '\0')
		str = str + 1;

	while (str != start - 1) {
		if (*str == c)
			return (char*)str;

		str = str - 1;
	}

	return NULL;
}

char* strstr(const char* haystack, const char* needle) {
	while (*haystack != '\0') {
		if (*haystack == *needle) {
			const char* haystack_start = haystack;
			const char* needle_start = needle;

			while (*needle != '\0' && *haystack == *needle) {
				haystack = haystack + 1;
				needle = needle + 1;
			}

			if (*needle == '\0')
				return (char*)haystack_start;

			haystack = haystack_start;
			needle = needle_start;
		}

		haystack = haystack + 1;
	}

	return NULL;
}

char* strrstr(const char* haystack, const char* needle) {
	const char* haystack_start = haystack;

	while (*haystack != '\0')
		haystack = haystack + 1;

	while (haystack != haystack_start - 1) {
		if (*haystack == *needle) {
			const char* haystack_start = haystack;
			const char* needle_start = needle;

			while (*needle != '\0' && *haystack == *needle) {
				haystack = haystack + 1;
				needle = needle + 1;
			}

			if (*needle == '\0')
				return (char*)haystack_start;

			haystack = haystack_start;
			needle = needle_start;
		}

		haystack = haystack - 1;
	}

	return NULL;
}

void* memcpy(void* destination, const void* source, size_t num) {
	const void* start = destination;
	size_t index = 0;

	while (index < num) {
		*((unsigned char*)destination) = *((unsigned char*)source);
		destination = (unsigned char*)destination + 1;
		source = (unsigned char*)source + 1;
		index = index + 1;
	}

	destination = (void*)start;
	return destination;
}

void* memmove(void* destination, const void* source, size_t num) {
	const void* start = destination;
	size_t index = 0;

	while (index < num) {
		*((unsigned char*)destination) = *((unsigned char*)source);
		destination = (unsigned char*)destination + 1;
		source = (unsigned char*)source + 1;
		index = index + 1;
	}

	destination = (void*)start;
	return destination;
}

int memcmp(const void* ptr1, const void* ptr2, size_t num) {
	size_t index = 0;

	while (index < num) {
		if (*((unsigned char*)ptr1) != *((unsigned char*)ptr2))
			return *((unsigned char*)ptr1) - *((unsigned char*)ptr2);

		ptr1 = (unsigned char*)ptr1 + 1;
		ptr2 = (unsigned char*)ptr2 + 1;
		index = index + 1;
	}

	return 0;
}

void* memset(void* source, int value, size_t num) {
	const void* start = source;
	size_t index = 0;

	while (index < num) {
		*((unsigned char*)source) = (unsigned char)value;
		source = (unsigned char*)source + 1;
		index = index + 1;
	}

	source = (void*)start;
	return source;
}
