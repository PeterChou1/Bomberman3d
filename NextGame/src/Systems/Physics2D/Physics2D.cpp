#include "stdafx.h"
#include "Physics2D.h"
#include "Components/Collider.h"
#include "Components/Collider2D.h"
#include "Components/Transform.h"


struct ContactResponse
{
	Vec3d normal;
	double penetration;
};

bool calculateContact(AABB& abox, AABB bbox, Transform& a, Transform& b, ContactResponse& c) {
	Vec3d aHalfExtents = (abox.PMax - abox.PMin) / 2.0;
	Vec3d bHalfExtents = (bbox.PMax - bbox.PMin) / 2.0;
	Vec3d aCenter = a.Position + (abox.PMax + abox.PMin) / 2.0;
	Vec3d bCenter = b.Position + (bbox.PMax + bbox.PMin) / 2.0;
	Vec3d centerDiff = bCenter - aCenter;
	double xDiff = abs(centerDiff.X);
	double zDiff = abs(centerDiff.Z);
	double xExtentSum = aHalfExtents.X + bHalfExtents.X;
	double zExtentSum = aHalfExtents.Z + bHalfExtents.Z;
	if (xDiff < xExtentSum && zDiff < zExtentSum) {
		// Calculate penetration depth along x and z axes
		double xPenetration = xExtentSum - xDiff;
		double zPenetration = zExtentSum - zDiff;
		// Select the axis of least penetration
		if (xPenetration < zPenetration) {
			// Resolve collision along x axis
			double sign = centerDiff.X > 0 ? 1 : -1;
			c.normal = Vec3d{ sign, 0.0, 0.0 };
			c.penetration = xPenetration;
		}
		else {
			// Resolve collision along z axis
			double sign = centerDiff.Z > 0 ? 1 : -1;
			c.normal = Vec3d{ 0.0, 0.0, sign };
			c.penetration = zPenetration;
		}
		return true;
	}
	return false;
}

void Physics2D::Update(const float deltaTime)
{
	using collisionPair = std::pair<EntityId, EntityId>;

	// STEP 1: transform aabb
	int collisonTransform[] = { GetId<AABB>(), GetId<Transform>() };
	const auto collisionObj = SceneIterator(SystemScene, collisonTransform, 2);
	for (const EntityId cObj : collisionObj)
	{
		auto cTransform = SystemScene.Get<Transform>(cObj);
		auto aabb = SystemScene.Get<AABB>(cObj);
		TransformAABB(*aabb, *cTransform);
	}


	int rigidbody[] = {GetId<Transform>(), GetId<AABB>()};
	const auto aabbIterator = SceneIterator(SystemScene, rigidbody, 2);
	std::vector<collisionPair> entityPair;

	for (const EntityId A : aabbIterator)
	{
		for (const EntityId B : aabbIterator)
		{
			if (A == B) continue;
			const auto abox = SystemScene.Get<AABB>(A);
			const auto bbox = SystemScene.Get<AABB>(B);
			if (abox->stationary && bbox->stationary) continue;
			if (Overlaps2d(*abox, *bbox))
			{
				entityPair.emplace_back(A, B);
			}
		}
	}
	// cull the pairVector to remove duplicates
	std::sort(entityPair.begin(), entityPair.end(),
	          [](const collisionPair& p1, const collisionPair& p2)
	          {
		          if (p1.first != p2.first)
			          return p1.first < p2.first;
		          return p1.second < p2.second;
	          });
	auto last = std::unique(entityPair.begin(), entityPair.end());
	entityPair.erase(last, entityPair.end());

	// calculate the penetration depth + 
	for (auto pair : entityPair)
	{
		ContactResponse c{};
		const auto abox = SystemScene.Get<AABB>(pair.first);
		const auto bbox = SystemScene.Get<AABB>(pair.second);
		const auto transformA = SystemScene.Get<Transform>(pair.first);
		const auto transformB = SystemScene.Get<Transform>(pair.second);
		if (calculateContact(*abox, *bbox, *transformA, *transformB, c))
		{
			// positional correction
			if (!abox->stationary)
			{
			    MoveTransform(*transformA, -1 * c.penetration * c.normal);
			}

			if (!bbox->stationary)
			{
				MoveTransform(*transformB, c.penetration * c.normal);
			}
		}
	}
}
