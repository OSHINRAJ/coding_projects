#define MAX_SHM_BUFFER 32
#include <stdint.h>

struct shm_buf {
	uint8_t buf[16*1024];
	uint32_t len;
};

typedef struct {
	struct shm_buf buffer[MAX_SHM_BUFFER];
	uint32_t head;
	uint32_t tail;
	uint32_t max_sz;
	uint32_t cur_sz;
	pthread_mutex_t mutex;
	pthread_mutexattr_t attr;
} ShmRingBuf_t;

void ShmRingBuf_init(ShmRingBuf_t *mem_pool, int mutex_type, int max_size);
int ShmDequeue(ShmRingBuf_t *mem_pool, uint8_t *buffer);
int ShmEnqueue(ShmRingBuf_t *mem_pool, uint8_t *buffer, uint32_t size);
