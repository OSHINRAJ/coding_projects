#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#include "ShmRingBuffer.h"
#define BYTES 16*1024

int main(int argc, char *argv[])
{
	FILE *fd;
	struct shm_t {
		uint32_t filesize;
		char file_name[20];
		uint8_t ready_to_send;
		uint8_t ready_to_read;
		uint8_t send_complete;
		ShmRingBuf_t shmbuf;
	};
	struct shm_t *shm;
	char file_[20];

	uint8_t data_to_send[BYTES];
	uint32_t sz, tot = 0;

	int shmid, n, bytes_read;

	if ((shmid = shmget (1234, sizeof(ShmRingBuf_t), 0666)) < 0) {
		perror("shmget");
		exit(1);
	}
	if ((shm = (struct shm_t *)shmat (shmid, NULL, 0)) == (struct shm_t *) -1) {
		perror("shmat");
		exit(1);
	}
	while (shm->ready_to_send != 1);

	sprintf(file_, "out_%s", shm->file_name);
	fd = fopen(file_, "w+");
	sz = shm->filesize;

	if (fd == NULL) {
		perror("file open");
		exit(1);
	}
	shm->ready_to_read = 1;

	while (tot != sz) {
			while((bytes_read = ShmDequeue(&shm->shmbuf, data_to_send)) < 0);
			fwrite(data_to_send, bytes_read, 1, fd);
			tot += bytes_read;
	}
	if (shm->send_complete == 1)
		shm->send_complete = 0;
	shmdt((void *) shm);
	return 0;
}

