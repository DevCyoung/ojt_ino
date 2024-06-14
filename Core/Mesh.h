#pragma once
#include <wrl.h>
#include <d3d11.h>
#include "Resource.h"
using namespace Microsoft::WRL;

struct tIndexInfo
{
	ComPtr<ID3D11Buffer>    pIB;
	D3D11_BUFFER_DESC       tIBDesc;
	UINT				    iIdxCount;
	void*					pIdxSysMem;
};

class StructuredBuffer;

class Mesh : public Resource
{
	friend class GraphicDeviceDX11;
	friend class ResourceManager;

private:
	Mesh() = default;
public:
	Mesh(
		const void* const vertexs,
		const size_t vertexCount,
		const size_t vertexSize,
		const UINT* const index,
		const size_t indexCount,
		const size_t indexSize);

	Mesh(
		const void* const vertexs,
		const size_t vertexCount,
		const size_t vertexSize,
		const UINT* const indexes,		
		const size_t* const indexCounts,
		const size_t indexesCount,
		const size_t indexSize);

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

public:
	virtual ~Mesh();	
	UINT GetVertexCount() const { return static_cast<UINT>(mVertexCount); }
	//UINT GetIndexCount()  const { return static_cast<UINT>(mIndexCount); }
	virtual HRESULT Load(const std::wstring& filePath) override;
	virtual HRESULT Save(const std::wstring& relativePath) override;
	UINT GetIndexBufferCount() 
	{
		return static_cast<UINT>(mIndexBuffers.size());
	}

	void SetMeshName(const std::wstring& meshName) { mMeshName = meshName; }
	const std::wstring& GetMeshName() { return mMeshName; }
private:
	void addIndexBuffer(const void* const indexs,const 
		size_t indexCount, 
		const size_t indexSize);
	void addVertex(const void* const vertexs);	 

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
	BYTE* mVertexData;
	size_t mVertexCount;
	size_t mVertexSize;
	D3D11_BUFFER_DESC mVertexDesc;	

	std::vector<tIndexInfo> mIndexBuffers;

	UINT*						mIndexesData;
	size_t*						mIndexCounts;
	size_t						mIndexesCount;


public:
	std::wstring				mMeshName;
};
