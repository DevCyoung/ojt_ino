#pragma once
#include <wrl.h>
#include <d3d11.h>	
#include "Resource.h"
//#include <Helper\DirectXTex.h>

class Texture : public Resource
{
	friend class ResourceManager;
	friend class GraphicDeviceDX11;

public:
	Texture(UINT _Width, UINT _Height, DXGI_FORMAT _pixelformat
		, UINT _BindFlag, D3D11_USAGE _Usage, bool bCreateView = true);
	Texture();
	virtual ~Texture();
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
public:
	float GetWidth() const { return static_cast<float>(mWidth); }
	float GetHeight() const { return static_cast<float>(mHeight); }	

	virtual HRESULT Load(const std::wstring& filePath) override;

	ID3D11Texture2D* GetID3D11Texture2D() const 
	{
		Assert(mTexture.Get(), ASSERT_MSG_NULL); return mTexture.Get();
	}
	ID3D11ShaderResourceView* GetSRVOrNull() const
	{
		return mSRV.Get();
	}
	ID3D11ShaderResourceView* GetSRV() const 
	{ 
		Assert(mSRV.Get(), ASSERT_MSG_NULL); return mSRV.Get();
	}
	ID3D11RenderTargetView* GetRTV() const 
	{ 
		Assert(mRTV.Get(), ASSERT_MSG_NULL); return mRTV.Get();
	}
	ID3D11DepthStencilView* GetDSV() const 
	{ 
		Assert(mDSV.Get(), ASSERT_MSG_NULL); return mDSV.Get();
	}
	ID3D11UnorderedAccessView* GetUAV() const 
	{
		Assert(mUAV.Get(), ASSERT_MSG_NULL); return mUAV.Get();
	}
	ID3D11RenderTargetView** GetAddressOf()
	{
		Assert(mRTV.GetAddressOf(), ASSERT_MSG_NULL); return mRTV.GetAddressOf();
	}

	void CreateDepthStencilView(const D3D11_DEPTH_STENCIL_VIEW_DESC* const descOrNull);
	void CreateShaderResourceView2(const D3D11_SHADER_RESOURCE_VIEW_DESC* const descOrNull);
	//void UpdateDate(const int slot);

private:
	ScratchImage mImage;
	UINT mWidth;
	UINT mHeight;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				mTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	mSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	    mRTV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	    mDSV;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>   mUAV;
};
