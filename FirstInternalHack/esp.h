#pragma once
#include "assaultcube.h"
#include "gldraw.h"
#include "gltext.h"
#include "geom.h"

// scaling
// Using windowed mode with specific screen width
const int VIRTUAL_SCREEN_WIDTH = 800;
const int GAME_UNIT_MAGIC = 400; // how the game represents length. Meters, feet, blocks?

// different for every game. Must be reversed.
const float PLAYER_HEIGHT = 5.25f;
const float PLAYER_WIDTH = 2.0f;
const float EYE_HEIGHT = 4.5f;
const float PLAYER_ASPECT_RATIO = PLAYER_HEIGHT / PLAYER_WIDTH;

// Consolas font
const int ESP_FONT_HEIGHT = 15;
const int ESP_FONT_WIDTH = 9;


class esp
{
public:
	int* gameModes = (int*)(0x50F49C);
	int* numOfPlayers = (int*)(0x50F500);
	float* matrix = (float*)(0x501AE8);
	Entity* localPlayer = *(Entity**)0x50F4F4;
	EntList* entList = *(EntList**)0x50F4F8;

	int viewport[4]; // OpenGL, 3rd and 4th variable are window width and height

	bool IsTeamGame();
	bool isEnemy(Entity* e);
	bool isValidEnt(Entity* ent);

	void DrawESPBOX(Entity* p, vec3, GL::Font& font);
	void Draw(GL::Font& font);
};

