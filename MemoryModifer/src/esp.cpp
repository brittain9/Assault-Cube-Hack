#include "pch.h"
#include "esp.h"

bool esp::IsTeamGame()
{
	if ((*gameModes == 0 || *gameModes == 4 || *gameModes == 5 || *gameModes == 7 || *gameModes == 11 || *gameModes == 13 || *gameModes == 14 || *gameModes == 16 || *gameModes == 17 || *gameModes == 20 || *gameModes == 21))
	{
		return true;
	}
	return false;
}

bool esp::isEnemy(Entity* e)
{
	if (localPlayer->team == e->team)
		return false;
	return true;
}

bool esp::isValidEnt(Entity* ent)
{
	if(ent)
	{
		if (ent->vTable == 0x4E4A98 || ent->vTable == 0x4E4AC0);
		{
			return true;
		}
	}
	return false;
}


void esp::DrawESPBOX(Entity* e, vec3 screen, GL::Font& font)
{
	const GLubyte* color = nullptr;

	if (IsTeamGame() && !isEnemy(e))
	{
		color = rgb::green;
	}
	else color = rgb::red;

	float dist = localPlayer->pos.Distance(e->pos);

	float scale = (GAME_UNIT_MAGIC / dist) * (viewport[2] / VIRTUAL_SCREEN_WIDTH);
	// puts to left of the player
	float x = screen.x - scale;
	// puts to top of the player
	float y = screen.y - scale * PLAYER_ASPECT_RATIO;
	float width = scale * 2;
	float height = scale * PLAYER_ASPECT_RATIO * 2;

	GL::DrawOutline(x, y, width, height, 2.0f, color);
	
	float textX = font.centerText(x, width, strlen(e->name) * ESP_FONT_WIDTH);
	float textY = y - ESP_FONT_HEIGHT / 2;
	font.Print(textX, textY, color, "%s", e->name);
}

void esp::Draw(GL::Font& font)
{
	// Get viewport / width and height of screen
	glGetIntegerv(GL_VIEWPORT, viewport);

	// loop through entity
	for (int i = 0; i < (*numOfPlayers); i++)
	{
		// if ent is valid we will draw them
		if ( entList && entList->ents && isValidEnt(entList->ents[i]) && entList->ents[i]->state == 0)
		{
			Entity* e = entList->ents[i];
			vec3 center = e->head;
			// get center of player
			center.z = center.z - EYE_HEIGHT + PLAYER_HEIGHT / 2;

			vec3 screenCoords;

			// only if world to screen works. Only if infront of us.
			if(WorldToScreen(center, screenCoords, matrix, viewport[2], viewport[3]))
			{
				DrawESPBOX(e, screenCoords, font);
			}
		}
	}
}