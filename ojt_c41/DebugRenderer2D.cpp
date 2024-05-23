#include "pch.h"
#include "DebugRenderer2D.h"
#include "Engine.h"
#include "GraphicDeviceDx11.h"
#include "CBCollection.h"
#include "StructBuffer.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "ResourceManager.h"
#include "EngineMath.h"

DebugRenderer2D::DebugRenderer2D()
	: mDebugMaterial{ 0, }
{
	{
		tDebugMaterial debugMaterial = {};
		debugMaterial.debugMesh = gResourceManager->Find<Mesh>(L"LineRect2D");
		debugMaterial.debugShader = gResourceManager->Find<Shader>(L"LineDebug2D");
		mDebugMaterial[static_cast<UINT>(eDebugDrawType::Rect2D)] = debugMaterial;
	}

	{
		tDebugMaterial debugMaterial = {};
		debugMaterial.debugMesh = gResourceManager->Find<Mesh>(L"FillRect2D");
		debugMaterial.debugShader = gResourceManager->Find<Shader>(L"FillDebug2D");
		mDebugMaterial[static_cast<UINT>(eDebugDrawType::FillRect2D)] = debugMaterial;
	}

	{
		tDebugMaterial debugMaterial = {};
		debugMaterial.debugMesh = gResourceManager->Find<Mesh>(L"LineCircle2D");
		debugMaterial.debugShader = gResourceManager->Find<Shader>(L"LineDebug2D");
		mDebugMaterial[static_cast<UINT>(eDebugDrawType::Circle2D)] = debugMaterial;
	}

	{
		tDebugMaterial debugMaterial = {};
		debugMaterial.debugMesh = gResourceManager->Find<Mesh>(L"FillCircle2D");
		debugMaterial.debugShader = gResourceManager->Find<Shader>(L"FillDebug2D");
		mDebugMaterial[static_cast<UINT>(eDebugDrawType::FillCircle2D)] = debugMaterial;
	}

	{
		tDebugMaterial debugMaterial = {};
		debugMaterial.debugMesh = gResourceManager->Find<Mesh>(L"Line");
		debugMaterial.debugShader = gResourceManager->Find<Shader>(L"LineDebug2D");
		mDebugMaterial[static_cast<UINT>(eDebugDrawType::Line2D)] = debugMaterial;
	}

	{
		tDebugMaterial debugMaterial = {};
		debugMaterial.debugMesh = gResourceManager->Find<Mesh>(L"FillRect2D");
		debugMaterial.debugShader = gResourceManager->Find<Shader>(L"DebugGrid2D");
		mDebugMaterial[static_cast<UINT>(eDebugDrawType::Grid2D)] = debugMaterial;
	}

	{
		tDebugMaterial debugMaterial = {};
		debugMaterial.debugMesh = gResourceManager->Find<Mesh>(L"DebugCube");
		debugMaterial.debugShader = gResourceManager->Find<Shader>(L"Std3DCubeDebug");
		mDebugMaterial[static_cast<UINT>(eDebugDrawType::Cube)] = debugMaterial;
	}
}

DebugRenderer2D::~DebugRenderer2D()
{
}

void DebugRenderer2D::DrawRect2D(const Vector3& worldPos,
	const Vector2& rectScale,
	const Vector3& rotation,
	const float drawTime, 
	const Vector4& outLineColor)
{
	tDebugDrawInfo drawInfo = {};

	drawInfo.DebugDrawType = eDebugDrawType::Rect2D;
	drawInfo.WorldPos = worldPos;
	drawInfo.Rotation = rotation;
	drawInfo.Scale = Vector3(rectScale.x, rectScale.y, 1.f);

	drawInfo.WorldMatrix =
		Transform::CreateWorldMatrix(drawInfo.WorldPos, drawInfo.Rotation, drawInfo.Scale);

	drawInfo.DrawTime = drawTime;
	drawInfo.OutLineColor = outLineColor;

	mDebugDrawInfos.push_back(drawInfo);
}

void DebugRenderer2D::DrawRect2D2(const Vector3& wolrdLeftUpPos,
	const Vector3& worldRightDownPos, 
	const Vector3& rotation,
	const float drawTime, 
	const Vector4& outLineColor)
{
	const Vector3 WORLD_POS = (wolrdLeftUpPos + worldRightDownPos) * 0.5f;
	const Vector3 RECT_SCALE_3D = worldRightDownPos - wolrdLeftUpPos;

	DrawRect2D(WORLD_POS, Vector2(RECT_SCALE_3D.x, RECT_SCALE_3D.y), rotation, drawTime, outLineColor);
}

