#include "pch.h"
#include "Texture.h"
#include "Engine.h"
#include "GraphicDeviceDx11.h"
#include "String.h"
#include "EnumResource.h"

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/Debug/DirectXTex_d.lib")
#else
#pragma comment(lib, "DirectXTex/Release/DirectXTex.lib")
#endif

Texture::Texture(const UINT width, 
	const UINT height, 
	const DXGI_FORMAT pixelFormat, 
	const UINT bindFlag, 
	const D3D11_USAGE usage, 
	const bool bCreateView)
	: Texture()	
{
	mWidth = width;
	mHeight = height;

	D3D11_TEXTURE2D_DESC                m_Desc = {};

	// ID3D11Texture2D 생성
	m_Desc.Format = pixelFormat;

	// 반드시 렌더타겟과 같은 해상도로 설정해야 함
	m_Desc.Width = width;
	m_Desc.Height = height;
	m_Desc.ArraySize = 1;

	m_Desc.BindFlags = bindFlag;
	m_Desc.Usage = usage;

	//if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == _Usage)
	//{
	//	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//}		
	//else if (D3D11_USAGE::D3D11_USAGE_STAGING == _Usage)
	//{
	//	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	//}
		
	m_Desc.MipLevels = 1;    // 원본만 생성
	m_Desc.SampleDesc.Count = 1;
	m_Desc.SampleDesc.Quality = 0;

	ID3D11Device* const device = gGraphicDevice->UnSafe_GetDevice();

	if (FAILED(device->CreateTexture2D(&m_Desc, nullptr, mTexture.GetAddressOf())))
	{
		Assert(false, ASSERT_MSG_INVALID);
	}

	if (!bCreateView)
	{
		return;
	}

	// 바인드 플래그에 맞는 View 를 생성해준다.
	if (m_Desc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL)
	{
		CreateDepthStencilView(nullptr);
	}
	else
	{
		if (m_Desc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET)
		{
			if (FAILED(device->CreateRenderTargetView(mTexture.Get(), nullptr, mRTV.GetAddressOf())))
			{
				Assert(false, ASSERT_MSG_INVALID);
			}
		}

		if (m_Desc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE)
		{
			CreateShaderResourceView2(nullptr);
		}

		if (m_Desc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS)
		{
			if (FAILED(device->CreateUnorderedAccessView(mTexture.Get(), nullptr, mUAV.GetAddressOf())))
			{
				Assert(false, ASSERT_MSG_INVALID);
			}
		}
	}
}

Texture::Texture()
	: Resource(eResourceType::Texture)
	, mImage()
	, mWidth(0)
	, mHeight(0)
	, mTexture(nullptr)
	, mSRV(nullptr)
	, mRTV(nullptr)
	, mDSV(nullptr)
	, mUAV(nullptr)
{
}

Texture::~Texture()
{
}

HRESULT Texture::Load(const std::wstring& filePath)
{
	const std::wstring& FILE_EXTENSION = StringHelper::GetFileExtension(filePath);

	if (FILE_EXTENSION == L".dds" || FILE_EXTENSION == L".DDS")
	{
		if (FAILED(LoadFromDDSFile(filePath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, mImage)))
		{
			Assert(false, ASSERT_MSG("fail load file .dds"));
		}
	}
	else if (FILE_EXTENSION == L".tga" || FILE_EXTENSION == L".TGA")
	{
		if (FAILED(LoadFromTGAFile(filePath.c_str(), nullptr, mImage)))
		{
			Assert(false, ASSERT_MSG("fail load file .tga"));
		}
	}
	else if (FILE_EXTENSION == L".png" || FILE_EXTENSION == L".PNG" ||
		FILE_EXTENSION == L".jpg" || FILE_EXTENSION == L".JPG" ||
		FILE_EXTENSION == L".jpeg" || FILE_EXTENSION == L".JPEG" ||
		FILE_EXTENSION == L".bmp" || FILE_EXTENSION == L".BMP") // WIC (png, jpg, jpeg, bmp )
	{
		if (FAILED(LoadFromWICFile(filePath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, mImage)))
		{
			Assert(false, ASSERT_MSG("fail load file other"));
		}
	}
	else
	{				
		Assert(false, ASSERT_MSG("is not supported"));
	}

	if (FAILED(CreateShaderResourceView(gGraphicDevice->UnSafe_GetDevice()
		, mImage.GetImages()
		, mImage.GetImageCount()
		, mImage.GetMetadata(), mSRV.GetAddressOf())))
	{
		Assert(false, ASSERT_MSG("fail create resource view"));
	}

	Assert(mSRV, ASSERT_MSG_NULL);

	mSRV->GetResource(reinterpret_cast<ID3D11Resource**>(mTexture.GetAddressOf()));

	mWidth  = static_cast<UINT>(mImage.GetMetadata().width);
	mHeight = static_cast<UINT>(mImage.GetMetadata().height);

	return S_OK;
}

void Texture::CreateDepthStencilView(const D3D11_DEPTH_STENCIL_VIEW_DESC* const descOrNull)
{	
	Assert(!mDSV.Get(), ASSERT_MSG_NOT_NULL);

	ID3D11Device* const device = gGraphicDevice->UnSafe_GetDevice();

	if (FAILED(device->CreateDepthStencilView(mTexture.Get(), descOrNull, mDSV.GetAddressOf())))
	{
		Assert(false, ASSERT_MSG_INVALID);
	}
}

void Texture::CreateShaderResourceView2(const D3D11_SHADER_RESOURCE_VIEW_DESC* const descOrNull)
{	
	Assert(!mSRV.Get(), ASSERT_MSG_NOT_NULL);

	ID3D11Device* const device = gGraphicDevice->UnSafe_GetDevice();

	if (FAILED(device->CreateShaderResourceView(mTexture.Get(), descOrNull, mSRV.GetAddressOf())))
	{
		Assert(false, ASSERT_MSG_INVALID);
	}
}

