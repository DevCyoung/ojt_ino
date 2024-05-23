#include "Header//ConstantBuffer.fxh"

float4 setColor(float4 color)
{
	if (1 == B5_bSetColorR)
	{
		color.r *= B5_R;
	}
	
	if (1 == B5_bSetColorG)
	{
		color.g *= B5_G;
	}
	
	if (1 == B5_bSetColorB)
	{
		color.b *= B5_B;
	}
	
	if (1 == B5_bSetColorA)
	{
		color.a *= B5_A;
	}
	
	return color;
}

float4 mulColor(float4 color)
{
	//Mul
	if (1 == B5_bMulColorR)
	{
		color.r *= B5_MulR;
	}
	
	if (1 == B5_bMulColorG)
	{
		color.g *= B5_MulG;
	}
	
	if (1 == B5_bMulColorB)
	{
		color.b *= B5_MulB;
	}
	
	if (1 == B5_bMulColorA)
	{
		color.a *= B5_MulA;
	}
	
	return color;
}