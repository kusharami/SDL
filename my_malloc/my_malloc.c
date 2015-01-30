#include <stdlib.h>

void *my_malloc(size_t size)
{
	return malloc(size);
}

void *my_calloc(size_t nmemb, size_t size)
{
	return calloc(nmemb, size);
}

void *my_realloc(void *ptr, size_t size)
{
	return realloc(ptr, size);
}

void my_free(void *ptr)
{
	free(ptr);
}