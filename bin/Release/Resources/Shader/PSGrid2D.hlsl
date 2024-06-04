#include "Header//Struct.fxh"
#include "Header//Sampler.fxh"
#include "Header//Texture.fxh"
#include "Header//ConstantBuffer.fxh"

#define FILL_COLOR B2_Color_1
#define RECT_SCALE B2_Scale
#define XY_COUNT B2_XYCount
#define MOUSE_POSITION B2_MousePos

float4 main(tVSOut In) : SV_TARGET
{	
	const int2 NORMAL_XY = int2(In.UV * RECT_SCALE + 1);	
	const int2 WIDTH = int2(RECT_SCALE.xy / XY_COUNT.xy);
	const int2 MOUSE_IDX = int2((MOUSE_POSITION.xy - 1) / WIDTH.xy);
	const int2 GRID_IDX = int2((NORMAL_XY.xy - 1) / WIDTH.xy);

	if (NORMAL_XY.x % WIDTH.x >= B2_Vector4_0.x && 
		NORMAL_XY.y % WIDTH.y >= B2_Vector4_0.x)
	{
		discard;
	}

	return FILL_COLOR;
	
}
