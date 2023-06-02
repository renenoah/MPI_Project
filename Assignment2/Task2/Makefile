path_to_lib = /home/lab/2023/s1750463/libs/
number_of_ps = 8
build/main: main.o $(path_to_lib)help.o
	rm -rf build && mkdir build
	mpicc -lm main.o $(path_to_lib)help.o -o ./build/main 
main.o: main.c
	mpicc -c main.c
$(path_to_lib)help.o: $(path_to_lib)help.c
	mpicc -c $(path_to_lib)help.c -o $(path_to_lib)/help.o
run: 
	mpiexec -np $(number_of_ps) ./build/main 
	