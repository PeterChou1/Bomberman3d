#include "stdafx.h"
#include "BombManager.h"

#include "Components/AABB.h"
#include "Components/Bomb.h"
#include "Components/Mesh.h"


struct Coordinate
{
    int x;
    int y;
};

std::vector<Coordinate> CreateBombCross(Map& map, int bombX, int bombY, int bombHeight, int bombWidth)
{
    std::vector<Coordinate> blastCoords;
    // Determine the height and width of the bomb blast radius
    // Blast the center
    blastCoords.push_back({ bombX, bombY });
    // Blast the top
    for (int i = 1; i < bombHeight; ++i) {
        if (map.BlockMap[bombY + i][bombX] == 1) {
            break;
        }
        blastCoords.push_back({ bombX, bombY + i });
    }
    // Blast the bottom
    for (int i = 1; i < bombHeight; ++i) {
        if (map.BlockMap[bombY - i][bombX] == 1) {
            break;
        }
        blastCoords.push_back({ bombX, bombY - i });
    }
    // Blast the left
    for (int i = 1; i < bombWidth; ++i) {
        if (map.BlockMap[bombY][bombX + i] == 1) {
            break;
        }
        blastCoords.push_back({ bombX + i, bombY });
    }
    // Blast the right
    for (int i = 1; i < bombWidth; ++i) {
        if (map.BlockMap[bombY][bombX - i] == 1) {
            break;
        }
        blastCoords.push_back({ bombX - i, bombY });
    }
    return blastCoords;
}

void BombManager::Update(float deltaTime)
{
	int collisonTransform[] = { GetId<Bomb>(), GetId<Transform>() };
	const auto collisionObj = SceneIterator(SystemScene, collisonTransform, 2);
	for (const EntityId bombId : collisionObj)
	{
		const auto bomb = SystemScene.Get<Bomb>(bombId);
		const auto bombTransform = SystemScene.Get<Transform>(bombId);
		if (bomb->Time < 0)
		{
            // spawn bomb cross
            SystemScene.DestroyEntity(bombId);
			int bombY = static_cast<int>(bombTransform->Position.X + (MAP_HEIGHT / 2));
			int bombX = static_cast<int>(bombTransform->Position.Z + (MAP_WIDTH / 2));
            std::vector<Coordinate> coords = CreateBombCross(*SceneMap, bombX, bombY, bomb->BombCrossSize, bomb->BombCrossSize);
            for (auto coord : coords)
            {

                const EntityId meshId = SystemScene.NewEntity();
                const auto mesh = SystemScene.AddComponent<Mesh>(meshId);
                const auto meshTransform = SystemScene.AddComponent<Transform>(meshId);
                const auto meshAABB = SystemScene.AddComponent<AABB>(meshId);
                // default explosion is 5 second
                Vec3d bombPosition = { static_cast<double>(-(MAP_HEIGHT / 2) + coord.y), 0.5, static_cast<double>(-(MAP_WIDTH / 2) + coord.x) };
                InitTransform(*meshTransform, bombPosition);
                LoadFromObjectFile("./TestData/unitcube.obj", *mesh);
                ComputeAABB(*mesh, *meshAABB);
                meshAABB->isBombTrigger = true;
                mesh->color = true;
                mesh->r = 1; mesh->g = 0; mesh->b = 0;
            }
		}
		bomb->Time -= deltaTime;
	}
    int aabb[] = { GetId<AABB>() };
    const auto aabbIterator = SceneIterator(SystemScene, aabb, 1);
    for (const EntityId fireId : aabbIterator)
    {
        const auto fire = SystemScene.Get<AABB>(fireId);

        if (fire->isBombTrigger)
        {
            if (fire->bombTime < 0)
            {
                SystemScene.DestroyEntity(fireId);
            }
        	else
            {
				fire->bombTime -= deltaTime;
            }
        }
    }
	// Get rid of bomb cross after some time
}
