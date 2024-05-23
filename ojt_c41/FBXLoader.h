#pragma once

#include "FBXLoadManager.h"
class Mesh;
class GameObject;
class MeshData;
//class tContainer;

//#ifdef _DEBUG
//#pragma comment(lib, "FBXLoader/Debug/FBXLoader_d")
//#else
//#pragma comment(lib, "FBXLoader/Release/FBXLoader")
//#endif

#pragma comment(lib, "shlwapi")


// ============
// Animation 3D
// ============
struct tFrameTrans
{
	Vector4	vTranslate;
	Vector4	vScale;
	Vector4	qRot;
};

struct tMTAnimClip
{
	std::wstring			strAnimName;
	int						iStartFrame;
	int						iEndFrame;
	int						iFrameLength;

	double					dStartTime;
	double					dEndTime;
	double					dTimeLength;
	float					fUpdateTime; // 이거 안씀

	FbxTime::EMode			eMode;
};

struct tMTKeyFrame
{
	double	dTime;
	int		iFrame;
	Vector3	vTranslate;
	Vector3	vScale;
	Vector4	qRot;
};

struct tMTBone
{
	std::wstring				strBoneName;
	int							iDepth;
	int							iParentIndx;
	Matrix						matOffset;	// Offset 행렬(뼈 -> 루트 까지의 행렬)
	Matrix						matBone;   // 이거 안씀
	std::vector<tMTKeyFrame>	vecKeyFrame;
};

class FBXLoader
{
public:
	static MeshData* loadContainer(const tContainer& container);
	static void LoadFBX(const std::wstring& relativePath);
	static void CreateMeshFromFBX();
	static MeshData* FbxInstantiate(const std::wstring& relativePath);
};

