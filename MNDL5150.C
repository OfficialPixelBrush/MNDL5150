#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <string.h>
#include <time.h>
#include <mem.h>
#include <math.h>
#include <graph.h>

#define COLOR_OFFSET 0/*56*/
#define NUM_COLORS 3/*10*/
int startcolor = 1;/*4;*/
#define SCREEN_HEIGHT 100
#define SCREEN_WIDTH 320

#define LEN 256
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))

/* map coordinates to screen */
double mapRange(double a1, double a2, double b1, double b2, double s) {
	return b1 + (s - a1) * (b2 - b1) / (a2 - a1);
}

/* define global variables */
unsigned int mode, format, juliaMode, funcNum, t, maxPower, scaleX, scaleY;
unsigned long xSize, ySize, x, y;
double cReal, cImaginary, xCoord, yCoord, Zoom, res; 
FILE * fp;
char str[128];
char * token;
char pointer;

/* complex number */
struct complexNumber {
	double real,imaginary;
};

/* complex number addition */
struct complexNumber complexAdd(struct complexNumber x, struct complexNumber y) {
	struct complexNumber result;
	result.real = x.real + y.real;
	result.imaginary = x.imaginary + y.imaginary;
	return result;
}

/* complex number subtraction */
struct complexNumber complexSubtract(struct complexNumber x, struct complexNumber y) {
	struct complexNumber result;
	result.real = x.real - y.real;
	result.imaginary = x.imaginary - y.imaginary;
	return result;
}

/* complex number multiplication */
struct complexNumber complexMultiply(struct complexNumber x, struct complexNumber y) {
	struct complexNumber result;
	result.real = x.real * y.real - x.imaginary * y.imaginary;
	result.imaginary =  x.real * y.imaginary + x.imaginary * y.real;
	return result;
}

/* complex number power */
struct complexNumber complexPower(struct complexNumber x, int power) {
	struct complexNumber result;
	int i;
	result = x;
	for (i = 1; i < power; i++) {
		result = complexMultiply(x,result);
	}
	
	/* automate power */
	if (power > maxPower) { maxPower = power; }
	
	return result;
}

/* complex number division */
struct complexNumber complexDivide(struct complexNumber x, struct complexNumber y) {
	struct complexNumber result;
	double denominator = y.real * y.real + y.imaginary * y.imaginary;
	result.real = ((x.real * y.real) + (x.imaginary * y.imaginary)) / denominator;
	result.imaginary =  ((x.imaginary * y.real) - (x.real * y.imaginary)) / denominator;
	return result;
}

/* complex number division */
double complexAbsSqr(struct complexNumber x) {
	return x.real * x.real + x.imaginary * x.imaginary;
}

/* dithering algorithm */
double smoothColor(int i, int x, int y) {	
	int out = ((i/4)%NUM_COLORS)+COLOR_OFFSET+startcolor;
	int iModFour = i%4;
	
	/* Change dithering based on iteration */
	switch (iModFour) {
		case 0:
			/* 1 */
			if (out > NUM_COLORS+COLOR_OFFSET+startcolor) {
				out = out-NUM_COLORS+COLOR_OFFSET+startcolor;
			}
			
			if (out > 3) { out = 1; }
			return out;
		case 1:
			/* 2 */
			if (y%2) {
				if (x%2) {
					if (out > NUM_COLORS+COLOR_OFFSET+startcolor) {
						out = out-NUM_COLORS+COLOR_OFFSET+startcolor;
					}
				} else {
					++out;
					if (out > NUM_COLORS+COLOR_OFFSET+startcolor) {
						out = out-NUM_COLORS+COLOR_OFFSET+startcolor;
					}
				}
			} else {
				if (out > NUM_COLORS+COLOR_OFFSET+startcolor) {
					out = out-NUM_COLORS+COLOR_OFFSET+startcolor;
				}
			}
			
			if (out > 3) { out = 1; }
			return out;
		case 2:
			/* 3 */
			if (y%2) {
				if (x%2) {
					if (out > NUM_COLORS+COLOR_OFFSET+startcolor) {
						out = out-NUM_COLORS+COLOR_OFFSET+startcolor;
					}
				} else {
					++out;
					if (out > NUM_COLORS+COLOR_OFFSET+startcolor) {
						out = out-NUM_COLORS+COLOR_OFFSET+startcolor;
					}
				}
			} else {
				if (x%2) {
					++out;
					if (out > NUM_COLORS+COLOR_OFFSET+startcolor) {
						out = out-NUM_COLORS+COLOR_OFFSET+startcolor;
					}
				} else {
					if (out > NUM_COLORS+COLOR_OFFSET+startcolor) {
						out = out-NUM_COLORS+COLOR_OFFSET+startcolor;
					}
				}
			}
			
			if (out > 3) { out = 1; }
			return out;
		case 3:
			/* 4 */
			if (y%2) {
				if (x%2) {
					++out;
					if (out > NUM_COLORS+COLOR_OFFSET+startcolor) {
						out = out-NUM_COLORS+startcolor;
					}
				} else {
					if (out > NUM_COLORS+COLOR_OFFSET+startcolor) {
						out = out-NUM_COLORS+startcolor;
					}
				}
			} else {
					++out;
				if (out > NUM_COLORS+COLOR_OFFSET+startcolor) {
					out = out-NUM_COLORS+startcolor;
				}
			}
			
			if (out < COLOR_OFFSET+startcolor) {
				out = out-1;
			}
			if (out > 3) { out = 1; }
			return out;
	}
	return out;
}

