graphicsTest: graphicsTest.o graphics.o
	gcc -o graphicsTest graphicsTest.o graphics.o `sdl2-config --cflags --libs`
graphicsTest.o: graphicsTest.c ../../srcs/graphics/graphics.h
	gcc -c -g graphicsTest.c
graphics.o: ../../srcs/graphics/graphics.c ../../srcs/graphics/graphics.h
	gcc -c -g ../../srcs/graphics/graphics.c
clean:
	touch *
	rm *.o graphicsTest

run:
	./graphicsTest
