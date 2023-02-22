#pragma once
#include <bitset>


constexpr int MAX_COLLIDER_GROUPS = 32;

struct Collider
{
	std::bitset<MAX_COLLIDER_GROUPS> CollisionMask;
};