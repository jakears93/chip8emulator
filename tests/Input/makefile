inputTest: inputTest.o input.o
	gcc -o inputTest inputTest.o input.o
inputTest.o: inputTest.c ../../srcs/input/input.h
	gcc -c -g inputTest.c
input.o: ../../srcs/input/input.c ../../srcs/input/input.h
	gcc -c -g ../../srcs/input/input.c
clean:
	touch *
	rm *.o inputTest

run:
	./inputTest
