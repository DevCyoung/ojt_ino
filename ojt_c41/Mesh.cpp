#include "pch.h"
#include "Mesh.h"
#include "Engine.h"
#include "GraphicDeviceDx11.h"
#include "EnumResource.h"
#include "StructuredBuffer.h"
#include "PathManager.h"
//#include <Helper\FileHelper.h>

Mesh::Mesh(
	const void* const vertexs,
	const size_t vertexCount,
	const size_t vertexSize,
	const UINT* const indexs,
	const size_t indexCount,
	const size_t indexSize)
	: Resource(eResourceType::Mesh)
	, mVertexBuffer(nullptr)
	, mVertexData(nullptr)
	, mVertexCount(vertexCount)
	, mVertexSize(vertexSize)
	, mVertexDesc{}
	, mIndexBuffers()	
	, mIndexesData(nullptr)
	, mIndexCounts(nullptr)
	, mIndexesCount(0)
	, mMeshName(L"None")
	, m_pBoneFrameData(nullptr)
	, m_pBoneOffset(nullptr)
{	
	(void)indexSize;

	addVertex(vertexs);
	
	addIndexBuffer(indexs, indexCount, indexSize);


}

Mesh::Mesh(const void* const vertexs,
	const size_t vertexCount,
	const size_t vertexSize,
	const UINT* const indexes,
	const size_t* const indexCounts,
	const size_t indexesCount,
	const size_t indexSize)
	: Resource(eResourceType::Mesh)
	, mVertexBuffer(nullptr)
	, mVertexData(nullptr)
	, mVertexCount(vertexCount)
	, mVertexSize(vertexSize)
	, mVertexDesc{}
	, mIndexBuffers()
	, mIndexesData(nullptr)
	, mIndexCounts(nullptr)
	, mIndexesCount(indexesCount)
	, mMeshName(L"None")
	, m_pBoneFrameData(nullptr)
	, m_pBoneOffset(nullptr)
{
	addVertex(vertexs);
	
	size_t indexesCounts = 0;
	for (size_t i = 0; i < mIndexesCount; ++i)
	{
		indexesCounts += indexCounts[i];
	}

	mIndexesData = new UINT[indexesCounts];
	memcpy(mIndexesData, indexes, indexesCounts * indexSize);

	mIndexCounts = new size_t[mIndexesCount];
	memcpy(mIndexCounts, indexCounts, mIndexesCount * sizeof(size_t));
	
	const UINT* temp = mIndexesData;

	for (size_t i = 0; i < mIndexesCount; ++i)
	{
		if (mIndexCounts[i] <= 0)
		{
			continue;
		}

		addIndexBuffer(temp, mIndexCounts[i], indexSize);
		temp += mIndexCounts[i];
	}
}

Mesh::~Mesh()
{	
	for (tIndexInfo& info : mIndexBuffers)
	{
		DELETE_POINTER(info.pIdxSysMem);
	}
	mIndexBuffers.clear();
	DELETE_POINTER(mVertexData);
	DELETE_POINTER(m_pBoneFrameData);
	DELETE_POINTER(m_pBoneOffset);	


	DELETE_POINTER(mIndexesData);
	DELETE_POINTER(mIndexCounts);

}

