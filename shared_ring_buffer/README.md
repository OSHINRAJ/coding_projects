# RING BUFFER LIBRARY
Implementation of ring buffer using shared memory pthread mutex. Contains follwing files
- ShmRingBuffer.c
- ShmRingBuffer.h
- imgRead.c
- imgWrite.c
- testapp.c
- Makefile

# APIs

- ShmRingBuf_init
- ShmEnqueue
- ShmDequeue

### Program Compilation
```sh
$ cd shm_ring_buf
$ make
```
Following binaries will be created after make.

- imgRead
- imgWrite
- testapp

imgRead input a given file and share with the imgWrite process through shared memory ring buffer, imgWrite will dump the data from the shared memory to a out_file.

#### imgRead
```sh
$ ./imgRead filename
```
#### imgWrite
```sh
$ ./imgWrite
```
#### testapp
```sh
$ ./testapp
```
