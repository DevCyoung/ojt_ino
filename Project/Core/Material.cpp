#include "pch.h"
#include "Material.h"
#include "Engine.h"
#include "GraphicDeviceDx11.h"
#include "ConstantBuffer.h"
#include "EnumShaderBindStage.h"
#include "Shader.h"
#include "EnumResource.h"
#include "PathManager.h"
//#include <Helper\FileHelper.h>	
#include "Texture.h"
#include "ResourceManager.h"
Material::Material()
	: Resource(eResourceType::Material)
	, mShader(nullptr)
	, mTextures{ 0, }
	, mRenderType(eRenderType::Opqaue)
{
}
Material::Material(const eRenderType renderPriorityType)
	: Resource(eResourceType::Material)
	, mShader(nullptr)
	, mTextures{0,}
	, mRenderType(renderPriorityType)
{
}

Material::~Material()
{
}

HRESULT Material::Load(const std::wstring& filePath)
{
	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"rb");

	//eRenderPriorityType
	fread(&mRenderType, sizeof(UINT), 1, file);

	std::wstring shaderPath;
	LoadWString(&shaderPath, file);

	if (!shaderPath.empty())
	{
		mShader = gResourceManager->FindAndLoad<Shader>(shaderPath);
	}	

	fread(&mData, sizeof(tMaterial), 1, file);

	// Texture
	for (UINT i = 0; i < (UINT)TEX_PARAM::TEX_END; ++i)
	{
		std::wstring path;

		LoadWString(&path, file);

		if (path.empty())
		{
			continue;
		}

		Texture* texture = gResourceManager->FindAndLoadOrNull<Texture>(path);
		mTextures[i] = texture;
	}

	fclose(file);
	return E_NOTIMPL;
}

HRESULT Material::Save(const std::wstring& relativePath)
{
	std::wstring filePath = gPathManager->GetResourcePath() + relativePath;

	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"wb");

	if (nullptr == file)
	{
		return S_OK;
	}

	//eRenderPriorityType
	fwrite(&mRenderType, sizeof(UINT), 1, file);
	
	// Shader	
	SaveWString(mShader->GetRelativePathorName(), file);

	// Constant
	fwrite(&mData, sizeof(tMaterial), 1, file);

	// Texture
	for (UINT i = 0; i < (UINT)TEX_PARAM::TEX_END; ++i)
	{
		if (mTextures[i] != nullptr)
		{
			SaveWString(mTextures[i]->GetRelativePathorName(), file);
		}		
		else
		{
			SaveWString(L"", file);
		}
	}

	fclose(file);
	return S_OK;
}

void Material::UpdateData()
{
	gGraphicDevice->BindIA(mShader);
	gGraphicDevice->BindVS(mShader);
	//gGraphicDevice->BindHS(mShader);
	//gGraphicDevice->BindGS(mShader);
	//gGraphicDevice->BindDS(mShader);
	gGraphicDevice->BindPS(mShader);

	gGraphicDevice->BindRS(mShader->GetRSType());
	gGraphicDevice->BindDS(mShader->GetDSType());
	gGraphicDevice->BindBS(mShader->GetBSType());

		
	// Texture Update
	for (UINT i = 0; i < TEX_END; ++i)
	{
		if (nullptr == mTextures[i])
		{
			mData.arrBTex[i] = 0;
			continue;
		}

		else
		{
			mData.arrBTex[i] = 1;
			gGraphicDevice->BindSRV(eShaderBindType::VS, i, mTextures[i]);
			gGraphicDevice->BindSRV(eShaderBindType::HS, i, mTextures[i]);
			gGraphicDevice->BindSRV(eShaderBindType::GS, i, mTextures[i]);
			gGraphicDevice->BindSRV(eShaderBindType::DS, i, mTextures[i]);
			gGraphicDevice->BindSRV(eShaderBindType::PS, i, mTextures[i]);
		}
	}


	gGraphicDevice->PassCB(eCBType::Material, sizeof(mData), &mData);
	gGraphicDevice->BindCB(eCBType::Material, eShaderBindType::VS);
	gGraphicDevice->BindCB(eCBType::Material, eShaderBindType::HS);
	gGraphicDevice->BindCB(eCBType::Material, eShaderBindType::GS);
	gGraphicDevice->BindCB(eCBType::Material, eShaderBindType::DS);
	gGraphicDevice->BindCB(eCBType::Material, eShaderBindType::PS);
}
