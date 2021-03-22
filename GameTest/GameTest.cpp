#include "stdafx.h"
#include <windows.h> 
#include <math.h>
#include "app\app.h"
#include <iostream>
#include <time.h>
#include <vector>
#include "Path.h"
#include "Turret.h"
#include "Enemy.h"

// DEFAULT BOX SIZE = 128

const int size = 48;

// Round counter
int cRound;
bool roundDone;
bool roundStart;
bool placeMode;
int chosen;

// Health Var
int health;
bool gameOver;

// Make sure enemies don't spawn at once
std::vector<int> enemyGo;
long int roundStartTime = 0;

// Initialize Towers vector
std::vector<Turret*> turrets;

// PlaceHolder Turrets (of each type)
CSimpleSprite* green;
CSimpleSprite* blue;
CSimpleSprite* red;
CSimpleSprite* purple;

// Circle to indicate range
CSimpleSprite* rCircle;

// Initialize enemies vector
std::vector<Enemy*> enemies;

// Init Map char array [int][int] and Queue Vector
int map[size][size];
std::vector<std::vector<int>> q;
std::vector<Path*> paths;

/* GenerateMap(int map[size][size], std::vector<std::vector<int>> &path, int lim)
	2d Map generation function
	map: 2d array of size * size defined at start of program.
	q: 2d vector array that is the path the enemies must take on the map
	min: the minimum amount of moves that can be in q
*/
bool GenerateMap(int map[size][size], std::vector<std::vector<int>>& path, int min) {
	// set random y position
	int y = rand() % size;
	// set the initial x position
	int x = 0;
	// Find final position
	int fx = 0;
	int fy = 0;
	int r = rand() % 3;
	switch (r) {
	case 0:
		fx = rand() % size;
		fy = 0;
		break;
	case 1:
		fx = rand() % size;
		fy = size-1;
		break;
	case 2:
		fx = size-1;
		fy = rand() % size;
		break;
	}
	
	map[y][x] = 1;
	path.push_back({ y, x });
	int border = false;

	while (y != fy || x != fx) {
		if (x == fx)
			border = true;
		else
			border = false;
		// Get a random next move. 3 options, one for each direction
		int nextMove = rand() % 3;
		switch (nextMove) {
		case 0:
			if (x < fx && map[y][x + 1] != 1)
				x += 1;
			break;
		case 1:
			if (!border && y + 1 <= (size - 1) && map[y + 1][x] != 1)
				y += 1;
			else if (border && y < fy && y + 1 <= (size - 1) && map[y + 1][x] != 1)
				y += 1;
			break;
		case 2:
			if (!border && y - 1 >= 0 && map[y - 1][x] != 1)
				y -= 1;
			else if (border && y > fy && y - 1 >= 0 && map[y - 1][x] != 1)
				y -= 1;
			break;
		}
		if (map[y][x] != 1) {
			map[y][x] = 1;
			path.push_back({ y,x });
		}
	}
	path.push_back({ y, x });
	if (path.size() < min)
		return false;
	return true;
}

// Function that runs at start of round
void StartRound(int round, int map[size][size], std::vector<std::vector<int>>& q) {
	for (auto i : turrets)
		i->RoundClear();
	roundStartTime = 0;
	enemyGo.clear();
	q.clear();
	for (auto i : enemies) {
		i->ShutDown();
		delete i;
	}
	enemies.clear();
	paths.clear();
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			map[i][j] = 0;
		}
	}
	for (auto i : turrets) {
		i->PickUp();
		i->SetPosition(-5, -5);
	}

	while (!GenerateMap(map, q, 5 * (round / 3))) {
		q.clear();
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				map[i][j] = 0;
			}
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (map[i][j] == 1) {
				paths.push_back(new Path(".\\res\\enemypath.bmp", 1));
				
			}
			else {
				paths.push_back(new Path(".\\res\\path.bmp", 0));
			}
			paths.back()->SetPosition(j * 16 + 8, i * 16 + 8);

		}
	}

	if (cRound % 16 == 0)
		turrets.push_back(new Turret(".\\res\\purple.bmp", 3));
	else if (cRound % 8 == 0)
		turrets.push_back(new Turret(".\\res\\red.bmp", 2));
	else if (cRound % 4 == 0)
		turrets.push_back(new Turret(".\\res\\blue.bmp", 1));
	else if (cRound % 2 == 0)
		turrets.push_back(new Turret(".\\res\\green.bmp", 0));
	for (int i = 1; i <= cRound; i++) {
		if (i % 15 == 0)
			enemies.push_back(new Enemy(".\\res\\redenemy.bmp", 3, q));
		if (i % 10 == 0)
			enemies.push_back(new Enemy(".\\res\\yellowenemy.bmp", 2, q));
		if (i % 5 == 0)
			enemies.push_back(new Enemy(".\\res\\greenenemy.bmp", 1, q));
		enemies.push_back(new Enemy(".\\res\\pinkenemy.bmp", 0, q));
	}
	for (auto i : enemies)
		enemyGo.push_back(0);
}

