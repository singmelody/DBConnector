#include "StdAfx.h"
#include "MyVector.h"
#include <math.h>
#include <float.h>

void float2::Normalize()
{

	float fInv = 1.0f/sqrt(x*x + y*y + FLT_MIN);
	x*=fInv;
	y*=fInv;
}

float2 operator-(const float2& op0, const float2& op1)
{
	return float2(op0.x-op1.x, op0.y-op1.y);
}
