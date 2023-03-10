#include "stdafx.h"
#define NOMINMAX

#include "Renderer.h"

#include <cassert>
#include <list>

#include "app/app.h"
#include "Components/Display.h"
#include "Components/Mesh.h"


/**
 * \brief Takes vector a and outputs coords in screen space of sceneCam
 *        returns false if point is not on screen
 */
inline void ToScreenSpace(const Vec3d a, Vec3d& coords, const Camera& sceneCam, const Transform& camTransform,
                          const Display& display)
{
	Vec3d proj = sceneCam.Perspective * camTransform.World2Local * a;
	coords.X = (proj.X + display.AspectRatio) * 0.5 * APP_VIRTUAL_WIDTH;
	coords.Y = (proj.Y + 1) * 0.5 * APP_VIRTUAL_HEIGHT;
	coords.Z = proj.Z;
}

inline bool IsOnScreen(Vec3d& proj, const Display& display)
{
	if (proj.X < -0.1 || proj.X > APP_VIRTUAL_WIDTH ||
		proj.Y < -0.1 || proj.Y > APP_VIRTUAL_HEIGHT || proj.Z < -1 || proj.Z > 1)
		return false;
}

Vec3d IntersectPlane(const Vec3d& point, Vec3d& normal, const Vec3d& lineStart, const Vec3d& lineEnd)
{
	Normalize(normal);
	const double planeDot = -Dot(normal, point);
	const double ad = Dot(lineStart, normal);
	const double bd = Dot(lineEnd, normal);
	const double t = (-planeDot - ad) / (bd - ad);
	const Vec3d start2End = t * (lineEnd - lineStart);
	return lineStart + start2End;
}


double Dist(const Vec3d& point, Vec3d planeN, Vec3d planeP)
{
	// Compute the vector from the plane point to the point
	const Vec3d planeToPoint = point - planeP;

	// Compute the dot product of the vector with the plane normal
	const double dotProduct = Dot(planeToPoint, planeN);

	// Compute the magnitude of the plane normal
	const double normal_mag = Mag(planeN);

	// Compute the distance as the absolute value of the dot product divided by the normal magnitude
	return dotProduct / normal_mag;
}

int TriangleClipAgainstPlane(const Vec3d planeP, Vec3d planeN, Triangle& inTri, Triangle& outTri1, Triangle& outTri2)
{
	Normalize(planeN);
	Vec3d* insidePoints[3];
	int nInsidePointCount = 0;
	Vec3d* outsidePoints[3];
	int nOutsidePointCount = 0;
	float d0 = Dist(inTri.P[0], planeN, planeP);
	float d1 = Dist(inTri.P[1], planeN, planeP);
	float d2 = Dist(inTri.P[2], planeN, planeP);
	if (d0 >= 0)
	{
		insidePoints[nInsidePointCount++] = &inTri.P[0];
	}
	else
	{
		outsidePoints[nOutsidePointCount++] = &inTri.P[0];
	}
	if (d1 >= 0)
	{
		insidePoints[nInsidePointCount++] = &inTri.P[1];
	}
	else
	{
		outsidePoints[nOutsidePointCount++] = &inTri.P[1];
	}
	if (d2 >= 0)
	{
		insidePoints[nInsidePointCount++] = &inTri.P[2];
	}
	else
	{
		outsidePoints[nOutsidePointCount++] = &inTri.P[2];
	}

	if (nInsidePointCount == 0)
	{
		return 0;
	}

	if (nInsidePointCount == 3)
	{
		outTri1 = inTri;
		return 1;
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2)
	{
		outTri1.P[0] = *insidePoints[0];
		outTri1.P[1] = IntersectPlane(planeP, planeN, *insidePoints[0], *outsidePoints[0]);
		outTri1.P[2] = IntersectPlane(planeP, planeN, *insidePoints[0], *outsidePoints[1]);
		return 1;
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1)
	{
		outTri1.P[0] = *insidePoints[0];
		outTri1.P[1] = *insidePoints[1];
		outTri1.P[2] = IntersectPlane(planeP, planeN, *insidePoints[0], *outsidePoints[0]);
		outTri2.P[0] = *insidePoints[1];
		outTri2.P[1] = outTri1.P[2];
		outTri2.P[2] = IntersectPlane(planeP, planeN, *insidePoints[1], *outsidePoints[0]);
		return 2;
	}
}


