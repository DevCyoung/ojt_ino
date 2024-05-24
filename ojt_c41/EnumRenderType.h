#pragma once

enum class eRenderType
{
	Defereed,
	Opqaue,
	CutOut,
	Transparent,
	End,
	PostProcess,
};

enum class eCameraPriorityType
{
	Editor,
	BackGround,
	Main,
	UI,
	End
};

enum class eCameraProjectionType
{
	Perspective,
	Orthographic,
	End
};
