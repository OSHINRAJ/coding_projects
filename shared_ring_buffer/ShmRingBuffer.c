#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include "ShmRingBuffer.h"


void ShmRingBuf_init(ShmRingBuf_t *mem_pool, int mutex_type, int max_size)
{
	int i;
	struct shm_buf *shm_buffer;

	pthread_mutexattr_init(&mem_pool->attr);
	if (mutex_type)
		pthread_mutexattr_setpshared(&mem_pool->attr, PTHREAD_PROCESS_SHARED);
	else
		pthread_mutexattr_setpshared(&mem_pool->attr, PTHREAD_PROCESS_PRIVATE);
	pthread_mutex_init(&mem_pool->mutex, &mem_pool->attr);

	shm_buffer = mem_pool->buffer;
	mem_pool->max_sz = max_size;
	mem_pool->cur_sz = 0; 
	mem_pool->head = 0;
	mem_pool->tail = 0;

	for (i = 0; i < max_size; i++) {
		shm_buffer[i].len = 0;
	}
}
int ShmDequeue(ShmRingBuf_t *mem_pool, uint8_t *buffer )
{
	int  max_sz, cur_sz;
	uint32_t head, size;
	struct shm_buf *shm_buffer;
	
	max_sz = mem_pool->max_sz;
	cur_sz = mem_pool->cur_sz;
	shm_buffer = mem_pool->buffer;
	
	pthread_mutex_lock(&mem_pool->mutex);
	if (cur_sz == 0) {
		pthread_mutex_unlock(&mem_pool->mutex);
		return -1;
	}
	head = mem_pool->head;
	size = shm_buffer[head].len;
	memcpy(buffer, shm_buffer[head].buf, shm_buffer[head].len); 
	if (head == max_sz - 1)
		mem_pool->head = 0;
	else
		mem_pool->head = head + 1;
	mem_pool->cur_sz -= 1;
	pthread_mutex_unlock(&mem_pool->mutex);

	return size;
}


int ShmEnqueue(ShmRingBuf_t *mem_pool, uint8_t *buffer, uint32_t size)
{
	int max_sz, cur_sz;
	uint32_t tail;
	struct shm_buf *shm_buffer;

	max_sz = mem_pool->max_sz;
	cur_sz = mem_pool->cur_sz;
	shm_buffer = mem_pool->buffer;

	pthread_mutex_lock(&mem_pool->mutex);
	if (max_sz == cur_sz) {
		pthread_mutex_unlock(&mem_pool->mutex);
		return -1;
	}
	tail = mem_pool->tail;
	shm_buffer[tail].len = size;
	memcpy(shm_buffer[tail].buf, buffer, shm_buffer[tail].len);
	if (tail == (max_sz - 1))
		mem_pool->tail = 0;
	else
		mem_pool->tail = tail + 1;

	mem_pool->cur_sz += 1;
	pthread_mutex_unlock(&mem_pool->mutex);

	return size;
}







