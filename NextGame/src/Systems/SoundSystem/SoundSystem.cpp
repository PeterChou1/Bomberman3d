#include "stdafx.h"
#include "SoundSystem.h"
#include "App/app.h"
#include "Components/Sound.h"


void SoundSystem::Update(float deltaTime)
{
	int collisonTransform[] = { GetId<SoundEffect>() };
	const auto soundIterator = SceneIterator(SystemScene, collisonTransform, 1);
	for (const EntityId soundID : soundIterator)
	{
		auto soundEffect = SystemScene.Get<SoundEffect>(soundID);
		App::PlaySound(soundEffect->Soundfile);
		SystemScene.DestroyEntity(soundID);
	}
}
