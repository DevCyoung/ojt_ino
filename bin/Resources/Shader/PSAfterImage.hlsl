#include "Header//Struct.fxh"
#include "Header//Sampler.fxh"
#include "Header//Texture.fxh"
#include "Header//ConstantBuffer.fxh"

float4 setColor(float4 color)
{
	if (1 == B1_bSetColorR)
	{
		color.r = B1_R;
	}
	
	if (1 == B1_bSetColorG)
	{
		color.g = B1_G;
	}
	
	if (1 == B1_bSetColorB)
	{
		color.b = B1_B;
	}
	
	if (1 == B1_bSetColorA)
	{
		color.a = B1_A;
	}
	
	return color;
}

float4 mulColor(float4 color)
{
	//Mul
	if (1 == B1_bMulColorR)
	{
		color.r *= B1_MulR;
	}
	
	if (1 == B1_bMulColorG)
	{
		color.g *= B1_MulG;
	}
	
	if (1 == B1_bMulColorB)
	{
		color.b *= B1_MulB;
	}
	
	if (1 == B1_bMulColorA)
	{
		color.a *= B1_MulA;
	}
	
	return color;
}

float4 animationColor(float2 uv)
{
	float4 outColor = float4(1.f, 0.f, 1.f, 1.f);
	
	float2 UV = B3_UVBackLeftTop + uv * B3_UVBackSIze;
	
	UV.x -= B3_UVOffset.x;
	UV.y += B3_UVOffset.y;
	
	if (B3_UVLeftTop.x <= UV.x && UV.x <= B3_UVLeftTop.x + B3_UVSize.x &&
		B3_UVLeftTop.y <= UV.y && UV.y <= B3_UVLeftTop.y + B3_UVSize.y)
	{
		outColor = AtlasAnimation2D.Sample(pointSampler, UV);
								
		if (outColor.a == 0.f)
		{
			discard;
		}
		else if (outColor.r == 1.f && outColor.g == 0.f && outColor.b == 1.f)
		{
			discard;
		}
	}
	else
	{
		discard;
	}
	
	outColor = setColor(outColor);
	outColor = mulColor(outColor);
	
	
	//float src = 0.3f;
	//float dst = 1.f - src;
		
	//outColor.r = 0.0f;
	//outColor.g = outColor.g + outColor.g * src + 1.f * dst;
	//outColor.b = outColor.b + outColor.b * src + 1.f * dst;
	//outColor.a = B1_cbColor.a;
	
	{
		//138, 43, 226
		//153,50,204
		//147,112,219
		//outColor.r = 153.f / 255.f;
		//outColor.g = 50.f / 255.f;
		//outColor.b = 204.f / 255.f;
		outColor.a = B1_cbColor.a;		
	}
	
	return outColor;
}

float4 main(tVSOut In) : SV_TARGET
{
	float4 outColor = animationColor(In.UV);
	
	return outColor;
}