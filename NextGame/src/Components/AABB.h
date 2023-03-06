#pragma once

#include <algorithm>
#include "Mesh.h"
#include "Transform.h"
#include "Math/Vec3d.h"
#include <vector>

/**
 * Axis Aligned Bounding Box Use for Broad Phase Collision Detection
 */
struct AABB
{
	// whether or not the Axis align bounding box is non moving objects
	bool Stationary = true;
	Vec3d PMax{ -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity() };
	Vec3d PMin{ std::numeric_limits<double>::infinity(),  std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() };
	Vec3d PMaxT{};
	Vec3d PMinT{};
};


inline bool Overlaps2d(const AABB& b1, const AABB& b2) {
	return b1.PMaxT.X >= b2.PMinT.X && b1.PMinT.X <= b2.PMaxT.X &&
		b1.PMaxT.Z >= b2.PMinT.Z && b1.PMinT.Z <= b2.PMaxT.Z;
}


/**
 * \brief Test if b1 and b2 overlaps 3d
 */
inline bool Overlaps(const AABB& b1, const AABB& b2) {
	return b1.PMaxT.X >= b2.PMinT.X && b1.PMinT.X <= b2.PMaxT.X &&
		b1.PMaxT.Y >= b2.PMinT.Y && b1.PMinT.Y <= b2.PMaxT.Y &&
		b1.PMaxT.Z >= b2.PMinT.Z && b1.PMinT.Z <= b2.PMaxT.Z;
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
			using std::max;
			using std::min;

			aabb.PMax.X = max(aabb.PMax.X, points.X);
			aabb.PMax.Y = max(aabb.PMax.Y, points.Y);
			aabb.PMax.Z = max(aabb.PMax.Z, points.Z);
			
			aabb.PMin.X = min(aabb.PMin.X, points.X);
			aabb.PMin.Y = min(aabb.PMin.Y, points.Y);
			aabb.PMin.Z = min(aabb.PMin.Z, points.Z);
		}
	}
	aabb.PMaxT = aabb.PMax;
	aabb.PMinT = aabb.PMin;
}

/**
 * \brief Transform Axis Aligned Bounding Box to position and orientation defined by t
 */
inline void TransformAABB(AABB& aabb, const Transform& t)
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
	aabb.PMinT = newPMin;
	aabb.PMaxT = newPMax;
}