#pragma once

class EngineResourceLoader
{
	friend class Engine;

private:
	EngineResourceLoader() = default;
	~EngineResourceLoader() = default;
	EngineResourceLoader(const EngineResourceLoader&) = delete;
	EngineResourceLoader& operator=(const EngineResourceLoader&) = delete;

	static void loadResource(int renderTargetWIdth, int renderTargetHeight, int shadowMapWIdth, int shadwMapHeight);
	static void loadMaterial();
	static void loadShader();
	static void loadComputeShader();
	static void loadMesh();
	static void loadTexture(int renderTargetWIdth, int renderTargetHeight, int shadowMapWIdth, int shadwMapHeight);
};

