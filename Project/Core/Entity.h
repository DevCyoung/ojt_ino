#pragma once

class Entity
{
public:
	Entity();
	virtual ~Entity();
	Entity(const Entity&);
	Entity& operator=(const Entity&) = delete;

	void SetName(const std::wstring& name) { mName = name; }
	const std::wstring& GetName() const { return mName; }
	UINT GetID() const { return mID; }

	//virtual void Save() {};

	virtual Entity* Clone() = 0;
private:
	inline static UINT sID = 0;
	std::wstring mName;
	UINT mID;
};