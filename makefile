chip8: chip8.o cpu.o graphics.o input.o
	gcc -o chip8 objs/chip8.o objs/graphics.o objs/input.o objs/cpu.o `sdl2-config --cflags --libs`
chip8.o: chip8.c srcs/input/input.h srcs/cpu/cpu.h srcs srcs/graphics/graphics.h
	gcc -c -g -o objs/chip8.o chip8.c
input.o: srcs/input/input.c srcs/input/input.h
	gcc -c -g -o objs/input.o srcs/input/input.c
graphics.o: srcs/graphics/graphics.c srcs/graphics/graphics.h
	gcc -c -g -o objs/graphics.o srcs/graphics/graphics.c
cpu.o: srcs/cpu/cpu.c srcs/cpu/cpu.h
	gcc -c -g -o objs/cpu.o srcs/cpu/cpu.c

clean:
	touch objs/*
	rm objs/*.o
	touch *
	rm chip8

run:
	./chip8
