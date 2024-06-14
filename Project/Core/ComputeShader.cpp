#include "pch.h"
#include "ComputeShader.h"
#include "PathManager.h"
#include "StringHelper.h"
#include "Engine.h"
#include "GraphicDeviceDx11.h"
#include "EnumResource.h"

ComputeShader::ComputeShader(const std::wstring& CSRelativePath, const std::wstring& CSFunName)
	: Resource(eResourceType::ComputeShader)
	, mGroupX(64)
	, mGroupY(64)
	, mGroupZ(1)
	, m_iGroupPerThreadX(1)
	, m_iGroupPerThreadY(1)
	, m_iGroupPerThreadZ(1)
{
	const std::wstring& SHADER_FULL_PATH = PathManager::GetInstance()->GetResourcePath() + CSRelativePath;
	const std::string& FUN_NAME = StringHelper::WStrToStr(CSFunName);
	const std::string& VERSION_NAME = StringHelper::WStrToStr(L"cs_5_0");

	Microsoft::WRL::ComPtr<ID3DBlob> errBlob;

	// Shader Compile	
	if (FAILED(D3DCompileFromFile(SHADER_FULL_PATH.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, FUN_NAME.c_str(), VERSION_NAME.c_str(), 0, 0, mCSBlob.GetAddressOf(), errBlob.GetAddressOf())))
	{
		MessageBoxA(nullptr, (const char*)errBlob->GetBufferPointer()
			, "Compute Shader Compile Failed!!", MB_OK);
	}

	// 컴파일된 객체로 Shader 를 만든다.
	gGraphicDevice->UnSafe_GetDevice()->CreateComputeShader(mCSBlob->GetBufferPointer(), 
		mCSBlob->GetBufferSize() , nullptr, mCS.GetAddressOf());
}

ComputeShader::~ComputeShader()
{
}

void ComputeShader::Execute()
{
	//UpdateData();

	//static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	//pCB->SetData(&m_Const);
	//pCB->UpdateData_CS();
	//gGraphicDevice->BindCS
	//CONTEXT->CSSetShader(m_CS.Get(), nullptr, 0);
	//CONTEXT->Dispatch(m_iGroupX, m_iGroupY, m_iGroupZ);

	//Clear();
}

HRESULT ComputeShader::Load(const std::wstring& filePath)
{
	(void)filePath;
	Assert(false, ASSERT_MSG_INVALID);

	return E_NOTIMPL;
}