HRESULT Mesh::Load(const std::wstring& filePath)
{
	// 읽기모드로 파일열기
	FILE* file = nullptr;
	errno_t err = _wfopen_s(&file, filePath.c_str(), L"rb");
	(void)err;
	//Assert(err, ASSERT_MSG_INVALID);
	Assert(file, ASSERT_MSG_NULL);

	std::wstring resourcePath = gPathManager->GetResourcePath();
	std::wstring relativePath = filePath.substr(resourcePath.length());
	
	SetRelativePath(relativePath);

	// 정점데이터		
	fread(&mVertexSize, sizeof(size_t), 1, file);
	fread(&mVertexCount, sizeof(size_t), 1, file);
	const size_t VERTEX_BYTE_WIDTH = mVertexSize * mVertexCount;
	BYTE* vertexData = new BYTE[VERTEX_BYTE_WIDTH];
	fread(vertexData, 1, VERTEX_BYTE_WIDTH, file);
	addVertex(vertexData);
	DELETE_ARRAY(vertexData);

	//인덱스	
	fread(&mIndexesCount, sizeof(size_t), 1, file);
	mIndexCounts = new size_t[mIndexesCount];
	fread(mIndexCounts, sizeof(size_t) * mIndexesCount, 1, file);
	size_t indexesCounts = 0;
	for (size_t i = 0; i < mIndexesCount; ++i)
	{
		indexesCounts += mIndexCounts[i];
	}
	mIndexesData = new UINT[indexesCounts];
	fread(mIndexesData, sizeof(UINT) * indexesCounts, 1, file);
	const UINT* temp = mIndexesData;
	for (size_t i = 0; i < mIndexesCount; ++i)
	{
		if (mIndexCounts[i] <= 0)
		{
			continue;
		}

		addIndexBuffer(temp, mIndexCounts[i], sizeof(UINT));
		temp += mIndexCounts[i];
	}


	//애니메이션
	UINT iCount = 0;
	fread(&iCount, sizeof(UINT), 1, file);
	for (UINT i = 0; i < iCount; ++i)
	{
		tMTAnimClip tClip = {};

		LoadWString(&tClip.strAnimName, file);
		fread(&tClip.dStartTime, sizeof(double), 1, file);
		fread(&tClip.dEndTime, sizeof(double), 1, file);
		fread(&tClip.dTimeLength, sizeof(double), 1, file);
		fread(&tClip.eMode, sizeof(int), 1, file);
		fread(&tClip.fUpdateTime, sizeof(float), 1, file);
		fread(&tClip.iStartFrame, sizeof(int), 1, file);
		fread(&tClip.iEndFrame, sizeof(int), 1, file);
		fread(&tClip.iFrameLength, sizeof(int), 1, file);

		m_vecAnimClip.push_back(tClip);
	}

	iCount = 0;
	fread(&iCount, sizeof(UINT), 1, file);
	m_vecBones.resize(iCount);

	UINT _iFrameCount = 0;
	for (UINT i = 0; i < iCount; ++i)
	{
		LoadWString(&m_vecBones[i].strBoneName, file);
		fread(&m_vecBones[i].iDepth, sizeof(int), 1, file);
		fread(&m_vecBones[i].iParentIndx, sizeof(int), 1, file);
		fread(&m_vecBones[i].matBone, sizeof(Matrix), 1, file);
		fread(&m_vecBones[i].matOffset, sizeof(Matrix), 1, file);

		UINT iFrameCount = 0;
		fread(&iFrameCount, sizeof(int), 1, file);
		m_vecBones[i].vecKeyFrame.resize(iFrameCount);
		_iFrameCount = max(_iFrameCount, iFrameCount);
		for (UINT j = 0; j < iFrameCount; ++j)
		{
			fread(&m_vecBones[i].vecKeyFrame[j], sizeof(tMTKeyFrame), 1, file);
		}
	}

	// Animation 이 있는 Mesh 경우 structuredbuffer 만들어두기
	if (m_vecAnimClip.size() > 0 && m_vecBones.size() > 0)
	{
		// BoneOffet 행렬
		std::vector<Matrix> vecOffset;
		std::vector<tFrameTrans> vecFrameTrans;
		vecFrameTrans.resize((UINT)m_vecBones.size() * _iFrameCount);

		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			vecOffset.push_back(m_vecBones[i].matOffset);

			for (size_t j = 0; j < m_vecBones[i].vecKeyFrame.size(); ++j)
			{
				vecFrameTrans[(UINT)m_vecBones.size() * j + i]
					= tFrameTrans{ Vector4(m_vecBones[i].vecKeyFrame[j].vTranslate.x,
										   m_vecBones[i].vecKeyFrame[j].vTranslate.y,
										   m_vecBones[i].vecKeyFrame[j].vTranslate.z, 0.f),
								   Vector4(m_vecBones[i].vecKeyFrame[j].vScale.x,
										   m_vecBones[i].vecKeyFrame[j].vScale.y,
										   m_vecBones[i].vecKeyFrame[j].vScale.z, 0.f)
					, m_vecBones[i].vecKeyFrame[j].qRot };
			}
		}

		m_pBoneOffset = new StructuredBuffer(eSBType::BoneOffset,
			eSRVTpye::BoneOffset,
			sizeof(Matrix),
			(UINT)vecOffset.size(),
			vecOffset.data(),
			gGraphicDevice->UnSafe_GetDevice());

		m_pBoneFrameData = new StructuredBuffer(eSBType::BoneFrameData,
			eSRVTpye::BoneFrameData,
			sizeof(tFrameTrans),
			(UINT)vecOffset.size() * _iFrameCount,
			vecFrameTrans.data(),
			gGraphicDevice->UnSafe_GetDevice());
	}


	fclose(file);

	return S_OK;
}

