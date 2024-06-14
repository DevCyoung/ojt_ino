#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Resource.h"
#include "DSCollection.h"
#include "RSCollection.h"
#include "BSCollection.h"
#include "SMCollection.h"
#include "EnumShaderBindStage.h"

enum class eResShader;



class	Shader : public Resource
{
	friend class GraphicDeviceDX11;
	friend class ResourceManager;

private:
	Shader() = default;
public:
	Shader(const D3D11_PRIMITIVE_TOPOLOGY topology,
		const eResShader VSRelativePath, const std::wstring& VSFunName,
		const eResShader PSRelativePath, const std::wstring& PSFunName,
		const eSMType SMType, const eRSType RSType, const eDSType DSType, const eBSType BSType);

	Shader(const D3D11_PRIMITIVE_TOPOLOGY topology,
		const std::wstring& VSRelativePath, const std::wstring& VSFunName,
		const std::wstring& PSRelativePath, const std::wstring& PSFunName,
		const eSMType SMType, const eRSType RSType, const eDSType DSType, const eBSType BSType);

	virtual ~Shader();
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	eRSType GetRSType() const  { return mRSType;  };
	eDSType GetDSType() const  { return mDSType;  };
	eBSType GetBSType() const  { return mBSType;  };

private:
	void createVSShader(const std::wstring& VSRelativePath,
		const std::wstring& VSFunName, const eSMType SMType);

	void createPSShader(const std::wstring& PSRelativePath,
		const std::wstring& PSFunName);

	void shaderCompile(const std::wstring& relativePath, 
		const std::wstring& funName, 
		const std::wstring& version,
		ID3DBlob** const ppBlob,
		ID3DBlob** const ppErrorBlob);

	virtual HRESULT Load(const std::wstring& filePath) override;

private:
	D3D11_PRIMITIVE_TOPOLOGY mTopology;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		mInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		mVS;
	Microsoft::WRL::ComPtr<ID3D11HullShader>		mHS;
	Microsoft::WRL::ComPtr<ID3D11DomainShader>		mDS;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>	mGS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		mPS;

	eRSType mRSType;
	eDSType mDSType;
	eBSType mBSType;
};
