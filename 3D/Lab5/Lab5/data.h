#ifndef DATA
#define DATA
#include <cmath>

#define MIN WIN_SIZE_X * 0.02 > WIN_SIZE_Y * 0.02 ? WIN_SIZE_Y * 0.02 : WIN_SIZE_X * 0.02

#define WIN_SIZE_X 100
#define WIN_SIZE_Y 100

#define ARRAY_SIZE 8 
#define PI 3.14159265

double const T = 0.5, F = 1.5;
double t = T, f = F, R = 1;

double const halfcube[ARRAY_SIZE][4]
{ 
//  {x, y, z}
	{ 10, 10, -10, 1 },
	{ -10, 10, -10, 1 },
	{ -10, -10, -10, 1 },
	{ 10, -10, -10, 1 },

	{ 10, 10, 10, 1 },
	{ -10, 10, 10, 1 },
	{ -10, -10, 10, 1 },
	{ 10, -10, 10, 1 },
};
double halfcube_2D[8][4];

double vid[4][4]{ { -sin(t), -cos(f) * cos(t), -sin(f) * cos(t), 0 },
				  { cos(t),  -cos(f) * sin(t), -sin(f) * sin(t), 0 },
				  { 0,                 sin(f),          -cos(f), 0 },
				  { 0,                      0,                R, 1 } };

double per[4][4]
{ 
	{ MIN, 0, 0, 0},
	{ 0, MIN, 0, 0 },
	{ 0, 0, 0, 0 },
	{ (WIN_SIZE_X / 2) * 0.9, (WIN_SIZE_Y / 2) * 0.9, 0, 0 }
};

#endif // !DATA

