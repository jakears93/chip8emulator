graphicsTest: graphicsTest.o cpu.o graphics.o input.o
	gcc -o graphicsTest graphicsTest.o graphics.o input.o cpu.o `sdl2-config --cflags --libs`
graphicsTest.o: graphicsTest.c ../../srcs/input/input.h ../../srcs/cpu/cpu.h ../../srcs/graphics/graphics.h
	gcc -c -g graphicsTest.c
input.o: ../../srcs/input/input.c ../../srcs/input/input.h
	gcc -c -g ../../srcs/input/input.c
graphics.o: ../../srcs/graphics/graphics.c ../../srcs/graphics/graphics.h
	gcc -c -g ../../srcs/graphics/graphics.c
cpu.o: ../../srcs/cpu/cpu.c ../../srcs/cpu/cpu.h
	gcc -c -g ../../srcs/cpu/cpu.c
clean:
	touch *
	rm *.o graphicsTest
run:
	./graphicsTest
