#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include "Math/Vec3d.h"

struct Triangle
{
	Vec3d P[3];
};

struct Mesh
{
	bool collided = false;
	std::vector<Triangle> Triangles; 
};



inline void LoadTransformMesh(Mesh& mesh)
{
	
}

/**
 * \brief Load wavefront obj file
 *        to export files that work in blender export go
 *		  under export > wavefront obj > geometry
 *		  check triangulate faces, keep vertex order
 * \param fileName File name to be imported in game
 * \param mesh Mesh to load data into
 */
inline void LoadFromObjectFile(const std::string& fileName, Mesh& mesh)
{
	std::ifstream f(fileName);
	if (!f.is_open()) {
		std::cout << "Warning file not found" << std::endl;
		return;
	}
	std::vector<Vec3d> vertexs;

	while (!f.eof())
	{
		// TODO: dangerous assumption assumes obj file is limited by 128 char per line
		// TODO: works for our purposes but if this game increase in scope this is an issue
		// TODO: that needs to be address also this loader only supports vertex and faces
		char line[128];
		f.getline(line, 128);
		std::stringstream s;
		s << line;
		char junk;
		if (line[0] == 'v')
		{
			Vec3d v;
			s >> junk >> v.X >> v.Y >> v.Z;
			vertexs.push_back(v);
		}
		else if (line[0] == 'f')
		{
			int f[3];
			s >> junk >> f[0] >> f[1] >> f[2];
			mesh.Triangles.push_back({ vertexs[f[0] - 1], vertexs[f[1] - 1], vertexs[f[2] - 1] });
		}
	}
}