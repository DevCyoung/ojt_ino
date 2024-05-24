#include "pch.h"
#include "RSCollection.h"

RSCollection::RSCollection(ID3D11Device* const device)
{
	Assert(device, ASSERT_MSG_NULL);

	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	device->CreateRasterizerState(&rasterizerDesc, mRStates[static_cast<UINT>(eRSType::CullBack)].GetAddressOf());
	Assert(mRStates[static_cast<UINT>(eRSType::CullBack)].Get(), ASSERT_MSG_NULL);

	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
	device->CreateRasterizerState(&rasterizerDesc, mRStates[static_cast<UINT>(eRSType::CullFront)].GetAddressOf());
	Assert(mRStates[static_cast<UINT>(eRSType::CullFront)].Get(), ASSERT_MSG_NULL);

	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	device->CreateRasterizerState(&rasterizerDesc, mRStates[static_cast<UINT>(eRSType::CullNone)].GetAddressOf());
	Assert(mRStates[static_cast<UINT>(eRSType::CullNone)].Get(), ASSERT_MSG_NULL);

	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	device->CreateRasterizerState(&rasterizerDesc, mRStates[static_cast<UINT>(eRSType::WireframeNone)].GetAddressOf());
	Assert(mRStates[static_cast<UINT>(eRSType::WireframeNone)].Get(), ASSERT_MSG_NULL);
}

RSCollection::~RSCollection()
{
}
