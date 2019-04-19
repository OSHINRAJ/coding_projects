all:
	gcc -fpic -shared ShmRingBuffer.c -o libShmRngBuf.so 
	gcc -L$(PWD) -Wl,-rpath=$(PWD) testapp.c -o test -lShmRngBuf -lpthread	
	gcc -L$(PWD) -Wl,-rpath=$(PWD) imgRead.c -o imgRead -lShmRngBuf -lpthread 
	gcc -L$(PWD) -Wl,-rpath=$(PWD) imgWrite.c -o imgWrite -lShmRngBuf -lpthread

clean:
	rm libShmRngBuf.so test imgRead imgWrite out_*