/* draw a pixel based on the render mode */
void drawBasedOnMode(int i, int x, int y, int blank, struct complexNumber z) {
	switch(mode) {
		case 1:
			if (blank > 0) {
				printf(" ");
			} else {
				printf("*");
			}
			if (x >= xSize-1) {
				printf("\n");
			}
			break;
		case 2:
			if (blank > 0) {
				_setcolor( smoothColor(i,x,y) );
			} else {
				_setcolor( 0 );
			}
			_setpixel( x, (y*2)  );
			_setpixel( x, (1+y*2));
			break;
		case 3:
			if (blank > 0) {
				_setcolor( smoothColor(i,x,y) );
			} else {
				_setcolor( 0 );
			}
			_setpixel( x/scaleX, (y*2)/scaleY);
			_setpixel( x/scaleX, (1+y*2)/scaleY);
			break;
		case 4:
			if (blank > 0) {
				_setcolor( smoothColor(i,x,y) );
			} else {
				_setcolor( 0 );
			}
			_setpixel( (x*2), y*2 );
			_setpixel( (x*2), 1+y*2 );
			break;
		case 5:
			if (blank > 0) {
				_setcolor( smoothColor(i,x,y) );
			} else {
				_setcolor( 0 );
			}
			_setpixel( (x*2)/scaleX, (y*2)/scaleY);
			_setpixel( (x*2)/scaleX, (1+y*2)/scaleY);
			break;
	}
}

/* This does the actual math */
void drawSetInMode() {
	int i;
	float aspect = 1.3333;
	int yStart = 0;
	scaleX = 1;
	scaleY = 1;
	
	switch(mode) {
		case 1:
			_setvideomode( _DEFAULTMODE );
			break;
		case 2:
			xSize = SCREEN_WIDTH;
			ySize = SCREEN_HEIGHT;
			_setvideomode( _MRES4COLOR );
			break;
		case 3:
			if (res < 1) { res = 1; }
			if ( t < 10) { t = 10; }
			xSize = SCREEN_WIDTH/res;
			ySize = SCREEN_HEIGHT/res;
			yStart = 33/2;
			_setvideomode( _MRES4COLOR );
			break;
		case 4:
			xSize = SCREEN_WIDTH/2;
			ySize = SCREEN_HEIGHT;
			_setvideomode( _MRES4COLOR );
			break;
		case 5:
			if (res < 1) { res = 1; }
			if ( t < 10) { t = 10; }
			xSize = SCREEN_WIDTH/res/2;
			ySize = SCREEN_HEIGHT/res;
			yStart = 33/2;
			_setvideomode( _MRES4COLOR );
			break;
	}	
	
	for (y = 0; y < ySize; y++) {
		/*if (mode == 4) {
			float percent = ((float)y/(float)ySize)*100.0;
			printf("%.2f%c\n", percent, 37);
		}*/
		for (x = 0; x < xSize; x++) {
			int i;
			struct complexNumber c,z;
			if (juliaMode) {
				c.real = cReal;
				c.imaginary = cImaginary;
				z.real = mapRange(0, xSize, xCoord-(20*aspect)/Zoom, xCoord+(20*aspect)/Zoom, x);
				z.imaginary = mapRange(0, ySize, yCoord+20/Zoom, yCoord-20/Zoom, y);
			} else {
				c.real = mapRange(0, xSize, xCoord-(20*aspect)/Zoom, xCoord+(20*aspect)/Zoom, x);
				if ((format==2) && (mode==4)) {
					c.imaginary = mapRange(0, ySize, yCoord-20/Zoom, yCoord+20/Zoom, y);
				} else {
					c.imaginary = mapRange(0, ySize, yCoord+20/Zoom, yCoord-20/Zoom, y);
				}
				z = c;
			}
			
			for (i = 0; i < t; i++) {
				//z = complexDivide(complexAdd(complexPower(complexDivide(complexPower(z,2),c),4),c),c);
				
				// Cool spiral 
				//z = complexAdd(complexPower(z,3),complexDivide(c,z));
				
				// Mandelbrot Set
				z = complexAdd(complexPower(z,2),c);
				
				/* Regular Escape */
				if ((z.real*z.real + z.imaginary*z.imaginary) > 4) {
					//drawBasedOnMode(i,(int)x+(SCREEN_WIDTH-xSize)/2,(int)(y)+(SCREEN_HEIGHT-ySize)/2, 1, z);
					drawBasedOnMode(i,x,y+yStart, 1, z);
					break;
				}
			}
			if (i==t) {
				//drawBasedOnMode(i,(int)x+(SCREEN_WIDTH-xSize)/2,(int)(y)+(SCREEN_HEIGHT-ySize)/2, 0, z);
				drawBasedOnMode(i,x,y+yStart, 0, z);
			}				
		}
	}
}

