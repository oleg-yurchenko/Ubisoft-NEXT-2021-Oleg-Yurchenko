#include "stdafx.h"
#include "Path.h"
#include "App/SimpleSprite.h"
#include "App/app.h"

Path::Path(const char* filename, int type) {
	m_sprite = App::CreateSprite(filename, 1, 1);
	m_sprite->SetFrame(0);
	// Need to change this to fit 16x16 scale
	m_sprite->SetScale(0.125);
	m_type = type;
}

int Path::GetType() {
	return m_type;
}

void Path::Render() {
	m_sprite->Draw();
}

void Path::Update(float dt) {
	m_sprite->Update(dt);
}

void Path::SetPosition(float x, float y) {
	m_sprite->SetPosition(x, y);
}

void Path::GetPosition(float& x, float& y) {
	m_sprite->GetPosition(x, y);
}

void Path::ShutDown() {
	delete m_sprite;
}