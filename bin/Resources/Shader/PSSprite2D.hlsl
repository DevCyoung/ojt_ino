#include "Header//Struct.fxh"
#include "Header//Sampler.fxh"
#include "Header//Texture.fxh"
#include "Header//ConstantBuffer.fxh"

float4 setColor(float4 color)
{
	if (1 == B5_bSetColorR)
	{
		color.r = B5_R;
	}
	
	if (1 == B5_bSetColorG)
	{
		color.g = B5_G;
	}
	
	if (1 == B5_bSetColorB)
	{
		color.b = B5_B;
	}
	
	if (1 == B5_bSetColorA)
	{
		color.a = B5_A;
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

float4 main(tVSOut In) : SV_TARGET
{
	float4 color = (float) 0.0f;
	
	color = MeshRenderer00.Sample(pointSampler, In.UV);	
	
	if (color.r == 1.0f && color.g == 0.0f && color.b == 1.0f)
	{
		discard;
	}	
	else if (color.a == 0.f)
	{
		discard;
	}		
	
	//color = setColor(color);
	//color = mulColor(color);


	//color.r = 1.f;
	//color.g = 1.f;	
	//color.b = 0.f;
	//color.a = 1.f;
	
	return color;
}