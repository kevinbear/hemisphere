#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES   
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"
//===============================define region=============================
#define radius 380
#define power(x,y) (x - radius)*(x - radius) + (y - radius)*(y - radius) // calculate x^2+y^2
#define h1(x) 1.5*pow((abs(x)),3)-2.5*pow((abs(x)),2)+1 //cubic function distant<1
#define h2(x) -0.5*pow((abs(x)),3)+2.5*pow((abs(x)),2)-4*abs(x)+2 //cubic function diatant<=2 && distant>1
#define h3(x) ((abs(x)>2) ? 0:( (abs(x)<=1)) ?  h1(x) : h2(x) ) 
//=========================================================================
using namespace std;
//================rgb_array region===================
int br, bg, bb;
int R[MaxBMPSizeX][MaxBMPSizeY];
int G[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];

int r[MaxBMPSizeX][MaxBMPSizeY];
int g[MaxBMPSizeX][MaxBMPSizeY];
int b[MaxBMPSizeX][MaxBMPSizeY];

char *bmpname = NULL, *chbmpname = NULL;//bmpfilename 
//====================================================
//=============================function region==========
inline float fun3(float w_in, float w_out, float d_out);
void bileanr(float x, float y); 
void bicubic(float x, float y);
//======================================================
int main(int argc, char *argv[])
{
	int width, height, offest, I;
	float din, dout, win = 0, wout = 0, X, Y;
	system("dir *.bmp");
	bmpname = (char *)malloc(sizeof(char) * 100);
	chbmpname = (char *)malloc(sizeof(char) * 100);
	puts("=====================================================");
	printf("Please input you want choose image:");
	scanf("%s", bmpname);
	printf("select interpolation format \"number\"\n");
	puts("----------------------------------");
	puts(" 1.Nearest  2.Bilnear  3.Bicubic ");
	puts("----------------------------------");
	scanf("%d", &I);
	open_bmp(bmpname, R, G, B, width, height);
	win = wout = width / 2;
	/*==============hemisphere================*/
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			dout = (float)sqrt(power(j, i)); // using over radius color on black to background 
			if (dout > radius)
			{
				r[i][j] = 0;
				g[i][j] = 0;
				b[i][j] = 0;
			}
			else //needed process image part
			{
				din = fun3(win, wout, dout);
				X = din*((j - radius) / dout);
				Y = din*((i - radius) / dout);
				X += radius, Y += radius;
				if ((X - (int)X) == 0 && (Y - (int)Y) == 0)//point position integer 
				{
					r[i][j] = R[(int)Y][(int)X];
					g[i][j] = G[(int)Y][(int)X];
					b[i][j] = B[(int)Y][(int)X];
				}
				else//point position float
				{
					switch (I)//interpolation 
					{
					case 1:
						Y = (int)Y, X = (int)X;
						r[i][j] = R[(int)Y][(int)X];
						g[i][j] = G[(int)Y][(int)X];
						b[i][j] = B[(int)Y][(int)X];
						break;
					case 2:
						bileanr(X, Y);
						r[i][j] = br;
						g[i][j] = bg;
						b[i][j] = bb;
						break;
					case 3:
						bicubic(X, Y);
						r[i][j] = br;
						g[i][j] = bg;
						b[i][j] = bb;
						break;
					default:
						bileanr(X, Y);
						r[i][j] = br;
						g[i][j] = bg;
						b[i][j] = bb;
						break;
					}

				}
			}
		}
	}
	/*==============================*/
	puts("----------------------------------");
	printf("File new name:");
	scanf("%s", chbmpname);
	save_bmp(chbmpname, r, g, b);
	printf("Job Finished!\n");
	puts("=====================================================");
	close_bmp();
	system("PAUSE");
	return 0;
}

inline float fun3(float w_in, float w_out, float d_out) //calulate din using conver to other x,y position
{
	float d_in;
	float arcans = asin(d_out / w_out);
	d_in = (2 * w_in*arcans) / M_PI;
	return d_in;
}

