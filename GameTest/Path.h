#pragma once
#include "App/SimpleSprite.h"

class Path
{
public:
	Path(const char *filename, int type);
	int GetType();
	void Render();
	void Update(float dt);
	void SetPosition(float x, float y);
	void GetPosition(float& x, float& y);
	void ShutDown();
private:
	CSimpleSprite *m_sprite;
	int m_type;
};

