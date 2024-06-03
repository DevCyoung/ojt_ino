#pragma once
#include "Component.h"

class Mesh;
class Material;
class Camera;

REGISTER_COMPONENT_TYPE(RenderComponent);

class RenderComponent : public Component
{
	friend class RenderTargetRenderer;

public:
	RenderComponent();
	virtual ~RenderComponent();
	RenderComponent(const eComponentType componentType);
	//RenderComponent(const RenderComponent&);
	RenderComponent& operator=(const RenderComponent&) = delete;

public:
	Mesh* GetMesh() const { Assert(mMesh, ASSERT_MSG_NULL); return mMesh; }
	Material* GetMaterial(const UINT idx) const 
	{ 
		Assert(mMaterials.size() > idx, ASSERT_MSG_NULL);
		Assert(mMaterials[idx], ASSERT_MSG_NULL); 
		return mMaterials[idx]; 
	}
	bool IsVisible() const { return mbVisible; }

	void SetMesh(Mesh* const mesh);
	void SetMaterial(Material* const material, const UINT idx)
	{		
		Assert(material, ASSERT_MSG_NULL);
		mMaterials[idx] = material;
	}
	//void SetVisible(const bool visible) { mbVisible = visible; }

	void TurnOnVisiblelity() { mbVisible = true; }
	void TurnOffVisiblelity() { mbVisible = false; }	

	UINT GetMaterialCount() 
	{ 
		return static_cast<UINT>(mMaterials.size()); 
	}
	
	virtual void Save(FILE* const file) override;
	virtual void Load(FILE* const file) override;
	CLONE(RenderComponent)

protected:
	virtual void update() override;

private:
	virtual void initialize() override;	
	virtual void lateUpdate() override;
	virtual void render(const Matrix& viewMatrix, const Matrix& projectionMatrix)
	{		
		(void)viewMatrix;
		(void)projectionMatrix;
		//Assert(false, ASSERT_MSG_INVALID);
	};

protected:	
	bool mbVisible;
	Mesh* mMesh; 
	std::vector<Material*> mMaterials;
};
