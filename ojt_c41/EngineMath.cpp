#include "pch.h"
#include "Engine.h"
#include "EngineMath.h"
#include "Camera.h"
#include "Transform.h"
#include "InputManager.h"
#include "GameObject.h"

/*
* 게임 에서 사용하는 UI 좌표계는 윈도우 좌표계와 동일하게 사용합니다.
* 윈도우 좌표계는 왼쪽 상단이 (0, 0)이고 오른쪽 하단이 (width, height)입니다.
*/
#include "WindowManager.h"
namespace helper
{
	Vector2 RenderTargetToWorldUI(const Vector2& renderTargetMousePos, const Vector2& renderTargetSize)
	{
		return Vector2(renderTargetMousePos.x - renderTargetSize.x / 2, -renderTargetMousePos.y + renderTargetSize.y / 2);
	}

	Vector3 ScreenMouseToWorld3D(const Vector3& screenMousePos,
		const Vector2& screenSize, const Camera* const camera)
	{
		Viewport viewport = {};
		viewport.width = screenSize.x;
		viewport.height = screenSize.y;
		viewport.x = 0;
		viewport.y = 0;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		const Vector3& mouseWorldPos3D =
			viewport.Unproject(screenMousePos, camera->GetProjection(), camera->GetView(), Matrix::Identity);

		return mouseWorldPos3D;
	}

	Vector3 WindowScreenMouseToWorld3D(const Camera* const camera)
	{
		//camera가속한 Scene에서 마우스 포지션을 가져오는 함수정의?
		//const Vector2& WINDOW_MOUSE_2D_POS = gInput->GetWindowMousePos();
		const Vector2& WINDOW_MOUSE_2D_POS = WindowManager::GetInstance()->GetWindowMousePos();
		const Vector3& WINDOW_MOUSE_3D_POS = Vector3(WINDOW_MOUSE_2D_POS.x, WINDOW_MOUSE_2D_POS.y, 0.0f);

		//FIXME
		//const Vector2& WINDOW_SCREEN_SIZE = gEngine->GetWindowScreenSize();
		const Vector2& WINDOW_SCREEN_SIZE = camera->GetRenderTargetSize();

		return ScreenMouseToWorld3D(WINDOW_MOUSE_3D_POS, WINDOW_SCREEN_SIZE, camera);
	}

	XMINT2 GridIndex(const Vector3& selectWorldPos,
		const Vector2& cellSize, const XMUINT2& tileCount)
	{
		Vector2 selectPointToCenterPos = Vector2(selectWorldPos.x, -selectWorldPos.y);

		selectPointToCenterPos.x += (cellSize.x * tileCount.x) / 2.f;
		selectPointToCenterPos.y += (cellSize.y * tileCount.y) / 2.f;

		int x = static_cast<int>(selectPointToCenterPos.x / cellSize.x);
		int y = static_cast<int>(selectPointToCenterPos.y / cellSize.y);

		if (selectPointToCenterPos.x < 0.f)
		{
			x = -1;
		}
		if (selectPointToCenterPos.y < 0.f)
		{
			y = -1;
		}

		return XMINT2(x, y);
	}

	Vector3 GridIndexToWorldPosition(const XMINT2& gridIndex,
		const Vector2& cellSize, const XMUINT2& tileCount)
	{
		Vector3 rectPos = Vector3::Zero;

		rectPos.x -= (cellSize.x * tileCount.x) / 2;
		rectPos.y += (cellSize.y * tileCount.y) / 2;

		rectPos.x += (gridIndex.x * cellSize.x) + (cellSize.x / 2);
		rectPos.y -= (gridIndex.y * cellSize.y) + (cellSize.y / 2);

		return rectPos;
	}



	bool IsInGrid(const XMINT2& gridIndex, const XMUINT2& tileCount)
	{
		XMINT2 countXY = {};

		countXY.x = static_cast<int>(tileCount.x);
		countXY.y = static_cast<int>(tileCount.y);

		return 0 <= gridIndex.x && gridIndex.x < countXY.x &&
			0 <= gridIndex.y && gridIndex.y < countXY.y;
	}
}

namespace helper::math
{
	void INT2MinAndMax(const XMINT2& a, const XMINT2& b, XMINT2* const outMin, XMINT2* const outMax)
	{
		Assert(outMin, ASSERT_MSG_NULL);
		Assert(outMax, ASSERT_MSG_NULL);

		outMin->x = min(a.x, b.x);
		outMin->y = min(a.y, b.y);

		outMax->x = max(a.x, b.x);
		outMax->y = max(a.y, b.y);
	}