HRESULT Mesh::Save(const std::wstring& relativePath)
{
	std::wstring filePath =  gPathManager->GetResourcePath() + relativePath;

	FILE* file = nullptr;
	errno_t err = _wfopen_s(&file, filePath.c_str(), L"wb");
	(void)err;

	//Assert(err, ASSERT_MSG_INVALID);
	Assert(file, ASSERT_MSG_NULL);

	// 정점			
	fwrite(&mVertexSize, sizeof(size_t), 1, file);
	fwrite(&mVertexCount, sizeof(size_t), 1, file);

	const size_t VERTEX_BYTE_WIDTH = mVertexSize * mVertexCount;
	fwrite(mVertexData, VERTEX_BYTE_WIDTH, 1, file);

	//인덱스
	fwrite(&mIndexesCount, sizeof(size_t), 1, file);
	fwrite(mIndexCounts, sizeof(size_t) * mIndexesCount, 1, file);
	size_t indexesCounts = 0;
	for (size_t i = 0; i < mIndexesCount; ++i)
	{
		indexesCounts += mIndexCounts[i];
	}
	fwrite(mIndexesData, sizeof(UINT) * indexesCounts, 1, file);


	//애니메이션
	const UINT ANIMATION_CLIP_COUNT = static_cast<UINT>(m_vecAnimClip.size());
	fwrite(&ANIMATION_CLIP_COUNT, sizeof(UINT), 1, file);
	for (UINT i = 0; i < ANIMATION_CLIP_COUNT; ++i)
	{
		SaveWString(m_vecAnimClip[i].strAnimName, file);
		fwrite(&m_vecAnimClip[i].dStartTime, sizeof(double), 1, file);
		fwrite(&m_vecAnimClip[i].dEndTime, sizeof(double), 1, file);
		fwrite(&m_vecAnimClip[i].dTimeLength, sizeof(double), 1, file);
		fwrite(&m_vecAnimClip[i].eMode, sizeof(int), 1, file);
		fwrite(&m_vecAnimClip[i].fUpdateTime, sizeof(float), 1, file);
		fwrite(&m_vecAnimClip[i].iStartFrame, sizeof(int), 1, file);
		fwrite(&m_vecAnimClip[i].iEndFrame, sizeof(int), 1, file);
		fwrite(&m_vecAnimClip[i].iFrameLength, sizeof(int), 1, file);
	}

	//뼈정보
	const UINT BONE_COUNT = static_cast<UINT>(m_vecBones.size());
	fwrite(&BONE_COUNT, sizeof(UINT), 1, file);

	for (UINT i = 0; i < BONE_COUNT; ++i)
	{
		SaveWString(m_vecBones[i].strBoneName, file);
		fwrite(&m_vecBones[i].iDepth, sizeof(int), 1, file);
		fwrite(&m_vecBones[i].iParentIndx, sizeof(int), 1, file);
		fwrite(&m_vecBones[i].matBone, sizeof(Matrix), 1, file);
		fwrite(&m_vecBones[i].matOffset, sizeof(Matrix), 1, file);

		int iFrameCount = (int)m_vecBones[i].vecKeyFrame.size();
		fwrite(&iFrameCount, sizeof(int), 1, file);

		for (int j = 0; j < m_vecBones[i].vecKeyFrame.size(); ++j)
		{
			fwrite(&m_vecBones[i].vecKeyFrame[j], sizeof(tMTKeyFrame), 1, file);
		}
	}

	fclose(file);

	return S_OK;
}

void Mesh::addIndexBuffer(const void* const indexs, 
	const size_t indexCount, 
	const size_t indexSize)
{
	D3D11_BUFFER_DESC indexDexc = {};
	indexDexc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	indexDexc.CPUAccessFlags = 0;
	indexDexc.Usage = D3D11_USAGE_DEFAULT;
	indexDexc.ByteWidth = static_cast<UINT>(indexCount * indexSize);
	
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	tIndexInfo info = {};
	info.pIdxSysMem = new BYTE[indexDexc.ByteWidth];
	memcpy(info.pIdxSysMem, indexs, indexDexc.ByteWidth);

	D3D11_SUBRESOURCE_DATA tIndexSub = {};
	tIndexSub.pSysMem = info.pIdxSysMem;

	if (FAILED(gGraphicDevice->UnSafe_GetDevice()->CreateBuffer(&indexDexc, &tIndexSub, indexBuffer.GetAddressOf())))
	{
		Assert(false, ASSERT_MSG("failed to create index buffer"));
	}
	
	info.iIdxCount = static_cast<UINT>(indexCount);
	info.pIB = indexBuffer;
	info.tIBDesc = indexDexc;	

	mIndexBuffers.push_back(info);
}

void Mesh::addVertex(const void* const vertexs)
{		
	mVertexDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	mVertexDesc.CPUAccessFlags = 0;
	mVertexDesc.Usage = D3D11_USAGE_DEFAULT;
	mVertexDesc.ByteWidth = static_cast<UINT>(mVertexCount * mVertexSize);

	DELETE_POINTER(mVertexData);
	mVertexData = new BYTE[mVertexDesc.ByteWidth];
	memcpy(mVertexData, vertexs, mVertexDesc.ByteWidth);
	
	D3D11_SUBRESOURCE_DATA outVertexSub = {};
	outVertexSub.pSysMem = mVertexData;
	if (FAILED(gGraphicDevice->UnSafe_GetDevice()->CreateBuffer(&mVertexDesc, &outVertexSub, mVertexBuffer.GetAddressOf())))
	{
		Assert(false, ASSERT_MSG("failed to create vertex buffer"));
	}
}

