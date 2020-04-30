## chip8emulator
A Chip 8 interpreter

My first emulator project.

I have tried to make this project as verbose as possible as it is a learning project.  
My goal is to write an NES emulator using the knowledge I have gained from this venture.

# Completed
Currently the emulator seems to run games with no issues.
I have set the opcode execution speed to 1000 fps as I find the games run a little better at this speed. However, you can still set the fps manually with the -f flag when launching. Used as ./chip8 -r <romName> -f <fps>

The default window size for the emulator is 960x480 (15x scale of the original 64x32 resolution).  You can also manually set the scale with the -v flag from 1-30, 1 being 64x32 and 32 being 1920x960.
Used as ./chip8 -r <romName> -v <scale>

# Next Steps
1)   Next I plan to implement sound and make sure that the two timers DT and ST are fully functional.  Audio will be processed via SDL as was graphics and input.

2)   I have to fix the getopt loop as current usage requires the -r flag to load a rom.  Example: ./chip8 -r <romName> -otherflags
The goal is for usage to be: ./chip8 <romName> -otherflags.
This is an easy fix however, I have put it on the back burner as I found implementing threading to be of more importance.

3)   Changing the background and active pixel colours via flags will be implemented in the future as well.  Currently it is only white/black display.

4)   Finally, I plan to clean up the code a little bit better; remove use of global variables and remove unused functions/checks that were previously used tot debug.  I tried to do this already and for example, pass the display buffer as a pointer but for reasons beyond my current understanding, performance took a drastic penalty, running at what I could visually estimate as about 60fps even at frame rate settings 1000 or higher.
