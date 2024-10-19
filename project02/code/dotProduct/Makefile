CPP     = g++
CPPFLAGS  = -O3 \
			-fopenmp

all: dotProduct

dotProduct: dotProduct.cpp walltime.h
	$(CPP) $(CPPFLAGS) $^ -o $@.exe

clean:
	rm -rf dotProduct.exe
