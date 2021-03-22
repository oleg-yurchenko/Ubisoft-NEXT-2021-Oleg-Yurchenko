#pragma once
#include "App/SimpleSprite.h"
#include "Enemy.h"

class Turret
{
public:
	Turret(const char* filename, int type);
	void Render();
	void Update(float dt, std::vector<Enemy*> enemies, long int roundStart);
	void SetPosition(float x, float y);
	void GetPosition(float& x, float& y);
	void ShutDown();
	void Place();
	void PickUp();
	int IsPlaced();
	int GetType();
	void RoundClear();
private:
	CSimpleSprite* m_sprite;
	int m_type;
	int placed;
	int damage;
	int range;
	int rate;
	Enemy* toShoot;
};