	void floatMinAndMax(const float a, const float b, float* const outMin, float* const outMax)
	{
		Assert(outMin, ASSERT_MSG_NULL);
		Assert(outMax, ASSERT_MSG_NULL);

		*outMin = min(a, b);
		*outMax = max(a, b);
	}

	void Vector2MinAndMax(const Vector2& a, const Vector2& b, Vector2* const outMin, Vector2* const outMax)
	{
		Assert(outMin, ASSERT_MSG_NULL);
		Assert(outMax, ASSERT_MSG_NULL);

		outMin->x = min(a.x, b.x);
		outMin->y = min(a.y, b.y);

		outMax->x = max(a.x, b.x);
		outMax->y = max(a.y, b.y);
	}

	void Vector3MinAndMax(const Vector3& a, const Vector3& b, Vector3* const outMin, Vector3* const outMax)
	{
		Assert(outMin, ASSERT_MSG_NULL);
		Assert(outMax, ASSERT_MSG_NULL);

		outMin->x = min(a.x, b.x);
		outMin->y = min(a.y, b.y);
		outMin->z = min(a.z, b.z);

		outMax->x = max(a.x, b.x);
		outMax->y = max(a.y, b.y);
		outMax->z = max(a.z, b.z);
	}

	float LerpCosBtwZeroAndOne(const float x)
	{
		return (cos(x) + 1.f) / 2.f;
	}

	float LerpSinBtwZeroAndOne(const float x)
	{
		return (sin(x) + 1.f) / 2.f;
	}

	bool LineAndLineCollision(const Vector3& s1, const Vector3& e1,
		const Vector3& s2, const Vector3& e2, Vector2* const outInter)
	{
		Assert(outInter, ASSERT_MSG_NULL);

		return _LineAndLineCollision(s1.x, s1.y, e1.x, e1.y, s2.x, s2.y, e2.x, e2.y, &outInter->x, &outInter->y);
	}


	bool LineAndLineCollision(const Vector2& s1, const Vector2& e1,
		const Vector2& s2, const Vector2& e2,
		Vector2* const outInter)
	{
		Assert(outInter, ASSERT_MSG_NULL);

		return _LineAndLineCollision(s1.x, s1.y, e1.x, e1.y, s2.x, s2.y, e2.x, e2.y, &outInter->x, &outInter->y);
	}

	bool BoxAndLineCollision(const Matrix& box, const Vector3& linePos,
		const Vector3& lineS, const Vector3& lineE, Vector2* const outInter)
	{
		Vector3 vertexPosArray[4] =
		{
			Vector3(-0.5f, 0.5f, 0.f),
			Vector3(0.5f, 0.5f, 0.f),
			Vector3(0.5f, -0.5f, 0.f),
			Vector3(-0.5f, -0.5f, 0.f),
		};

		Vector3 tp = linePos;
		Vector3 sideDistArray[4] = {};
		Matrix boxWorldMatrix = box;

		bool result = false;

		for (UINT i = 0; i < 4; i++)
		{
			//box 0 ---- 1
			Vector3 s1 = XMVector3TransformCoord(vertexPosArray[i], boxWorldMatrix);
			Vector3 e1 = XMVector3TransformCoord(vertexPosArray[(i + 1) % 4], boxWorldMatrix);

			Vector3 s2 = lineS + tp;
			Vector3 e2 = lineE + tp;

			if (helper::math::LineAndLineCollision(s1, e1, s2, e2, outInter))
			{
				result = true;
				break;
			}
		}

		return result;
	}

	bool _LineAndLineCollision(float x1, float y1,
		float x2, float y2,
		float x3, float y3,
		float x4, float y4,
		float* outInterX, float* outInterY)
	{
		// calculate the distance to intersection point
		float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
		float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

		// if uA and uB are between 0-1, lines are colliding
		if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {

			// optionally, draw a circle where the lines meet
			float intersectionX = x1 + (uA * (x2 - x1));
			float intersectionY = y1 + (uA * (y2 - y1));

			*outInterX = intersectionX;
			*outInterY = intersectionY;

			return true;
		}
		return false;
	}


