#pragma once
#include "Components/PlayerInfoObj.h"
#include "Systems/System.h"


/**
 * \brief Renderer deals with any entity with a Mesh and a Transform Attach to it
 */
class UI : public virtual System
{
public:
	UI(Scene& scene, PlayerInfoObj* playerInfo) : System(scene), PlayerInfo(playerInfo) {}
	void Update(float deltaTime) override {}
	void Render() override;
	~UI() override
	{
	}
private:
	PlayerInfoObj* PlayerInfo{ nullptr };
};