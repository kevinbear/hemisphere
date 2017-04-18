#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES   
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"
#define radius 380
#define power(x,y) (x - radius)*(x - radius) + (y - radius)*(y - radius)
using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
int G[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];
int g[MaxBMPSizeX][MaxBMPSizeY];
int b[MaxBMPSizeX][MaxBMPSizeY];
char *bmpname = NULL, *chbmpname = NULL;
inline float fun3(float w_in, float w_out, float d_out);
int main(int argc, char *argv[])
{
	int width, height;
	float din, dout, win, wout, X, Y;
	system("dir *.bmp");
	bmpname = (char *)malloc(sizeof(char) * 100);
	chbmpname = (char *)malloc(sizeof(char) * 100);
	printf("Please input you want choose image:");
	scanf("%s", bmpname);
	open_bmp(bmpname, R, G, B, width, height);
	win=wout = width / 2;
	/*==============hemisphere================*/
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//x = j - width / 2;
			//y = height / 2 - i;
			dout = sqrt(power(j,i));
			if (dout > radius)
			{
				r[i][j] = 255;
				g[i][j] = 255;
				b[i][j] = 255;
			}
			else
			{
				din = fun3(win, wout, dout);
				X = din*(j / dout);
				Y = din*(i / dout);
				if ( (X - (int)X) == 0 && (Y - (int)Y) == 0)//point position integer 
				{
					r[i][j] = R[(int)Y][(int)X];
					g[i][j] = G[(int)Y][(int)X];
					b[i][j] = B[(int)Y][(int)X];
				}
				else//point position float
				{
					Y = (int)Y, X = int(X);
					r[i][j] = R[(int)Y][(int)X];
					g[i][j] = G[(int)Y][(int)X];
					b[i][j] = B[(int)Y][(int)X];
				}
			}
		}
	}
	/*==============================*/
	printf("File new name:");
	scanf("%s", chbmpname);
	save_bmp(chbmpname, r, g, b);
	printf("Job Finished!\n");
	close_bmp();
	system("PAUSE");
	return 0;
}

inline float fun3(float w_in, float w_out, float d_out)
{
	float d_in;
	float arcans = asin(d_out / w_out);
	d_in = (2 * w_in*arcans) / M_PI;
	return d_in;
}