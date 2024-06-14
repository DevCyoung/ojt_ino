#pragma once
#include <d3d11.h>

class Camera;
class Shader;
class Mesh;

class DebugRenderer2D
{
public:
	friend class RenderTargetRenderer;
private:
	DebugRenderer2D();
	virtual ~DebugRenderer2D();

public:
	void DrawRect2D(const Vector3& worldPos, 
		const Vector2& rectScale, 
		const Vector3& rotation,
		const float drawTime, 
		const Vector4& outLineColor);

	void DrawRect2D2(const Vector3& wolrdLeftUpPos, 
		const Vector3& worldRightDownPos, 
		const Vector3& rotation,
		const float drawTime, 
		const Vector4& outLineColor);

	void DrawRect2D3(const Matrix& worldMatrix, 
		const float drawTime, const 
		Vector4& outLineColor);

	void DrawFillRect2D(const Vector3& worldPos, 
		const Vector2& rectScale,
		const float drawTime, 
		const Vector4& fillColor);

	void DrawFillRect2D2(const Vector3& wolrdLeftUpPos, 
		const Vector3& worldRightDownPos, 
		const float drawTime,
		const Vector4& fillColor);

	void DrawFillRect2D3(const Matrix& worldMatrix,
		const float drawTime, const
		Vector4& outLineColor);
	

	void DrawCircle2D(const Vector3& worldPos, 
		float raduis,
		const float drawTime, 
		const Vector4& outLineColor);

	void DrawCircle2D2(const Matrix& worldMatrix,
		const float drawTime,
		const Vector4& outLineColor);

	void DrawFillCircle2D(const Vector3& worldPos,
		float raduis,
		const float drawTime, 
		const Vector4& fillColor);

	void DrawLine2D(const Vector3& startPos, 
		const Vector3& endPos,
		const float drawTime, 
		const Vector4& lineColor);

	void DrawLine2D2(const Vector3& startPos,
		const Vector2& direction,
		const float length,
		const float drawTime,
		const Vector4& lineColor);

	void DrawGrid2D(const Vector3& worldPos, 
		const Vector2& cellSizeXY, 
		const XMUINT2& tileCountXY,
		const float drawTime, 
		const Vector4& fillColor);

	void DrawCube3D(const Matrix& matrix,
		const float drawTime,
		const Vector4& lineColor = Vector4(1.f, 1.f, 1.f, 1.f));

private:
	enum class eDebugDrawType
	{
		Rect2D,
		FillRect2D,
		Circle2D,
		FillCircle2D,
		Line2D,
		Grid2D,
		Cube,
		End,
	};

	struct tDebugDrawInfo
	{
		eDebugDrawType DebugDrawType;
		Vector3 WorldPos;
		Vector3 Rotation;
		Vector3 Scale;
		Matrix WorldMatrix;
		Vector3 MousePos;
		Vector4 FillColor;
		Vector4 OutLineColor;
		XMUINT2 XYCount;
		float DrawTime;
	};

	struct tDebugMaterial
	{
		Shader* debugShader;
		Mesh* debugMesh;
	};

	void renderFill2D(const tDebugDrawInfo& debugDrawInfo) const;
	void renderGrid2D(const tDebugDrawInfo& debugDrawInfo, const Camera* const camera) const;
	void renderLine2D(const tDebugDrawInfo& debugDrawInfo) const;
	void render3D(const tDebugDrawInfo& debugDrawInfo) const;
	void render(const Camera* const camera) const;
	void flush();


private:	
	tDebugMaterial mDebugMaterial[static_cast<UINT>(eDebugDrawType::End)];
	std::vector<tDebugDrawInfo> mDebugDrawInfos;
};


