# Old Code

void DrawLineProjected(Vec3d a, Vec3d b)
{
	Vec3d projectedA = cam.perspective * (cam.world2cam * a);
	Vec3d projectedB = cam.perspective * (cam.world2cam * b);
	if (projectedA.x < -aspect_ratio || projectedA.x > aspect_ratio ||
		projectedA.y < -1 || projectedA.y > 1 ||
		projectedB.x < -aspect_ratio || projectedB.x > aspect_ratio ||
		projectedB.y < -1 || projectedB.y > 1) return;

	float sx = std::min(static_cast<float>(WINDOW_WIDTH - 1), static_cast<float>((projectedA.x + aspect_ratio) * 0.5 * WINDOW_WIDTH));
	float sy = std::min(static_cast<float>(WINDOW_HEIGHT - 1), static_cast<float>((projectedA.y + 1) * 0.5 * WINDOW_HEIGHT));
	float ex = std::min(static_cast<float>(WINDOW_WIDTH - 1), static_cast<float>((projectedB.x + aspect_ratio) * 0.5 * WINDOW_WIDTH));
	float ey = std::min(static_cast<float>(WINDOW_HEIGHT - 1), static_cast<float>((projectedB.y + 1) * 0.5 * WINDOW_HEIGHT));

	App::DrawLine(sx, sy, ex, ey, 0.0, 0.0, 1.0);
}

// transform Vector3d to virtual screen space return
// false if point is clipped o/w return true
bool toScreenSpace(Vec3d a, Vec3d& coords, Camera scene_cam)
{
	Vec3d proj = scene_cam.perspective * a;
	if (proj.x < -aspect_ratio || proj.x > aspect_ratio ||
		proj.y < -1 || proj.y > 1 || proj.z < -1 || proj.z > 1) return false;

	coords.x = std::min(static_cast<double>(WINDOW_WIDTH - 1), (proj.x + aspect_ratio) * 0.5 * WINDOW_WIDTH);
	coords.y = std::min(static_cast<double>(WINDOW_HEIGHT - 1), (proj.y + 1) * 0.5 * WINDOW_HEIGHT);
	coords.z = proj.z;//-a.z;
	return true;
}

// check if coordinate is in triangle via barycentric coordinates
bool InsideTriangle(Triangle t, Vec3d p)
{
	return false;
}

// fill triangle only if its visible to user
void FillTriangle(Triangle t, Camera scene_cam, double *zbuffer)
{
	Vec3d p0 = t.p[0];
	Vec3d p1 = t.p[1];
	Vec3d p2 = t.p[2];
	// cross product
	Vec3d lineA = p0 - p1;
	Vec3d lineB = p0 - p2;
	Vec3d normal = lineB.cross(lineA);
	Vec3d lookAt = p0 - cam_pos;
	// normal facing away from cube do not render it
	if (lookAt.dot(normal) < 0) return;

	p0 = cam.world2cam * p0;
	p1 = cam.world2cam * p1;
	p2 = cam.world2cam * p2;

	Vec3d t1;
	Vec3d t2;
	Vec3d t3;
	if (!toScreenSpace(p0, t1, scene_cam) ||
		!toScreenSpace(p1, t2, scene_cam) ||
		!toScreenSpace(p2, t3, scene_cam))
		return;
	// get the bounding box of a triangle
	Vec3d BBMin = { static_cast<double>(WINDOW_WIDTH), static_cast<double>(WINDOW_HEIGHT), 0 };
	Vec3d BBMax = { 0, 0, 0 };
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
			// algorithm explained here: https://en.wikipedia.org/wiki/Barycentric_coordinate_system#Edge_approach
			double det = (t2.y - t3.y)*(t1.x - t3.x) + (t3.x - t2.x)*(t1.y - t3.y);
			if (abs(det) <= std::numeric_limits<double>::epsilon()) continue;
			double u = ((t2.y - t3.y)*(x - t3.x) + (t3.x - t2.x)*(y - t3.y)) / det;
			double v = ((t3.y - t1.y)*(x - t3.x) + (t1.x - t3.x)*(y - t3.y)) / det;
			double w = 1 - u - v;
			if (u < 0 || v < 0 || u + v > 1) continue; 
			double zindex = w * t1.z + v * t2.z + u * t3.z;
			if (zindex < zbuffer[x + y * WINDOW_WIDTH])
			{
				zbuffer[x + y * WINDOW_WIDTH] = zindex;
				points.push_back({ static_cast<float>(x), static_cast<float>(y) });
				App::DrawPoint(static_cast<float>(x), static_cast<float>(y), u, v, w);
			}
		}
	}
	//App::DrawPoints(points, 1, 0, 0);
}
