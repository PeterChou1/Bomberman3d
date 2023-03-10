#include "stdafx.h"
#include "BombManager.h"

#include "Components/AABB.h"
#include "Components/Bomb.h"
#include "Components/Mesh.h"
#include "Components/Sound.h"


struct Coordinate
{
    int X;
    int Y;
};

std::vector<Coordinate> CreateBombCross(const Map& map, const int bombX, const int bombY, const int bombHeight, const int bombWidth)
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

void BombManager::Update(const float deltaTime)
{
	int collisonTransform[] = { GetId<Bomb>(), GetId<Transform>() };
	const auto collisionObj = SceneIterator(SystemScene, collisonTransform, 2);
    // spawn bomb cross
	for (const EntityId bombId : collisionObj)
	{
		const auto bomb = SystemScene.Get<Bomb>(bombId);
		const auto bombTransform = SystemScene.Get<Transform>(bombId);
		if (bomb->Time < 0)
		{
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
                Vec3d bombPosition = { static_cast<double>(-(MAP_HEIGHT / 2) + coord.Y), bombTransform->Position.Y, static_cast<double>(-(MAP_WIDTH / 2) + coord.X) };
                InitTransform(*meshTransform, bombPosition);
                LoadFromObjectFile("./TestData/unitcube.obj", *mesh);
                ComputeAABB(*mesh, *meshAABB);
                meshAABB->IsBombTrigger = true;
                mesh->color = true;
                mesh->r = bombExplosionColorR; mesh->g = bombExplosionColorG; mesh->b = bombExplosionColorB;
            }
            // Add an explosion sound effect
            const EntityId soundId = SystemScene.NewEntity();
            const auto soundEffect = SystemScene.AddComponent<SoundEffect>(soundId);
            soundEffect->Soundfile = "./TestData/Test.wav";
		}
		bomb->Time -= deltaTime;
	}
    int aabb[] = { GetId<AABB>() };
    const auto aabbIterator = SceneIterator(SystemScene, aabb, 1);
    // Get rid of bomb cross after some time
    for (const EntityId fireId : aabbIterator)
    {
        const auto fire = SystemScene.Get<AABB>(fireId);

        if (fire->IsBombTrigger)
        {
            if (fire->BombTime < 0)
            {
                SystemScene.DestroyEntity(fireId);
            }
        	else
            {
				fire->BombTime -= deltaTime;
            }
        }
    }
}
