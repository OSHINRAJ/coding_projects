#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

	uint8_t data_to_send[BYTES];
	uint32_t sz, tot = 0;

	int shmid, n, bytes_read;

	if (argc < 2) {
		perror("Invalid Arguments");
		exit(1);
	}

	if ((shmid = shmget (1234, sizeof(ShmRingBuf_t), IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		exit(1);
	}
	if ((shm = (struct shm_t *)shmat (shmid, NULL, 0)) == (struct shm_t *) -1) {
		perror("shmat");
		exit(1);
	}
	ShmRingBuf_init(&shm->shmbuf, 1, 32);

	fd = fopen(argv[1], "r+");
	if (fd == NULL) {
		perror("file open");
		exit(1);
	}

	fseek(fd, 0L, SEEK_END);
	sz=ftell(fd);
	rewind(fd);
	strcpy(shm->file_name, argv[1]);
	shm->filesize = sz;
	shm->ready_to_read = 0;
	shm->send_complete = 0;
	shm->ready_to_send = 1;	
	while (shm->ready_to_read != 1);

	while (tot != sz) {
			bytes_read = fread(data_to_send, 1, BYTES, fd);
			if (bytes_read > 0) {
				while(ShmEnqueue(&shm->shmbuf, data_to_send, bytes_read) < 0);
				tot += bytes_read;
			}
	}
	shm->send_complete = 1;
	while(shm->send_complete != 0);
	shmdt((void *) shm);
	shmctl(shmid, IPC_RMID, NULL);
	return 0;
}

