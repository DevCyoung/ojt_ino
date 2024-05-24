#include "pch.h"
#include "GraphicDeviceDX11.h"
#include "Texture.h"
#include "Shader.h"
#include "ComputeShader.h"
#include "Mesh.h"
#include "CBCollection.h"
#include "RSCollection.h"
#include "BSCollection.h"
#include "DSCollection.h"
#include "SMCollection.h"
#include "SBCollection.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#pragma region Comment Date : 2023-05-26 
/*
* Trouble : Fxing
* Date : 2023-05-26
*
* 현재 SwapChain생성중 Warning 발생
* DXGI WARNING: IDXGIFactory::CreateSwapChain: Blt-model swap effects (DXGI_SWAP_EFFECT_DISCARD and DXGI_SWAP_EFFECT_SEQUENTIAL) are legacy swap effects that are predominantly superceded by their flip-model counterparts (DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL and DXGI_SWAP_EFFECT_FLIP_DISCARD). Please consider updating your application to leverage flip-model swap effects to benefit from modern presentation enhancements. More information is available at http://aka.ms/dxgiflipmodel. [ MISCELLANEOUS WARNING #294: ]*
* DXGI_SWAP_CHAIN_DESC 구조체의 swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
* 부분을 DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD 으로 교체하면 Warning은 사라지지만 프레임이 60으로 고정
* 되거나 혹은 떨어지는 현상이 발생한다.
* 해결하려고 시도중입니다.
* Date : 2023-05-29
* 아직진행중...
*
* Trouble : Fxing
* Date : 2023-07-02
*
* 다시 알아보던중 실제 모니터의 해상도에 맞춰서 present에 블럭또는 병목현상이 걸리는것으로 유추하게되었음
* 동기 블럭킹 함수인것으로 생각되며 넌블럭킹으로 바꿔야할것으로 보인다.
* 이것저것 알아보고 테스트도 많이해서 갈아 엎어야 할것으로 생각되므로 추후에 다시 진행할 예정
*/
#pragma endregion

#pragma region Constructor
GraphicDeviceDX11::GraphicDeviceDX11(const HWND hWnd,
	const UINT renderTargetWidth, const UINT renderTargetHeight)
	: mCBCollection(nullptr)
	, mRSCollection(nullptr)
	, mBSCollection(nullptr)
	, mDSCollection(nullptr)
	, mSMCollection(nullptr)
	, mSBCollection(nullptr)
	, mDxStateData{}
{
	Assert(hWnd, ASSERT_MSG_NULL);

#pragma region Create Device And Context

#ifdef _DEBUG
	constexpr UINT DEVICE_FLAG = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#else		
	constexpr UINT DEVICE_FLAG = 0;
#endif

	//https://learn.microsoft.com/ko-kr/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_feature_level
	//셰이더 모델 5를 포함하여 Direct3D 11.0에서 지원하는 기능을 대상으로 합니다.
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, DEVICE_FLAG, nullptr, 0, D3D11_SDK_VERSION,
		mDevice.GetAddressOf(), &featureLevel,
		mContext.GetAddressOf())))
	{
		Assert(false, ASSERT_MSG("failed to create device"));
		return;
	}

	Assert(mDevice.Get(), ASSERT_MSG_NULL);
	Assert(mContext.Get(), ASSERT_MSG_NULL);

#pragma endregion

#pragma region Create SwaphChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	swapChainDesc.BufferCount = 2;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = TRUE;

	swapChainDesc.BufferDesc.Width = renderTargetWidth;
	swapChainDesc.BufferDesc.Height = renderTargetHeight;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 240;   //최대프레임
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;   //최소프레임
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM; //픽셀 포맷          

	Microsoft::WRL::ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGIAdapter> pDXGIAdapter = nullptr;
	Microsoft::WRL::ComPtr<IDXGIFactory> pDXGIFactory = nullptr;

	if (FAILED(mDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(pDXGIDevice.GetAddressOf()))))
	{
		Assert(false, ASSERT_MSG("failed to create IDXGIDevice"));
		return;
	}

	if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(pDXGIAdapter.GetAddressOf()))))
	{
		Assert(false, ASSERT_MSG("failed to create IDXGIAdapter"));
		return;
	}

	if (FAILED(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pDXGIFactory.GetAddressOf()))))
	{
		Assert(false, ASSERT_MSG("failed to create IDXGIFactory"));
		return;
	}

	if (FAILED(pDXGIFactory->CreateSwapChain(mDevice.Get(), &swapChainDesc, mSwapChain.GetAddressOf())))
	{
		Assert(false, ASSERT_MSG("failed to create IDXGISwapChain"));
		return;
	}

	if (FAILED(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(mRenderTargetTexture.GetAddressOf()))))
	{
		Assert(false, ASSERT_MSG("failed to get buffer"));
		return;
	}

	if (FAILED(mDevice->CreateRenderTargetView(mRenderTargetTexture.Get(), nullptr, mRenderTargetView.GetAddressOf())))
	{
		Assert(false, ASSERT_MSG("failed to create render target view"));
		return;
	}
