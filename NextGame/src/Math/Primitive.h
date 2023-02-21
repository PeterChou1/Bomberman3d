#pragma once

#include "Vec3d.h";
#include <vector>
#include <fstream>
#include <iostream>

struct Triangle
{
	Vec3d p[3];
};


struct Mesh
{
	std::vector<Triangle> triangles;

	static Mesh LoadFromObjectFile(std::string file_name)
	{
		Mesh m;
		std::ifstream f(file_name);
		if (!f.is_open()) {
			std::cout << "Warning file not found" << std::endl;
			return m;
		}
		std::vector<Vec3d> vertexs;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);
			std::stringstream s;
			s << line;
			char junk;
			if (line[0] == 'v')
			{
				Vec3d v;
				s >> junk >> v.x >> v.y >> v.z;
				vertexs.push_back(v);
			}
			else if (line[0] == 'f')
			{
				int f[3];
				s >> junk >> f[0] >> f[1] >> f[2];
				m.triangles.push_back({ vertexs[f[0] - 1], vertexs[f[1] - 1], vertexs[f[2] - 1] });
			}
		}

		return m;
	}
};