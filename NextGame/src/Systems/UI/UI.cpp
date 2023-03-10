#include "stdafx.h"
#include "UI.h"

#include "app/app.h"
#include "Components/AABB.h"
#include "Components/AI.h"

void UI::Render()
{
	int count = 0;
	int aiCount[] = { GetId<AABB>(), GetId<Mesh>(), GetId<AI>() };
	const auto aiCountIterator = SceneIterator(SystemScene, aiCount, 3);
	for (const EntityId _ : aiCountIterator)
	{
		count += 1;
	}
	// Basic UI Logic
	if (count == 0)
	{
		std::string youWin = "You Win";
		App::Print(APP_VIRTUAL_WIDTH / 2, APP_VIRTUAL_HEIGHT / 2, youWin.c_str());
	}
	else if (PlayerInfo->GameOver)
	{
		std::string gameOver = "Game Over";
		App::Print(APP_VIRTUAL_WIDTH / 2, APP_VIRTUAL_HEIGHT / 2, gameOver.c_str());
	}
	else
	{
		char BombCooldown[180];
		sprintf(BombCooldown, "Bomb cool down (%f)", PlayerInfo->CurrentCooldown);
		App::Print(100, 150, BombCooldown);
		char AICounter[180];
		sprintf(AICounter, "Enemies (%d)", count);
		App::Print(100, 200, AICounter);
		std::string instructions = "WASD (MOVE) SPACE (DROP BOMB) E (THROW GERNADE)";
		App::Print(100, 750, instructions.c_str());
	}
}
