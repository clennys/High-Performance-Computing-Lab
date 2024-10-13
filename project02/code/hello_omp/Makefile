CPP     = g++
CPPFLAGS  = -fopenmp

hello_omp: hello_omp.cpp
	$(CPP) $(CPPFLAGS) $^ -o $@.exe

.PHONY: clean
clean:
	$(RM) hello_omp
