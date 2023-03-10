#include "stdafx.h"
#include "AIController.h"
#include "Components/AI.h"
#include "Components/Transform.h"


bool DirectionBlocking(const Map& map, const std::vector<std::pair<int, int>>& otherEntities,
	const int excludeIndex, const int agentX, const int agentY, const Vec3d direction,
	const int probeDirection = 1)
{
	const int checkX = agentX + probeDirection * static_cast<int>(direction.X);
	const int checkY = agentY + probeDirection * static_cast<int>(direction.Z);
	if (checkX < 0 || checkY < 0 || checkX > MAP_WIDTH || checkY > MAP_HEIGHT)
	{
		return true;
	}
	for (int i = 0; i < otherEntities.size(); i++)
	{
		if (i == excludeIndex) continue;
		auto position = otherEntities[i];
		if (checkX == position.first && checkY == position.second)
		{
			return true;
		}
	}
	return map.BlockMap[checkX][checkY] == 1;
}


std::vector<Vec3d> PossibleDirection(Map& map, int agentX, int agentY, const std::vector<std::pair<int, int>>& otherEntities, const int excludeIndex, int probedirection = 1)
{
	std::vector<Vec3d> possibleDirection;
	Vec3d forward = { 1, 0, 0 };
	if (!DirectionBlocking(map, otherEntities, excludeIndex, agentX, agentY, forward, probedirection))
	{
		possibleDirection.push_back(forward);
	}
	Vec3d backward = { -1, 0, 0 };
	if (!DirectionBlocking(map, otherEntities, excludeIndex, agentX, agentY, backward, probedirection))
	{
		possibleDirection.push_back(backward);
	}
	Vec3d right = { 0, 0, 1 };
	if (!DirectionBlocking(map, otherEntities, excludeIndex, agentX, agentY, right, probedirection))
	{
		possibleDirection.push_back(right);
	}
	Vec3d left = { 0, 0, -1 };
	if (!DirectionBlocking(map, otherEntities, excludeIndex, agentX, agentY, left, probedirection))
	{
		possibleDirection.push_back(left);
	}
	return possibleDirection;
}

void AIController::Update(float deltaTime)
{
	int componentIds[] = { GetId<AI>(), GetId<Transform>() };
	const auto sceneIterator = SceneIterator(SystemScene, componentIds, 2);

	std::vector<Transform*> aiTransforms;
	std::vector<AI*> agents;
	std::vector<std::pair<int, int>> ai2dPositions;

	for (const EntityId entity : sceneIterator)
	{
		auto ai = SystemScene.Get<AI>(entity);
		auto aiTransform = SystemScene.Get<Transform>(entity);
		// Get map position of the AI
		int x = static_cast<int>(aiTransform->Position.X + (MAP_HEIGHT / 2));
		int y = static_cast<int>(aiTransform->Position.Z + (MAP_WIDTH / 2));
		ai->X = x;
		ai->Y = y;
		aiTransforms.push_back(aiTransform);
		ai2dPositions.emplace_back(x, y);
		agents.push_back(ai);
	}

	for (int i = 0; i < agents.size(); i++)
	{
		AI* agent = agents[i];
		Transform* agentTransform = aiTransforms[i];

		const std::pair<int, int> position = ai2dPositions[i];
		std::vector<Vec3d> possibleDirection = PossibleDirection(*SceneMap, position.first, position.second, ai2dPositions, i);
		bool blocking = DirectionBlocking(*SceneMap, ai2dPositions, i, position.first, position.second, agent->Direction);
		if ((agent->Direction.X == 0 && agent->Direction.Z == 0) || (blocking && possibleDirection.size() >= 1))
		{
			// pick a random non blocking new direction
			int randNum = rand() % possibleDirection.size();
			agent->Direction = possibleDirection[randNum];
		}
		if (agent->Direction.X != 0 || agent->Direction.Z != 0)
		{
			MoveTransform(*agentTransform, agent->Direction * agent->Speed * deltaTime);
		}

	}
}
