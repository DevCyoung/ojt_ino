#include "Header//Struct.fxh"
#include "Header//Texture.fxh"
#include "Header//ConstantBuffer.fxh"
#include "Header//Sampler.fxh"

float noise(float2 p)
{
	float s = Noise01.Sample(pointSampler, float2(1.0, 2.0 * cos(B4_GlobalTime)) * B4_GlobalTime * 8.0 + p * 1.0).x;
	
	s *= s;
	return s;
}

float onOff(float a, float b, float c)
{	
	return step(c, sin(B4_GlobalTime + a * cos(B4_GlobalTime * b)));
}

float ramp(float y, float start, float end)
{
	float inside = step(start, y) - step(end, y);
	float fact = (y - start) / (end - start) * inside;
	return (1.0 - fact) * inside;
}

float stripes(float2 uv)
{
	float noi = noise(uv * float2(0.5, 1.0) + float2(1.0, 3.0));
	float speed = 1.1f;
	return ramp(fmod((1.f - uv.y * B5_R) * 6.0 + B4_GlobalTime * speed, 1.0), 0.5, 0.6) * noi;
}

float3 getVideo(float2 uv)
{
	float2 look = uv;
	float window = 1.0 / (1.0 + 20.0 * (look.y - fmod(B4_GlobalTime / 4.0, 1.0)) * (look.y - fmod(B4_GlobalTime / 4.0, 1.0)));
	look.x = look.x + sin(look.y * 10.0 + B4_GlobalTime) / 50.0 * onOff(4.0, 4.0, 0.3) * (1.0 + cos(B4_GlobalTime * 80.0)) * window;
	float vShift = 0.4 * onOff(2.0, 3.0, 0.9) * (sin(B4_GlobalTime) * sin(B4_GlobalTime * 20.0) + (0.5 + 0.1 * sin(B4_GlobalTime * 200.0) * cos(B4_GlobalTime)));
	look.y = fmod(look.y + vShift, 1.0);
	float3 video = Rendertarget.Sample(pointSampler, look).xyz;
	return video;
}

float2 screenDistort(float2 uv)
{
	uv -= float2(0.5, 0.5);
	uv = uv * 1.2f * (1.0 / (1.2 + 2.0 * uv.x * uv.x * uv.y * uv.y));
	uv += float2(0.5, 0.5);
	return uv;
}

//float4 main(float4 fragCoord : SV_POSITION) : SV_TARGET
//{
//	float2 uv = fragCoord.xy / B4_Resolution.xy;
//	uv = screenDistort(uv);
//	float3 video = getVideo(uv);
//	float vigAmt = 3.0 + 0.3 * sin(B4_GlobalTime + 5.0 * cos(B4_GlobalTime * 5.0));
//	float vignette = (1.0 - vigAmt * (uv.y - 0.5) * (uv.y - 0.5)) * (1.0 - vigAmt * (uv.x - 0.5) * (uv.x - 0.5));
    
//	video += stripes(uv);
//	video += noise(uv * 2.0) / 2.0;
//	video *= vignette;
//	video *= (12.0 + fmod(uv.y * 30.0 + B4_GlobalTime, 1.0)) / 13.0;
    
//	return float4(video, 1.0);
//}

float2 curve(float2 uv)
{
	uv = (uv - 0.5) * 2.0;
	uv *= 1.1;
	uv.x *= 1.0 + pow((abs(uv.y) / 5.0), 2.0);
	uv.y *= 1.0 + pow((abs(uv.x) / 4.0), 2.0);
	
	uv = (uv / 2.0) + 0.5;
	uv = uv * 0.92 + 0.04;
	return uv;
}

float4 main(float4 fragCoord : SV_POSITION) : SV_TARGET
{
	float2 q = fragCoord.xy / B4_Resolution.xy;
	float2 uv = q;
	uv = curve(uv);
	float3 oricol = Rendertarget.Sample(pointSampler, float2(q.x, q.y)).xyz;
	float3 col;
	float x = sin(0.3 * B4_GlobalTime.x + uv.y * 21.0) * sin(0.7 * B4_GlobalTime.x + uv.y * 29.0) * sin(0.3 + 0.33 * B4_GlobalTime.x + uv.y * 31.0) * 0.0017;

	col.r = Rendertarget.Sample(pointSampler, uv).x;
	col.g = Rendertarget.Sample(pointSampler, uv).y;
	col.b = Rendertarget.Sample(pointSampler, uv).z;

	//col.r = Rendertarget.Sample(pointSampler, float2(x + uv.x + 0.001, uv.y + 0.001)).x + 0.05;
	//col.g = Rendertarget.Sample(pointSampler, float2(x + uv.x + 0.000, uv.y - 0.002)).y + 0.05;
	//col.b = Rendertarget.Sample(pointSampler, float2(x + uv.x - 0.002, uv.y + 0.000)).z + 0.05;
	
	//col.r += 0.08 * Rendertarget.Sample(pointSampler, 0.75 * float2(x + 0.025, -0.027) + float2(uv.x + 0.001, uv.y + 0.001)).x;
	//col.g += 0.05 * Rendertarget.Sample(pointSampler, 0.75 * float2(x - 0.022, -0.02) + float2(uv.x + 0.000, uv.y - 0.002)).y;
	//col.b += 0.08 * Rendertarget.Sample(pointSampler, 0.75 * float2(x - 0.02, -0.018) + float2(uv.x - 0.002, uv.y + 0.000)).z;

	col = clamp(col * 0.6 + 0.4 * col * col * 1.0, 0.0, 1.0);

	float vig = (0.0 + 1.0 * 16.0 * uv.x * uv.y * (1.0 - uv.x) * (1.0 - uv.y));
	col *= pow(vig, 0.3);

	col *= float3(0.95, 1.05, 0.95);
	col *= 3.8; // ¹à±â
	

	float scans = clamp(0.35 + 0.35 * sin(3.5 * B4_GlobalTime.x + uv.y * B4_Resolution.y * 1.5), 0.0, 1.0);

	float s = pow(scans, 1.7);
	col = col * (0.4 + 0.7 * s);

	col *= 1.0 + 0.01 * sin(110.0 * B4_GlobalTime.x);
	if (uv.x < 0.0 || uv.x > 1.0)
		col *= 0.0;
	if (uv.y < 0.0 || uv.y > 1.0)
		col *= 0.0;

	col *= 1.0 - 0.65 * clamp((fmod(fragCoord.x, 2.0) - 1.0) * 2.0, 0.0, 1.0);

	float comp = smoothstep(0.1, 0.9, sin(B4_GlobalTime.x));
 
    // Remove the next line to stop cross-fade between original and postprocess
    //col = lerp(col, oricol, comp);
	
	//col += noise(uv * 2.f) / 2.f;
	col += stripes(uv);
	
	float vigAmt = 3. + .3 * sin(B4_GlobalTime + 5. * cos(B4_GlobalTime * 5.));
	float vignette = (1. - vigAmt * (uv.y - .5) * (uv.y - .5)) * (1. - vigAmt * (uv.x - .5) * (uv.x - .5));
	col *= vignette;
	
	//gray
	float4 vColor = Rendertarget.Sample(pointSampler, uv);
	float vAver = (col.r + col.g + col.b) / 3.f;
	col = float4(vAver, vAver, vAver, 1.f);

	return float4(col, 1.0);
}