void Renderer::Render()
{
	// clear zbuffer
	std::fill_n(Display->Zbuffer, Display->Resolution, std::numeric_limits<float>::infinity());
	int componentIds[] = {GetId<Mesh>(), GetId<Transform>()};
	const auto sceneIterator = SceneIterator(SystemScene, componentIds, 2);
	for (const EntityId entity : sceneIterator)
	{
		std::vector<Triangle> screenSpaceTriangle;
		const Mesh* mesh = SystemScene.Get<Mesh>(entity);
		const Transform* meshTransform = SystemScene.Get<Transform>(entity);
		Vec3d CamPosition = CamTransform->Position;
		// Rendering Algorithm
		// STEP 1: project triangle against screen space
		for (auto& triangle : mesh->Triangles)
		{
			Vec3d p0 = triangle.P[0], p1 = triangle.P[1], p2 = triangle.P[2];
			p0 = meshTransform->Local2World * p0;
			p1 = meshTransform->Local2World * p1;
			p2 = meshTransform->Local2World * p2;

			Vec3d normal = Cross(p0 - p1, p0 - p2);
			Vec3d lookAt = p0 - CamPosition;
			// Back face culling cull triangle looking away
			if (Dot(lookAt, normal) > 0) continue;
			Triangle triIn;
			triIn.P[0] = p0;
			triIn.P[1] = p1;
			triIn.P[2] = p2;
			ToScreenSpace(p0, triIn.P[0], *Cam, *CamTransform, *Display);
			ToScreenSpace(p1, triIn.P[1], *Cam, *CamTransform, *Display);
			ToScreenSpace(p2, triIn.P[2], *Cam, *CamTransform, *Display);
			screenSpaceTriangle.push_back(triIn);
		}
		for (auto& triangle : screenSpaceTriangle)
		{
			// Rendering Algorithm
			// STEP 2: Clip Triangle Against the all four Camera Plane
			Triangle clipped[2];
			std::list<Triangle> listTriangles;
			// Add initial triangle
			listTriangles.push_back(triangle);
			int nNewTriangles = 1;
			for (int p = 0; p < 4; p++)
			{
				int nTrisToAdd = 0;
				while (nNewTriangles > 0)
				{
					// Take triangle from front of queue
					Triangle test = listTriangles.front();
					listTriangles.pop_front();
					nNewTriangles--;
					switch (p)
					{
					case 0: nTrisToAdd = TriangleClipAgainstPlane({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, test,
					                                              clipped[0], clipped[1]);
						break;
					case 1: nTrisToAdd = TriangleClipAgainstPlane({0.0, APP_VIRTUAL_HEIGHT - 1, 0.0},
					                                              {0.0, -1.0, 0.0}, test, clipped[0], clipped[1]);
						break;
					case 2: nTrisToAdd = TriangleClipAgainstPlane({0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, test,
					                                              clipped[0], clipped[1]);
						break;
					case 3: nTrisToAdd = TriangleClipAgainstPlane({APP_VIRTUAL_WIDTH - 1, 0.0, 0.0},
					                                              {-1.0, 0.0, 0.0}, test, clipped[0], clipped[1]);
						break;
					}
					for (int w = 0; w < nTrisToAdd; w++)
						listTriangles.push_back(clipped[w]);
				}
				nNewTriangles = listTriangles.size();
			}

			for (auto triangle : listTriangles)
			{
				Vec3d t1 = triangle.P[0];
				Vec3d t2 = triangle.P[1];
				Vec3d t3 = triangle.P[2];
				if (!IsOnScreen(t1, *Display) || !IsOnScreen(t2, *Display) || !IsOnScreen(t3, *Display)) continue;

				// Minor optimization only draw against
				Vec3d bbMin = {std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), 0};
				Vec3d bbMax = {-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), 0};
				for (const auto& vertex : {t1, t2, t3})
				{
					bbMin.X = std::max(0.0, std::min(bbMin.X, vertex.X));
					bbMin.Y = std::max(0.0, std::min(bbMin.Y, vertex.Y));
					bbMax.X = std::min(static_cast<double>(APP_VIRTUAL_WIDTH - 1), std::max(bbMax.X, vertex.X));
					bbMax.Y = std::min(static_cast<double>(APP_VIRTUAL_HEIGHT - 1), std::max(bbMax.Y, vertex.Y));
				}
				const int minX = static_cast<int>(bbMin.X);
				const int minY = static_cast<int>(bbMin.Y);
				const int maxX = static_cast<int>(bbMax.X);
				const int maxY = static_cast<int>(bbMax.Y);

				// TODO: hacky solution for clipping issue not sure what's wrong but this works (figure out later)
				if (fabs(bbMax.X - bbMin.X) > (APP_VIRTUAL_WIDTH / 5)) continue;
				if (fabs(bbMax.Y - bbMin.Y) > (APP_VIRTUAL_HEIGHT / 5)) continue;

				for (int x = minX; x < maxX; x++)
				{
					for (int y = minY; y < maxY; y++)
					{
						// STEP 3: draw the points if the zindex is greater than the less than the zbuffer
						//         (ie the points are closer)
						// algorithm explained here: https://en.wikipedia.org/wiki/Barycentric_coordinate_system#Edge_approach
						const double det = (t2.Y - t3.Y) * (t1.X - t3.X) + (t3.X - t2.X) * (t1.Y - t3.Y);
						if (abs(det) <= std::numeric_limits<double>::epsilon()) continue;
						const auto u = static_cast<float>(((t2.Y - t3.Y) * (x - t3.X) + (t3.X - t2.X) * (y - t3.Y)) /
							det);
						const auto v = static_cast<float>(((t3.Y - t1.Y) * (x - t3.X) + (t1.X - t3.X) * (y - t3.Y)) /
							det);
						const float w = 1 - u - v;
						if (u < 0 || v < 0 || u + v > 1) continue;
						const double zindex = u * t1.Z + v * t2.Z + w * t3.Z;
						if (zindex < Display->Zbuffer[x + y * APP_VIRTUAL_WIDTH])
						{
							Display->Zbuffer[x + y * APP_VIRTUAL_WIDTH] = zindex;
							float finalX = static_cast<float>(x);
							float finalY = static_cast<float>(y);
							if (u < 0.01 || v < 0.01 || w < 0.01)
							{
								App::DrawLine(finalX, finalY, finalX + 1, finalY + 1, 0, 0, 0);
							}
							else if (mesh->color)
							{
								App::DrawLine(finalX, finalY, finalX + 1, finalY + 1, mesh->r, mesh->g, mesh->b);
							}
							else
							{
								App::DrawLine(finalX, finalY, finalX + 1, finalY + 1, u, v, w);
							}
						}
					}
				}
			}
		}
	}
}
