CPP     = g++
CPPFLAGS  = -O3 \
			-fopenmp
all: pi

pi: pi.cpp
	$(CPP) $(CPPFLAGS) $^ -o $@.exe
clean:
	rm -rf pi
