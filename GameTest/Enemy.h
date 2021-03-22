#pragma once
#include "App/SimpleSprite.h"
#include <vector>

class Enemy
{
public:
	Enemy(const char* filename, int type, std::vector<std::vector<int>> paths);
	void Render();
	int Update(float dt);
	void SetPosition(float x, float y);
	void GetPosition(float& x, float& y);
	void ShutDown();
	int GetDamage();
	void Hit(float amount);
	int IsAlive();
private:
	CSimpleSprite* m_sprite;
	int m_type;
	std::vector<std::vector<int>> m_paths;
	std::vector<int> m_dest;
	float m_hp;
	int m_damage;
	int m_alive;
};

