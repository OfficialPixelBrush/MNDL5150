MNDL5150
=======

`MNDL5150` is a modified version of my Mandelbrot Exploration Program, `MNDLBRT` (short for Mandelbrot), made to support CGA graphics, modified to not use a floating-point unit and full 8086 compatiblity (from what I've seen).
It is not very fast but it works. 

`MNDLBRT` started as a fun little project earlier this year, just finding my footing in C programming, eventually resulting in a very capable fractal exploration program, running well on an AMD K-5, clocked at ~100MHz. It eventually spiraled into a massive game of porting that program to as many platforms as possible, which became my [Mandelbrot Ports Page](https://pixelbrush.dev/apps/mandelbrotPorts), on which a lot of Consoles are. Anything from the Nintendo GameBoy, up to the Nintendo 3DS and even the Apple ]\[! (The 3DS version is awesome btw. Lets you save and render images on the go!)

As I'm now trying to learn 8086 Assembly, I may try to redo this entire program in that, maybe even making it fully usable on an real IBM 5150! Though for now, this simple port shall suffice for the PCjam! It was fun to make, too bad I didn't have more time for further optimizations. It was my first time doing anything with CGA, especially the composite mode.

`MNDL5150` was made for
[PCjam 2021](https://pcjam.gitlab.io/).

Building
--------

Watcom 11's `wcl` was originally used for compiling, alongside it's Graphics library
 
Running
--------

Typing in "MNDL5150" should start the program right up.
- `Text` Renders the Fractal in whatever size using Asterix's the user desires
- `Fullscreen CGA` will fill the entire screen with a standard CGA Render
- `Explore CGA` will render the fractal at a smaller scale, ideal for exploration and finding cool spots
- `Fullscreen CGA Comp` does the same as `Fullscreen CGA` but with half the horziontal lines, doubling as a composite compatbility mode
- `Explore CGA` does the same as `Explore CGA` but with half the horziontal lines, doubling as a composite compatbility mode
- Any number above 5 can be used to exit

`Set C values` refers to if one would like to generate a Julia-set (Recommended: 0)

`xCoord` refers to the X position of the Camera (Recommended: 0)

`yCoord` refers to the Y position of the Camera (Recommended: 0)

`Zoom` refers to the Zoom of the Camera (Recommended: 10)

`Iterations` refers to the number of Iterations the program uses to render the fractal (Recommended: 10)
 
Controls
--------
`1 2 3 4 5` can be used as hot-keys for quickly swapping between the 5 different render modes (as shown above in Running)

`W A S D` for up, down, left and right

`Q` for zooming in, `E` for zooming out

`R` to increase the iterations by 10, `F` to decrease the iterations by 10

`T` to increase the resolution, `G` to decrease the resolution

`Escape` to exit (bit buggy)


Screenshots
-----------

![Monochrome CGA Fullsreen Render](https://user-images.githubusercontent.com/23031327/128101835-78e4951f-8a84-4fc9-8e2b-bac878edbfd4.png)
 
Monochrome CGA Fullsreen Render


![CGA Composite Fullsreen Render](https://user-images.githubusercontent.com/23031327/128101170-d4aaca9b-ab67-44da-86e7-68068ef9ce11.png)

CGA Composite Fullsreen Render


![Modern CGA Render 1](https://user-images.githubusercontent.com/23031327/128101708-48190fb6-8430-427b-b603-06a05ecf4621.png)

Modern CGA Renders
