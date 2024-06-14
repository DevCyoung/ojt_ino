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
	DELETE_POINTER(mIndexesData);
	DELETE_POINTER(mIndexCounts);

}

HRESULT Mesh::Load(const std::wstring& filePath)
{
	return S_OK;
}

HRESULT Mesh::Save(const std::wstring& relativePath)
{
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

