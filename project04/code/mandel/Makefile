CC=mpicc
CFLAGS=-O3 -lpng

all: mandel_mpi

mandel_mpi:  mandel_mpi.c pngwriter.c
	$(CC) $^ $(CFLAGS) -o $@.exe

clean:
	rm -f mandel_mpi.exe

