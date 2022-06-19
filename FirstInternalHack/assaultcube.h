#pragma once
#include <cstdint>
#include <Windows.h>
#include "geom.h"

enum gameModes
{
	TEAMDEATHMATCH = 0,
	COOPEDIT,
	DEATHMATCH,
	SURVIVOR,
	TEAMSURVIVOR,
	CTF,
	PISTOLFRENZY,
	BOTTEAMDEATHMATCH,
	BOTDEATHMATCH,
	LASTSWISSSTANDING,
	ONESHOTONEKILL,
	TEAMONESHOTONEKILL,
	BOTONESHOTONEKILL,
	HUNTTHEFLAG,
	TEAMKEEPTHEFLAG,
	KEEPTHEFLAG,
	TEAMPF,
	TEAMLSS,
	BOTPISTOLFRENZY,
	BOtlSS,
	BOTTEAMSURVIVOR,
	BOTTEAMONESHOTONEKILL,
	NUM
};

class Entity
{
public:
	DWORD vTable;
	Vector3 head; //0x0004
	char pad_0010[36]; //0x0010
	Vector3 pos; //0x0034
	Vector3 angle; //0x0040
	char pad_004C[37]; //0x004C
	int8_t bScoping; //0x0071
	char pad_0072[134]; //0x0072
	int32_t health; //0x00F8
	int32_t armor; //0x00FC
	char pad_0100[292]; //0x0100
	int8_t bAttacking; //0x0224
	char name[16]; //0x0225
	char pad_0235[247]; //0x0235
	int8_t team; //0x032C
	char pad_032D[11]; //0x032D
	int8_t state; //0x0338
	char pad_0339[59]; //0x0339
	class weapon* weaponPtr; //0x0374

}; //Size: 0x0378

class weapon
{
public:
	char pad_0000[20]; //0x0000
	class weaponClip* weaponClipPtr; //0x0014
	char pad_0018[44]; //0x0018
}; //Size: 0x0044

class weaponClip
{
public:
	int32_t clip; //0x0000
	char pad_0004[64]; //0x0004
}; //Size: 0x0044

struct EntList
{
	Entity* ents[31];
}; 