// Function to handle selection of towers
int Selector() {
	float x;
	float y;
	App::GetMousePos(x, y);
	if (x > 768 && y > 325) {
		if (y < 425)
			return 3;
		else if (y < 525)
			return 2;
		else if (y < 625)
			return 1;
		else if (y < 725)
			return 0;
		else
			return 4;
	}
	return 5;
}

// Init Function
void Init() {

	/*Raul's Scuffed Console™*/
	BOOL Monkey;
	Monkey = AllocConsole();
	if (Monkey && IsDebuggerPresent()) {
		freopen("CONOUT$", "w", stdout);
	}
	/*-----------------------*/

	// Init placeholder sprites
	green = App::CreateSprite(".\\res\\green.bmp", 1, 1);
	green->SetPosition(950, 675);
	green->SetScale(0.5f);
	blue = App::CreateSprite(".\\res\\blue.bmp", 1, 1);
	blue->SetPosition(950, 575);
	blue->SetScale(0.5f);
	red = App::CreateSprite(".\\res\\red.bmp", 1, 1);
	red->SetPosition(950, 475);
	red->SetScale(0.5f);
	purple = App::CreateSprite(".\\res\\purple.bmp", 1, 1);
	purple->SetPosition(950, 375);
	purple->SetScale(0.5f);
	rCircle = App::CreateSprite(".\\res\\circle.bmp", 1, 1);
	rCircle->SetColor(0, 0, 0, 0);
	rCircle->SetScale(0.0f);

	cRound = 1;
	roundDone = false;
	roundStart = false;
	placeMode = false;
	chosen = 5;
	health = 100;
	turrets.push_back(new Turret(".\\res\\green.bmp", 0));

	srand(time(NULL));
	StartRound(cRound, map, q);
	/*
	for (auto i : q) {
		std::cout << i[0] << " " << i[1] << std::endl;
	}
	*/
}

