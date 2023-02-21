#include "stdafx.h"
#define NOMINMAX

#include "Renderer.h"

#include "app/app.h"
#include "Components/Display.h"
#include "Components/Mesh.h"
#include "App/main.h"


/**
 * \brief Takes vector a and outputs coords in screen space
 * \param a 
 * \param coords 
 * \param scene_cam 
 * \param display 
 * \return bool
 */
bool toScreenSpace(Vec3d a, Vec3d& coords, Camera& scene_cam, Transform& cam_transform, Display& display)
{
	Vec3d proj = scene_cam.perspective * cam_transform.world2local * a;

	if (proj.x < -display.aspect_ratio|| proj.x > display.aspect_ratio ||
		proj.y < -1 || proj.y > 1 || proj.z < -1 || proj.z > 1)
		return false;

	coords.x = std::min(static_cast<double>(WINDOW_WIDTH - 1), (proj.x + display.aspect_ratio) * 0.5 * WINDOW_WIDTH);
	coords.y = std::min(static_cast<double>(WINDOW_HEIGHT - 1), (proj.y + 1) * 0.5 * WINDOW_HEIGHT);
	coords.z = proj.z;

	return true;
}



void Renderer::Render()
{
	// TODO: add transform to Mesh. So we can move meshes
	// clear zbuffer
	std::fill_n(display->zbuffer, display->resolution, std::numeric_limits<float>::infinity());
	// App::DrawLine(0, 0, static_cast<float>(WINDOW_WIDTH - 1), static_cast<float>(WINDOW_HEIGHT - 1), 1, 0, 0);
	// loop through every mesh and fill triangles

	int componentIds[] = { GetId<Mesh>(), GetId<Transform>() };
	auto sceneView = SceneView(m_scene);
	sceneView.init(componentIds, 2);


	for (EntityID entity : sceneView)
	{
		Mesh* mesh = m_scene.Get<Mesh>(entity);
		Transform* mesh_transform = m_scene.Get<Transform>(entity);

		for (auto triangle : mesh->triangles)
		{
			// Hidden surface algorithm
			// STEP 1: check if normal is facing away from the camera

			// TODO: move this to mesh initialization so we don't have to calculate this every time
			Vec3d p0 = mesh_transform->local2world * triangle.p[0],
			      p1 = mesh_transform->local2world * triangle.p[1],
			      p2 = mesh_transform->local2world * triangle.p[2];

			// Vec3d p0 = triangle.p[0], p1 = triangle.p[1], p2 = triangle.p[2];

			Vec3d normal = (p0 - p1).cross(p0 - p2);
			Vec3d lookAt = p0 - cam_transform->position;
			if (lookAt.dot(normal) > 0) continue;
	
			Vec3d t1;
			Vec3d t2;
			Vec3d t3;
			if (!toScreenSpace(p0, t1, *cam, *cam_transform, *display) ||
				!toScreenSpace(p1, t2, *cam, *cam_transform, *display) ||
				!toScreenSpace(p2, t3, *cam, *cam_transform, *display))
				continue;
			// get the bounding box of a triangle
			Vec3d BBMin = {static_cast<double>(WINDOW_WIDTH), static_cast<double>(WINDOW_HEIGHT), 0};
			Vec3d BBMax = {0, 0, 0};
			for (auto vertex : {t1, t2, t3})
			{
				BBMin.x = std::max(0.0, std::min(BBMin.x, vertex.x));
				BBMin.y = std::max(0.0, std::min(BBMin.y, vertex.y));
				BBMax.x = std::min(static_cast<double>(WINDOW_WIDTH - 1), std::max(BBMax.x, vertex.x));
				BBMax.y = std::min(static_cast<double>(WINDOW_HEIGHT - 1), std::max(BBMax.y, vertex.y));
			}
			int minX = static_cast<int>(BBMin.x);
			int minY = static_cast<int>(BBMin.y);
			int maxX = static_cast<int>(BBMax.x);
			int maxY = static_cast<int>(BBMax.y);

			std::vector<std::array<float, 2>> points;
	
			for (int x = minX; x < maxX; x++)
			{
				for (int y = minY; y < maxY; y++)
				{
					// App::DrawPoint(static_cast<float>(x), static_cast<float>(y), 1, 0, 0);
					// std::cout << "draw point at (" << x << y << ")" << std::endl;
					// algorithm explained here: https://en.wikipedia.org/wiki/Barycentric_coordinate_system#Edge_approach
					double det = (t2.y - t3.y) * (t1.x - t3.x) + (t3.x - t2.x) * (t1.y - t3.y);
					if (abs(det) <= std::numeric_limits<double>::epsilon()) continue;
					double u = ((t2.y - t3.y) * (x - t3.x) + (t3.x - t2.x) * (y - t3.y)) / det;
					double v = ((t3.y - t1.y) * (x - t3.x) + (t1.x - t3.x) * (y - t3.y)) / det;
					double w = 1 - u - v;
					if (u < 0 || v < 0 || u + v > 1) continue;
					double zindex = w * t1.z + v * t2.z + u * t3.z;
					if (zindex < display->zbuffer[x + y * WINDOW_WIDTH])
					{
						display->zbuffer[x + y * WINDOW_WIDTH] = zindex;
						App::DrawPoint(static_cast<float>(x), static_cast<float>(y), u, v, w);
					}
				}
			}
		}
	}
}
