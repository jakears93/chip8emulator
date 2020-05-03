# chip8emulator
A Chip 8 interpreter

My first emulator project.

I have tried to make this project as verbose as possible as it is a learning project.
My goal is to write an NES emulator using the knowledge I have gained from this venture.

## Completed
------------
-    Currently the emulator seems to run games with no issues.
I have set the opcode execution speed to 1000 fps as I find the games run a little better at this speed. However, you can still set the fps manually with the -f flag when launching. Used as ./chip8 romName -f fps

-    The default window size for the emulator is 960x480 (15x scale of the original 64x32 resolution).  You can also manually set the scale with the -v flag from 1-30, 1 being 64x32 and 32 being 1920x960.
Used as ./chip8 romName -v scale

-    The default window background colour is black but can now be set using the command line.  Used as ./chip8 romName -b colourstring

-    The default sprite colour is white but can now be set using the command line.  Used as ./chip8 romName -s colourstring

-    The emulator speed can be increased and decreased using the keypad plus and minus keys.  Each SDL poll event of the key down changes the frames per second value by +- 100

-    Emulation can be paused using the backspace key.  This pauses cpu execution and rendering of the screen.  Event polling must remain live as a second backspace key press un-pauses the emulation.

-    A ROM can be restarted by pressing the F1 key.  This sets the memory, register and display pixel values back to how they were at the initial loading of the ROM.


## Next Steps
-------------
1)   Next I plan to implement sound and make sure that the two timers DT and ST are fully functional.  Audio will be processed via SDL as was graphics and input.

2)   Finally, I plan to clean up the code a little bit better; remove use of global variables and remove unused functions/checks that were previously used tot debug.  I tried to do this already and for example, pass the display buffer as a pointer but for reasons beyond my current understanding, performance took a drastic penalty, running at what I could visually estimate as about 60fps even at frame rate settings 1000 or higher.

## Update Log
-------------
#### version 0.2
1)	Fixed ADD-VxVy to accurately set V[F] flag
2)	Fixed Rom load to read file as binary
3) 	Added program to create rom files (no error checking)
4) 	Added getopt loop to set framerate, window size and instruction step mode via cmd args
5) 	Moved loading of rom to cpu init
6) 	Edited makefile so object files are created and stored in directory objs

#### version 0.3
1)	Fixed bug that caused V[x] to compare to kk instead of being set to it
2)	Fixed bug that cause sprites to be drawn flipped.  While setting pixelOn, shift (7-x) bits right instead of x bits
3)	Implemented keyboard inputs
4)	Changed standard framerate from 60 to 1000 as 60 produced a very slow game
5)	Changed makefile so that if option DF=1 is used, Debug flag is set and opcodes are printed during execution.
6)	Fixed cpu function that loads values from I into V0-Vx.  Was causing bug in pong where right player score would not be drawn
7)	Put input event and draw screen loops into separate threads to increase performance
8)	Fixed SHL-Vx function as it was always evaluating as false
9)	Initialized each SDL component separately and safely close them at program exit.
10)	Created new makefile with -Ofast flags to cut down size of built program and boost performance
11)	Can now be run by setting makefile option DF=1, allowing for debug statements to be printed to console during execution
12)	Reorganized code

#### version 0.4
1)   Implemented a pause game feature by pressing backspace.
2)   Implemented a reset rom feature by pressing F1
3)   Implemented control of game speed with keypad plus (increase) and keypad minus (decrease)
4)   Fixed getopt loop to correctly process arguments
5)   Added the ability to change sprite and background colours using the flags -s and -b respectively followed by the colour in lowercase.

#### version 0.5
1)   Added audio support via the std function printf "\a" console beep
2)   Created a separate thread to handle audio
3)   Adjusted RET and CALL opcode functions to set PC properly