// Runs every physics frame
void Update(float dt) {
	if (gameOver) {
		if (App::IsKeyPressed(0x52)) { // R key
			cRound = 0;
			StartRound(++cRound, map, q);
			roundDone = false;
			roundStart = false;
			gameOver = false;
		}
	}
	else if (placeMode && !roundStart) {
		float mx, my;
		App::GetMousePos(mx, my);
		rCircle->SetPosition(mx, my);
		float circleScale;
		switch (chosen) {
		case 0:
			circleScale = 0.125 * 3;
			break;
		case 1:
			circleScale = 0.125 * 2;
			break;
		case 2:
		case 3:
			circleScale = 0.125 * 10;
			break;
		default:
			circleScale = 0;
		}
		rCircle->SetScale(circleScale);
		rCircle->SetColor(0, 0, 0, 0.25f);
		if (App::IsKeyPressed(VK_LBUTTON) && mx < 728 && my < 728) {
			for (auto i : turrets) {
				if (i->GetType() == chosen && !i->IsPlaced() && map[(int)floor(my/16)][(int)floor(mx/16)] != 1) {
					i->SetPosition(floor(mx/16)*16+8, floor(my/16)*16+8);
					i->Place();
					rCircle->SetColor(0, 0, 0, 0.0f);
					break;
				}
			}
			placeMode = false;
		}
	}
	else if (!roundDone && !roundStart) {
		// POLL KEY PRESSES AND CHECK IF THEY ARE AVAILABLE. IF THEY ARE ENTER PLACEMENT MODE
		if (App::IsKeyPressed(VK_LBUTTON)) {
			chosen = Selector();
			switch (chosen) {
			case 0:
			case 1:
			case 2:
			case 3:
				placeMode = true;
				break;
			case 4:
			case 5:
				placeMode = false;
				break;
			}
		}
		int turrCount = 0;
		for (auto i : turrets)
			turrCount += i->IsPlaced();
		if (turrCount == turrets.size())
			roundStart = true;
		for (auto i : paths)
			i->Update(dt);
	}
	else if (!roundDone && roundStart) {
		if(roundStartTime == 0)
			roundStartTime = static_cast<long int>(time(NULL));
		long int tempTime = static_cast<long int>(time(NULL)) - roundStartTime;
		if (tempTime < enemyGo.size())
			enemyGo[tempTime] = 1;
		if (tempTime > enemyGo.size()) {
			for (int i = 0; i < enemyGo.size(); ++i) {
				enemyGo[i] = 1;
			}
		}
		int dead = 0;
		for (auto i : enemies) {
			if (i == nullptr || i->IsAlive() == 1)
				dead++;
		}
		if (dead == enemies.size())
			roundDone = true;
		for (auto i : turrets)
			i->Update(dt, enemies, roundStartTime);
		for (int i = 0; i < enemies.size(); ++i) {
			if (enemies[i] != nullptr || enemies[i]->IsAlive() == 0) {
				int updt = 0;
				if(enemyGo[i] == 1)
					updt = enemies[i]->Update(dt);
				switch (updt) {
				case 1:
					health -= enemies[i]->GetDamage();
				case 2:
					break;
				}
			}
		}
		for (auto i : paths)
			i->Update(dt);
	}
	else {
		StartRound(++cRound, map, q);
		roundDone = false;
		roundStart = false;
	}
	if (health <= 0)
		gameOver = true;
	else
		gameOver = false;
}

// Runs every frame (used for drawing)
void Render() {
	
	int gCount = 0;
	int bCount = 0;
	int rCount = 0;
	int pCount = 0;

	for (auto i : turrets) {
		switch (i->GetType()) {
		case 0:
			if(!i->IsPlaced())
				gCount += 1;
			break;
		case 1:
			if (!i->IsPlaced())
				bCount += 1;
			break;
		case 2:
			if (!i->IsPlaced())
				rCount += 1;
			break;
		case 3:
			if (!i->IsPlaced())
				pCount += 1;
			break;
		}
	}

	char cg[4];
	char bg[4];
	char rg[4];
	char pg[4];
	char hp[4];
	char cr[4];

	_itoa(gCount, cg, 10);
	_itoa(bCount, bg, 10);
	_itoa(rCount, rg, 10);
	_itoa(pCount, pg, 10);
	_itoa(health, hp, 10);
	_itoa(cRound, cr, 10);

	App::Print(790, 737.5, "Health:");
	App::Print(855, 737.5, hp);

	App::Print(905, 737.5, "Round:");
	App::Print(975, 737.5, cr);

	App::Print(800, 700, "Greens:", 0, 255, 0);
	App::Print(800, 650, cg, 0, 255, 0);
	green->Draw();
	App::Print(800, 600, "Blues:", 0, 0, 255);
	App::Print(800, 550, bg, 0, 0, 255);
	blue->Draw();
	App::Print(800, 500, "Reds:", 255, 0, 0);
	App::Print(800, 450, rg, 255, 0, 0);
	red->Draw();
	App::Print(800, 400, "Purples:", 0.8f, 0.01f, 0.8f);
	App::Print(800, 350, pg, 0.8f, 0.01f, 0.8f);
	purple->Draw();

	for (auto i : paths)
		i->Render();

	rCircle->Draw();

	if (!turrets.empty()) {
		for (auto i : turrets)
			i->Render();
	}
	if (!enemies.empty()) {
		for (auto i : enemies) {
			if (i != nullptr && i->IsAlive() == 0)
				i->Render();
		}
	}
	
	if (gameOver) {
		App::Print(312, 470, "GAME OVER");
		App::Print(307, 450, "Max Round:");
		char cr[10];
		_itoa(cRound, cr, 10);
		App::Print(417, 450, cr);
	}
}