void DebugRenderer2D::DrawRect2D3(const Matrix& worldMatrix, const float drawTime, const Vector4& outLineColor)
{
	tDebugDrawInfo drawInfo = {};

	drawInfo.DebugDrawType = eDebugDrawType::Rect2D;

	drawInfo.WorldMatrix = worldMatrix;
	drawInfo.DrawTime = drawTime;
	drawInfo.OutLineColor = outLineColor;

	mDebugDrawInfos.push_back(drawInfo);
}

void DebugRenderer2D::DrawFillRect2D(const Vector3& worldPos,
	const Vector2& rectScale,
	const float drawTime, const Vector4& fillColor)
{
	tDebugDrawInfo drawInfo = {};

	drawInfo.DebugDrawType = eDebugDrawType::FillRect2D;

	drawInfo.WorldPos = worldPos;
	drawInfo.Rotation = Vector3::Zero;
	drawInfo.Scale = Vector3(rectScale.x, rectScale.y, 1.f);

	drawInfo.WorldMatrix =
		Transform::CreateWorldMatrix(drawInfo.WorldPos, drawInfo.Rotation, drawInfo.Scale);


	drawInfo.DrawTime = drawTime;
	drawInfo.FillColor = fillColor;

	mDebugDrawInfos.push_back(drawInfo);
}

void DebugRenderer2D::DrawFillRect2D2(const Vector3& wolrdLeftUpPos,
	const Vector3& worldRightDownPos,
	const float drawTime, const Vector4& fillColor)
{
	const Vector3 WORLD_POS = (wolrdLeftUpPos + worldRightDownPos) * 0.5f;
	const Vector3 RECT_SCALE_3D = worldRightDownPos - wolrdLeftUpPos;

	DrawFillRect2D(WORLD_POS, Vector2(RECT_SCALE_3D.x, RECT_SCALE_3D.y), drawTime, fillColor);
}

void DebugRenderer2D::DrawFillRect2D3(const Matrix& worldMatrix,
	const float drawTime, const
	Vector4& outLineColor)
{
	tDebugDrawInfo drawInfo = {};

	drawInfo.DebugDrawType = eDebugDrawType::FillRect2D;

	drawInfo.WorldMatrix = worldMatrix;
	drawInfo.DrawTime = drawTime;
	drawInfo.FillColor = outLineColor;

	mDebugDrawInfos.push_back(drawInfo);
}

void DebugRenderer2D::DrawCircle2D(const Vector3& worldPos,
	float raduis,
	const float drawTime, const Vector4& outLineColor)
{
	tDebugDrawInfo drawInfo = {};

	drawInfo.DebugDrawType = eDebugDrawType::Circle2D;

	drawInfo.WorldPos = worldPos;
	drawInfo.Rotation = Vector3::Zero;
	drawInfo.Scale = Vector3(raduis * 2.f, raduis * 2.f, 1.f);

	drawInfo.WorldMatrix =
		Transform::CreateWorldMatrix(drawInfo.WorldPos, drawInfo.Rotation, drawInfo.Scale);

	drawInfo.DrawTime = drawTime;
	drawInfo.OutLineColor = outLineColor;

	mDebugDrawInfos.push_back(drawInfo);
}


void DebugRenderer2D::DrawCircle2D2(const Matrix& worldMatrix,
	const float drawTime,
	const Vector4& outLineColor)
{
	tDebugDrawInfo drawInfo = {};

	drawInfo.DebugDrawType = eDebugDrawType::Circle2D;
	drawInfo.WorldMatrix = worldMatrix;

	drawInfo.DrawTime = drawTime;
	drawInfo.OutLineColor = outLineColor;

	mDebugDrawInfos.push_back(drawInfo);
}

void DebugRenderer2D::DrawFillCircle2D(const Vector3& worldPos,
	float raduis,
	const float drawTime, const Vector4& fillColor)
{
	tDebugDrawInfo drawInfo = {};

	drawInfo.DebugDrawType = eDebugDrawType::FillCircle2D;

	drawInfo.WorldPos = worldPos;
	drawInfo.Rotation = Vector3::Zero;
	drawInfo.Scale = Vector3(raduis * 2.f, raduis * 2.f, 1.f);

	drawInfo.WorldMatrix =
		Transform::CreateWorldMatrix(drawInfo.WorldPos, drawInfo.Rotation, drawInfo.Scale);

	drawInfo.DrawTime = drawTime;
	drawInfo.FillColor = fillColor;

	mDebugDrawInfos.push_back(drawInfo);
}