	Vector2 GetBoxAndBoxInterBoxSize(const Matrix& box1, const Matrix& box2)
	{
		Matrix mat1 = box1;
		Matrix mat2 = box2;

		//90도 회전되있을시 다시 바꾼다.
		if (mat1._11 < 0.f)
		{
			mat1._11 *= -1.f;
		}

		if (mat2._11 < 0.f)
		{
			mat1._11 *= -1.f;
		}

		Vector3 leftTop = Vector3(-0.5f, 0.5f, 0.f);
		Vector3 rightBottom = Vector3(0.5f, -0.5f, 0.f);

		Vector3 lt1 = XMVector3TransformCoord(leftTop, mat1);
		Vector3 rb1 = XMVector3TransformCoord(rightBottom, mat1);

		Vector3 lt2 = XMVector3TransformCoord(leftTop, mat2);
		Vector3 rb2 = XMVector3TransformCoord(rightBottom, mat2);

		Vector3 ltInter = Vector3::Zero;
		Vector3 rbInter = Vector3::Zero;

		ltInter.x = max(lt1.x, lt2.x);
		ltInter.y = min(lt1.y, lt2.y);

		rbInter.x = min(rb1.x, rb2.x);
		rbInter.y = max(rb1.y, rb2.y);

		Vector2 size = Vector2::Zero;

		size.x = abs(rbInter.x - ltInter.x);
		size.y = abs(ltInter.y - rbInter.y);

		return size;
	}

	Vector3 GetDirection3D(const GameObject* const origin, const GameObject* const dest)
	{
		Vector3 pos1 = origin->GetComponent<Transform>()->GetWorldMatrix().Translation();
		Vector3 pos2 = dest->GetComponent<Transform>()->GetWorldMatrix().Translation();

		Vector3 dir = pos2 - pos1;
		dir.Normalize();

		return dir;
	}

	Vector2 GetDirection2D(const GameObject* const origin, const GameObject* const dest)
	{
		Vector3 pos1 = origin->GetComponent<Transform>()->GetWorldMatrix().Translation();
		Vector3 pos2 = dest->GetComponent<Transform>()->GetWorldMatrix().Translation();

		pos1.z = 0.f;
		pos2.z = 0.f;

		Vector3 dir = pos2 - pos1;
		dir.Normalize();

		return Vector2(dir.x, dir.y);
	}

	Vector2 GetDirection2D(Vector2 origin, Vector2 dest)
	{
		Vector2 direction = dest - origin;

		direction.Normalize();
		return direction;
	}

	Vector2 GetDirection2D(Vector3 origin, Vector3 dest)
	{
		Vector2 origin2D = Vector2(origin.x, origin.y);
		Vector2 dest2D = Vector2(dest.x, dest.y);

		return GetDirection2D(origin2D, dest2D);
	}	

	float GetDistance3D(const GameObject* const a, const GameObject* const b)
	{
		Vector3 pos1 = a->GetComponent<Transform>()->GetWorldMatrix().Translation();
		Vector3 pos2 = b->GetComponent<Transform>()->GetWorldMatrix().Translation();

		return Vector3::Distance(pos1, pos2);
	}

	float GetDistance2D(const GameObject* const a, const GameObject* const b)
	{
		Vector3 pos1 = a->GetComponent<Transform>()->GetWorldMatrix().Translation();
		Vector3 pos2 = b->GetComponent<Transform>()->GetWorldMatrix().Translation();

		pos1.z = 0.f;
		pos2.z = 0.f;

		return Vector3::Distance(pos1, pos2);
	}

	float GetAngle2D(Vector3 direction)
	{		
		return Rad2Deg(atan2(direction.y, direction.x));
	}

	float GetAngle2D(Vector2 direction)
	{
		return Rad2Deg(atan2(direction.y, direction.x));
	}

	XMVECTOR QuaternionEuler(float x, float y, float z)
	{
		// DirectXMath는 기본적으로 radian을 사용하므로, degree를 radian으로 변환
		float radX = XMConvertToRadians(x);
		float radY = XMConvertToRadians(y);
		float radZ = XMConvertToRadians(z);

		// Roll, Pitch, Yaw 각도를 이용하여 Quaternion을 생성
		return XMQuaternionRotationRollPitchYaw(radX, radY, radZ);	
	}

	/*float GetAngle3D(const GameObject* const origin, const GameObject* const dest)
	{
		Vector3 direction = GetDirection3D(origin, dest);

		return 0.0f;
	}

	float GetAngle2D(const GameObject* const origin, const GameObject* const dest)
	{
		return 0.0f;
	}*/


}

namespace helper::rand
{
	int RandInt(const int a, const int b)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(a, b);

		return dis(gen);
	}
}

