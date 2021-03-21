#include "stdafx.h"
#include "Turret.h"
#include "App/SimpleSprite.h"
#include "App/app.h"

Turret::Turret(const char* filename, int type) {
	m_sprite = App::CreateSprite(filename, 1, 1);
	m_sprite->SetFrame(0);
	m_sprite->SetScale(0.125);
	m_type = type;
	placed = false;
}

void Turret::Render() {
	m_sprite->Draw();
}

void Turret::Update(float dt) {
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