// Free memory
void Shutdown() {
	for (auto i : paths) {
		i->ShutDown();
		delete i;
	}
	if (!turrets.empty()) {
		for (auto i : turrets) {
			i->ShutDown();
			delete i;
		}
	}
	if (!enemies.empty()) {
		for (auto i : enemies) {
			if (i != nullptr) {
				i->ShutDown();
				delete i;
			}
		}
	}
	paths.clear();
	turrets.clear();
	q.clear();
	enemies.clear();
}

/*
//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Eample data....
//------------------------------------------------------------------------
CSimpleSprite *testSprite;
CSimpleSprite *testSprite2;
enum
{
	ANIM_FORWARDS,
	ANIM_BACKWARDS,
	ANIM_LEFT,
	ANIM_RIGHT,
};
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	

	testSprite = App::CreateSprite(".\\TestData\\Test.bmp", 8, 4);
	testSprite->SetPosition(400.0f, 400.0f);
	float speed = 1.0f / 15.0f;
	testSprite->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
	testSprite->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
	testSprite->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });
	testSprite->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });
	testSprite->SetScale(2.0f);

	testSprite2 = App::CreateSprite(".\\TestData\\Ships.bmp", 2, 12);
	testSprite2->SetPosition(400.0f, 400.0f);	
	testSprite2->SetFrame(2);
	testSprite2->SetScale(1.0f);
	//------------------------------------------------------------------------
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	testSprite->Update(deltaTime);
	testSprite2->Update(deltaTime);
	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		testSprite->SetAnimation(ANIM_RIGHT);
		float x, y;
		testSprite->GetPosition(x, y);
		x += 1.0f;
		testSprite->SetPosition(x, y);
	}
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		testSprite->SetAnimation(ANIM_LEFT);
		float x, y;
		testSprite->GetPosition(x, y);
		x -= 1.0f;
		testSprite->SetPosition(x, y);
	}
	if (App::GetController().GetLeftThumbStickY() > 0.5f)
	{
		testSprite->SetAnimation(ANIM_FORWARDS);
		float x, y;
		testSprite->GetPosition(x, y);
		y += 1.0f;
		testSprite->SetPosition(x, y);
	}
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		testSprite->SetAnimation(ANIM_BACKWARDS);
		float x, y;
		testSprite->GetPosition(x, y);
		y -= 1.0f;
		testSprite->SetPosition(x, y);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
	{
		testSprite->SetScale(testSprite->GetScale() + 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
	{
		testSprite->SetScale(testSprite->GetScale() - 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
	{
		testSprite->SetAngle(testSprite->GetAngle() + 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
	{
		testSprite->SetAngle(testSprite->GetAngle() - 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
	{
		testSprite->SetAnimation(-1);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	{
		testSprite->SetVertex(0, testSprite->GetVertex(0) + 5.0f);
	}
	//------------------------------------------------------------------------
	// Sample Sound.
	//------------------------------------------------------------------------
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	{
		App::PlaySound(".\\TestData\\Test.wav");
	}
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{	

	//------------------------------------------------------------------------
	// Example Line Drawing.
	//------------------------------------------------------------------------
	static float a = 0.0f;
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
	a += 0.1f;
	for (int i = 0; i < 20; i++)
	{

		float sx = 200 + sinf(a + i * 0.1f)*60.0f;
		float sy = 200 + cosf(a + i * 0.1f)*60.0f;
		float ex = 700 - sinf(a + i * 0.1f)*60.0f;
		float ey = 700 - cosf(a + i * 0.1f)*60.0f;
		g = (float)i / 20.0f;
		b = (float)i / 20.0f;
		App::DrawLine(sx, sy, ex, ey,r,g,b);
	}

	//------------------------------------------------------------------------
	// Example Sprite Code....
	testSprite->Draw();
	testSprite2->Draw();
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	// Example Text.
	//------------------------------------------------------------------------
	App::Print(100, 100, "Sample Text");

}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	
	//------------------------------------------------------------------------
	// Example Sprite Code....
	delete testSprite;
	delete testSprite2;
	//------------------------------------------------------------------------
}
*/