void DebugRenderer2D::DrawLine2D(const Vector3& startPos,
	const Vector3& endPos,
	const float drawTime, const Vector4& lineColor)
{
	tDebugDrawInfo drawInfo = {};

	const Vector3 pivot = (startPos + endPos) * 0.5f;
	const Vector3 dist = endPos - startPos;
	const Vector3 rotation = Vector3(0.f, 0.f, Rad2Deg(atan2f(dist.y, dist.x)));
	const Vector3 scale = Vector3(dist.Length(), 1.f, 1.f);

	drawInfo.DebugDrawType = eDebugDrawType::Line2D;

	drawInfo.WorldPos = pivot;
	drawInfo.Rotation = rotation;
	drawInfo.Scale = scale;

	drawInfo.WorldMatrix =
		Transform::CreateWorldMatrix(drawInfo.WorldPos, drawInfo.Rotation, drawInfo.Scale);

	drawInfo.DrawTime = drawTime;
	drawInfo.OutLineColor = lineColor;

	mDebugDrawInfos.push_back(drawInfo);
}

void DebugRenderer2D::DrawLine2D2(const Vector3& startPos,
	const Vector2& direction,
	const float length,
	const float drawTime, const Vector4& lineColor)
{
	Vector2 directionNorm = direction;
	directionNorm.Normalize();
	const Vector3 END_POS = startPos + (directionNorm * length);

	DrawLine2D(startPos, END_POS, drawTime, lineColor);
}

void DebugRenderer2D::DrawGrid2D(const Vector3& worldPos,
	const Vector2& cellSizeXY, const XMUINT2& tileCountXY,
	const float drawTime, const Vector4& fillColor)
{
	tDebugDrawInfo drawInfo = {};

	drawInfo.DebugDrawType = eDebugDrawType::Grid2D;

	drawInfo.WorldPos = worldPos;
	drawInfo.Rotation = Vector3::Zero;
	drawInfo.Scale = Vector3(cellSizeXY.x * tileCountXY.x, cellSizeXY.y * tileCountXY.y, 1.f);

	drawInfo.WorldMatrix =
		Transform::CreateWorldMatrix(drawInfo.WorldPos, drawInfo.Rotation, drawInfo.Scale);

	drawInfo.DrawTime = drawTime;
	drawInfo.XYCount = tileCountXY;
	drawInfo.FillColor = fillColor;

	mDebugDrawInfos.push_back(drawInfo);
}

void DebugRenderer2D::DrawCube3D(const Matrix& matrix, 
	const float drawTime, 
	const Vector4& lineColor)
{
	tDebugDrawInfo drawInfo = {};

	drawInfo.DebugDrawType = eDebugDrawType::Cube;

	drawInfo.DrawTime = drawTime;
	drawInfo.WorldMatrix = matrix;
	drawInfo.OutLineColor = lineColor;

	mDebugDrawInfos.push_back(drawInfo);
}


