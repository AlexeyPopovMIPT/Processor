#pragma once
#ifndef INC_SMBUF

#define INC_SMBUF

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct Buffer {
	void* buffer;
	size_t capacity;
	size_t count;
};


static int MakeBuffer(struct Buffer* thou, size_t capacity) 
{
	assert(thou);

	if (capacity < 0) return 1;

	if (capacity > 0) {
		thou->buffer = calloc(capacity, 1);
		if (thou->buffer == NULL) return 1;
	}

	capacity = 256;
	for (capacity = 256; (thou->buffer = calloc(capacity, 1)) == NULL
		                    && capacity > 8; capacity /= 2) ;
	if (capacity == 4) return 1;

	thou->capacity = capacity;
	thou->count = 0;
	return 0;
}

static void BufferDistruct(struct Buffer* thou) 
{
	assert(thou);
	assert(thou->buffer);

	free(thou->buffer);
	thou->buffer = NULL;
	thou->capacity = 0;
	thou->count = 0;
}

static int Add(struct Buffer* thou, size_t len, size_t index, void* source) 
{
	assert(thou);
	assert(thou->buffer);

	while (index + len > thou->capacity) 
	{
		size_t grow = thou->capacity;
		void* newbuffer;

		while ((newbuffer = realloc(thou->buffer, thou->capacity + grow)) == NULL && grow > 1)
			grow /= 2;

		if (newbuffer == NULL)
			return 1;

		thou->buffer = newbuffer;
		thou->capacity += grow;
	}

	memcpy((char*)thou->buffer + index, source, len);
	thou->count = index + len > thou->count ? index + len : thou->count;
	return 0;
}

static int AddToEnd(struct Buffer* thou, size_t len, void* source) 
{
	assert(thou);
	return Add(thou, len, thou->count, source);
}

static int ResizeTo(struct Buffer* thou, size_t newcpc) 
{
	assert(thou);

	if (newcpc == 0) newcpc = thou->count;
	if (newcpc == 0) newcpc = 1;

	void* newbuffer;
	if ((newbuffer = realloc(thou->buffer, newcpc)) == NULL)
		return 1;

	thou->buffer = newbuffer;
	thou->capacity = newcpc;
	return 0;

}

static void BufferDump(struct Buffer thou) 
{
	printf("BufferDump:\n");
	printf("count: %d\n", thou.count);
	printf("capacity: %d", thou.capacity);
	for (size_t i = 0; i < thou.capacity; i++) {
		unsigned char byte = ((char*)thou.buffer)[i];
		putchar(i % 16 == 0 ? '\n' : ' ');
		putchar(byte / 16 <= 9 ? byte / 16 + '0' : byte / 16 - 10 + 'A');
		putchar(byte % 16 <= 9 ? byte % 16 + '0' : byte % 16 - 10 + 'A');
	}
	putchar('\n');
}

#endif //INC_SMBUF
