#pragma once
#include "Resource.h"
#include "EnumRenderType.h"
#include "StructVertex.h"
#include "StructBuffer.h"

class Shader;
class Texture;

class Material : public Resource
{	
	friend class GraphicDeviceDX11;
public:
	Material();
	Material(const eRenderType renderPriorityType);
	virtual ~Material();
	Material(const Material&) = delete;
	Material& operator=(const Material&) = delete;

public:
	const Texture*  GetTexture(const TEX_PARAM param) const 
	{ 
		Assert(mTextures[param], ASSERT_MSG_NULL); return mTextures[param];
	}
	const Texture* GetTextureOrNull(const TEX_PARAM param) const
	{
		return mTextures[param];
	}

	Texture* GetTexture(const TEX_PARAM param)
	{
		Assert(mTextures[param], ASSERT_MSG_NULL); return mTextures[param];
	}
	Texture* GetTextureOrNull(const TEX_PARAM param)
	{
		return mTextures[param];
	}

	const Shader* GetShader() const { Assert(mShader, ASSERT_MSG_NULL); return mShader; }
	eRenderType GetRenderType() const { return mRenderType; }

	void SetShader(Shader* const shader) { Assert(shader, ASSERT_MSG_NULL); mShader = shader; }
	void SetTexture(const TEX_PARAM param, Texture* const textureOrNull)
	{ 
		mTextures[param] = textureOrNull;
	}

	void SetAnim3D(bool _bTrue) { mData.arrAnimData[0] = (int)_bTrue; }
	void SetBoneCount(int _iBoneCount) { mData.arrAnimData[1] = _iBoneCount; }

	virtual HRESULT Load(const std::wstring& filePath) override final;
	virtual HRESULT Save(const std::wstring& relativePath);

	void UpdateData();

	tMaterial& GetMaterialData() { return mData; }

private:
	Shader*					mShader;	
	tMaterial				mData;
	Texture*				mTextures[TEX_END];
	eRenderType		mRenderType;
};
