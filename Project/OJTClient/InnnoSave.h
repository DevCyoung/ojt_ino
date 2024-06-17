#pragma once	

enum eInnoSaveState_
{
	_ZsPos,
	_ZsSpeed,
	_ZsAcc,
	_ZuPos,
	_ZuSpeed,
	_ZuAcc,
	_Zr,
	_XPos,
	_XSpeed,
	_XPosOther,
};

class InnnoSave
{
public:
	InnnoSave();
	~InnnoSave();

	void Save();

	bool mbZsPos;
	bool mbZsSpeed;
	bool mbZsAcc;
	bool mbZuPos;
	bool mbZuSpeed;
	bool mbZuAcc;
	bool mbZr;
	bool mbXPos;
	bool mbXSpeed;
	bool mbXPosOther;

};