#pragma endregion

#pragma region Create DepthStencil
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};

	depthStencilDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.Width = renderTargetWidth;
	depthStencilDesc.Height = renderTargetHeight;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.MiscFlags = 0;

	if (FAILED(mDevice->CreateTexture2D(&depthStencilDesc, nullptr, mDepthStencilTexture.GetAddressOf())))
	{
		Assert(false, ASSERT_MSG("failed to create depth stencil texture"));
		return;
	}

	if (FAILED(mDevice->CreateDepthStencilView(mDepthStencilTexture.Get(), nullptr, mDepthStencilView.GetAddressOf())))
	{
		Assert(false, ASSERT_MSG("failed to create depth stencil view"));
		return;
	}
#pragma endregion

#pragma region Create Sampler
	D3D11_SAMPLER_DESC tSamDesc = {};

	tSamDesc = {};
	tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	mDevice->CreateSamplerState(&tSamDesc, m_Sampler[0].GetAddressOf());
	mContext->VSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	mContext->HSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	mContext->DSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	mContext->GSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	mContext->PSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());

	tSamDesc = {};
	tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	mDevice->CreateSamplerState(&tSamDesc, m_Sampler[1].GetAddressOf());
	mContext->VSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	mContext->HSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	mContext->DSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	mContext->GSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	mContext->PSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
#pragma endregion

#pragma region Create Collection
	mCBCollection = new CBCollection(mDevice.Get());
	mRSCollection = new RSCollection(mDevice.Get());
	mBSCollection = new BSCollection(mDevice.Get());
	mDSCollection = new DSCollection(mDevice.Get());
	mSMCollection = new SMCollection();
	mSBCollection = new SBCollection(mDevice.Get());
#pragma endregion

}
#pragma endregion

GraphicDeviceDX11::~GraphicDeviceDX11()
{
	DELETE_POINTER_NOT_NULL(mSBCollection);
	DELETE_POINTER_NOT_NULL(mSMCollection);
	DELETE_POINTER_NOT_NULL(mDSCollection);
	DELETE_POINTER_NOT_NULL(mBSCollection);
	DELETE_POINTER_NOT_NULL(mRSCollection);
	DELETE_POINTER_NOT_NULL(mCBCollection);
}

void GraphicDeviceDX11::BindIA(const Shader* const shader) const
{
	Assert(shader, ASSERT_MSG_NULL);
	Assert(shader->mInputLayout.Get(), ASSERT_MSG_NULL);

	mContext->IASetInputLayout(shader->mInputLayout.Get());
	mContext->IASetPrimitiveTopology(shader->mTopology);
}

