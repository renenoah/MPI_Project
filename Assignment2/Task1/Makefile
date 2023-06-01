build/main: main.c	 
	rm -rf build && mkdir build
	mpicc -o ./build/main main.c 
run: 
	mpiexec -np 8 ./build/main
	