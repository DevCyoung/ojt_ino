#pragma once

struct tVertex
{
	Vector3 Position;
	Vector4 Color;
	Vector2 UV;

	Vector3 Normal;
	Vector3 Tangent;
	Vector3 Binormal;

	// Animation ����ġ �� �ε���
	Vector4 vWeights;
	Vector4 vIndices;
};