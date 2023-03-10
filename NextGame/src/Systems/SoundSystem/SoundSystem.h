#pragma once
#include "Systems/System.h"

/**
 * \brief Controls the all sound in scene
 */
class SoundSystem : public virtual System
{
public:
	explicit SoundSystem(Scene& scene) : System(scene) {}
	void Update(float deltaTime) override;
	void Render() override {}
	~SoundSystem() override = default;
};

