#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ShmRingBuffer.h"
int main() 
{
	uint8_t buffer[16*1024];
	int size;
	int ch = 1, val, ret;
	ShmRingBuf_t mem_pool;
	ShmRingBuf_init(&mem_pool, 0, 4);

	while(ch !=0) {
		printf("\nOPERATIONS \n 1:PUSH \n 2:PULL \n 3:EXIT\nEnter your choice : ");
		scanf("%d", &ch);
		switch (ch){
			case 1: printf("\nvalue to push: ");
					scanf("%d", &val);
					memset(buffer, val, 16*1024);
					ret = ShmEnqueue(&mem_pool, buffer, 16*1024);
					if(ret < 0)
						printf("\nOVERFLOW\n");
					break;

			case 2: size = ShmDequeue(&mem_pool, buffer);
					if (size < 0)
						printf("\nEMPTY\n");
					else
						printf("\nvalue :%d		size :%d", buffer[0], size);
					break;

			default: ch = 0;
					break;
		}
	}
	return 0;
}