void DebugRenderer2D::render(const Camera* const camera) const
{
	Assert(camera, ASSERT_MSG_NULL);

	if (mDebugDrawInfos.empty())
	{
		return;
	}

	tCBTransform CBTransform = {};

	CBTransform.View = camera->GetView();
	CBTransform.Proj = camera->GetProjection();

	for (const tDebugDrawInfo& DRAW_INFO : mDebugDrawInfos)
	{
		const tDebugMaterial& debugMat =
			mDebugMaterial[static_cast<UINT>(DRAW_INFO.DebugDrawType)];

		const Mesh* const P_MESH = debugMat.debugMesh;
		const Shader* const P_SHADER = debugMat.debugShader;

		Assert(P_MESH, ASSERT_MSG_NULL);
		Assert(P_SHADER, ASSERT_MSG_NULL);

		const Vector3& SCALE		= Vector3(10, 10, 10.f);
		const Matrix& SCALE_MATRIX  = Matrix::CreateScale(SCALE);
		CBTransform.World			= SCALE_MATRIX * DRAW_INFO.WorldMatrix;
		//CBTransform.View			= camera->GetView();
		//CBTransform.Proj			= camera->GetProjection();
		CBTransform.WV				= CBTransform.World * CBTransform.View;
		CBTransform.WVP				= CBTransform.WV * CBTransform.Proj;	

		gGraphicDevice->PassCB(eCBType::Transform, sizeof(CBTransform), &CBTransform);
		gGraphicDevice->BindCB(eCBType::Transform, eShaderBindType::VS);

		gGraphicDevice->BindMesh(P_MESH, 0);

		gGraphicDevice->BindIA(P_SHADER);
		gGraphicDevice->BindPS(P_SHADER);
		gGraphicDevice->BindVS(P_SHADER);

		gGraphicDevice->BindRS(P_SHADER->GetRSType());
		gGraphicDevice->BindBS(P_SHADER->GetBSType());
		gGraphicDevice->BindDS(P_SHADER->GetDSType());

		switch (DRAW_INFO.DebugDrawType)
		{
		case eDebugDrawType::Rect2D:
			renderLine2D(DRAW_INFO);
			break;

		case eDebugDrawType::FillRect2D:
			renderFill2D(DRAW_INFO);
			break;

		case eDebugDrawType::Circle2D:
			renderLine2D(DRAW_INFO);
			break;

		case eDebugDrawType::FillCircle2D:
			renderFill2D(DRAW_INFO);
			break;

		case eDebugDrawType::Line2D:
			renderLine2D(DRAW_INFO);
			break;

		case eDebugDrawType::Grid2D:			
			renderGrid2D(DRAW_INFO, camera);
			break;

		case eDebugDrawType::Cube:
			render3D(DRAW_INFO);
			break;

		default:
			Assert(false, ASSERT_MSG_INVALID);
			break;
		}

		gGraphicDevice->Draw(P_MESH, 0);
	}
}

void DebugRenderer2D::renderLine2D(const tDebugDrawInfo& drawInfo) const
{
	tCBDebugInfo CBRectInfo = {};
	{
		CBRectInfo.Color_1 = drawInfo.OutLineColor;

		gGraphicDevice->PassCB(eCBType::DebugInfo, sizeof(CBRectInfo), &CBRectInfo);
		gGraphicDevice->BindCB(eCBType::DebugInfo, eShaderBindType::PS);
	}
}

void DebugRenderer2D::render3D(const tDebugDrawInfo& drawInfo) const
{
	tCBDebugInfo CBRectInfo = {};
	{
		CBRectInfo.Color_1 = drawInfo.OutLineColor;

		gGraphicDevice->PassCB(eCBType::DebugInfo, sizeof(CBRectInfo), &CBRectInfo);
		gGraphicDevice->BindCB(eCBType::DebugInfo, eShaderBindType::PS);
	}
}

void DebugRenderer2D::renderFill2D(const tDebugDrawInfo& drawInfo) const
{
	tCBDebugInfo CBRectInfo = {};
	{
		CBRectInfo.Color_1 = drawInfo.FillColor;

		gGraphicDevice->PassCB(eCBType::DebugInfo, sizeof(CBRectInfo), &CBRectInfo);
		gGraphicDevice->BindCB(eCBType::DebugInfo, eShaderBindType::PS);
	}
}

void DebugRenderer2D::flush()
{
	mDebugDrawInfos.erase(std::remove_if(mDebugDrawInfos.begin(), mDebugDrawInfos.end(),
		[](tDebugDrawInfo& drawInfo) 
		{ 
			drawInfo.DrawTime -= gDeltaTime;
			return drawInfo.DrawTime <= 0.f;
		}), mDebugDrawInfos.end());
}

void DebugRenderer2D::renderGrid2D(const tDebugDrawInfo& debugDrawInfo, const Camera* const camera) const
{
	tCBDebugInfo CBGridInfo = {};
	{
		CBGridInfo.MouseWorldPos = debugDrawInfo.MousePos;
		CBGridInfo.Scale = Vector2(debugDrawInfo.Scale.x, debugDrawInfo.Scale.y);
		CBGridInfo.XYCount[0] = static_cast<int>(debugDrawInfo.XYCount.x);
		CBGridInfo.XYCount[1] = static_cast<int>(debugDrawInfo.XYCount.y);

		CBGridInfo.Color_0 = debugDrawInfo.OutLineColor;
		CBGridInfo.Color_1 = debugDrawInfo.FillColor;

		CBGridInfo.Vector4_0 = Vector4(camera->GetCameraSize());

		gGraphicDevice->PassCB(eCBType::DebugInfo, sizeof(CBGridInfo), &CBGridInfo);
		gGraphicDevice->BindCB(eCBType::DebugInfo, eShaderBindType::PS);
	}
}
