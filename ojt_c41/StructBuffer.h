#pragma once

struct alignas(16) tCBTransform // b0
{
	Matrix World;
	Matrix WorldInv;

	Matrix View;
	Matrix ViewInv;

	Matrix Proj;
	Matrix ProjInv;

	Matrix WV;
	Matrix WVP;
}; // 80 bytes


struct /*alignas(16)*/ tCBColorInfo // b1
{
	int	bUseColor;
	int bColorType;
	Vector2 Dummy;
	Vector4 Color;

	int bSetColorR;
	float R;
	int bSetColorG;
	float G;
	int bSetColorB;
	float B;
	int bSetColorA;
	float A;

	int bMulColorR;
	float MulR;
	int bMulColorG;
	float MulG;
	int bMulColorB;
	float MulB;
	int bMulColorA;
	float MulA;
}; // 32 bytes


struct alignas(16) tCBDebugInfo // b2
{
	Vector3 MouseWorldPos;
	float dummy;
	Vector2 Scale;
	int XYCount[2];

	Vector4 Color_0;
	Vector4 Color_1;
	Vector4 Color_2;
	Vector4 Color_3;

	Vector4 Vector4_0;
	Vector4 Vector4_1;
	Vector4 Vector4_2;
	Vector4 Vector4_3;

}; // 16 bytes


struct alignas(16) tCBAnimationInfo // b3
{
	Vector2 UVBackLeftTop;
	Vector2 UVBackSIze;

	Vector2 UVLeftTop;
	Vector2 UVSize;

	Vector2 UVOffset;
	Vector2 Dummy;
}; // 16 bytes


struct alignas(16) tLightColor
{
	Vector4 Diffuse;	// ���� ����
	Vector4 Ambient;	// �ֺ� ��(ȯ�� ��)
	Vector4 Specular; // �ݻ籤 ����
};

// LightInfo
struct alignas(16) tLightInfo // b4
{
	tLightColor Color;		 // ���� ����

	Vector4		WorldPos;   // ������ ���� �����̽� ��ġ
	Vector4		WorldDir;	 // ���� ������ ����

	UINT		LightType;   // ���� Ÿ��(���⼺, ��, ����Ʈ)
	float		Radius;		 // ���� �ݰ�(��Ÿ�)
	float		Angle;		 // ���� ����
	int			padding;

	Matrix		LightPV;
};

struct alignas(16) tGlobalInfo // b5
{
	Vector2	Resolution;
	float	Deltatime;
	float   GlobalTime;
	float	AccTime;	
	UINT	Light2DCount;
	UINT	Light3DCount;
	int		globalpadding;
};


struct alignas(16) tParticleInfo // 
{
	Vector2	Resolution;
	float	Deltatime;
	float   GlobalTime;
	float	AccTime;
	UINT	Light2DCount;
	UINT	Light3DCount;
	int		globalpadding;
};

struct /*alignas(16)*/ tSprite2DInfo // b5
{
	Vector2 Dummy;
	Vector2 UV;

	int bSetColorR;
	float R;
	int bSetColorG;
	float G;
	int bSetColorB;	
	float B;	
	int bSetColorA;
	float A;


	int bMulColorR;
	float MulR;
	int bMulColorG;
	float MulG;
	int bMulColorB;
	float MulB;
	int bMulColorA;
	float MulA;
};

struct alignas(16) tWaveInfo // b6
{
	float WaveXPower;
	float WaveYPower;
	float WaveSpeed;	
	float dummy2;
};
enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,
	TEX_6,
	TEX_7,

	TEX_CUBE_0,
	TEX_CUBE_1,

	TEX_ARR_0,
	TEX_ARR_1,

	TEX_END,
};

// Material ���
struct tMtrlData
{
	Vector4 vDiff;
	Vector4 vSpec;
	Vector4 vAmb;
	Vector4 vEmv;
};

struct /*alignas(16)*/ tMaterial // b7
{
	tMtrlData mtrl;

	int arrInt[16];
	float arrFloat[16];
	Vector2 arrV2[16];
	Vector4 arrV4[16];
	Matrix arrMat[16];

	// �ؽ��� ���� true / false �뵵
	int arrBTex[(UINT)TEX_PARAM::TEX_END];

	// 3D Animation ����
	int	arrAnimData[4];
};

//static float3 g_vLightPos = float3(0.f, 0.f, 0.f);
//static float3 g_vLightDir = float3(1.f, -1.f, 1.f);
//
//static float3 g_vLightColor = float3(1.f, 1.f, 1.f);
//static float g_fLightSpecCoeff = 0.3f;
//static float3 g_vLightAmb = float3(0.15f, 0.15f, 0.15f); // ȯ�汤
//¦��������
struct /*alignas(16)*/ tLight3DInfo
{
	Vector4 LightPos;
	Vector4 LightDir;

	Vector4 LightColor;
	Vector4 LightAmb;
	
	float	LightSpecCoeff;
	int		pad[3];
};


struct /*alignas(16)*/ tGraphicDebug
{
	int Debug0;
	int Debug1;
	int Debug2;
	int Debug3;

	int Debug4;
	int Debug5;
	int Debug6;
	int Debug7;

	float Fdebug0;
	float Fdebug1;
	float Fdebug2;
	float Fdebug3;

	float Fdebug4;
	float Fdebug5;
	float Fdebug6;
	float Fdebug7;
};

