chip8: chip8.o cpu.o graphics.o input.o
	gcc -o chip8 chip8.o graphics.o input.o cpu.o `sdl2-config --cflags --libs`
chip8.o: chip8.c srcs/input/input.h srcs/cpu/cpu.h srcs srcs/graphics/graphics.h
	gcc -c -g chip8.c
input.o: srcs/input/input.c srcs/input/input.h
	gcc -c -g srcs/input/input.c
graphics.o: srcs/graphics/graphics.c srcs/graphics/graphics.h
	gcc -c -g srcs/graphics/graphics.c
cpu.o: srcs/cpu/cpu.c srcs/cpu/cpu.h
	gcc -c -g srcs/cpu/cpu.c

clean:
	touch *
	rm *.o chip8

run:
	./chip8