void GraphicDeviceDX11::BindMesh(const Mesh* const mesh, int idex) const
{
	Assert(mesh, ASSERT_MSG_NULL);
	Assert(mesh->mVertexBuffer, ASSERT_MSG_NULL);

	const UINT OFFSET = 0;
	const UINT STRIDE = static_cast<UINT>(mesh->mVertexSize);

	mContext->IASetVertexBuffers(0, 1, mesh->mVertexBuffer.GetAddressOf(), &STRIDE, &OFFSET);
	mContext->IASetIndexBuffer(mesh->mIndexBuffers[idex].pIB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void GraphicDeviceDX11::BindSRV(const eShaderBindType stageType,
	const UINT startSlot,
	const Texture* const texture) const
{
	Assert(eShaderBindType::End != stageType, ASSERT_MSG_INVALID);
	Assert(texture, ASSERT_MSG_NULL);
	Assert(texture->mSRV, ASSERT_MSG_NULL);

	switch (stageType)
	{
	case eShaderBindType::VS:
		mContext->VSSetShaderResources(startSlot, 1, texture->mSRV.GetAddressOf());
		break;
	case eShaderBindType::HS:
		mContext->HSSetShaderResources(startSlot, 1, texture->mSRV.GetAddressOf());
		break;
	case eShaderBindType::DS:
		mContext->DSSetShaderResources(startSlot, 1, texture->mSRV.GetAddressOf());
		break;
	case eShaderBindType::GS:
		mContext->GSSetShaderResources(startSlot, 1, texture->mSRV.GetAddressOf());
		break;
	case eShaderBindType::PS:
		mContext->PSSetShaderResources(startSlot, 1, texture->mSRV.GetAddressOf());
		break;
	case eShaderBindType::CS:
		mContext->CSSetShaderResources(startSlot, 1, texture->mSRV.GetAddressOf());
		break;
	default:
		Assert(false, ASSERT_MSG_SWITCH_DEFAULT);
		break;
	}
}

void GraphicDeviceDX11::BindUAV(const UINT startSlot, const Texture* const texture) const
{	
	Assert(texture, ASSERT_MSG_NULL);
	Assert(texture->mUAV, ASSERT_MSG_NULL);

	UINT i = static_cast<UINT>(-1);	
	mContext->CSSetUnorderedAccessViews(startSlot, 1, texture->mUAV.GetAddressOf(), &i);	
}

void GraphicDeviceDX11::UnBindUAV(const UINT startSlot) const
{
	ID3D11UnorderedAccessView* const pUAV = nullptr;	

	//UINT i = static_cast<UINT>(-1);
	mContext->CSSetUnorderedAccessViews(startSlot, 1, &pUAV, nullptr);
}

void GraphicDeviceDX11::BindCB(const eCBType CBType, const eShaderBindType stageType) const
{
	Assert(eCBType::End != CBType, ASSERT_MSG_INVALID);
	Assert(eShaderBindType::End != stageType, ASSERT_MSG_INVALID);

	const ConstantBuffer& CB = mCBCollection->GetConstantBuffer(CBType);
	const UINT START_SLOT = static_cast<UINT>(CB.mType);

	switch (stageType)
	{
	case eShaderBindType::VS:
		mContext->VSSetConstantBuffers(START_SLOT, 1, CB.mBuffer.GetAddressOf());
		break;
	case eShaderBindType::HS:
		mContext->HSSetConstantBuffers(START_SLOT, 1, CB.mBuffer.GetAddressOf());
		break;
	case eShaderBindType::DS:
		mContext->DSSetConstantBuffers(START_SLOT, 1, CB.mBuffer.GetAddressOf());
		break;
	case eShaderBindType::GS:
		mContext->GSSetConstantBuffers(START_SLOT, 1, CB.mBuffer.GetAddressOf());
		break;
	case eShaderBindType::PS:
		mContext->PSSetConstantBuffers(START_SLOT, 1, CB.mBuffer.GetAddressOf());
		break;
	case eShaderBindType::CS:
		mContext->CSSetConstantBuffers(START_SLOT, 1, CB.mBuffer.GetAddressOf());
		break;
	default:
		Assert(false, ASSERT_MSG_SWITCH_DEFAULT);
		break;
	}
}

void GraphicDeviceDX11::BindCBAllShader(const eCBType CBType) const
{
	Assert(eCBType::End != CBType, ASSERT_MSG_INVALID);	

	const ConstantBuffer& CB = mCBCollection->GetConstantBuffer(CBType);
	const UINT START_SLOT = static_cast<UINT>(CB.mType);

	mContext->VSSetConstantBuffers(START_SLOT, 1, CB.mBuffer.GetAddressOf());
	mContext->HSSetConstantBuffers(START_SLOT, 1, CB.mBuffer.GetAddressOf());
	mContext->DSSetConstantBuffers(START_SLOT, 1, CB.mBuffer.GetAddressOf());
	mContext->GSSetConstantBuffers(START_SLOT, 1, CB.mBuffer.GetAddressOf());
	mContext->PSSetConstantBuffers(START_SLOT, 1, CB.mBuffer.GetAddressOf());
}

void GraphicDeviceDX11::PassCB(const eCBType CBType, const UINT dataSize, const void* const data) const
{
	Assert(eCBType::End != CBType, ASSERT_MSG_INVALID);
	Assert(data, ASSERT_MSG_NULL);

	UNREFERENCED_PARAMETER(dataSize);

	ConstantBuffer& CB = mCBCollection->GetConstantBuffer(CBType);

	Assert(CB.mSize == dataSize, ASSERT_MSG("data size not ali 16"));

	D3D11_MAPPED_SUBRESOURCE subResource = {};

	mContext->Map(CB.mBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy_s(subResource.pData, CB.mDesc.ByteWidth, data, CB.mDesc.ByteWidth);
	}
	mContext->Unmap(CB.mBuffer.Get(), 0);
}

void GraphicDeviceDX11::BindSB(const eSBType SBType, const eShaderBindType stageType) const
{
	Assert(eSBType::End != SBType, ASSERT_MSG_INVALID);
	Assert(eShaderBindType::End != stageType, ASSERT_MSG_INVALID);

	const StructuredBuffer* SB = mSBCollection->GetStructuredBuffer(SBType);	
	const UINT START_SLOT = static_cast<UINT>(SB->mSRVType);	

	switch (stageType)
	{
	case eShaderBindType::VS:
		mContext->VSSetShaderResources(START_SLOT, 1, SB->mSRV.GetAddressOf());
		break;
	case eShaderBindType::HS:
		mContext->HSSetShaderResources(START_SLOT, 1, SB->mSRV.GetAddressOf());
		break;
	case eShaderBindType::DS:
		mContext->DSSetShaderResources(START_SLOT, 1, SB->mSRV.GetAddressOf());
		break;
	case eShaderBindType::GS:
		mContext->GSSetShaderResources(START_SLOT, 1, SB->mSRV.GetAddressOf());
		break;
	case eShaderBindType::PS:
		mContext->PSSetShaderResources(START_SLOT, 1, SB->mSRV.GetAddressOf());
		break;
	case eShaderBindType::CS:
		mContext->CSSetShaderResources(START_SLOT, 1, SB->mSRV.GetAddressOf());
		break;
	default:
		Assert(false, ASSERT_MSG_SWITCH_DEFAULT);
		break;
	}	
}

void GraphicDeviceDX11::BindSB(UINT slot, StructuredBuffer* SB, const eShaderBindType stageType) const
{
	switch (stageType)
	{
	case eShaderBindType::VS:
		mContext->VSSetShaderResources(slot, 1, SB->mSRV.GetAddressOf());
		break;
	case eShaderBindType::HS:
		mContext->HSSetShaderResources(slot, 1, SB->mSRV.GetAddressOf());
		break;
	case eShaderBindType::DS:
		mContext->DSSetShaderResources(slot, 1, SB->mSRV.GetAddressOf());
		break;
	case eShaderBindType::GS:
		mContext->GSSetShaderResources(slot, 1, SB->mSRV.GetAddressOf());
		break;
	case eShaderBindType::PS:
		mContext->PSSetShaderResources(slot, 1, SB->mSRV.GetAddressOf());
		break;
	case eShaderBindType::CS:
		mContext->CSSetShaderResources(slot, 1, SB->mSRV.GetAddressOf());
		break;
	default:
		Assert(false, ASSERT_MSG_SWITCH_DEFAULT);
		break;
	}
}

void GraphicDeviceDX11::BindSBCS(UINT slot, StructuredBuffer* SB) const
{
	UINT i = UINT_MAX;
	//SB->
	mContext->CSSetUnorderedAccessViews(slot, 1, SB->m_UAV.GetAddressOf(), &i);
}

void GraphicDeviceDX11::PassSB(const eSBType SBType, 
	const UINT dataSize, 
	const UINT stride, 
	const void* const data) const
{
	Assert(eSBType::End != SBType, ASSERT_MSG_INVALID);
	//Assert(data, WCHAR_IS_NULLPTR);

	StructuredBuffer* SB = mSBCollection->GetStructuredBuffer(SBType);

	Assert(SB, ASSERT_MSG_NULL);

	if (SB->mElementSize * SB->mElementCount < dataSize * stride)
	{
		mSBCollection->ReSizeStructuredBuffer(SBType, SB->mSRVType, dataSize, stride, data, mDevice.Get());
		SB = mSBCollection->GetStructuredBuffer(SBType);		
	}

	D3D11_MAPPED_SUBRESOURCE subResource = {};

	mContext->Map(SB->mBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy_s(subResource.pData, SB->mDesc.ByteWidth, data, SB->mDesc.ByteWidth);
	}

	mContext->Unmap(SB->mBuffer.Get(), 0);
}

void GraphicDeviceDX11::BindVS(const Shader* const shader) const
{
	Assert(shader, ASSERT_MSG_NULL);
	Assert(shader->mVS.Get(), ASSERT_MSG_NULL);

	mContext->VSSetShader(shader->mVS.Get(), nullptr, 0);
}

void GraphicDeviceDX11::BindHS(const Shader* const shader) const
{
	Assert(false, ASSERT_MSG_NULL);
	Assert(shader, ASSERT_MSG_NULL);
	Assert(shader->mVS.Get(), ASSERT_MSG_NULL);

	(void)shader;
	Assert(false, ASSERT_MSG_INVALID);
	//mContext->HSSetShader(shader->mVS.Get(), nullptr, 0);
}

void GraphicDeviceDX11::BindGS(const Shader* const shader) const
{
	Assert(false, ASSERT_MSG_NULL);
	Assert(shader, ASSERT_MSG_NULL);
	Assert(shader->mVS.Get(), ASSERT_MSG_NULL);

	(void)shader;
	Assert(false, ASSERT_MSG_INVALID);
	//mContext->GSSetShader(shader->mVS.Get(), nullptr, 0);
}

void GraphicDeviceDX11::BindDS(const Shader* const shader) const
{
	Assert(false, ASSERT_MSG_NULL);
	Assert(shader, ASSERT_MSG_NULL);
	Assert(shader->mVS.Get(), ASSERT_MSG_NULL);

	(void)shader;
	Assert(false, ASSERT_MSG_INVALID);
	//mContext->DSSetShader(shader->mVS.Get(), nullptr, 0);
}

void GraphicDeviceDX11::BindPS(const Shader* const shader) const
{
	Assert(shader, ASSERT_MSG_NULL);
	Assert(shader->mPS.Get(), ASSERT_MSG_NULL);

	mContext->PSSetShader(shader->mPS.Get(), nullptr, 0);
}

void GraphicDeviceDX11::BindCS(const ComputeShader* const computShader) const
{
	Assert(computShader, ASSERT_MSG_NULL);
	Assert(computShader->mCS.Get(), ASSERT_MSG_NULL);

	mContext->CSSetShader(computShader->mCS.Get(), nullptr, 0);	
}

void GraphicDeviceDX11::BindBS(const eBSType BSType)
{
	Assert(eBSType::End != BSType, ASSERT_MSG_INVALID);
	mDxStateData.BS = BSType;
	mContext->OMSetBlendState(mBSCollection->mBStates[static_cast<UINT>(BSType)].Get(), nullptr, 0xffffffff);
}

void GraphicDeviceDX11::BindDS(const eDSType DSType) const
{
	Assert(eDSType::End != DSType, ASSERT_MSG_INVALID);

	mContext->OMSetDepthStencilState(mDSCollection->mDStates[static_cast<UINT>(DSType)].Get(), 0);
}

void GraphicDeviceDX11::BindRS(const eRSType RSType)
{
	Assert(eRSType::End != RSType, ASSERT_MSG_INVALID);
	mDxStateData.RS = RSType;
	mContext->RSSetState(mRSCollection->mRStates[static_cast<UINT>(RSType)].Get());
}

void GraphicDeviceDX11::Draw(const Mesh* const mesh, int index) const
{
	Assert(mesh, ASSERT_MSG_NULL);
	Assert(!mesh->mIndexBuffers.empty(), ASSERT_MSG_NULL);
	Assert(index < mesh->mIndexBuffers.size(), ASSERT_MSG_NULL);

	const UINT INDEX_COUNT = mesh->mIndexBuffers[index].iIdxCount;
	const UINT START_VERTEX_LOCATION = 0;

	mContext->DrawIndexed(INDEX_COUNT, START_VERTEX_LOCATION, 0);
}

void GraphicDeviceDX11::Distpatch(const ComputeShader* const computeShader) const
{
	mContext->Dispatch(computeShader->mGroupX, computeShader->mGroupY, computeShader->mGroupZ);
}

void GraphicDeviceDX11::ClearRenderTarget(ID3D11RenderTargetView* const* const ppRnderTargetView,
	ID3D11DepthStencilView* const depthStencilView,
	const FLOAT (&backgroundColor)[4]) const
{
	if (ppRnderTargetView)
	{
		mContext->ClearRenderTargetView(*ppRnderTargetView, backgroundColor);
	}

	mContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void GraphicDeviceDX11::CopyResource(ID3D11Resource* const dst, ID3D11Resource* const src)
{
	Assert(dst, ASSERT_MSG_NULL);
	Assert(src, ASSERT_MSG_NULL);

	mContext->CopyResource(dst, src);
}

void GraphicDeviceDX11::BindRenderTarget(const UINT renderTargetWidth,
	const UINT renderTargetHeight,
	const UINT renderTargetCount,
	ID3D11RenderTargetView* const* const ppRnderTargetView,
	ID3D11DepthStencilView* const depthStencilView) const
{
	const D3D11_VIEWPORT VIEW_PORT =
	{
		0.0f, 0.0f,
		static_cast<FLOAT>(renderTargetWidth), static_cast<FLOAT>(renderTargetHeight),
		0.0f, 1.0f
	};

	mContext->RSSetViewports(1, &VIEW_PORT);
	mContext->OMSetRenderTargets(renderTargetCount, ppRnderTargetView, depthStencilView);
}

void GraphicDeviceDX11::present()
{
	mSwapChain->Present(0, 0);
}
