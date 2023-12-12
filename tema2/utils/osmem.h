#pragma once

#include "printf.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void* os_malloc(size_t size);

void os_free(void* ptr);

void* os_calloc(size_t nmemb, size_t size);

void* os_realloc(void* ptr, size_t size);