int main() {	
	int readI;
	clock_t begin, end;
	double time_spent;
	char filename[8];
	char dest[255] = "C:\\";
	char kc = 0;
	maxPower = 1;
	
	res = 5;
	mode = 3;
	xSize = SCREEN_WIDTH;
	ySize = SCREEN_HEIGHT;
	juliaMode = 0;
	t = 10;
	xCoord = 0.0f;
	yCoord = 0.0f;
	Zoom = 10.0f;
	
	start:
	_setvideomode( _DEFAULTMODE );
	printf(" .  . .  .  __  .\n");
	printf(" |\\/| |\\ | |  \\ |\n"); 
	printf(" |  | | \\| |__/ |__\n");
	printf("  __     __  __\n");
	printf("  \\_  \\  \\_  \\ \\\n");
	printf("   _\\  \\  _\\  \\_\\\n");
	
	printf("\nMandelbrot Set (for the IBM 5150)\n");
	printf("Written by PixelBrushArt (2021)\n");
	printf("v8.3\n");

	printf("\n- Render Mode -");
	printf("\n1) Text\n2) Fullscreen CGA\n3) Explore CGA\n4) Fullscreen CGA Comp\n5) Explore CGA Comp\n");
	scanf("%d", &mode);
	if (mode > 5) {
		goto end;
	}
	
	printf("\nSet C values (0/1) ");
	scanf("%d", &juliaMode);
	
	// Julia mode 
	if (juliaMode) {
		printf("\nReal of c: ");
		scanf("%lf", &cReal);
		printf("\nImaginary of c: ");
		scanf("%lf", &cImaginary);
	}
	
	// Get location 
	printf("\nxCoord: ");
	scanf("%lf", &xCoord);
	printf("\nyCoord: ");
	scanf("%lf", &yCoord);
	printf("\nZoom: ");
	scanf("%lf", &Zoom);
	printf("\nIterations: ");
	scanf("%d", &t);

	if ((mode == 1)) {		
		printf("\nWidth: ");
		scanf("%d", &xSize);
		printf("\nHeight: ");
		scanf("%d", &ySize);
	}
	
	/* Calculating */
	draw:
		drawSetInMode();
	end = clock();
	switch( mode ) {
		case 3:
			printf("\ni: %d",(int)t);
			printf("\nx: %8.24g",xCoord);
			printf("\ny: %8.24g",yCoord);
			printf("\nZ: %.02f",Zoom);
			break;
		case 5:
			printf("\ni: %d",(int)t);
			printf("\nx: %8.24g",xCoord);
			printf("\ny: %8.24g",yCoord);
			printf("\nZ: %.02f",Zoom);
			break;
	}
	end = 0;
	time_spent = 0;
	
	/* Keyboard input */
	while( kc != 0x1b ) {
		if (kbhit()) {kc = getch();
			/* Regular Chars */
			switch( kc ) {
				case 0x1b:
					goto start;
				case 0x74: /* increase resolution, T */
					res /= 2;
					break;
				case 0x67: /* decrease resolution, G */
					res *= 2;
					break;
				case 0x77: /* up, W */
					yCoord = yCoord+2/Zoom;
					break;
				case 0x73: /* down, S */
					yCoord = yCoord-2/Zoom;
					break;
				case 0x61: /* left, A */
					xCoord = xCoord-2/Zoom;
					break;
				case 0x64: /* right, D */
					xCoord = xCoord+2/Zoom;
					break;
				case 0x65: /* zoom out, Q */
					Zoom = Zoom*1.25;
					break;
				case 0x71: /* zoom in, E */
					Zoom = Zoom/1.25;
					break;
				case 0x72: /* increase iterations, R */
					t = t+10;
					break;
				case 0x66: /* decrease iterations, F */
					t = t-10;
					if (t < 1) {
						t = 10;
					}
					break;
				case 0x31: /* text */
					mode = 1;
					printf("\nIterations: ");
					scanf("%d", &t);
					printf("\nWidth: ");
					scanf("%d", &xSize);
					printf("\nHeight: ");
					scanf("%d", &ySize);
					break;
				case 0x32: /* cga full */
					mode = 2;
					printf("\nIterations: ");
					scanf("%d", &t);
					break;
				case 0x33: /* cga explore */
					mode = 3;
					break;
				case 0x34: /* cga comp full */
					mode = 4;
					printf("\nIterations: ");
					scanf("%d", &t);
					break;
				case 0x35: /* cga comp explore */
					mode = 5;
					break;
			}
			
			goto draw;
		}
	}
	
	end:
	return 0;
}