#pragma once
#include "App/SimpleSprite.h"

class Turret
{
public:
	Turret(const char* filename, int type);
	void Render();
	void Update(float dt);
	void SetPosition(float x, float y);
	void GetPosition(float& x, float& y);
	void ShutDown();
	void Place();
	void PickUp();
	int IsPlaced();
	int GetType();
private:
	CSimpleSprite* m_sprite;
	int m_type;
	int placed;
};