void bileanr(float x, float y)
{
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	int x1, y1, x2, y2, x3, y3, x4, y4;
	float c1, c2, c3, c4;
	//===============postion====================
	x1 = (int)x, y1 = (int)y;//f(x,y) left-up
	x2 = x1 + 1, y2 = y1;//f(x2,y2) right-up
	x3 = x1, y3 = y1 + 1;//f(x2,y2) left-down
	x4 = x1 + 1, y4 = y1 + 1;//f(x2,y2) right-down
	//==========================================

	//===============calulate coffcient for bilinear interpolation====================
	c1 = 1 - (x - x1), c2 = x - x1, c3 = 1 - (y - y1), c4 = y - y1;
	br = (int)(R[y1][x1] * c1*c3 + R[y2][x2] * c2*c3 + R[y3][x3] * c4*c1 + R[y4][x4] * c2*c4);
	bg = (int)(G[y1][x1] * c1*c3 + G[y2][x2] * c2*c3 + G[y3][x3] * c4*c1 + G[y4][x4] * c2*c4);
	bb = (int)(B[y1][x1] * c1*c3 + B[y2][x2] * c2*c3 + B[y3][x3] * c4*c1 + B[y4][x4] * c2*c4);
	//================================================================================
}

void bicubic(float x, float y)
{
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	float ux, uy;
	//===============postion array====================
	int x1[4][4] = { 0 }, y1[4][4] = { 0 };
	x1[0][0] = (int)x - 1, y1[0][0] = (int)y - 1;
	x1[0][1] = x1[0][0] + 1, y1[0][1] = y1[0][0];
	x1[0][2] = x1[0][0] + 2, y1[0][2] = y1[0][0];
	x1[0][3] = x1[0][0] + 3, y1[0][3] = y1[0][0];

	x1[1][0] = x1[0][0], y1[1][0] = y1[0][0] + 1;
	x1[1][1] = x1[0][0] + 1, y1[1][1] = y1[0][0] + 1;
	x1[1][2] = x1[0][0] + 2, y1[1][2] = y1[0][0] + 1;
	x1[1][3] = x1[0][0] + 3, y1[1][3] = y1[0][0] + 1;

	x1[2][0] = x1[0][0], y1[2][0] = y1[0][0] + 2;
	x1[2][1] = x1[0][0] + 1, y1[2][1] = y1[0][0] + 2;
	x1[2][2] = x1[0][0] + 2, y1[2][2] = y1[0][0] + 2;
	x1[2][3] = x1[0][0] + 3, y1[2][3] = y1[0][0] + 2;

	x1[3][0] = x1[0][0], y1[3][0] = y1[0][0] + 3;
	x1[3][1] = x1[0][0] + 1, y1[3][1] = y1[0][0] + 3;
	x1[3][2] = x1[0][0] + 2, y1[3][2] = y1[0][0] + 3;
	x1[3][3] = x1[0][0] + 3, y1[3][3] = y1[0][0] + 3;
	//================================================

	//===============calulate coffcient for bicubic interpolation====================
	ux = x - (int)x, uy = y - (int)y;
	br =(int)( R[y1[0][0]][x1[0][0]] * h3(-ux - 1)*h3(-uy - 1) + R[y1[0][1]][x1[0][1]] * h3(-ux)*h3(-uy - 1) + R[y1[0][2]][x1[0][2]] * h3(1 - ux)*h3(-uy - 1) + R[y1[0][3]][x1[0][3]] * h3(2 - ux)*h3(-uy - 1) +
		R[y1[1][0]][x1[1][0]] * h3(-ux - 1)*h3(-uy) + R[y1[1][1]][x1[1][1]] * h3(-ux)*h3(-uy) + R[y1[1][2]][x1[1][2]] * h3(1 - ux)*h3(-uy) + R[y1[1][3]][x1[1][3]] * h3(2 - ux)*h3(-uy) +
		R[y1[2][0]][x1[2][0]] * h3(-ux - 1)*h3(1 - uy) + R[y1[2][1]][x1[2][1]] * h3(-ux)*h3(1 - uy) + R[y1[2][2]][x1[2][2]] * h3(1 - ux)*h3(1 - uy) + R[y1[2][3]][x1[2][3]] * h3(2 - ux)*h3(1 - uy) +
		R[y1[3][0]][x1[3][0]] * h3(-ux - 1)*h3(2 - uy) + R[y1[3][1]][x1[3][1]] * h3(-ux)*h3(2 - uy) + R[y1[3][2]][x1[3][2]] * h3(1 - ux)*h3(2 - uy) + R[y1[3][3]][x1[3][3]] * h3(2 - ux)*h3(2 - uy));

	bg = (int)(G[y1[0][0]][x1[0][0]] * h3(-ux - 1)*h3(-uy - 1) + G[y1[0][1]][x1[0][1]] * h3(-ux)*h3(-uy - 1) + G[y1[0][2]][x1[0][2]] * h3(1 - ux)*h3(-uy - 1) + G[y1[0][3]][x1[0][3]] * h3(2 - ux)*h3(-uy - 1) +
		G[y1[1][0]][x1[1][0]] * h3(-ux - 1)*h3(-uy) + G[y1[1][1]][x1[1][1]] * h3(-ux)*h3(-uy) + G[y1[1][2]][x1[1][2]] * h3(1 - ux)*h3(-uy) + G[y1[1][3]][x1[1][3]] * h3(2 - ux)*h3(-uy) +
		G[y1[2][0]][x1[2][0]] * h3(-ux - 1)*h3(1 - uy) + G[y1[2][1]][x1[2][1]] * h3(-ux)*h3(1 - uy) + G[y1[2][2]][x1[2][2]] * h3(1 - ux)*h3(1 - uy) + G[y1[2][3]][x1[2][3]] * h3(2 - ux)*h3(1 - uy) +
		G[y1[3][0]][x1[3][0]] * h3(-ux - 1)*h3(2 - uy) + G[y1[3][1]][x1[3][1]] * h3(-ux)*h3(2 - uy) + G[y1[3][2]][x1[3][2]] * h3(1 - ux)*h3(2 - uy) + G[y1[3][3]][x1[3][3]] * h3(2 - ux)*h3(2 - uy));

	bb =(int)( B[y1[0][0]][x1[0][0]] * h3(-ux - 1)*h3(-uy - 1) + B[y1[0][1]][x1[0][1]] * h3(-ux)*h3(-uy - 1) + B[y1[0][2]][x1[0][2]] * h3(1 - ux)*h3(-uy - 1) + B[y1[0][3]][x1[0][3]] * h3(2 - ux)*h3(-uy - 1) +
		B[y1[1][0]][x1[1][0]] * h3(-ux - 1)*h3(-uy) + B[y1[1][1]][x1[1][1]] * h3(-ux)*h3(-uy) + B[y1[1][2]][x1[1][2]] * h3(1 - ux)*h3(-uy) + B[y1[1][3]][x1[1][3]] * h3(2 - ux)*h3(-uy) +
		B[y1[2][0]][x1[2][0]] * h3(-ux - 1)*h3(1 - uy) + B[y1[2][1]][x1[2][1]] * h3(-ux)*h3(1 - uy) + B[y1[2][2]][x1[2][2]] * h3(1 - ux)*h3(1 - uy) + B[y1[2][3]][x1[2][3]] * h3(2 - ux)*h3(1 - uy) +
		B[y1[3][0]][x1[3][0]] * h3(-ux - 1)*h3(2 - uy) + B[y1[3][1]][x1[3][1]] * h3(-ux)*h3(2 - uy) + B[y1[3][2]][x1[3][2]] * h3(1 - ux)*h3(2 - uy) + B[y1[3][3]][x1[3][3]] * h3(2 - ux)*h3(2 - uy));
	//==============================================================================

	//===============avoid br bg bb value over (unsigned char) cause pixel became irrcorrect color====================
	br=((br > 255) ? 255 : (br < 0 ? 0 : br)); //slove picture error point
	bg = ((bg > 255) ? 255 : (bg < 0 ? 0 : bg));
	bb = ((bb > 255) ? 255 : (bb < 0 ? 0 : bb));
	//===================================
}