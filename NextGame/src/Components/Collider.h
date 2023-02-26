#pragma once
#include "Mesh.h"
#include "Transform.h"
#include "Math/Vec3d.h"

/**
 * Axis Aligned Bounding Box Use for Broad Phase Collision Detection
 */
struct AABB
{
	Vec3d PMax;
	Vec3d PMin;
};


struct Collider
{
	AABB aabb;
};

/**
 * \brief Test if b1 and b2 overlaps
 */
inline bool Overlaps(const AABB& b1, const AABB& b2) {
	return b1.PMax.X >= b2.PMin.X && b1.PMin.X <= b2.PMax.X &&
		b1.PMax.Y >= b2.PMin.Y && b1.PMin.Y <= b2.PMax.Y &&
		b1.PMax.Z >= b2.PMin.Z && b1.PMin.Z <= b2.PMax.Z;
}

/**
 * \brief Computes AABB for a mesh
 */
inline void ComputeAABB(Mesh& mesh, AABB& aabb)
{
	for(auto triangle : mesh.Triangles)
	{
		for (Vec3d points : triangle.P)
		{
			aabb.PMax.X = std::max(aabb.PMax.X, points.X);
			aabb.PMax.Y = std::max(aabb.PMax.Y, points.Y);
			aabb.PMax.Z = std::max(aabb.PMax.Z, points.Z);

			aabb.PMin.X = std::min(aabb.PMin.X, points.X);
			aabb.PMin.Y = std::min(aabb.PMin.Y, points.Y);
			aabb.PMin.Z = std::min(aabb.PMin.Z, points.Z);
		}
	}
}

/**
 * \brief Transform Axis Aligned Bounding Box to position and orientation defined by t
 */
inline void Transform(AABB& aabb, const Transform& t)
{
	// Algorithm based off of James Arvo 1990 paper for efficient transform of bounding box
	// explanation for algorithm found here:
	// https://twitter.com/Herschel/status/1188613724665335808/photo/1
    Vec3d transform = t.Position;
    Mat3X3 rotation = GetRotationMatrix(t);
    Vec3d newPMin = aabb.PMin;
    Vec3d newPMax = aabb.PMax;

    for (int i = 0; i < 3; i++) {
        newPMin[i] = newPMax[i] = transform[i];
        for (int j = 0; j < 3; j++) {
	        const double coeff = rotation[i][j];
	        const double pMinComponent = coeff * aabb.PMin[i];
	        const double pMaxComponent = coeff * aabb.PMax[i];
            if (pMinComponent < pMaxComponent) {
                newPMin[i] += pMinComponent;
                newPMax[i] += pMaxComponent;
            }
            else {
                newPMin[i] += pMaxComponent;
                newPMax[i] += pMinComponent;
            }
        }
    }
}