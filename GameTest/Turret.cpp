#include "stdafx.h"
#include "Turret.h"
#include "App/SimpleSprite.h"
#include "App/app.h"
#include "Enemy.h"
#include <time.h>
#include <iostream>

Turret::Turret(const char* filename, int type) {
	m_sprite = App::CreateSprite(filename, 1, 1);
	m_sprite->SetFrame(0);
	m_sprite->SetScale(0.125);
	m_type = type;
	placed = false;
	switch (type) {
	case 0:
		damage = 1;
		range = 4;
		rate = 2;
		break;
	case 1:
		damage = 3;
		range = 3;
		rate = 3;
		break;
	case 2:
		damage = 1;
		range = 10;
		rate = 1;
		break;
	case 3:
		damage = 15;
		range = 10;
		rate = 5;
		break;
	}
	toShoot = nullptr;
}

void Turret::Render() {
	m_sprite->Draw();
	float px, py;
	m_sprite->GetPosition(px, py);
	if(toShoot != nullptr && toShoot->IsAlive() == 0) {
		float ex, ey;
		toShoot->GetPosition(ex, ey);
		float cDist = ceil(sqrt(pow((ex - px), 2) + pow((ey - py), 2)) / 16);
		if (cDist < range)
			App::DrawLine(px, py, ex, ey);
	}
}

void Turret::Update(float dt, std::vector<Enemy*> enemies, long int roundStart) {
	if ((int)(static_cast<long int>(time(NULL)) - roundStart) % rate == 0) {
		float px, py;
		m_sprite->GetPosition(px, py);
		int maxDistance = 0;
		if (toShoot == nullptr) {
			std::vector<Enemy*> targetableEnemies;
			for (auto i : enemies) {
				float ex, ey;
				if (i != nullptr && i->IsAlive() == 0) {
					i->GetPosition(ex, ey);
					if (ceil(sqrt(pow(ex - px, 2) + pow(ey - py, 2)) / 16) <= range)
						targetableEnemies.push_back(i);
				}
			}
			for (auto i : targetableEnemies) {
				if(i != nullptr && i->IsAlive() == 0) {
					float ex, ey;
					i->GetPosition(ex, ey);
					float cDist = ceil(sqrt(pow((ex - px), 2) + pow((ey - py), 2)) / 16);
					if (cDist > maxDistance) {
						toShoot = i;
						maxDistance = cDist;
					}
				}
			}
		}
		else {
			float ex, ey;
			if(toShoot != nullptr && toShoot->IsAlive() == 0) {
				toShoot->GetPosition(ex, ey);
				float cDist = ceil(sqrt(pow((ex - px), 2) + pow((ey - py), 2)) / 16);
				if (cDist > range) {
					toShoot = nullptr;
				}
				else {
					toShoot->Hit(damage * (dt/250));
				}
			}
			else {
				toShoot = nullptr;
			}
		}
	}
	m_sprite->Update(dt);
}

void Turret::SetPosition(float x, float y) {
	m_sprite->SetPosition(x, y);
}

void Turret::GetPosition(float& x, float& y) {
	m_sprite->GetPosition(x, y);
}

void Turret::ShutDown() {
	delete m_sprite;
}

void Turret::Place() {
	placed = true;
}

void Turret::PickUp() {
	placed = false;
}

int Turret::IsPlaced() {
	return placed;
}

int Turret::GetType() {
	return m_type;
}

void Turret::RoundClear() {
	placed = false;
	toShoot = nullptr;
}