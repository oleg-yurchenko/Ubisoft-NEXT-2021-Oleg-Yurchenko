#include "stdafx.h"
#include <windows.h> 
#include <math.h>
#include "app\app.h"
#include <iostream>
#include <time.h>
#include <vector>

const int size = 16;

/* GenerateMap(int map[size][size], std::vector<std::vector<int>> &q, int lim)
	2d Map generation function
	map: 2d array of size * size defined at start of program.
	q: 2d vector array that is the path the enemies must take on the map
	min: the minimum amount of moves that can be in q
*/
bool GenerateMap(int map[size][size], std::vector<std::vector<int>> &q, int min) {

	// set random y position
	int y = rand() % (size-2) + 1;
	// set the 2nd move to x=1
	int x = 1;
	// Change pos at (0,y) and (1,y) to mark as visited
	map[y][0] = 1;
	map[y][x] = 1;
	// add the positions from above to the q array
	q.push_back({ y, 0 });
	q.push_back({ y, x });

	// loops used to keep track of length of path (mloops is max length of the path, or if the script gets stuck, max loop limit)
	int loops = 0;
	int mloops = 500;

	// While loop to check if the x or y position is touching an edge
	while ((x != size-1 && x != 0) && (y != size-1 && y != 0)) {
		// Get a random next move. 4 options, one for each direction
		int nextMove = rand() % 4;
		// Switch statement to see where to move next
		switch(nextMove) {
		// Move x 1 right
		case(0):
			if (x - 1 >= 0 && map[y][x - 1] != 1)
				x -= 1;
			break;
		// Move y 1 up
		case(1):
			if (y + 1 < size && map[y + 1][x] != 1)
				y += 1;
			break;
		// Move x 1 left
		case(2):
			if (x + 1 < size && map[y][x + 1] != 1)
				x += 1;
			break;
		// Move y 1 down
		case(3):
			if (y - 1 >= 0 && map[y - 1][x] != 1)
				y -= 1;
			break;
		}
		// Boolean to keep track whether the move is repeated or not.
		bool isOrig = true;
		for (int i = 0; i < q.size(); ++i) {
			std::vector<int> currPos = { y, x };
			if (currPos == q[i]) {
				isOrig = false;
				break;
			}
		}
		// Add the move to the path and mark the map position as visited
		q.push_back({ y, x });
		map[y][x] = 1;
		// If the amount of loops exceeds the maximum amount of loops, break out of for loop.
		if (++loops > mloops)
			break;
	}
	// If max length exceeded or the size of the path is too small (less than min) return false to signal failed map generation
	if (loops > mloops || q.size() < min)
		return false;
	return true;
}

// Function that runs at start of round
void StartRound(int round) {

}


// Initialize Towers vector

// Initialize enemies vector

// Init Map char array [int][int] and Queue Vector
int map[size][size];
std::vector<std::vector<int>> q;

// Init Function
void Init() {
	/*Raul's Scuffed Console™*/
	BOOL Monkey;
	Monkey = AllocConsole();
	if (Monkey && IsDebuggerPresent()) {
		freopen("CONOUT$", "w", stdout);
	}
	/*---------------------*/

	srand(time(NULL));
	// Keep generating map until the function returns true. If it returns false then just keep clearing the map and q
	while (!GenerateMap(map, q, 5/* 5 * rounds */)) {
		q.clear();
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				map[i][j] = 0;
			}
		}
	}
	
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			std::cout << map[i][j] << "\t";
		}
		std::cout << std::endl;
	}

	for (auto i : q) {
		std::cout << i[0] << " " << i[1] << std::endl;
	}
}

// Runs every physics frame
void Update(float dt) {

}

// Runs every frame (used for drawing)
void Render() {


}

// Free memory
void Shutdown() {


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