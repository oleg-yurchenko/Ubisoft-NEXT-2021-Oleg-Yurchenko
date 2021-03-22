#include "stdafx.h"
#include "Enemy.h"
#include "Path.h"
#include <vector>
#include "App/app.h"
#include <iostream>

Enemy::Enemy(const char* filename, int type, std::vector<std::vector<int>> paths) {
	m_sprite = App::CreateSprite(filename, 1, 1);
	m_sprite->SetFrame(0);
	m_sprite->SetScale(0.125);
	m_type = type;
	m_paths = paths;
	m_dest = m_paths[0];
	m_sprite->SetPosition(m_dest[1] * 16 + 8, m_dest[0] * 16 + 8);
	m_paths.erase(m_paths.begin());
	switch (type) {
	case 0:
		m_hp = 5;
		m_damage = 2;
		break;
	case 1:
		m_hp = 15;
		m_damage = 5;
		break;
	case 2:
		m_hp = 30;
		m_damage = 10;
		break;
	case 3:
		m_hp = 60;
		m_damage = 20;
		break;
	}
	m_alive = 0;
}

void Enemy::Render() {
	m_sprite->Draw();
}

int Enemy::Update(float dt) {
	m_sprite->Update(dt);
	if (m_alive == 1)
		return 2;
	if (m_hp <= 0)
		m_alive = 1;
	float cposx, cposy;
	if (m_paths.size() != 0) {
		m_sprite->GetPosition(cposx, cposy);
		float dx = m_dest[1] * 16 + 8;
		float dy = m_dest[0] * 16 + 8;
		if (cposx == dx && cposy == dy) {
			m_dest = m_paths[0];
			m_paths.erase(m_paths.begin());
		}
		else {
			if (cposx < dx)
				m_sprite->SetPosition(cposx + 1, cposy);
			if (cposy < dy)
				m_sprite->SetPosition(cposx, cposy + 1);
			if (cposy > dy)
				m_sprite->SetPosition(cposx, cposy - 1);
		}
	}
	else {
		m_alive = 1;
		return 1;
	}
	return 0;
}

void Enemy::SetPosition(float x, float y) {
	m_sprite->SetPosition(x, y);
}

void Enemy::GetPosition(float& x, float& y) {
	m_sprite->GetPosition(x, y);
}

void Enemy::ShutDown() {
	delete m_sprite;
	m_sprite = nullptr;
}

int Enemy::GetDamage() {
	return m_damage;
}

void Enemy::Hit(float amount) {
	m_hp -= amount;
}

int Enemy::IsAlive() {
	return m_alive;
}