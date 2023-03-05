#include "stdafx.h"
#define NOMINMAX

#include "Renderer.h"
#include "app/app.h"
#include "Components/Display.h"
#include "Components/Mesh.h"


/**
 * \brief Takes vector a and outputs coords in screen space of sceneCam
 *        returns false if point is not on screen
 */
bool ToScreenSpace(const Vec3d a, Vec3d& coords, const Camera& sceneCam, const Transform& camTransform, const Display& display)
{
	const Vec3d proj = sceneCam.Perspective * camTransform.World2Local * a;


	coords.X = std::min(static_cast<double>(APP_VIRTUAL_WIDTH - 1), (proj.X + display.AspectRatio) * 0.5 * APP_VIRTUAL_WIDTH);
	coords.Y = std::min(static_cast<double>(APP_VIRTUAL_HEIGHT - 1), (proj.Y + 1) * 0.5 * APP_VIRTUAL_HEIGHT);
	coords.Z = proj.Z;

	if (proj.X < -display.AspectRatio || proj.X > display.AspectRatio ||
		proj.Y < -1 || proj.Y > 1 || proj.Z < -1 || proj.Z > 1)
		return false;

	return true;
}


void Renderer::Render()
{
	// TODO: add transform to Mesh. So we can move meshes
	// clear zbuffer
	std::fill_n(Display->Zbuffer, Display->Resolution, std::numeric_limits<float>::infinity());
	int componentIds[] = {GetId<Mesh>(), GetId<Transform>()};
	const auto sceneIterator = SceneIterator(SystemScene, componentIds, 2);

	for (const EntityId entity : sceneIterator)
	{
		const Mesh* mesh = SystemScene.Get<Mesh>(entity);
		const Transform* meshTransform = SystemScene.Get<Transform>(entity);
		

		for (const auto& triangle : mesh->Triangles)
		{
			// Hidden surface algorithm
			// STEP 1: check if normal is facing away from the camera
			Vec3d p0 = meshTransform->Local2World * triangle.P[0],
			      p1 = meshTransform->Local2World * triangle.P[1],
			      p2 = meshTransform->Local2World * triangle.P[2];

			Vec3d normal = Cross(p0 - p1, p0 - p2);
			Vec3d lookAt = p0 - CamTransform->Position;

			// Backface culling cull triangle looking away
			if (Dot(lookAt, normal) > 0) continue;

			// STEP 2: project screen to screen space cull the triangle if any points on triangle 
			Vec3d t1{};
			Vec3d t2{};
			Vec3d t3{};

			bool t1InScreen = ToScreenSpace(p0, t1, *Cam, *CamTransform, *Display);
			bool t2InScreen = ToScreenSpace(p1, t2, *Cam, *CamTransform, *Display);
			bool t3InScreen = ToScreenSpace(p2, t3, *Cam, *CamTransform, *Display);

			bool canCull = t1InScreen || t2InScreen || t3InScreen;
			if (canCull) continue;

			bool onScreen = t1InScreen && t2InScreen && t3InScreen;
			int minX;
			int minY;
			int maxX;
			int maxY;

			if (onScreen)
			{
				// Minor optimization only draw around the bounding box of the triangle
				Vec3d bbMin = { static_cast<double>(APP_VIRTUAL_WIDTH), static_cast<double>(APP_VIRTUAL_HEIGHT), 0 };
				Vec3d bbMax = { 0, 0, 0 };
				for (const auto& vertex : { t1, t2, t3 })
				{
					bbMin.X = std::max(0.0, std::min(bbMin.X, vertex.X));
					bbMin.Y = std::max(0.0, std::min(bbMin.Y, vertex.Y));
					bbMax.X = std::min(static_cast<double>(APP_VIRTUAL_WIDTH - 1), std::max(bbMax.X, vertex.X));
					bbMax.Y = std::min(static_cast<double>(APP_VIRTUAL_HEIGHT - 1), std::max(bbMax.Y, vertex.Y));
				}
				minX = static_cast<int>(bbMin.X);
				minY = static_cast<int>(bbMin.Y);
				maxX = static_cast<int>(bbMax.X);
				maxY = static_cast<int>(bbMax.Y);
			}
			else
			{
				minX
			}



			std::vector<std::array<float, 2>> points;

			for (int x = minX; x < maxX; x++)
			{
				for (int y = minY; y < maxY; y++)
				{
					// STEP 3: draw the points if the zindex is greater than the less than the zbuffer
					//         (ie the points are closer)
					// algorithm explained here: https://en.wikipedia.org/wiki/Barycentric_coordinate_system#Edge_approach
					const double det = (t2.Y - t3.Y) * (t1.X - t3.X) + (t3.X - t2.X) * (t1.Y - t3.Y);
					if (abs(det) <= std::numeric_limits<double>::epsilon()) continue;
					const auto u = static_cast<float>(((t2.Y - t3.Y) * (x - t3.X) + (t3.X - t2.X) * (y - t3.Y)) / det);
					const auto v = static_cast<float>(((t3.Y - t1.Y) * (x - t3.X) + (t1.X - t3.X) * (y - t3.Y)) / det);
					const float w = 1 - u - v;
					if (u < 0 || v < 0 || u + v > 1) continue;
					const double zindex = u * t1.Z + v * t2.Z + w * t3.Z;
					if (zindex < Display->Zbuffer[x + y * APP_VIRTUAL_WIDTH])
					{
						Display->Zbuffer[x + y * APP_VIRTUAL_WIDTH] = zindex;
						//App::DrawPoint(static_cast<float>(x), static_cast<float>(y), 1, 0, 0);
						App::DrawPoint(static_cast<float>(x), static_cast<float>(y), u, v, w);
					}
				}
			}
		}
	}
}
