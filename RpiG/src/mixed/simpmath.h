#pragma once

#include "math.h"

int powTen(int pow)
{
	int j = 1;
	for(int i = 0; i < pow; i++)
		j *= 10;
	return j;
}

double pow(double base, int pow)
{
    double j = 1;
    for(int i = 0; i < pow; i++)
        j *= base;
    return j;
}

float max(float f1, float f2)
{
	if(f1 > f2)
		return f1;
	return f2;
}

float min(float f1, float f2)
{
	if(f1 < f2)
		return f1;
	return f2;
}